/**
 * main.c - PROS Kernel Startup
 *
 * Contains the main startup code for PROS 3.0. main is called from vexStartup
 * code. Our main() initializes data structures and starts the FreeRTOS
 * scheduler.
 *
 * Copyright (c) 2017, Purdue University ACM SIGBots.
 * All rights reservered.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdio.h>

#include "rtos/FreeRTOS.h"
#include "rtos/Task.h"
#include "rtos/semphr.h"

#include "ifi/v5_api.h"

#include "system/comp_state.h"

extern void rtos_initialize();

int main() {
	rtos_initialize();

	system_daemon_initialize();

	rtos_sched_start();

	vexDisplayPrintf(10, 60, 1, "failed to start scheduler\n");

	printf("Failed to start Scheduler\n");
	for (;;)
		;
}
