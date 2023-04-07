/**
 * \file pros/rtos.h
 * \ingroup c-rtos
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
 * \defgroup c-rtos RTOS Facilities C API
 * \note Additional example code for this module can be found in its [Tutorial.](@ref multitasking)
 */	

#ifndef _PROS_RTOS_H_
#define _PROS_RTOS_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
namespace pros {
#endif

/// \ingroup c-rtos

/// \addtogroup c-rtos 
/// @{

/// \name Macros 
/// @{

/**
 * The highest priority that can be assigned to a task.
 * 
 * A task with this priority will always run if it is available to. Beware of
 * deadlocks when using this priority.
 */
#define TASK_PRIORITY_MAX 16

/**
 * The lowest priority that can be assigned to a task.
 *
 * This can cause severe performance problems and is generally not recommended
 * that users use this priority.
 */
#define TASK_PRIORITY_MIN 1

/**
 * The default task priority, which should be used for most tasks unless you
 * have a specific need for a higher or lower priority task.
 * 
 * The default tasks, such as autonomous(), are run with this priority
 */
#define TASK_PRIORITY_DEFAULT 8

/**
 * The recommended stack size for a new task.
 * 
 * This stack size is used for the default tasks such as autonomous(). This
 * size is 8,192 words, or 32,768 bytes. This should be enough for the majority
 * of tasks
 */
#define TASK_STACK_DEPTH_DEFAULT 0x2000

/**
 * The minimal stack size for a task.
 * 
 * This equates to  512 words, or 2,048 bytes. 
 */
#define TASK_STACK_DEPTH_MIN 0x200

/**
 * The maximum number of characters allowed in a task's name.
 */
#define TASK_NAME_MAX_LEN 32

/**
 * The maximum timeout value that can be given to, for instance, a mutex grab.
 */
#define TIMEOUT_MAX ((uint32_t)0xffffffffUL)

/// @} Name: Macros

/// \name Typedefs
/// @{

/**
 * An opaque type that pontis to a task handle. This is used for referencing a
 * task.
 */
typedef void* task_t;

/**
 * A pointer to a task's function. 
 * 
 * Such a function is called when a task starts, and exiting said function will
 * terminate the task.
 */
typedef void (*task_fn_t)(void*);

/// @} Name: Typedefs


/// \name Enumerations
/// @{

/**
 * The state of a task.
 */
typedef enum {
	E_TASK_STATE_RUNNING = 0, /**< The task is actively executing. */
	E_TASK_STATE_READY, /**< The task exists and is available to run, but is not currently running. */
	E_TASK_STATE_BLOCKED, /**< The task is delayed or blocked by a mutex, semaphore, or I/O operation. */
	E_TASK_STATE_SUSPENDED, /**< The task is supended using task_suspend. */
	E_TASK_STATE_DELETED, /**< The task has been deleted using task_delete. */
	E_TASK_STATE_INVALID /**< The task handle does not point to a current or past task.*/
} task_state_e_t;

/**
 * brief The action to take when a task is notified.
 */
typedef enum {
	E_NOTIFY_ACTION_NONE, /**< The task’s notification value will not be touched.*/
	E_NOTIFY_ACTION_BITS, /**< The task’s notification value will be bitwise ORed with the new value.*/
	E_NOTIFY_ACTION_INCR, /**< The task’s notification value will be incremented by one, effectively using it as a notification counter.*/
	E_NOTIFY_ACTION_OWRITE, /**< The task’s notification value will be unconditionally set to the new value.*/
	E_NOTIFY_ACTION_NO_OWRITE /**< The task’s notification value will be set to the new value if the task does not already have a pending notification.*/
} notify_action_e_t;

/// @} Name: Enumerations

/// \name Simple enum names
/// @{

#ifdef PROS_USE_SIMPLE_NAMES
#ifdef __cplusplus
#define TASK_STATE_RUNNING pros::E_TASK_STATE_RUNNING
#define TASK_STATE_READY pros::E_TASK_STATE_READY
#define TASK_STATE_BLOCKED pros::E_TASK_STATE_BLOCKED
#define TASK_STATE_SUSPENDED pros::E_TASK_STATE_SUSPENDED
#define TASK_STATE_DELETED pros::E_TASK_STATE_DELETED
#define TASK_STATE_INVALID pros::E_TASK_STATE_INVALID
#define NOTIFY_ACTION_NONE pros::E_NOTIFY_ACTION_NONE
#define NOTIFY_ACTION_BITS pros::E_NOTIFY_ACTION_BITS
#define NOTIFY_ACTION_INCR pros::E_NOTIFY_ACTION_INCR
#define NOTIFY_ACTION_OWRITE pros::E_NOTIFY_ACTION_OWRITE
#define NOTIFY_ACTION_NO_OWRITE pros::E_NOTIFY_ACTION_NO_OWRITE
#else
#define TASK_STATE_RUNNING E_TASK_STATE_RUNNING
#define TASK_STATE_READY E_TASK_STATE_READY
#define TASK_STATE_BLOCKED E_TASK_STATE_BLOCKED
#define TASK_STATE_SUSPENDED E_TASK_STATE_SUSPENDED
#define TASK_STATE_DELETED E_TASK_STATE_DELETED
#define TASK_STATE_INVALID E_TASK_STATE_INVALID
#define NOTIFY_ACTION_NONE E_NOTIFY_ACTION_NONE
#define NOTIFY_ACTION_BITS E_NOTIFY_ACTION_BITS
#define NOTIFY_ACTION_INCR E_NOTIFY_ACTION_INCR
#define NOTIFY_ACTION_OWRITE E_NOTIFY_ACTION_OWRITE
#define NOTIFY_ACTION_NO_OWRITE E_NOTIFY_ACTION_NO_OWRITE
#endif
#endif

/// @} Name: Simple enum names

/// \name Typedefs

/**
 * A [mutex.](@ref multitasking)
 * 
 * A mutex is a synchronization object that can be used to protect a shared
 * resource from being accessed by multiple tasks at the same time. A mutex can 
 * be claimed by a task, which will prevent other tasks from claiming it until
 * that task releases it.
 */
typedef void* mutex_t;

/// @} Name: Typedefs

/**
 * The task handle of the currently running task. 
 */
#ifdef __cplusplus
#define CURRENT_TASK ((pros::task_t)NULL)
#else
#define CURRENT_TASK ((task_t)NULL)
#endif

/// @} (add to group: c-rtos)

#ifdef __cplusplus
namespace c {
#endif

/// \ingroup c-rtos
/// \addtogroup c-rtos
/// @{

/**
 * Gets the number of milliseconds since PROS initialized.
 *
 * \return The number of milliseconds since PROS initialized
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   uint32_t now = millis();
 *   while (true) {
 *     // Do opcontrol things
 *     task_delay_until(&now, 2);
 *   }
 * }
 * \endcode
 */
uint32_t millis(void);

/**
 * Gets the number of microseconds since PROS initialized,
 * 
 * \return The number of microseconds since PROS initialized
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   uint64_t now = micros();
 *   while (true) {
 *     // Do opcontrol things
 *     task_delay_until(&now, 2000);
 *   }
 * }
 * \endcode
 */
uint64_t micros(void);

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
 *        Pointer to memory that will be used as a parameter for the task being
 *        created. This memory should not typically come from stack, but rather
 *        from dynamically (i.e., malloc'd) or statically allocated memory.
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
 * \return A handle by which the newly created task can be referenced. If an
 * error occurred, NULL will be returned and errno can be checked for hints as
 * to why task_create failed.
 * 
 * \b Example
 * \code
 * void my_task_fn(void* param) {
 *   printf("Hello %s\n", (char*)param);
 *   // ...
 * }
 * 
 * void initialize() {
 *   task_t my_task = task_create(my_task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
 *                               TASK_STACK_DEPTH_DEFAULT, "My Task");
 * }
 * \endcode
 */
task_t task_create(task_fn_t function, void* const parameters, uint32_t prio, const uint16_t stack_depth,
                   const char* const name);

/**
 * Removes a task from the RTOS real time kernel's management. The task being
 * deleted will be removed from all ready, blocked, suspended and event lists.
 *
 * Memory dynamically allocated by the task is not automatically freed, and
 * should be freed before the task is deleted.
 *
 * \param task
 *        The handle of the task to be deleted.  Passing NULL will cause the
 *        calling task to be deleted.
 * 
 * \b Example
 * \code
 * void my_task_fn(void* param) {
 *   printf("Hello %s\n", (char*)param);
 *   // ...
 * }
 * 
 * void initialize() {
 *   task_t my_task = task_create(my_task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
 *                            TASK_STACK_DEPTH_DEFAULT, "My Task");
 *   // Do other things
 *   task_delete(my_task);
 * }
 * \endcode
 */
void task_delete(task_t task);

/**
 * Delays the current task for a given number of milliseconds.
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
 *     task_delay(2);
 *   }
 * }
 * \endcode
 */
void task_delay(const uint32_t milliseconds);

/**
 * Delays the current task for a given number of milliseconds.
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
 * while (true) {
 *   // Do opcontrol things
 *   delay(2);
 *   }
 * }
 * \endcode
 */
void delay(const uint32_t milliseconds);

/**
 * Delays the current task until a specified time.  This function can be used 
 * by periodic tasks to ensure a constant execution frequency.
 *
 * The task will be woken up at the time *prev_time + delta, and *prev_time will
 * be updated to reflect the time at which the task will unblock.
 *
 * \param prev_time
 *        A pointer to the location storing the setpoint time. This should
 *        typically be initialized to the return value of millis().
 * \param delta
 *        The number of milliseconds to wait (1000 milliseconds per second)
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   uint32_t now = millis();
 *   while (true) {
 *     // Do opcontrol things
 *     task_delay_until(&now, 2);
 *   }
 * }
 * \endcode
 */
void task_delay_until(uint32_t* const prev_time, const uint32_t delta);

/**
 * Gets the priority of the specified task.
 *
 * \param task
 *        The task to check
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
 *   task_t my_task = task_create(my_task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
 *                            TASK_STACK_DEPTH_DEFAULT, "My Task");
 *   printf("Task Priority: %d\n", task_get_priority(my_task));
 * }
 * \endcode
 */
uint32_t task_get_priority(task_t task);

/**
 * Sets the priority of the specified task.
 *
 * If the specified task's state is available to be scheduled (e.g. not blocked)
 * and new priority is higher than the currently running task, a context switch
 * may occur.
 *
 * \param task
 *        The task to set
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
 *   task_t my_task = task_create(my_task_fn, NULL, TASK_PRIORITY_DEFAULT,
 *                             TASK_STACK_DEPTH_DEFAULT, "Example Task");
 *   task_set_priority(my_task, TASK_PRIORITY_DEFAULT + 1);
 * }
 * \endcode
 */
void task_set_priority(task_t task, uint32_t prio);

/**
 * Gets the state of the specified task.
 *
 * \param task
 *        The task to check
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
 *   task_t my_task = task_create(my_task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
 *                            TASK_STACK_DEPTH_DEFAULT, "My Task");
 *   printf("Task's State: %d\n", task_get_state(my_task));
 * }
 * \endcode
 */
task_state_e_t task_get_state(task_t task);

/**
 * Suspends the specified task, making it ineligible to be scheduled.
 *
 * \param task
 *        The task to suspend
 * 
 * \b Example
 * \code
 * mutex_t counter_mutex;
 * int counter = 0;
 * 
 * void my_task_fn(void* param) {
 *   while(true) {
 *     mutex_take(counter_mutex, TIMEOUT_MAX);// Mutexes are used for protecting shared resources
 *     counter++;
 *     mutex_give(counter_mutex);
 *     pros::delay(10);
 *   }
 * }
 * 
 * void opcontrol() {
 *   task_t task = task_create(my_task_fn, NULL, TASK_PRIORITY_DEFAULT,;
 * 
 *   while(true) {
 * 	   mutex_take(counter_mutex, TIMEOUT_MAX);
 *     if(counter > 100) {
 *       task_suspepend(task);
 * 	   }
 *     mutex_give(counter_mutex);
 *     pros::delay(10);
 *   }
 * }
 * \endcode
 */
void task_suspend(task_t task);

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
 *     delay(10);
 *   }
 * }
 * 
 * task_t task;
 * 
 * void initialize() {
 *   task = task_create(my_task_fn, NULL, TASK_PRIORITY_DEFAULT,
 * 					TASK_STACK_DEPTH_DEFAULT, "My Task");
 * }
 * 
 * void autonomous() {
 *   task_resume(task);
 * 
 *   // Run autonomous , then suspend the task so it doesn't interfere run
 *   
 *   // outside of autonomous or opcontrol
 *   task_suspend(task);
 * }
 * 
 * void opcontrol() {
 *   task_resume(task);
 *   // Opctonrol code here
 *   task_suspend(task);
 * }
 * 
 * \endcode
 */
void task_resume(task_t task);

/**
 * Gets the number of tasks the kernel is currently managing, including all
 * ready, blocked, or suspended tasks. A task that has been deleted, but not yet
 * reaped by the idle task will also be included in the count. Tasks recently
 * created may take one context switch to be counted.
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
 * void initialize() {
 *   task_t my_task = task_create(my_task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
 *                            TASK_STACK_DEPTH_DEFAULT, "My Task");
 *   printf("Number of Running Tasks: %d\n", task_get_count());
 * }
 * \endcode
 */
uint32_t task_get_count(void);

/**
 * Gets the name of the specified task.
 *
 * \param task
 *        The task to check
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
 *   task_t my_task = task_create(my_task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
 *                            TASK_STACK_DEPTH_DEFAULT, "My Task");
 *   printf("Task Name: %d\n", task_get_name(my_task));
 * }
 * \endcode
 */
char* task_get_name(task_t task);

/**
 * Gets a task handle from the specified name
 *
 * The operation takes a relatively long time and should be used sparingly.
 *
 * \param name
 *        The name to query
 *
 * \return A task handle with a matching name, or NULL if none were found.
 * 
 * \b Example
 * \code
 * void my_task_fn(void* param) {
 *   printf("Hello %s\n", (char*)param);
 *   // ...
 * }
 * 
 * void initialize() {
 *   task_t my_task = task_create(my_task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
 *                            TASK_STACK_DEPTH_DEFAULT, "My Task");
 *   // Do other things
 *   task_delete(task_get_by_name("My Task"));
 * }
 * \endcode
 */
task_t task_get_by_name(const char* name);

/**
 * Get the currently running task handle. This could be useful if a task
 * wants to tell another task about itself.
 *
 * \return The currently running task handle.
 * 
 * \b Example
 * \code
 * void my_task_fn(void* param) {
 *   task_t this_task = task_get_current();
 *   if (task_get_state(this_take) == E_TASK_STATE_RUNNING) {
 *     printf("This task is currently running\n");
 *   }
 * // ...
 * }
 * 
 * void initialize() {
 *   task_t my_task = task_create(my_task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
 *                            TASK_STACK_DEPTH_DEFAULT, "My Task");
 * }
 * \endcode
 */
task_t task_get_current();

/**
 * Sends a simple notification to task and increments the notification counter.
 *
 * \param task
 *        The task to notify
 *
 * \return Always returns true.
 * 
 * \b Example
 * \code
 * void my_task_fn(void* ign) {
 *   while(task_notify_take(true) == 0) {
 *     // Code while waiting
 *   }
 *   puts("I was unblocked!");
 * }
 * 
 * void opcontrol() {
 *   task_t my_task = task_create(my_task_fn, NULL, TASK_PRIORITY_DEFAULT,
 *                             TASK_STACK_DEPTH_DEFAULT, "Notify me! Task");
 *   while(true) {
 *     if(controller_get_digital(CONTROLLER_MASTER, DIGITAL_L1)) {
 *       task_notify(my_task);
 *     }
 *   }
 * }
 * \endcode
 */
uint32_t task_notify(task_t task);

/**
 * 
 * Utilizes task notifications to wait until specified task is complete and deleted,
 * then continues to execute the program. Analogous to std::thread::join in C++.
 * 
 * \param task
 *        The handle of the task to wait on.
 * 
 * \return void
 * 
 * \b Example
 * \code
 * void my_task_fn(void* ign) {
 *   lcd_print(1, "%s running", task_get_name(NULL));
 *   task_delay(1000);
 *   lcd_print(2, "End of %s", task_get_name(NULL));
 * }
 * 
 * void opcontrol() {
 *   task_t my_task = task_create(my_task_fn, NULL, TASK_PRIORITY_DEFAULT,
 *                             TASK_STACK_DEPTH_DEFAULT, "Example Task");
 *   lcd_set_text(0, "Running task.");
 *   task_join(my_task);
 *   lcd_set_text(3, "Task completed.");
 * }
 * \endcode
 */
void task_join(task_t task);

/**
 * Sends a notification to a task, optionally performing some action. Will also
 * retrieve the value of the notification in the target task before modifying
 * the notification value.
 *
 * \param task
 *        The task to notify
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
 *   while(true) {
 *     // Wait until we have been notified 20 times before running the code
 *     if(task_notify_take(false, TIMEOUT_MAX) == 20) {
 *       // ... Code to do stuff here ...
 * 
 *       // Reset the notification counter
 *       task_notify_take(true, TIMEOUT_MAX);
 *     }
 *     delay(10);
 * 	 }
 * }
 * 
 * void opcontrol() {
 * 	 task_t task = task_create(my_task_fn, NULL, TASK_PRIORITY_DEFAULT,
 * 						   TASK_STACK_DEPTH_DEFAULT, "My Task");
 *   
 *   int count = 0;
 *   
 *   while(true) {
 *     if(controller_get_digital(CONTROLLER_MASTER, DIGITAL_L1)) {
 *       task_notify_ext(task, 1, NOTIFY_ACTION_INCREMENT, &count);
 *     }
 *     
 *     delay(20);
 *   }
 * }
 * \endcode
 */
uint32_t task_notify_ext(task_t task, uint32_t value, notify_action_e_t action, uint32_t* prev_value);

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
 *   task_t current_task = task_get_current();
 *   while(task_notify_take(current_task, true, TIMEOUT_MAX)) {
 *     puts("I was unblocked!");
 *   }
 * }
 * 
 * void opcontrol() {
 *   task_t my_task = task_create(my_task_fn, NULL, TASK_PRIORITY_DEFAULT,
 *                             TASK_STACK_DEPTH_DEFAULT, "Notify me! Task");
 *   while(true) {
 *     if(controller_get_digital(CONTROLLER_MASTER, DIGITAL_L1)) {
 *       task_notify(my_task);
 *     }
 *   }
 * }
 * \endcode
 */
uint32_t task_notify_take(bool clear_on_exit, uint32_t timeout);

/**
 * Clears the notification for a task.
 *
 * See https://pros.cs.purdue.edu/v5/tutorials/topical/notifications.html for
 * details.
 *
 * \param task
 *        The task to clear
 *
 * \return False if there was not a notification waiting, true if there was
 * 
 * \b Example
 * \code
 * void my_task_fn(void* param) {
 *   task_t task = task_get_current();
 *   while(true) {
 *     printf("Waiting for notification...\n");
 * 	   printf("Got a notification: %d\n", task_notify_take(task, false, TIMEOUT_MAX));
 * 
 * 	   task_notify_clear(task);
 *     delay(10):
 *   }
 * }
 * 
 * void opcontrol() {
 *  task_t task = task_create(my_task_fn, NULL, TASK_PRIORITY_DEFAULT,
 *                            TASK_STACK_DEPTH_DEFAULT, "My Task");
 * 
 *   while(true) {
 *     if(controller_get_digital(CONTROLLER_MASTER, DIGITAL_L1)) {
 *       task_notify(task);
 *     }
 *     delay(10);
 *   }
 * }
 * \endcode
 */
bool task_notify_clear(task_t task);

/**
 * Creates a mutex.
 *
 * See https://pros.cs.purdue.edu/v5/tutorials/topical/multitasking.html#mutexes
 * for details.
 *
 * \return A handle to a newly created mutex. If an error occurred, NULL will be
 * returned and errno can be checked for hints as to why mutex_create failed.
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
 * mutex_t odom_mutex;
 * 
 * void odom_task(void* param) {
 *   while(true) {
 *     // First we fetch the odom coordinates from the previous iteration of the
 *     // odometry task. These are put into local variables so that we can
 *     // keep the size of the critical section as small as possible. This lets
 *     // other tasks that need to use the odometry data run until we need to
 *     // update it again.
 *     mutex_take(odom_mutex, MAX_DELAY);
 *     double x_old = odom_x;
 *     double y_old = odom_y;
 *     double heading_old = odom_heading;
 * 	   mutex_give(odom_mutex);
 * 
 *     double x_new = 0.0;
 *     double y_new = 0.0;
 *     double heading_new = 0.0;
 *     
 *     // --- Calculate new pose for the robot here ---
 * 
 *     // Now that we have the new pose, we can update the global variables
 *     mutex_take(odom_mutex, MAX_DELAY);
 *     odom_x = x_new;
 *     odom_y = y_new;
 *     odom_heading = heading_new;
 *     mutex_give(odom_mutex);
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
 *     mutex_take(odom_mutex, MAX_DELAY);
 *     double current_x = odom_x;
 *     double current_y = odom_y;
 *     double current_heading = odom_heading;
 *     mutex_give(odom_mutex);
 *     
 *     // ---- Move the robot using the current locations goes here ----
 *     
 *     delay(10);
 *   }
 * }
 * 
 * void initialize() {
 *   odom_mutex = mutex_create();
 * 
 *   task_create(odom_task, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Odometry Task");
 *   task_create(chassis_task, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Chassis Task");
 * }
 * \endcode
 */
mutex_t mutex_create(void);

/**
 * Takes and locks a mutex, waiting for up to a certain number of milliseconds
 * before timing out.
 *
 * See https://pros.cs.purdue.edu/v5/tutorials/topical/multitasking.html#mutexes
 * for details.
 *
 * \param mutex
 *        Mutex to attempt to lock.
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
 * mutex_t odom_mutex;
 * 
 * void odom_task(void* param) {
 *   while(true) {
 *     // First we fetch the odom coordinates from the previous iteration of the
 *     // odometry task. These are put into local variables so that we can
 *     // keep the size of the critical section as small as possible. This lets
 *     // other tasks that need to use the odometry data run until we need to
 *     // update it again.
 *     mutex_take(odom_mutex, MAX_DELAY);
 *     double x_old = odom_x;
 *     double y_old = odom_y;
 *     double heading_old = odom_heading;
 * 	   mutex_give(odom_mutex);
 * 
 *     double x_new = 0.0;
 *     double y_new = 0.0;
 *     double heading_new = 0.0;
 *     
 *     // --- Calculate new pose for the robot here ---
 * 
 *     // Now that we have the new pose, we can update the global variables
 *     mutex_take(odom_mutex, MAX_DELAY);
 *     odom_x = x_new;
 *     odom_y = y_new;
 *     odom_heading = heading_new;
 *     mutex_give(odom_mutex);
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
 *     mutex_take(odom_mutex, MAX_DELAY);
 *     double current_x = odom_x;
 *     double current_y = odom_y;
 *     double current_heading = odom_heading;
 *     mutex_give(odom_mutex);
 *     
 *     // ---- Move the robot using the current locations goes here ----
 *     
 *     delay(10);
 *   }
 * }
 * 
 * void initialize() {
 *   odom_mutex = mutex_create();
 * 
 *   task_create(odom_task, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Odometry Task");
 *   task_create(chassis_task, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Chassis Task");
 * }
 * \endcode
 */
bool mutex_take(mutex_t mutex, uint32_t timeout);

/**
 * Unlocks a mutex.
 *
 * See https://pros.cs.purdue.edu/v5/tutorials/topical/multitasking.html#mutexes
 * for details.
 *
 * \param mutex
 *        Mutex to unlock.
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
 * mutex_t odom_mutex;
 * 
 * void odom_task(void* param) {
 *   while(true) {
 *     // First we fetch the odom coordinates from the previous iteration of the
 *     // odometry task. These are put into local variables so that we can
 *     // keep the size of the critical section as small as possible. This lets
 *     // other tasks that need to use the odometry data run until we need to
 *     // update it again.
 *     mutex_take(odom_mutex, MAX_DELAY);
 *     double x_old = odom_x;
 *     double y_old = odom_y;
 *     double heading_old = odom_heading;
 * 	   mutex_give(odom_mutex);
 * 
 *     double x_new = 0.0;
 *     double y_new = 0.0;
 *     double heading_new = 0.0;
 *     
 *     // --- Calculate new pose for the robot here ---
 * 
 *     // Now that we have the new pose, we can update the global variables
 *     mutex_take(odom_mutex, MAX_DELAY);
 *     odom_x = x_new;
 *     odom_y = y_new;
 *     odom_heading = heading_new;
 *     mutex_give(odom_mutex);
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
 *     mutex_take(odom_mutex, MAX_DELAY);
 *     double current_x = odom_x;
 *     double current_y = odom_y;
 *     double current_heading = odom_heading;
 *     mutex_give(odom_mutex);
 *     
 *     // ---- Move the robot using the current locations goes here ----
 *     
 *     delay(10);
 *   }
 * }
 * 
 * void initialize() {
 *   odom_mutex = mutex_create();
 * 
 *   task_create(odom_task, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Odometry Task");
 *   task_create(chassis_task, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Chassis Task");
 * }
 * \endcode
 */
bool mutex_give(mutex_t mutex);

/**
 * Deletes a mutex
 *
 * \param mutex
 *        Mutex to unlock.
 * 
 * \b Example
 * \code
 * mutex_t mutex = mutex_create();
 * // Acquire the mutex; other tasks using this command will wait until the mutex is released
 * // timeout can specify the maximum time to wait, or MAX_DELAY to wait forever
 * // If the timeout expires, "false" will be returned, otherwise "true"
 * mutex_take(mutex, MAX_DELAY);
 * // do some work
 * // Release the mutex for other tasks
 * mutex_give(mutex);
 * // Delete the mutex
 * mutex_delete(mutex);
 * \endcode
 */
void mutex_delete(mutex_t mutex);

/// @} Add to group: c-rtos

#ifdef __cplusplus
}  // namespace c
}  // namespace pros
}
#endif

#endif  // _PROS_RTOS_H_
