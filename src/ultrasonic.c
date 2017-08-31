/*
 * ultrasonic.c - VEX Ultrasonic sensor driver
 *
 * Copyright (c) 2011-2016, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include <FreeRTOS.h>
#include <kernel.h>
#include <periph.h>

// Status flags for the flag variable
// Set if and only if ping process is running
#define ULTRA_PINGING 0x1
// Set if and only if ping process is waiting for a ping to return; false if simply waiting out
// the pulse duration
#define ULTRA_WAITING 0x2
// Ultrasonic's default value if a ping is not properly returned
#define ULTRA_BAD_RESPONSE -1

// Ultrasonic flags
typedef struct {
	// Flags for ping process
	volatile uint8_t flags;
	// Last port pinged
	volatile uint8_t lastPort;
} UltrasonicData;
static UltrasonicData ultraFlags;

// fireUltrasonic - Fires the ultrasonic sensor on the specified port
static void fireUltrasonic(uint8_t port) {
	Sensor_TypeDef *ultra = &_sensorState[port];
	// Begin pulse
	ultra->value = ULTRA_BAD_RESPONSE;
	digitalWrite((uint32_t)(ultra->portBottom) + 1, true);
	// Schedule 128 us later on high-res timer
	ultraFlags.flags &= ~ULTRA_WAITING;
	_highResSchedule(3, 128);
	// Save port
	ultraFlags.lastPort = port;
}

// Interrupt routine for the "echo" connection to an ultrasonic sensor
static void _ultraISR(unsigned char pin) {
	Sensor_TypeDef *ultra = &_sensorState[pin - 1];
	if ((ultra->flags & (uint16_t)0x0005) && (pin - 1) == ultraFlags.lastPort) {
		// We set the value = now - lastValue (= start time)
		ultra->value = timeHighRes() - ultra->lastValue;
		// Increases frame rate but causes other problems
		//fireUltrasonic(ultra->portTop);
	}
}

// ultrasonicTimeout - Either pulse has been fired, or timeout has occurred
void _ultrasonicTimeout() {
	Sensor_TypeDef *ultra = &_sensorState[ultraFlags.lastPort];
	uint8_t flags = ultraFlags.flags;
	if (flags & ULTRA_PINGING) {
		if (flags & ULTRA_WAITING)
			// Timeout or next pulse
			fireUltrasonic(ultra->portTop);
		else {
			// Lower ping pin
			digitalWrite((uint32_t)(ultra->portBottom) + 1, false);
			// Arm timer
			ultra->lastValue = timeHighRes();
			// Waiting...
			ultraFlags.flags = flags | ULTRA_WAITING;
			// Re-schedule for ~50 ms timeout
			_highResSchedule(3, 49152);
		}
	} else
		// Disable repeat interrupts
		TIM8->DIER &= ~TIM_DIER_CC4IE;
}

// ultrasonicInit - Initializes an ultrasonic sensor on the specified digital ports
// portEcho is the orange cable and portPing is the yellow cable
Ultrasonic ultrasonicInit(unsigned char portEcho, unsigned char portPing) {
	Sensor_TypeDef *ultra, *slave;
	// Change to 0..11 range
	uint32_t pe = portEcho - 1, pp = portPing - 1, i;
	// Check range
	if (pe < BOARD_NR_DIGITAL_IO && pe != 9 && _sensorState[pe].flags == 0 &&
			pp < BOARD_NR_DIGITAL_IO && _sensorState[pp].flags == 0) {
		_enterCritical();
		{
			ultra = &_sensorState[pe];
			// Set echo pin to input pull-up
			pinMode(portEcho, DDR_INPUT_PULLUP);
			// Set ping pin to output push-pull
			pinMode(portPing, DDR_OUTPUT);
			// Set state of master pin (top)
			ultra->flags = (uint16_t)0x0005;
			ultra->portBottom = (uint8_t)pp;
			ultra->value = 0;
			ultra->lastValue = 0;
			// We need to find the next sonar in line to create a link
			// Already guaranteed that _sensorState[pp].flags == 0
			for (i = portEcho; ; i++) {
				// Wrap around; can't loop forever, we will find ourselves
				if (i >= BOARD_NR_DIGITAL_IO)
					i = 0;
				if (_sensorState[i].flags == (uint16_t)0x0005) {
					// This is the next sensor's echo port
					ultra->portTop = (uint8_t)i;
					break;
				}
			}
			// Previous sonar in line
			for (i = pe - 1; ; i--) {
				// Wrap around; can't loop forever, we will find ourselves
				if (i >= BOARD_NR_DIGITAL_IO)
					// 0 - 1 = 0xFFFFFFFF (overflow)
					i = (BOARD_NR_DIGITAL_IO - 1);
				slave = &_sensorState[i];
				if (slave->flags == (uint16_t)0x0005) {
					// This is the previous sensor's echo port
					slave->portTop = (uint8_t)pe;
					break;
				}
			}
			// Set slaved state so that another sensor will not try to use it
			slave = &_sensorState[pp];
			slave->flags = (uint16_t)0x0004;
			slave->portTop = (uint8_t)pe;
			slave->portBottom = (uint8_t)pp;
			// Interrupt on echo falling
			ioSetInterrupt(portEcho, INTERRUPT_EDGE_FALLING, _ultraISR);
			ioClearInterrupt(portPing);
			// If not pinging, start it
			if (!(ultraFlags.flags & ULTRA_PINGING)) {
				ultraFlags.flags = ULTRA_PINGING;
				fireUltrasonic(pe);
			}
		}
		_exitCritical();
		return (Ultrasonic)ultra;
	}
	return NULL;
}

// ultrasonicGet - Gets the current ultrasonic sensor value in cm
int ultrasonicGet(Ultrasonic ult) {
	Sensor_TypeDef *ultra = (Sensor_TypeDef*)ult;
	int32_t value = ultra->value;
	if (value == ULTRA_BAD_RESPONSE)
		return (int)(value);
	if (value > 306 && value < 11877)
		return (int)((value - 277) / 58);
	return ULTRA_BAD_RESPONSE;
}

// ultrasonicShutdown - Stops and disables the ultrasonic sensor
void ultrasonicShutdown(Ultrasonic ult) {
	Sensor_TypeDef *ultra = (Sensor_TypeDef*)ult, *sen;
	if (ultra) {
		_enterCritical();
		{
			uint8_t pb = ultra->portBottom;
			uint32_t i, pt = (uint32_t)(_sensorState[pb].portTop);
			// Clear run flags
			ultra->flags = 0;
			_sensorState[pb].flags = 0;
			// Find our previous sensor (who links to us) and set its portTop to skip us
			for (i = 0; i < BOARD_NR_DIGITAL_IO; i++) {
				sen = &_sensorState[i];
				if (_sensorState[sen->portTop].portBottom == pb &&
						(sen->flags & (uint16_t)0x0005) != 0) {
					sen->portTop = pt;
					pb = 255;
					break;
				}
			}
			// If we were the only ultrasonic, stop pinging
			if (pb < 255)
				ultraFlags.flags = 0;
			// Clear interrupts
			ioClearInterrupt(pt + 1);
			// Return pinger to pullup input
			pinMode(pb + 1, DDR_INPUT_PULLUP);
		}
		_exitCritical();
	}
}
