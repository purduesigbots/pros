/*
 * ime.c - VEX Integrated Motor Encoder (IME) support over I2C
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
#include <queue.h>
#include <periph.h>
#include <task.h>

// Default IME address
#define IME_DEFAULT_ADDR 0x30
// IME address can range from 0x10 to 0x2F, with all addresses valid.
#define IME_ADDR_MIN 0x10
#define IME_ADDR_MAX 0x2F
#define IME_MAX_NUM (IME_ADDR_MAX - IME_ADDR_MIN)
// General call address to address all IMEs
#define IME_ADDR_GEN_CALL 0x00

// Information registers on the IME
#define IME_REG_VERSION 0x00
#define IME_REG_VENDOR 0x08
#define IME_REG_ID 0x10
#define IME_REG_INFO 0x20

// Motor tick bytes (big endian!)
#define IME_REG_ROT_0 0x41
#define IME_REG_ROT_1 0x40
#define IME_REG_ROT_2 0x43
#define IME_REG_ROT_3 0x42
#define IME_REG_ROT_4 0x47
#define IME_REG_ROT_5 0x46

// Motor velocity bytes (this is a delta time, unsigned!)
#define IME_REG_VEL_0 0x45
#define IME_REG_VEL_1 0x44

// Command registers on the IME
#define IME_REG_RESET 0x4A
#define IME_REG_DISABLE_TERMINATOR 0x4B
#define IME_REG_ENABLE_TERMINATOR 0x4C
#define IME_REG_SET_ADDR 0x4D

// Scratch pad read command
#define IME_REG_SPAD_READ_MIN 0x60
#define IME_REG_SPAD_READ_MAX 0x9B

// Scratch pad write command
#define IME_REG_SPAD_WRITE_MIN 0xA0
#define IME_REG_SPAD_WRITE_MAX 0xDB

// General-call command reset
#define IME_REG_RESET_ALL 0x4E
#define IME_CMD_BYTE 0xCA
#define IME_CMD_RESET 0x03

// IME information structure
typedef struct {
	// Current IME count
	volatile int32_t count;
	// Current IME offset
	volatile int32_t offset;
	// Current IME velocity bits
	volatile int16_t velocity;
	// IME flags
	volatile uint16_t flags;
} IME_TypeDef;

static IME_TypeDef ime[IME_MAX_NUM];
static volatile TaskHandle imeTask;

// imeSetAddress - Changes the address of an IME
static bool imeSetAddress(uint8_t currentAddress, uint8_t newAddress) {
	return i2cWriteRegister(currentAddress, IME_REG_SET_ADDR, newAddress);
}

// imeTerminate - Sets a previously configured IME to terminate
static bool imeTerminate(uint8_t address) {
	uint8_t temp = IME_REG_ENABLE_TERMINATOR;
	// No value required for this
	return i2cWrite(address, &temp, 1);
}

// imeUnTerminate - Sets a previously configured IME to not terminate
static bool imeUnTerminate(uint8_t address) {
	uint8_t temp = IME_REG_DISABLE_TERMINATOR;
	// No value required for this
	return i2cWrite(address, &temp, 1);
}

// imeInitialize - Initializes the first IME on the chain, sets a new address, and
// opens its terminator. Avoid this function! Use imeInitializeAll() instead.
static bool imeInitialize(uint8_t newAddress) {
	bool ret = imeSetAddress(IME_DEFAULT_ADDR, newAddress << 1);
	if (ret) {
		taskDelay(2);
		if (!imeUnTerminate(newAddress)) ret = false;
	}
	return ret;
}

// imeStatusUpdate - Updates the status of the given IME
static INLINE bool imeStatusUpdate(uint32_t address) {
	uint32_t ans; int32_t sAns, oldCount; uint16_t vv; int16_t sVv;
	bool ret = false; uint8_t data[6];
	ret = i2cReadRegister((uint8_t)address + IME_ADDR_MIN, IME_REG_ROT_1, data, 6);
	if (ret) {
		oldCount = ime[address].count;
		// Byte #0-3
		ans = (uint32_t)data[1];
		ans |= ((uint32_t)data[0]) << 8;
		ans |= ((uint32_t)data[3]) << 16;
		ans |= ((uint32_t)data[2]) << 24;
		sAns = (int32_t)ans;
		ime[address].count = sAns;
		// Byte #4-5
		vv = (uint16_t)data[5];
		vv |= ((uint16_t)data[4]) << 8;
		if (vv > 16384)
			vv = 0;
		sVv = (int16_t)vv;
		// Update sign of answer
		if (oldCount > sAns)
			sVv = -sVv;
		ime[address].velocity = sVv;
	}
	return ret;
}

// imeLoop - Polls IMEs in a loop
static void imeLoop(void *cnt) {
	uint32_t count = (uint32_t) cnt, index = 0;
	// Avoid dying immediately on spawn
	while (imeTask == NULL)
		taskDelay(10);
	// Loop until stop requested
	while (imeTask != NULL) {
		if (!imeStatusUpdate(index)) {
			// Error error! Re-initialize the available IMEs
			taskDelay(250);
			imeInitializeAll();
			if (!imeStatusUpdate(index) && index > 0) {
				// IME is no longer responding, unplugged?
				// If it came unplugged, everything after it is also dead
				count = index;
				index = 0;
			}
		} else {
			// Roll through
			index++;
			if (index >= count) index = 0;
			// Maximum update rate, does not really stall the CPU
			taskDelay(1);
		}
	}
	// Stop IME chain
	uint8_t data[3];
	data[0] = IME_REG_RESET_ALL;
	data[1] = IME_CMD_BYTE;
	data[2] = IME_CMD_RESET;
	i2cWrite(IME_ADDR_GEN_CALL, data, 3);
}

// imeInitializeAll - Initializes all IMEs and returns the number of IMEs thus initialized
unsigned int imeInitializeAll() {
	uint8_t newAddress = IME_ADDR_MIN; unsigned int encodersFound;
	// Clear out status array
	for (encodersFound = 0; encodersFound < IME_MAX_NUM; encodersFound++)
		ime[encodersFound].flags = (uint16_t)0;
	// Initialize IMEs until failure occurs
	encodersFound = 0;
	while (newAddress <= IME_ADDR_MAX) {
		if (!imeUnTerminate(newAddress)) {
			// Increment new address and give it 2 chances to fire up
			// Cortex I2C was changed to be standards-compliant by taking the target address
			// in the low 7 bits of the byte and left-shifting by 1 before adding r/w bit, so
			// IMEs now get addresses 1/2 of what they were, but can use odd and even
			if (!imeInitialize(newAddress) && !imeInitialize(newAddress)) break;
		}
		// Store count and offset (counts are zero initialized)
		ime[encodersFound].offset = -ime[encodersFound].count;
		ime[encodersFound].count = 0;
		ime[encodersFound].flags = (uint16_t)0x0001;
		ime[encodersFound].velocity = 0U;
		newAddress++;
		encodersFound++;
	}
	if (encodersFound > 0)
		// If at least one encoder was found, terminate that one
		imeTerminate(newAddress - 1);
	if (imeTask == NULL) {
		// Start IME handler task if not already running
		TaskHandle h = taskCreate(imeLoop, TASK_MINIMAL_STACK_SIZE, (void *)encodersFound,
			TASK_PRIORITY_HIGHEST - 1);
		if (!h)
			_exit(STATUS_CANT_START_SYSTEM);
		imeTask = h;
	}
	return encodersFound;
}

// imeGet - Gets the current count of the specified IME
bool imeGet(unsigned char address, int *value) {
	if (address < IME_MAX_NUM && ime[address].flags != 0) {
		*value = (int)(ime[address].count - ime[address].offset);
		return true;
	}
	return false;
}

// imeGetVelocity - Gets the current velocity of the specified IME
bool imeGetVelocity(unsigned char address, int *value) {
	int16_t v;
	if (address < IME_MAX_NUM && ime[address].flags != 0) {
		v = ime[address].velocity;
		// Convert using formula on VEX forum into encoder RPM
		if (v > -16 && v < 16)
			*value = 0;
		else
			*value = 468750 / (int)v;
		return true;
	}
	return false;
}

#if 0
// imeGetID - Gets the device ID of the specified IME and puts it as ASCII into the
// provided character array (must have >= 8 characters)
bool imeGetID(uint8_t address, char *id) {
	return i2cReadRegister(address + IME_ADDR_MIN, IME_REG_ID, (uint8_t*)id, 8);
}

// imeGetVendor - Gets the vendor of the specified IME and puts it as ASCII into the
// provided character array (must have >= 8 characters)
bool imeGetVendor(uint8_t address, char *vendor) {
	return i2cReadRegister(address + IME_ADDR_MIN, IME_REG_VENDOR, (uint8_t*)vendor, 8);
}

// imeGetVersion - Gets the version of the specified IME and puts it as ASCII into the
// provided character array (must have >= 8 characters)
bool imeGetVersion(uint8_t address, char *version) {
	return i2cReadRegister(address + IME_ADDR_MIN, IME_REG_VERSION, (uint8_t*)version, 8);
}
#endif

// imeReset - Resets the specified IME's counters to 0
bool imeReset(unsigned char address) {
	if (address < IME_MAX_NUM && ime[address].flags != 0) {
		ime[address].offset = ime[address].count;
		return true;
	}
	return false;
}

// imeShutdown - Resets the entire IME chain's addresses to default
void imeShutdown() {
	imeTask = NULL;
	taskDelay(10);
}
