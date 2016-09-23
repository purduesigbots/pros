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

#include <kernel.h>
#include <FreeRTOS.h>
#include <task.h>
#include <taskpriv.h>

/*
 * Macro to define the amount of stack available to the idle task.
 */
#define TASK_IDLE_STACK_SIZE TASK_MINIMAL_STACK_SIZE
/*
 * Defines the priority used by the idle task. This must not be modified.
 */
#define TASK_IDLE_PRIORITY 0
/*
 * Scheduler status flags.
 */
#define SCHEDULER_RUNNING 0x01
#define SCHEDULER_SUSPENDED 0x02
/* Constants required to set up the initial stack. */
#define INITIAL_XPSR 0x01000000

/* Arbitrarily chosen pad value to check the stack for overflow. */
#define STACK_NOT_OVERFLOWED 0x71AA462F

/*
 * Several functions take an taskHandle parameter that can optionally be NULL,
 * where NULL is used to indicate that the handle of the currently executing
 * task should be used in place of the parameter.  This macro simply checks to
 * see if the parameter is NULL and returns a pointer to the appropriate TCB.
 */
#define _tcbFromHandle(handle) (((handle) == NULL) ? (TCB*)currentTCB : (TCB*)(handle))

/*
 * Task control block.  A task control block (TCB) is allocated to each task,
 * and stores the context of the task.
 */
typedef struct TaskControlBlock {
	/* Location of the last item on the tasks stack.  THIS MUST BE THE FIRST MEMBER. */
	volatile uint32_t *topOfStack;
	OSListItem genericListItem; /* List item used to place the TCB in queues. */
	OSListItem eventListItem; /* List item used to place the TCB in event lists. */
	uint32_t priority; /* The priority of the task where 0 is the lowest priority. */
	uint32_t *stack; /* Points to the initial location (the end) of the stack. */
	uint32_t basePriority; /* Priority last given to the task - used by priority inheritance. */
} TCB;

static OSList readyTasks[TASK_MAX_PRIORITIES]; /* Prioritised ready tasks. */
static OSList delayedTasks; /* Delayed tasks. */
static OSList pendingReadyTasks; /* Tasks that readied while the scheduler was suspended. */
static OSList deadTasks; /* Tasks that have been deleted but their memory not yet freed. */
static OSList suspendedTasks; /* Tasks that are suspended. */

/* File private variables. */
static volatile TCB* currentTCB = NULL;
static volatile uint32_t taskCount = 0;
static volatile uint32_t deadTaskCount;
static uint32_t topUsedPriority = TASK_IDLE_PRIORITY;
static volatile uint32_t topReadyPriority = TASK_IDLE_PRIORITY;
static volatile uint32_t schedulerStatus = 0x00;
static volatile clock_t nextTaskUnblockTime;
static volatile bool missedYield = false;

/* Tasks maintain their interrupt status in the critical nesting variable. */
volatile uint32_t _criticalNesting = 0;

/* Can't be static or inline, called from assembly fragment in PendSV */
void _contextSwitch() {
	uint32_t *sp, pr, ss = schedulerStatus;
	if (ss & SCHEDULER_SUSPENDED)
		/* The scheduler is currently suspended - do not allow a context switch. */
		missedYield = true;
	else if (ss & SCHEDULER_RUNNING) {
		pr = topReadyPriority;
		/* Find the highest priority queue that contains ready tasks. */
		while (listIsEmpty(&(readyTasks[pr])))
			--pr;
		/* listGetNextOwner walks through the list, so the tasks of the
		 same priority get an equal share of the processor time. */
		currentTCB = listGetNextOwner(&(readyTasks[pr]));
		topReadyPriority = pr;
		/* Stack overflow checking */
		sp = currentTCB->stack;
		if (currentTCB->topOfStack <= sp || *sp != STACK_NOT_OVERFLOWED)
			_exit(STATUS_STACK_OVERFLOW);
	}
}

/*
 * Handle supervisor calls to start the first task.
 */
IRQ __attribute__ ((naked)) ISR_SVC() {
	asm volatile (
		"ldr r3, =currentTCB\n\t" /* Restore the context. */
		"ldr r1, [r3]\n\t"
		"ldr r0, [r1]\n\t" /* The first item in currentTCB is the task top of stack. */
		"ldmia r0!, {r4-r11}\n\t" /* Pop the registers that are not automatically saved on exception entry and the critical nesting count. */
		"msr psp, r0\n\t" /* Restore the task stack pointer. */
		"orr lr, #0x0D\n\t"
		"bx lr\n\t"
	);
}

/*
 * Starts the first task.
 */
static void  __attribute__ ((naked)) _startFirstTask() {
	asm volatile (
		"ldr r0, =0xE000ED08\n\t" /* Use the NVIC offset register to locate the stack. */
		"ldr r0, [r0]\n\t"
		"ldr r0, [r0]\n\t"
		"msr msp, r0\n\t" /* Set the stack pointer back to the start of the stack. */
		"cpsie i\n\t" /* Globally enable interrupts. */
		"svc 0\n\t" /* System call to start first task. */
		"nop\n\t"
		: : : "r0"
	);
}

/*
 * Handle PendSV calls to switch context.
 */
IRQ __attribute__ ((naked)) ISR_PendSV() {
	asm volatile (
		"mrs r0, psp\n\t"
		"ldr r3, =currentTCB\n\t" /* Get the location of the current TCB. */
		"ldr r2, [r3]\n\t"
		"stmdb r0!, {r4-r11}\n\t" /* Save the remaining registers. */
		"str r0, [r2]\n\t" /* Save the new top of stack into the first member of the TCB. */
		"stmdb sp!, {r3, lr}\n\t"
		"cpsid i\n\t" /* Mask interrupts, then switch context */
		"bl _contextSwitch\n\t"
		"cpsie i\n\t"
		"ldmia sp!, {r3, lr}\n\t"
		"ldr r1, [r3]\n\t" /* Restore the context, including the critical nesting count. */
		"ldr r0, [r1]\n\t" /* The first item in the TCB is the task top of stack. */
		"ldmia r0!, {r4-r11}\n\t" /* Pop the registers. */
		"msr psp, r0\n\t"
		"bx lr\n\t"
	);
}

/*
 * Place the task represented by tcb into the appropriate ready queue for
 * the task.  It is inserted at the end of the list.  One quirk of this is
 * that if the task being inserted is at the same priority as the currently
 * executing task, then it will only be rescheduled after the currently
 * executing task has been rescheduled.
 */
static void _taskAddToReadyQueue(TCB *tcb) {
	const uint32_t pr = tcb->priority;
	if (pr > topReadyPriority)
		topReadyPriority = pr;
	listInsertEnd(&readyTasks[pr], &(tcb->genericListItem));
}

/*
 * Macro that looks at the list of tasks that are currently delayed to see if
 * any require waking.
 *
 * Tasks are stored in the queue in the order of their wake time - meaning
 * once one tasks has been found whose timer has not expired we need not look
 * any further down the list.
 */
static INLINE void _taskCheckDelays() {
	TCB *tcb; clock_t itemValue, ticks = timeLowRes();
	if (ticks >= nextTaskUnblockTime)
		/* Tasks may have become unblocked. */
		while (1) {
			if (listIsEmpty(&delayedTasks)) {
				/* No more tasks to handle here. */
				nextTaskUnblockTime = MAX_DELAY;
				break;
			}
			tcb = (TCB*)listGetFirstOwner(&delayedTasks);
			itemValue = listGetValue(&(tcb->genericListItem));
			if (ticks < itemValue) {
				/* Not yet time, but record the next time when it should occur. */
				nextTaskUnblockTime = itemValue;
				break;
			}
			listRemove(&(tcb->genericListItem));
			/* Remove references if timed out waiting on semaphore/mutex. */
			if (tcb->eventListItem.container != NULL)
				listRemove(&(tcb->eventListItem));
			_taskAddToReadyQueue(tcb);
		}
}

/* Ticks the kernel forward one millisecond. */
void _taskTick() {
	if (schedulerStatus == SCHEDULER_RUNNING) {
		/* See if this tick has made a timeout expire. */
		_enterCritical();
		{
			_taskCheckDelays();
		}
		_exitCritical();
		_taskYield();
	}
}

/* Initializes a fresh TCB with the specified variables. */
static INLINE void _taskInitTCB(TCB *tcb, const uint32_t priority, const uint32_t stackDepth) {
	tcb->priority = priority;
	tcb->basePriority = priority;
	listInitItem(&(tcb->genericListItem));
	listInitItem(&(tcb->eventListItem));
	/* Set the tcb as a link back from the OSListItem.  This is so we can get
	 back to the containing TCB from a generic item in a list. */
	listSetOwner(&(tcb->genericListItem), tcb);
	listSetOwner(&(tcb->eventListItem), tcb);
	/* Event lists are always in priority order. */
	listSetValue(&(tcb->eventListItem), (clock_t)(TASK_MAX_PRIORITIES - priority));
}

/* Arm ready tasks lists, and all other lists. */
static INLINE void _taskInitLists() {
	uint32_t priority;
	for (priority = 0; priority < TASK_MAX_PRIORITIES; priority++)
		listInit(&(readyTasks[priority]));
	listInit(&delayedTasks);
	listInit(&pendingReadyTasks);
	listInit(&deadTasks);
	listInit(&suspendedTasks);
}

/* Clean up tasks which have died. */
static INLINE void _taskReapDead() {
	bool isEmpty;
	if (deadTaskCount > 0) {
		// Dead tasks to clean up?
		isEmpty = listIsEmpty(&deadTasks);
		if (!isEmpty) {
			TCB *tcb;
			/* We have found a dead task, clean it up */
			_enterCritical();
			{
				tcb = (TCB*)listGetFirstOwner(&deadTasks);
				listRemove(&(tcb->genericListItem));
				taskCount--;
				/* If more than one task dies at once, they will be cleaned up one at a time */
				deadTaskCount--;
			}
			_exitCritical();
			/* Free up the memory allocated by the scheduler for the task.  It is up to
			 the task to free any memory allocated at the application level. */
			free(tcb->stack);
			free(tcb);
		} else
			deadTaskCount = 0;
	}
}

static void _taskSetDelayed(const clock_t wakeTime) {
	/* The list item will be inserted in wake time order. */
	listSetValue((OSListItem*)&(currentTCB->genericListItem), wakeTime);
	listInsert(&delayedTasks, (OSListItem*)&(currentTCB->genericListItem));
	/* If the task entering the blocked state was placed at the head of the
	 list of blocked tasks then nextTaskUnblockTime needs to be updated too. */
	if (wakeTime < nextTaskUnblockTime)
		nextTaskUnblockTime = wakeTime;
}

static INLINE TCB* _taskAllocateTCB(uint32_t stackDepth) {
	TCB *tcb;
	/* Allocate space for the TCB.  Where the memory comes from depends on
	 the implementation of the port malloc function. */
	tcb = (TCB*)malloc(sizeof(TCB));
	if (tcb != NULL) {
		/* Allocate space for the stack used by the task being created.
		 The base of the stack memory is stored in the TCB so the task can
		 be deleted later if required. */
		uint32_t *sp = (uint32_t*)malloc(stackDepth * sizeof(uint32_t));
		if (sp == NULL) {
			/* Could not allocate the stack.  Delete the allocated TCB. */
			free(tcb);
			tcb = NULL;
		} else {
			/* Initialize no-overflow flag. */
			tcb->stack = sp;
			*sp = STACK_NOT_OVERFLOWED;
		}
	}
	return tcb;
}

/* The cleanup function to clear dead tasks. */
static void _taskCleanUp() {
	taskDelete(NULL);
}

/* The idle task. */
static void _taskIdleLoop(void* ignore) {
	/* Delegate LCD dumping to the idle task */
	void _lcdDump(uint32_t lcdIndex);
	clock_t lcdUpdateTime = 0, now;
	while (1) {
		_taskReapDead();
		now = timeLowRes();
		/* Every 16 ms, dump data to the LCDs */
		if (now >= lcdUpdateTime) {
			_lcdDump(0);
			_lcdDump(1);
			lcdUpdateTime = now + 16;
		}
		if (listSize(&(readyTasks[TASK_IDLE_PRIORITY])) > 1)
			/* If anyone else at low priority is ready to run, run them */
			_taskYield();
		else
			/* Conserve power, wait for next interrupt */
			__sleep();
	}
}

/*
 * Initializes a TCB stack with variables.
 */
static INLINE uint32_t* _initStack(uint32_t *topOfStack, const TaskCode code,
		const void *parameters) {
	/* Simulate the stack frame as it would be created by a context switch interrupt. */
	topOfStack--;
	*topOfStack = INITIAL_XPSR; /* xPSR */
	topOfStack--;
	*topOfStack = (uint32_t)code; /* PC */
	topOfStack--;
	*topOfStack = (uint32_t)_taskCleanUp; /* LR = "delete current task" */
	topOfStack -= 5; /* R12, R3, R2 and R1. */
	*topOfStack = (uint32_t)parameters; /* R0 */
	topOfStack -= 8; /* R11, R10, R9, R8, R7, R6, R5 and R4. */
	return topOfStack;
}

/*
 * Yields control depending on whether the scheduler is running.
 */
void _yield() {
	if (schedulerStatus == SCHEDULER_RUNNING)
		_taskYield();
	else
		__sleep();
}

/*-----------------------------------------------------------
 * TASK CREATION API documented in task.h
 *----------------------------------------------------------*/

TaskHandle taskCreate(TaskCode taskCode, const unsigned int stackDepth, void *parameters,
		const unsigned int priority) {
	uint32_t pr;
	TCB *newTCB;
	/* Check task count */
	if (taskCount > TASK_MAX)
		return NULL;
	/* Limit priority */
	pr = priority;
	if (pr >= TASK_MAX_PRIORITIES)
		pr = TASK_MAX_PRIORITIES - 1;
	/* Allocate the memory required by the TCB and stack for the new task,
	 checking that the allocation was successful. */
	newTCB = _taskAllocateTCB(stackDepth);
	if (newTCB != NULL) {
		uint32_t *stackTop;
		/* Calculate top of stack address. */
		stackTop = newTCB->stack + (size_t)(stackDepth - 1);
		stackTop = (uint32_t*)((size_t)stackTop & ~BYTE_ALIGNMENT_MASK);
		/* Setup the newly allocated TCB with the initial state of the task. */
		_taskInitTCB(newTCB, pr, stackDepth);
		/* Initialize the TCB stack to look as if the task was already running,
		 but had been interrupted by the scheduler.  The return address is set
		 to the start of the task function. Once the stack has been initialised
		 the top of stack variable is updated. */
		newTCB->topOfStack = _initStack(stackTop, taskCode, parameters);
		/* We are going to manipulate the task queues to add this task to a
		 ready list, so must make sure no interrupts occur. */
		_enterCritical();
		{
			taskCount++;
			if (currentTCB == NULL) {
				/* There are no other tasks, or all the other tasks are in
				 the suspended state - make this the current task. */
				currentTCB = newTCB;
				if (taskCount == 1)
					/* This is the first task to be created so do the preliminary
					 initialisation required.  We will not recover if this call
					 fails, but we will report the failure. */
					_taskInitLists();
			} else if (!(schedulerStatus & SCHEDULER_RUNNING) && currentTCB->priority <= pr)
				/* If the scheduler is not already running, make this task the
				 current task if it is the highest priority task to be created
				 so far. */
				currentTCB = newTCB;
			/* Remember the top priority to make context switching faster.  Use
			 the priority in tcb as this has been capped to a valid value. */
			if (pr > topUsedPriority)
				topUsedPriority = pr;
			_taskAddToReadyQueue(newTCB);
		}
		_exitCritical();
	} else
		return NULL;
	if ((schedulerStatus & SCHEDULER_RUNNING) && currentTCB->priority < pr)
		/* If the created task is of a higher priority than the current task
		 then it should run now. */
		_taskYield();
	return (TaskHandle) newTCB;
}

void taskDelete(TaskHandle taskToDelete) {
	TCB *tcb;
	_enterCritical();
	{
		/* If NULL is passed in here then we are deleting ourselves. */
		tcb = _tcbFromHandle(taskToDelete);
		/* Remove task from the ready list and place in the	termination list.
		 This will stop the task from be scheduled.  The idle task will check
		 the termination list and free up any memory allocated by the
		 scheduler for the TCB and stack. */
		listRemove(&(tcb->genericListItem));
		/* Is the task waiting on an event also? */
		if (tcb->eventListItem.container != NULL)
			listRemove(&(tcb->eventListItem));
		listInsertEnd(&deadTasks, &(tcb->genericListItem));
		/* Increment the ucTasksDeleted variable so the idle task knows
		 there is a task that has been deleted and that it should therefore
		 check the xTasksWaitingTermination list. */
		deadTaskCount++;
	}
	_exitCritical();
	/* Force a reschedule if we have just deleted the current task. */
	if ((schedulerStatus & SCHEDULER_RUNNING) && tcb == currentTCB)
		_taskYield();
}

/*-----------------------------------------------------------
 * TASK CONTROL API documented in task.h
 *----------------------------------------------------------*/

void taskDelayUntil(unsigned long *previousWakeTime, const unsigned long cycleTime) {
	clock_t timeToWake;
	bool yield, delay = false;
	taskSuspendAll();
	{
		/* Generate the tick time at which the task wants to wake. */
		timeToWake = (clock_t)(*previousWakeTime + cycleTime);
		/* The tick time will not overflow; it would take over 2000 hours of runtime... */
		if (timeToWake > timeLowRes())
			delay = true;
		/* Update the wake time ready for the next call. */
		*previousWakeTime = (unsigned long)timeToWake;
		if (delay) {
			/* We must remove ourselves from the ready list before adding
			 ourselves to the blocked list as the same list item is used for
			 both lists. */
			listRemove((OSListItem*)&(currentTCB->genericListItem));
			_taskSetDelayed(timeToWake);
		}
	}
	yield = taskResumeAll();
	/* Force a reschedule if taskResumeAll has not already done so, we may
	 have put ourselves to sleep. */
	if (!yield)
		_taskYield();
}

void taskDelay(const unsigned long ticksToDelay) {
	clock_t timeToWake;
	bool yield = false;
	/* A delay time of zero just forces a reschedule. */
	if (ticksToDelay > 0) {
		taskSuspendAll();
		{
			/* A task that is removed from the event list while the
			 scheduler is suspended will not get placed in the ready
			 list or removed from the blocked list until the scheduler
			 is resumed. This task cannot be in an event list as it
			 is the currently executing task. */
			timeToWake = timeLowRes() + (clock_t)ticksToDelay;
			/* We must remove ourselves from the ready list before adding
			 ourselves to the blocked list as the same list item is used for
			 both lists. */
			listRemove((OSListItem*)&(currentTCB->genericListItem));
			_taskSetDelayed(timeToWake);
		}
		yield = taskResumeAll();
	}
	/* Force a reschedule if taskResumeAll has not already done so, we may
	 have put ourselves to sleep. */
	if (!yield)
		_taskYield();
}

unsigned int taskPriorityGet(const TaskHandle task) {
	TCB *tcb;
	uint32_t pri;
	_enterCritical();
	{
		/* If null is passed in here then we are finding the
		 priority of the calling function. */
		tcb = _tcbFromHandle(task);
		pri = tcb->priority;
	}
	_exitCritical();
	return pri;
}

void taskPrioritySet(TaskHandle task, const unsigned int newP) {
	TCB *tcb;
	uint32_t priority, newPriority = newP;
	bool yield = false;
	/* Ensure the new priority is valid. */
	if (newPriority >= TASK_MAX_PRIORITIES)
		newPriority = TASK_MAX_PRIORITIES - 1;
	_enterCritical();
	{
		/* If NULL is passed in here then we are changing the
		 priority of the calling function. */
		tcb = _tcbFromHandle(task);
		priority = tcb->basePriority;
		if (priority != newPriority) {
			/* The priority change may have readied a task of higher
			 priority than the calling task. */
			if (newPriority > priority) {
				if (task != NULL)
					/* The priority of another task is being raised.  If we
					 were raising the priority of the currently running task
					 there would be no need to switch as it must have already
					 been the highest priority task. */
					yield = true;
			} else if (task == NULL)
				/* Setting our own priority down means there may now be another
				 task of higher priority that is ready to execute. */
				yield = true;
			/* Only change the priority being used if the task is not
			 currently using an inherited priority. */
			if (tcb->basePriority == tcb->priority )
				tcb->priority = newPriority;
			/* The base priority gets set whatever. */
			tcb->basePriority = newPriority;
			listSetValue(&(tcb->eventListItem), (clock_t)(TASK_MAX_PRIORITIES - newPriority));
			/* If the task is in the blocked or suspended list we need do
			 nothing more than change it's priority variable. However, if
			 the task is in a ready list it needs to be removed and placed
			 in the queue appropriate to its new priority. */
			if (listContains(&(readyTasks[priority]), &(tcb->genericListItem))) {
				/* The task is currently in its ready list - remove before adding
				 it to it's new ready list.  As we are in a critical section we
				 can do this even if the scheduler is suspended. */
				listRemove(&(tcb->genericListItem));
				_taskAddToReadyQueue(tcb);
			}
			if (yield)
				_taskYield();
		}
	}
	_exitCritical();
}

void taskSuspend(TaskHandle taskToSuspend) {
	TCB *tcb;
	_enterCritical();
	{
		/* If null is passed in here then we are suspending ourselves. */
		tcb = _tcbFromHandle(taskToSuspend);
		/* Remove task from the ready/delayed list and place in the	suspended list. */
		listRemove(&(tcb->genericListItem));
		/* Is the task waiting on an event also? */
		if (tcb->eventListItem.container != NULL)
			listRemove(&(tcb->eventListItem));
		listInsertEnd(&suspendedTasks, &(tcb->genericListItem));
	}
	_exitCritical();
	if (taskToSuspend == NULL) {
		if (schedulerStatus & SCHEDULER_RUNNING)
			/* We have just suspended the current task. */
			_taskYield();
		else {
			/* The scheduler is not running, but the task that was pointed
			 to by currentTCB has just been suspended and currentTCB
			 must be adjusted to point to a different task. */
			if (listSize(&suspendedTasks) == taskCount)
				/* No other tasks are ready, so set currentTCB back to
				 NULL so when the next task is created currentTCB will
				 be set to point to it no matter what its relative priority
				 is. */
				currentTCB = NULL;
			else
				_contextSwitch();
		}
	}
}

void taskResume(TaskHandle taskToResume) {
	TCB *tcb  = (TCB*)taskToResume;
	/* Remove the task from whichever list it is currently in, and place
	 it in the ready list. The parameter cannot be NULL as it is impossible
	 to resume the currently executing task. */
	if (tcb != NULL && tcb != currentTCB) {
		_enterCritical();
		{
			// Is task actually suspended?
			if (listContains(&suspendedTasks, &(tcb->genericListItem)) &&
				listContains(NULL, &(tcb->eventListItem))) {
				/* As we are in a critical section we can access the ready
				 lists even if the scheduler is suspended. */
				listRemove(&(tcb->genericListItem));
				_taskAddToReadyQueue(tcb);
				/* We may have just resumed a higher priority task. */
				if (tcb->priority >= currentTCB->priority)
					/* This yield may not cause the task just resumed to run, but
					 will leave the lists in the correct state for the next yield. */
					_taskYield();
			}
		}
		_exitCritical();
	}
}

bool taskResumeISR(TaskHandle taskToResume) {
	bool yield = false;
	TCB *tcb = (TCB*)taskToResume;
	_enterCritical();
	{
		if (tcb != NULL) {
			if (schedulerStatus & SCHEDULER_SUSPENDED)
				/* We cannot access the delayed or ready lists, so will hold this
				 task pending until the scheduler is resumed, at which point a
				 yield will be performed if necessary. */
				listInsertEnd(&(pendingReadyTasks), &(tcb->eventListItem));
			else {
				yield = (tcb->priority >= currentTCB->priority);
				listRemove(&(tcb->genericListItem));
				_taskAddToReadyQueue(tcb);
			}
		}
	}
	_exitCritical();
	return yield;
}

unsigned int taskGetState(TaskHandle task) {
	TCB *tcb = _tcbFromHandle(task);
	OSList *list = (OSList *)(tcb->genericListItem.container);
	// Delayed/blocked tasks inhabit this list
	if (list == &delayedTasks)
		return TASK_SLEEPING;
	// Suspended tasks are in the suspended list
	if (list == &suspendedTasks) {
		if ((OSList *)(tcb->eventListItem.container) == NULL)
			return TASK_SUSPENDED;
		else
			return TASK_SLEEPING;
	}
	// Currently executing task?
	if (tcb == currentTCB)
		return TASK_RUNNING;
	// In a runnable list?
	if (list >= &readyTasks[0] && list <= &readyTasks[TASK_MAX_PRIORITIES - 1])
		return TASK_RUNNABLE;
	return TASK_DEAD;
}

/*-----------------------------------------------------------
 * PUBLIC SCHEDULER CONTROL documented in task.h
 *----------------------------------------------------------*/

void taskStartScheduler() {
	/* Add the idle task at the lowest priority. */
	if (taskCreate(_taskIdleLoop, TASK_IDLE_STACK_SIZE, NULL, TASK_IDLE_PRIORITY) != NULL) {
		/* Interrupts are turned off here, to ensure a tick does not occur
		 before or during the call to taskStartScheduler().  The stacks of
		 the created tasks contain a status word with interrupts switched on
		 so interrupts will automatically get re-enabled when the first task
		 starts to run.
		 STEPPING THROUGH HERE USING A DEBUGGER CAN CAUSE BIG PROBLEMS IF THE
		 DEBUGGER ALLOWS INTERRUPTS TO BE PROCESSED. */
		__disable_irq();
		/* Reset scheduler variables. */
		nextTaskUnblockTime = MAX_DELAY;
		deadTaskCount = 0;
		schedulerStatus = SCHEDULER_RUNNING;
		/* Initialise the critical nesting count ready for the first task. */
		_criticalNesting = 0;
		/* Start the first task. This never returns... */
		_startFirstTask();
	}
}

void taskSuspendAll() {
	schedulerStatus |= SCHEDULER_SUSPENDED;
}

bool taskResumeAll() {
	TCB *tcb;
	bool yield = false;
	/* It is possible that an ISR caused a task to be removed from an event
	 list while the scheduler was suspended.  If this was the case then the
	 removed task will have been added to the pendingReadyTasks list.  Once
	 the scheduler has been resumed it is safe to move all the pending ready
	 tasks from this list into their appropriate ready list. */
	_enterCritical();
	{
		schedulerStatus &= ~SCHEDULER_SUSPENDED;
		if (taskCount > 0) {
			/* Move any readied tasks from the pending list into the
			 appropriate ready list. */
			while (!listIsEmpty(&pendingReadyTasks)) {
				tcb = (TCB*)listGetFirstOwner(&pendingReadyTasks);
				listRemove(&(tcb->eventListItem));
				listRemove(&(tcb->genericListItem));
				_taskAddToReadyQueue(tcb);
				/* If we have moved a task that has a priority higher than
				 the current task then we should yield. */
				if (tcb->priority >= currentTCB->priority)
					yield = true;
			}
			/* If any ticks occurred while the scheduler was suspended then
			 they should be processed now.  This ensures the tick count does not
			 slip, and that any delayed tasks are resumed at the correct time. */
			_taskTick();
			if (yield || missedYield) {
				missedYield = false;
				_taskYield();
			}
		}
	}
	_exitCritical();
	return yield;
}

/*-----------------------------------------------------------
 * PUBLIC TASK UTILITIES documented in task.h
 *----------------------------------------------------------*/

unsigned int taskGetCount() {
	/* A critical section is not required because this variable is atomic... */
	return taskCount;
}

/*-----------------------------------------------------------
 * SCHEDULER INTERNALS AVAILABLE FOR PORTING PURPOSES
 * documented in task.h
 *----------------------------------------------------------*/

/* THIS FUNCTION MUST BE CALLED WITH INTERRUPTS DISABLED OR THE
 SCHEDULER SUSPENDED. */
void _taskSetEvent(OSList *eventList, const clock_t timeout) {
	/* Place the event list item of the TCB in the appropriate event list.
	 This is placed in the list in priority order so the highest priority task
	 is the first to be woken by the event. */
	listInsert(eventList, (OSListItem*)&(currentTCB->eventListItem));
	/* We must remove ourselves from the ready list before adding ourselves
	 to the blocked list as the same list item is used for both lists.  We have
	 exclusive access to the ready lists as the scheduler is locked. */
	listRemove((OSListItem*)&(currentTCB->genericListItem));
	if (timeout == MAX_DELAY)
		/* Add ourselves to the suspended task list instead of a delayed task
		 list to ensure we are not woken by a timing event.  We will block
		 indefinitely. */
		listInsertEnd(&suspendedTasks, (OSListItem*)&(currentTCB->genericListItem));
	else
		/* Calculate the time at which the task should be woken if the event does
		 not occur. This may overflow but this doesn't matter. */
		_taskSetDelayed(timeLowRes() + timeout);
}

/* THIS FUNCTION MUST BE CALLED WITH INTERRUPTS DISABLED OR THE
 SCHEDULER SUSPENDED.  It can also be called from within an ISR. */
bool _taskClearEvent(OSList *eventList) {
	TCB *tcb;
	/* The event list is sorted in priority order, so we can remove the
	 first in the list, remove the TCB from the delayed list, and add
	 it to the ready list.
	 This function assumes that a check has already been made to ensure that
	 eventList is not empty. */
	tcb = (TCB*)listGetFirstOwner(eventList);
	listRemove(&(tcb->eventListItem));
	if (schedulerStatus & SCHEDULER_SUSPENDED)
		/* Queue up change for resume */
		listInsertEnd(&pendingReadyTasks, (OSListItem*)&(tcb->eventListItem));
	else {
		listRemove(&(tcb->genericListItem));
		_taskAddToReadyQueue(tcb);
	}
	/* If we woke a high-priority task, indicate required yield */
	return (tcb->priority >= currentTCB->priority);
}

TaskHandle taskGetCurrent() {
	/* A critical section is not required as this is not called from
	 an interrupt and the current TCB will always be the same for any
	 individual execution thread. */
	return (TaskHandle)currentTCB;
}

void _taskPriorityInherit(TaskHandle mutexHolder) {
	TCB *tcb = (TCB*)mutexHolder;
	if (tcb != NULL && tcb->priority < currentTCB->priority) {
		/* Adjust the mutex holder state to account for its new priority. */
		listSetValue(&(tcb->eventListItem), (clock_t)(TASK_MAX_PRIORITIES -
			currentTCB->priority));
		/* If the task being modified is in the ready state it will need to
		 be moved in to a new list. */
		if (listContains(&(readyTasks[tcb->priority]), &(tcb->genericListItem))) {
			listRemove(&(tcb->genericListItem));
			/* Inherit the priority before being moved into the new list. */
			tcb->priority = currentTCB->priority;
			_taskAddToReadyQueue(tcb);
		} else
			/* Just inherit the priority. */
			tcb->priority = currentTCB->priority;
	}
}

void _taskPriorityDisinherit(TaskHandle mutexHolder) {
	TCB *tcb = (TCB*)mutexHolder;
	if (tcb != NULL && tcb->priority != tcb->basePriority) {
		/* We must be the running task to be able to give the mutex back.
		 Remove ourselves from the ready list we currently appear in. */
		listRemove(&(tcb->genericListItem));
		/* Disinherit the priority before adding the task into the new
		 ready list. */
		tcb->priority = tcb->basePriority;
		listSetValue(&(tcb->eventListItem), (clock_t)(TASK_MAX_PRIORITIES - tcb->priority));
		_taskAddToReadyQueue(tcb);
	}
}
