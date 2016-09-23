/*
 * motorcontrol.c - Control the Cortex FET switches for 2-wire motor ports 1 and 10, and
 * low-level routines for the eight supervisor-controlled motor ports 2-9
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
#include <supervisor.h>
#include <periph.h>

// Motor flags for needing update
#define MOTOR_FLAG_1 1
#define MOTOR_FLAG_10 2

// Motor #1 and #10 control values
static volatile uint8_t motor1, motor10;
// Motor flag bits for consolidating the motor application calls
static volatile uint16_t motorFlags;

// _motorApply1 - Set PWM value for motor port #1
static void _motorApply1() {
	uint8_t value = motor1;
	// PWM values 126 and 128 do not give any meaningful motor impulse
	if (value > 128) {
		// Forward
		TIM4->CCR1 = (uint16_t)0;
		ioSetOutput(GPIOD, 4, 0);
		ioSetOutput(GPIOD, 3, 1);
		TIM4->CCR2 = (uint16_t)(value - 128);
	} else if (value <= 126) {
		// Reverse
		TIM4->CCR2 = (uint16_t)0;
		ioSetOutput(GPIOD, 3, 0);
		ioSetOutput(GPIOD, 4, 1);
		TIM4->CCR1 = (uint16_t)(127 - value);
	} else {
		// Stop - clear GPIOD 3 and 4
		TIM4->CCR1 = (uint16_t)0;
		TIM4->CCR2 = (uint16_t)0;
		GPIOD->BRR = 0x0018;
	}
	TIM4->CCMR1 = (uint16_t)0x6868;
}

// _motorApply10 - Set PWM value for motor port #10
static void _motorApply10() {
	uint8_t value = motor10;
	// PWM values 126 and 128 do not give any meaningful motor impulse
	if (value > 128) {
		// Forward
		TIM4->CCR3 = (uint16_t)0;
		ioSetOutput(GPIOD, 8, 0);
		ioSetOutput(GPIOD, 7, 1);
		TIM4->CCR4 = (uint16_t)(value - 128);
	} else if (value <= 126) {
		// Reverse
		TIM4->CCR4 = (uint16_t)0;
		ioSetOutput(GPIOD, 7, 0);
		ioSetOutput(GPIOD, 8, 1);
		TIM4->CCR3 = (uint16_t)(127 - value);
	} else {
		// Stop - clear GPIOD 7 and 8
		TIM4->CCR3 = (uint16_t)0;
		TIM4->CCR4 = (uint16_t)0;
		GPIOD->BRR = 0x0180;
	}
	TIM4->CCMR2 = (uint16_t)0x6868;
}

// _motorApply - Apply PWM values for motors as needed
void _motorApply() {
	uint16_t flags = motorFlags;
	if (flags & MOTOR_FLAG_1)
		// If motor 1 needs applying, apply it
		_motorApply1();
	if (flags & MOTOR_FLAG_10)
		// If motor 10 needs applying, apply it
		_motorApply10();
	// Reset all flags
	motorFlags = 0;
}

// motorControlGet - Gets the last sent PWM value of a channel 1..10 from 0 to 255
uint8_t motorControlGet(uint32_t channel) {
	// Map the channel from 1..10 to 0..9
	channel--;
	if (channel == 0)
		return motor1;
	else if (channel == 9)
		return motor10;
	else if (channel < 9)
		return svGetOutputData(channel - 1);
	return 0;
}

// Set the PWM value of channel 1 from 0 to 255
static INLINE void motorControlSet1(uint8_t value) {
	_enterCritical();
	{
		uint8_t oldMotor = motor1;
		uint16_t flag = motorFlags & MOTOR_FLAG_1;
		motor1 = value;
		if (!flag) {
			if ((value > 127 && oldMotor < 127) || (value < 127 && oldMotor > 127)) {
				TIM4->CCR1 = (uint16_t)0;
				TIM4->CCR2 = (uint16_t)0;
				GPIOD->BRR = 0x0018;
				TIM4->CCMR1 = (uint16_t)0x4848;
				_highResSchedule(1, 360);
				motorFlags |= MOTOR_FLAG_1;
			} else
				// Avoid rapid flip-flops from racing to _motorApply1()
				_motorApply1();
		}
	}
	_exitCritical();
}

// Set the PWM value of channel 10 from 0 to 255
static INLINE void motorControlSet10(uint8_t value) {
	_enterCritical();
	{
		uint8_t oldMotor = motor10;
		uint16_t flag = motorFlags & MOTOR_FLAG_10;
		motor10 = value;
		if (!flag) {
			if ((value > 127 && oldMotor < 127) || (value < 127 && oldMotor > 127)) {
				TIM4->CCR3 = (uint16_t)0;
				TIM4->CCR4 = (uint16_t)0;
				GPIOD->BRR = 0x0180;
				TIM4->CCMR2 = (uint16_t)0x4848;
				_highResSchedule(1, 360);
				motorFlags |= MOTOR_FLAG_10;
			} else
				// Avoid rapid flip-flops from racing to _motorApply10()
				_motorApply10();
		}
	}
	_exitCritical();
}

// motorControlSet - Sets the PWM value of a channel 1..10 from 0 to 255
void motorControlSet(uint32_t channel, uint8_t value) {
	// Map the channel from 1..10 to 0..9
	channel--;
	if (channel == 0)
		motorControlSet1(value);
	else if (channel == 9)
		motorControlSet10(value);
	else if (channel < 9)
		svSetData(channel - 1, value);
}

// motorControlStop - Stops all motors
void motorControlStop() {
	_enterCritical();
	{
		// Clear the following pins: PD3, PD4, PD7, PD8
		GPIOD->BRR = 0x0198;
		// Reset TIM4's CCRs to zero
		TIM4->CCR1 = (uint16_t)0;
		TIM4->CCR2 = (uint16_t)0;
		TIM4->CCR3 = (uint16_t)0;
		TIM4->CCR4 = (uint16_t)0;
		// Reset motor values
		motor1 = motor10 = 127;
		motorFlags = 0;
		// Load neutral into all supervisor data values
		svSetAllData(127);
	}
	_exitCritical();
}
