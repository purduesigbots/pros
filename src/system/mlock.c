/**
 * \file system/mlock.c
 *
 * memory lock newlib stubs
 *
 * Contains implementations of memory-locking functions for newlib.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "rtos/task.h"

 void __malloc_lock(void) {
 	rtos_suspend_all();
 }

 void __malloc_unlock(void) {
 	rtos_resume_all();
 }
