/*
 * invoke.h - Handles calls into user code from the kernel
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

#ifndef INVOKE_H_
#define INVOKE_H_

// Begin C++ extern to C
#ifdef __cplusplus
extern "C" {
#endif

// invDisable - Disables the robot and stops autonomous / teleop
void invDisable();
// invStartAuto - Starts the autonomous task if not already running; kills teleop if running
void invStartAuto();
// invStartInit - Starts the init task; must not be called from an ISR!
void invStartInit();
// invStartTeleop - Starts the teleop task if it is not already running; kills auto if running
void invStartTeleop();
// invWakeup - Wakes up from an ISR
void invWakeup();

// autonomous - runs user autonomous code
void autonomous();
// initializeIO - called before the kernel is started, nothing should be in this function unless
// one explicitly knows that it has to be here [it is very self-evident when this is the case]
void initializeIO();
// initialize - initializes user sensors, run once after the kernel is started, possibly while
// still looking for a VexNET connection; most initialization should be here [e.g. InitGyro]
void initialize();
// operatorControl - runs user control code
void operatorControl();

// End C++ extern to C
#ifdef __cplusplus
}
#endif

#endif
