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

#include "rtos/task.h"
#include "v5_api.h"

void _exit(int status) {
	if(status != 0) kprintf("Error %d\n", status);
	vexSystemExitRequest();
}

int usleep( useconds_t period ) {
	// naive blocking delay, still needs testing as I'm not sure if the tight loop will work.
	uint64_t endTime = vexSystemHighResTimeGet() + period;
	while(vexSystemHighResTimeGet() < endTime) asm("NOP");
	return 0;
}

unsigned sleep( unsigned period ) {
	return usleep(period * 1000000);
}

// HACK: this helps confused libc++ functions call the right instruction. for
// info see https://github.com/purduesigbots/pros/issues/153#issuecomment-519335375
void __sync_synchronize(void) {
	__sync_synchronize();
}
