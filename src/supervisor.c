/*
 * supervisor.c - Functions for interfacing with the NXP supervisor processor responsible for
 * handling the gory details of VEXnet
 *
 * Copyright (c) 2011-2017, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include <comm.h>
#include <invoke.h>
#include <kernel.h>
#include <supervisor.h>
#include <periph.h>

// A magic number for communication!
#define SV_MAGIC ((uint16_t)0xC917)

// Release-ready?
#if (FW_VERSION_TYPE == RELEASE)
#define FW_DISCLAIMER ""
#else
#define FW_DISCLAIMER "This is a BETA version of PROS and may have issues.\r\n"
#endif

// Buffers for SPI communications (16 words = 32 bytes)
volatile uint16_t spiBufferRX[16], spiBufferTX[16];
// Flag bits for the FMS
volatile uint16_t svFlags;
// Last flag bits for the FMS
static volatile uint16_t svLastFlags;
// Value used to fix backup battery + power expander problems
static uint16_t svPowerFlags;
// Index counter used for the interrupt-driven transfer
static uint8_t svIndex;
// Packet number, wraps around every 256th packet
static uint8_t svPacket;
// Team name to report when asked for configuration
char svTeamName[8];

// Supervisor transfer complete, check the flags sent
static INLINE void _svComplete() {
	uint8_t temp, i; uint16_t flags = 0;
	// Start checking the configuration bits
	if (SV_IN->key == SV_MAGIC) {
		temp = SV_IN->inMode;
		if (temp & 2) {
			// Configuration (team name?)
			if (temp & 1) {
				// Default data
				SV_OUT->mode = 3;
				SV_OUT->outMode = 0;
				svSetAllData(127);
			} else {
				// Team name
				SV_OUT->mode = 2;
				SV_OUT->outMode = 85;
				for (i = 0; i < 8; i++)
					SV_OUT->data[i] = svTeamName[i];
			}
		} else if (temp & 4)
			// Ready for mode set
			SV_OUT->mode = 8;
		else if (temp == 8) {
			// Mode set
			SV_OUT->mode = 8;
			flags = SV_CONNECTED;
			// Check the game state
			temp = SV_IN->gameStatus;
			if (temp & (uint8_t)0x08) {
				// FMS is connected
				flags |= SV_FMS;
				if (temp & (uint8_t)0x40)
					// Autonomous mode
					flags |= SV_AUTONOMOUS;
				if (!(temp & (uint8_t)0x80))
					// Enabled
					flags |= SV_ENABLED;
			}
			if (flags != svLastFlags) {
				// Something changed! Wake up the daemon to take action
				svLastFlags = flags;
				invWakeup();
			}
			// Backup battery fix
			if (SV_IN->mainBattery <= 32) {
				if (svPowerFlags < 64) svPowerFlags++;
			} else {
				if (svPowerFlags > 0) svPowerFlags--;
			}
		}
		// Update the supervisor flags
		svFlags = flags;
	}
}

// _spiDoNext - Reads an SPI byte and inserts delay if needed
static void _spiDoNext() {
	uint8_t idx = svIndex;
	// Receive buffer full, read data
	spiBufferRX[idx++] = SPI1->DR;
	svIndex = idx;
	// Negate NSS
	ioSetOutput(GPIOE, 0, 1);
	if (idx < 16) {
		if (idx & 3) {
			// Setup/hold time of 8? us
			_highResSchedule(0, 8);
		} else {
			// Setup/hold time of 73? us
			_highResSchedule(0, 73);
		}
	} else {
		// All done
		_svComplete();
		svIndex = 0;
	}
}

// _svNextByte - Helper to send off the next byte for SPI
void _svNextByte() {
	uint8_t idx = svIndex;
	if (idx == 4)
		ioSetOutput(GPIOA, 11, 0);
	// Assert SS (GPIOE 0) and store next value
	ioSetOutput(GPIOE, 0, 0);
	SPI1->DR = spiBufferTX[idx];
}

// standaloneModeEnable - enable standalone operation
void standaloneModeEnable() {
	// set flag in outgoing SPI buffer to enable standlone mode
	SV_OUT->flags |= 1;
}

// svInit - Initialize supervisor communications
void svInit() {
	uint8_t i;
	// Wait for the supervisor to come up
	ioSetOutput(GPIOE, 0, 1);
	svPacket = (uint8_t)0x00;
	// Read 8 bytes of garbage data (?)
	for (i = 8; i; i--)
		(void)SPI1->DR;
	// When the requisite port E pins [3 and 4] go low, master is ready for SPI data
	while (ioGetInput(GPIOE, 3) || ioGetInput(GPIOE, 4)) __sleep();
	// Reset all flags
	svFlags = (uint32_t)0x00000000;
	svLastFlags = (uint32_t)0x00000000;
	svPowerFlags = (uint16_t)0x0000;
	svIndex = (uint8_t)0x00;
	// Initialize the array
	for (i = 1; i < 16; i++)
		spiBufferTX[i] = (uint16_t)0x0000;
	SV_OUT->key = SV_MAGIC;
	SV_OUT->mode = 2;
	SV_OUT->flags = 0;
	SV_OUT->version = 1;
	// Load in empty motor values
	svSetAllData((uint8_t)0x7F);
	// Load in neutral joystick values
	for (i = 0; i < 6; i++) {
		SV_IN->joystick[0].axis[i] = (uint8_t)0x7F;
		SV_IN->joystick[1].axis[i] = (uint8_t)0x7F;
	}
	// Load in no buttons pressed
	SV_IN->joystick[0].button56 = (uint8_t)0x00;
	SV_IN->joystick[0].button78 = (uint8_t)0x00;
	SV_IN->joystick[1].button56 = (uint8_t)0x00;
	SV_IN->joystick[1].button78 = (uint8_t)0x00;
	// Default team name shows up during firmware flashing
	svTeamName[0] = 'P';
	svTeamName[1] = 'R';
	svTeamName[2] = 'O';
	svTeamName[3] = 'S';
	svTeamName[4] = ' ';
	svTeamName[5] = ' ';
	svTeamName[6] = ' ';
	svTeamName[7] = ' ';
	// Now we are ready to enable RXNE interrupt
	SPI1->CR2 = SPI_CR2_RXNEIE;
}

// svSynchronize - Waits for the supervisor to synchronize, then prints the startup message
void svSynchronize() {
	while (!(svFlags & SV_CONNECTED)) __sleep();
	// HELLO message!
	kwait(50);
	print("\r\nPowered by PROS " FW_VERSION "\r\n" FW_DISCLAIMER
		"\r\nPROS (C)2011-2017 Purdue ACM SIGBOTS\r\n");
	print("This program has ABSOLUTELY NO WARRANTY, not even an implied\r\n"
		"warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\r\n\r\n");
}

// ISR_SPI1 - Interrupt-driven routine to handle master communication
void ISR_SPI1() {
	if (SPI1->SR & SPI_SR_RXNE)
		_spiDoNext();
}

// Updates the SPI master on each tick
void svStartTransfer() {
	// Check power level to fix backup battery issues
	if (svPowerFlags > 32)
		svSetAllData((uint8_t)0x7F);
	if (svIndex == 0) {
		// Packet number goes in the last byte (assuming that overflow is fine here)
		SV_OUT->packetNum = svPacket++;
		if (SPI1->SR & SPI_SR_TXE) {
			// Raise GPIOA 11 high
			ioSetOutput(GPIOA, 11, 1);
			// Assert SS
			ioSetOutput(GPIOE, 0, 0);
			// Start by storing first data value
			SPI1->DR = spiBufferTX[0];
		}
	} else
		_spiDoNext();
}
