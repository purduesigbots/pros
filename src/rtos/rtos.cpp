/**
 * pros/rtos.cpp
 *
 * Contains functions for the PROS RTOS kernel for use by typical
 * VEX programmers.
 *
 * See https://pros.cs.purdue.edu/v5/tutorials/multitasking to learn more.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 * All rights reservered.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "kapi.h"
#include "pros/rtos.hpp"

namespace pros {
using namespace pros::c;

  Task::Task(task_fn_t function, void* parameters,
            std::uint32_t prio,
            std::uint16_t stack_depth,
            const char* name) {
    task = task_create(function, parameters, prio, stack_depth, name);
  }
  Task::Task(task_t task) : task(task) { }
  void Task::operator = (const task_t in) {
    task = in;
  }
  Task::~Task(void) {
    task_delete(task);
  }

  std::uint32_t Task::get_priority(void) {
    return task_get_priority(task);
  }

  void Task::set_priority(std::uint32_t prio) {
    task_set_priority(task, prio);
  }

  std::uint32_t Task::get_state(void) {
    return task_get_state(task);
  }

  void Task::suspend(void) {
    task_suspend(task);
  }

  void Task::resume(void) {
    task_resume(task);
  }

  const char* Task::get_name(void) {
    return task_get_name(task);
  }

  std::uint32_t Task::notify(void) {
    return task_notify(task);
  }

  std::uint32_t Task::notify_ext(std::uint32_t value, notify_action_e_t action, std::uint32_t* prev_value) {
    return task_notify_ext(task, value, action, prev_value);
  }

  std::uint32_t Task::notify_take(bool clear_on_exit, std::uint32_t timeout) {
    return task_notify_take(clear_on_exit, timeout);
  }

  bool Task::notify_clear(void) {
    return task_notify_clear(task);
  }

  void Task::delay(const std::uint32_t milliseconds) {
    task_delay(milliseconds);
  }

  void Task::delay_until(std::uint32_t* const prev_time, const std::uint32_t delta) {
    task_delay_until(prev_time, delta);
  }

  std::uint32_t Task::get_count(void) {
    return task_get_count();
  }

  Mutex::Mutex(void) : mutex(mutex_create()) { }

  bool Mutex::take(std::uint32_t timeout) {
    return mutex_take(mutex, timeout);
  }

  bool Mutex::give(void) {
    return mutex_give(mutex);
  }
}
