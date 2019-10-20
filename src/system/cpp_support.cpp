/**
 * \file system/cpp_support.cpp
 *
 * C++ support hooks
 *
 * Copyright (c) 2017-2019, Purdue University ACM SIGBots
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "rtos/FreeRTOS.h"
#include "rtos/task.h"
#include "v5_api.h"

extern "C" void task_fn_wrapper(task_fn_t fn, void* args) {
#ifdef __cpp_exceptions
	try {
#endif
		fn(args);
#ifdef __cpp_exceptions
	} catch (const std::runtime_error& re) {
		std::cerr << "Runtime error: " << re.what() << std::endl;
		vexDisplayString(5, "an runtime error occured:");
		vexDisplayString(6, "%s", re.what());
		vexDisplayString(7, "open terminal to see full error message");
	} catch (const std::exception& ex) {
		std::cerr << "Exception occurred: " << ex.what() << std::endl;
		vexDisplayString(5, "an exception occured:");
		vexDisplayString(6, "%s", ex.what());
		vexDisplayString(7, "open terminal to see full error message");
	} catch (...) {
		std::cerr << "Unknown error occurred. Possible memory corruption" << std::endl;
		vexDisplayString(5, "an unknown error occurred");
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
