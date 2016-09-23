/*
 * io.c - Basic I/O functions for Cortex (lowest level functions, not Wiring compatible)
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
#include <periph.h>

// ADC data storage
uint16_t adcDataIn[16];

// Analog channel lookup table
const uint8_t _adcChannelTable[BOARD_NR_ADC_PINS] = {
	// Ports go in order PA0 PA1 PA2 PA3 PC2 PC3 PC0 PC1
	0, 1, 2, 3, 12, 13, 10, 11
};

// I/O pin offset lookup table
const uint8_t _pinIndexTable[BOARD_NR_GPIO_PINS] = {
	// 12 Digital pins and the SP output
	4, 9, 11, 6, 7, 13, 14, 8, 10, 12, 7, 0, 1,
	// 8 Analog pins
	0, 1, 2, 3, 2, 3, 0, 1,
	// 6 Communications pins
	5, 6, 10, 11, 8, 9
};

// I/O pin register lookup table
const GPIO_TypeDef* const _pinLookupTable[BOARD_NR_GPIO_PINS] = {
	// 12 Digital pins and the SP output
	GPIOA, GPIOE, GPIOE, GPIOC, GPIOC, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOD, GPIOD,
	// 8 Analog pins
	GPIOA, GPIOA, GPIOA, GPIOA, GPIOC, GPIOC, GPIOC, GPIOC,
	// 6 Communications pins
	GPIOD, GPIOD, GPIOC, GPIOC, GPIOB, GPIOB
};

// _ioConfigure - Internal routine to configure the CRL/CRH registers
static void _ioConfigure(__IO uint32_t *CR, uint8_t index, uint8_t bits) {
	uint32_t value;
	// Index goes up by 4 for every pin, so multiply by 4
	index <<= 2;
	// Mask off original value
	value = *CR;
	value &= ~(((uint32_t)0xF) << index);
	// Move in new value
	value |= ((uint32_t)bits) << index;
	*CR = value;
}

// adcOff - Stops the ADC, use before reconfiguring
void adcOff() {
	_enterCritical();
	{
		ADC1->CR2 &= ~ADC_CR2_ADON;
		DMA1_Channel1->CCR &= ~DMA_CCR_EN;
	}
	_exitCritical();
}

// adcOn - Starts the ADC
void adcOn() {
	_enterCritical();
	{
		uint32_t temp = ADC1->CR2;
		if (!(temp & ADC_CR2_ADON))
			temp |= ADC_CR2_ADON;
		DMA1_Channel1->CCR |= DMA_CCR_EN;
		ADC1->CR2 = temp | ADC_CR2_SWSTART;
	}
	_exitCritical();
}

// adcRead - Reads a channel 0-15 from the ADC
uint16_t adcRead(uint32_t channel) {
	return adcDataIn[channel & (uint32_t)0xF];
}

// adcSetChannels - Sets the channels sampled by the ADC (ADC must be off!)
void adcSetChannels(uint32_t num, ...) {
	uint32_t i, offset = 0;
	va_list args;
	if (num && num <= 16) {
		_enterCritical();
		{
			// Store count
			uint32_t sqr[3], *ptr = &sqr[2];
			sqr[0] = (num - 1) << 20;
			sqr[1] = 0;
			sqr[2] = 0;
			DMA1_Channel1->CNDTR = num;
			va_start(args, num);
			for (i = 0; i < num; i++) {
				// Store in 5-bit positions
				*ptr |= ((uint32_t)(va_arg(args, int)) & 0x1F) << offset;
				offset += 5;
				if (offset >= 30) {
					// Move up a register
					ptr--;
					offset = 0;
				}
			}
			va_end(args);
			ADC1->SQR1 = sqr[0];
			ADC1->SQR2 = sqr[1];
			ADC1->SQR3 = sqr[2];
		}
		_exitCritical();
	}
}

// ioMultiSetDirection - Configures the specified pins in the bit mask to the given type
void ioMultiSetDirection(GPIO_TypeDef* port, uint16_t pinMask, uint32_t type) {
	uint32_t i, mod;
	type &= 0x0F;
	mod = type & 0x0D;
	_enterCritical();
	{
		for (i = 0; i < 8; i++) {
			// Look for pins to configure
			if (pinMask & 1) {
				_ioConfigure(&(port->CRL), i, mod);
				// Set the pullup/pulldown depending on the twos bit
				if (mod == DDR_INPUT_PULLDOWN)
					ioSetOutput(port, i, type & 0x02);
			}
			pinMask >>= 1;
		}
		for (i = 0; i < 8; i++) {
			// Look for pins to configure
			if (pinMask & 1) {
				_ioConfigure(&(port->CRH), i, mod);
				// Set the pullup/pulldown depending on the twos bit
				if (mod == DDR_INPUT_PULLDOWN)
					ioSetOutput(port, i + 8, type & 0x02);
			}
			pinMask >>= 1;
		}
	}
	_exitCritical();
}

// ioSetDirection - Configures the pin as an input or output with a variety of settings
void ioSetDirection(GPIO_TypeDef* port, uint32_t pin, uint32_t type) {
	uint32_t mod;
	_enterCritical();
	{
		// Force pin in range
		pin &= 0x0F;
		type &= 0x0F;
		mod = type & 0x0D;
		if (pin > 7)
			// High bits
			_ioConfigure(&(port->CRH), pin - 8, mod);
		else
			// Low bits
			_ioConfigure(&(port->CRL), pin, mod);
		// Set the pullup/pulldown depending on the twos bit
		if (mod == DDR_INPUT_PULLDOWN)
			ioSetOutput(port, pin, type & 0x02);
	}
	_exitCritical();
}
