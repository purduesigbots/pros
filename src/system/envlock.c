/**
 * \file system/envlock.c
 *
 * environment lock newlib stubs
 *
 * Contains implementations of environment-locking functions for newlib.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "rtos/task.h"

void __env_lock(void) {
	rtos_suspend_all();
}

void __env_unlock(void) {
	rtos_resume_all();
}
