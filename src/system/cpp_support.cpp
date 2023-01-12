/**
 * \file system/cpp_support.cpp
 *
 * C++ support hooks
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstddef>
#include <cstdlib>
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
		fprintf(stderr, "Runtime error: %s \n", re.what());
		vexDisplayString(5, "A runtime error occurred:");
		vexDisplayString(6, "%s", re.what());
		vexDisplayString(7, "Note: open terminal for error message");
	} catch (const std::exception& ex) {
		fprintf(stderr, "Exception occurred: %s \n", ex.what());
		vexDisplayString(5, "An exception occurred:");
		vexDisplayString(6, "%s", ex.what());
		vexDisplayString(7, "Note: open terminal for error message");
	} catch (...) {
		fprintf(stderr, "Unknown error occurred. \n");
		vexDisplayString(5, "An unknown error occurred");
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
