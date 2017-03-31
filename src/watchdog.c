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

static bool iwdgEnabled = false;

// iwdgInit - Enables the watchdog
void watchdogInit() {
	iwdgEnabled = true;
}

// iwdgFeed - Resets the watchdog's timer
static inline void _iwdgFeed() {
	IWDG->KR = 0xAAAA;
}

static void _iwdgTask(void* ud) {
	(void)(ud);
	clock_t now = timeLowRes();
	for (;;) {
		_iwdgFeed();
		taskDelayUntil(&now, 125);
	}
}

// iwdgStart - Initializes the watchdog with a timeout
void watchdogStart() {
  // implementation based on STM32 IWDG guidance. (https://goo.gl/sQaIF3)
	if (!iwdgEnabled) return;

	IWDG->KR = 0x5555; // enable access to iwdg reg (prevents accidental writes into iwdg cofig by runaway code)
	IWDG->PR = 2; // prescaler divides low speed internal (LSI) oscillator
	IWDG->RLR = 2500; // (timeout * 40) / exp(2, (2 + prescalerCode)) as per specification
	IWDG->KR = 0xAAAA; // refresh the watchdog (i.e. load these settings)
	IWDG->KR = 0xCCCC; // start the watchdog
	_iwdgFeed(); // feed it once and start the task

	taskCreate(_iwdgTask, TASK_MINIMAL_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
}
