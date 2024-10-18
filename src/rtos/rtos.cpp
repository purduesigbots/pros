/**
 * \file rtos/rtos.cpp
 *
 * Contains functions for the PROS RTOS kernel for use by typical
 * VEX programmers.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "pros/rtos.hpp"

#include <atomic>
#include <system_error>

#include "kapi.h"
#include "pros/rtos.h"

namespace pros {
using namespace pros::c;

Task::Task(task_fn_t function, void* parameters, std::uint32_t prio, std::uint16_t stack_depth, const char* name) {
	task = task_create(function, parameters, prio, stack_depth, name);
}

Task::Task(task_fn_t function, void* parameters, const char* name)
    : Task(function, parameters, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, name) {}

Task::Task(task_t task) : task(task) {}
Task& Task::operator=(const task_t in) {
	task = in;
	return *this;
}

Task Task::current() {
	return Task{task_get_current()};
}

void Task::remove() {
	return task_delete(task);
}

std::uint32_t Task::get_priority() {
	return task_get_priority(task);
}

void Task::set_priority(std::uint32_t prio) {
	task_set_priority(task, prio);
}

std::uint32_t Task::get_state() {
	return task_get_state(task);
}

void Task::suspend() {
	task_suspend(task);
}

void Task::resume() {
	task_resume(task);
}

const char* Task::get_name() {
	return task_get_name(task);
}

std::uint32_t Task::notify() {
	return task_notify(task);
}

void Task::join() {
	return task_join(task);
}

std::uint32_t Task::notify_ext(std::uint32_t value, notify_action_e_t action, std::uint32_t* prev_value) {
	return task_notify_ext(task, value, action, prev_value);
}

std::uint32_t Task::notify_take(bool clear_on_exit, std::uint32_t timeout) {
	return task_notify_take(clear_on_exit, timeout);
}

bool Task::notify_clear() {
	return task_notify_clear(task);
}

void Task::delay(const std::uint32_t milliseconds) {
	task_delay(milliseconds);
}

void Task::delay_until(std::uint32_t* const prev_time, const std::uint32_t delta) {
	task_delay_until(prev_time, delta);
}

std::uint32_t Task::get_count() {
	return task_get_count();
}

Clock::time_point Clock::now() {
	return Clock::time_point{Clock::duration{millis()}};
}

mutex_t Mutex::lazy_init() {
		mutex_t _mutex;
		if(unlikely((_mutex = mutex.load(std::memory_order::relaxed)) == nullptr)) {
			taskENTER_CRITICAL();
			if(likely((_mutex = mutex.load()) == nullptr)) {	
				mutex.store((_mutex = pros::c::mutex_create()));
			}
			taskEXIT_CRITICAL();
		}
		return _mutex;
}

bool Mutex::take() {
	return mutex_take(lazy_init(), TIMEOUT_MAX);
}

bool Mutex::take(std::uint32_t timeout) {
	return mutex_take(lazy_init(), timeout);
}

bool Mutex::give() {
	return mutex_give(lazy_init());
}

void Mutex::lock() {
	if (!take(TIMEOUT_MAX)) {
		throw std::system_error(errno, std::system_category(), "Cannot obtain lock!");
	}
}

void Mutex::unlock() {
	give();
}

bool Mutex::try_lock() {
	return take(0);
}

Mutex::~Mutex() {
	mutex_t _mutex = mutex.exchange(reinterpret_cast<mutex_t>(~0));
	pros::c::mutex_delete(_mutex);
}

mutex_t RecursiveMutex::lazy_init() {
		mutex_t _mutex;
		if(unlikely((_mutex = mutex.load(std::memory_order::relaxed)) == nullptr)) {
			taskENTER_CRITICAL();
			if(likely((_mutex = mutex.load()) == nullptr)) {	
				mutex.store((_mutex = pros::c::mutex_recursive_create()));
			}
			taskEXIT_CRITICAL();
		}
		return _mutex;
}

bool RecursiveMutex::take() {
	return mutex_recursive_take(lazy_init(), TIMEOUT_MAX);
}

bool RecursiveMutex::take(std::uint32_t timeout) {
	return mutex_recursive_take(lazy_init(), timeout);
}

bool RecursiveMutex::give() {
	return mutex_recursive_give(lazy_init());
}

void RecursiveMutex::lock() {
	if (!take(TIMEOUT_MAX)) {
		throw std::system_error(errno, std::system_category(), "Cannot obtain lock!");
	}
}

void RecursiveMutex::unlock() {
	give();
}

bool RecursiveMutex::try_lock() {
	return take(0);
}

RecursiveMutex::~RecursiveMutex() {
	mutex_t _mutex = mutex.exchange(reinterpret_cast<mutex_t>(~0));
	pros::c::mutex_delete(_mutex);
}
}  // namespace pros
