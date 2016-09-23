/*
 * pwm.c - PWM driver for Digital pins supporting PWM from Timer 1 only
 * (Digital 1, 2, 5, and 6)
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

#ifdef PWM_ENABLED
// Maps Digital pins to PWM channels
static const uint8_t pwmPinToChannel[] = { 1, 2, 0, 0, 3, 4, 0, 0, 0, 0, 0, 0 };

// The largest value that the PWM can count to
#define MAX_OVERFLOW 65535

// pwmSetEnable - Enables or disables PWM on the specified pin. If PWM is enabled, the initial
// value for a channel is the last value set by pwmWrite() or 0 if never set before
void pwmSetEnable(uint32_t pin, bool enable) {
	pin--;
	if (pin < 12) {
		// Look up channel
		uint8_t channel = pwmPinToChannel[pin];
		if (channel) {
			uint16_t mask = (uint16_t)(1 << (4 * (channel - 1)));
			// Set pin to output mode
			pinMode(pin + 1, DDR_AFO);
			if (enable)
				// Enable PWM
				TIM1->CCER |= mask;
			else {
				// Disable PWM
				TIM1->CCER &= ~mask;
				// Default LOW
				digitalWrite(pin + 1, false);
			}
		}
	}
}

// pwmSetMaximum - Sets the maximum PWM value for all channels from 1 to 65535
void pwmSetMaximum(uint16_t max) {
	if (max)
		TIM1->ARR = max;
}

// pwmSetInvert - Inverts the PWM driver on the specified pin to be LOW for the "on" period and
// HIGH for the "off" period
void pwmSetInvert(uint32_t pin, bool invert) {
	pin--;
	if (pin < 12) {
		// Look up channel
		uint8_t channel = pwmPinToChannel[pin];
		if (channel) {
			uint16_t mask = (uint16_t)(2 << (4 * (channel - 1)));
			if (invert)
				// Invert enable, set active low
				TIM1->CCER |= mask;
			else
				// Invert disable, set active high
				TIM1->CCER &= ~mask;
		}
	}
}

// pwmSetPeriod - Sets the period of each PWM cycle (1 / frequency) in microseconds. This
// affects all four PWM channels equally. The maximum PWM value is configured to 65535
// Range: 900 us to 10 000 000 us (10 s)
void pwmSetPeriod(uint32_t period_us) {
	if (period_us && period_us <= 10000000) {
		uint32_t period_cyc = period_us * CYCLES_PER_MICROSECOND;
		uint16_t prescaler = (uint16_t)(period_cyc / MAX_OVERFLOW + 1);
		pwmSetScaler(prescaler);
		pwmSetMaximum(MAX_OVERFLOW);
	}
}

// pwmSetScaler - Sets the PWM clock prescaler for all channels from 1 to 65535
void pwmSetScaler(uint16_t scaler) {
	if (scaler)
		TIM1->PSC = scaler;
}

// pwmSetPeriodPrecise - Sets the period of each PWM cycle as precisely as possible in
// microseconds. This affects all four PWM channels equally. The maximum PWM value is
// determined automatically and returned as an integer from 1 to 65535
// Range: 1 us to 10 000 000 us (10 s)
uint16_t pwmSetPeriodPrecise(uint32_t period_us) {
	if (period_us && period_us <= 10000000) {
		uint32_t period_cyc = period_us * CYCLES_PER_MICROSECOND;
		uint16_t prescaler = (uint16_t)(period_cyc / MAX_OVERFLOW + 1);
		uint16_t overflow = (uint16_t)((period_cyc + (uint32_t)(prescaler >> 1)) / prescaler);
		pwmSetScaler(prescaler);
		pwmSetMaximum(overflow);
		return overflow;
	}
	return MAX_OVERFLOW;
}

// pwmWrite - Sets PWM period "on" value from 0 to configured maximum value (default 65535)
void pwmWrite(uint32_t pin, uint16_t value) {
	pin--;
	if (pin < 12) {
		// Look up channel
		uint8_t channel = pwmPinToChannel[pin];
		if (channel) {
			volatile uint16_t *cptr = &(TIM1->CCR1);
			// Set pin to output mode
			pinMode(pin + 1, DDR_AFO);
			// Enable PWM if not already enabled
			channel--;
			TIM1->CCER |= (uint16_t)(1 << (4 * channel));
			// 2 * channel due to alignment
			cptr[2 * channel] = value;
		}
	}
}
#endif
