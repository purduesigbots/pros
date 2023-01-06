/**
 * \file kapi.h
 *
 * Kernel API header
 *
 * Contains additional declarations for use internally within kernel
 * development. This file includes the FreeRTOS header, which allows for
 * creation of statically allocated FreeRTOS primitives like tasks, semaphores,
 * and queues.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "api.h"
#include "pros/apix.h"
#include "rtos/FreeRTOS.h"
#include "rtos/stream_buffer.h"

#ifdef __cplusplus
extern "C" {
#define task_t pros::task_t
#define task_fn_t pros::task_fn_t
#define mutex_t pros::mutex_t
#define sem_t pros::c::sem_t
#define queue_t pros::c::queue_t
#endif

#define KDBG_FILENO 3

#define warn_printf(fmt, ...) dprintf(STDERR_FILENO, "%s:%d -- " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define warn_wprint(str) wprintf("%s", str)

#define kprintf(fmt, ...) dprintf(KDBG_FILENO, "%s:%d -- " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define kprint(str) kprintf("%s", str)

#ifndef PROS_RELEASING
#define kassert(cond)                     \
	do {                                    \
		if (!(cond)) {                        \
			kprint("Assertion failed: " #cond); \
		}                                     \
	} while (0)
#else
#define kassert(cond)
#endif

typedef uint32_t task_stack_t;

/**
 * Suspends the scheduler without disabling interrupts. context switches will
 * not occur while the scheduler is suspended. RTOS ticks that occur while the
 * scheduler is suspended will be held pending until the scheduler has been
 * unsuspended with rtos_resume_all()
 *
 * When used correctly, this function ensures that operations occur atomically
 * w.r.t. multitasking. Functions like task_delay, queue_send, and other
 * functions MUST NOT be called while the scheduler is disabled.
 */
void rtos_suspend_all(void);

/**
 * Resumes the scheduler. It does not resume unsuspended tasks that were
 * previously suspended by task_suspend.
 *
 * if(rtos_resume_all()) {
 *     task_delay(0); // force context switch
 * }
 * \return True if a context switch is necessary.
 */
int32_t rtos_resume_all(void);

/**
 * Creates a task using statically allocated buffers. All tasks used by the PROS
 * system must use statically allocated buffers.
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
 */
task_t task_create_static(task_fn_t task_code, void* const param, uint32_t priority, const size_t stack_size,
                          const char* const name, task_stack_t* const stack_buffer, static_task_s_t* const task_buffer);

/**
 * Creates a statically allocated mutex.
 *
 * All FreeRTOS primitives must be created statically if they are required for
 * operation of the kernel.
 *
 * \param[out] mutex_buffer
 *             A buffer to store the mutex in
 *
 * \return A handle to a newly created mutex. If an error occurred, NULL will be
 * returned and errno can be checked for hints as to why mutex_create failed.
 */
mutex_t mutex_create_static(static_sem_s_t* mutex_buffer);

/**
 * Creates a statically allocated semaphore.
 *
 * All FreeRTOS primitives must be created statically if they are required for
 * operation of the kernel.
 *
 * \param max_count
 *        The maximum count value that can be reached.
 * \param init_count
 *        The initial count value assigned to the new semaphore.
 * \param[out] semaphore_buffer
 *             A buffer to store the semaphore in
 *
 * \return A newly created semaphore. If an error occurred, NULL will be
 * returned and errno can be checked for hints as to why sem_create failed.
 */
sem_t sem_create_static(uint32_t max_count, uint32_t init_count, static_sem_s_t* semaphore_buffer);

/**
 * Creates a statically allocated queue.
 *
 * All FreeRTOS primitives must be created statically if they are required for
 * operation of the kernel.
 *
 * \param length
 *        The maximum number of items that the queue can contain.
 * \param item_size
 *        The number of bytes each item in the queue will require.
 * \param[out] storage_buffer
 *             A memory location for data storage
 * \param[out] queue_buffer
 *             A buffer to store the queue in
 *
 * \return A handle to a newly created queue, or NULL if the queue cannot be
 * created.
 */
queue_t queue_create_static(uint32_t length, uint32_t item_size, uint8_t* storage_buffer,
                            static_queue_s_t* queue_buffer);

/**
 * Display a non-fatal error to the built-in LCD/touch screen.
 *
 * Note that this function is thread-safe, which requires that the scheduler be
 * in a functioning state. For situations in which it is unclear whether the
 * scheduler is working, use `display_fatal_error` instead.
 *
 * \param[in] text
 *            The text string to display to the screen
 */
void display_error(const char* text);

/**
 * Display a fatal error to the built-in LCD/touch screen.
 *
 * This function is intended to be used when the integrity of the RTOS cannot be
 * trusted. No thread-safety mechanisms are used and this function only relies
 * on the use of the libv5rts.
 *
 * \param[in] text
 *            The text string to display to the screen
 */
void display_fatal_error(const char* text);

/**
 * Prints hex characters to the terminal.
 *
 * \param[in] s
 *            The array of hex characters to print
 * \param len
 *        The number of hex characters to print
 */
void kprint_hex(uint8_t* s, size_t len);

int32_t xTaskGetSchedulerState();
#define taskSCHEDULER_SUSPENDED ((int32_t)0)
#define taskSCHEDULER_NOT_STARTED ((int32_t)1)
#define taskSCHEDULER_RUNNING ((int32_t)2)

#ifdef __cplusplus
#undef task_t
#undef task_fn_t
#undef mutex_t
}
#endif
