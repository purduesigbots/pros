/**
 * unwind.c - Unwind functions specialized for PROS
 *
 * Unwinding is necessary in PROS because tasks containing C++ stack frames may
 * be arbitrarily stopped, requiring us to call all the destructors of the task
 * to be killed.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <unwind.h>
#include <malloc.h>

#include "rtos/task.h"
#include "rtos/tcb.h"

#include "unwind-arm-common.h"

typedef struct __EIT_entry
{
  _uw fnoffset;
  _uw content;
} __EIT_entry;

extern __EIT_entry __exidx_start;
extern __EIT_entry __exidx_end;

// _Unwind_Ptr __gnu_Unwind_Find_exidx (_Unwind_Ptr pc, int * nrec) {
// 	// TODO: support hot/cold here
// 	printf("\t0x%08lx\n", pc);
// 	*nrec = &__exidx_end - &__exidx_start;
// 	return &__exidx_start;
// }

extern void task_clean_up();

_Unwind_Reason_Code trace_fn(_Unwind_Context* unwind_ctx, void* d) {
	uint32_t pc = _Unwind_GetIP(unwind_ctx);
	printf("\t0x%08lx\n", pc);
	if (pc == (uint32_t)task_clean_up) {
		return _URC_FAILURE;
	}
	return _URC_NO_REASON;
}

#define VRS_PC(vrs) ((vrs)->core.r[R_PC])
#define VRS_SP(vrs) ((vrs)->core.r[R_SP])
#define VRS_RETURN(vrs) ((vrs)->core.r[R_LR])

struct core_regs
{
  _uw r[16];
};

/* This must match the structure created by the assembly wrappers.  */
typedef struct
{
  _uw demand_save_flags;
  struct core_regs core;
} phase2_vrs;


static inline void print_phase2_vrs(phase2_vrs* vrs) {
	static const char registers[16][4] = {
		"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "sp", "lr", "pc"
	};
	for(size_t i = 0; i < 16; i++) {
		printf("%3s: 0x%08x ", registers[i], vrs->core.r[i]);
    if(i % 8 == 7) printf("\n");
	}
  printf("\n");
}

// recover registers from a data abort. Specific knowledge about callee stack
// used from FreeRTOS_DataAbortHandler and it calling DataAbortInterrupt
void p2vrs_from_data_abort(_uw* sp, phase2_vrs* vrs) {
  // sp is stack pointer when FreeRTOS_DataAbortHandler invokes DataAbortInterrupt
	vrs->demand_save_flags = 0;
  // start pulling these registers off the stack
  // see xilinx_vectors.s:114 stmdb	sp!,{r0-r3,r12,lr}
  vrs->core.r[0] = sp[0];
  vrs->core.r[1] = sp[1];
  vrs->core.r[2] = sp[2];
  vrs->core.r[3] = sp[3];
  vrs->core.r[4] = sp[-1]; // DataAbortInterrupt pushes this onto stack, so grab it back
  asm("stm %0!, {r5-r11}\n" : : "r"(vrs->core.r + 5)); // r5-r11 were never touched
  vrs->core.r[12] = sp[4];
  // sp/lr are in (banked) user registers. recover them from there. ref B9.3.17 of AARM for v7-a/r
  asm("stm %0, {r13,r14}^" :  : "r"(vrs->core.r + 13));
  vrs->core.r[15] = sp[5] - 8;
}

void backtrace_from_data_abort(void* _sp) {
  phase2_vrs vrs;
  p2vrs_from_data_abort((_uw*)_sp, &vrs);
  puts("DATA ABORT EXCEPTION\n");
  puts("REGISTERS AT ABORT");
	print_phase2_vrs(&vrs);

  puts("BEGIN STACK TRACE");
  __gnu_Unwind_Backtrace(trace_fn, NULL, &vrs);
	puts("END OF TRACE");

  puts("HEAP USAGE");
	struct mallinfo info = mallinfo();
  printf("Used bytes: %d\tFree bytes: %d\n", info.uordblks, info.fordblks);
}

// /******************************************************************************/
// /**               Modified RTOS-Task Target Unwinder Functions               **/
// /**                                                                          **/
// /** These functions use the __gnu_Unwind_* functions providing our helper    **/
// /** functions and phase2_vrs structure based on a target task                **/
// /******************************************************************************/
#define REGISTER_BASE		67
static inline phase2_vrs p2vrs_from_task(task_t task) {
	// should be called with the task scheduler suspended
	taskENTER_CRITICAL();

	TCB_t* tcb = (TCB_t*)task;
	size_t i;

	phase2_vrs vrs;
	if(tcb == NULL) {
		tcb = pxCurrentTCB;
	}

	vrs.demand_save_flags = 0;
	switch(task_get_state(task)) {
		case E_TASK_STATE_READY:
			for(i = 0; i < 12; i++) {
				vrs.core.r[i] = tcb->pxTopOfStack[REGISTER_BASE + i];
			}
			vrs.core.r[13] = (_uw)(tcb->pxTopOfStack + REGISTER_BASE + 16); // r13 is sp
			vrs.core.r[14] = tcb->pxTopOfStack[REGISTER_BASE + 13]; // r14 is lr
			vrs.core.r[15] = tcb->pxTopOfStack[REGISTER_BASE + 14]; // r15 is pc
			break;
		case E_TASK_STATE_BLOCKED:
			// for(i = 0; i < 12; i++) {
			// 	vrs.core.r[i] = tcb->pxTopOfStack[REGISTER_BASE + i];
			// }
			// vrs.core.r[13] = (_uw)(tcb->pxTopOfStack + REGISTER_BASE + 29);
			// vrs.core.r[14] = tcb->pxTopOfStack[REGISTER_BASE + ]
			break;
		case E_TASK_STATE_RUNNING:
			break;
		case E_TASK_STATE_SUSPENDED:
			break;
		case E_TASK_STATE_DELETED: // will never happen
		case E_TASK_STATE_INVALID:
		default:
			break;
	}
	taskEXIT_CRITICAL();
	return vrs;
}

void backtrace_task(task_t task) {
	phase2_vrs vrs = p2vrs_from_task(task);
  printf("Trace:\n");
  _Unwind_Control_Block ucbp;
	__gnu_Unwind_Backtrace(trace_fn, NULL, &vrs);
	printf("finished trace\n");
}
