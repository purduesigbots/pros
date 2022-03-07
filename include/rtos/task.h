/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#include "FreeRTOS.h"

#ifndef INC_TASK_H
#define INC_TASK_H

#ifndef INC_FREERTOS_H
	#error "include FreeRTOS.h must appear in source files before include task.h"
#endif

#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------
 * MACROS AND DEFINITIONS
 *----------------------------------------------------------*/

#define tskKERNEL_VERSION_NUMBER "V10.0.1"
#define tskKERNEL_VERSION_MAJOR 10
#define tskKERNEL_VERSION_MINOR 0
#define tskKERNEL_VERSION_BUILD 1

/**
 * task. h
 *
 * Type by which tasks are referenced.  For example, a call to task_create
 * returns (via a pointer parameter) an task_t variable that can then
 * be used as a parameter to task_delete to delete the task.
 *
 * \defgroup task_t task_t
 * \ingroup Tasks
 */
typedef void * task_t;

/*
 * Defines the prototype to which the application task hook function must
 * conform.
 */
typedef int32_t (*TaskHookFunction_t)( void * );

/* Task states returned by task_get_state. */
typedef enum
{
	E_TASK_STATE_RUNNING = 0,	/* A task is querying the state of itself, so must be running. */
	E_TASK_STATE_READY,			/* The task being queried is in a read or pending ready list. */
	E_TASK_STATE_BLOCKED,		/* The task being queried is in the Blocked state. */
	E_TASK_STATE_SUSPENDED,		/* The task being queried is in the Suspended state, or is in the Blocked state with an infinite time out. */
	E_TASK_STATE_DELETED,		/* The task being queried has been deleted, but its TCB has not yet been freed. */
	E_TASK_STATE_INVALID			/* Used as an 'invalid state' value. */
} task_state_e_t;

/* Actions that can be performed when vTaskNotify() is called. */
typedef enum
{
	E_NOTIFY_ACTION_NONE = 0,				/* Notify the task without updating its notify value. */
	E_NOTIFY_ACTION_BITS,					/* Set bits in the task's notification value. */
	E_NOTIFY_ACTION_INCR,					/* Increment the task's notification value. */
	E_NOTIFY_ACTION_OWRITE,		/* Set the task's notification value to a specific value even if the previous value has not yet been read by the task. */
	E_NOTIFY_ACTION_NO_OWRITE	/* Set the task's notification value if the previous value has been read by the task. */
} notify_action_e_t;

/*
 * Used internally only.
 */
typedef struct xTIME_OUT
{
	int32_t xOverflowCount;
	uint32_t xTimeOnEntering;
} TimeOut_t;


/*
 * Parameters required to create an MPU protected task.
 */
typedef struct xTASK_PARAMETERS
{
	task_fn_t pvTaskCode;
	const char * const pcName;	/*lint !e971 Unqualified char types are allowed for strings and single characters only. */
	uint16_t usStackDepth;
	void *pvParameters;
	uint32_t uxPriority;
	task_stack_t *puxStackBuffer;
} TaskParameters_t;

/* Used with the uxTaskGetSystemState() function to return the state of each task
in the system. */
typedef struct xTASK_STATUS
{
	task_t xHandle;			/* The handle of the task to which the rest of the information in the structure relates. */
	const char *pcTaskName;			/* A pointer to the task's name.  This value will be invalid if the task was deleted since the structure was populated! */ /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
	uint32_t xTaskNumber;		/* A number unique to the task. */
	task_state_e_t eCurrentState;		/* The state in which the task existed when the structure was populated. */
	uint32_t uxCurrentPriority;	/* The priority at which the task was running (may be inherited) when the structure was populated. */
	uint32_t uxBasePriority;		/* The priority to which the task will return if the task's current priority has been inherited to avoid unbounded priority inversion when obtaining a mutex.  Only valid if configUSE_MUTEXES is defined as 1 in FreeRTOSConfig.h. */
	uint32_t ulRunTimeCounter;		/* The total run time allocated to the task so far, as defined by the run time stats clock.  See http://www.freertos.org/rtos-run-time-stats.html.  Only valid when configGENERATE_RUN_TIME_STATS is defined as 1 in FreeRTOSConfig.h. */
	task_stack_t *pxStackBase;		/* Points to the lowest address of the task's stack area. */
	uint16_t usStackHighWaterMark;	/* The minimum amount of stack space that has remained for the task since the task was created.  The closer this value is to zero the closer the task has come to overflowing its stack. */
} TaskStatus_t;

/* Possible return values for eTaskConfirmSleepModeStatus(). */
typedef enum
{
	eAbortSleep = 0,		/* A task has been made ready or a context switch pended since portSUPPORESS_TICKS_AND_SLEEP() was called - abort entering a sleep mode. */
	eStandardSleep,			/* Enter a sleep mode that will not last any longer than the expected idle time. */
	eNoTasksWaitingTimeout	/* No tasks are waiting for a timeout so it is safe to enter a sleep mode that can only be exited by an external interrupt. */
} eSleepModeStatus;

/**
 * Defines the priority used by the idle task.  This must not be modified.
 *
 * \ingroup TaskUtils
 */
#define tskIDLE_PRIORITY			( ( uint32_t ) 0U )

/**
 * task. h
 *
 * Macro for forcing a context switch.
 *
 * \defgroup taskYIELD taskYIELD
 * \ingroup SchedulerControl
 */
#define taskYIELD()					portYIELD()

/**
 * task. h
 *
 * Macro to mark the start of a critical code region.  Preemptive context
 * switches cannot occur when in a critical region.
 *
 * NOTE: This may alter the stack (depending on the portable implementation)
 * so must be used with care!
 *
 * \defgroup taskENTER_CRITICAL taskENTER_CRITICAL
 * \ingroup SchedulerControl
 */
#define taskENTER_CRITICAL()		portENTER_CRITICAL()
#define taskENTER_CRITICAL_FROM_ISR() portSET_INTERRUPT_MASK_FROM_ISR()

/**
 * task. h
 *
 * Macro to mark the end of a critical code region.  Preemptive context
 * switches cannot occur when in a critical region.
 *
 * NOTE: This may alter the stack (depending on the portable implementation)
 * so must be used with care!
 *
 * \defgroup taskEXIT_CRITICAL taskEXIT_CRITICAL
 * \ingroup SchedulerControl
 */
#define taskEXIT_CRITICAL()			portEXIT_CRITICAL()
#define taskEXIT_CRITICAL_FROM_ISR( x ) portCLEAR_INTERRUPT_MASK_FROM_ISR( x )
/**
 * task. h
 *
 * Macro to disable all maskable interrupts.
 *
 * \defgroup taskDISABLE_INTERRUPTS taskDISABLE_INTERRUPTS
 * \ingroup SchedulerControl
 */
#define taskDISABLE_INTERRUPTS()	portDISABLE_INTERRUPTS()

/**
 * task. h
 *
 * Macro to enable microcontroller interrupts.
 *
 * \defgroup taskENABLE_INTERRUPTS taskENABLE_INTERRUPTS
 * \ingroup SchedulerControl
 */
#define taskENABLE_INTERRUPTS()		portENABLE_INTERRUPTS()

/* Definitions returned by xTaskGetSchedulerState().  taskSCHEDULER_SUSPENDED is
0 to generate more optimal code when configASSERT() is defined as the constant
is used in assert() statements. */
#define taskSCHEDULER_SUSPENDED		( ( int32_t ) 0 )
#define taskSCHEDULER_NOT_STARTED	( ( int32_t ) 1 )
#define taskSCHEDULER_RUNNING		( ( int32_t ) 2 )


/*-----------------------------------------------------------
 * TASK CREATION API
 *----------------------------------------------------------*/

/**
 * task. h
 *<pre>
 int32_t task_create(
							  task_fn_t pvTaskCode,
							  const char * const pcName,
							  configSTACK_DEPTH_TYPE usStackDepth,
							  void *pvParameters,
							  uint32_t uxPriority,
							  task_t *pvCreatedTask
						  );</pre>
 *
 * Create a new task and add it to the list of tasks that are ready to run.
 *
 * Internally, within the FreeRTOS implementation, tasks use two blocks of
 * memory.  The first block is used to hold the task's data structures.  The
 * second block is used by the task as its stack.  If a task is created using
 * task_create() then both blocks of memory are automatically dynamically
 * allocated inside the task_create() function.  (see
 * http://www.freertos.org/a00111.html).  If a task is created using
 * task_create_static() then the application writer must provide the required
 * memory.  task_create_static() therefore allows a task to be created without
 * using any dynamic memory allocation.
 *
 * See task_create_static() for a version that does not use any dynamic memory
 * allocation.
 *
 * task_create() can only be used to create a task that has unrestricted
 * access to the entire microcontroller memory map.  Systems that include MPU
 * support can alternatively create an MPU constrained task using
 * xTaskCreateRestricted().
 *
 * @param pvTaskCode Pointer to the task entry function.  Tasks
 * must be implemented to never return (i.e. continuous loop).
 *
 * @param pcName A descriptive name for the task.  This is mainly used to
 * facilitate debugging.  Max length defined by configMAX_TASK_NAME_LEN - default
 * is 16.
 *
 * @param usStackDepth The size of the task stack specified as the number of
 * variables the stack can hold - not the number of bytes.  For example, if
 * the stack is 16 bits wide and usStackDepth is defined as 100, 200 bytes
 * will be allocated for stack storage.
 *
 * @param pvParameters Pointer that will be used as the parameter for the task
 * being created.
 *
 * @param uxPriority The priority at which the task should run.  Systems that
 * include MPU support can optionally create tasks in a privileged (system)
 * mode by setting bit portPRIVILEGE_BIT of the priority parameter.  For
 * example, to create a privileged task at priority 2 the uxPriority parameter
 * should be set to ( 2 | portPRIVILEGE_BIT ).
 *
 * @param pvCreatedTask Used to pass back a handle by which the created task
 * can be referenced.
 *
 * @return pdPASS if the task was successfully created and added to a ready
 * list, otherwise an error code defined in the file projdefs.h
 *
 * Example usage:
   <pre>
 // Task to be created.
 void vTaskCode( void * pvParameters )
 {
	 for( ;; )
	 {
		 // Task code goes here.
	 }
 }

 // Function that creates a task.
 void vOtherFunction( void )
 {
 static uint8_t ucParameterToPass;
 task_t xHandle = NULL;

	 // Create the task, storing the handle.  Note that the passed parameter ucParameterToPass
	 // must exist for the lifetime of the task, so in this case is declared static.  If it was just an
	 // an automatic stack variable it might no longer exist, or at least have been corrupted, by the time
	 // the new task attempts to access it.
	 task_create( vTaskCode, "NAME", STACK_SIZE, &ucParameterToPass, tskIDLE_PRIORITY, &xHandle );
     configASSERT( xHandle );

	 // Use the handle to delete the task.
     if( xHandle != NULL )
     {
	     task_delete( xHandle );
     }
 }
   </pre>
 * \defgroup task_create task_create
 * \ingroup Tasks
 */
#if( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
	task_t task_create(task_fn_t function, void* const parameters,
	                   uint32_t prio, const uint16_t stack_depth,
	                   const char* const name);
#endif

/**
 * task. h
 *<pre>
 task_t task_create_static( task_fn_t pvTaskCode,
								 const char * const pcName,
								 uint32_t ulStackDepth,
								 void *pvParameters,
								 uint32_t uxPriority,
								 task_stack_t *pxStackBuffer,
								 static_task_s_t *pxTaskBuffer );</pre>
 *
 * Create a new task and add it to the list of tasks that are ready to run.
 *
 * Internally, within the FreeRTOS implementation, tasks use two blocks of
 * memory.  The first block is used to hold the task's data structures.  The
 * second block is used by the task as its stack.  If a task is created using
 * task_create() then both blocks of memory are automatically dynamically
 * allocated inside the task_create() function.  (see
 * http://www.freertos.org/a00111.html).  If a task is created using
 * task_create_static() then the application writer must provide the required
 * memory.  task_create_static() therefore allows a task to be created without
 * using any dynamic memory allocation.
 *
 * @param pvTaskCode Pointer to the task entry function.  Tasks
 * must be implemented to never return (i.e. continuous loop).
 *
 * @param pcName A descriptive name for the task.  This is mainly used to
 * facilitate debugging.  The maximum length of the string is defined by
 * configMAX_TASK_NAME_LEN in FreeRTOSConfig.h.
 *
 * @param ulStackDepth The size of the task stack specified as the number of
 * variables the stack can hold - not the number of bytes.  For example, if
 * the stack is 32-bits wide and ulStackDepth is defined as 100 then 400 bytes
 * will be allocated for stack storage.
 *
 * @param pvParameters Pointer that will be used as the parameter for the task
 * being created.
 *
 * @param uxPriority The priority at which the task will run.
 *
 * @param pxStackBuffer Must point to a task_stack_t array that has at least
 * ulStackDepth indexes - the array will then be used as the task's stack,
 * removing the need for the stack to be allocated dynamically.
 *
 * @param pxTaskBuffer Must point to a variable of type static_task_s_t, which will
 * then be used to hold the task's data structures, removing the need for the
 * memory to be allocated dynamically.
 *
 * @return If neither pxStackBuffer or pxTaskBuffer are NULL, then the task will
 * be created and pdPASS is returned.  If either pxStackBuffer or pxTaskBuffer
 * are NULL then the task will not be created and
 * errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY is returned.
 *
 * Example usage:
   <pre>

    // Dimensions the buffer that the task being created will use as its stack.
    // NOTE:  This is the number of words the stack will hold, not the number of
    // bytes.  For example, if each stack item is 32-bits, and this is set to 100,
    // then 400 bytes (100 * 32-bits) will be allocated.
    #define STACK_SIZE 200

    // Structure that will hold the TCB of the task being created.
    static_task_s_t xTaskBuffer;

    // Buffer that the task being created will use as its stack.  Note this is
    // an array of task_stack_t variables.  The size of task_stack_t is dependent on
    // the RTOS port.
    task_stack_t xStack[ STACK_SIZE ];

    // Function that implements the task being created.
    void vTaskCode( void * pvParameters )
    {
        // The parameter value is expected to be 1 as 1 is passed in the
        // pvParameters value in the call to task_create_static().
        configASSERT( ( uint32_t ) pvParameters == 1UL );

        for( ;; )
        {
            // Task code goes here.
        }
    }

    // Function that creates a task.
    void vOtherFunction( void )
    {
        task_t xHandle = NULL;

        // Create the task without using any dynamic memory allocation.
        xHandle = task_create_static(
                      vTaskCode,       // Function that implements the task.
                      "NAME",          // Text name for the task.
                      STACK_SIZE,      // Stack size in words, not bytes.
                      ( void * ) 1,    // Parameter passed into the task.
                      tskIDLE_PRIORITY,// Priority at which the task is created.
                      xStack,          // Array to use as the task's stack.
                      &xTaskBuffer );  // Variable to hold the task's data structure.

        // puxStackBuffer and pxTaskBuffer were not NULL, so the task will have
        // been created, and xHandle will be the task's handle.  Use the handle
        // to suspend the task.
        task_suspend( xHandle );
    }
   </pre>
 * \defgroup task_create_static task_create_static
 * \ingroup Tasks
 */
#if( configSUPPORT_STATIC_ALLOCATION == 1 )
	task_t task_create_static(	task_fn_t task_code, void* const param,
	                            uint32_t priority, const size_t stack_size,
	                            const char* const name,
	                            task_stack_t * const stack_buffer,
	                            static_task_s_t * const task_buffer ) ;
#endif /* configSUPPORT_STATIC_ALLOCATION */

/**
 * task. h
 * <pre>void task_delete( task_t xTask );</pre>
 *
 * INCLUDE_vTaskDelete must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 * Remove a task from the RTOS real time kernel's management.  The task being
 * deleted will be removed from all ready, blocked, suspended and event lists.
 *
 * NOTE:  The idle task is responsible for freeing the kernel allocated
 * memory from tasks that have been deleted.  It is therefore important that
 * the idle task is not starved of microcontroller processing time if your
 * application makes any calls to task_delete ().  Memory allocated by the
 * task code is not automatically freed, and should be freed before the task
 * is deleted.
 *
 * See the demo application file death.c for sample code that utilises
 * task_delete ().
 *
 * @param xTask The handle of the task to be deleted.  Passing NULL will
 * cause the calling task to be deleted.
 *
 * Example usage:
   <pre>
 void vOtherFunction( void )
 {
 task_t xHandle;

	 // Create the task, storing the handle.
	 task_create( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY, &xHandle );

	 // Use the handle to delete the task.
	 task_delete( xHandle );
 }
   </pre>
 * \defgroup task_delete task_delete
 * \ingroup Tasks
 */
void task_delete( task_t xTaskToDelete ) ;

/*-----------------------------------------------------------
 * TASK CONTROL API
 *----------------------------------------------------------*/

/**
 * task. h
 * <pre>void task_delay( const uint32_t xTicksToDelay );</pre>
 *
 * Delay a task for a given number of ticks.  The actual time that the
 * task remains blocked depends on the tick rate.  The constant
 * portTICK_PERIOD_MS can be used to calculate real time from the tick
 * rate - with the resolution of one tick period.
 *
 * INCLUDE_vTaskDelay must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 *
 * task_delay() specifies a time at which the task wishes to unblock relative to
 * the time at which task_delay() is called.  For example, specifying a block
 * period of 100 ticks will cause the task to unblock 100 ticks after
 * task_delay() is called.  task_delay() does not therefore provide a good method
 * of controlling the frequency of a periodic task as the path taken through the
 * code, as well as other task and interrupt activity, will effect the frequency
 * at which task_delay() gets called and therefore the time at which the task
 * next executes.  See task_delay_until() for an alternative API function designed
 * to facilitate fixed frequency execution.  It does this by specifying an
 * absolute time (rather than a relative time) at which the calling task should
 * unblock.
 *
 * @param xTicksToDelay The amount of time, in tick periods, that
 * the calling task should block.
 *
 * Example usage:

 void vTaskFunction( void * pvParameters )
 {
 // Block for 500ms.
 const uint32_t xDelay = 500 / portTICK_PERIOD_MS;

	 for( ;; )
	 {
		 // Simply toggle the LED every 500ms, blocking between each toggle.
		 vToggleLED();
		 task_delay( xDelay );
	 }
 }

 * \defgroup task_delay task_delay
 * \ingroup TaskCtrl
 */
void task_delay( const uint32_t xTicksToDelay ) ;

/**
 * task. h
 * <pre>void task_delay_until( uint32_t *pxPreviousWakeTime, const uint32_t xTimeIncrement );</pre>
 *
 * INCLUDE_vTaskDelayUntil must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 * Delay a task until a specified time.  This function can be used by periodic
 * tasks to ensure a constant execution frequency.
 *
 * This function differs from task_delay () in one important aspect:  task_delay () will
 * cause a task to block for the specified number of ticks from the time task_delay () is
 * called.  It is therefore difficult to use task_delay () by itself to generate a fixed
 * execution frequency as the time between a task starting to execute and that task
 * calling task_delay () may not be fixed [the task may take a different path though the
 * code between calls, or may get interrupted or preempted a different number of times
 * each time it executes].
 *
 * Whereas task_delay () specifies a wake time relative to the time at which the function
 * is called, task_delay_until () specifies the absolute (exact) time at which it wishes to
 * unblock.
 *
 * The constant portTICK_PERIOD_MS can be used to calculate real time from the tick
 * rate - with the resolution of one tick period.
 *
 * @param pxPreviousWakeTime Pointer to a variable that holds the time at which the
 * task was last unblocked.  The variable must be initialised with the current time
 * prior to its first use (see the example below).  Following this the variable is
 * automatically updated within task_delay_until ().
 *
 * @param xTimeIncrement The cycle time period.  The task will be unblocked at
 * time *pxPreviousWakeTime + xTimeIncrement.  Calling task_delay_until with the
 * same xTimeIncrement parameter value will cause the task to execute with
 * a fixed interface period.
 *
 * Example usage:
   <pre>
 // Perform an action every 10 ticks.
 void vTaskFunction( void * pvParameters )
 {
 uint32_t xLastWakeTime;
 const uint32_t xFrequency = 10;

	 // Initialise the xLastWakeTime variable with the current time.
	 xLastWakeTime = millis ();
	 for( ;; )
	 {
		 // Wait for the next cycle.
		 task_delay_until( &xLastWakeTime, xFrequency );

		 // Perform action here.
	 }
 }
   </pre>
 * \defgroup task_delay_until task_delay_until
 * \ingroup TaskCtrl
 */
void task_delay_until( uint32_t * const pxPreviousWakeTime, const uint32_t xTimeIncrement ) ;

/**
 * task. h
 * <pre>int32_t task_abort_delay( task_t xTask );</pre>
 *
 * INCLUDE_xTaskAbortDelay must be defined as 1 in FreeRTOSConfig.h for this
 * function to be available.
 *
 * A task will enter the Blocked state when it is waiting for an event.  The
 * event it is waiting for can be a temporal event (waiting for a time), such
 * as when task_delay() is called, or an event on an object, such as when
 * queue_recv() or task_notify_take() is called.  If the handle of a task
 * that is in the Blocked state is used in a call to task_abort_delay() then the
 * task will leave the Blocked state, and return from whichever function call
 * placed the task into the Blocked state.
 *
 * @param xTask The handle of the task to remove from the Blocked state.
 *
 * @return If the task referenced by xTask was not in the Blocked state then
 * pdFAIL is returned.  Otherwise pdPASS is returned.
 *
 * \defgroup task_abort_delay task_abort_delay
 * \ingroup TaskCtrl
 */
int32_t task_abort_delay( task_t xTask ) ;

/**
 * task. h
 * <pre>uint32_t task_get_priority( task_t xTask );</pre>
 *
 * INCLUDE_uxTaskPriorityGet must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 * Obtain the priority of any task.
 *
 * @param xTask Handle of the task to be queried.  Passing a NULL
 * handle results in the priority of the calling task being returned.
 *
 * @return The priority of xTask.
 *
 * Example usage:
   <pre>
 void vAFunction( void )
 {
 task_t xHandle;

	 // Create a task, storing the handle.
	 task_create( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY, &xHandle );

	 // ...

	 // Use the handle to obtain the priority of the created task.
	 // It was created with tskIDLE_PRIORITY, but may have changed
	 // it itself.
	 if( task_get_priority( xHandle ) != tskIDLE_PRIORITY )
	 {
		 // The task has changed it's priority.
	 }

	 // ...

	 // Is our priority higher than the created task?
	 if( task_get_priority( xHandle ) < task_get_priority( NULL ) )
	 {
		 // Our priority (obtained using NULL handle) is higher.
	 }
 }
   </pre>
 * \defgroup task_get_priority task_get_priority
 * \ingroup TaskCtrl
 */
uint32_t task_get_priority( task_t xTask ) ;

/**
 * task. h
 * <pre>uint32_t uxTaskPriorityGetFromISR( task_t xTask );</pre>
 *
 * A version of task_get_priority() that can be used from an ISR.
 */
uint32_t uxTaskPriorityGetFromISR( task_t xTask ) ;

/**
 * task. h
 * <pre>task_state_e_t task_get_state( task_t xTask );</pre>
 *
 * INCLUDE_eTaskGetState must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 * Obtain the state of any task.  States are encoded by the task_state_e_t
 * enumerated type.
 *
 * @param xTask Handle of the task to be queried.
 *
 * @return The state of xTask at the time the function was called.  Note the
 * state of the task might change between the function being called, and the
 * functions return value being tested by the calling task.
 */
task_state_e_t task_get_state( task_t xTask ) ;

/**
 * task. h
 * <pre>void vTaskGetInfo( task_t xTask, TaskStatus_t *pxTaskStatus, int32_t xGetFreeStackSpace, task_state_e_t eState );</pre>
 *
 * configUSE_TRACE_FACILITY must be defined as 1 for this function to be
 * available.  See the configuration section for more information.
 *
 * Populates a TaskStatus_t structure with information about a task.
 *
 * @param xTask Handle of the task being queried.  If xTask is NULL then
 * information will be returned about the calling task.
 *
 * @param pxTaskStatus A pointer to the TaskStatus_t structure that will be
 * filled with information about the task referenced by the handle passed using
 * the xTask parameter.
 *
 * @xGetFreeStackSpace The TaskStatus_t structure contains a member to report
 * the stack high water mark of the task being queried.  Calculating the stack
 * high water mark takes a relatively long time, and can make the system
 * temporarily unresponsive - so the xGetFreeStackSpace parameter is provided to
 * allow the high water mark checking to be skipped.  The high watermark value
 * will only be written to the TaskStatus_t structure if xGetFreeStackSpace is
 * not set to pdFALSE;
 *
 * @param eState The TaskStatus_t structure contains a member to report the
 * state of the task being queried.  Obtaining the task state is not as fast as
 * a simple assignment - so the eState parameter is provided to allow the state
 * information to be omitted from the TaskStatus_t structure.  To obtain state
 * information then set eState to E_TASK_STATE_INVALID - otherwise the value passed in
 * eState will be reported as the task state in the TaskStatus_t structure.
 *
 * Example usage:
   <pre>
 void vAFunction( void )
 {
 task_t xHandle;
 TaskStatus_t xTaskDetails;

    // Obtain the handle of a task from its name.
    xHandle = task_get_by_name( "Task_Name" );

    // Check the handle is not NULL.
    configASSERT( xHandle );

    // Use the handle to obtain further information about the task.
    vTaskGetInfo( xHandle,
                  &xTaskDetails,
                  pdTRUE, // Include the high water mark in xTaskDetails.
                  E_TASK_STATE_INVALID ); // Include the task state in xTaskDetails.
 }
   </pre>
 * \defgroup vTaskGetInfo vTaskGetInfo
 * \ingroup TaskCtrl
 */
void vTaskGetInfo( task_t xTask, TaskStatus_t *pxTaskStatus, int32_t xGetFreeStackSpace, task_state_e_t eState ) ;

/**
 * task. h
 * <pre>void task_set_priority( task_t xTask, uint32_t uxNewPriority );</pre>
 *
 * INCLUDE_vTaskPrioritySet must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 * Set the priority of any task.
 *
 * A context switch will occur before the function returns if the priority
 * being set is higher than the currently executing task.
 *
 * @param xTask Handle to the task for which the priority is being set.
 * Passing a NULL handle results in the priority of the calling task being set.
 *
 * @param uxNewPriority The priority to which the task will be set.
 *
 * Example usage:
   <pre>
 void vAFunction( void )
 {
 task_t xHandle;

	 // Create a task, storing the handle.
	 task_create( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY, &xHandle );

	 // ...

	 // Use the handle to raise the priority of the created task.
	 task_set_priority( xHandle, tskIDLE_PRIORITY + 1 );

	 // ...

	 // Use a NULL handle to raise our priority to the same value.
	 task_set_priority( NULL, tskIDLE_PRIORITY + 1 );
 }
   </pre>
 * \defgroup task_set_priority task_set_priority
 * \ingroup TaskCtrl
 */
void task_set_priority( task_t xTask, uint32_t uxNewPriority ) ;

/**
 * task. h
 * <pre>void task_suspend( task_t xTaskToSuspend );</pre>
 *
 * INCLUDE_vTaskSuspend must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 * Suspend any task.  When suspended a task will never get any microcontroller
 * processing time, no matter what its priority.
 *
 * Calls to task_suspend are not accumulative -
 * i.e. calling task_suspend () twice on the same task still only requires one
 * call to task_resume () to ready the suspended task.
 *
 * @param xTaskToSuspend Handle to the task being suspended.  Passing a NULL
 * handle will cause the calling task to be suspended.
 *
 * Example usage:
   <pre>
 void vAFunction( void )
 {
 task_t xHandle;

	 // Create a task, storing the handle.
	 task_create( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY, &xHandle );

	 // ...

	 // Use the handle to suspend the created task.
	 task_suspend( xHandle );

	 // ...

	 // The created task will not run during this period, unless
	 // another task calls task_resume( xHandle ).

	 //...


	 // Suspend ourselves.
	 task_suspend( NULL );

	 // We cannot get here unless another task calls task_resume
	 // with our handle as the parameter.
 }
   </pre>
 * \defgroup task_suspend task_suspend
 * \ingroup TaskCtrl
 */
void task_suspend( task_t xTaskToSuspend ) ;

/**
 * task. h
 * <pre>void task_resume( task_t xTaskToResume );</pre>
 *
 * INCLUDE_vTaskSuspend must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 * Resumes a suspended task.
 *
 * A task that has been suspended by one or more calls to task_suspend ()
 * will be made available for running again by a single call to
 * task_resume ().
 *
 * @param xTaskToResume Handle to the task being readied.
 *
 * Example usage:
   <pre>
 void vAFunction( void )
 {
 task_t xHandle;

	 // Create a task, storing the handle.
	 task_create( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY, &xHandle );

	 // ...

	 // Use the handle to suspend the created task.
	 task_suspend( xHandle );

	 // ...

	 // The created task will not run during this period, unless
	 // another task calls task_resume( xHandle ).

	 //...


	 // Resume the suspended task ourselves.
	 task_resume( xHandle );

	 // The created task will once again get microcontroller processing
	 // time in accordance with its priority within the system.
 }
   </pre>
 * \defgroup task_resume task_resume
 * \ingroup TaskCtrl
 */
void task_resume( task_t xTaskToResume ) ;

/**
 * task. h
 * <pre>void xTaskResumeFromISR( task_t xTaskToResume );</pre>
 *
 * INCLUDE_xTaskResumeFromISR must be defined as 1 for this function to be
 * available.  See the configuration section for more information.
 *
 * An implementation of task_resume() that can be called from within an ISR.
 *
 * A task that has been suspended by one or more calls to task_suspend ()
 * will be made available for running again by a single call to
 * xTaskResumeFromISR ().
 *
 * xTaskResumeFromISR() should not be used to synchronise a task with an
 * interrupt if there is a chance that the interrupt could arrive prior to the
 * task being suspended - as this can lead to interrupts being missed. Use of a
 * semaphore as a synchronisation mechanism would avoid this eventuality.
 *
 * @param xTaskToResume Handle to the task being readied.
 *
 * @return pdTRUE if resuming the task should result in a context switch,
 * otherwise pdFALSE. This is used by the ISR to determine if a context switch
 * may be required following the ISR.
 *
 * \defgroup vTaskResumeFromISR vTaskResumeFromISR
 * \ingroup TaskCtrl
 */
int32_t xTaskResumeFromISR( task_t xTaskToResume ) ;

/*-----------------------------------------------------------
 * SCHEDULER CONTROL
 *----------------------------------------------------------*/

/**
 * task. h
 * <pre>void rtos_sched_start( void );</pre>
 *
 * Starts the real time kernel tick processing.  After calling the kernel
 * has control over which tasks are executed and when.
 *
 * See the demo application file main.c for an example of creating
 * tasks and starting the kernel.
 *
 * Example usage:
   <pre>
 void vAFunction( void )
 {
	 // Create at least one task before starting the kernel.
	 task_create( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );

	 // Start the real time kernel with preemption.
	 rtos_sched_start ();

	 // Will not get here unless a task calls rtos_sched_stop ()
 }
   </pre>
 *
 * \defgroup rtos_sched_start rtos_sched_start
 * \ingroup SchedulerControl
 */
void rtos_sched_start( void ) ;

/**
 * task. h
 * <pre>void rtos_sched_stop( void );</pre>
 *
 * NOTE:  At the time of writing only the x86 real mode port, which runs on a PC
 * in place of DOS, implements this function.
 *
 * Stops the real time kernel tick.  All created tasks will be automatically
 * deleted and multitasking (either preemptive or cooperative) will
 * stop.  Execution then resumes from the point where rtos_sched_start ()
 * was called, as if rtos_sched_start () had just returned.
 *
 * See the demo application file main. c in the demo/PC directory for an
 * example that uses rtos_sched_stop ().
 *
 * rtos_sched_stop () requires an exit function to be defined within the
 * portable layer (see vPortEndScheduler () in port. c for the PC port).  This
 * performs hardware specific operations such as stopping the kernel tick.
 *
 * rtos_sched_stop () will cause all of the resources allocated by the
 * kernel to be freed - but will not free resources allocated by application
 * tasks.
 *
 * Example usage:
   <pre>
 void vTaskCode( void * pvParameters )
 {
	 for( ;; )
	 {
		 // Task code goes here.

		 // At some point we want to end the real time kernel processing
		 // so call ...
		 rtos_sched_stop ();
	 }
 }

 void vAFunction( void )
 {
	 // Create at least one task before starting the kernel.
	 task_create( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );

	 // Start the real time kernel with preemption.
	 rtos_sched_start ();

	 // Will only get here when the vTaskCode () task has called
	 // rtos_sched_stop ().  When we get here we are back to single task
	 // execution.
 }
   </pre>
 *
 * \defgroup rtos_sched_stop rtos_sched_stop
 * \ingroup SchedulerControl
 */
void rtos_sched_stop( void ) ;

/**
 * task. h
 * <pre>void rtos_suspend_all( void );</pre>
 *
 * Suspends the scheduler without disabling interrupts.  Context switches will
 * not occur while the scheduler is suspended.
 *
 * After calling rtos_suspend_all () the calling task will continue to execute
 * without risk of being swapped out until a call to rtos_resume_all () has been
 * made.
 *
 * API functions that have the potential to cause a context switch (for example,
 * task_delay_until(), xQueueSend(), etc.) must not be called while the scheduler
 * is suspended.
 *
 * Example usage:
   <pre>
 void vTask1( void * pvParameters )
 {
	 for( ;; )
	 {
		 // Task code goes here.

		 // ...

		 // At some point the task wants to perform a long operation during
		 // which it does not want to get swapped out.  It cannot use
		 // taskENTER_CRITICAL ()/taskEXIT_CRITICAL () as the length of the
		 // operation may cause interrupts to be missed - including the
		 // ticks.

		 // Prevent the real time kernel swapping out the task.
		 rtos_suspend_all ();

		 // Perform the operation here.  There is no need to use critical
		 // sections as we have all the microcontroller processing time.
		 // During this time interrupts will still operate and the kernel
		 // tick count will be maintained.

		 // ...

		 // The operation is complete.  Restart the kernel.
		 rtos_resume_all ();
	 }
 }
   </pre>
 * \defgroup rtos_suspend_all rtos_suspend_all
 * \ingroup SchedulerControl
 */
void rtos_suspend_all( void ) ;

/**
 * task. h
 * <pre>int32_t rtos_resume_all( void );</pre>
 *
 * Resumes scheduler activity after it was suspended by a call to
 * rtos_suspend_all().
 *
 * rtos_resume_all() only resumes the scheduler.  It does not unsuspend tasks
 * that were previously suspended by a call to task_suspend().
 *
 * @return If resuming the scheduler caused a context switch then pdTRUE is
 *		  returned, otherwise pdFALSE is returned.
 *
 * Example usage:
   <pre>
 void vTask1( void * pvParameters )
 {
	 for( ;; )
	 {
		 // Task code goes here.

		 // ...

		 // At some point the task wants to perform a long operation during
		 // which it does not want to get swapped out.  It cannot use
		 // taskENTER_CRITICAL ()/taskEXIT_CRITICAL () as the length of the
		 // operation may cause interrupts to be missed - including the
		 // ticks.

		 // Prevent the real time kernel swapping out the task.
		 rtos_suspend_all ();

		 // Perform the operation here.  There is no need to use critical
		 // sections as we have all the microcontroller processing time.
		 // During this time interrupts will still operate and the real
		 // time kernel tick count will be maintained.

		 // ...

		 // The operation is complete.  Restart the kernel.  We want to force
		 // a context switch - but there is no point if resuming the scheduler
		 // caused a context switch already.
		 if( !rtos_resume_all () )
		 {
			  taskYIELD ();
		 }
	 }
 }
   </pre>
 * \defgroup rtos_resume_all rtos_resume_all
 * \ingroup SchedulerControl
 */
int32_t rtos_resume_all( void ) ;

/*-----------------------------------------------------------
 * TASK UTILITIES
 *----------------------------------------------------------*/

/**
 * task. h
 * <PRE>uint32_t millis( void );</PRE>
 *
 * @return The count of ticks since rtos_sched_start was called.
 *
 * \defgroup millis millis
 * \ingroup TaskUtils
 */
uint32_t millis( void ) ;

/**
 * task. h
 * <PRE>uint32_t xTaskGetTickCountFromISR( void );</PRE>
 *
 * @return The count of ticks since rtos_sched_start was called.
 *
 * This is a version of millis() that is safe to be called from an
 * ISR - provided that uint32_t is the natural word size of the
 * microcontroller being used or interrupt nesting is either not supported or
 * not being used.
 *
 * \defgroup xTaskGetTickCountFromISR xTaskGetTickCountFromISR
 * \ingroup TaskUtils
 */
uint32_t xTaskGetTickCountFromISR( void ) ;

/**
 * task. h
 * <PRE>uint16_t task_get_count( void );</PRE>
 *
 * @return The number of tasks that the real time kernel is currently managing.
 * This includes all ready, blocked and suspended tasks.  A task that
 * has been deleted but not yet freed by the idle task will also be
 * included in the count.
 *
 * \defgroup task_get_count task_get_count
 * \ingroup TaskUtils
 */
uint32_t task_get_count( void ) ;

/**
 * task. h
 * <PRE>char *task_get_name( task_t xTaskToQuery );</PRE>
 *
 * @return The text (human readable) name of the task referenced by the handle
 * xTaskToQuery.  A task can query its own name by either passing in its own
 * handle, or by setting xTaskToQuery to NULL.
 *
 * \defgroup task_get_name task_get_name
 * \ingroup TaskUtils
 */
char *task_get_name( task_t xTaskToQuery ) ; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

/**
 * task. h
 * <PRE>task_t task_get_by_name( const char *pcNameToQuery );</PRE>
 *
 * NOTE:  This function takes a relatively long time to complete and should be
 * used sparingly.
 *
 * @return The handle of the task that has the human readable name pcNameToQuery.
 * NULL is returned if no matching name is found.  INCLUDE_xTaskGetHandle
 * must be set to 1 in FreeRTOSConfig.h for pcTaskGetHandle() to be available.
 *
 * \defgroup pcTaskGetHandle pcTaskGetHandle
 * \ingroup TaskUtils
 */
task_t task_get_by_name( const char *pcNameToQuery ) ; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

/**
 * task.h
 * <PRE>uint32_t uxTaskGetStackHighWaterMark( task_t xTask );</PRE>
 *
 * INCLUDE_uxTaskGetStackHighWaterMark must be set to 1 in FreeRTOSConfig.h for
 * this function to be available.
 *
 * Returns the high water mark of the stack associated with xTask.  That is,
 * the minimum free stack space there has been (in words, so on a 32 bit machine
 * a value of 1 means 4 bytes) since the task started.  The smaller the returned
 * number the closer the task has come to overflowing its stack.
 *
 * @param xTask Handle of the task associated with the stack to be checked.
 * Set xTask to NULL to check the stack of the calling task.
 *
 * @return The smallest amount of free stack space there has been (in words, so
 * actual spaces on the stack rather than bytes) since the task referenced by
 * xTask was created.
 */
uint32_t uxTaskGetStackHighWaterMark( task_t xTask ) ;

/* When using trace macros it is sometimes necessary to include task.h before
FreeRTOS.h.  When this is done TaskHookFunction_t will not yet have been defined,
so the following two prototypes will cause a compilation error.  This can be
fixed by simply guarding against the inclusion of these two prototypes unless
they are explicitly required by the configUSE_APPLICATION_TASK_TAG configuration
constant. */
#ifdef configUSE_APPLICATION_TASK_TAG
	#if configUSE_APPLICATION_TASK_TAG == 1
		/**
		 * task.h
		 * <pre>void vTaskSetApplicationTaskTag( task_t xTask, TaskHookFunction_t pxHookFunction );</pre>
		 *
		 * Sets pxHookFunction to be the task hook function used by the task xTask.
		 * Passing xTask as NULL has the effect of setting the calling tasks hook
		 * function.
		 */
		void vTaskSetApplicationTaskTag( task_t xTask, TaskHookFunction_t pxHookFunction ) ;

		/**
		 * task.h
		 * <pre>void xTaskGetApplicationTaskTag( task_t xTask );</pre>
		 *
		 * Returns the pxHookFunction value assigned to the task xTask.
		 */
		TaskHookFunction_t xTaskGetApplicationTaskTag( task_t xTask ) ;
	#endif /* configUSE_APPLICATION_TASK_TAG ==1 */
#endif /* ifdef configUSE_APPLICATION_TASK_TAG */

#if( configNUM_THREAD_LOCAL_STORAGE_POINTERS > 0 )

	/* Each task contains an array of pointers that is dimensioned by the
	configNUM_THREAD_LOCAL_STORAGE_POINTERS setting in FreeRTOSConfig.h.  The
	kernel does not use the pointers itself, so the application writer can use
	the pointers for any purpose they wish.  The following two functions are
	used to set and query a pointer respectively. */
	void vTaskSetThreadLocalStoragePointer( task_t xTaskToSet, int32_t xIndex, void *pvValue ) ;
	void *pvTaskGetThreadLocalStoragePointer( task_t xTaskToQuery, int32_t xIndex ) ;

#endif

/**
 * task.h
 * <pre>int32_t xTaskCallApplicationTaskHook( task_t xTask, void *pvParameter );</pre>
 *
 * Calls the hook function associated with xTask.  Passing xTask as NULL has
 * the effect of calling the Running tasks (the calling task) hook function.
 *
 * pvParameter is passed to the hook function for the task to interpret as it
 * wants.  The return value is the value returned by the task hook function
 * registered by the user.
 */
int32_t xTaskCallApplicationTaskHook( task_t xTask, void *pvParameter ) ;

/**
 * xTaskGetIdleTaskHandle() is only available if
 * INCLUDE_xTaskGetIdleTaskHandle is set to 1 in FreeRTOSConfig.h.
 *
 * Simply returns the handle of the idle task.  It is not valid to call
 * xTaskGetIdleTaskHandle() before the scheduler has been started.
 */
task_t xTaskGetIdleTaskHandle( void ) ;

/**
 * configUSE_TRACE_FACILITY must be defined as 1 in FreeRTOSConfig.h for
 * uxTaskGetSystemState() to be available.
 *
 * uxTaskGetSystemState() populates an TaskStatus_t structure for each task in
 * the system.  TaskStatus_t structures contain, among other things, members
 * for the task handle, task name, task priority, task state, and total amount
 * of run time consumed by the task.  See the TaskStatus_t structure
 * definition in this file for the full member list.
 *
 * NOTE:  This function is intended for debugging use only as its use results in
 * the scheduler remaining suspended for an extended period.
 *
 * @param pxTaskStatusArray A pointer to an array of TaskStatus_t structures.
 * The array must contain at least one TaskStatus_t structure for each task
 * that is under the control of the RTOS.  The number of tasks under the control
 * of the RTOS can be determined using the task_get_count() API function.
 *
 * @param uxArraySize The size of the array pointed to by the pxTaskStatusArray
 * parameter.  The size is specified as the number of indexes in the array, or
 * the number of TaskStatus_t structures contained in the array, not by the
 * number of bytes in the array.
 *
 * @param pulTotalRunTime If configGENERATE_RUN_TIME_STATS is set to 1 in
 * FreeRTOSConfig.h then *pulTotalRunTime is set by uxTaskGetSystemState() to the
 * total run time (as defined by the run time stats clock, see
 * http://www.freertos.org/rtos-run-time-stats.html) since the target booted.
 * pulTotalRunTime can be set to NULL to omit the total run time information.
 *
 * @return The number of TaskStatus_t structures that were populated by
 * uxTaskGetSystemState().  This should equal the number returned by the
 * task_get_count() API function, but will be zero if the value passed
 * in the uxArraySize parameter was too small.
 *
 * Example usage:
   <pre>
    // This example demonstrates how a human readable table of run time stats
	// information is generated from raw data provided by uxTaskGetSystemState().
	// The human readable table is written to pcWriteBuffer
	void vTaskGetRunTimeStats( char *pcWriteBuffer )
	{
	TaskStatus_t *pxTaskStatusArray;
	volatile uint32_t uxArraySize, x;
	uint32_t ulTotalRunTime, ulStatsAsPercentage;

		// Make sure the write buffer does not contain a string.
		*pcWriteBuffer = 0x00;

		// Take a snapshot of the number of tasks in case it changes while this
		// function is executing.
		uxArraySize = task_get_count();

		// Allocate a TaskStatus_t structure for each task.  An array could be
		// allocated statically at compile time.
		pxTaskStatusArray = kmalloc( uxArraySize * sizeof( TaskStatus_t ) );

		if( pxTaskStatusArray != NULL )
		{
			// Generate raw status information about each task.
			uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, &ulTotalRunTime );

			// For percentage calculations.
			ulTotalRunTime /= 100UL;

			// Avoid divide by zero errors.
			if( ulTotalRunTime > 0 )
			{
				// For each populated position in the pxTaskStatusArray array,
				// format the raw data as human readable ASCII data
				for( x = 0; x < uxArraySize; x++ )
				{
					// What percentage of the total run time has the task used?
					// This will always be rounded down to the nearest integer.
					// ulTotalRunTimeDiv100 has already been divided by 100.
					ulStatsAsPercentage = pxTaskStatusArray[ x ].ulRunTimeCounter / ulTotalRunTime;

					if( ulStatsAsPercentage > 0UL )
					{
						sprintf( pcWriteBuffer, "%s\t\t%lu\t\t%lu%%\r\n", pxTaskStatusArray[ x ].pcTaskName, pxTaskStatusArray[ x ].ulRunTimeCounter, ulStatsAsPercentage );
					}
					else
					{
						// If the percentage is zero here then the task has
						// consumed less than 1% of the total run time.
						sprintf( pcWriteBuffer, "%s\t\t%lu\t\t<1%%\r\n", pxTaskStatusArray[ x ].pcTaskName, pxTaskStatusArray[ x ].ulRunTimeCounter );
					}

					pcWriteBuffer += strlen( ( char * ) pcWriteBuffer );
				}
			}

			// The array is no longer needed, free the memory it consumes.
			kfree( pxTaskStatusArray );
		}
	}
	</pre>
 */
uint32_t uxTaskGetSystemState( TaskStatus_t * const pxTaskStatusArray, const uint32_t uxArraySize, uint32_t * const pulTotalRunTime ) ;

/**
 * task. h
 * <PRE>void vTaskList( char *pcWriteBuffer );</PRE>
 *
 * configUSE_TRACE_FACILITY and configUSE_STATS_FORMATTING_FUNCTIONS must
 * both be defined as 1 for this function to be available.  See the
 * configuration section of the FreeRTOS.org website for more information.
 *
 * NOTE 1: This function will disable interrupts for its duration.  It is
 * not intended for normal application runtime use but as a debug aid.
 *
 * Lists all the current tasks, along with their current state and stack
 * usage high water mark.
 *
 * Tasks are reported as blocked ('B'), ready ('R'), deleted ('D') or
 * suspended ('S').
 *
 * PLEASE NOTE:
 *
 * This function is provided for convenience only, and is used by many of the
 * demo applications.  Do not consider it to be part of the scheduler.
 *
 * vTaskList() calls uxTaskGetSystemState(), then formats part of the
 * uxTaskGetSystemState() output into a human readable table that displays task
 * names, states and stack usage.
 *
 * vTaskList() has a dependency on the sprintf() C library function that might
 * bloat the code size, use a lot of stack, and provide different results on
 * different platforms.  An alternative, tiny, third party, and limited
 * functionality implementation of sprintf() is provided in many of the
 * FreeRTOS/Demo sub-directories in a file called printf-stdarg.c (note
 * printf-stdarg.c does not provide a full snprintf() implementation!).
 *
 * It is recommended that production systems call uxTaskGetSystemState()
 * directly to get access to raw stats data, rather than indirectly through a
 * call to vTaskList().
 *
 * @param pcWriteBuffer A buffer into which the above mentioned details
 * will be written, in ASCII form.  This buffer is assumed to be large
 * enough to contain the generated report.  Approximately 40 bytes per
 * task should be sufficient.
 *
 * \defgroup vTaskList vTaskList
 * \ingroup TaskUtils
 */
void vTaskList( char * pcWriteBuffer ) ; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

/**
 * task. h
 * <PRE>void vTaskGetRunTimeStats( char *pcWriteBuffer );</PRE>
 *
 * configGENERATE_RUN_TIME_STATS and configUSE_STATS_FORMATTING_FUNCTIONS
 * must both be defined as 1 for this function to be available.  The application
 * must also then provide definitions for
 * portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() and portGET_RUN_TIME_COUNTER_VALUE()
 * to configure a peripheral timer/counter and return the timers current count
 * value respectively.  The counter should be at least 10 times the frequency of
 * the tick count.
 *
 * NOTE 1: This function will disable interrupts for its duration.  It is
 * not intended for normal application runtime use but as a debug aid.
 *
 * Setting configGENERATE_RUN_TIME_STATS to 1 will result in a total
 * accumulated execution time being stored for each task.  The resolution
 * of the accumulated time value depends on the frequency of the timer
 * configured by the portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() macro.
 * Calling vTaskGetRunTimeStats() writes the total execution time of each
 * task into a buffer, both as an absolute count value and as a percentage
 * of the total system execution time.
 *
 * NOTE 2:
 *
 * This function is provided for convenience only, and is used by many of the
 * demo applications.  Do not consider it to be part of the scheduler.
 *
 * vTaskGetRunTimeStats() calls uxTaskGetSystemState(), then formats part of the
 * uxTaskGetSystemState() output into a human readable table that displays the
 * amount of time each task has spent in the Running state in both absolute and
 * percentage terms.
 *
 * vTaskGetRunTimeStats() has a dependency on the sprintf() C library function
 * that might bloat the code size, use a lot of stack, and provide different
 * results on different platforms.  An alternative, tiny, third party, and
 * limited functionality implementation of sprintf() is provided in many of the
 * FreeRTOS/Demo sub-directories in a file called printf-stdarg.c (note
 * printf-stdarg.c does not provide a full snprintf() implementation!).
 *
 * It is recommended that production systems call uxTaskGetSystemState() directly
 * to get access to raw stats data, rather than indirectly through a call to
 * vTaskGetRunTimeStats().
 *
 * @param pcWriteBuffer A buffer into which the execution times will be
 * written, in ASCII form.  This buffer is assumed to be large enough to
 * contain the generated report.  Approximately 40 bytes per task should
 * be sufficient.
 *
 * \defgroup vTaskGetRunTimeStats vTaskGetRunTimeStats
 * \ingroup TaskUtils
 */
void vTaskGetRunTimeStats( char *pcWriteBuffer ) ; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

/**
 * task. h
 * <PRE>int32_t xTaskNotify( task_t xTaskToNotify, uint32_t ulValue, notify_action_e_t eAction );</PRE>
 *
 * configUSE_TASK_NOTIFICATIONS must be undefined or defined as 1 for this
 * function to be available.
 *
 * When configUSE_TASK_NOTIFICATIONS is set to one each task has its own private
 * "notification value", which is a 32-bit unsigned integer (uint32_t).
 *
 * Events can be sent to a task using an intermediary object.  Examples of such
 * objects are queues, semaphores, mutexes and event groups.  Task notifications
 * are a method of sending an event directly to a task without the need for such
 * an intermediary object.
 *
 * A notification sent to a task can optionally perform an action, such as
 * update, overwrite or increment the task's notification value.  In that way
 * task notifications can be used to send data to a task, or be used as light
 * weight and fast binary or counting semaphores.
 *
 * A notification sent to a task will remain pending until it is cleared by the
 * task calling task_notify_wait() or task_notify_take().  If the task was
 * already in the Blocked state to wait for a notification when the notification
 * arrives then the task will automatically be removed from the Blocked state
 * (unblocked) and the notification cleared.
 *
 * A task can use task_notify_wait() to [optionally] block to wait for a
 * notification to be pending, or task_notify_take() to [optionally] block
 * to wait for its notification value to have a non-zero value.  The task does
 * not consume any CPU time while it is in the Blocked state.
 *
 * See http://www.FreeRTOS.org/RTOS-task-notifications.html for details.
 *
 * @param xTaskToNotify The handle of the task being notified.  The handle to a
 * task can be returned from the task_create() API function used to create the
 * task, and the handle of the currently running task can be obtained by calling
 * task_get_current().
 *
 * @param ulValue Data that can be sent with the notification.  How the data is
 * used depends on the value of the eAction parameter.
 *
 * @param eAction Specifies how the notification updates the task's notification
 * value, if at all.  Valid values for eAction are as follows:
 *
 * E_NOTIFY_ACTION_BITS -
 * The task's notification value is bitwise ORed with ulValue.  xTaskNofify()
 * always returns pdPASS in this case.
 *
 * E_NOTIFY_ACTION_INCR -
 * The task's notification value is incremented.  ulValue is not used and
 * xTaskNotify() always returns pdPASS in this case.
 *
 * E_NOTIFY_ACTION_OWRITE -
 * The task's notification value is set to the value of ulValue, even if the
 * task being notified had not yet processed the previous notification (the
 * task already had a notification pending).  xTaskNotify() always returns
 * pdPASS in this case.
 *
 * E_NOTIFY_ACTION_NO_OWRITE -
 * If the task being notified did not already have a notification pending then
 * the task's notification value is set to ulValue and xTaskNotify() will
 * return pdPASS.  If the task being notified already had a notification
 * pending then no action is performed and pdFAIL is returned.
 *
 * E_NOTIFY_ACTION_NONE -
 * The task receives a notification without its notification value being
 * updated.  ulValue is not used and xTaskNotify() always returns pdPASS in
 * this case.
 *
 *  pulPreviousNotificationValue -
 *  Can be used to pass out the subject task's notification value before any
 *  bits are modified by the notify function.
 *
 * @return Dependent on the value of eAction.  See the description of the
 * eAction parameter.
 *
 * \defgroup xTaskNotify xTaskNotify
 * \ingroup TaskNotifications
 */
int32_t task_notify_ext( task_t xTaskToNotify, uint32_t ulValue, notify_action_e_t eAction, uint32_t *pulPreviousNotificationValue ) ;
#define xTaskNotify( xTaskToNotify, ulValue, eAction ) task_notify_ext( ( xTaskToNotify ), ( ulValue ), ( eAction ), NULL )
#define xTaskNotifyAndQuery( xTaskToNotify, ulValue, eAction, pulPreviousNotifyValue ) task_notify_ext( ( xTaskToNotify ), ( ulValue ), ( eAction ), ( pulPreviousNotifyValue ) )

/**
 * task. h
 * <PRE>int32_t xTaskNotifyFromISR( task_t xTaskToNotify, uint32_t ulValue, notify_action_e_t eAction, int32_t *pxHigherPriorityTaskWoken );</PRE>
 *
 * configUSE_TASK_NOTIFICATIONS must be undefined or defined as 1 for this
 * function to be available.
 *
 * When configUSE_TASK_NOTIFICATIONS is set to one each task has its own private
 * "notification value", which is a 32-bit unsigned integer (uint32_t).
 *
 * A version of xTaskNotify() that can be used from an interrupt service routine
 * (ISR).
 *
 * Events can be sent to a task using an intermediary object.  Examples of such
 * objects are queues, semaphores, mutexes and event groups.  Task notifications
 * are a method of sending an event directly to a task without the need for such
 * an intermediary object.
 *
 * A notification sent to a task can optionally perform an action, such as
 * update, overwrite or increment the task's notification value.  In that way
 * task notifications can be used to send data to a task, or be used as light
 * weight and fast binary or counting semaphores.
 *
 * A notification sent to a task will remain pending until it is cleared by the
 * task calling task_notify_wait() or task_notify_take().  If the task was
 * already in the Blocked state to wait for a notification when the notification
 * arrives then the task will automatically be removed from the Blocked state
 * (unblocked) and the notification cleared.
 *
 * A task can use task_notify_wait() to [optionally] block to wait for a
 * notification to be pending, or task_notify_take() to [optionally] block
 * to wait for its notification value to have a non-zero value.  The task does
 * not consume any CPU time while it is in the Blocked state.
 *
 * See http://www.FreeRTOS.org/RTOS-task-notifications.html for details.
 *
 * @param xTaskToNotify The handle of the task being notified.  The handle to a
 * task can be returned from the task_create() API function used to create the
 * task, and the handle of the currently running task can be obtained by calling
 * task_get_current().
 *
 * @param ulValue Data that can be sent with the notification.  How the data is
 * used depends on the value of the eAction parameter.
 *
 * @param eAction Specifies how the notification updates the task's notification
 * value, if at all.  Valid values for eAction are as follows:
 *
 * E_NOTIFY_ACTION_BITS -
 * The task's notification value is bitwise ORed with ulValue.  xTaskNofify()
 * always returns pdPASS in this case.
 *
 * E_NOTIFY_ACTION_INCR -
 * The task's notification value is incremented.  ulValue is not used and
 * xTaskNotify() always returns pdPASS in this case.
 *
 * E_NOTIFY_ACTION_OWRITE -
 * The task's notification value is set to the value of ulValue, even if the
 * task being notified had not yet processed the previous notification (the
 * task already had a notification pending).  xTaskNotify() always returns
 * pdPASS in this case.
 *
 * E_NOTIFY_ACTION_NO_OWRITE -
 * If the task being notified did not already have a notification pending then
 * the task's notification value is set to ulValue and xTaskNotify() will
 * return pdPASS.  If the task being notified already had a notification
 * pending then no action is performed and pdFAIL is returned.
 *
 * E_NOTIFY_ACTION_NONE -
 * The task receives a notification without its notification value being
 * updated.  ulValue is not used and xTaskNotify() always returns pdPASS in
 * this case.
 *
 * @param pxHigherPriorityTaskWoken  xTaskNotifyFromISR() will set
 * *pxHigherPriorityTaskWoken to pdTRUE if sending the notification caused the
 * task to which the notification was sent to leave the Blocked state, and the
 * unblocked task has a priority higher than the currently running task.  If
 * xTaskNotifyFromISR() sets this value to pdTRUE then a context switch should
 * be requested before the interrupt is exited.  How a context switch is
 * requested from an ISR is dependent on the port - see the documentation page
 * for the port in use.
 *
 * @return Dependent on the value of eAction.  See the description of the
 * eAction parameter.
 *
 * \defgroup xTaskNotify xTaskNotify
 * \ingroup TaskNotifications
 */
int32_t xTaskGenericNotifyFromISR( task_t xTaskToNotify, uint32_t ulValue, notify_action_e_t eAction, uint32_t *pulPreviousNotificationValue, int32_t *pxHigherPriorityTaskWoken ) ;
#define xTaskNotifyFromISR( xTaskToNotify, ulValue, eAction, pxHigherPriorityTaskWoken ) xTaskGenericNotifyFromISR( ( xTaskToNotify ), ( ulValue ), ( eAction ), NULL, ( pxHigherPriorityTaskWoken ) )
#define xTaskNotifyAndQueryFromISR( xTaskToNotify, ulValue, eAction, pulPreviousNotificationValue, pxHigherPriorityTaskWoken ) xTaskGenericNotifyFromISR( ( xTaskToNotify ), ( ulValue ), ( eAction ), ( pulPreviousNotificationValue ), ( pxHigherPriorityTaskWoken ) )

/**
 * task. h
 * <PRE>int32_t task_notify_wait( uint32_t ulBitsToClearOnEntry, uint32_t ulBitsToClearOnExit, uint32_t *pulNotificationValue, uint32_t xTicksToWait );</pre>
 *
 * configUSE_TASK_NOTIFICATIONS must be undefined or defined as 1 for this
 * function to be available.
 *
 * When configUSE_TASK_NOTIFICATIONS is set to one each task has its own private
 * "notification value", which is a 32-bit unsigned integer (uint32_t).
 *
 * Events can be sent to a task using an intermediary object.  Examples of such
 * objects are queues, semaphores, mutexes and event groups.  Task notifications
 * are a method of sending an event directly to a task without the need for such
 * an intermediary object.
 *
 * A notification sent to a task can optionally perform an action, such as
 * update, overwrite or increment the task's notification value.  In that way
 * task notifications can be used to send data to a task, or be used as light
 * weight and fast binary or counting semaphores.
 *
 * A notification sent to a task will remain pending until it is cleared by the
 * task calling task_notify_wait() or task_notify_take().  If the task was
 * already in the Blocked state to wait for a notification when the notification
 * arrives then the task will automatically be removed from the Blocked state
 * (unblocked) and the notification cleared.
 *
 * A task can use task_notify_wait() to [optionally] block to wait for a
 * notification to be pending, or task_notify_take() to [optionally] block
 * to wait for its notification value to have a non-zero value.  The task does
 * not consume any CPU time while it is in the Blocked state.
 *
 * See http://www.FreeRTOS.org/RTOS-task-notifications.html for details.
 *
 * @param ulBitsToClearOnEntry Bits that are set in ulBitsToClearOnEntry value
 * will be cleared in the calling task's notification value before the task
 * checks to see if any notifications are pending, and optionally blocks if no
 * notifications are pending.  Setting ulBitsToClearOnEntry to ULONG_MAX (if
 * limits.h is included) or 0xffffffffUL (if limits.h is not included) will have
 * the effect of resetting the task's notification value to 0.  Setting
 * ulBitsToClearOnEntry to 0 will leave the task's notification value unchanged.
 *
 * @param ulBitsToClearOnExit If a notification is pending or received before
 * the calling task exits the task_notify_wait() function then the task's
 * notification value (see the xTaskNotify() API function) is passed out using
 * the pulNotificationValue parameter.  Then any bits that are set in
 * ulBitsToClearOnExit will be cleared in the task's notification value (note
 * *pulNotificationValue is set before any bits are cleared).  Setting
 * ulBitsToClearOnExit to ULONG_MAX (if limits.h is included) or 0xffffffffUL
 * (if limits.h is not included) will have the effect of resetting the task's
 * notification value to 0 before the function exits.  Setting
 * ulBitsToClearOnExit to 0 will leave the task's notification value unchanged
 * when the function exits (in which case the value passed out in
 * pulNotificationValue will match the task's notification value).
 *
 * @param pulNotificationValue Used to pass the task's notification value out
 * of the function.  Note the value passed out will not be effected by the
 * clearing of any bits caused by ulBitsToClearOnExit being non-zero.
 *
 * @param xTicksToWait The maximum amount of time that the task should wait in
 * the Blocked state for a notification to be received, should a notification
 * not already be pending when task_notify_wait() was called.  The task
 * will not consume any processing time while it is in the Blocked state.  This
 * is specified in kernel ticks, the macro pdMS_TO_TICSK( value_in_ms ) can be
 * used to convert a time specified in milliseconds to a time specified in
 * ticks.
 *
 * @return If a notification was received (including notifications that were
 * already pending when task_notify_wait was called) then pdPASS is
 * returned.  Otherwise pdFAIL is returned.
 *
 * \defgroup task_notify_wait task_notify_wait
 * \ingroup TaskNotifications
 */
int32_t task_notify_wait( uint32_t ulBitsToClearOnEntry, uint32_t ulBitsToClearOnExit, uint32_t *pulNotificationValue, uint32_t xTicksToWait ) ;

/**
 * task. h
 * <PRE>int32_t task_notify( task_t xTaskToNotify );</PRE>
 *
 * configUSE_TASK_NOTIFICATIONS must be undefined or defined as 1 for this macro
 * to be available.
 *
 * When configUSE_TASK_NOTIFICATIONS is set to one each task has its own private
 * "notification value", which is a 32-bit unsigned integer (uint32_t).
 *
 * Events can be sent to a task using an intermediary object.  Examples of such
 * objects are queues, semaphores, mutexes and event groups.  Task notifications
 * are a method of sending an event directly to a task without the need for such
 * an intermediary object.
 *
 * A notification sent to a task can optionally perform an action, such as
 * update, overwrite or increment the task's notification value.  In that way
 * task notifications can be used to send data to a task, or be used as light
 * weight and fast binary or counting semaphores.
 *
 * xTaskNotifyGive() is a helper macro intended for use when task notifications
 * are used as light weight and faster binary or counting semaphore equivalents.
 * Actual FreeRTOS semaphores are given using the sem_post() API function,
 * the equivalent action that instead uses a task notification is
 * xTaskNotifyGive().
 *
 * When task notifications are being used as a binary or counting semaphore
 * equivalent then the task being notified should wait for the notification
 * using the ulTaskNotificationTake() API function rather than the
 * task_notify_wait() API function.
 *
 * See http://www.FreeRTOS.org/RTOS-task-notifications.html for more details.
 *
 * @param xTaskToNotify The handle of the task being notified.  The handle to a
 * task can be returned from the task_create() API function used to create the
 * task, and the handle of the currently running task can be obtained by calling
 * task_get_current().
 *
 * @return xTaskNotifyGive() is a macro that calls xTaskNotify() with the
 * eAction parameter set to E_NOTIFY_ACTION_INCR - so pdPASS is always returned.
 *
 * \defgroup xTaskNotifyGive xTaskNotifyGive
 * \ingroup TaskNotifications
 */
int32_t task_notify(task_t xTaskToNotify);

/**
 * task. h
 * <PRE>void vTaskNotifyGiveFromISR( task_t xTaskHandle, int32_t *pxHigherPriorityTaskWoken );
 *
 * configUSE_TASK_NOTIFICATIONS must be undefined or defined as 1 for this macro
 * to be available.
 *
 * When configUSE_TASK_NOTIFICATIONS is set to one each task has its own private
 * "notification value", which is a 32-bit unsigned integer (uint32_t).
 *
 * A version of xTaskNotifyGive() that can be called from an interrupt service
 * routine (ISR).
 *
 * Events can be sent to a task using an intermediary object.  Examples of such
 * objects are queues, semaphores, mutexes and event groups.  Task notifications
 * are a method of sending an event directly to a task without the need for such
 * an intermediary object.
 *
 * A notification sent to a task can optionally perform an action, such as
 * update, overwrite or increment the task's notification value.  In that way
 * task notifications can be used to send data to a task, or be used as light
 * weight and fast binary or counting semaphores.
 *
 * vTaskNotifyGiveFromISR() is intended for use when task notifications are
 * used as light weight and faster binary or counting semaphore equivalents.
 * Actual FreeRTOS semaphores are given from an ISR using the
 * xSemaphoreGiveFromISR() API function, the equivalent action that instead uses
 * a task notification is vTaskNotifyGiveFromISR().
 *
 * When task notifications are being used as a binary or counting semaphore
 * equivalent then the task being notified should wait for the notification
 * using the ulTaskNotificationTake() API function rather than the
 * task_notify_wait() API function.
 *
 * See http://www.FreeRTOS.org/RTOS-task-notifications.html for more details.
 *
 * @param xTaskToNotify The handle of the task being notified.  The handle to a
 * task can be returned from the task_create() API function used to create the
 * task, and the handle of the currently running task can be obtained by calling
 * task_get_current().
 *
 * @param pxHigherPriorityTaskWoken  vTaskNotifyGiveFromISR() will set
 * *pxHigherPriorityTaskWoken to pdTRUE if sending the notification caused the
 * task to which the notification was sent to leave the Blocked state, and the
 * unblocked task has a priority higher than the currently running task.  If
 * vTaskNotifyGiveFromISR() sets this value to pdTRUE then a context switch
 * should be requested before the interrupt is exited.  How a context switch is
 * requested from an ISR is dependent on the port - see the documentation page
 * for the port in use.
 *
 * \defgroup task_notify_wait task_notify_wait
 * \ingroup TaskNotifications
 */
void vTaskNotifyGiveFromISR( task_t xTaskToNotify, int32_t *pxHigherPriorityTaskWoken ) ;

/**
 * task. h
 * <PRE>uint32_t task_notify_take( int32_t xClearCountOnExit, uint32_t xTicksToWait );</pre>
 *
 * configUSE_TASK_NOTIFICATIONS must be undefined or defined as 1 for this
 * function to be available.
 *
 * When configUSE_TASK_NOTIFICATIONS is set to one each task has its own private
 * "notification value", which is a 32-bit unsigned integer (uint32_t).
 *
 * Events can be sent to a task using an intermediary object.  Examples of such
 * objects are queues, semaphores, mutexes and event groups.  Task notifications
 * are a method of sending an event directly to a task without the need for such
 * an intermediary object.
 *
 * A notification sent to a task can optionally perform an action, such as
 * update, overwrite or increment the task's notification value.  In that way
 * task notifications can be used to send data to a task, or be used as light
 * weight and fast binary or counting semaphores.
 *
 * task_notify_take() is intended for use when a task notification is used as a
 * faster and lighter weight binary or counting semaphore alternative.  Actual
 * FreeRTOS semaphores are taken using the sem_wait() API function, the
 * equivalent action that instead uses a task notification is
 * task_notify_take().
 *
 * When a task is using its notification value as a binary or counting semaphore
 * other tasks should send notifications to it using the xTaskNotifyGive()
 * macro, or xTaskNotify() function with the eAction parameter set to
 * E_NOTIFY_ACTION_INCR.
 *
 * task_notify_take() can either clear the task's notification value to
 * zero on exit, in which case the notification value acts like a binary
 * semaphore, or decrement the task's notification value on exit, in which case
 * the notification value acts like a counting semaphore.
 *
 * A task can use task_notify_take() to [optionally] block to wait for a
 * the task's notification value to be non-zero.  The task does not consume any
 * CPU time while it is in the Blocked state.
 *
 * Where as task_notify_wait() will return when a notification is pending,
 * task_notify_take() will return when the task's notification value is
 * not zero.
 *
 * See http://www.FreeRTOS.org/RTOS-task-notifications.html for details.
 *
 * @param xClearCountOnExit if xClearCountOnExit is pdFALSE then the task's
 * notification value is decremented when the function exits.  In this way the
 * notification value acts like a counting semaphore.  If xClearCountOnExit is
 * not pdFALSE then the task's notification value is cleared to zero when the
 * function exits.  In this way the notification value acts like a binary
 * semaphore.
 *
 * @param xTicksToWait The maximum amount of time that the task should wait in
 * the Blocked state for the task's notification value to be greater than zero,
 * should the count not already be greater than zero when
 * task_notify_take() was called.  The task will not consume any processing
 * time while it is in the Blocked state.  This is specified in kernel ticks,
 * the macro pdMS_TO_TICSK( value_in_ms ) can be used to convert a time
 * specified in milliseconds to a time specified in ticks.
 *
 * @return The task's notification count before it is either cleared to zero or
 * decremented (see the xClearCountOnExit parameter).
 *
 * \defgroup task_notify_take task_notify_take
 * \ingroup TaskNotifications
 */
uint32_t task_notify_take( bool xClearCountOnExit, uint32_t xTicksToWait ) ;

/**
 * task. h
 * <PRE>int32_t task_notify_clear( task_t xTask );</pre>
 *
 * If the notification state of the task referenced by the handle xTask is
 * eNotified, then set the task's notification state to eNotWaitingNotification.
 * The task's notification value is not altered.  Set xTask to NULL to clear the
 * notification state of the calling task.
 *
 * @return pdTRUE if the task's notification state was set to
 * eNotWaitingNotification, otherwise pdFALSE.
 * \defgroup task_notify_clear task_notify_clear
 * \ingroup TaskNotifications
 */
int32_t task_notify_clear( task_t xTask );

/*-----------------------------------------------------------
 * SCHEDULER INTERNALS AVAILABLE FOR PORTING PURPOSES
 *----------------------------------------------------------*/

/*
 * THIS FUNCTION MUST NOT BE USED FROM APPLICATION CODE.  IT IS ONLY
 * INTENDED FOR USE WHEN IMPLEMENTING A PORT OF THE SCHEDULER AND IS
 * AN INTERFACE WHICH IS FOR THE EXCLUSIVE USE OF THE SCHEDULER.
 *
 * Called from the real time kernel tick (either preemptive or cooperative),
 * this increments the tick count and checks if any tasks that are blocked
 * for a finite period required removing from a blocked list and placing on
 * a ready list.  If a non-zero value is returned then a context switch is
 * required because either:
 *   + A task was removed from a blocked list because its timeout had expired,
 *     or
 *   + Time slicing is in use and there is a task of equal priority to the
 *     currently running task.
 */
int32_t xTaskIncrementTick( void ) ;

/*
 * THIS FUNCTION MUST NOT BE USED FROM APPLICATION CODE.  IT IS AN
 * INTERFACE WHICH IS FOR THE EXCLUSIVE USE OF THE SCHEDULER.
 *
 * THIS FUNCTION MUST BE CALLED WITH INTERRUPTS DISABLED.
 *
 * Removes the calling task from the ready list and places it both
 * on the list of tasks waiting for a particular event, and the
 * list of delayed tasks.  The task will be removed from both lists
 * and replaced on the ready list should either the event occur (and
 * there be no higher priority tasks waiting on the same event) or
 * the delay period expires.
 *
 * The 'unordered' version replaces the event list item value with the
 * xItemValue value, and inserts the list item at the end of the list.
 *
 * The 'ordered' version uses the existing event list item value (which is the
 * owning tasks priority) to insert the list item into the event list is task
 * priority order.
 *
 * @param pxEventList The list containing tasks that are blocked waiting
 * for the event to occur.
 *
 * @param xItemValue The item value to use for the event list item when the
 * event list is not ordered by task priority.
 *
 * @param xTicksToWait The maximum amount of time that the task should wait
 * for the event to occur.  This is specified in kernel ticks,the constant
 * portTICK_PERIOD_MS can be used to convert kernel ticks into a real time
 * period.
 */
void vTaskPlaceOnEventList( List_t * const pxEventList, const uint32_t xTicksToWait ) ;
void vTaskPlaceOnUnorderedEventList( List_t * pxEventList, const uint32_t xItemValue, const uint32_t xTicksToWait ) ;

/*
 * THIS FUNCTION MUST NOT BE USED FROM APPLICATION CODE.  IT IS AN
 * INTERFACE WHICH IS FOR THE EXCLUSIVE USE OF THE SCHEDULER.
 *
 * THIS FUNCTION MUST BE CALLED WITH INTERRUPTS DISABLED.
 *
 * This function performs nearly the same function as vTaskPlaceOnEventList().
 * The difference being that this function does not permit tasks to block
 * indefinitely, whereas vTaskPlaceOnEventList() does.
 *
 */
void vTaskPlaceOnEventListRestricted( List_t * const pxEventList, uint32_t xTicksToWait, const int32_t xWaitIndefinitely ) ;

/*
 * THIS FUNCTION MUST NOT BE USED FROM APPLICATION CODE.  IT IS AN
 * INTERFACE WHICH IS FOR THE EXCLUSIVE USE OF THE SCHEDULER.
 *
 * THIS FUNCTION MUST BE CALLED WITH INTERRUPTS DISABLED.
 *
 * Removes a task from both the specified event list and the list of blocked
 * tasks, and places it on a ready queue.
 *
 * xTaskRemoveFromEventList()/vTaskRemoveFromUnorderedEventList() will be called
 * if either an event occurs to unblock a task, or the block timeout period
 * expires.
 *
 * xTaskRemoveFromEventList() is used when the event list is in task priority
 * order.  It removes the list item from the head of the event list as that will
 * have the highest priority owning task of all the tasks on the event list.
 * vTaskRemoveFromUnorderedEventList() is used when the event list is not
 * ordered and the event list items hold something other than the owning tasks
 * priority.  In this case the event list item value is updated to the value
 * passed in the xItemValue parameter.
 *
 * @return pdTRUE if the task being removed has a higher priority than the task
 * making the call, otherwise pdFALSE.
 */
int32_t xTaskRemoveFromEventList( const List_t * const pxEventList ) ;
void vTaskRemoveFromUnorderedEventList( list_item_t * pxEventListItem, const uint32_t xItemValue ) ;

/*
 * THIS FUNCTION MUST NOT BE USED FROM APPLICATION CODE.  IT IS ONLY
 * INTENDED FOR USE WHEN IMPLEMENTING A PORT OF THE SCHEDULER AND IS
 * AN INTERFACE WHICH IS FOR THE EXCLUSIVE USE OF THE SCHEDULER.
 *
 * Sets the pointer to the current TCB to the TCB of the highest priority task
 * that is ready to run.
 */
void vTaskSwitchContext( void ) ;

/*
 * THESE FUNCTIONS MUST NOT BE USED FROM APPLICATION CODE.  THEY ARE USED BY
 * THE EVENT BITS MODULE.
 */
uint32_t uxTaskResetEventItemValue( void ) ;

/*
 * Return the handle of the calling task.
 */
task_t task_get_current( void ) ;

/*
 * Capture the current time status for future reference.
 */
void vTaskSetTimeOutState( TimeOut_t * const pxTimeOut ) ;

/*
 * Compare the time status now with that previously captured to see if the
 * timeout has expired.
 */
int32_t xTaskCheckForTimeOut( TimeOut_t * const pxTimeOut, uint32_t * const pxTicksToWait ) ;

/*
 * Shortcut used by the queue implementation to prevent unnecessary call to
 * taskYIELD();
 */
void vTaskMissedYield( void ) ;

/*
 * Returns the scheduler state as taskSCHEDULER_RUNNING,
 * taskSCHEDULER_NOT_STARTED or taskSCHEDULER_SUSPENDED.
 */
int32_t xTaskGetSchedulerState( void ) ;

/*
 * Raises the priority of the mutex holder to that of the calling task should
 * the mutex holder have a priority less than the calling task.
 */
int32_t xTaskPriorityInherit( task_t const pxMutexHolder ) ;

/*
 * Set the priority of a task back to its proper priority in the case that it
 * inherited a higher priority while it was holding a semaphore.
 */
int32_t xTaskPriorityDisinherit( task_t const pxMutexHolder ) ;

/*
 * If a higher priority task attempting to obtain a mutex caused a lower
 * priority task to inherit the higher priority task's priority - but the higher
 * priority task then timed out without obtaining the mutex, then the lower
 * priority task will disinherit the priority again - but only down as far as
 * the highest priority task that is still waiting for the mutex (if there were
 * more than one task waiting for the mutex).
 */
void vTaskPriorityDisinheritAfterTimeout( task_t const pxMutexHolder, uint32_t uxHighestPriorityWaitingTask ) ;

/*
 * Get the uxTCBNumber assigned to the task referenced by the xTask parameter.
 */
uint32_t uxTaskGetTaskNumber( task_t xTask ) ;

/*
 * Set the uxTaskNumber of the task referenced by the xTask parameter to
 * uxHandle.
 */
void vTaskSetTaskNumber( task_t xTask, const uint32_t uxHandle ) ;

/*
 * Only available when configUSE_TICKLESS_IDLE is set to 1.
 * If tickless mode is being used, or a low power mode is implemented, then
 * the tick interrupt will not execute during idle periods.  When this is the
 * case, the tick count value maintained by the scheduler needs to be kept up
 * to date with the actual execution time by being skipped forward by a time
 * equal to the idle period.
 */
void vTaskStepTick( const uint32_t xTicksToJump ) ;

/*
 * Only avilable when configUSE_TICKLESS_IDLE is set to 1.
 * Provided for use within portSUPPRESS_TICKS_AND_SLEEP() to allow the port
 * specific sleep function to determine if it is ok to proceed with the sleep,
 * and if it is ok to proceed, if it is ok to sleep indefinitely.
 *
 * This function is necessary because portSUPPRESS_TICKS_AND_SLEEP() is only
 * called with the scheduler suspended, not from within a critical section.  It
 * is therefore possible for an interrupt to request a context switch between
 * portSUPPRESS_TICKS_AND_SLEEP() and the low power mode actually being
 * entered.  eTaskConfirmSleepModeStatus() should be called from a short
 * critical section between the timer being stopped and the sleep mode being
 * entered to ensure it is ok to proceed into the sleep mode.
 */
eSleepModeStatus eTaskConfirmSleepModeStatus( void ) ;

/*
 * For internal use only.  Increment the mutex held count when a mutex is
 * taken and return the handle of the task that has taken the mutex.
 */
void *pvTaskIncrementMutexHeldCount( void ) ;

/*
 * For internal use only.  Same as vTaskSetTimeOutState(), but without a critial
 * section.
 */
void vTaskInternalSetTimeOutState( TimeOut_t * const pxTimeOut ) ;


#ifdef __cplusplus
}
#endif
#endif /* INC_TASK_H */
