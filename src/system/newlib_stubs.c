/**
 * \file system/newlib_stubs.c
 *
 * Port of newlib to PROS for the V5
 *
 * Contains the various methods needed to enable standard C library support
 * through the use of the Arm-distributed implementation of newlib.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
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

#include "rtos/task.h"
#include "v5_api.h"

#include "hot.h"
#include "pros/misc.h"

#define SEC_TO_MSEC 1000
#define SEC_TO_MICRO 1000000
#define MICRO_TO_NANO 1000

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

// These variables are used to store the user-set time.
// When user_time_set is false, the realtime clock will use the timestamp as the
// base time. When it is true will use user_time_spec as the base time instead.
// set_microseconds stores the value of the microsecond timer when the user set
// the time.
static bool user_time_set = false;
static struct timespec user_time_spec;
static int64_t set_microseconds = 0;

int clock_settime(clockid_t clock_id, const struct timespec *tp) {
	if(tp == NULL) {
		errno = EINVAL;
		return -1;
	}

	int retval = -1;

	switch(clock_id) {
	case CLOCK_REALTIME:
		user_time_set = true;
		user_time_spec	 = *tp;
		set_microseconds = vexSystemHighResTimeGet();
		retval = 0;
	default:
		errno = EINVAL;
	}

	return retval;
}

int clock_gettime(clockid_t clock_id, struct timespec* tp) {
	if(tp == NULL) {
		errno = EINVAL;
		return -1;
	}
	
	struct timeval tv;
	int retval = -1;

	switch (clock_id)
	{
	case CLOCK_REALTIME:
		retval = gettimeofday(&tv, NULL);
		if (!retval) TIMEVAL_TO_TIMESPEC(&tv, tp);
		break;
	case CLOCK_MONOTONIC: {
		uint64_t totalTime = vexSystemHighResTimeGet();
		uint64_t secs = totalTime / SEC_TO_MICRO;
		uint64_t micros = totalTime - secs * SEC_TO_MICRO;

		tp->tv_sec = secs;
		tp->tv_nsec = micros * MICRO_TO_NANO;
		break;
	}
	default:
		errno = EINVAL;
		break;
	}

	return retval;
}

// HACK:
//
// This function pointer serves as a callback so that _gettimeofday() can call
// a function inside the hot package. Without this, _gettimeofday() cannot
// access any symbols in the hot package (where _PROS_COMPILE_TIMESTAMP_INT 
// lives), and linker errors occur. 
//
// When the hot package is initialized, it calls set_get_timestamp_int_func()
// and sets the callback to a function that returns the unix timestamp.
//
// Essentially, when the hot process starts:
//   1) Pass the get_timestamp_int_func to the cold package
//   2) When the cold package (this library) needs to access the timestamp,
//      call the callback
//   3) Then the cold package 
static const int (*get_timestamp_int_func)(void) = NULL;

void set_get_timestamp_int_func(const int (*func)(void))
{
	get_timestamp_int_func = func;
}

int _gettimeofday(struct timeval* tp, void* tzvp) {
	if(get_timestamp_int_func == NULL) {
		return -1;
	}

	if(user_time_set) {
		tp->tv_sec = user_time_spec.tv_sec;
		tp->tv_usec = user_time_spec.tv_nsec * 1000;
		tp->tv_usec += vexSystemHighResTimeGet() - set_microseconds;
	}
	else if (competition_is_connected()) {
		// TODO: update this to get the date/time through VexOS. Apparently,
		// the time is kept properly only when competition controls are
		// connected. I haven't had time to check or confirm this.
		//https://github.com/purduesigbots/pros/pull/127#issuecomment-1095361338
		tp->tv_sec = get_timestamp_int_func();
		tp->tv_usec = vexSystemHighResTimeGet();
	}
	else {
		// When competition isn't connected, the vex's date/time functions do
		// not work. Here we use a timestamp compiled into the program and then
		// add the number of microseconds the program has been running to get 
		// the best estimate.
		tp->tv_sec = get_timestamp_int_func();
		tp->tv_usec = vexSystemHighResTimeGet();
	}

	return 1;
}
