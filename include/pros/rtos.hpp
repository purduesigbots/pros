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
 * Copyright (c) 2017-2022, Purdue University ACM SIGBots.
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
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <memory>
#include <optional>
#include <type_traits>

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
	Task(task_fn_t function, void* parameters = nullptr, std::uint32_t prio = TASK_PRIORITY_DEFAULT,
	     std::uint16_t stack_depth = TASK_STACK_DEPTH_DEFAULT, const char* name = "");

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
	 * \param name
	 *        A descriptive name for the task.  This is mainly used to facilitate
	 *        debugging. The name may be up to 32 characters long.
	 *
	 */
	Task(task_fn_t function, void* parameters, const char* name);

	/**
	 * Creates a new task and add it to the list of tasks that are ready to run.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENOMEM - The stack cannot be used as the TCB was not created.
	 *
	 * \param function
	 *        Callable object to use as entry function
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
	template <class F>
	static task_t create(F&& function, std::uint32_t prio = TASK_PRIORITY_DEFAULT,
	                     std::uint16_t stack_depth = TASK_STACK_DEPTH_DEFAULT, const char* name = "") {
		static_assert(std::is_invocable_r_v<void, F>);
		return pros::c::task_create(
		    [](void* parameters) {
			    std::unique_ptr<std::function<void()>> ptr{static_cast<std::function<void()>*>(parameters)};
			    (*ptr)();
		    },
		    new std::function<void()>(std::forward<F>(function)), prio, stack_depth, name);
	}

	/**
	 * Creates a new task and add it to the list of tasks that are ready to run.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENOMEM - The stack cannot be used as the TCB was not created.
	 *
	 * \param function
	 *        Callable object to use as entry function
	 * \param name
	 *        A descriptive name for the task.  This is mainly used to facilitate
	 *        debugging. The name may be up to 32 characters long.
	 *
	 */
	template <class F>
	static task_t create(F&& function, const char* name) {
		return Task::create(std::forward<F>(function), TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, name);
	}

	/**
	 * Creates a new task and add it to the list of tasks that are ready to run.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENOMEM - The stack cannot be used as the TCB was not created.
	 *
	 * \param function
	 *        Callable object to use as entry function
	 * \param prio
	 *        The priority at which the task should run.
	 *        TASK_PRIO_DEFAULT plus/minus 1 or 2 is typically used.
	 * \param stack_depth
	 *        The number of words (i.e. 4 * stack_depth) available on the task's
	 *        stack. TASK_STACK_DEPTH_DEFAULT is typically sufficient.
	 * \param name
	 *        A descriptive name for the task.  This is mainly used to facilitate
	 *        debugging. The name may be up to 32 characters long.
	 *
	 */
	template <class F>
	explicit Task(F&& function, std::uint32_t prio = TASK_PRIORITY_DEFAULT,
	              std::uint16_t stack_depth = TASK_STACK_DEPTH_DEFAULT, const char* name = "")
	    : Task(
	          [](void* parameters) {
		          std::unique_ptr<std::function<void()>> ptr{static_cast<std::function<void()>*>(parameters)};
		          (*ptr)();
	          },
	          new std::function<void()>(std::forward<F>(function)), prio, stack_depth, name) {
		static_assert(std::is_invocable_r_v<void, F>);
	}

	/**
	 * Creates a new task and add it to the list of tasks that are ready to run.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENOMEM - The stack cannot be used as the TCB was not created.
	 *
	 * \param function
	 *        Callable object to use as entry function
	 * \param name
	 *        A descriptive name for the task.  This is mainly used to facilitate
	 *        debugging. The name may be up to 32 characters long.
	 *
	 */
	template <class F>
	Task(F&& function, const char* name)
	    : Task(std::forward<F>(function), TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, name) {}

	/**
	 * Create a C++ task object from a task handle
	 *
	 * \param task
	 *        A task handle from task_create() for which to create a pros::Task
	 *        object.
	 */
	explicit Task(task_t task);

	/**
	 * Get the currently running Task
	 */
	static Task current();

	/**
	 * Creates a new task and add it to the list of tasks that are ready to run.
	 *
	 * \param in
	 *        A task handle from task_create() for which to create a pros::Task
	 *        object.
	 */
	Task& operator=(task_t in);

	/**
	 * Removes the Task from the RTOS real time kernel's management. This task
	 * will be removed from all ready, blocked, suspended and event lists.
	 *
	 * Memory dynamically allocated by the task is not automatically freed, and
	 * should be freed before the task is deleted.
	 */
	void remove();

	/**
	 * Gets the priority of the specified task.
	 *
	 * \return The priority of the task
	 */
	std::uint32_t get_priority();

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
	std::uint32_t get_state();

	/**
	 * Suspends the specified task, making it ineligible to be scheduled.
	 */
	void suspend();

	/**
	 * Resumes the specified task, making it eligible to be scheduled.
	 *
	 * \param task
	 *        The task to resume
	 */
	void resume();

	/**
	 * Gets the name of the specified task.
	 *
	 * \return A pointer to the name of the task
	 */
	const char* get_name();

	/**
	 * Convert this object to a C task_t handle
	 */
	explicit operator task_t() {
		return task;
	}

	/**
	 * Sends a simple notification to task and increments the notification
	 * counter.
	 *
	 * See https://pros.cs.purdue.edu/v5/tutorials/topical/notifications.html for
	 * details.
	 *
	 * \return Always returns true.
	 */
	std::uint32_t notify();

	/**
	 * Utilizes task notifications to wait until specified task is complete and deleted,
	 * then continues to execute the program. Analogous to std::thread::join in C++.
	 *
	 * See https://pros.cs.purdue.edu/v5/tutorials/topical/notifications.html for
	 * details.
	 *
	 * \return void
	 */
	void join();

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
	static std::uint32_t notify_take(bool clear_on_exit, std::uint32_t timeout);

	/**
	 * Clears the notification for a task.
	 *
	 * See https://pros.cs.purdue.edu/v5/tutorials/topical/notifications.html for
	 * details.
	 *
	 * \return False if there was not a notification waiting, true if there was
	 */
	bool notify_clear();

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
	 *        A pointer to the location storing the setpoint time. This should
	 *        typically be initialized to the return value from pros::millis().
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
	static std::uint32_t get_count();

	private:
	task_t task{};
};

// STL Clock compliant clock
struct Clock {
	using rep = std::uint32_t;
	using period = std::milli;
	using duration = std::chrono::duration<rep, period>;
	using time_point = std::chrono::time_point<Clock>;
	const bool is_steady = true;

	/**
	 * Gets the current time.
	 *
	 * Effectively a wrapper around pros::millis()
	 *
	 * \return The current time
	 */
	static time_point now();
};

class Mutex {
	std::shared_ptr<std::remove_pointer_t<mutex_t>> mutex;

	public:
	Mutex();

	// disable copy and move construction and assignment per Mutex requirements
	// (see https://en.cppreference.com/w/cpp/named_req/Mutex)
	Mutex(const Mutex&) = delete;
	Mutex(Mutex&&) = delete;

	Mutex& operator=(const Mutex&) = delete;
	Mutex& operator=(Mutex&&) = delete;

	/**
	 * Takes and locks a mutex indefinetly.
	 *
	 * See
	 * https://pros.cs.purdue.edu/v5/tutorials/topical/multitasking.html#mutexes
	 * for details.
	 *
	 * \return True if the mutex was successfully taken, false otherwise. If false
	 * is returned, then errno is set with a hint about why the the mutex
	 * couldn't be taken.
	 */
	bool take();

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
	bool give();

	/**
	 * Takes and locks a mutex, waiting for up to TIMEOUT_MAX milliseconds.
	 *
	 * Effectively equivalent to calling pros::Mutex::take with TIMEOUT_MAX as
	 * the parameter.
	 *
	 * Conforms to named requirment BasicLockable
	 * \see https://en.cppreference.com/w/cpp/named_req/BasicLockable
	 *
	 * \note Consider using a std::unique_lock, std::lock_guard, or
	 * 		 std::scoped_lock instead of interacting with the Mutex directly.
	 *
	 * \exception std::system_error Mutex could not be locked within TIMEOUT_MAX
	 *			  milliseconds. see errno for details.
	 */
	void lock();

	/**
	 * Unlocks a mutex.
	 *
	 * Equivalent to calling pros::Mutex::give.
	 *
	 * Conforms to named requirement BasicLockable
	 * \see https://en.cppreference.com/w/cpp/named_req/BasicLockable
	 *
	 * \note Consider using a std::unique_lock, std::lock_guard, or
	 * 		 std::scoped_lock instead of interacting with the Mutex direcly.
	 */
	void unlock();

	/**
	 * Try to lock a mutex.
	 *
	 * Returns immediately if unsucessful.
	 *
	 * Conforms to named requirement Lockable
	 * \see https://en.cppreference.com/w/cpp/named_req/Lockable
	 *
	 * \return True when lock was acquired succesfully, or false otherwise.
	 */
	bool try_lock();

	/**
	 * Takes and locks a mutex, waiting for a specified duration.
	 *
	 * Equivalent to calling pros::Mutex::take with a duration specified in
	 * milliseconds.
	 *
	 * Conforms to named requirement TimedLockable
	 * \see https://en.cppreference.com/w/cpp/named_req/TimedLockable
	 *
	 * \param rel_time Time to wait before the mutex becomes available.
	 * \return True if the lock was acquired succesfully, otherwise false.
	 */
	template <typename Rep, typename Period>
	bool try_lock_for(const std::chrono::duration<Rep, Period>& rel_time) {
		return take(std::chrono::duration_cast<Clock::duration>(rel_time).count());
	}

	/**
	 * Takes and locks a mutex, waiting until a specified time.
	 *
	 * Conforms to named requirement TimedLockable
	 * \see https://en.cppreference.com/w/cpp/named_req/TimedLockable
	 *
	 * \param abs_time Time point until which to wait for the mutex.
	 * \return True if the lock was acquired succesfully, otherwise false.
	 */
	template <typename Duration>
	bool try_lock_until(const std::chrono::time_point<Clock, Duration>& abs_time) {
		return take(std::max(static_cast<uint32_t>(0), (abs_time - Clock::now()).count()));
	}
};

template <typename Var>
class MutexVar;

template <typename Var>
class MutexVarLock {
	Mutex& mutex;
	Var& var;

	friend class MutexVar<Var>;

	constexpr MutexVarLock(Mutex& mutex, Var& var) : mutex(mutex), var(var) {}

	public:
	/**
	 * Accesses the value of the mutex-protected variable.
	 */
	constexpr Var& operator*() const {
		return var;
	}

	/**
	 * Accesses the value of the mutex-protected variable.
	 */
	constexpr Var* operator->() const {
		return &var;
	}

	~MutexVarLock() {
		mutex.unlock();
	}
};

template <typename Var>
class MutexVar {
	Mutex mutex;
	Var var;

	public:
	/**
	 * Creates a mutex-protected variable which is initialized with the given
	 * constructor arguments.
	 *
	 * \param args
	          The arguments to provide to the Var constructor.
	 */
	template <typename... Args>
	MutexVar(Args&&... args) : mutex(), var(std::forward<Args>(args)...) {}

	/**
	 * Try to lock the mutex-protected variable.
	 *
	 * \param timeout
	 *        Time to wait before the mutex becomes available, in milliseconds. A
	 *        timeout of 0 can be used to poll the mutex.
	 *
	 * \return A std::optional which contains a MutexVarLock providing access to
	 * the protected variable if locking is successful.
	 */
	std::optional<MutexVarLock<Var>> try_lock(std::uint32_t timeout) {
		if (mutex.take(timeout)) {
			return {{mutex, var}};
		} else {
			return {};
		}
	}

	/**
	 * Try to lock the mutex-protected variable.
	 *
	 * \param timeout
	 *        Time to wait before the mutex becomes available. A timeout of 0 can
	 *        be used to poll the mutex.
	 *
	 * \return A std::optional which contains a MutexVarLock providing access to
	 * the protected variable if locking is successful.
	 */
	template <typename Rep, typename Period>
	std::optional<MutexVarLock<Var>> try_lock(const std::chrono::duration<Rep, Period>& rel_time) {
		try_lock(std::chrono::duration_cast<Clock::duration>(rel_time).count());
	}

	/**
	 * Lock the mutex-protected variable, waiting indefinitely.
	 *
	 * \return A MutexVarLock providing access to the protected variable.
	 */
	MutexVarLock<Var> lock() {
		while (!mutex.take(TIMEOUT_MAX))
			;
		return {mutex, var};
	}
};

/**
 * Gets the number of milliseconds since PROS initialized.
 *
 * \return The number of milliseconds since PROS initialized
 */
using pros::c::millis;

/**
 * Gets the number of microseconds since PROS initialized.
 *
 * \return The number of microseconds since PROS initialized
 */
using pros::c::micros;

/**
 * Delays a task for a given number of milliseconds.
 *
 * This is not the best method to have a task execute code at predefined
 * intervals, as the delay time is measured from when the delay is requested.
 * To delay cyclically, use task_delay_until().
 *
 * \param milliseconds
 * 		  The number of milliseconds to wait (1000 milliseconds per second)
 */
using pros::c::delay;
}  // namespace pros

#endif  // _PROS_RTOS_HPP_
