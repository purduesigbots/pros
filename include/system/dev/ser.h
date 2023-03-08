/**
 * \file system/dev/ser.h
 *
 * Serial driver header
 *
 * See system/dev/ser_driver.c and system/dev/ser_daemon.c for discussion
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "vfs.h"

extern const struct fs_driver* const ser_driver;
int ser_open_r(struct _reent* r, const char* path, int flags, int mode);
void ser_initialize(void);
