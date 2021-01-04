/**
 * \file system/dev/dev_driver.c
 *
 * Generic Serial Device driver
 *
 * Contains temporary stubs for the file system to allow compilation under gcc 9.2
 * This is temporary and should be removed as part of #184
 *
 * Copyright (c) 2017-2020, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <errno.h>
#include <sys/stat.h>

int chdir(const char* path) {
	errno = ENOSYS;
	return -1;
}

int mkdir(const char* pathname, mode_t mode) {
	errno = ENOSYS;
	return -1;
}

int chmod(const char* pathname, mode_t mode) {
	errno = ENOSYS;
	return -1;
}

long pathconf(const char* path, int name) {
	errno = ENOSYS;
	return -1;
}

char* getcwd(char* buf, size_t size) {
	errno = ENOSYS;
	return NULL;
}

int _unlink(const char* name) {
	errno = ENOSYS;
	return -1;
}

int _link(const char* old, const char* new) {
	errno = ENOSYS;
	return -1;
}

int _stat(const char* file, struct stat* st) {
	errno = ENOSYS;
	return -1;
}
