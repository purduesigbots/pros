/**
 * system/common/string.h - Extra string functions header
 *
 * See system/common/string.c for discussion
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 * All rights reservered.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

/**
 * strdup but uses the kernel heap
 */
char* kstrdup(const char* s);

/**
 * strndup but uses the kernel heap
 */
char* kstrndup(const char* s, size_t n);