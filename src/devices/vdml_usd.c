/**
 * \file devices/usd.c
 *
 * Contains functions for interacting with the SD card.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "kapi.h"
#include "v5_api.h"

int32_t usd_is_installed(void) {
	return vexFileDriveStatus(0);
}
static const int FRESULTMAP[] = {0,       EIO,    EINVAL, EBUSY, ENOENT,  ENOENT, EINVAL, EACCES,  // FR_DENIED
                                 EEXIST,  EINVAL, EROFS,  ENXIO, ENOBUFS, ENXIO,  EIO,    EACCES,  // FR_LOCKED
                                 ENOBUFS, ENFILE, EINVAL};

int32_t usd_list_files(const char* path, char* buffer, int32_t len) {
	FRESULT result =  vexFileDirectoryGet(path, buffer, len);
	if (result != F_OK) {
		errno = FRESULTMAP[result];
		return PROS_ERR;
	}
	return PROS_SUCCESS;
}
