/**
 * kernel.h - PROS Kernel private definitions (public kernel API is kept in API.h)
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


#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdint.h>
#include <invoke.h>
#include <sys/types.h>

// Begin C++ extern to C
#ifdef __cplusplus
extern "C" {
#endif

#define FW_VERSION_MAJOR "2"
#define FW_VERSION_MINOR "11"
#define FW_VERSION_PATCH "1"

#define FW_VERSION FW_VERSION_MAJOR "." FW_VERSION_MINOR "." FW_VERSION_PATCH

// PWM enable
//#define PWM_ENABLED

// No space left to start system task
#define STATUS_CANT_START_SYSTEM 4
// Stack overflow
#define STATUS_STACK_OVERFLOW 3
// Illegal instruction exception
#define STATUS_ILLEGAL_INSTRUCTION 2
// Hardware fault
#define STATUS_HARDFAULT 1
// Memory access or bus access violation
#define STATUS_SEGFAULT 0

// _exit - Aborts execution with the specified status code
void __attribute__ ((noreturn)) _exit(int status);
// initMCU - MCU startup routine, should not be called by anything other than startKernel
void initMCU();
// kwait - goes to sleep until the specified time elapses
void kwait(clock_t millis);
// startKernel - Start the PROS kernel, directly from the reset handler
void startKernel();
// timeHighRes - Gets the high-resolution tick count (microseconds)
// This will theoretically overflow every hour and a half or so, but the risk of this in VeX
// is pretty small. Any download, mode switch, or robot reset will reset the clock anyway
clock_t timeHighRes();
// timeLowRes - Gets the low-resolution tick count (milliseconds)
clock_t timeLowRes();

// End C++ extern to C
#ifdef __cplusplus
}
#endif

#endif
