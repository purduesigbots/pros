/*
 * semphr.c - Semaphores and mutexes
 *
 * Copyright (c) 2011-2016, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include <semphr.h>

// semaphoreCreate - Creates a semaphore which starts available
Semaphore semaphoreCreate() {
	Semaphore target = queueCreate(1, 0);
	if (target != NULL)
		semaphoreGive(target);
	return target;
}

// mutexCreate - Creates a mutex which starts unowned
Mutex mutexCreate() {
	return (Mutex)queueCreateMutex();
}

// semaphoreGive - Gives a semaphore object; use for synchronization to unblock taking tasks
bool semaphoreGive(Semaphore semaphore) {
	return queueSendBack((MessageQueue)semaphore, NULL, 0);
}

// semaphoreGiveISR - Version of semaphoreGive() safe for use in interrupts
bool semaphoreGiveISR(Semaphore semaphore, bool *contextSwitch) {
	return queueSendISR((MessageQueue)semaphore, NULL, contextSwitch, QUEUE_TO_BACK);
}

// semaphoreTake - Takes a semaphore object; use for synchronization to wait for giving tasks
bool semaphoreTake(Semaphore semaphore, const unsigned long blockTime) {
	return queueReceive((MessageQueue)semaphore, NULL, (clock_t)blockTime);
}

// mutexTake - Takes, or requests, a mutex object
bool mutexTake(Mutex mutex, const unsigned long blockTime) {
	return queueReceive((MessageQueue)mutex, NULL, (clock_t)blockTime);
}

// mutexGive - Gives, or relinquishes, a mutex object
bool mutexGive(Mutex mutex) {
	return queueSendBack((MessageQueue)mutex, NULL, 0);
}

// semaphoreDelete - Deletes the semaphore
void semaphoreDelete(Semaphore semaphore) {
	queueDelete((MessageQueue)semaphore);
}

// mutexDelete - Deletes the mutex
void mutexDelete(Mutex mutex) {
	queueDelete((MessageQueue)mutex);
}
