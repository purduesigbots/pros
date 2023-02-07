/**
 * \file system/dev/usd_driver.c
 *
 * Contains the driver for writing files to the microSD card.
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
#include "system/dev/usd.h"
#include "system/dev/vfs.h"
#include "system/optimizers.h"
#include "v5_api.h"

typedef struct usd_file_arg {
	FIL* ifi_fptr;
} usd_file_arg_t;

static const int FRESULTMAP[] = {0,       EIO,    EINVAL, EBUSY, ENOENT,  ENOENT, EINVAL, EACCES,  // FR_DENIED
                                 EEXIST,  EINVAL, EROFS,  ENXIO, ENOBUFS, ENXIO,  EIO,    EACCES,  // FR_LOCKED
                                 ENOBUFS, ENFILE, EINVAL};
enum fa_flags {
	FA_READ = 1 << 0,
	FA_WRITE = 1 << 1,
	FA_CREATE_ALWAYS = 1 << 2,
	FA_OPEN_APPEND = 1 << 3,
	FA_CREATE_NEW = 1 << 4
};

/******************************************************************************/
/**                         newlib driver functions                          **/
/******************************************************************************/
int usd_read_r(struct _reent* r, void* const arg, uint8_t* buffer, const size_t len) {
	usd_file_arg_t* file_arg = (usd_file_arg_t*)arg;
	// TODO: mutex here. Global or file lock?
	int32_t result = vexFileRead((char*)buffer, sizeof(*buffer), len, file_arg->ifi_fptr);
	return result;
}

int usd_write_r(struct _reent* r, void* const arg, const uint8_t* buf, const size_t len) {
	usd_file_arg_t* file_arg = (usd_file_arg_t*)arg;
	// TODO: mutex here. Global or file lock?
	int32_t result = vexFileWrite((char*)buf, sizeof(*buf), len, file_arg->ifi_fptr);
	return result;
}

int usd_close_r(struct _reent* r, void* const arg) {
	usd_file_arg_t* file_arg = (usd_file_arg_t*)arg;
	vexFileClose(file_arg->ifi_fptr);
	return 0;
}

int usd_fstat_r(struct _reent* r, void* const arg, struct stat* st) {
	usd_file_arg_t* file_arg = (usd_file_arg_t*)arg;
	st->st_size = vexFileSize(file_arg->ifi_fptr);
	return 0;
}

int usd_isatty_r(struct _reent* r, void* const arg) {
	return 0;
}

off_t usd_lseek_r(struct _reent* r, void* const arg, off_t ptr, int dir) {
	usd_file_arg_t* file_arg = (usd_file_arg_t*)arg;
	// TODO: mutex here. Global or file lock?
	FRESULT result = vexFileSeek(file_arg->ifi_fptr, ptr, dir);
	if (result != FR_OK) {
		r->_errno = FRESULTMAP[result];
		return (off_t)-1;
	}
	return vexFileTell(file_arg->ifi_fptr);
}

int usd_ctl(void* const arg, const uint32_t cmd, void* const extra_arg) {
	return 0;
}

/******************************************************************************/
/**                           Driver description                             **/
/******************************************************************************/

const struct fs_driver _usd_driver = {.close_r = usd_close_r,
                                      .fstat_r = usd_fstat_r,
                                      .isatty_r = usd_isatty_r,
                                      .lseek_r = usd_lseek_r,
                                      .read_r = usd_read_r,
                                      .write_r = usd_write_r,
                                      .ctl = usd_ctl};
const struct fs_driver* const usd_driver = &_usd_driver;

int usd_open_r(struct _reent* r, const char* path, int flags, int mode) {
	FRESULT result = vexFileMountSD();
	if (result != F_OK) {
		r->_errno = FRESULTMAP[result];
		return -1;
	}

	usd_file_arg_t* file_arg = kmalloc(sizeof(*file_arg));

	switch (flags & O_ACCMODE) {
		case O_RDONLY:
			file_arg->ifi_fptr = vexFileOpen(path, "");  // mode is ignored
			break;
		case O_WRONLY:
			if (flags & O_APPEND) {
				file_arg->ifi_fptr = vexFileOpenWrite(path);
			} else {
				file_arg->ifi_fptr = vexFileOpenCreate(path);
			}
			break;
		default:
			r->_errno = EINVAL;
			return -1;
	}

	if (!file_arg->ifi_fptr) {
		r->_errno = ENFILE;  // up to 8 files max as of vexOS 0.7.4b55
		return -1;
	}
	return vfs_add_entry_r(r, usd_driver, file_arg);
}
