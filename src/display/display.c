/*
 * \file display/display.c
 *
 * Main source code for interacting with the V5 Brain's LCD screen.
 *
 * Copyright (c) 2017-2022, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "kapi.h"
#include "v5_api.h"

static task_stack_t disp_daemon_task_stack[TASK_STACK_DEPTH_DEFAULT];
static static_task_s_t disp_daemon_task_buffer;
static task_t disp_daemon_task;

static void disp_daemon(void* ign) {
	uint32_t time = millis();
	while (true) {
		task_delay_until(&time, 2);
	}
}

static void vex_display_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
	vexDisplayCopyRect(x1, y1, x2, y2, (uint32_t*)color, x2 - x1 + 1);
}

