/*
 * invoke.c - Used to invoke the user tasks; all bridges of control flow from OS to usercode
 * should go in this file
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

#include <invoke.h>
#include <kernel.h>
#include <periph.h>
#include <supervisor.h>
#include <task.h>

// Delay in ms before daemon task starts
#define STARTUP_DELAY 1536

// State flags for the state variable
#define AUTO_RUNNING 1
#define TELEOP_RUNNING 2
#define INIT_RUNNING 4

// State flags to indicate what is running
static volatile uint32_t state = 0;
// References to the autonomous, teleop, and init tasks (if running)
static TaskHandle taskAutonomous;
static TaskHandle taskInit;
static TaskHandle taskTeleop;
static TaskHandle taskDaemon;

// Invoke autonomous
static void autonomous_task(void *ignore) {
	autonomous();
	_enterCritical();
	{
		// Clean up
		motorControlStop();
		state &= ~AUTO_RUNNING;
		taskAutonomous = NULL;
	}
	_exitCritical();
}

// Run VEX daemon
static void daemon_task(void *ignore) {
	// We must wait for about 1 s to limit start up glitches before running serious stuff
	uint16_t flags;
	while (1) {
		flags = svFlags;
		if (flags & SV_FMS) {
			// On-line, read VEX mode control
			if (!(flags & SV_ENABLED))
				invDisable();
			else if (flags & SV_AUTONOMOUS)
				invStartAuto();
			else
				invStartTeleop();
			// We will be woken on the next mode change
		} else
			// Not on-line, start teleop
			invStartTeleop();
		taskSuspend(NULL);
	}
}

// Invoke initialize
static void initialize_task(void *ignore) {
	clock_t startTime = timeLowRes() + STARTUP_DELAY, now;
	initialize();
	_enterCritical();
	{
		// Clean up
		state &= ~INIT_RUNNING;
		taskInit = NULL;
	}
	_exitCritical();
	// Wait if necessary
	now = timeLowRes();
	if (now < startTime)
		taskDelay(startTime - now);
	// Start the daemon task, which runs above autonomous() and operatorControl() but is
	// almost always suspended during runs
	taskDaemon = taskCreate(daemon_task, TASK_MINIMAL_STACK_SIZE, NULL, TASK_PRIORITY_HIGHEST);
	if (taskDaemon == NULL)
		_exit(STATUS_CANT_START_SYSTEM);
}

// Invoke operator control
static void opcontrol_task(void *ignore) {
	operatorControl();
	_enterCritical();
	{
		// Clean up
		motorControlStop();
		state &= ~TELEOP_RUNNING;
		taskTeleop = NULL;
	}
	_exitCritical();
}

// invDisable - Disables the robot and stops autonomous / teleop
void invDisable() {
	uint32_t s = state;
	taskSuspendAll();
	if (s & AUTO_RUNNING) {
		// Kill autonomous
		taskDelete(taskAutonomous);
		taskAutonomous = NULL;
	}
	if (s & TELEOP_RUNNING) {
		// Kill teleop
		taskDelete(taskTeleop);
		taskTeleop = NULL;
	}
	state = s & ~(AUTO_RUNNING | TELEOP_RUNNING);
	motorControlStop();
	taskResumeAll();
}

// invStartAuto - Starts the autonomous task if not already running; kills teleop if running
// Must not be called from an ISR!
void invStartAuto() {
	uint32_t s = state;
	if (!(s & AUTO_RUNNING)) {
		taskSuspendAll();
		if (s & TELEOP_RUNNING) {
			// Kill teleop
			taskDelete(taskTeleop);
			taskTeleop = NULL;
			motorControlStop();
			s &= ~TELEOP_RUNNING;
		}
		taskAutonomous = taskCreate(autonomous_task, TASK_DEFAULT_STACK_SIZE, NULL,
			TASK_PRIORITY_DEFAULT);
		if (taskAutonomous == NULL)
			_exit(STATUS_CANT_START_SYSTEM);
		// Start autonomous running
		state = s | AUTO_RUNNING;
		taskResumeAll();
	}
}

// invStartInit - Starts the init task; must not be called from an ISR!
void invStartInit() {
	state = 0;
	taskInit = taskCreate(initialize_task, TASK_DEFAULT_STACK_SIZE, NULL,
		TASK_PRIORITY_DEFAULT);
	if (taskInit == NULL)
		_exit(STATUS_CANT_START_SYSTEM);
	// Start init running
	state |= INIT_RUNNING;
}

// invStartTeleop - Starts the teleop task if it is not already running; kills auto if running
// Must not be called from an ISR!
void invStartTeleop() {
	uint32_t s = state;
	if (!(s & TELEOP_RUNNING)) {
		taskSuspendAll();
		if (s & AUTO_RUNNING) {
			// Kill autonomous
			taskDelete(taskAutonomous);
			taskAutonomous = NULL;
			motorControlStop();
			s &= ~AUTO_RUNNING;
		}
		taskTeleop = taskCreate(opcontrol_task, TASK_DEFAULT_STACK_SIZE, NULL,
			TASK_PRIORITY_DEFAULT);
		if (taskTeleop == NULL)
			_exit(STATUS_CANT_START_SYSTEM);
		// Start teleop running
		state = s | TELEOP_RUNNING;
		taskResumeAll();
	}
}

// invWakeup - Wakes up the daemon task from an ISR
void invWakeup() {
	if (taskDaemon != NULL)
		taskResumeISR(taskDaemon);
}

// Aliases to stop undefined references from confusing users if they delete operatorControl(),
// autonomous(), initialize(), etc.
#define WEAK __attribute__((weak))

WEAK void operatorControl() {
	taskSuspend(NULL);
}
WEAK void autonomous() { }
WEAK void initialize() { }
WEAK void initializeIO() { }
