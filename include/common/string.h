/**
 * \file common/string.h
 *
 * Extra string functions header
 *
 * See common/string.c for discussion
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

/**
 * strdup but uses the kernel heap
 *
 * \param s
 *        Pointer to the string to duplicate
 *
 * \return The duplicate string
 */
char* kstrdup(const char* s);

/**
 * strndup but uses the kernel heap
 *
 * \param s
 *        Pointer to the string to duplicate
 * \param n
 *        The number of characters to duplicate
 *
 * \return The duplicate string
 */
char* kstrndup(const char* s, size_t n);
