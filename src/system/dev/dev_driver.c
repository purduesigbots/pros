/**
 * \file system/dev/dev_driver.c
 *
 * Generic Serial Device driver
 *
 * Contains the driver for writing to any smart port with no regard to the
 * device on the other end.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
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
} dev_file_arg_t;

/******************************************************************************/
/**                         newlib driver functions                          **/
/******************************************************************************/
int dev_read_r(struct _reent* r, void* const arg, uint8_t* buffer, const size_t len) {
	dev_file_arg_t* file_arg = (dev_file_arg_t*)arg;
	uint32_t port = file_arg->port;
	claim_port(port - 1, E_DEVICE_GENERIC);
	// check flags
	int32_t result = vexDeviceGenericSerialReceive(device->device_info, buffer, len);
	return_port(port - 1, result);
}

int dev_write_r(struct _reent* r, void* const arg, const uint8_t* buf, const size_t len) {
	dev_file_arg_t* file_arg = (dev_file_arg_t*)arg;
	uint32_t port = file_arg->port;
	claim_port(port - 1, E_DEVICE_GENERIC);
	int32_t result = vexDeviceGenericSerialTransmit(device->device_info, (uint8_t*)buf, (int32_t)len);
	vexDeviceGenericSerialFlush(device->device_info);
	return_port(port - 1, result);
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
	claim_port(port - 1, E_DEVICE_GENERIC);
	int rtn;
	switch (cmd) {
		case DEVCTL_FIONREAD:
			rtn = vexDeviceGenericSerialReceiveAvail(device->device_info);
			return_port(port - 1, rtn);
		case DEVCTL_SET_BAUDRATE:
			vexDeviceGenericSerialBaudrate(device->device_info, (int32_t)extra_arg);
			return_port(port - 1, 0);
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
	claim_port(port - 1, E_DEVICE_GENERIC);
	// the options param is for VEX internal use
	vexDeviceGenericSerialEnable(device->device_info, 0);
	port_mutex_give(port - 1);

	dev_file_arg_t* arg = (dev_file_arg_t*)kmalloc(sizeof(dev_file_arg_t));
	arg->port = port;
	return vfs_add_entry_r(r, dev_driver, arg);
}
