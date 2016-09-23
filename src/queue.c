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

#include <string.h>
#include <queue.h>
#include <task.h>
#include <taskpriv.h>

/*-----------------------------------------------------------
 * PUBLIC QUEUE API documented in queue.h
 *----------------------------------------------------------*/

/* mutexHolder = tail, mutexCheck = head, recursiveCallCount = readFrom */

/*
 * Definition of the queue used by the scheduler.
 * Items are queued by copy, not reference.
 */
typedef struct {
	char *head; /* Points to the beginning of the queue storage area. */
	char *tail; /* Points to the byte at the end of the queue storage area.  Once more byte is allocated than necessary to store the queue items, this is used as a marker. */
	char *writeTo; /* Points to the free next place in the storage area. */
	char *readFrom; /* Points to the last place that a queued item was read from. */
	OSList tasksWaitingToSend; /* List of tasks that are blocked waiting to post onto this queue.  Stored in priority order. */
	OSList tasksWaitingToReceive; /* List of tasks that are blocked waiting to read from this queue.  Stored in priority order. */
	volatile uint32_t messagesWaiting; /* The number of items currently in the queue. */
	uint32_t length; /* The length of the queue defined as the number of items it will hold, not the number of bytes. */
	size_t itemSize; /* The size of each items that the queue will hold. */
} OSQueue;

/*
 * Utilities to query queue that are safe to use from an ISR.
 */
static INLINE bool _queueIsEmpty(const OSQueue *queue) {
	return queue->messagesWaiting == 0;
}

static INLINE bool _queueIsFull(const OSQueue *queue) {
	return queue->messagesWaiting == queue->length;
}

/*
 * Copies an item into the queue, either at the front of the queue or the
 * back of the queue.
 */
static void _queueCopyIn(OSQueue *queue, const void *item, const uint32_t position) {
	char *pos;
	size_t size = queue->itemSize;
	if (size == 0) {
		if (queue->head == NULL) {
			/* The mutex is no longer being held. */
			_taskPriorityDisinherit((void*)queue->tail);
			queue->tail = NULL;
		}
	} else if (position == QUEUE_TO_BACK) {
		/* Append at end of list */
		pos = queue->writeTo;
		memcpy((void*)pos, item, size);
		pos += size;
		if (pos >= queue->tail)
			pos = queue->head;
		queue->writeTo = pos;
	} else {
		/* Insert before read position */
		pos = queue->readFrom;
		memcpy((void*)pos, item, size);
		pos -= size;
		if (pos < queue->head)
			pos = queue->tail - size;
		queue->readFrom = pos;
	}
	(queue->messagesWaiting)++;
}

/*
 * Copies an item out of a queue.
 */
static void _queueCopyOut(OSQueue *queue, void *buffer) {
	char *pos;
	size_t size = queue->itemSize;
	if (size > 0) {
		/* Read from data (note that readFrom is before the item) */
		pos = queue->readFrom;
		pos += size;
		if (pos >= queue->tail)
			pos = queue->head;
		memcpy(buffer, (void*)pos, size);
		queue->readFrom = pos;
	}
}

/*-----------------------------------------------------------
 * PUBLIC QUEUE MANAGEMENT API documented in queue.h
 *----------------------------------------------------------*/

MessageQueue queueCreate(const uint32_t queueLength, const size_t itemSize) {
	OSQueue *newQueue = NULL;
	/* Allocate the new queue structure. */
	if (queueLength > 0) {
		newQueue = (OSQueue*)malloc(sizeof(OSQueue));
		if (newQueue != NULL) {
			/* Create the list of pointers to queue items.  The queue is one byte
			 longer than asked for to make wrap checking easier/faster. */
			const size_t size = queueLength * itemSize;
			char *head = (char*)malloc(size + 1);
			if (head != NULL) {
				/* Initialise the queue members as described above where the
				 queue type is defined. */
				newQueue->head = head;
				newQueue->tail = head + size;
				newQueue->messagesWaiting = 0;
				newQueue->writeTo = head;
				newQueue->readFrom = head + size - itemSize;
				newQueue->length = queueLength;
				newQueue->itemSize = itemSize;
				/* Likewise ensure the event queues start with the correct state. */
				listInit(&(newQueue->tasksWaitingToSend));
				listInit(&(newQueue->tasksWaitingToReceive));
			} else {
				// No space
				free(newQueue);
				newQueue = NULL;
			}
		}
	}
	return (MessageQueue)newQueue;
}

MessageQueue queueCreateMutex() {
	/* Allocate the new queue structure. */
	OSQueue *newQueue = (OSQueue*)malloc(sizeof(OSQueue));
	if (newQueue != NULL) {
		/* Information required for priority inheritance. */
		newQueue->tail = NULL;
		newQueue->head = NULL;
		/* Queues used as a mutex no data is actually copied into or out
		 of the queue. */
		newQueue->writeTo = NULL;
		newQueue->readFrom = NULL;
		/* Each mutex has a length of 1 (like a binary semaphore) and
		 an item size of 0 as nothing is actually copied into or out
		 of the mutex. */
		newQueue->messagesWaiting = 0;
		newQueue->length = 1;
		newQueue->itemSize = 0;
		/* Ensure the event queues start with the correct state. */
		listInit(&(newQueue->tasksWaitingToSend));
		listInit(&(newQueue->tasksWaitingToReceive));
		/* Start with the semaphore in the expected state. */
		queueSend(newQueue, NULL, 0, QUEUE_TO_BACK);
	}
	return (MessageQueue)newQueue;
}

bool queueSend(MessageQueue q, const void *item, const clock_t timeout,
		const uint32_t position) {
	OSQueue *queue = (OSQueue*)q;
	clock_t startTime = timeLowRes();
	while (1) {
		_enterCritical();
		{
			/* Is there room on the queue now?  To be running we must be
			 the highest priority task wanting to access the queue. */
			if (!_queueIsFull(queue)) {
				_queueCopyIn(queue, item, position);
				/* If there was a task waiting for data to arrive on the
				 queue then unblock it now. */
				if (!listIsEmpty(&(queue->tasksWaitingToReceive)) &&
						_taskClearEvent(&(queue->tasksWaitingToReceive)))
					/* The unblocked task has a priority higher than
					 our own so yield immediately. */
					_taskYield();
				_exitCritical();
				return true;
			} else if (timeout == 0)
				/* Cannot satisfy timeout */
				break;
		}
		_exitCritical();

		_enterCritical();
		{
			if ((timeLowRes() - startTime) < timeout) {
				/* If queue is full, set task on queue to be activated */
				if (_queueIsFull(queue)) {
					_taskSetEvent(&(queue->tasksWaitingToSend), timeout);
					_taskYield();
				}
			} else
				break;
		}
		_exitCritical();
	}
	_exitCritical();
	return false;
}

bool _queueReceive(MessageQueue q, void *buffer, const clock_t timeout, const bool peek) {
	OSQueue *queue = (OSQueue*)q;
	clock_t startTime = timeLowRes();
	char *originalReadPosition;
	while (1) {
		_enterCritical();
		{
			if (!_queueIsEmpty(queue)) {
				/* Remember our read position in case we are just peeking. */
				originalReadPosition = queue->readFrom;
				_queueCopyOut(queue, buffer);
				if (!peek) {
					/* We are actually removing data. */
					(queue->messagesWaiting)--;
					if (queue->head == NULL)
						/* Record the information required to implement
						 priority inheritance should it become necessary. */
						queue->tail = taskGetCurrent();
					if (!listIsEmpty(&(queue->tasksWaitingToSend)) &&
							_taskClearEvent(&(queue->tasksWaitingToSend)))
						_taskYield();
				} else {
					/* We are not removing the data, so reset our read pointer. */
					queue->readFrom = originalReadPosition;
					/* The data is being left in the queue, so see if there are
					 any other tasks waiting for the data. */
					if (!listIsEmpty(&(queue->tasksWaitingToReceive)) &&
							_taskClearEvent(&(queue->tasksWaitingToReceive)))
						/* Tasks that are removed from the event list will get added to
						 the pending ready list as the scheduler is still suspended. */
						_taskYield();
				}
				_exitCritical();
				return true;
			} else if (timeout == 0)
				/* Cannot satisfy timeout */
				break;
		}
		_exitCritical();

		_enterCritical();
		{
			if ((timeLowRes() - startTime) < timeout) {
				if (_queueIsEmpty(queue)) {
					/* Inherit priority for mutexes to prevent priority inversion */
					if (queue->head == NULL)
						_taskPriorityInherit((void*)queue->tail);
					/* If queue is empty, set task on queue to be activated */
					_taskSetEvent(&(queue->tasksWaitingToReceive), timeout);
					_taskYield();
				}
			} else
				break;
		}
		_exitCritical();
	}
	_exitCritical();
	return false;
}

bool queueSendISR(MessageQueue q, const void *item, bool *contextSwitch, const uint32_t copyPosition) {
	OSQueue *queue = (OSQueue*)q;
	bool ret = false;
	/* Similar to queueSend, except we don't block if there is no room
	 in the queue.  Also we don't directly wake a task that was blocked on a
	 queue read, instead we return a flag to say whether a context switch is
	 required or not (i.e. has a task with a higher priority than us been woken
	 by this post). */
	_enterCritical();
	{
		if (!_queueIsFull(queue)) {
			_queueCopyIn(queue, item, copyPosition);
			if (!listIsEmpty(&(queue->tasksWaitingToReceive)) &&
					_taskClearEvent(&(queue->tasksWaitingToReceive)))
				/* The task waiting has a higher priority so record that a
				 context switch is required. */
				*contextSwitch = true;
			ret = true;
		}
	}
	_exitCritical();
	return ret;
}

bool queueReceiveISR(MessageQueue q, void *buffer, bool *taskWoken) {
	OSQueue *queue = (OSQueue*)q;
	bool ret = false;
	_enterCritical();
	{
		/* We cannot block from an ISR, so check there is data available. */
		if (!_queueIsEmpty(queue)) {
			_queueCopyOut(queue, buffer);
			(queue->messagesWaiting)--;
			if (!listIsEmpty(&(queue->tasksWaitingToSend)) &&
					_taskClearEvent(&(queue->tasksWaitingToSend)))
				/* The task waiting has a higher priority so record that a
				 context switch is required. */
				*taskWoken = true;
			ret = true;
		}
	}
	_exitCritical();
	return ret;
}

uint32_t queueSize(const MessageQueue queue) {
	return ((OSQueue*)queue)->messagesWaiting;
}

void queueDelete(MessageQueue queue) {
	OSQueue *q = (OSQueue*)queue;
	free(q->head);
	free(q);
}
