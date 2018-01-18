/**
 * cpp_support.cpp - C++ support hooks
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstddef>
#include <cstdlib>
#include <stdexcept>

#include "ifi/v5_api.h"

#include "rtos/FreeRTOS.h"
#include "rtos/task.h"

void* operator new(size_t size) {
	return malloc(size);
}

void* operator new[](size_t size) {
	return malloc(size);
}

void operator delete(void* p) {
	free(p);
}

void operator delete[](void* p) {
	free(p);
}

extern "C" void task_fn_wrapper(task_fn_t fn, void* args) {
#ifdef __cpp_exceptions
	try {
#endif
		fn(args);
#ifdef __cpp_exceptions
	} catch (std::runtime_error re) {
		vexDisplayString(7, "caught runtime error: %s", re.what());
	} catch (...) {
		vexDisplayString(7, "caught an unknown error");
	}
#endif
}

/******************************************************************************/
/**                        C++ Linkages for User Tasks                       **/
/******************************************************************************/
__attribute__((weak)) void autonomous() {}
__attribute__((weak)) void initialize() {}
__attribute__((weak)) void opcontrol() {}
__attribute__((weak)) void disabled() {}
__attribute__((weak)) void competition_initialize() {}

extern "C" void cpp_autonomous() {
	autonomous();
}
extern "C" void cpp_initialize() {
	initialize();
}
extern "C" void cpp_opcontrol() {
	opcontrol();
}
extern "C" void cpp_disabled() {
	disabled();
}
extern "C" void cpp_competition_initialize() {
	competition_initialize();
}
