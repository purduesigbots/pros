/*
 * supervisor.h - Supervisor functions to communicate with the NXP VEXnet processor
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

#ifndef SUPERVISOR_H_
#define SUPERVISOR_H_

#include <stdint.h>
#include <cortex.h>

// Begin C++ extern to C
#ifdef __cplusplus
extern "C" {
#endif

// Structure representing a joystick
typedef struct {
	// Joystick axes
	volatile uint8_t axis[6];
	// Appears to be always 0x7F
	volatile uint8_t RESERVED0;
	// Joystick button #5 and 6
	volatile uint8_t button56;
	// Joystick button #7 and 8
	volatile uint8_t button78;
	// Appears to be always zero
	volatile uint8_t RESERVED1[3];
} Joystick_TypeDef;

// Structure representing the 32 bytes of data received from the supervisor
typedef struct {
	// Operation key, should be C917 during normal operation
	volatile uint16_t key;
	// Determines what type of information should be sent
	volatile uint8_t inMode;
	// Game status
	volatile uint8_t gameStatus;
	// Main battery "voltage"
	volatile uint8_t mainBattery;
	// Backup battery "voltage"
	volatile uint8_t backupBattery;
	// Joysticks
	volatile Joystick_TypeDef joystick[2];
	// Version
	volatile uint8_t version;
	volatile uint8_t RESERVED;
} Supervisor_RecvTypeDef;

// Structure representing the 32 bytes of data sent to the supervisor
typedef struct {
	// Should always be the magic number 0xC917
	volatile uint16_t key;
	// Response mode to the input mode
	volatile uint8_t mode;
	// Additional flags (brake mode!?)
	volatile uint8_t flags;
	volatile uint8_t RESERVED0;
	// Unsure
	volatile uint8_t outMode;
	// Data values (motor PWM or team name)
	volatile uint8_t data[8];
	volatile uint32_t RESERVED1[4];
	// Version
	volatile uint8_t version;
	// Packet number
	volatile uint8_t packetNum;
} Supervisor_TransTypeDef;

// Supervisor flags
extern volatile uint16_t svFlags;
// Buffers for SPI communications (16 words = 32 bytes)
extern volatile uint16_t spiBufferRX[16], spiBufferTX[16];
// Team name to report when asked for configuration
extern char svTeamName[8];
// Alias buffers to structures for easy lookup
#define SV_IN ((Supervisor_RecvTypeDef*)(&spiBufferRX))
#define SV_OUT ((Supervisor_TransTypeDef*)(&spiBufferTX))

// Autonomous flag bit
#define SV_AUTONOMOUS ((uint16_t)0x0004)
// Connected flag bit
#define SV_CONNECTED ((uint16_t)0x0001)
// Enabled flag bit
#define SV_ENABLED ((uint16_t)0x0002)
// FMS enabled bit
#define SV_FMS ((uint16_t)0x0008)

// svSetAllData - Loads data into all eight output registers to be sent to the supervisor
static INLINE void svSetAllData(uint8_t value) {
	uint32_t i;
	for (i = 0; i < 8; i++)
		SV_OUT->data[i] = value;
}

// svGetOutputData - Reads data from a specified output register that would be sent to the
// supervisor; intended for motorGet() call support
static INLINE uint8_t svGetOutputData(uint8_t index) {
	return SV_OUT->data[index];
}

// svGetBackupBattery - Gets the backup battery voltage in millivolts, or 0 if not connected
static INLINE uint32_t svGetBackupBattery() {
	uint32_t volts = ((uint32_t)SV_IN->backupBattery) * 59;
	if (volts < 1000)
		volts = 0;
	return volts;
}

// svGetJoystickAnalog - Gets an analog joystick axis or accelerometer axis from the supervisor
// NO check on the mode when doing so
static INLINE int8_t svGetJoystickAnalog(uint8_t joystick, uint8_t axis) {
	uint8_t value;
	// Force in range from 0..5 (mapped from 1..6)
	axis = (axis - 1) % 6;
	// Force in range from 0..1 (mapped from 1..2)
	joystick = (joystick - 1) & (uint8_t)0x01;
	value = SV_IN->joystick[joystick].axis[axis];
	// Prevent return of 128
	if (value == 0xFF)
		value = 0xFE;
	value -= 127;
	if (axis == 1 || axis == 2)
		value = -value;
	return (int8_t)value;
}

// svGetJoystickDigital - Gets a digital joystick button's bitmask value from the supervisor
// NO check on the mode when doing so
static INLINE uint8_t svGetJoystickDigital(uint8_t joystick, uint8_t button) {
	uint8_t value; volatile Joystick_TypeDef *ref;
	// Force in range from 0..3 (mapped from 5..8)
	button = (button - 5) & (uint8_t)0x03;
	// Force in range from 0..1 (mapped from 1..2)
	joystick = (joystick - 1) & (uint8_t)0x01;
	ref = &SV_IN->joystick[joystick];
	// 5 and 6 need some mangling to move the twos bit up to the fours
	switch (button) {
	case 0:
		// 5
		value = ref->button56;
		return ((value & 0x02) << 1) | (value & 0x01);
	case 1:
		// 6
		value = ref->button56 >> 2;
		return ((value & 0x02) << 1) | (value & 0x01);
	case 2:
		// 7
		return ref->button78 >> 4;
	default:
		// 8 (No other case is possible...)
		return ref->button78 & 0x0F;
	}
}

// svGetMainBattery - Gets the main battery voltage in millivolts, or 0 if not connected
static INLINE uint32_t svGetMainBattery() {
	uint32_t volts = ((uint32_t)SV_IN->mainBattery) * 59;
	if (volts < 1000)
		volts = 0;
	return volts;
}

// svIsAutonomous - Checks to see if the system is in autonomous mode
static INLINE uint8_t svIsAutonomous() {
	return (svFlags & SV_AUTONOMOUS) >> 2;
}

// svIsEnabled - Checks to see if the system is enabled
static INLINE uint8_t svIsEnabled() {
	return (svFlags & SV_ENABLED) >> 1;
}

// svIsJoystickConnected - Checks to see if a joystick is connected
static INLINE uint8_t svIsJoystickConnected(uint8_t joystick) {
	// Force in range from 0..1 (mapped from 1..2)
	joystick = (joystick - 1) & (uint8_t)0x01;
	// If both accelerometer axes are zero, the joystick is likely not plugged in
	// Obviously there is a chance for false positives but extremely unlikely due to the noise
	// of the analog accelerometers
	return SV_IN->joystick[joystick].axis[4] != (uint8_t)0x7F ||
		SV_IN->joystick[joystick].axis[5] != (uint8_t)0x7F;
}

// svIsOnline - Checks to see if the system is connected to a FMS/competition switch
static INLINE uint8_t svIsOnline() {
	return (svFlags & SV_FMS) >> 3;
}

// svSetData - Loads data into a specified output register to be sent to the supervisor
static INLINE void svSetData(uint8_t index, uint8_t value) {
	SV_OUT->data[index] = value;
}

// svSetTeamName - Changes the team name reported to the supervisor
static INLINE void svSetTeamName(const char *name) {
	char c; uint8_t i;
	for (i = 0; i < 8; i++) {
		// Copy up to 8 characters, respect zero terminator
		c = (char)(*name++);
		if (!c) break;
		svTeamName[i] = c;
	}
	// Space-pad
	for (; i < 8; i++)
		svTeamName[i] = ' ';
}

// svInit - Initialize supervisor communications
void svInit();
// svSetAllData - Loads data into all eight output registers to be sent to the supervisor
void svSetAllData(uint8_t value);
// svStartTransfer - Begins a supervisor transfer
void svStartTransfer();
// svSynchronize - Waits for the supervisor to synchronize, then reads the game status bits
void svSynchronize();

// End C++ extern to C
#ifdef __cplusplus
}
#endif

#endif
