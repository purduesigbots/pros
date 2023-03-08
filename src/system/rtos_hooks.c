/**
 * \file system/rtos_hooks.c
 *
 * FreeRTOS hooks for initialization and interrupts
 *
 * FreeRTOS requires some porting to each platform to handle certain tasks. This
 * file contains the various methods required to be implemented for FreeRTOS.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "rtos/FreeRTOS.h"
#include "rtos/semphr.h"
#include "rtos/task.h"
#include "rtos/tcb.h"

#include "v5_api.h"
#include "v5_color.h"

// Fast interrupt handler
void FIQInterrupt() {
	vexSystemFIQInterrupt();
}
// Replacement for DataAbortInterrupt
void DataAbortInterrupt() {
	taskDISABLE_INTERRUPTS();

	register int sp;
	asm("add %0,sp,#8\n" : "=r"(sp));
	extern void report_data_abort(uint32_t);
	report_data_abort(sp);
	for (;;) {
		vexBackgroundProcessing();
		extern void ser_output_flush();
		ser_output_flush();
	}
}
// Replacement for PrefetchAbortInterrupt
void PrefetchAbortInterrupt() {
	vexSystemPrefetchAbortInterrupt();
}

void _boot() {
	vexSystemBoot();
}

extern void vPortInstallFreeRTOSVectorTable(void);
void rtos_initialize() {
	vexSystemTimerStop();

	portDISABLE_INTERRUPTS();

	vPortInstallFreeRTOSVectorTable();

	void task_notify_when_deleting_init();
	task_notify_when_deleting_init();
}

extern void FreeRTOS_Tick_Handler(void);
void rtos_tick_interrupt_config() {
	vexSystemTimerReinitForRtos(portLOWEST_USABLE_INTERRUPT_PRIORITY << portPRIORITY_SHIFT,
	                            (void (*)(void*))FreeRTOS_Tick_Handler);
}

void rtos_tick_interrupt_clear() {
	vexSystemTimerClearInterrupt();
}

void vApplicationFPUSafeIRQHandler(uint32_t ulICCIAR) {
	vexSystemApplicationIRQHandler(ulICCIAR);
}

void vInitialiseTimerForRunTimeStats(void) {
	vexSystemWatchdogReinitRtos();
}

void vApplicationMallocFailedHook(void) {
	// Called if a call to kmalloc() fails because there is insufficient free
	// memory available in the FreeRTOS heap.  kmalloc() is called internally by
	// FreeRTOS API functions that create tasks, queues, software timers, and
	// semaphores.  The size of the FreeRTOS heap is set by the
	// configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h.
	taskDISABLE_INTERRUPTS();

	for (;;)
		;
}

void vApplicationStackOverflowHook(task_t pxTask, char* pcTaskName) {
	(void)pcTaskName;
	(void)pxTask;

	vexSerialWriteBuffer(1, (uint8_t*)"FATAL ERROR!! Task ", 19);
	vexSerialWriteBuffer(1, (uint8_t*)pcTaskName, 30);
	vexSerialWriteBuffer(1, (uint8_t*)" overflowed its stack!\n", 23);

	// Run time stack overflow checking is performed if
	// configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook function is
	// called if a stack overflow is detected.
	taskDISABLE_INTERRUPTS();
	for (;;) vexBackgroundProcessing();
	;
}

void vApplicationIdleHook(void) {
	volatile size_t xFreeHeapSpace;

	// This is just a trivial example of an idle hook.  It is called on each cycle
	// of the idle task.  It must *NOT* attempt to block.  In this case the idle
	// task just queries the amount of FreeRTOS heap that remains.  See the memory
	// management section on the http://www.FreeRTOS.org web site for memory
	// management options.  If there is a lot of heap memory free then the
	// configTOTAL_HEAP_SIZE value in FreeRTOSConfig.h can be reduced to free up
	// RAM.
	xFreeHeapSpace = xPortGetFreeHeapSize();

	// Remove compiler warning about xFreeHeapSpace being set but never used.
	(void)xFreeHeapSpace;
}

void vAssertCalled(const char* pcFile, unsigned long ulLine) {
	volatile unsigned long ul = 0;

	(void)pcFile;
	(void)ulLine;

	taskENTER_CRITICAL();
	{
		// Set ul to a non-zero value using the debugger to step out of
		// this function.
		while (ul == 0) {
			portNOP();
		}
	}
	taskEXIT_CRITICAL();
}

// FreeRTOS V9 now allows static memory allocation for tasks
// the helper functions below are copied verbatim from demo code
#if (configSUPPORT_STATIC_ALLOCATION == 1)
// configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide
// an implementation of vApplicationGetIdleTaskMemory() to provide the memory
// that is used by the Idle task.
void vApplicationGetIdleTaskMemory(static_task_s_t** ppxIdleTaskTCBBuffer, task_stack_t** ppxIdleTaskStackBuffer,
                                   uint32_t* pulIdleTaskStackSize) {
	// If the buffers to be provided to the Idle task are declared inside this
	// function then they must be declared static - otherwise they will be
	// allocated on the stack and so not exists after this function exits.
	static static_task_s_t xIdleTaskTCB;
	static task_stack_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

	// Pass out a pointer to the static_task_s_t structure in which the Idle
	// task's state will be stored.
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

	// Pass out the array that will be used as the Idle task's stack.
	*ppxIdleTaskStackBuffer = uxIdleTaskStack;

	// Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
	// Note that, as the array is necessarily of type task_stack_t,
	// configMINIMAL_STACK_SIZE is specified in words, not bytes.
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

// configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so
// the application must provide an implementation of
// vApplicationGetTimerTaskMemory() to provide the memory that is used by the
// Timer service task.
void vApplicationGetTimerTaskMemory(static_task_s_t** ppxTimerTaskTCBBuffer, task_stack_t** ppxTimerTaskStackBuffer,
                                    uint32_t* pulTimerTaskStackSize) {
	// If the buffers to be provided to the Timer task are declared inside this
	// function then they must be declared static - otherwise they will be
	// allocated on the stack and so not exists after this function exits.
	static static_task_s_t xTimerTaskTCB;
	static task_stack_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

	// Pass out a pointer to the static_task_s_t structure in which the Timer
	// task's state will be stored.
	*ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

	// Pass out the array that will be used as the Timer task's stack.
	*ppxTimerTaskStackBuffer = uxTimerTaskStack;

	// Pass out the size of the array pointed to by
	// *ppxTimerTaskStackBuffer. Note that, as the array is necessarily of type
	// task_stack_t, configMINIMAL_STACK_SIZE is specified in words, not bytes.
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
#endif

/******************************************************************************/
/**                      Replacements for mem functions                      **/
/**                                                                          **/
/** See https://goo.gl/cHUA9b for more details                               **/
/** Unsure if necessary due to Arm's newlib optimization                     **/
/******************************************************************************/
#if 0
#if (configUSE_TASK_FPU_SUPPORT == 2)
void* memcpy(void* restrict out, void const* restrict in, size_t n) {
  // modified GNU algorithm to copy a word at a time
  uint32_t* restrict lout;
  uint32_t const* restrict lin;

  if (!((uint32_t)in & 0xFFFFFFFC) && !((uint32_t)out & 0xFFFFFFFC)) {
    while(n >= sizeof(unt32_t)) {
      *lout++ = *lin++;
      n -= sizeof(uint32_t);
    }
  }
}
void* memcpy(void *pvDest, void const *pvSource, size_t len) {
  long *plDst = (long *)pvDest;
  long const *plSrc = (long const *)pvSource;

  if (!((uint32_t)pvSource & 0xFFFFFFFC) && !((uint32_t)pvDest & 0xFFFFFFFC)) {
    while (len >= 4) {
      *plDst++ = *plSrc++;
      len -= 4;
    }
  }

  char *pcDst = (char *)plDst;
  char const *pcSrc = (char const *)plSrc;

  while (len--)
    *pcDst++ = *pcSrc++;

  return (pvDest);
}

/*-----------------------------------------------------------*/

void *memset(void *pvDest, int iValue, size_t ulBytes) {
  volatile unsigned char *pcDest = (unsigned char *)pvDest;

  if (ulBytes > 0) {
    do {
      *pcDest++ = (unsigned char)iValue;
    } while (--ulBytes != 0);
  }

  return pvDest;
}

/*-----------------------------------------------------------*/

int memcmp(const void *pvMem1, const void *pvMem2, size_t ulBytes) {
  const unsigned char *pucMem1 = pvMem1, *pucMem2 = pvMem2;
  size_t x;

  for (x = 0; x < ulBytes; x++) {
    if (pucMem1[x] != pucMem2[x]) {
      break;
    }
  }

  return ulBytes - x;
}
#endif
#endif
