/**
 * kapi.h - Kernel API header
 *
 * Contains additional declarations for use internally within kernel development.
 * This file includes the FreeRTOS header, which allows for creation of statically
 * allocated FreeRTOS primitives like tasks, semaphores, and queues.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 * All rights reservered.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "api.h"
#include "apix.h"

#include "rtos/FreeRTOS.h"

#ifdef __cplusplus
extern "C" {
#endif

#define KDBG_FILENO 3

#define wprintf(fmt, ...) dprintf(STDERR_FILENO, "%s:%d -- " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define wprint(str) wprintf("%s", str)

#define kprintf(fmt, ...) dprintf(KDBG_FILENO, "%s:%d -- " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define kprint(str) kprintf("%s", str)

#ifndef PROS_RELEASING
#define kassert(cond)                                                                                                  \
	do {                                                                                                           \
		if (!(cond)) {                                                                                         \
			kprint("Assertion failed: " #cond);                                                            \
		}                                                                                                      \
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
 * previously suspended by task_suspend. Returns true if a context switch is
 * necessary.
 *
 * if(rtos_resume_all()) {
 *     task_delay(0); // force context switch
 * }
 */
int32_t rtos_resume_all(void);

/**
 * Creates a task using statically allocated buffers. All tasks used by the PROS
 * system must use statically allocated buffers.
 */
task_t task_create_static(task_fn_t task_code,
                          void* const param,
                          uint32_t priority,
                          const size_t stack_size,
                          const char* const name,
                          task_stack_t* const stack_buffer,
                          static_task_s_t* const task_buffer);

/**
 * Creates a statically allocated mutex. See the documentation in api.h for mutex_create().
 *
 * All FreeRTOS primitives must be created statically if they are required for operation of the kernel.
 */
mutex_t mutex_create_static(static_sem_s_t* pxMutexBuffer);

/**
 * Creates a statically allocated semaphore. See the documentation in api.h for sem_create().
 *
 * All FreeRTOS primitives must be created statically if they are required for operation of the kernel.
 */
sem_t sem_create_static(uint32_t uxMaxCount, uint32_t uxInitialCount, static_sem_s_t* pxSemaphoreBuffer);

/**
 * Creates a statically allocated queue. See the documentation in apix.h for queue_create()
 *
 * All FreeRTOS primitives must be created statically if they are required for operation of the kernel.
 */
queue_t
    queue_create_static(uint32_t length, uint32_t item_size, uint8_t* storage_buffer, static_queue_s_t* queue_buffer);

void kprint_hex(uint8_t* s, size_t len);
#ifdef __cplusplus
}
#endif