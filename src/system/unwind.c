/**
 * unwind.c - Unwind functions specialized for PROS
 *
 * Unwinding is necessary in PROS because tasks containing C++ stack frames may
 * be arbitrarily stopped, requiring us to call all the destructors of the task
 * to be killed.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <malloc.h>
#include <stdio.h>
#include <unwind.h>

#include "unwind-arm-common.h"

#include "rtos/task.h"
#include "rtos/tcb.h"
#include "system/hot.h"

#include "v5_api.h"

/******************************************************************************/
/**                           Helpful Unwind Files                           **/
/*******************************************************************************
https://github.com/gcc-mirror/gcc/blob/master/libgcc/unwind-arm-common.inc
https://github.com/gcc-mirror/gcc/blob/master/libgcc/config/arm/unwind-arm.c
https://github.com/gcc-mirror/gcc/blob/master/libgcc/config/arm/unwind-arm.h
https://github.com/gcc-mirror/gcc/blob/master/gcc/ginclude/unwind-arm-common.h
http://infocenter.arm.com/help/topic/com.arm.doc.ihi0038b/IHI0038B_ehabi.pdf
*******************************************************************************/

/******************************************************************************/
/**                            Unwind Definitions                            **/
/**                                                                          **/
/** These structs and extern'd functions come from unwind-arm.c              **/
/******************************************************************************/
#define R_SP 12
#define R_LR 13
#define R_PC 15

struct core_regs {
	_uw r[16];
};

struct phase2_vrs {
	_uw demand_save_flags;
	struct core_regs core;
};

// from unwind-arm-common.inc
extern _Unwind_Reason_Code __gnu_Unwind_Backtrace(_Unwind_Trace_Fn, void*, struct phase2_vrs*);

/******************************************************************************/
/**                              Unwind Helpers                              **/
/**                                                                          **/
/** These functions and definitions are helpers for supporting PROS's usage  **/
/** of unwinding                                                             **/
/******************************************************************************/
static inline void print_phase2_vrs(struct phase2_vrs* vrs) {
	static const char registers[16][4] = {"r0", "r1", "r2",  "r3",  "r4",  "r5", "r6", "r7",
	                                      "r8", "r9", "r10", "r11", "r12", "sp", "lr", "pc"};
	for (size_t i = 0; i < 16; i++) {
		fprintf(stderr, "%3s: 0x%08x ", registers[i], vrs->core.r[i]);
		if (i % 8 == 7) printf("\n");
	}
	fputs("\n", stderr);
}

// exidx is the table that tells the unwinder how to unwind a stack frame
// for a PC. Under hot/cold, there's two tables and the unwinder was kind
// enough to let us implement a function to give it a table for a PC so
// support for hot/cold is as easy as it gets
struct __EIT_entry {
	_uw fnoffset;
	_uw content;
};
// these are all defined by the linker
extern struct __EIT_entry __exidx_start;
extern struct __EIT_entry __exidx_end;
extern uint8_t start_of_cold_mem, end_of_cold_mem, start_of_hot_mem, end_of_hot_mem;

_Unwind_Ptr __gnu_Unwind_Find_exidx(_Unwind_Ptr pc, int* nrec) {
	// check if pc is in the hot region
	if (HOT_TABLE && (void*)&start_of_hot_mem < (void*)pc && (void*)pc < (void*)&end_of_hot_mem) {
		*nrec = (struct __EIT_entry*)HOT_TABLE->__exidx_end - (struct __EIT_entry*)HOT_TABLE->__exidx_start;
		return (_Unwind_Ptr)HOT_TABLE->__exidx_start;
	}
	// otherwise, we're in a monolith build or the cold region of a hot/cold build
	*nrec = &__exidx_end - &__exidx_start;
	return (_Unwind_Ptr)&__exidx_start;
}

_Unwind_Reason_Code trace_fn(_Unwind_Context* unwind_ctx, void* d) {
	uint32_t pc = _Unwind_GetIP(unwind_ctx);
	fprintf(stderr, "\t%p\n", (void*)pc);
	extern void task_clean_up();
	if (pc == (uint32_t)task_clean_up) {
		return _URC_FAILURE;
	}
	return _URC_NO_REASON;
}

/******************************************************************************/
/**                            Data Abort Handler                            **/
/**                                                                          **/
/** These functions use the __gnu_Unwind_* functions providing our helper    **/
/** functions and phase2_vrs structure based on a target task                **/
/******************************************************************************/
// recover registers from a data abort. Specific knowledge about callee stack
// used from FreeRTOS_DataAbortHandler and it calling DataAbortInterrupt
void p2vrs_from_data_abort(_uw* sp, struct phase2_vrs* vrs) {
	// sp is stack pointer when FreeRTOS_DataAbortHandler invokes DataAbortInterrupt
	vrs->demand_save_flags = 0;
	// start pulling these registers off the stack
	// see xilinx_vectors.s:114 stmdb	sp!,{r0-r3,r12,lr}
	vrs->core.r[0] = sp[0];
	vrs->core.r[1] = sp[1];
	vrs->core.r[2] = sp[2];
	vrs->core.r[3] = sp[3];
	vrs->core.r[4] = sp[-1];  // DataAbortInterrupt pushes this onto stack, so grab it back
	// r5-r11 were never touched, so we can just plop them in
	asm("stm %0!, {r5-r11}\n" : : "r"(vrs->core.r + 5));
	vrs->core.r[12] = sp[4];
	// sp/lr are in (banked) user registers. recover them from there. ref B9.3.17 of AARM for v7-a/r
	asm("stm %0, {r13,r14}^" : : "r"(vrs->core.r + 13));
	vrs->core.r[15] = sp[5] - 8;
}

// called by DataAbortInterrupt in rtos_hooks.c
void report_data_abort(uint32_t _sp) {
	struct phase2_vrs vrs;
	p2vrs_from_data_abort((_uw*)_sp, &vrs);

	fputs("\n\nDATA ABORT EXCEPTION\n\n", stderr);
	vexDisplayForegroundColor(ClrWhite);
	vexDisplayBackgroundColor(ClrRed);
	vexDisplayRectClear(0, 25, 480, 125);
	vexDisplayString(2, "DATA ABORT EXCEPTION");
	vexDisplayString(3, "PC: %x", vrs.core.r[R_PC]);
	if (pxCurrentTCB) {
		vexDisplayString(4, "CURRENT TASK: %.32s\n", pxCurrentTCB->pcTaskName);
		fprintf(stderr, "CURRENT TASK: %.32s\n", pxCurrentTCB->pcTaskName);
	}

	fputs("REGISTERS AT ABORT\n", stderr);
	print_phase2_vrs(&vrs);

	fputs("BEGIN STACK TRACE\n", stderr);
	fprintf(stderr, "\t%p\n", (void*)vrs.core.r[R_PC]);
	__gnu_Unwind_Backtrace(trace_fn, NULL, &vrs);
	fputs("END OF TRACE\n", stderr);

	struct mallinfo info = mallinfo();
	fprintf(stderr, "HEAP USED: %d bytes\n", info.uordblks);
	if (pxCurrentTCB) {
		fprintf(stderr, "STACK REMAINING AT ABORT: %lu bytes\n", vrs.core.r[R_SP] - (uint32_t)pxCurrentTCB->pxStack);
	}
}

/******************************************************************************/
/**               Modified RTOS-Task Target Unwinder Functions               **/
/**                                                                          **/
/** These functions use the __gnu_Unwind_* functions providing our helper    **/
/** functions and phase2_vrs structure based on a target task                **/
/******************************************************************************/
#define REGISTER_BASE 67
static inline struct phase2_vrs p2vrs_from_task(task_t task) {
	// should be called with the task scheduler suspended
	taskENTER_CRITICAL();

	TCB_t* tcb = (TCB_t*)task;
	size_t i;

	struct phase2_vrs vrs;
	if (tcb == NULL) {
		tcb = pxCurrentTCB;
	}

	vrs.demand_save_flags = 0;
	switch (task_get_state(task)) {
		case E_TASK_STATE_READY:
			for (i = 0; i < 12; i++) {
				vrs.core.r[i] = tcb->pxTopOfStack[REGISTER_BASE + i];
			}
			vrs.core.r[13] = (_uw)(tcb->pxTopOfStack + REGISTER_BASE + 16);  // r13 is sp
			vrs.core.r[14] = tcb->pxTopOfStack[REGISTER_BASE + 13];          // r14 is lr
			vrs.core.r[15] = tcb->pxTopOfStack[REGISTER_BASE + 14];          // r15 is pc
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
		case E_TASK_STATE_DELETED:  // will never happen
		case E_TASK_STATE_INVALID:
		default:
			break;
	}
	taskEXIT_CRITICAL();
	return vrs;
}

void backtrace_task(task_t task) {
	struct phase2_vrs vrs = p2vrs_from_task(task);
	printf("Trace:\n");
	__gnu_Unwind_Backtrace(trace_fn, NULL, &vrs);
	printf("finished trace\n");
}
