/**
 * newlib_stubs.c - Port of newlib to PROS for the V5
 *
 * Contains the various methods needed to enable standard C library support
 * through the use of the Arm-distributed implementation of newlib.
 *
 * Copyright (c) 2017, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <errno.h>
#include <stdint.h>
#include <unistd.h>

#include "ifi/v5_api.h"
#include "rtos/task.h"

ssize_t _write_r(struct _reent* r, int fd, const void* buf, size_t size)
{
	if (fd == STDIN_FILENO || fd == STDOUT_FILENO) {
		return vexSerialWriteBuffer(1, (uint8_t*)buf, size);
	}
	r->_errno = EBADF;
	return 0;
}

void __malloc_lock() { vTaskSuspendAll(); }

void __malloc_unlock() { xTaskResumeAll(); }

void __env_lock() { vTaskSuspendAll(); }

void __env_unlock() { xTaskResumeAll(); }