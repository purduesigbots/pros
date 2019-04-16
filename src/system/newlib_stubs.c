/**
 * \file system/newlib_stubs.c
 *
 * Port of newlib to PROS for the V5
 *
 * Contains the various methods needed to enable standard C library support
 * through the use of the Arm-distributed implementation of newlib.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <errno.h>
#include <stdint.h>
#include <unistd.h>

#include "rtos/task.h"
#include "v5_api.h"

void _exit(int status) {
	// TODO: print status code, maybe backtrace as well
	while (1) {
		vexBackgroundProcessing();
	}
}

void __malloc_lock(void) {
	rtos_suspend_all();
}

void __malloc_unlock(void) {
	rtos_resume_all();
}

void __env_lock(void) {
	rtos_suspend_all();
}

void __env_unlock(void) {
	rtos_resume_all();
}
void __sync_synchronize(void) {
	/* Perform a memory barrier to workaround malfunctional lock */
	asm volatile("dmb" : : : "memory");
}

// FIXME: Shoud we also implement atomic operations? Zynq is a SMP SoC.
