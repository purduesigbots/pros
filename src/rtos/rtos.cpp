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
  Task::Task(task_fn_t function, void* parameters,
            uint32_t prio,
            uint16_t stack_depth,
            const char* name) {
    task = task_create(function, parameters, prio, stack_depth, name);
  }
  Task::Task(task_t task) : task(task) { }
  void Task::operator = (task_t const in) {
    task = in;
  }
  Task::~Task() {
    task_delete(task);
  }

  uint32_t Task::get_priority() {
    return task_get_priority(task);
  }

  void Task::set_priority(uint32_t prio) {
    task_set_priority(task, prio);
  }

  uint32_t Task::get_state() {
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

  uint32_t Task::notify() {
    return task_notify(task);
  }

  uint32_t Task::notify_ext(uint32_t value, notify_action_e_t action, uint32_t* prev_value) {
    return task_notify_ext(task, value, action, prev_value);
  }

  uint32_t Task::notify_take(bool clear_on_exit, uint32_t timeout) {
    return task_notify_take(clear_on_exit, timeout);
  }

  bool Task::notify_clear() {
    return task_notify_clear(task);
  }

  void Task::delay(const uint32_t milliseconds) {
    task_delay(milliseconds);
  }

  void Task::delay_until(uint32_t* const prev_time, const uint32_t delta) {
    task_delay_until(prev_time, delta);
  }

  Mutex::Mutex() : mutex(mutex_create()) { }

  bool Mutex::take(uint32_t timeout) {
    return mutex_take(mutex, timeout);
  }

  bool Mutex::give() {
    return mutex_give(mutex);
  }

  uint32_t millis(void) {
    return millis();
  }
}
