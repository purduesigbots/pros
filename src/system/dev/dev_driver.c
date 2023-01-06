/**
 * \file system/dev/dev_driver.c
 *
 * Generic Serial Device driver
 *
 * Contains the driver for writing to any smart port with no regard to the
 * device on the other end.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "common/set.h"
#include "common/string.h"
#include "kapi.h"
#include "system/dev/dev.h"
#include "system/dev/vfs.h"
#include "system/optimizers.h"
#include "v5_api.h"
#include "vdml/vdml.h"

#define ASCII_ZERO 48

typedef struct dev_file_arg {
	uint32_t port;
	int flags;
} dev_file_arg_t;

/******************************************************************************/
/**                         newlib driver functions                          **/
/******************************************************************************/
int dev_read_r(struct _reent* r, void* const arg, uint8_t* buffer, const size_t len) {
	dev_file_arg_t* file_arg = (dev_file_arg_t*)arg;
	uint32_t port = file_arg->port;
	int32_t recv = 0;
	while (true) {
		recv = serial_read(port, (uint8_t*)(buffer + recv), len - recv);
		if (recv == PROS_ERR) {
			return 0;
		}
		if (file_arg->flags & O_NONBLOCK || recv >= 1) {
			break;
		}
		task_delay(2);
	}
	if (recv == 0) {
		errno = EAGAIN;
		return 0;
	}
	return recv;
}

int dev_write_r(struct _reent* r, void* const arg, const uint8_t* buf, const size_t len) {
	dev_file_arg_t* file_arg = (dev_file_arg_t*)arg;
	uint32_t port = file_arg->port;
	int32_t wrtn = 0;
	while (true) {
		int32_t w = serial_write(port, (uint8_t*)(buf + wrtn), len - wrtn);
		if (w == PROS_ERR) {
			return wrtn;
		}
		wrtn += w;
		if (file_arg->flags & O_NONBLOCK || wrtn >= len) {
			break;
		}
		task_delay(2);
	}
	if (wrtn == 0) {
		errno = EAGAIN;
		return 0;
	}
	return wrtn;
}

int dev_close_r(struct _reent* r, void* const arg) {
	return 0;
}

int dev_fstat_r(struct _reent* r, void* const arg, struct stat* st) {
	// this is a complete implementation
	st->st_mode = S_IFCHR;
	return 0;
}

int dev_isatty_r(struct _reent* r, void* const arg) {
	// this is a complete implementation
	return 0;
}

off_t dev_lseek_r(struct _reent* r, void* const arg, off_t ptr, int dir) {
	// lseek doesn't make sense on a serial line
	r->_errno = ESPIPE;
	return -1;
}

int dev_ctl(void* const arg, const uint32_t cmd, void* const extra_arg) {
	dev_file_arg_t* file_arg = (dev_file_arg_t*)arg;
	uint32_t port = file_arg->port;
	switch (cmd) {
		case DEVCTL_FIONREAD:
			return serial_get_read_avail(port);
		case DEVCTL_FIONWRITE:
			return serial_get_write_free(port);
		case DEVCTL_SET_BAUDRATE:
			return serial_set_baudrate(port, (int32_t)extra_arg);
		default:
			errno = EINVAL;
			return PROS_ERR;
	}
}

/******************************************************************************/
/**                           Driver description                             **/
/******************************************************************************/
const struct fs_driver _dev_driver = {.close_r = dev_close_r,
                                      .fstat_r = dev_fstat_r,
                                      .isatty_r = dev_isatty_r,
                                      .lseek_r = dev_lseek_r,
                                      .read_r = dev_read_r,
                                      .write_r = dev_write_r,
                                      .ctl = dev_ctl};

const struct fs_driver* const dev_driver = &_dev_driver;

int dev_open_r(struct _reent* r, const char* path, int flags, int mode) {
	if (*path == '\0') {
		return STDOUT_FILENO;
	}

	if (*path == '/') {
		path++;
	}

	// check length of path - it MUST be at most 2 characters
	size_t i;
	for (i = 0; i < 3; i++) {
		if (path[i] == '\0') {
			break;
		}
	}
	int32_t port;
	if (path[i] != '\0') {  // i will the length of the path or the third character
		r->_errno = ENAMETOOLONG;
		return -1;
	}
	if (i == 2) {
		// Port number is two characters
		port = 10 * (path[0] - ASCII_ZERO) + (path[1] - ASCII_ZERO);
	} else {
		port = path[0] - ASCII_ZERO;
	}
	serial_enable(port);

	dev_file_arg_t* arg = (dev_file_arg_t*)kmalloc(sizeof(dev_file_arg_t));
	arg->port = port;
	arg->flags = flags;
	return vfs_add_entry_r(r, dev_driver, arg);
}
