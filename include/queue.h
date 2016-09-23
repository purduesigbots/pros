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

#ifndef QUEUE_H
#define QUEUE_H

#include <FreeRTOS.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Type by which queue objects are referenced.
 */
typedef void* MessageQueue;

/* For internal use only. */
#define	QUEUE_TO_BACK 0
#define	QUEUE_TO_FRONT 1

/**
 * It is preferred that the functions queueSendFront() and queueSendBack()
 * are used in place of calling this function directly.
 *
 * Post an item on a queue.  The item is queued by copy, not by reference.
 * This function must not be called from an interrupt service routine.
 * See queueSendISR() for an alternative which may be used in an ISR.
 *
 * @param q The handle to the queue on which the item is to be posted.
 *
 * @param item A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvitem
 * into the queue storage area.
 *
 * @param timeout The maximum amount of time the task should block
 * waiting for space to become available on the queue, should it already
 * be full.  The call will return immediately if this is set to 0 and the
 * queue is full.  The time is defined in milliseconds.
 *
 * @param position Can take the value QUEUE_TO_BACK to place the
 * item at the back of the queue, or QUEUE_TO_FRONT to place the item
 * at the front of the queue (for high priority messages).
 *
 * @return true if the item was successfully posted, otherwise false.
 */
bool queueSend(MessageQueue q, const void *item, const clock_t timeout,
	const uint32_t position);

/**
 * This is a function that calls queueSend().
 *
 * Post an item to the front of a queue.  The item is queued by copy, not by
 * reference.  This function must not be called from an interrupt service
 * routine.  See queueSendISR() for an alternative which may be used
 * in an ISR.
 *
 * @param queue The handle to the queue on which the item is to be posted.
 *
 * @param item A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvitem
 * into the queue storage area.
 *
 * @param timeout The maximum amount of time the task should block
 * waiting for space to become available on the queue, should it already
 * be full.  The call will return immediately if this is set to 0 and the
 * queue is full.  The time is defined in milliseconds.
 *
 * @return true if the item was successfully posted, otherwise false.
 */
static INLINE bool queueSendFront(MessageQueue queue, const void *item, const clock_t timeout) {
	return queueSend(queue, item, timeout, QUEUE_TO_FRONT);
}

/**
 * This is a function that calls queueSend().
 *
 * Post an item to the back of a queue.  The item is queued by copy, not by
 * reference.  This function must not be called from an interrupt service
 * routine.  See queueSendISR() for an alternative which may be used
 * in an ISR.
 *
 * @param queue The handle to the queue on which the item is to be posted.
 *
 * @param item A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvitem
 * into the queue storage area.
 *
 * @param timeout The maximum amount of time the task should block
 * waiting for space to become available on the queue, should it already
 * be full.  The call will return immediately if this is set to 0 and the queue
 * is full.  The time is defined in milliseconds.
 *
 * @return true if the item was successfully posted, otherwise false.
 */
static INLINE bool queueSendBack(MessageQueue queue, const void *item, const clock_t timeout) {
	return queueSend(queue, item, timeout, QUEUE_TO_BACK);
}

bool _queueReceive(MessageQueue q, void *buffer, const clock_t timeout, const bool peek);

/**
 * This is a function that calls the queueReceive() function.
 *
 * Receive an item from a queue without removing the item from the queue.
 * The item is received by copy so a buffer of adequate size must be
 * provided.  The number of bytes copied into the buffer was defined when
 * the queue was created.
 *
 * Successfully received items remain on the queue so should be returned again
 * by the next call, or a call to queueReceive().  Insertion of items at the front
 * of the queue between time of check and time of use may change this value!
 *
 * This macro must not be used in an interrupt service routine.
 *
 * @param queue The handle to the queue from which the item is to be
 * received.
 *
 * @param buffer Pointer to the buffer into which the received item will
 * be copied.
 *
 * @param timeout The maximum amount of time the task should block
 * waiting for an item to receive should the queue be empty at the time
 * of the call.	 The time is defined in milliseconds. queuePeek() will return
 * immediately if this value is 0 and the queue is empty.
 *
 * @return true if an item was successfully received from the queue,
 * otherwise false.
 */
static INLINE bool queuePeek(MessageQueue queue, void *buffer, const clock_t timeout) {
	return _queueReceive(queue, buffer, timeout, true);
}

/**
 * This is a function that calls the queueReceive() function.
 *
 * Receive an item from a queue.  The item is received by copy so a buffer of
 * adequate size must be provided.  The number of bytes copied into the buffer
 * was defined when the queue was created.
 *
 * Successfully received items are removed from the queue.
 *
 * This function must not be used in an interrupt service routine.  See
 * xQueueReceiveFromISR for an alternative that can.
 *
 * @param queue The handle to the queue from which the item is to be
 * received.
 *
 * @param buffer Pointer to the buffer into which the received item will
 * be copied.
 *
 * @param timeout The maximum amount of time the task should block
 * waiting for an item to receive should the queue be empty at the time
 * of the call.	 The time is defined in milliseconds. queueReceive() will return
 * immediately if this value is 0 and the queue is empty.
 *
 * @return true if an item was successfully received from the queue,
 * otherwise false.
 */
static INLINE bool queueReceive(MessageQueue queue, void *buffer, const clock_t timeout) {
	return _queueReceive(queue, buffer, timeout, false);
}

/**
 * Return the number of messages stored in a queue.
 *
 * @param queue A handle to the queue being queried.
 *
 * @return The number of messages available in the queue.  Beware of time of
 * check to time of use problems.
 */
uint32_t queueSize(const MessageQueue queue);

/**
 * Delete a queue - freeing all the memory allocated for storing of items
 * placed on the queue.
 *
 * @param queue A handle to the queue to be deleted.
 */
void queueDelete(MessageQueue queue);

/**
 * Post an item on a queue.  It is safe to use this function from within an
 * interrupt service routine.
 *
 * Items are queued by copy not reference so it is preferable to only
 * queue small items, especially when called from an ISR.  In most cases
 * it would be preferable to store a pointer to the item being queued.
 *
 * @param q The handle to the queue on which the item is to be posted.
 *
 * @param item A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from item
 * into the queue storage area.
 *
 * @param contextSwitch queueSendISR() will set *contextSwitch to true if
 * sending to the queue caused a task to unblock, and the unblocked task has a
 * priority higher than the currently running task.  If so, a context switch
 * should be requested before the interrupt exits.
 *
 * @param copyPosition Can take the value QUEUE_TO_BACK to place the
 * item at the back of the queue, or QUEUE_TO_FRONT to place the item
 * at the front of the queue (for high priority messages).
 *
 * @return true if the data was successfully sent to the queue, otherwise false.
 */
bool queueSendISR(MessageQueue q, const void *item, bool *contextSwitch,
	const uint32_t position);

/**
 * Receive an item from a queue.  It is safe to use this function from within an
 * interrupt service routine.
 *
 * @param q The handle to the queue from which the item is to be
 * received.
 *
 * @param nuffer Pointer to the buffer into which the received item will
 * be copied.
 *
 * @param taskWoken A task may be blocked waiting for space to become
 * available on the queue.  If queueReceiveISR causes such a task to
 * unblock *taskWoken will get set to true, otherwise *taskWoken will
 * remain unchanged.
 *
 * @return true if an item was successfully received from the queue,
 * otherwise false.
 */
bool queueReceiveISR(MessageQueue q, void *buffer, bool *taskWoken);

/*
 * For internal use only.  Use mutexCreate() instead of calling this function directly.
 */
MessageQueue queueCreateMutex();

/*
 * Generic version of the queue creation function, which is in turn called by 
 * any queue, semaphore or mutex creation function or macro.
 */
MessageQueue queueCreate(const uint32_t queueLength, const size_t itemSize);

#ifdef __cplusplus
}
#endif

#endif
