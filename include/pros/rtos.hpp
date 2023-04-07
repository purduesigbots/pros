/**
 * \file pros/rtos.hpp
 * \ingroup cpp-rtos
 *
 * Contains declarations for the PROS RTOS kernel for use by typical VEX
 * programmers.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * \defgroup cpp-rtos RTOS Facilities C++ API
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
#include <optional>
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
	 *   printf("Hello %s\n", (char*)param);
	 *   // ...
	 * }
	 * 
	 * void initialize() {
	 *   pros::Task my_task(my_task_fn, (void*)"PROS");
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
	 *   printf("Hello %s\n", (char*)param);
	 *   // ...
	 * }
	 * 
	 * void initialize() {
	 *   pros::Task my_task(my_task_fn, (void*)"PROS", "My Task");
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
	 * void my_task_fn(void* param) {
	 *   printf("Hello %s\n", (char*)param);
	 *   // ...
	 * }
	 * 
	 * void initialize() {
	 *   pros::c::task_t my_task = pros::Task::create(my_task_fn, (void*)"PROS");
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
	 * \b Example
	 * \code
	 * void my_task_fn(void* param) {
	 *   printf("Hello %s\n", (char*)param);
	 *   // ...
	 * }
	 * 
	 * void initialize() {
	 *   pros::c::task_t my_task = pros::Task::create(my_task_fn, "My Task");
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
	 * 
	 * void initialize() {
	 *   // Create a task function using lambdas
	 *   auto task_fn = [](void* param) {
	 *     printf("Hello %s\n", (char*)param);
	 *   }
	 * 
	 *   pros::Task my_task(task_fn, (void*)"PROS", "My Task");
	 * }
	 * \endcode
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
	 * \b Example
	 * \code
	 * void my_task_fn(void* param) {
	 *   printf("Hello %s\n", (char*)param);
	 *   // ...
	 * }
	 * 
	 * void initialize() {
	 *   pros::Task my_task(
	 *     [](void* param) {
	 *       printf("Inside the task!\n");
	 *     },
	 *     "My Task"
	 *   );
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
	 * void my_task_fn(void* param) {
	 *   printf("Hello %s\n", (char*)param);
	 *   // ...
	 * }
	 * 
	 * void initialize() {
	 *   pros::c::task_t my_task = pros::Task::create(my_task_fn, "My Task");
	 * 
	 *   pros::Task my_task_cpp(my_task);
	 * }
	 * \endcode
	 */
	explicit Task(task_t task);

	/**
	 * Get the currently running Task
	 * 
	 * @return The currently running Task.
	 * 
	 * \b Example
	 * \code
	 * void my_task_fn(void* param) {
	 *   printf("The name of this task is \"%s\"\n", pros::Task::current().get_name()
	 * }
	 * 
	 * void initialize() {
	 *   pros::Task my_task(my_task_fn, pros::TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "My Task");
	 * }
	 * \endcode
	 */
	static Task current();

	/**
	 * Creates a task object from the passed task handle.
	 *
	 * \param in
	 *        A task handle from task_create() for which to create a pros::Task
	 *        object.
	 * 
	 * \b Example
	 * \code
	 * void my_task_fn(void* param) {
	 *   printf("The name of this task is \"%s\"\n", pros::Task::current().get_name()
	 * }
	 * 
	 * void initialize() {
	 *   pros::c::task_t my_task = pros::Task::create(my_task_fn, "My Task");
	 * 
	 *   pros::Task my_task_cpp = my_task;
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
     * void my_task_fn(void* param) {
     *   printf("Hello %s\n", (char*)param);
     *   // ...
     * }
     * 
     * void initialize() {
	 *   pros::Task my_task(my_task_fn, "My Task");
	 *   
	 *   my_task.remove();
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
     *   printf("Hello %s\n", (char*)param);
     *   // ...
     * }
     * 
     * void initialize() {
	 *   pros::Task my_task(my_task_fn, "My Task");
	 * 
     *   printf("Task Priority: %d\n", my_task.get_priority());
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
     * void my_task_fn(void* param) {
     *   printf("Hello %s\n", (char*)param);
     *   // ...
     * }
     * 
     * void initialize() {
	 *   pros::Task my_task(my_task_fn, "My Task");
	 * 
	 *   Task.set_priority(pros::DEFAULT_PRIORITY + 1);
     * }
     * \endcode
	 */
	void set_priority(std::uint32_t prio);

	/**
	 * Gets the state of the specified task.
	 *
	 * \return The state of the task
	 * 
	 * \b Example
     * \code
     * void my_task_fn(void* param) {
     *   printf("Hello %s\n", (char*)param);
     *   // ...
     * }
     * 
     * void initialize() {
	 *   pros::Task my_task(my_task_fn, "My Task");
	 * 
     *   printf("Task State: %d\n", my_task.get_state());
     * }
     * \endcode
	 */
	std::uint32_t get_state();

	/**
	 * Suspends the specified task, making it ineligible to be scheduled.
	 * 
	 * \b Example
     * \code
	 * pros::Mutex counter_mutex;
     * int counter = 0;
     * 
     * void my_task_fn(void* param) {
     *   while(true) {
	 * 	   counter_mutex.take(); // Mutexes are used for protecting shared resources
     *     counter++;
	 *     counter_mutex.give();
     *     pros::delay(10);
     *   }
     * }
     * 
     * void opcontrol() {
	 *   pros::Task task(my_task_fn, "My Task");
     * 
     *   while(true) {
     *     counter_mutex.take();
     *     if(counter > 100) {
     *       task_suspepend(task);
     * 	   }
     *     counter_mutex.give();
     *     pros::delay(10);
     *   }
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
     * void my_task_fn(void* param) {
     *   while(true) {
     *     // Do stuff
     *     pros::delay(10);
     *   }
     * }
     * 
     * pros::Task task(my_task_fn);
     * 
     * void autonomous() {
     *   task.resume();
     * 
     *   // Run autonomous , then suspend the task so it doesn't interfere run
     *   // outside of autonomous or opcontrol
     *   task.suspend();
     * }
     * 
     * void opcontrol() {
     *   task.resume();
     *   // Opctonrol code here
     *   task.suspend();
     * }
     * 
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
     *   printf("Hello %s\n", (char*)param);
     *   // ...
     * }
     * 
     * void initialize() {
	 *   pros::Task my_task(my_task_fn, "My Task");
     *   printf("Number of Running Tasks: %d\n", my_task.get_name());
     * }
     * \endcode
	 */
	const char* get_name();

	/**
	 * Convert this object to a C task_t handle
	 * 
	 * \b Example
     * \code
     * void my_task_fn(void* param) {
     *   printf("Hello %s\n", (char*)param);
     *   // ...
     * }
     * 
     * void initialize() {
	 *   pros::Task my_task(my_task_fn, "My Task");
	 * 
	 *   pros::c::task_t my_task_c = (pros::c::task_t)my_task;
     * }
     * \endcode
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
	 * 
	 * \b Example
     * \code
     * void my_task_fn(void* ign) {
     *   while(pros::Task::current_task().notify_take(true) == 0) {
	 *     // Code while waiting
     *   }
	*    puts("I was unblocked!");
     * }
     * 
     * void opcontrol() {
     *   pros::Task my_task(my_task_fn);
	 *   
     *   while(true) {
     *     if(controller_get_digital(CONTROLLER_MASTER, DIGITAL_L1)) {
     *       my_task.notify();
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
	 * See https://pros.cs.purdue.edu/v5/tutorials/topical/notifications.html for
	 * details.
	 *
	 * \return void
	 * 
	 * \b Example
     * \code
     * void my_task_fn(void* ign) {
     *   lcd_print(1, "%s running", pros::Task::current_task().get_name());
     *   task_delay(1000);
     *   lcd_print(2, "End of %s", pros::Task::current_task().get_name());
     * }
     * 
     * void opcontrol() {
	 *   pros::Task my_task(my_task_fn);
     *   pros::lcd::set_text(0, "Running task.");
     *   my_task.join();
     *   pros::lcd::lcd_set_text(3, "Task completed.");
     * }
     * \endcode
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
	 * 
	 * \b Example
     * \code
     * void my_task_fn(void* param) {
	 *   pros::Task task = pros::Task::current();
	 * 
     *   while(true) {
     *     // Wait until we have been notified 20 times before running the code
     *     if(task.notify_take(false, TIMEOUT_MAX) == 20) {
     *       // ... Code to do stuff here ...
     * 
     *       // Reset the notification counter
     *       task.notify_clear();
     *     }
     *     delay(10);
     * 	 }
     * }
     * 
     * void opcontrol() {
	 *   pros::Task task(my_task_fn);
     *   
     *   int count = 0;
     *   
     *   while(true) {
     *     if(controller_get_digital(CONTROLLER_MASTER, DIGITAL_L1)) {
     *       task.notify_ext(1, NOTIFY_ACTION_INCREMENT, &count);
     *     }
     *     
     *     delay(20);
     *   }
     * }
     * \endcode
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
	 * 
	 * \b Example
     * \code
     * void my_task_fn(void* ign) {
     *   pros::Task task = pros::task::current();
     *   while(task.notify_take(true, TIMEOUT_MAX)) {
     *     puts("I was unblocked!");
     *   }
     * }
     * 
     * void opcontrol() {
	 *   pros::Task task(my_task_fn);
     *   while(true) {
     *     if(controller_get_digital(CONTROLLER_MASTER, DIGITAL_L1)) {
     *       task.notify(my_task);
     *     }
     *   }
     * }
     * \endcode
	 */
	static std::uint32_t notify_take(bool clear_on_exit, std::uint32_t timeout);

	/**
	 * Clears the notification for a task.
	 *
	 * See https://pros.cs.purdue.edu/v5/tutorials/topical/notifications.html for
	 * details.
	 *
	 * \return False if there was not a notification waiting, true if there was
	 * \b Example
     * \code
     * void my_task_fn(void* param) {
	 *   pros::Task task = pros::Task::current();
     *   while(true) {
     *     printf("Waiting for notification...\n");
     * 	   printf("Got a notification: %d\n", task.notify_take(false, TIMEOUT_MAX));
     * 
     * 	   tasK_notify(task);
     *     delay(10):
     *   }
     * }
     * 
     * void opcontrol() {
	 *   pros::Task task(my_task_fn);
     *   while(true) {
     *     if(controller_get_digital(CONTROLLER_MASTER, DIGITAL_L1)) {
     *       task.notify();
     *     }
     *     delay(10);
     *   }
     * }
     * \endcode
	 */
	bool notify_clear();

	/**
	 * Delays the current task for a specified number of milliseconds.
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
     *     // Do opcontrol things
     *     pros::Task::delay(2);
     *   }
     * \endcode
	 */
	static void delay(const std::uint32_t milliseconds);

	/**
	 * Delays the current Task until a specified time.  This function can be used by
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
     *   while (true) {
     *     // Do opcontrol things
     *     pros::Task::delay(2);
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
     *   printf("Hello %s\n", (char*)param);
     *   // ...
     * }
	 * 
     * void opcontrol() {
	 *   pros::Task my_task(my_task_fn);
     *   printf("There are %d tasks running\n", pros::Task::get_count());
     * }
     * \endcode
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
	 * 
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Clock::time_point start = pros::Clock::now();
	 *   pros::Clock::time_point end = pros::Clock::now();
	 *   pros::Clock::duration duration = end - start;
	 *   printf("Duration: %d\n", duration.count());
	 * 
	 *   if(duration.count() == 500) {
	 *     // If you see this comment in the DOCS, ping @pros in VTOW. 
	 *     // If you are the first person to do so, you will receive a free PROS 
	 * 	   // holo!
	 *     printf("Duration is 500 milliseconds\n");
	 *   }
	 * }
	 * \endcode
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
	 * couldn't be taken
	 * 
	 * \b Example
     * \code
     * // Global variables for the robot's odometry, which the rest of the robot's
     * // subsystems will utilize
     * double odom_x = 0.0;
     * double odom_y = 0.0;
     * double odom_heading = 0.0;
     * 
     * // This mutex protects the odometry data. Whenever we read or write to the
     * // odometry data, we should make copies into the local variables, and read
     * // all 3 values at once to avoid errors.
     * pros::Mutex odom_mutex;
     * 
     * void odom_task(void* param) {
     *   while(true) {
     *     // First we fetch the odom coordinates from the previous iteration of the
     *     // odometry task. These are put into local variables so that we can
     *     // keep the size of the critical section as small as possible. This lets
     *     // other tasks that need to use the odometry data run until we need to
     *     // update it again.
	 *     odom_mutex.take();
     *     double x_old = odom_x;
     *     double y_old = odom_y;
     *     double heading_old = odom_heading;
	 *     odom_mutex.give();
     * 
     *     double x_new = 0.0;
     *     double y_new = 0.0;
     *     double heading_new = 0.0;
     *     
     *     // --- Calculate new pose for the robot here ---
     * 
     *     // Now that we have the new pose, we can update the global variables
	 *     odom_mutex.take();
     *     odom_x = x_new;
     *     odom_y = y_new;
     *     odom_heading = heading_new;
	 *     odom_mutex.give();
     *     
     *     delay(10);
     *   }
     * }
     * 
     * void chassis_task(void* param) {
     *   while(true) {
     *     // Here we copy the current odom values into local variables so that
     *     // we can use them without worrying about the odometry task changing say,
     *     // the y value right after we've read the x. This ensures our values are
     *     // sound.
	 *     odom_mutex.take();
     *     double current_x = odom_x;
     *     double current_y = odom_y;
     *     double current_heading = odom_heading;
	 *     odom_mutex.give();
     *     
     *     // ---- Move the robot using the current locations goes here ----
     *     
     *     delay(10);
     *   }
     * }
     * 
     * void initialize() {
	 *   odom_mutex = pros::Mutex();
	 * 
	 *   pros::Task odom_task(odom_task, "Odometry Task");
	 *   pros::Task chassis_task(odom_task, "Chassis Control Task");
     * }
     * \endcode.
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
	 * 
	 * \b Example
     * \code
     * // Global variables for the robot's odometry, which the rest of the robot's
     * // subsystems will utilize
     * double odom_x = 0.0;
     * double odom_y = 0.0;
     * double odom_heading = 0.0;
     * 
     * // This mutex protects the odometry data. Whenever we read or write to the
     * // odometry data, we should make copies into the local variables, and read
     * // all 3 values at once to avoid errors.
     * pros::Mutex odom_mutex;
     * 
     * void odom_task(void* param) {
     *   while(true) {
     *     // First we fetch the odom coordinates from the previous iteration of the
     *     // odometry task. These are put into local variables so that we can
     *     // keep the size of the critical section as small as possible. This lets
     *     // other tasks that need to use the odometry data run until we need to
     *     // update it again.
	 *     odom_mutex.take();
     *     double x_old = odom_x;
     *     double y_old = odom_y;
     *     double heading_old = odom_heading;
	 *     odom_mutex.give();
     * 
     *     double x_new = 0.0;
     *     double y_new = 0.0;
     *     double heading_new = 0.0;
     *     
     *     // --- Calculate new pose for the robot here ---
     * 
     *     // Now that we have the new pose, we can update the global variables
	 *     odom_mutex.take();
     *     odom_x = x_new;
     *     odom_y = y_new;
     *     odom_heading = heading_new;
	 *     odom_mutex.give();
     *     
     *     delay(10);
     *   }
     * }
     * 
     * void chassis_task(void* param) {
     *   while(true) {
     *     // Here we copy the current odom values into local variables so that
     *     // we can use them without worrying about the odometry task changing say,
     *     // the y value right after we've read the x. This ensures our values are
     *     // sound.
	 *     odom_mutex.take();
     *     double current_x = odom_x;
     *     double current_y = odom_y;
     *     double current_heading = odom_heading;
	 *     odom_mutex.give();
     *     
     *     // ---- Move the robot using the current locations goes here ----
     *     
     *     delay(10);
     *   }
     * }
     * 
     * void initialize() {
	 *   odom_mutex = pros::Mutex();
	 * 
	 *   pros::Task odom_task(odom_task, "Odometry Task");
	 *   pros::Task chassis_task(odom_task, "Chassis Control Task");
     * }
     * \endcode.
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
	 * 
	 * \b Example
     * \code
     * // Global variables for the robot's odometry, which the rest of the robot's
     * // subsystems will utilize
     * double odom_x = 0.0;
     * double odom_y = 0.0;
     * double odom_heading = 0.0;
     * 
     * // This mutex protects the odometry data. Whenever we read or write to the
     * // odometry data, we should make copies into the local variables, and read
     * // all 3 values at once to avoid errors.
     * pros::Mutex odom_mutex;
     * 
     * void odom_task(void* param) {
     *   while(true) {
     *     // First we fetch the odom coordinates from the previous iteration of the
     *     // odometry task. These are put into local variables so that we can
     *     // keep the size of the critical section as small as possible. This lets
     *     // other tasks that need to use the odometry data run until we need to
     *     // update it again.
	 *     odom_mutex.take();
     *     double x_old = odom_x;
     *     double y_old = odom_y;
     *     double heading_old = odom_heading;
	 *     odom_mutex.give();
     * 
     *     double x_new = 0.0;
     *     double y_new = 0.0;
     *     double heading_new = 0.0;
     *     
     *     // --- Calculate new pose for the robot here ---
     * 
     *     // Now that we have the new pose, we can update the global variables
	 *     odom_mutex.take();
     *     odom_x = x_new;
     *     odom_y = y_new;
     *     odom_heading = heading_new;
	 *     odom_mutex.give();
     *     
     *     delay(10);
     *   }
     * }
     * 
     * void chassis_task(void* param) {
     *   while(true) {
     *     // Here we copy the current odom values into local variables so that
     *     // we can use them without worrying about the odometry task changing say,
     *     // the y value right after we've read the x. This ensures our values are
     *     // sound.
	 *     odom_mutex.take();
     *     double current_x = odom_x;
     *     double current_y = odom_y;
     *     double current_heading = odom_heading;
	 *     odom_mutex.give();
     *     
     *     // ---- Move the robot using the current locations goes here ----
     *     
     *     delay(10);
     *   }
     * }
     * 
     * void initialize() {
	 *   odom_mutex = pros::Mutex();
	 * 
	 *   pros::Task odom_task(odom_task, "Odometry Task");
	 *   pros::Task chassis_task(odom_task, "Chassis Control Task");
     * }
     * \endcode.
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
	 * 
	 * \b Example
     * \code
     * // Global variables for the robot's odometry, which the rest of the robot's
     * // subsystems will utilize
     * double odom_x = 0.0;
     * double odom_y = 0.0;
     * double odom_heading = 0.0;
     * 
     * // This mutex protects the odometry data. Whenever we read or write to the
     * // odometry data, we should make copies into the local variables, and read
     * // all 3 values at once to avoid errors.
     * pros::Mutex odom_mutex;
     * 
     * void odom_task(void* param) {
     *   while(true) {
     *     // First we fetch the odom coordinates from the previous iteration of the
     *     // odometry task. These are put into local variables so that we can
     *     // keep the size of the critical section as small as possible. This lets
     *     // other tasks that need to use the odometry data run until we need to
     *     // update it again.
	 *     odom_mutex.lock();
     *     double x_old = odom_x;
     *     double y_old = odom_y;
     *     double heading_old = odom_heading;
	 *     odom_mutex.unlock();
     * 
     *     double x_new = 0.0;
     *     double y_new = 0.0;
     *     double heading_new = 0.0;
     *     
     *     // --- Calculate new pose for the robot here ---
     * 
     *     // Now that we have the new pose, we can update the global variables
	 *     odom_mutex.lock();
     *     odom_x = x_new;
     *     odom_y = y_new;
     *     odom_heading = heading_new;
	 *     odom_mutex.unlock();
     *     
     *     delay(10);
     *   }
     * }
     * 
     * void chassis_task(void* param) {
     *   while(true) {
     *     // Here we copy the current odom values into local variables so that
     *     // we can use them without worrying about the odometry task changing say,
     *     // the y value right after we've read the x. This ensures our values are
     *     // sound.
	 *     odom_mutex.lock();
     *     double current_x = odom_x;
     *     double current_y = odom_y;
     *     double current_heading = odom_heading;
	 *     odom_mutex.unlock();
     *     
     *     // ---- Move the robot using the current locations goes here ----
     *     
     *     delay(10);
     *   }
     * }
     * 
     * void initialize() {
	 *   odom_mutex = pros::Mutex();
	 * 
	 *   pros::Task odom_task(odom_task, "Odometry Task");
	 *   pros::Task chassis_task(odom_task, "Chassis Control Task");
     * }
     * \endcode.
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
	 * 
	 * \b Example
     * \code
     * // Global variables for the robot's odometry, which the rest of the robot's
     * // subsystems will utilize
     * double odom_x = 0.0;
     * double odom_y = 0.0;
     * double odom_heading = 0.0;
     * 
     * // This mutex protects the odometry data. Whenever we read or write to the
     * // odometry data, we should make copies into the local variables, and read
     * // all 3 values at once to avoid errors.
     * pros::Mutex odom_mutex;
     * 
     * void odom_task(void* param) {
     *   while(true) {
     *     // First we fetch the odom coordinates from the previous iteration of the
     *     // odometry task. These are put into local variables so that we can
     *     // keep the size of the critical section as small as possible. This lets
     *     // other tasks that need to use the odometry data run until we need to
     *     // update it again.
	 *     odom_mutex.lock();
     *     double x_old = odom_x;
     *     double y_old = odom_y;
     *     double heading_old = odom_heading;
	 *     odom_mutex.unlock();
     * 
     *     double x_new = 0.0;
     *     double y_new = 0.0;
     *     double heading_new = 0.0;
     *     
     *     // --- Calculate new pose for the robot here ---
     * 
     *     // Now that we have the new pose, we can update the global variables
	 *     odom_mutex.lock();
     *     odom_x = x_new;
     *     odom_y = y_new;
     *     odom_heading = heading_new;
	 *     odom_mutex.unlock();
     *     
     *     delay(10);
     *   }
     * }
     * 
     * void chassis_task(void* param) {
     *   while(true) {
     *     // Here we copy the current odom values into local variables so that
     *     // we can use them without worrying about the odometry task changing say,
     *     // the y value right after we've read the x. This ensures our values are
     *     // sound.
	 *     odom_mutex.lock();
     *     double current_x = odom_x;
     *     double current_y = odom_y;
     *     double current_heading = odom_heading;
	 *     odom_mutex.unlock();
     *     
     *     // ---- Move the robot using the current locations goes here ----
     *     
     *     delay(10);
     *   }
     * }
     * 
     * void initialize() {
	 *   odom_mutex = pros::Mutex();
	 * 
	 *   pros::Task odom_task(odom_task, "Odometry Task");
	 *   pros::Task chassis_task(odom_task, "Chassis Control Task");
     * }
     * \endcode.
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
	 * 
	 * pros::Mutex mutex;
	 * 
	 * void my_task_fn(void* param) {
	 *   while (true) {
	 *     if(mutex.try_lock()) {
	 *       printf("Mutex aquired successfully!\n");
	 *       // Do stuff that requires the protected resource here
	 *     }
	 *     else {
	 *       printf("Mutex not aquired!\n");
	 *     }
	 *   }
	 * }
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
	 * 
	 * \b Example
	 * \code
	 * void my_task_fn(void* param) {
	 *   while (true) {
	 *     if(mutex.try_lock_for(std::chrono::milliseconds(100))) {
	 *       printf("Mutex aquired successfully!\n");
	 *       // Do stuff that requires the protected resource here
	 *     }
	 *     else {
	 *       printf("Mutex not aquired after 100 milliseconds!\n");
	 *     }
	 *   }
	 * }
	 * \endcode
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
	 * 
	 * \b Example
	 * \code
	 * void my_task_fn(void* param) {
	 *   while (true) {
	 *     // Get the current time point
	 *     auto now = std::chrono::system_clock::now();
	 * 
	 *     // Calculate the time point 100 milliseconds from now
	 *     auto abs_time = now + std::chrono::milliseconds(100);
	 * 
	 *     if(mutex.try_lock_until(abs_time)) {
	 *       printf("Mutex aquired successfully!\n");
	 *       // Do stuff that requires the protected resource here
	 *     }
	 *     else {
	 *       printf("Mutex not aquired after 100 milliseconds!\n");
	 *     }
	 *   }
	 * }
	 * \endcode
	 */
	template <typename Duration>
	bool try_lock_until(const std::chrono::time_point<Clock, Duration>& abs_time) {
		return take(std::max(static_cast<uint32_t>(0), (abs_time - Clock::now()).count()));
	}
	///@}
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
	 *        The arguments to provide to the Var constructor.
     *
     * \b Example
	 * \code
	 * // We create a pose class to contain all our odometry data in a single
	 * // variable that can be protected by a MutexVar. Otherwise, we would have
	 * // three seperate variables which could not be protected in a single 
	 * // MutexVar
	 * struct Pose {
	 *   double x;
	 *   double y;
	 *   double heading;
	 * }
	 * 
	 * pros::MutexVar<Pose> odom_pose(0.0, 0.0, 0.0);
	 * 
	 * void odom_task(void* param) {
     *   while(true) {
	 *     Pose old_pose = *odom_pose.lock();
     * 
	 *     Pose new_pose{0.0, 0.0, 0.0};
     *     
     *     // --- Calculate new pose for the robot here ---
     * 
     *     // Now that we have the new pose, we can update the global variables
	 * 
	 *     *odom_pose.take() = new_pose;
	 *     
	 *     delay(10);
	 *   }
	 * }
     * 
     * void chassis_task(void* param) {
     *   while(true) {
	 * 
	 *     Pose cur_pose = *odom_pose.take();
     *     
     *     // ---- Move the robot using the current locations goes here ----
     *     
     *     delay(10);
     *   }
     * }
     * 
     * void initialize() {
	 *   odom_mutex = pros::Mutex();
	 * 
	 *   pros::Task odom_task(odom_task, "Odometry Task");
	 *   pros::Task chassis_task(odom_task, "Chassis Control Task");
     * }
	 * 
	 * \endcode
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
	 * 
	 * \b Example
	 * \code
	 * pros::MutexVar<Pose> odom_pose;
	 * 
	 * void my_task(void* param) {
	 *   while(true) {
	 *     std::optional<pros::MutexVar<Pose>> cur_pose_opt = odom_pose.try_lock(100);
	 *     
	 *     if(cur_pose_opt.has_value()) {
	 *       Pose* cur_pose = **cur_pose_opt;
	 *     }
	 *     else {
	 *       printf("Could not lock the mutex var!");
	 *     }
	 * 
	 *     pros::delay(10);
	 *   }
	 * }
	 * \endcode
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
	 * 
	 * \b Example
	 * \code
	 * pros::MutexVar<Pose> odom_pose;
	 * 
	 * void my_task(void* param) {
	 *   while(true) {
	 *     std::chrono::duration<int, std::milli> timeout(100);
	 *     std::optional<pros::MutexVar<Pose>> cur_pose_opt = odom_pose.try_lock(timeout);
	 *     
	 *     if(cur_pose_opt.has_value()) {
	 *       Pose* cur_pose = **cur_pose_opt;
	 *     }
	 *     else {
	 *       printf("Could not lock the mutex var!");
	 *     }
	 * 
	 *     pros::delay(10);
	 *   }
	 * }
	 * \endcode
	 */
	template <typename Rep, typename Period>
	std::optional<MutexVarLock<Var>> try_lock(const std::chrono::duration<Rep, Period>& rel_time) {
		try_lock(std::chrono::duration_cast<Clock::duration>(rel_time).count());
	}

	/**
	 * Lock the mutex-protected variable, waiting indefinitely.
	 *
	 * \return A MutexVarLock providing access to the protected variable.
	 * 
	 * \b Example
	 * \code
	 * pros::MutexVar<Pose> odom_pose;
	 * 
	 * void my_task(void* param) {
	 *   while(true) {
	 *     pros::delay(10);
	 * 
	 *     pros::MutexVarLock<Pose> cur_pose = odom_pose.lock();
	 *     Pose cur_pose = *cur_pose;
	 *      
	 *     // do stuff with cur_pose
	 *   }
	 * }
	 * \endcode
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
}  // namespace rtos
}  // namespace pros

#endif  // _PROS_RTOS_HPP_
