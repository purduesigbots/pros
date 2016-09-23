/*
 * i2c.c - I2C implementation
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

#include <semphr.h>
#include <FreeRTOS.h>
#include <periph.h>

// I2C statuses
#define I2C_READ_BIT ((uint8_t)0x01)
#define I2C_STATUS_ERR ((uint8_t)0x01)
#define I2C_STATUS_RESTART ((uint8_t)0x02)
#define I2C_STATUS_NOSTOP ((uint8_t)0x04)

// I2C status to use
typedef struct {
	// Up or down?
	volatile uint8_t status;
	// Target I2C address
	volatile uint8_t address;
	// Byte count left
	volatile uint16_t count;
	// Byte buffer to use
	volatile uint8_t *buffer;
	// I2C sync semaphore
	volatile Semaphore sync;
} I2CStatus_TypeDef;

volatile I2CStatus_TypeDef i2cState;

// _i2cDisableInt - Common I2C end routine to disable interrupts
static INLINE void _i2cDisableInt() {
	I2C1->CR2 &= ~(I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN);
}

// _i2cEnableInt - Common I2C start routine to enable interrupts
static INLINE void _i2cEnableInt() {
	I2C1->CR2 |= I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN;
}

// I2C status flag initialization
void _i2cInit() {
	Semaphore sync = semaphoreCreate();
	semaphoreTake(sync, 0UL);
	i2cState.sync = sync;
	i2cState.status = 0;
}

// _i2cRead - Reads the specified number of data bytes from the specified address
static bool _i2cRead(uint8_t addr, uint8_t *data, uint16_t count) {
	volatile I2CStatus_TypeDef *state = &i2cState;
	__disable_irq();
	{
		// Set the I2C direction to reception, set LSB to receive properly
		state->address = (addr << 1) | I2C_READ_BIT;
		// Set up buffers
		state->buffer = data;
		state->count = count;
		// Send START condition
		I2C1->CR1 |= I2C_CR1_ACK | I2C_CR1_START;
		state->status = 0;
	}
	__enable_irq();
	// Wait until START is reset (transmission begins)
	do {
		_taskYield();
		if (state->status & I2C_STATUS_ERR)
			return false;
	} while (!(I2C1->SR1 & I2C_SR1_SB));
	// Enable I2C interrupts
	_i2cEnableInt();
	do {
		// Wait until BUSY flag is reset (until a STOP is generated)
		semaphoreTake(i2cState.sync, 2);
		// Error occurred?
		if (state->status & I2C_STATUS_ERR)
			return false;
	} while (i2cState.count > 0);
	return true;
}

// Writes data to the I2C interface, observing the NOSTOP flag for repeated-start generation
static bool _i2cWrite(uint8_t addr, uint8_t *data, uint16_t count) {
	uint8_t status;
	volatile I2CStatus_TypeDef *state = &i2cState;
	__disable_irq();
	{
		// Set the I2C direction to transmission, clear LSB to transmit properly
		state->address = addr << 1;
		// Set up buffers
		state->buffer = data;
		state->count = count;
		// Send START condition
		state->status &= ~(I2C_STATUS_ERR | I2C_STATUS_RESTART);
		I2C1->CR1 |= I2C_CR1_START | I2C_CR1_ACK;
	}
	__enable_irq();
	// Enable I2C interrupts
	_i2cEnableInt();
	do {
		// Wait until BUSY flag is reset (until a STOP is generated) or repeated-start bit set
		semaphoreTake(i2cState.sync, 2);
		status = state->status;
		// Error occurred?
		if (status & I2C_STATUS_ERR)
			return false;
	} while (i2cState.count > 0);
	return true;
}

// i2cRead - Reads the specified number of data bytes from the specified address
bool i2cRead(uint8_t addr, uint8_t *data, uint16_t count) {
	return _i2cRead(addr, data, count);
}

// i2cReadRegister - Reads the specified amount of data from the given register address on
// the specified I2C address
bool i2cReadRegister(uint8_t addr, uint8_t reg, uint8_t *value, uint16_t count) {
	i2cState.status |= I2C_STATUS_NOSTOP;
	// Write out the location we want and read in data
	return _i2cWrite(addr, &reg, 1) && _i2cRead(addr, value, count);
}

// i2cSetAddress - Sets the Cortex's I2C address
void i2cSetAddress(uint8_t addr) {
	uint16_t oar;
	// Limit to 7 bits
	addr &= 0xFE;
	// Put it into OAR
	oar = I2C1->OAR1;
	oar &= ~0x00FE;
	oar |= addr;
	I2C1->OAR1 = oar;
}

// i2cWrite - Writes the specified number of data bytes to the specified address
bool i2cWrite(uint8_t addr, uint8_t *data, uint16_t count) {
	i2cState.status &= ~I2C_STATUS_NOSTOP;
	return _i2cWrite(addr, data, count);
}

// i2cWriteRegister - Writes the specified data to a register on the specified I2C address
bool i2cWriteRegister(uint8_t addr, uint8_t reg, uint16_t value) {
	uint8_t data[2];
	// Write register, value
	data[0] = reg;
	data[1] = value;
	// Write information
	return i2cWrite(addr, data, 2);
}

// _i2cEnd - Helper for stop bit checking to clean up the I2C state
void _i2cEnd() {
	bool cs = false;
	// Unfortunately, no irq for "I2C bus is free" or "stop bit set"; thus we wait off the
	// stop bit using a timer since we cannot fail to send a stop bit if we are the master
	semaphoreGiveISR(i2cState.sync, &cs);
	if (cs)
		_taskYield();
}

// I2C1 event interrupt
void ISR_I2C1_EV() {
	uint16_t sr1 = I2C1->SR1;
	volatile I2CStatus_TypeDef *state = &i2cState;
	bool cs = false;
	I2C1->SR2;
	if (sr1 & I2C_SR1_SB)
		// If SB = 1, START sent (EV5)
		I2C1->DR = state->address;
	else if (sr1 & I2C_SR1_ADDR) {
		// If ADDR = 1, address sent (EV6)
		uint8_t count = state->count;
		// Write the first data when transmitting (LSB clear)
		if (!(state->address & I2C_READ_BIT)) {
			// Write the first data in the data register
			I2C1->DR = *((state->buffer)++);
			count--;
			state->count = count;
			// If no further data, disable buffer interrupt so as not to have a TXE interrupt
			if (count == 0)
				I2C1->CR2 &= ~I2C_CR2_ITBUFEN;
		} else if (count == 1) {
			// EV6_1, used for single byte reception. Disable ACK and program STOP
			uint16_t cr = I2C1->CR1;
			// No ACK, send STOP
			cr = (cr & ~I2C_CR1_ACK) | I2C_CR1_STOP;
			I2C1->CR1 = cr;
			cs = true;
		}
	} else if (sr1 & I2C_SR1_TXE) {
		uint8_t count = state->count;
		// TXE set, transmit data
		if (sr1 & I2C_SR1_BTF) {
			uint8_t status = state->status;
			// EV8_2, both BTF and TXE are set
			if (status & I2C_STATUS_NOSTOP)
				// Prepare for repeated START bit
				state->status = (status & ~I2C_STATUS_NOSTOP) | I2C_STATUS_RESTART;
			else
				// Send STOP
				I2C1->CR1 |= I2C_CR1_STOP;
			// Disable event interrupt to avoid second BTF interrupt
			_i2cDisableInt();
			cs = true;
		} else if (count > 0) {
			// Write data
			I2C1->DR = *((state->buffer)++);
			count--;
			state->count = count;
			// If no data left, avoid last TXE interrupt
			if (count == 0)
				I2C1->CR2 &= ~I2C_CR2_ITBUFEN;
		}
	} else if (sr1 & I2C_SR1_RXNE) {
		uint8_t count = state->count;
		// RXNE set, read in data
		*((state->buffer)++) = I2C1->DR;
		count--;
		state->count = count;
		// If one byte left, disable ACK and send STOP (EV7_1)
		if (count == 1)
			// No ACK, send STOP
			I2C1->CR1 = (I2C1->CR1 & ~I2C_CR1_ACK) | I2C_CR1_STOP;
		else if (count == 0) {
			// All done
			_i2cDisableInt();
			cs = true;
		}
	}
	if (cs)
		// One bit clock = 2.5us, add safety margin
		_highResSchedule(2, 4);
}

// I2C1 error interrupt
void ISR_I2C1_ER() {
	uint16_t cr, flags = I2C1->SR1;
	bool cs = false;
	// Store interrupt flags, then clear them
	I2C1->SR1 = flags & ~(I2C_SR1_ARLO | I2C_SR1_BERR | I2C_SR1_OVR | I2C_SR1_AF);
	cr = I2C1->CR1;
	// Recover from arbitration lost and bus error via resetting I2C
	if (flags & (I2C_SR1_ARLO | I2C_SR1_BERR | I2C_SR1_OVR)) {
		// Software reset the I2C peripheral
		I2C1->CR1 = cr | I2C_CR1_SWRST;
		// This delay required for I2C to acknowledge the request
		for (uint32_t delay = 0; delay < 32; delay++) asm volatile("");
		// Clear software reset
		I2C1->CR1 = cr;
	} else
		// Send STOP bit
		I2C1->CR1 = cr | I2C_CR1_STOP;
	// Let SW know to break out of wait state
	i2cState.status |= I2C_STATUS_ERR;
	// Disable future interrupts
	_i2cDisableInt();
	semaphoreGiveISR(i2cState.sync, &cs);
	if (cs)
		_taskYield();
}
