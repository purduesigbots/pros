/**
 * \file tests/rtos_function_linking.c
 *
 * Test that all the FreeRTOS functions link correctly.
 *
 * Do not actually run this test - just make sure it compiles
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "api.h"

void x(void* ign) {}
void test_all() {
	task_delete(task_create(x, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "no name"));
	task_delay(1);

	task_delay_until(NULL, 3);

	task_get_priority(NULL);

	task_set_priority(NULL, 4);

	task_get_state(NULL);

	task_suspend(NULL);
	task_resume(NULL);

	task_get_count();

	task_get_name(NULL);

	task_get_by_name("Hello");

	task_notify(NULL);

	task_notify_ext(NULL, 4, E_NOTIFY_ACTION_NONE, NULL);

	task_notify_take(false, MAX_TIMEOUT);

	task_notify_clear(NULL);

	mutex_t mutex = mutex_create();
	mutex_take(mutex, MAX_TIMEOUT);
	mutex_give(mutex);

	sem_t sem = sem_create(4, 0);
	sem_wait(sem, MAX_TIMEOUT);
	sem_post(sem);
}
