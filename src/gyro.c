/*
 * PROS Analog Gyro Library for VEX Yaw-Rate 1000dps LY3100ALH gyro
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

#include "periph.h"
#include "task.h"

// Default gyro multiplier
// Calculation: 1.1 mV/dps = 1.365 quid/dps = 0.0007326007... dpms/quid ~= 196>>18
#define GYRO_MULTIPLIER_DEFAULT 196
// Integration interval in milliseconds
#define DT 2
// Rate noise elimination threshold
#define RATE_NOISE_LIMIT 4

// Sensor status storage
Analog_TypeDef _analogState[BOARD_NR_ADC_PINS];
static volatile unsigned int gyroCount = 0;

// Gets the current gyro angle in degrees
double gyroGet(Gyro g) {
	Analog_TypeDef *gyro = (Analog_TypeDef*)g;
	if (gyro)
		return (double) gyro->value / 256;
	return 0;
}

// Gyro integration routine
static INLINE void gyroIntegrate(uint32_t port) {
	Analog_TypeDef *gyro = &_analogState[port];
	// LSLed by 4 as offset
	int32_t reading = (int32_t)analogReadCalibratedHR(port + 1);
	// Multiplier is (0.0007...<<18) dpms * DT ms * (reading<<4) quid = degrees<<22
	// So we need to get from LSL22 to LSL8 = LSR14
	int32_t d = ((int32_t)(gyro->lastValue) * DT * reading + 0x2000) >> 14;
	if (d < -RATE_NOISE_LIMIT || d > RATE_NOISE_LIMIT)
		gyro->value += d;
}

// Gyro integration task which integrates all gyros on robot
static void gyroIntegrateTask(void *ignore) {
	clock_t now = timeLowRes();
	while (gyroCount > 0) {
		for (uint32_t i = 0; i < BOARD_NR_ADC_PINS; i++)
			// If active, integrate it
			if (_analogState[i].flags & (uint8_t)0x02)
				gyroIntegrate(i);
		taskDelayUntil(&now, DT);
	}
}

// Initialize the gyro - call in initialize()
Gyro gyroInit(unsigned char port, unsigned short multiplier) {
	// Initialize gyro
	if (multiplier == 0) multiplier = GYRO_MULTIPLIER_DEFAULT;
	port--;
	if (port < 8) {
		unsigned int gc = gyroCount;
		// Calibrate the port using standard PROS API
		analogCalibrate(port + 1);
		// Check to ensure that the gyro is properly plugged in
		/*if (_analogState[port].calibValue > 512) {*/
			__disable_irq();
			// Mark in use, non reversed (gyros cannot be reversed)
			_analogState[port].flags = (uint8_t)0x02;
			_analogState[port].value = 0;
			// Store multiplier (can be independent per gyro to account for differences)
			_analogState[port].lastValue = multiplier;
			gyroCount = gc + 1;
			__enable_irq();
			if (gc == 0)
				// Start integrator
				taskCreate(gyroIntegrateTask, TASK_MINIMAL_STACK_SIZE, NULL,
					TASK_PRIORITY_DEFAULT + 1);
			return (Gyro)(&_analogState[port]);
		/*}*/
	}
	return NULL;
}

// Resets the gyro angle to zero
void gyroReset(Gyro g) {
	Analog_TypeDef *gyro = (Analog_TypeDef*)g;
	if (gyro)
		gyro->value = 0;
}

// Stop gyro and freeze the value
void gyroShutdown(Gyro g) {
	Analog_TypeDef *gyro = (Analog_TypeDef*)g;
	if (gyro) {
		__disable_irq();
		// Mark inactive
		gyro->flags = (uint8_t)0x00;
		gyroCount--;
		__enable_irq();
	}
}
