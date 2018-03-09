/**
 * vfs.c - Virtual File System
 *
 * VFS is responsible for maintaining the global file table and routing all basic
 * I/O to the appropriate driver. There are two drivers implemented, ser and usd
 * which correspond to the serial driver and microSD card, respectively.
 *
 * VFS implements all of the I/O newlib stubs like open/read/write and delegates
 * them to the file's driver. Drivers don't actually have any knowledge of the
 * fileno. A file number maps to a driver and driver argument, which would be
 * whatever metadata the driver needs to open the file
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <errno.h>
#include <stdint.h>
#include <unistd.h>

#include "common/gid.h"
#include "common/string.h"
#include "ifi/v5_api.h"
#include "kapi.h"
#include "system/dev/ser.h"
#include "system/dev/vfs.h"

#define MAX_FILELEN 128
#define MAX_FILES_OPEN 31

#define RESERVED_FILENOS 4  // reserve stdin, stdout, stderr, kdbg

// gids bitmap buffer
static uint32_t file_table_bmp[gid_size_to_words(MAX_FILES_OPEN)];
// the gid structure itself
static struct gid_metadata file_table_gids = {.bitmap = file_table_bmp,
	                                      .max = MAX_FILES_OPEN,
	                                      .reserved = RESERVED_FILENOS,
	                                      .bitmap_size = gid_size_to_words(MAX_FILES_OPEN) };

// file table mapping a file descriptor number to a driver and driver argument
static struct file_entry file_table[MAX_FILES_OPEN];

void vfs_initialize(void) {
	gid_init(&file_table_gids);

	ser_initialize();
}

// adds an entry to the the file system
int vfs_add_entry_r(struct _reent* r, struct fs_driver const* const driver, void* arg) {
	uint32_t gid = gid_alloc(&file_table_gids);
	if (gid == 0) {
		r->_errno = ENFILE;
		return -1;
	}

	file_table[gid].driver = driver;
	file_table[gid].arg = arg;
	return gid;
}

// update a given fileno driver and arg. Used by ser_driver_initialize to initialize stdout, stdin, stderr, and kdbg
int vfs_update_entry(int file, struct fs_driver const* const driver, void* arg) {
	if (file < 0 || !gid_check(&file_table_gids, file)) {
		kprintf("BAD vfs update %d", file);
		return -1;
	}
	if (driver != NULL) {
		file_table[file].driver = driver;
	}
	if (arg != (void*)-1) {
		file_table[file].arg = arg;
	}
	return 0;
}

int _open_r(struct _reent* r, const char* file, int flags, int mode) {
	// Check if the filename is too long or not NULL terminated
	size_t i = 0;
	for (i = 0; i < MAX_FILELEN; i++) {
		if (file[i] == '\0') {
			break;
		}
	}
	if (i == MAX_FILELEN) {
		r->_errno = ENAMETOOLONG;
		return -1;
	}
	if (strstr(file, "/ser") == file) {
		// is a serial pseudofile
		return ser_open_r(r, file + strlen("/ser"), flags, mode);
	} else if (strstr(file, "/usd") == file) {
		r->_errno = ENOSYS;  // not yet implemented
		return -1;
	}

	r->_errno = ENOENT;
	return -1;
}

ssize_t _write_r(struct _reent* r, int file, const void* buf, size_t len) {
	if (file < 0 || !gid_check(&file_table_gids, file)) {
		r->_errno = EBADF;
		kprintf("BAD write %d", file);
		return -1;
	}
	return file_table[file].driver->write_r(r, file_table[file].arg, buf, len);
}

ssize_t _read_r(struct _reent* r, int file, void* buf, size_t len) {
	if (file < 0 || !gid_check(&file_table_gids, file)) {
		r->_errno = EBADF;
		kprintf("BAD read %d", file);
		return -1;
	}
	return file_table[file].driver->read_r(r, file_table[file].arg, buf, len);
}

int _close_r(struct _reent* r, int file) {
	// NOTE: newlib automatically closes all open files for a given task when
	// the task is deleted.
	if (file > 0 && file < RESERVED_FILENOS) {
		// Do not close the reserved file handles
		return 0;
	}
	if (file < 0 || !gid_check(&file_table_gids, file)) {
		r->_errno = EBADF;
		kprintf("BAD close %d", file);
		return -1;
	}
	int ret = file_table[file].driver->close_r(r, file_table[file].arg);
	if (ret == 0) {
		gid_free(&file_table_gids, file);
	}
	return ret;
}

int _fstat_r(struct _reent* r, int file, struct stat* st) {
	if (file < 0 || !gid_check(&file_table_gids, file)) {
		r->_errno = EBADF;
		kprintf("BAD fstat %d", file);
		return -1;
	}
	return file_table[file].driver->fstat_r(r, file_table[file].arg, st);
}

off_t _lseek_r(struct _reent* r, int file, off_t ptr, int dir) {
	if (file < 0 || !gid_check(&file_table_gids, file)) {
		r->_errno = EBADF;
		kprintf("BAD lseek %d", file);
		return -1;
	}
	return file_table[file].driver->lseek_r(r, file_table[file].arg, ptr, dir);
}

int _isatty_r(struct _reent* r, int file) {
	if (file < 0 || !gid_check(&file_table_gids, file)) {
		r->_errno = EBADF;
		kprintf("BAD isatty %d", file);
		return -1;
	}
	return file_table[file].driver->isatty_r(r, file_table[file].arg);
}

int32_t fdctl(int file, const uint32_t action, void* const extra_arg) {
	if (file < 0 || !gid_check(&file_table_gids, file)) {
		errno = EBADF;
		return -1;
	}
	return file_table[file].driver->ctl(file_table[file].arg, action, extra_arg);
}