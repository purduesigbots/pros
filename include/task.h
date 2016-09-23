/*
 FreeRTOS V7.1.0 - Copyright (C) 2011 Real Time Engineers Ltd.

 ***************************************************************************
 *                                                                       *
 *    FreeRTOS tutorial books are available in pdf and paperback.        *
 *    Complete, revised, and edited pdf reference manuals are also       *
 *    available.                                                         *
 *                                                                       *
 *    Purchasing FreeRTOS documentation will not only help you, by       *
 *    ensuring you get running as quickly as possible and with an        *
 *    in-depth knowledge of how to use FreeRTOS, it will also help       *
 *    the FreeRTOS project to continue with its mission of providing     *
 *    professional grade, cross platform, de facto standard solutions    *
 *    for microcontrollers - completely free of charge!                  *
 *                                                                       *
 *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
 *                                                                       *
 *    Thank you for using FreeRTOS, and thank you for your support!      *
 *                                                                       *
 ***************************************************************************

 This file is part of the FreeRTOS distribution.

 FreeRTOS is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License (version 2) as published by the
 Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
 >>>NOTE<<< The modification to the GPL is included to allow you to
 distribute a combined work that includes FreeRTOS without being obliged to
 provide the source code for proprietary components outside of the FreeRTOS
 kernel.  FreeRTOS is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 more details. You should have received a copy of the GNU General Public
 License and the FreeRTOS license exception along with FreeRTOS; if not it
 can be viewed here: http://www.freertos.org/a00114.html and also obtained
 by writing to Richard Barry, contact details for whom are available on the
 FreeRTOS WEB site.

 http://www.FreeRTOS.org - Documentation, latest information, license and
 contact details.

 This version of FreeRTOS has been modified explicitly for use on the VEX
 Cortex by Stephen Carlson of Purdue ACM SIGBots, College Challenge team #BLRS.
 */

#ifndef TASK_H
#define TASK_H

#include <FreeRTOS.h>
#include <list.h>

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------
 * MACROS AND DEFINITIONS
 *----------------------------------------------------------*/

/**
 * Type by which tasks are referenced.  For example, a call to taskCreate
 * returns (via a pointer parameter) a TaskHandle variable that can then
 * be used as a parameter to taskDelete to delete the task.
 */
typedef void * TaskHandle;

/**
 * Function signature of tasks.
 */
typedef void (*TaskCode)(void *);

/**
 * Only this many tasks can exist at once. Changing this value will not
 * change the effective limit without a kernel recompile.
 */
#define TASK_MAX 16

/**
 * Maximum number of available task priorities, which run from 0 to (TASK_MAX_PRIORITIES - 1).
 */
#define TASK_MAX_PRIORITIES 6
/**
 * Lowest priority.
 */
#define TASK_PRIORITY_LOWEST 0
/**
 * Highest priority.
 */
#define TASK_PRIORITY_HIGHEST (TASK_MAX_PRIORITIES - 1)
/**
 * Default task priority.
 */
#define TASK_PRIORITY_DEFAULT 2

/**
 * Minimum stack depth for a task.
 */
#define TASK_MINIMAL_STACK_SIZE	64
/**
 * Default stack depth for a task.
 */
#define TASK_DEFAULT_STACK_SIZE 512

/**
 * Constant returned from taskGetState() when the task is dead or nonexistant.
 */
#define TASK_DEAD 0
/**
 * Constant returned from taskGetState() when the task is actively executing.
 */
#define TASK_RUNNING 1
/**
 * Constant returned from taskGetState() when the task is exists and is available to run, but
 * not currently running.
 */
#define TASK_RUNNABLE 2
/**
 * Constant returned from taskGetState() when the task is delayed or blocked waiting for a
 * semaphore, mutex, or I/O operation.
 */
#define TASK_SLEEPING 3
/**
 * Constant returned from taskGetState() when the task is suspended using taskSuspend().
 */
#define TASK_SUSPENDED 4

/*-----------------------------------------------------------
 * TASK CREATION API
 *----------------------------------------------------------*/

/**
 * Create a new task and add it to the list of tasks that are ready to run.
 *
 * @param taskCode Pointer to the task entry function.
 *
 * @param stackDepth The size of the task stack specified as the number of
 * variables the stack can hold - not the number of bytes.  For example, if
 * stackDepth is defined as 100, 400 bytes will be allocated for stack storage.
 *
 * @param parameters Pointer that will be used as the parameter for the task
 * being created.
 *
 * @param priority The priority at which the task should run.
 *
 * @return A handle by which the created task can be referenced, or NULL if an
 * error occurred.
 */
TaskHandle taskCreate(TaskCode taskCode, const unsigned int stackDepth, void *parameters,
	const unsigned int priority);

/**
 * Remove a task from the RTOS real time kernels management.  The task being
 * deleted will be removed from all ready, blocked, suspended and event lists.
 *
 * NOTE:  The idle task is responsible for freeing the kernel allocated
 * memory from tasks that have been deleted.  It is therefore important that
 * the idle task is not starved of microcontroller processing time if your
 * application makes any calls to taskDelete().  Memory allocated by the
 * task code is not automatically freed, and should be freed before the task
 * is deleted.
 *
 * @param taskToDelete The handle of the task to be deleted.  Passing NULL will
 * cause the calling task to be deleted.
 */
void taskDelete(TaskHandle taskToDelete);

/*-----------------------------------------------------------
 * TASK CONTROL API
 *----------------------------------------------------------*/

/**
 * Delays the current task for a given number of milliseconds.
 *
 * taskDelay() specifies a time at which the task wishes to unblock relative to
 * the time at which taskDelay() is called.  For example, specifying a block
 * period of 100 ticks will cause the task to unblock 100 ticks after
 * taskDelay() is called.  taskDelay() does not therefore provide a good method
 * of controlling the frequency of a cyclical task as the path taken through the
 * code, as well as other task and interrupt activity, will effect the frequency
 * at which taskDelay() gets called and therefore the time at which the task
 * next executes.  See taskDelayUntil() for an alternative API function designed
 * to facilitate fixed frequency execution.  It does this by specifying an
 * absolute time (rather than a relative time) at which the calling task should
 * unblock.
 *
 * @param ticksToDelay The amount of time, in milliseconds, that
 * the calling task should block.
 */
void taskDelay(const unsigned long ticksToDelay);

/**
 * Delays the current task until a specified time.  This function can
 * be used by cyclical tasks to ensure a constant execution frequency.
 *
 * Whereas taskDelay() specifies a wake time relative to the time at which the function
 * is called, taskDelayUntil() specifies the absolute (exact) time at which it wishes to
 * unblock.  Storing the loop start time and delaying until an offset is effective.
 *
 * @param previousWakeTime Pointer to a variable that holds the time at which the
 * task was last unblocked.  The variable must be initialised with the current time
 * prior to its first use.  Following this the variable is automatically updated
 * within taskDelayUntil().
 *
 * @param cycleTime The cycle time period.  The task will be unblocked at
 * time *previousWakeTime + cycleTime.  Calling taskDelayUntil with the
 * same cycleTime parameter value will cause the task to execute with
 * a fixed interface period.
 */
void taskDelayUntil(unsigned long *previousWakeTime, const unsigned long cycleTime);

/**
 * Obtain the priority of a task.
 *
 * @param task Handle of the task to be queried.  Passing a NULL
 * handle results in the priority of the calling task being returned.
 *
 * @return The priority of the specified task.
 */
unsigned int taskPriorityGet(const TaskHandle task);

/**
 * Set the priority of a task.
 *
 * A context switch may occur before the function returns if the priority
 * being set is higher than the currently executing task and the task being
 * mutated is available to be scheduled.
 *
 * @param task Handle to the task for which the priority is being set.
 * Passing a NULL handle results in the priority of the calling task being set.
 *
 * @param newPriority The priority to which the task will be set.
 */
void taskPrioritySet(TaskHandle task, const unsigned int newPriority);

/**
 * Suspends a task.  When suspended a task will not be scheduled, regardless
 * of whether it might be otherwise available to run.
 *
 * @param taskToSuspend Handle to the task being suspended.  Passing a NULL
 * handle will cause the calling task to be suspended.
 */
void taskSuspend(TaskHandle taskToSuspend);

/**
 * Resumes a suspended task.
 *
 * A task that has been suspended by one or more calls to taskSuspend()
 * will be made available for scheduling again by a call to
 * taskResume().
 *
 * @param taskToResume Handle to the task being readied.
 */
void taskResume(TaskHandle taskToResume);

/**
 * An implementation of taskResume() that can be called from within an ISR.
 *
 * @param taskToResume Handle to the task being readied.
 *
 * @return Whether a context switch is required after the task was resumed.
 */
bool taskResumeISR(TaskHandle taskToResume);

/**
 * Retrieves the state of the specified task. Note that the state of tasks which have died may
 * be re-used for future tasks, causing the value returned by this function to reflect a
 * different task than possibly intended in this case.
 *
 * @param task Handle to the task to query. Passing NULL will query the current task status
 * (which will, by definition, be TASK_RUNNING if this call returns)
 *
 * @return A value reflecting the task's status, one of the constants TASK_DEAD, TASK_RUNNING,
 * TASK_RUNNABLE, TASK_SLEEPING, or TASK_SUSPENDED
 */
unsigned int taskGetState(TaskHandle task);

/*-----------------------------------------------------------
 * SCHEDULER CONTROL
 *----------------------------------------------------------*/

/**
 * Suspends all real time kernel activity while keeping interrupts enabled.
 *
 * After calling taskSuspendAll() the calling task will continue to execute
 * without risk of being swapped out until a call to taskResumeAll() has been
 * made.
 *
 * Scheduler API functions such as wait() must not be called while the
 * scheduler is suspended, or deadlock will occur.
 */
void taskSuspendAll();

/**
 * Resumes real time kernel activity following a call to taskSuspendAll().
 * After a call to taskResumeAll() the kernel will take control of which
 * task is executing at any time.
 *
 * @return If resuming the scheduler caused a context switch then true is
 *		  returned, otherwise false is returned.
 */
bool taskResumeAll();

/*-----------------------------------------------------------
 * TASK UTILITIES
 *----------------------------------------------------------*/

/**
 * @return The number of tasks that the real time kernel is currently managing.
 * This includes all ready, blocked and suspended tasks.  A task that
 * has been deleted but not yet freed by the idle task will also be
 * included in the count.
 */
unsigned int taskGetCount();

#ifdef __cplusplus
}
#endif
#endif
