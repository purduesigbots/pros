/**
 * \file system/newlib_stubs.c
 *
 * Port of newlib to PROS for the V5
 *
 * Contains the various methods needed to enable standard C library support
 * through the use of the Arm-distributed implementation of newlib.
 *
 * Copyright (c) 2017-2022, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <errno.h>
#include <stdint.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

#include "kapi.h"

#include "rtos/task.h"
#include "v5_api.h"

#include "hot.h"
#include "pros/misc.h"

#define SEC_TO_MSEC 1000

void _exit(int status) {
	if(status != 0) dprintf(3, "Error %d\n", status); // kprintf
	vexSystemExitRequest();
}

int usleep( useconds_t period ) {
	// Compromise: If the delay is in microsecond range, it will block threads.
	// if not, it will not block threads but not be accurate to the microsecond range.
	if(period >= 1000) {
		task_delay (period / SEC_TO_MSEC);
		return 0;
	}
	uint64_t endTime = vexSystemHighResTimeGet() + period;
	while(vexSystemHighResTimeGet() < endTime) asm("YIELD");
	return 0;
}

unsigned sleep( unsigned period ) {
	task_delay(period * SEC_TO_MSEC);
	return 1;
}

int getentropy(void *_buffer, size_t _length) {
	errno = ENOSYS;
	return -1;
}

// HACK: this helps confused libc++ functions call the right instruction. for
// info see https://github.com/purduesigbots/pros/issues/153#issuecomment-519335375
void __sync_synchronize(void) {
	__sync_synchronize();
}

int clock_gettime(clockid_t clock_id, struct timespec* tp) {
	struct timeval tv;
	int retval = -1;

	switch (clock_id)
	{
	case CLOCK_REALTIME:
		retval = gettimeofday(&tv, NULL);
		if (!retval) TIMEVAL_TO_TIMESPEC(&tv, tp);
		break;
	
	default:
		errno = EINVAL;
		break;
	}

	return retval;
}

// This function pointer serves as a callback so that _gettimeofday() can call
// a function inside the hot package. Without this, _gettimeofday() cannot
// access any symbols in the hot package (where _PROS_COMPILE_TIMESTAMP_INT 
// lives), and linker errors occur. 
//
// When the hot package is initialized, it calls set_get_timestamp_int_func()
// and sets the callback to a function that returns the unix timestamp.
static const int (*get_timestamp_int_func)(void) = NULL;

void set_get_timestamp_int_func(const int (*func)(void))
{
	get_timestamp_int_func = func;
}

int _gettimeofday(struct timeval* tp, void* tzvp) {
	if (competition_is_connected()) {
		kprintf("Using competition calculation");
		tp->tv_sec = get_timestamp_int_func();
		tp->tv_usec = vexSystemHighResTimeGet();
	}
	else {
		kprintf("Using other calculation");
		tp->tv_sec = get_timestamp_int_func();
		tp->tv_usec = vexSystemHighResTimeGet();
	}

	return 1;
}
