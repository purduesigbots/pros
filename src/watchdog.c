/*
 * watchdog.c - IWDG API for reseting the cortex when something goes wrong
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

#include <stdint.h>
#include <watchdog.h>
#include <task.h>

#define IWDG_TIMEOUT (1000)

static bool iwdgEnabled = false;

// iwdgStart - Enables the watchdog 
void iwdgEnable() {
	iwdgEnabled = true;
}

static void iwdgTask(void* ud) {
	(void)(ud);
	for (;;) {
		iwdgFeed();
		taskDelay(IWDG_TIMEOUT / 2);
	}
}

// iwdgStart - Initializes the watchdog with a timeout
void iwdgStart() {
	if (!iwdgEnabled) return;

	uint16_t prescalerCode = 0;
	uint16_t prescaler = 4;

	while (IWDG_TIMEOUT * (40 / prescaler) >= 0x7FF && prescalerCode < 7) {
		prescalerCode++;
		prescaler *= 2;
	}

	IWDG->KR = 0x5555;
	IWDG->PR = prescalerCode;
	IWDG->RLR = IWDG_TIMEOUT * (40 / prescaler);
	IWDG->KR = 0xAAAA;
	IWDG->KR = 0xCCCC;
	iwdgFeed();

	taskCreate(iwdgTask, TASK_MINIMAL_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
}

// iwdgFeed - Resets the watchdog's timer
void iwdgFeed() {
	IWDG->KR = 0xAAAA;
}