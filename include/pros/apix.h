/**
 * \file pros/apix.h
 * \ingroup apix
 *
 * PROS Extended API header
 *
 * Contains additional declarations for use by advaned users of PROS. These
 * functions do not typically have as much error handling or require deeper
 * knowledge of real time operating systems.
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
 * \defgroup apix Extended API
 * \note Also included in the Extended API is [LVGL.](https://lvgl.io/)
 */

#ifndef _PROS_API_EXTENDED_H_
#define _PROS_API_EXTENDED_H_

#include "api.h"
#include "pros/device.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic pop
#include "pros/serial.h"

#ifdef __cplusplus
#include "pros/serial.hpp"
namespace pros::c {
extern "C" {
#endif

/**
 * \ingroup apix
 */

/**
 * \addtogroup apix
 *  @{
 */

/// \name RTOS Facilities
///@{

typedef void* queue_t;
typedef void* sem_t;

/**
 * Unblocks a task in the Blocked state (e.g. waiting for a delay, on a
 * semaphore, etc.).
 *
 * \param task
 *        The task to unblock
 *
 * \return True if the task was unblocked, false otherwise
 * 
 * \b Example:
 * \code
 * task_t task = task_create(task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
 *                           TASK_STACK_DEPTH_DEFAULT, "task_fn");
 * task_delay(1000);
 * // in another task somewhere else, this will abort the task_delay bove:
 * task_abort_delay(task);
 * \endcode
 */
bool task_abort_delay(task_t task);

/**
 * Notify a task when a target task is being deleted.
 *
 * \param target_task
 *				The task being watched for deletion
 * \param task_to_notify
 *        The task to notify when target_task is deleted
 * \param value
 *				The value to supply to task_notify_ext
 * \param notify_action
 * 				The action to supply to task_notify_ext
 * 
    * \b Example:
    * \code
    * task_t task_to_delete = task_create(task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
    *                           TASK_STACK_DEPTH_DEFAULT, "task_fn");
    * task_t task_to_notify = task_create(task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
    *                           TASK_STACK_DEPTH_DEFAULT, "task_fn2");
    * 
    * task_notify_ext(task_to_notify, 0, NOTIFY_ACTION_INCREMENT, NULL);
    * 
    * task_notify_when_deleting(task_to_delete, task_get_current(), 0, NOTIFY_ACTION_NONE);
    * task_delete(task_to_delete);
    * \endcode
 */
void task_notify_when_deleting(task_t target_task, task_t task_to_notify, uint32_t value,
                               notify_action_e_t notify_action);

/**
 * Creates a recursive mutex which can be locked recursively by the owner.
 *
 * \return A newly created recursive mutex.
 * 
 * \b Example:
 * \code
 * mutex_t mutex = mutex_recursive_create();
 * 
 * void task_fn(void* param) {
 *   while(1) {
 *     mutex_recursive_take(mutex, 1000);
 *     // critical section
 *     mutex_recursive_give(mutex);
 *     task_delay(1000);
 *   }
 * }
 * task_create(task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
 *                           TASK_STACK_DEPTH_DEFAULT, "task_fn");
 *
 * \endcode
 */
mutex_t mutex_recursive_create(void);

/**
 * Takes a recursive mutex.
 *
 * \param mutex
 *        A mutex handle created by mutex_recursive_create
 * \param wait_time
 *        Amount of time to wait before timing out
 *
 * \return 1 if the mutex was obtained, 0 otherwise
 * 
 * \b Example:
 * \code
 * mutex_t mutex = mutex_recursive_create();
 *
 * void task_fn(void* param) {
 *   while(1) {
 *     mutex_recursive_take(mutex, 1000);
 *     // critical section
 *     mutex_recursive_give(mutex);
 *     task_delay(1000);
 *   }
 * }
 * task_create(task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
 *                           TASK_STACK_DEPTH_DEFAULT, "task_fn");
 *
 * \endcode
 */
bool mutex_recursive_take(mutex_t mutex, uint32_t timeout);

/**
 * Gives a recursive mutex.
 *
 * \param mutex
 *        A mutex handle created by mutex_recursive_create
 *
 * \return 1 if the mutex was obtained, 0 otherwise
 * 
 * \b Example:
 * \code
 * mutex_t mutex = mutex_recursive_create();
 *
 * void task_fn(void* param) {
 *   while(1) {
 *     mutex_recursive_take(mutex, 1000);
 *     // critical section
 *     mutex_recursive_give(mutex);
 *     task_delay(1000);
 *   }
 * }
 * task_create(task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
 *                           TASK_STACK_DEPTH_DEFAULT, "task_fn");
 *
 * \endcode
 */
bool mutex_recursive_give(mutex_t mutex);

/**
 * Returns a handle to the current owner of a mutex.
 *
 * \param mutex
 *        A mutex handle
 *
 * \return A handle to the current task that owns the mutex, or NULL if the
 * mutex isn't owned.
 * 
 * \b Example:
 * \code
 * mutex_t mutex = mutex_create();
 *
 * void task_fn(void* param) {
 *   while(1) {
 *     mutex_take(mutex, 1000);
 *     // critical section
 *     mutex_give(mutex);
 *     task_delay(1000);
 *   }
 * }
 * task_create(task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
 *                           TASK_STACK_DEPTH_DEFAULT, "task_fn");
 *
 * void opcontrol(void) {
*   while (1) {
*     if (joystick_get_digital(1, 7, JOY_UP)) {
*       task_t owner = mutex_get_owner(mutex);
*       if (owner != NULL) {
*         printf("Mutex is owned by task %s", task_get_name(owner));
*       } else {
*         printf("Mutex is not owned");
*       }
*     }
*     task_delay(20);
*   }
* }
 * \endcode
 */
task_t mutex_get_owner(mutex_t mutex);

/**
 * Creates a counting sempahore.
 *
 * \param max_count
 *        The maximum count value that can be reached.
 * \param init_count
 *        The initial count value assigned to the new semaphore.
 *
 * \return A newly created semaphore. If an error occurred, NULL will be
 * returned and errno can be checked for hints as to why sem_create failed.
 *
 * \b Example:
 * \code
 * // Binary semaphore acts as a mutex
 * sem_t sem = sem_create(1, 0);
 *
 * void task_fn(void* param) {
 *   while(1) {
 *     sem_take(sem, 1000);
 *     // critical section
 *     sem_give(sem);
 *     task_delay(1000);
 *   }
 * }
 * task_create(task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
 *                           TASK_STACK_DEPTH_DEFAULT, "task_fn");
 *
 * \endcode
 */
sem_t sem_create(uint32_t max_count, uint32_t init_count);

/**
 * Deletes a semaphore (or binary semaphore)
 *
 * \param sem
 * 			  Semaphore to delete
 *
 * \b Example:
 * \code
 * // Binary semaphore acts as a mutex
 * sem_t sem = sem_create(1, 0);
 * 
 * void task_fn(void* param) { 
 *   while(1) {
 *     sem_take(sem, 1000);
 *     // critical section
 *     sem_give(sem);
 *     task_delay(1000);
 *   }
 * }
 * task_create(task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
 *                           TASK_STACK_DEPTH_DEFAULT, "task_fn");
 *
 * void opcontrol(void) {
 *   while (1) {
 *     if (joystick_get_digital(1, 7, JOY_UP)) {
 *       // honestly this is a bad example because you should never 
 *       // delete a semaphore like this
 *       sem_delete(sem);
 *     }
 *     task_delay(20);
 *   }
 * }
 * 
 * \endcode
 */
void sem_delete(sem_t sem);

/**
 * Creates a binary semaphore.
 *
 * \return A newly created semaphore.
 * 
 * \b Example:
 * \code
 * // Binary semaphore acts as a mutex
 * sem_t sem = sem_binary_create();
 *
 * void task_fn(void* param) {
 *   while(1) {
 *     sem_take(sem, 1000);
 *     // critical section
 *     sem_give(sem);
 *     task_delay(1000);
 *   }
 * }
 * task_create(task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
 *                           TASK_STACK_DEPTH_DEFAULT, "task_fn");
 *
 * \endcode
 */
sem_t sem_binary_create(void);

/**
 * Waits for the semaphore's value to be greater than 0. If the value is already
 * greater than 0, this function immediately returns.
 *
 * \param sem
 *        Semaphore to wait on
 * \param timeout
 *        Time to wait before the semaphore's becomes available. A timeout of 0
 *        can be used to poll the sempahore. TIMEOUT_MAX can be used to block
 *        indefinitely.
 *
 * \return True if the semaphore was successfully take, false otherwise. If
 * false is returned, then errno is set with a hint about why the sempahore
 * couldn't be taken.
 * 
 * \b Example:
 * \code
 * // Binary semaphore acts as a mutex
 * sem_t sem = sem_create(1, 0);
 *
 * void task_fn(void* param) {
 *   while(1) {
 *     if(!sem_wait(sem, 1000)) {
 *       printf("Failed to take semaphore");
 *       task_delay(1000);
 *       continue;
 *     }
 *     // critical section
 *     sem_give(sem);
 *     task_delay(1000);
 *   }
 * }
 * task_create(task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
 *                           TASK_STACK_DEPTH_DEFAULT, "task_fn");
 *
 * void opcontrol(void) {
 *   while (1) {
 *     if (sem_wait(sem, 0))) {
 *       printf("Semaphore is available");
 *     }
 *     task_delay(20);
 *   }
 * }
 * \endcode
 */
bool sem_wait(sem_t sem, uint32_t timeout);

/**
 * Increments a semaphore's value.
 *
 * \param sem
 *        Semaphore to post
 *
 * \return True if the value was incremented, false otherwise. If false is
 * returned, then errno is set with a hint about why the semaphore couldn't be
 * taken.
 * 
 * \b Example:
 * \code
 * // Binary semaphore acts as a mutex
 * sem_t sem = sem_create(1, 0);
 *
 * void task_fn(void* param) {
 *   while(1) {
 *     sem_post(sem); // increments, mimicking to "claim"
 *     // critical section
 *     sem_give(sem);
 *     task_delay(1000);
 *   }
 * }
 * task_create(task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT,
 *                           TASK_STACK_DEPTH_DEFAULT, "task_fn");
 *
 * \endcode
 */
bool sem_post(sem_t sem);

/**
 * Returns the current value of the semaphore.
 *
 * \param sem
 *        A semaphore handle
 *
 * \return The current value of the semaphore (e.g. the number of resources
 * available)
 * 
 * \b Example of sem_get_count:
 * \code
 * // Binary semaphore acts as a mutex
 * sem_t sem = sem_create(1, 0);
 * printf("semaphore count: %d", sem_get_count(sem));
 *  // semaphore count: 0
 * sem_take(sem, 1000);
 * printf("semaphore count: %d", sem_get_count(sem));
 *  // semaphore count: 1
 * sem_give(sem);
 * printf("semaphore count: %d", sem_get_count(sem));
 *  // semaphore count: 0
 *
 * \endcode
 */
uint32_t sem_get_count(sem_t sem);

/**
 * Creates a queue.
 *
 * \param length
 *        The maximum number of items that the queue can contain.
 * \param item_size
 *        The number of bytes each item in the queue will require.
 *
 * \return A handle to a newly created queue, or NULL if the queue cannot be
 * created.
 * 
 * \b Example:
 * \code
 * void opcontrol(void) {
 *   queue_t queue = queue_create(10, sizeof(int));
 *   int item[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
 *   queue_prepend(queue, item, 1000);
 *   queue_append(queue, item, 1000);
 *   printf("queue length: %d", queue_get_length(queue));
 * }
 * \endcode
 */
queue_t queue_create(uint32_t length, uint32_t item_size);

/**
 * Posts an item to the front of a queue. The item is queued by copy, not by
 * reference.
 *
 * \param queue
 *        The queue handle
 * \param item
 *        A pointer to the item that will be placed on the queue.
 * \param timeout
 *        Time to wait for space to become available. A timeout of 0 can be used
 *        to attempt to post without blocking. TIMEOUT_MAX can be used to block
 *        indefinitely.
 *
 * \return True if the item was preprended, false otherwise.
 *
 * \b Example:
 * \code
 * void opcontrol(void) {
 *   queue_t queue = queue_create(10, sizeof(int));
 *   int item[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
 *   queue_prepend(queue, item, 1000);
 *   queue_append(queue, item, 1000);
 *   printf("queue length: %d", queue_get_length(queue));
 * }
 */
bool queue_prepend(queue_t queue, const void* item, uint32_t timeout);

/**
 * Posts an item to the end of a queue. The item is queued by copy, not by
 * reference.
 *
 * \param queue
 *        The queue handle
 * \param item
 *        A pointer to the item that will be placed on the queue.
 * \param timeout
 *        Time to wait for space to become available. A timeout of 0 can be used
 *        to attempt to post without blocking. TIMEOUT_MAX can be used to block
 *        indefinitely.
 *
 * \return True if the item was preprended, false otherwise.
 * 
 * \b Example:
 * \code
 * void opcontrol(void) {
 *   queue_t queue = queue_create(10, sizeof(int));
 *   int item[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
 *   queue_prepend(queue, item, 1000);
 *   queue_append(queue, item, 1000);
 *   printf("queue length: %d", queue_get_length(queue));
 * }
 * \endcode
 */
bool queue_append(queue_t queue, const void* item, uint32_t timeout);

/**
 * Receive an item from a queue without removing the item from the queue.
 *
 * \param queue
 *        The queue handle
 * \param buffer
 *        Pointer to a buffer to which the received item will be copied
 * \param timeout
 *        The maximum amount of time the task should block waiting for an item to receive should the queue be empty at
 *        the time of the call. TIMEOUT_MAX can be used to block indefinitely.
 *
 * \return True if an item was copied into the buffer, false otherwise.
 * 
 * \b Example:
 * \code
 * void opcontrol(void) {
 *   queue_t queue = queue_create(10, sizeof(int));
 *   char* item = "Hello! this is a test";
 *   queue_prepend(queue, item, 1000);
 *   queue_append(queue, item, 1000);
 *   char* recv = malloc(sizeof("Hello! this is a test"));
 *   queue_peek(queue, recv, 1000);
 *   printf("Queue: %s", recv);
 *   free(recv);
 * }
 * \endcode
 */
bool queue_peek(queue_t queue, void* const buffer, uint32_t timeout);

/**
 * Receive an item from the queue.
 *
 * \param queue
 *        The queue handle
 * \param buffer
 *        Pointer to a buffer to which the received item will be copied
 * \param timeout
 *        The maximum amount of time the task should block
 *        waiting for an item to receive should the queue be empty at the time
 *        of the call. queue_recv() will return immediately if timeout
 *        is zero and the queue is empty.
 *
 * \return True if an item was copied into the buffer, false otherwise.
 *
 * \b Example:
 * \code
 * void opcontrol(void) {
 *   queue_t queue = queue_create(10, sizeof(int));
 *   char* item = "Hello! this is a test";
 *   queue_prepend(queue, item, 1000);
 *   queue_append(queue, item, 1000);
 *   char* recv = malloc(sizeof("Hello! this is a test"));
 *   queue_recv(queue, recv, 1000);
 *   printf("Queue: %s", recv);
 *   free(recv);
 * }
 * \endcode
 */
bool queue_recv(queue_t queue, void* const buffer, uint32_t timeout);

/**
 * Return the number of messages stored in a queue.
 *
 * \param queue
 *        The queue handle.
 *
 * \return The number of messages available in the queue.
 *
 * \b Example:
 * \code
 * void opcontrol(void) {
 *   queue_t queue = queue_create(10, sizeof(int));
 * 
 *   int item[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
 *   queue_prepend(queue, item, 1000);
 *   queue_append(queue, item, 1000);
 *   printf("queue waiting: %d", queue_get_waiting(queue));
 * }
 * \endcode
 */
uint32_t queue_get_waiting(const queue_t queue);

/**
 * Return the number of spaces left in a queue.
 *
 * \param queue
 *        The queue handle.
 *
 * \return The number of spaces available in the queue.
 *
 * \b Example:
 * \code
 * void opcontrol(void) {
 *   queue_t queue = queue_create(10, sizeof(int));
 * 
 *   int item[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
 *   queue_prepend(queue, item, 1000);
 *   queue_append(queue, item, 1000);
 *   printf("queue available: %d", queue_get_available(queue));
 * }
 * \endcode
 */
uint32_t queue_get_available(const queue_t queue);

/**
 * Delete a queue.
 *
 * \param queue
 *        Queue handle to delete
 *
 * \b Example:
 * \code
 * void opcontrol(void) {
 *   queue_t queue = queue_create(10, sizeof(int));
 *   queue_delete(queue);
 * }
 * \endcode
 */
void queue_delete(queue_t queue);

/**
 * Resets a queue to an empty state
 *
 * \param queue
 *        Queue handle to reset
 * 
 * \b Example:
 * \code
 * void opcontrol(void) {
 *   queue_t queue = queue_create(10, sizeof(int));
 *   int item[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
 *   queue_prepend(queue, item, 1000);
 *   queue_append(queue, item, 1000);
 *   queue_reset(queue);
 * }
 * \endcode
 */
void queue_reset(queue_t queue);

///@}

/// \name Device Registration
///@{

/**
 * Registers a device in the given zero-indexed port
 *
 * Registers a device of the given type in the given port into the registry, if
 * that type of device is detected to be plugged in to that port.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (0-20), or a
 * a different device than specified is plugged in.
 * EADDRINUSE - The port is already registered to another device.
 *
 * \param port
 *        The port number to register the device
 * \param device
 *        The type of device to register
 *
 * \return 1 upon success, PROS_ERR upon failure
 * 
 * \b Example:
 * \code
 * void opcontrol(void) {
 *   registry_bind_port(1, E_DEVICE_MOTOR);
 * }
 * \endcode
 */
int registry_bind_port(uint8_t port, v5_device_e_t device_type);

/**
 * Deregisters a devices from the given zero-indexed port
 *
 * Removes the device registed in the given port, if there is one.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (0-20).
 *
 * \param port
 *        The port number to deregister
 *
 * \return 1 upon success, PROS_ERR upon failure
 *
 * \b Example:
 * \code
 * void opcontrol(void) {
 *   registry_bind_port(1, E_DEVICE_MOTOR);
 *   registry_unbind_port(1);
 * }
 * \endcode
 */
int registry_unbind_port(uint8_t port);

/**
 * Returns the type of device registered to the zero-indexed port.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (0-20).
 *
 * \param port
 *        The V5 port number from 0-20
 *
 * \return The type of device that is registered into the port (NOT what is
 * plugged in)
 *
 * \b Example:
 * \code
 * void opcontrol(void) {
 *   registry_bind_port(1, E_DEVICE_MOTOR);
 *   printf("port 1 is registered to a motor: %d", registry_get_bound_type(1) == E_DEVICE_MOTOR);
 * }
 * \endcode
 */
v5_device_e_t registry_get_bound_type(uint8_t port);

/**
 * Returns the type of the device plugged into the zero-indexed port.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (0-20).
 *
 * \param port
 *        The V5 port number from 0-20
 *
 * \return The type of device that is plugged into the port (NOT what is
 * registered)
 *
 * \b Example:
 * \code
 * void opcontrol(void) {
 *   registry_bind_port(1, E_DEVICE_MOTOR);
 *   printf("port 1 is registered to a motor: %d", registry_get_plugged_type(1) == E_DEVICE_MOTOR);
 * }
 * \endcode
 */
v5_device_e_t registry_get_plugged_type(uint8_t port);

///@}

/// \name Filesystem
///@{

/**
 * Control settings of the serial driver.
 *
 * \param action
 * 			An action to perform on the serial driver. See the SERCTL_* macros for
 * 			details on the different actions.
 * \param extra_arg
 * 			An argument to pass in based on the action
 * 
 * \b Example:
 * \code
 * void opcontrol(void) {
 *   serctl(SERCTL_SET_BAUDRATE, (void*) 9600);
 * }
 */
int32_t serctl(const uint32_t action, void* const extra_arg);

/*
 * Control settings of the microSD card driver.
 *
 * \param action
 * 			An action to perform on the microSD card driver. See the USDCTL_* macros
 *      for details on the different actions.
 * \param extra_arg
 * 		   	An argument to pass in based on the action
 */
// Not yet implemented
// int32_t usdctl(const uint32_t action, void* const extra_arg);

/**
 * Control settings of the way the file's driver treats the file
 *
 * \param file
 * 			A valid file descriptor number
 * \param action
 * 			An action to perform on the file's driver. See the *CTL_* macros for
 * 			details on the different actions. Note that the action passed in must
 *      match the correct driver (e.g. don't perform a SERCTL_* action on a
 *      microSD card file)
 * \param extra_arg
 * 		  	An argument to pass in based on the action
 *
 * \b Example:
 * \code
 * void opcontrol(void) {
 *   int32_t fd = open("serial", O_RDWR);
 *   fdctl(fd, SERCTL_SET_BAUDRATE, (void*) 9600);
 * }
 * \endcode
 */
int32_t fdctl(int file, const uint32_t action, void* const extra_arg);

/**
 * Sets the reverse flag for the motor.
 *
 * This will invert its movements and the values returned for its position.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1-21
 * \param reverse
 *        True reverses the motor, false is default
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * void autonomous() {
 *   motor_set_reversed(1, true);
 *   printf("Is this motor reversed? %d\n", motor_is_reversed(1));
 * }
 * \endcode
 */
int32_t motor_set_reversed(int8_t port, const bool reverse);

/**
 * Gets the operation direction of the motor as set by the user.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return 1 if the motor has been reversed and 0 if the motor was not reversed,
 * or PROS_ERR if the operation failed, setting errno.
 * 
 * \b Example
 * \code
 * void initialize() {
 *   printf("Is the motor reversed? %d\n", motor_is_reversed(1));
 *   // Prints "Is the motor reversed? 0"
 * }
 * \endcode
 */
int32_t motor_is_reversed(int8_t port);

/**
 * Action macro to pass into serctl or fdctl that activates the stream
 * identifier.
 *
 * When used with serctl, the extra argument must be the little endian
 * representation of the stream identifier (e.g. "sout" -> 0x74756f73)
 *
 */
#define SERCTL_ACTIVATE 10

/**
 * Action macro to pass into serctl or fdctl that deactivates the stream
 * identifier.
 *
 * When used with serctl, the extra argument must be the little endian
 * representation of the stream identifier (e.g. "sout" -> 0x74756f73)
 *
 */
#define SERCTL_DEACTIVATE 11

/**
 * Action macro to pass into fdctl that enables blocking writes for the file
 *
 * The extra argument is not used with this action, provide any value (e.g.
 * NULL) instead
 *
 */
#define SERCTL_BLKWRITE 12

/**
 * Action macro to pass into fdctl that makes writes non-blocking for the file
 *
 * The extra argument is not used with this action, provide any value (e.g.
 * NULL) instead
 *
 */
#define SERCTL_NOBLKWRITE 13

/**
 * Action macro to pass into serctl that enables advanced stream multiplexing
 * capabilities
 *
 * The extra argument is not used with this action, provide any value (e.g.
 * NULL) instead
 *
 */
#define SERCTL_ENABLE_COBS 14

/**
 * Action macro to pass into serctl that disables advanced stream multiplexing
 * capabilities
 *
 * The extra argument is not used with this action, provide any value (e.g.
 * NULL) instead
 *
 */
#define SERCTL_DISABLE_COBS 15

/**
 * Action macro to check if there is data available from the Generic Serial
 * Device
 *
 */
#define DEVCTL_FIONREAD 16

/**
 * Action macro to check if there is space available in the Generic Serial
 * Device's output buffer
 *
 */
#define DEVCTL_FIONWRITE 18

/**
 * Action macro to set the Generic Serial Device's baudrate.
 *
 * The extra argument is the baudrate.
 */
#define DEVCTL_SET_BAUDRATE 17

///@}

///@}

#ifdef __cplusplus
}
}
#endif

#endif  // _PROS_API_EXTENDED_H_
