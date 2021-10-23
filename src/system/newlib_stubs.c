/**
 * \file system/newlib_stubs.c
 *
 * Port of newlib to PROS for the V5
 *
 * Contains the various methods needed to enable standard C library support
 * through the use of the Arm-distributed implementation of newlib.
 *
 * Copyright (c) 2017-2021, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>

#include "pros/rtos.h"
#include "v5_api.h"

#define SEC_TO_MSEC 1000

void _exit(int status) {
	if(status != 0) dprintf(3, "Error %d\n", status); // kprintf
	vexSystemExitRequest();
}

int usleep( useconds_t period ) {
	task_delay_micros(period);
	return 1;

}

unsigned sleep( unsigned period ) {
	task_delay(period * SEC_TO_MSEC);
	return 1;
}

// HACK: this helps confused libc++ functions call the right instruction. for
// info see https://github.com/purduesigbots/pros/issues/153#issuecomment-519335375
void __sync_synchronize(void) {
	__sync_synchronize();
}
