/**
 * main.c - PROS Kernel Startup
 *
 * Contains the main startup code for PROS 3.0. main is called from vexStartup
 * code. Our main() initializes data structures and starts the FreeRTOS
 * scheduler.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 * All rights reservered.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdio.h>

#include "kapi.h"

#include "ifi/v5_api.h"

extern void rtos_initialize();
extern void vfs_initialize();
extern void system_daemon_initialize();
// extern void graphical_context_daemon_initialize(void);
extern void display_initialize(void);
extern void rtos_sched_start();

int main() {
  rtos_initialize();

  vfs_initialize();

  system_daemon_initialize();

  // graphical_context_daemon_initialize();
  display_initialize();

  rtos_sched_start();

  vexDisplayPrintf(10, 60, 1, "failed to start scheduler\n");

  printf("Failed to start Scheduler\n");
  for (;;)
    ;
}
