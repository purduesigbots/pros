#pragma once

#include "api.h"
#include "apix.h"

#include "rtos/FreeRTOS.h"
#include "system/system.h"

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