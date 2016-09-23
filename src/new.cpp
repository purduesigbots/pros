/*
 * new.cpp - operator new (nothrow = no-exceptions) and delete for C++ support
 *
 * Copyright (c) 2011-2016, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include <stdlib.h>
#include <stdint.h>

void * operator new(size_t size) {
	return malloc(size);
}

void * operator new[](size_t size) {
	return malloc(size);
}

void operator delete(void * ptr) {
	free(ptr);
}

void operator delete[](void * ptr) {
	free(ptr);
}

void __cxa_pure_virtual(void) {
}
