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

#ifndef INC_FREERTOS_H
#define INC_FREERTOS_H

/*
 * Include the generic headers required for the FreeRTOS port being used.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

/* Definitions specific to the port being used. */
#include <cortex.h>
#include <kernel.h>

/* Essential parts of FreeRTOS. */
#include <list.h>

/* A few miscellaneous macros. */
#define BYTE_ALIGNMENT_MASK 0x07
#define BYTE_ALIGNMENT 0x08
#define MAX_DELAY ((clock_t)0xFFFFFFFF)

/* Used to control critical section nesting */
extern volatile uint32_t _criticalNesting;

/* Enter a critical section. */
static INLINE void _enterCritical() {
	__disable_irq();
	_criticalNesting++;
}

/* Exit a critical section. */
static INLINE void _exitCritical() {
	const uint32_t newCritical = _criticalNesting - 1;
	_criticalNesting = newCritical;
	if (newCritical == 0)
		__enable_irq();
}

/** Internals for FreeRTOS files **/

/*
 * THIS FUNCTION MUST BE CALLED WITH INTERRUPTS DISABLED.
 *
 * Removes the calling task from the ready list and places it both
 * on the list of tasks waiting for a particular event, and the
 * list of delayed tasks.  The task will be removed from both lists
 * and replaced on the ready list should either the event occur (and
 * there be no higher priority tasks waiting on the same event) or
 * the delay period expires.
 *
 * @param eventList The list containing tasks that are blocked waiting
 * for the event to occur.
 *
 * @param timeout The maximum amount of time that the task should wait
 * for the event to occur.  This is specified in milliseconds.
 */
void _taskSetEvent(OSList *eventList, const clock_t timeout);

/*
 * THIS FUNCTION MUST BE CALLED WITH INTERRUPTS DISABLED.
 *
 * Removes a task from both the specified event list and the list of blocked
 * tasks, and places it on a ready queue.
 *
 * _taskClearEvent() will be called if either an event occurs to
 * unblock a task, or the block timeout period expires.
 *
 * @param eventList The list containing tasks that are blocked waiting
 * for the event to occur.
 *
 * @return true if the task being removed has a higher priority than the task
 * making the call, otherwise false.
 */
bool _taskClearEvent(OSList *eventList);

static INLINE void _taskYield() {
	/* Set a PendSV to request a context switch. */
	SCB->ICSR |= SCB_ICSR_PENDSV;
}

/* Yield command works with the scheduler both stopped and running. */
void _yield();

#endif
