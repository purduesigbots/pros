/**
 * \file system/startup.c
 *
 * Contains the main startup code for PROS 3.0. main is called from vexStartup
 * code. Our main() initializes data structures and starts the FreeRTOS
 * scheduler.
 *
 * Copyright (c) 2017-2020, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdio.h>

#include "kapi.h"
#include "v5_api.h"

extern void rtos_initialize();
extern void vfs_initialize();
extern void system_daemon_initialize();
extern void rtos_sched_start();
extern void vdml_initialize();
extern void invoke_install_hot_table();

// XXX: pros_pre_init and pros_init happen inside __libc_init_array, and
// before any globalC++ constructors are invoked. This is accomplished by
// instructing GCC to include this function in the __init_array. These
// arguments give the compiler instructions on the priority of the constructor,
// from 0-~65k. The first 0-100 priorities are reserved for language
// implementation.

#define PROS_KERNEL_PRE_INIT 110
#define PROS_KERNEL_INIT     120

__attribute__((constructor(PROS_KERNEL_PRE_INIT))) static void pros_pre_init(void) {
	rtos_initialize();

	vfs_initialize();

	vdml_initialize();
}

// HACK: External templates can hook into initialization process by setting
// their priority between PROS_KERNEL_PRE_INIT and PROS_KERNEL_INIT. For
// exmaple the priority of display initialization is 115. All other initialize
// functions should be called before pros_init function. For an exmaple of
// what could happen if this is not the case, see
// https://github.com/purduesigbots/pros/pull/144/#issuecomment-496901942

__attribute__((constructor(PROS_KERNEL_INIT))) static void pros_init(void) {
	system_daemon_initialize();

	invoke_install_hot_table();
}

int main() {
	rtos_sched_start();

	vexDisplayPrintf(10, 60, 1, "failed to start scheduler\n");

	printf("Failed to start Scheduler\n");
	for (;;)
		;
}
