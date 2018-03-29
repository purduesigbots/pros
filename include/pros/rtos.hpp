/**
 * \file pros/rtos.hpp
 *
 * \brief Contains declarations for the PROS RTOS kernel for use by typical
 * VEX programmers in C++.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/multitasking to learn more.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 * All rights reservered.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _PROS_RTOS_HPP_
#define _PROS_RTOS_HPP_

#include "pros/rtos.h"
#undef delay

namespace pros {
class Task {
	public:
	Task(task_fn_t function,
	     void* parameters = NULL,
	     std::uint32_t prio = TASK_PRIORITY_DEFAULT,
	     std::uint16_t stack_depth = TASK_STACK_DEPTH_DEFAULT,
	     const char* name = "");
	Task(task_t task);
	void operator=(const task_t in);
	virtual ~Task();

	std::uint32_t get_priority();
	void set_priority(std::uint32_t prio);
	std::uint32_t get_state();
	void suspend();
	void resume();
	const char* get_name();
	std::uint32_t notify();
	std::uint32_t notify_ext(std::uint32_t value, notify_action_e_t action, std::uint32_t* prev_value);
	std::uint32_t notify_take(bool clear_on_exit, std::uint32_t timeout);
	bool notify_clear();

	static void delay(const std::uint32_t milliseconds);
	static void delay_until(std::uint32_t* const prev_time, const std::uint32_t delta);

	private:
	task_t task;
};

class Mutex {
	public:
	Mutex();

	bool take(std::uint32_t timeout);
	bool give();

	private:
	mutex_t mutex;
};

std::uint32_t millis(void);
}

#endif
