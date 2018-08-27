/**
 * \file pros/rtos.hpp
 *
 * Contains declarations for the PROS RTOS kernel for use by typical VEX
 * programmers.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/multitasking.html to
 * learn more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_RTOS_HPP_
#define _PROS_RTOS_HPP_

#include "pros/rtos.h"
#undef delay
#include <cstdint>
#include <cstdlib>

namespace pros {
class Task {
	public:
	/**
	 * Creates a new task and add it to the list of tasks that are ready to run.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENOMEM - The stack cannot be used as the TCB was not created.
	 *
	 * \param function
	 *        Pointer to the task entry function
	 * \param parameters
	 *        Pointer to memory that will be used as a parameter for the task
	 *        being created. This memory should not typically come from stack,
	 *        but rather from dynamically (i.e., malloc'd) or statically
	 *        allocated memory.
	 * \param prio
	 *        The priority at which the task should run.
	 *        TASK_PRIO_DEFAULT plus/minus 1 or 2 is typically used.
	 * \param stack_depth
	 *        The number of words (i.e. 4 * stack_depth) available on the task's
	 *        stack. TASK_STACK_DEPTH_DEFAULT is typically sufficienct.
	 * \param name
	 *        A descriptive name for the task.  This is mainly used to facilitate
	 *        debugging. The name may be up to 32 characters long.
	 *
	 */
	Task(task_fn_t function, void* parameters = NULL, std::uint32_t prio = TASK_PRIORITY_DEFAULT,
	     std::uint16_t stack_depth = TASK_STACK_DEPTH_DEFAULT, const char* name = "");
	/**
	 * Creates a new task and add it to the list of tasks that are ready to run.
	 *
	 * \param task
	 *        A task handle from task_create() for which to create a pros::Task
	 *        object.
	 */
	Task(task_t task);

	/**
	 * Creates a new task and add it to the list of tasks that are ready to run.
	 *
	 * \param in
	 *        A task handle from task_create() for which to create a pros::Task
	 *        object.
	 */
	void operator=(const task_t in);

	/**
	 * Gets the priority of the specified task.
	 *
	 * \return The priority of the task
	 */
	std::uint32_t get_priority(void);

	/**
	 * Sets the priority of the specified task.
	 *
	 * If the specified task's state is available to be scheduled (e.g. not
	 * blocked) and new priority is higher than the currently running task,
	 * a context switch may occur.
	 *
	 * \param prio
	 *        The new priority of the task
	 */
	void set_priority(std::uint32_t prio);

	/**
	 * Gets the state of the specified task.
	 *
	 * \return The state of the task
	 */
	std::uint32_t get_state(void);

	/**
	 * Suspends the specified task, making it ineligible to be scheduled.
	 */
	void suspend(void);

	/**
	 * Resumes the specified task, making it eligible to be scheduled.
	 *
	 * \param task
	 *        The task to resume
	 */
	void resume(void);

	/**
	 * Gets the name of the specified task.
	 *
	 * \return A pointer to the name of the task
	 */
	const char* get_name(void);

	/**
	 * Sends a simple notification to task and increments the notification
	 * counter.
	 *
	 * See https://pros.cs.purdue.edu/v5/tutorials/topical/notifications.html for
	 * details.
	 *
	 * \return Always returns true.
	 */
	std::uint32_t notify(void);

	/**
	 * Sends a notification to a task, optionally performing some action. Will
	 * also retrieve the value of the notification in the target task before
	 * modifying the notification value.
	 *
	 * See https://pros.cs.purdue.edu/v5/tutorials/topical/notifications.html for
	 * details.
	 *
	 * \param value
	 *        The value used in performing the action
	 * \param action
	 *        An action to optionally perform on the receiving task's notification
	 *        value
	 * \param prev_value
	 *        A pointer to store the previous value of the target task's
	 *        notification, may be NULL
	 *
	 * \return Dependent on the notification action.
	 * For NOTIFY_ACTION_NO_WRITE: return 0 if the value could be written without
	 * needing to overwrite, 1 otherwise.
	 * For all other NOTIFY_ACTION values: always return 0
	 */
	std::uint32_t notify_ext(std::uint32_t value, notify_action_e_t action, std::uint32_t* prev_value);

	/**
	 * Waits for a notification to be nonzero.
	 *
	 * See https://pros.cs.purdue.edu/v5/tutorials/topical/notifications.html for
	 * details.
	 *
	 * \param clear_on_exit
	 *        If true (1), then the notification value is cleared.
	 *        If false (0), then the notification value is decremented.
	 * \param timeout
	 *        Specifies the amount of time to be spent waiting for a notification
	 *        to occur.
	 *
	 * \return The value of the task's notification value before it is decremented
	 * or cleared
	 */
	std::uint32_t notify_take(bool clear_on_exit, std::uint32_t timeout);

	/**
	 * Clears the notification for a task.
	 *
	 * See https://pros.cs.purdue.edu/v5/tutorials/topical/notifications.html for
	 * details.
	 *
	 * \return False if there was not a notification waiting, true if there was
	 */
	bool notify_clear(void);

	/**
	 * Delays a task for a given number of milliseconds.
	 *
	 * This is not the best method to have a task execute code at predefined
	 * intervals, as the delay time is measured from when the delay is requested.
	 * To delay cyclically, use task_delay_until().
	 *
	 * \param milliseconds
	 *        The number of milliseconds to wait (1000 milliseconds per second)
	 */
	static void delay(const std::uint32_t milliseconds);

	/**
	 * Delays a task until a specified time.  This function can be used by
	 * periodic tasks to ensure a constant execution frequency.
	 *
	 * The task will be woken up at the time *prev_time + delta, and *prev_time
	 * will be updated to reflect the time at which the task will unblock.
	 *
	 * \param prev_time
	 *        A pointer to the location storing the setpoint time
	 * \param delta
	 *        The number of milliseconds to wait (1000 milliseconds per second)
	 */
	static void delay_until(std::uint32_t* const prev_time, const std::uint32_t delta);

	/**
	 * Gets the number of tasks the kernel is currently managing, including all
	 * ready, blocked, or suspended tasks. A task that has been deleted, but not
	 * yet reaped by the idle task will also be included in the count.
	 * Tasks recently created may take one context switch to be counted.
	 *
	 * \return The number of tasks that are currently being managed by the kernel.
	 */
	static std::uint32_t get_count(void);

	private:
	task_t task;
};

class Mutex {
	public:
	Mutex(void);

	/**
	 * Takes and locks a mutex, waiting for up to a certain number of milliseconds
	 * before timing out.
	 *
	 * See
	 * https://pros.cs.purdue.edu/v5/tutorials/topical/multitasking.html#mutexes
	 * for details.
	 *
	 * \param timeout
	 *        Time to wait before the mutex becomes available. A timeout of 0 can
	 *        be used to poll the mutex. TIMEOUT_MAX can be used to block
	 *        indefinitely.
	 *
	 * \return True if the mutex was successfully taken, false otherwise. If false
	 * is returned, then errno is set with a hint about why the the mutex
	 * couldn't be taken.
	 */
	bool take(std::uint32_t timeout);

	/**
	 * Unlocks a mutex.
	 *
	 * See
	 * https://pros.cs.purdue.edu/v5/tutorials/topical/multitasking.html#mutexes
	 * for details.
	 *
	 * \return True if the mutex was successfully returned, false otherwise. If
	 * false is returned, then errno is set with a hint about why the mutex
	 * couldn't be returned.
	 */
	bool give(void);

	private:
	mutex_t mutex;
};

/**
 * Gets the number of milliseconds since PROS initialized.
 *
 * \return The number of milliseconds since PROS initialized
 */
using pros::c::millis;

/**
 * Delays a task for a given number of milliseconds.
 *
 * This is not the best method to have a task execute code at predefined
 * intervals, as the delay time is measured from when the delay is requested.
 * To delay cyclically, use task_delay_until().
 *
 * \param milliseconds
 *        The number of milliseconds to wait (1000 milliseconds per second)
 */
using pros::c::delay;
}  // namespace pros

#endif  // _PROS_RTOS_HPP_s
