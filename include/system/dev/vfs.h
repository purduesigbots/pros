/**
 * \file system/dev/vfs.h
 *
 * Virtual File System header
 *
 * See system/dev/vfs.c for discussion
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <errno.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>

struct fs_driver {
	ssize_t (*read_r)(struct _reent*, void* const, uint8_t*, const size_t);
	int (*write_r)(struct _reent*, void* const, const uint8_t*, const size_t);
	int (*close_r)(struct _reent*, void* const);
	int (*fstat_r)(struct _reent*, void* const, struct stat*);
	int (*isatty_r)(struct _reent*, void* const);
	off_t (*lseek_r)(struct _reent*, void* const, off_t, int);
	int (*ctl)(void* const, const uint32_t, void* const);
};

struct file_entry {
	struct fs_driver const* driver;
	void* arg;
};

// adds an entry to the file table
int vfs_add_entry_r(struct _reent* r, struct fs_driver const* const driver, void* arg);

// update an entry to the file table. Returns -1 if there was an error.
// If driver is NULL, then the driver isn't updated. If arg is (void*)-1, then
// the arg isn't updated.
int vfs_update_entry(int file, struct fs_driver const* const driver, void* arg);
