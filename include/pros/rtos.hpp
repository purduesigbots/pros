/**
 * \file pros/rtos.hpp
 * \ingroup cpp-rtos
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
 * \copyright (c) 2017-2022, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * \defgroup cpp-rtos RTOS Facilities C API
 * \note Additional example code for this module can be found in its [Tutorial.](@ref multitasking)
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
#include <type_traits>

namespace pros {
inline namespace rtos {
/**
 * \ingroup cpp-rtos
 */
class Task {
	/**
	 * \addtogroup cpp-rtos
	 *  @{
	 */
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
	 * \b Example
	 * \code
	 * void my_task_fn(void* param) {
	 *   std::cout << "Hello" << (char*)param;
	 *   // ...
	 * }
	 * 
	 * void initialize() {
	 *   pros::Task my_task (my_task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
	 *     TASK_STACK_DEPTH_DEFAULT, "My Task");
	 * }
	 * \endcode
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
	 * \b Example
	 * \code
	 * void my_task_fn(void* param) {
	 *   std::cout << "Hello" << (char*)param;
	 *   // ...
	 * }
	 * 
	 * void initialize() {
	 *   pros::Task my_cpp_task (my_task_fn, (void*)"PROS", "My Task");
	 * }
	 * \endcode
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
	 * \b Example
	 * \code
	 * void initialize() {
	 *   std::unique_ptr<int> data{new int(7)};
	 *   pros::Task my_callable_task ([=] {
	 *     pros::delay(1000);
	 *     std::cout << *data << std::endl;
	 *   });
	 * }
	 * \endcode
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
	 * Create a new task from any C++ [Callable](https://en.cppreference.com/w/cpp/named_req/Callable)
	 * object and add it to the list of tasks that are ready to run.
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
	 * \b Example
	 * \code
	 * void initialize() {
	 *   std::unique_ptr<int> data{new int(7)};
	 *   pros::Task my_callable_task ([=] {
	 *     pros::delay(1000);
	 *     std::cout << *data << std::endl;
	 *   }, "callable_task");
	 * }
	 * \endcode
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
	 * \b Example
	 * \code
	 * void initialize() {
	 *   std::unique_ptr<int> data{new int(7)};
	 *   pros::Task my_callable_task ([=] {
	 *     pros::delay(1000);
	 *     std::cout << *data << std::endl;
	 *   });
	 * }
	 * \endcode
	 */
	template <class F>
	explicit Task(F&& function, std::uint32_t prio = TASK_PRIORITY_DEFAULT, std::uint16_t stack_depth = TASK_STACK_DEPTH_DEFAULT,
	     const char* name = "")
	    : Task(
	          [](void* parameters) {
		          std::unique_ptr<std::function<void()>> ptr{static_cast<std::function<void()>*>(parameters)};
		          (*ptr)();
	          },
	          new std::function<void()>(std::forward<F>(function)), prio, stack_depth, name) {
		static_assert(std::is_invocable_r_v<void, F>);
	}

	/**
	 * Create a new task from any C++ [Callable](https://en.cppreference.com/w/cpp/named_req/Callable)
	 * object and add it to the list of tasks that are ready to run.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENOMEM - The stack cannot be used as the TCB was not created.
	 *
	 * \param function Callable object to use as entry function. Must also satisfy std::is_invocable_r_v<void, F>.	 
	 * \param name
	 *        A descriptive name for the task.  This is mainly used to facilitate
	 *        debugging. The name may be up to 32 characters long.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   std::unique_ptr<int> data{new int(7)};
	 *   pros::Task my_callable_task ([=] {
	 *     pros::delay(1000);
	 *     std::cout << *data << std::endl;
	 *   }, "callable_task");
	 * }
	 * \endcode
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
	 * 
	 * \b Example
	 * \code
	 * 
	 * \endcode
	 */
	explicit Task(task_t task);

	/**
	 * Get a handle to the task which called this function.
	 * 
	 * \b Example
	 * \code
	 * void my_task_fn(void* param) {
	 *   Task this_task = pros::Task::current();
	 *   if (this_task.get_state() == pros::E_TASK_STATE_RUNNING) {
	 *     std::cout << "This task is currently running" std::endl;
	 *   }
	 *   // ...
	 * }
	 * 
	 * void initialize() {
	 *   Task my_task (my_task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
	 *     TASK_STACK_DEPTH_DEFAULT, "My Task");
	 * }
	 * \endcode
	 */
	static Task current();

	/**
	 * Creates a new task and add it to the list of tasks that are ready to run.
	 *
	 * \param in
	 *        A task handle from task_create() for which to create a pros::Task
	 *        object.
	 * 
	 * \b Example
	 * \code
	 * void my_task_fn(void* param) {
	 *   std::cout << "Hello" << (char*)param;
	 *   // ...
	 * }
	 * 
	 * void initialize() {
	 *   pros::task_t my_task = task_create(my_task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
	 *     TASK_STACK_DEPTH_DEFAULT, "My Task");
	 *   Task my_cpp_task = my_task;
	 * }
	 * \endcode
	 */
	Task& operator=(task_t in);

	/**
	 * Removes the Task from the RTOS real time kernel's management. This task
	 * will be removed from all ready, blocked, suspended and event lists.
	 *
	 * Memory dynamically allocated by the task is not automatically freed, and
	 * should be freed before the task is deleted.
	 * 
	 * \b Example
	 * \code
	 * void my_task_fn(void* ign) {
	 *   // Do things
	 * }
	 * 
	 * void opcontrol() {
	 *   Task my_task (my_task_fn, NULL, TASK_PRIORITY_DEFAULT,
	 *   TASK_STACK_DEPTH_DEFAULT, "Example Task");
	 *   // Do things
	 *   my_task.remove(); // Delete the task
	 *   std::cout << "Task State: " << my_task.get_state() << std::endl;
	 *   // Prints the value of E_TASK_STATE_DELETED
	 * }
	 * \endcode
	 */
	void remove();

	/**
	 * Gets the priority of the specified task.
	 *
	 * \return The priority of the task
	 * 
	 * \b Example
	 * \code
	 * void my_task_fn(void* param) {
	 *   std::cout << "Hello" << (char*)param;
	 *   // ...
	 * }
	 * 
	 * void initialize() {
	 *   Task my_task (my_task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
	 *     TASK_STACK_DEPTH_DEFAULT, "My Task");
	 *   std::cout << "Task Priority:" << my_task.get_priority();
	 * }
	 * \endcode
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
	 * 
	 * \b Example
	 * \code
	 * void my_task_fn(void* ign) {
	 *   // Do things
	 * }
	 * 
	 * void opcontrol() {
	 *   Task my_task (my_task_fn, NULL, TASK_PRIORITY_DEFAULT,
	 *   TASK_STACK_DEPTH_DEFAULT, "Example Task");
	 *   my_task.set_priority(TASK_PRIORITY_DEFAULT + 1);
	 * }
	 * \endcode
	 */
	void set_priority(std::uint32_t prio);

	/**
	 * Gets the state of the specified task.
	 *
	 * \return The state of the task. See task_state_e_t.
	 * 
	 * \b Example
	 * \code
	 * void my_task_fn(void* param) {
	 *   std::cout << "Hello" << (char*)param;
	 *   // ...
	 * }
	 * 
	 * void initialize() {
	 *   Task my_task (my_task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
	 *   TASK_STACK_DEPTH_DEFAULT, "My Task");
	 *   std::cout << "Task's State:" << my_task.get_state();
	 * }
	 * \endcode
	 */
	std::uint32_t get_state();

	/**
	 * Suspends the specified task, making it ineligible to be scheduled.
	 * 
	 * \b Example
	 * \code
	 * void my_task_fn(void* ign) {
	 *   // Do things
	 * }
	 * 
	 * void opcontrol() {
	 *   Task my_task (my_task_fn, NULL, TASK_PRIORITY_DEFAULT,
	 *     TASK_STACK_DEPTH_DEFAULT, "Example Task");
	 *   // Do things
	 *   my_task.suspend(); // The task will no longer execute
	 *   // Do other things
	 *   my_task.resume(); // The task will resume execution
	 * }
	 * \endcode
	 */
	void suspend();

	/**
	 * Resumes the specified task, making it eligible to be scheduled.
	 *
	 * \param task
	 *        The task to resume
	 * 
	 * \b Example
	 * \code
	 * void my_task_fn(void* ign) {
	 *   // Do things
	 * }
	 * 
	 * void opcontrol() {
	 *   Task my_task (my_task_fn, NULL, TASK_PRIORITY_DEFAULT,
	 *     TASK_STACK_DEPTH_DEFAULT, "Example Task");
	 *   // Do things
	 *   my_task.suspend(); // The task will no longer execute
	 *   // Do other things
	 *   my_task.resume(); // The task will resume execution
	 * }
	 * \endcode
	 */
	void resume();

	/**
	 * Gets the name of the specified task.
	 *
	 * \return A pointer to the name of the task
	 * 
	 * \b Example
	 * \code
	 * void my_task_fn(void* param) {
	 *   std::cout << "Hello" << (char*)param;
	 *   // ...
	 * }
	 * 
	 * void initialize() {
	 *   Task my_task (my_task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
	 *     TASK_STACK_DEPTH_DEFAULT, "My Task");
	 *   std::cout << "Task Name:" << my_task.get_name();
	 * }
	 * \endcode
	 */
	const char* get_name();

	/**
	 * Convert this object to a C task_t handle
	 * 
	 * \b Example
	 * \code
	 * 
	 * \endcode
	 */
	explicit operator task_t() {
		return task;
	}

	/**
	 * Sends a simple notification to task and increments the notification
	 * counter.
	 *
	 * See @ref notifications for details.
	 *
	 * \return Always returns true.
	 * 
	 * \b Example
	 * \code
	 * void my_task_fn(void* ign) {
	 *   while(my_task.notify_take(true, TIMEOUT_MAX)) {
	 *   std::cout << "I was unblocked!";
	 *   }
	 * }
	 * 
	 * void opcontrol() {
	 *   Task my_task (my_task_fn, NULL, TASK_PRIORITY_DEFAULT,
	 *   TASK_STACK_DEPTH_DEFAULT, "Notify me! Task");
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while(true) {
	 *     if(master.get_digital(DIGITAL_L1)) {
	 *       my_task.notify(my_task);
	 *     }
	 *   }
	 * }
	 * \endcode
	 */
	std::uint32_t notify();

	/**
 	 * Utilizes task notifications to wait until specified task is complete and deleted,
 	 * then continues to execute the program. Analogous to std::thread::join in C++.
	 *
	 * See @ref notifications for details.
	 * 
	 * \b Example
	 * \code
	 * void my_task(void* ign) {
	 *   std::cout << "Task running" <<
	 *   pros::Task::delay(1000);
	 *   std::cout << "End of task" <<
	 * }
	 * 
	 * void opcontrol() {
	 *   Task my_task (my_task_fn, NULL, TASK_PRIORITY_DEFAULT,
	 *     TASK_STACK_DEPTH_DEFAULT, "Task One");
	 *   std::cout << "Before task" <<
	 *   my_task.join();
	 *   std::cout << "After task" <<
	 * }
	 * \endcode
	 */
	void join();

	/**
	 * Sends a notification to a task, optionally performing some action. Will
	 * also retrieve the value of the notification in the target task before
	 * modifying the notification value.
	 *
	 * See @ref notifications for details.
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
	 * 
	 * \b Example
	 * \code
	 * TBA
	 * \endcode
	 */
	std::uint32_t notify_ext(std::uint32_t value, notify_action_e_t action, std::uint32_t* prev_value);

	/**
	 * Waits for a notification to be nonzero.
	 *
	 * See @ref notifications for details.
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
	 * 
	 * \b Example
	 * \code
	 * void my_task_fn(void* ign) {
	 *   while(my_task.notify_take(true, TIMEOUT_MAX)) {
	 *   std::cout << "I was unblocked!";
	 *   }
	 * }
	 * 
	 * void opcontrol() {
	 *   Task my_task (my_task_fn, NULL, TASK_PRIORITY_DEFAULT,
	 *     TASK_STACK_DEPTH_DEFAULT, "Notify me! Task");
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while(true) {
	 *     if(master.get_digital(DIGITAL_L1)) {
	 *       my_task.notify(my_task);
	 *     }
	 *   }
	 * }
	 * \endcode
	 */
	static std::uint32_t notify_take(bool clear_on_exit, std::uint32_t timeout);

	/**
	 * Clears the notification for a task.
	 *
	 * See @ref notifications for details.
	 *
	 * \return False if there was not a notification waiting, true if there was
	 * 
	 * \b Example
	 * \code
	 * TBA
	 * \endcode
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
	 * 
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   while (true) {
	 *   // Do opcontrol things
	 *   pros::Task::delay(2);
	 *   }
	 * }
	 * \endcode
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
	 * 
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   std::uint32_t now = pros::millis();
	 *   while (true) {
	 *     // Do opcontrol things
	 *     pros::Task::delay_until(&now, 2);
	 *   }
	 * }
	 * \endcode
	 */
	static void delay_until(std::uint32_t* const prev_time, const std::uint32_t delta);

	/**
	 * Gets the number of tasks the kernel is currently managing, including all
	 * ready, blocked, or suspended tasks. A task that has been deleted, but not
	 * yet reaped by the idle task will also be included in the count.
	 * Tasks recently created may take one context switch to be counted.
	 *
	 * \return The number of tasks that are currently being managed by the kernel.
	 * 
	 * \b Example
	 * \code
	 * void my_task_fn(void* param) {
	 *   std::cout << "Hello" << (char*)param;
	 *   // ...
	 * }
	 * 
	 * void initialize() {
	 *   Task my_task (my_task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
	 *     TASK_STACK_DEPTH_DEFAULT, "My Task");
	 *     std::cout << "Number of Running Tasks:" << pros::Task::get_count();
	 * }
	 * \endcode
	 */
	static std::uint32_t get_count();

	private:
	task_t task{};
	///@}
}; // class task

/**
 * \struct STL Clock compliant clock
 */
struct Clock {
	/**
	 * \addtogroup cpp-rtos
	 *  @{
	 */
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
}; //struct clock

class Mutex {
	/**
	 * \addtogroup cpp-rtos
	 *  @{
	 */
	std::shared_ptr<std::remove_pointer_t<mutex_t>> mutex;

	public:
	
	/**
	 * @brief Creates a mutex.
	 * 
	 * See @ref multitasking for details.
	 * 
	 * \b Example
	 * \code
	 * Mutex mutex;
	 * // Acquire the mutex; other tasks using this command will wait until the mutex is released
	 * // timeout can specify the maximum time to wait, or MAX_DELAY to wait forever
	 * // If the timeout expires, "false" will be returned, otherwise "true"
	 * mutex.take(MAX_DELAY);
	 * // do some work
	 * // Release the mutex for other tasks
	 * mutex.give();
	 * \endcode
	 */
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
	 * See @ref multitasking for details.
	 *
	 * \return True if the mutex was successfully taken, false otherwise. If false
	 * is returned, then errno is set with a hint about why the the mutex
	 * couldn't be taken.
	 * 
	 * \b Example
	 * \code
	 * Mutex mutex;
	 * 
	 * // Acquire the mutex; does not time out if parameter not specified.
	 * mutex.take();
	 * // do some work
	 * // Release the mutex for other tasks
	 * mutex.give();
	 * \endcode
	 */
	bool take();

	/**
	 * Takes and locks a mutex, waiting for up to a certain number of milliseconds
	 * before timing out.
	 *
	 * See @ref multitasking for details.
	 *
	 * \param timeout
	 *        Time to wait before the mutex becomes available. A timeout of 0 can
	 *        be used to poll the mutex. TIMEOUT_MAX can be used to block
	 *        indefinitely.
	 *
	 * \return True if the mutex was successfully taken, false otherwise. If false
	 * is returned, then errno is set with a hint about why the the mutex
	 * couldn't be taken.
	 * 
	 * \b Example
	 * \code
	 * Mutex mutex;
	 * // Acquire the mutex; other tasks using this command will wait until the mutex is released
	 * // timeout can specify the maximum time to wait, or MAX_DELAY to wait forever
	 * // If the timeout expires, "false" will be returned, otherwise "true"
	 * mutex.take(MAX_DELAY);
	 * // do some work
	 * // Release the mutex for other tasks
	 * mutex.give();
	 * \endcode
	 */
	bool take(std::uint32_t timeout);

	/**
	 * Unlocks a mutex.
	 *
	 * See @ref multitasking for details.
	 *
	 * \return True if the mutex was successfully returned, false otherwise. If
	 * false is returned, then errno is set with a hint about why the mutex
	 * couldn't be returned.
	 * 
	 * \b Example
	 * \code
	 * Mutex mutex;
	 * // Acquire the mutex; other tasks using this command will wait until the mutex is released
	 * // timeout can specify the maximum time to wait, or MAX_DELAY to wait forever
	 * // If the timeout expires, "false" will be returned, otherwise "true"
	 * mutex.take(MAX_DELAY);
	 * // do some work
	 * // Release the mutex for other tasks
	 * mutex.give();
	 * \endcode
	 */
	bool give();

	/**
	 * Takes and locks a mutex, waiting for up to TIMEOUT_MAX milliseconds.
	 *
	 * Effectively equivalent to calling pros::Mutex::take with TIMEOUT_MAX as
	 * the parameter.
	 *
	 * Conforms to named requirement [BasicLockable](https://en.cppreference.com/w/cpp/named_req/BasicLockable)
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
	 * Conforms to named requirement [BasicLockable](https://en.cppreference.com/w/cpp/named_req/BasicLockable)
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
	 * Conforms to named requirement [TimedLockable](https://en.cppreference.com/w/cpp/named_req/TimedLockable)
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
	 * Conforms to named requirement [TimedLockable](https://en.cppreference.com/w/cpp/named_req/TimedLockable)
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
	 * Conforms to named requirement [TimedLockable](https://en.cppreference.com/w/cpp/named_req/TimedLockable)
	 *
	 * \param abs_time Time point until which to wait for the mutex.
	 * \return True if the lock was acquired succesfully, otherwise false.
	 */
	template <typename Duration>
	bool try_lock_until(const std::chrono::time_point<Clock, Duration>& abs_time) {
		return take(std::max(static_cast<uint32_t>(0), (abs_time - Clock::now()).count()));
	}
	///@}
}; // class mutex
} // namespace rtos

/**
 * \name PROS Time-related functions
 * @{
 */

/**
 * Gets the number of milliseconds since PROS initialized.
 *
 * \return The number of milliseconds since PROS initialized
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   std::uint32_t now = pros::millis();
 *   while (true) {
 *     // Do opcontrol things
 *     pros::Task::delay_until(&now, 2);
 *   }
 * }
 * \endcode
 */
using pros::c::millis;

/**
 * Gets the number of microseconds since PROS initialized.
 *
 * \return The number of microseconds since PROS initialized
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   std::uint64_t now = pros::micros();
 *   while (true) {
 *     // Do opcontrol things
 *     pros::Task::delay_until(&now, 2000);
 *   }
 * }
 * \endcode
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
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   while (true) {
 *     // Do opcontrol things
 *     pros::Task::delay(20);
 *   }
 * }
 * \endcode
 */

///@}

using pros::c::delay;
}  // namespace pros

#endif  // _PROS_RTOS_HPP_
