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

#include "competition.h"

extern void rtos_install();

StaticTask_t firstTaskBuffer;
StackType_t firstTaskStack[8192];

int main()
{
	vexDisplayStringAt(10, 40, "hello world!");

	rtos_install();

	competition_initialize();

	vTaskStartScheduler();

	printf("Failed to start Scheduler\n");
	for (;;)
		;
}
