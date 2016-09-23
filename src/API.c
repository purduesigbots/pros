/*
 * API.c - Source code for user-level API functions for PROS
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

#include <kernel.h>
#include <periph.h>
#include <supervisor.h>
#include <task.h>

// Structure for helping with taskRunLoop()
typedef struct {
	void (*fn)(void);
	clock_t period;
} LoopTask;

// Helper task for taskRunLoop()
static void _taskRunLoopHelper(void *data) {
	// Extract information from pointer
	LoopTask *info = (LoopTask*)data;
	void (*fn)(void) = info->fn;
	clock_t period = info->period;
	// Avoid leaking data
	free(data);
	// Start counter
	clock_t now = timeLowRes();
	uint16_t sf = svFlags & (SV_AUTONOMOUS | SV_ENABLED | SV_FMS);
	do {
		fn();
		taskDelayUntil(&now, period);
		// Auto-terminate on mode switch, drop, or disable
	} while ((svFlags & (SV_AUTONOMOUS | SV_ENABLED | SV_FMS)) == sf);
}

// analogCalibrate - Calibrates the analog sensor on the specified channel, assuming that it is
// not actively changing. Approximately 512 samples are taken, 1 ms apart, for a 0.5 s period
// of calibration. The average value thus calculated is returned and stored for later calls
// to analogReadCalibrated()
int analogCalibrate(unsigned char channel) {
	channel--;
	if (channel < BOARD_NR_ADC_PINS) {
		// We have the true channel, do this in a loop
		uint32_t total = 0, i;
		for (i = 0; i < 512; i++) {
			total += adcRead(channel);
			taskDelay(1);
		}
		// Store value (fixed to correctly round to nearest to avoid positive bias)
		_analogState[channel].calibValue = (total + 16) >> 5;
		return (int)((total + 256) >> 9);
	}
	return 0;
}

// analogRead - Reads an analog input channel 1-8 and returns the 12-bit value from 0 to 4095
int analogRead(unsigned char channel) {
	channel--;
	if (channel < BOARD_NR_ADC_PINS)
		return (int)adcRead(channel);
	return 0;
}

// analogReadCalibrated - Reads the calibrated value of an analog input channel 1-8;
// analogCalibrate() must be run first. This is inappropriate for integrated sensors as the
// round-off error can accumulate. Use analogReadCalibratedHR() instead.
int analogReadCalibrated(unsigned char channel) {
	channel--;
	if (channel < BOARD_NR_ADC_PINS)
		return (int)adcRead(channel) - (int)(_analogState[channel].calibValue >> 4);
	return 0;
}

// analogReadCalibratedHR - Reads the calibrated value of an analog input channel 1-8;
// analogCalibrate() must be run first. This is intended for integrated sensors to reduce
// drift due to round-off.
int analogReadCalibratedHR(unsigned char channel) {
	// The value returned actually has 16 bits of "precision", even though the ADC only reads
	// 12 bits, so that errors induced by the average value being between two values come out
	// in the wash when integrated over time. Think of the value as the true value << 4.
	channel--;
	if (channel < BOARD_NR_ADC_PINS)
		return (int)(adcRead(channel) << 4) - (int)(_analogState[channel].calibValue);
	return 0;
}

// delay - taskDelay alias
void delay(const unsigned long time) {
	taskDelay((clock_t)time);
}

// delayMicroseconds - Wait for approximately the given number of microseconds
void delayMicroseconds(const unsigned long time) {
	clock_t us = (clock_t)time;
	if (us) {
		// Excellent for 1-5 us, can murder CPU usage if in high-priority task
		/*us *= 12; us--;
		asm volatile("   mov r0, %[us]\n\t"
			"1: subs r0, #1\n\t"
			"   bhi 1b\n\t"
			: : [us] "r" (us) : "r0");*/
		if (us > 1000) {
			// Wait off the milliseconds part first
			clock_t millis = (clock_t)(us / 1000UL);
			us %= 1000UL;
			taskDelay(millis);
		}
		if (us) {
			// Spin on the highres timer value (next IRQ might be up to 1ms away!)
			// Power consumption is bad, CPU usage possibly worse, but accuracy is acceptable
			uint16_t start = TIM8->CNT;
			while (TIM8->CNT - start < us);
		}
	}
}

// digitalRead - Gets the digital value (1 or 0) of a pin configured as a digital input
bool digitalRead(unsigned char pin) {
	if (pin >= BOARD_NR_GPIO_PINS)
		return 0;
	return ioGetInput((GPIO_TypeDef*)_pinLookupTable[pin], (uint8_t)_pinIndexTable[pin]);
}

// digitalWrite - Sets the digital value (1 or 0) of a pin configured as a digital output
void digitalWrite(unsigned char pin, bool value) {
	// We will let this work when disabled, even though it might change a solenoid, since it
	// is needed to set initial solenoid states in initializeIO() when enabled/disabled is
	// undefined.
	if (pin < BOARD_NR_GPIO_PINS)
		ioSetOutput((GPIO_TypeDef*)_pinLookupTable[pin], (uint8_t)_pinIndexTable[pin], value);
}

// isAutonomous - Checks to see if the system is in autonomous mode
bool isAutonomous() {
	return (svFlags & SV_AUTONOMOUS) ? true : false;
}

// isEnabled - Checks to see if the system is enabled
bool isEnabled() {
	uint16_t flags = svFlags;
	if (!(flags & SV_ENABLED)) {
		if (flags & SV_FMS)
			return false;
	}
	return true;
}

// isJoystickConnected - Checks to see if the specified joystick is plugged in
bool isJoystickConnected(unsigned char joystick) {
	return svIsJoystickConnected(joystick) ? true : false;
}

// isOnline - Checks to see if the system is connected to a FMS/competition switch
bool isOnline() {
	return (svFlags & SV_FMS) ? true : false;
}

// joystickGetAnalog - Retrieve an analog axis of a joystick
int joystickGetAnalog(unsigned char joystick, unsigned char axis) {
	// Safety first here
	if (isOnline() && isAutonomous()) return 0;
	return (int)svGetJoystickAnalog(joystick, axis);
}

// joystickGetDigital - Retrieve a digital button of a joystick
bool joystickGetDigital(unsigned char joystick, unsigned char buttonGroup,
		unsigned char button) {
	// Safety first here
	if (isOnline() && isAutonomous()) return false;
	return (svGetJoystickDigital(joystick, buttonGroup) & button) ? true : false;
}

// micros - Arduino-compatible alias for timeHighRes()
unsigned long micros() {
	return (unsigned long)timeHighRes();
}

// millis - Arduino-compatible alias for timeLowRes()
unsigned long millis() {
	return (unsigned long)timeLowRes();
}

// motorGet - Gets last commanded speed of the specified motor channel; this speed is reset
// to 0 if motors are stopped by the kernel for any reason
int motorGet(unsigned char channel) {
	// Convert range
	return (int)motorControlGet(channel) - 127;
}

// motorSet - Sets the speed of the specified motor channel from 1 to 10, where -127 is full
// reverse and 127 is full forward, with 0 being off
void motorSet(unsigned char channel, int speed) {
	if (isEnabled()) {
		int ns = speed + 127;
		// Equalize the bias
		if (ns < 0) ns = 0;
		if (ns > 255) ns = 255;
		motorControlSet(channel, (uint8_t)ns);
	}
}

// motorStop - Stops the motor on the specified channel, equivalent to calling motorSet() with
// an argument of zero; this performs a coasting stop, not an active brake
void motorStop(unsigned char channel) {
	motorControlSet(channel, 127);
}

// motorStopAll - Stops all motors; significantly faster than looping through all motor ports
// and calling motorSet(channel, 0) on each one
void motorStopAll() {
	// It is safe to stop all motors when disabled, as that is their disabled state anyways.
	motorControlStop();
}

// pinMode - Configures the pin as an input or output with a variety of settings
void pinMode(unsigned char pin, unsigned char mode) {
	if (pin < BOARD_NR_GPIO_PINS)
		ioSetDirection((GPIO_TypeDef*)_pinLookupTable[pin], (uint8_t)_pinIndexTable[pin], mode);
}

// powerLevelBackup - Get backup battery voltage in millivolts, or 0 if not connected
unsigned int powerLevelBackup() {
	return svGetBackupBattery();
}

// powerLevelMain - Get main battery voltage in millivolts, or 0 if not connected
unsigned int powerLevelMain() {
	return svGetMainBattery();
}

// setTeamName - Sets the team name of the Cortex
void setTeamName(const char *name) {
	svSetTeamName(name);
}

// taskRunLoop - Starts a task at higher-than-normal priority to run the specified function
// in intervals as close to the specified period in milliseconds as possible
// For a stack size or priority different than the default, launch a custom task
TaskHandle taskRunLoop(void (*fn)(void), const unsigned long increment) {
	// Initialize data (will be later freed)
	LoopTask *lt = malloc(sizeof(LoopTask));
	lt->fn = fn;
	lt->period = (clock_t)increment;
	// Create and start the task
	return taskCreate(_taskRunLoopHelper, TASK_DEFAULT_STACK_SIZE, (void*)lt,
		TASK_PRIORITY_DEFAULT + 1);
}

// wait - taskDelay alias
void wait(const unsigned long time) {
	taskDelay((clock_t)time);
}

// waitUntil - taskDelayUntil alias
void waitUntil(unsigned long *previousWakeTime, unsigned long time) {
	taskDelayUntil((clock_t*)previousWakeTime, (clock_t)time);
}
