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
#include <setjmp.h>

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

#define REGISTER_BASE		67
//
extern void task_clean_up();

_Unwind_Reason_Code trace_fn(_Unwind_Context* unwind_ctx, void* d) {
	// printf("trace_fn: %x\n", d);
	// int* depth = (int*)d;
	// phase2_vrs* vrs = (phase2_vrs*)unwind_ctx;
	// for(uint8_t i = 0; i < 16; i++) {
	// 	printf("r%d: %x\n", i, vrs->core.r[i]);
	// }
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

/* We use normal integer types here to avoid the compiler generating
   coprocessor instructions.  */
struct vfp_regs
{
  _uw64 d[16];
  _uw pad;
};

struct vfpv3_regs
{
  /* Always populated via VSTM, so no need for the "pad" field from
     vfp_regs (which is used to store the format word for FSTMX).  */
  _uw64 d[16];
};

struct wmmxd_regs
{
  _uw64 wd[16];
};

struct wmmxc_regs
{
  _uw wc[4];
};

/* The ABI specifies that the unwind routines may only use core registers,
   except when actually manipulating coprocessor state.  This allows
   us to write one implementation that works on all platforms by
   demand-saving coprocessor registers.
   During unwinding we hold the coprocessor state in the actual hardware
   registers and allocate demand-save areas for use during phase1
   unwinding.  */

typedef struct
{
  /* The first fields must be the same as a phase2_vrs.  */
  _uw demand_save_flags;
  struct core_regs core;
  _uw prev_sp; /* Only valid during forced unwinding.  */
  struct vfp_regs vfp;
  struct vfpv3_regs vfp_regs_16_to_31;
  struct wmmxd_regs wmmxd;
  struct wmmxc_regs wmmxc;
} phase1_vrs;

#define DEMAND_SAVE_VFP 1	/* VFP state has been saved if not set */
#define DEMAND_SAVE_VFP_D 2	/* VFP state is for FLDMD/FSTMD if set */
#define DEMAND_SAVE_VFP_V3 4    /* VFPv3 state for regs 16 .. 31 has
                                   been saved if not set */
#define DEMAND_SAVE_WMMXD 8	/* iWMMXt data registers have been
				   saved if not set.  */
#define DEMAND_SAVE_WMMXC 16	/* iWMMXt control registers have been
				   saved if not set.  */

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
void recover_vrs_from_data_abort(_uw* sp, phase2_vrs* vrs) {
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
  recover_vrs_from_data_abort((_uw*)_sp, &vrs);
  puts("DATA ABORT EXCEPTION\n");
  puts("REGISTERS AT ABORT");
	print_phase2_vrs(&vrs);
  _Unwind_Control_Block ucbp;
  ucbp.unwinder_cache.reserved1 = 0;

  puts("BEGIN STACK TRACE");
  __gnu_Unwind_Backtrace(trace_fn, NULL, &vrs);
	puts("END OF TRACE");
  // extern task_t competition_task;
  // backtrace_task(competition_task);
}

static inline phase2_vrs create_phase2_vrs(task_t task) {
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

// struct unwind_cleanup {
// 	jmp_buf env;
// 	int depth;
// 	TCB_t* tcb;
// };
//
// extern void unwind_setjmp(struct unwind_cleanup* ptr, void* ret);
//
// extern void* __cxa_allocate_exception(size_t size);
// extern _Unwind_Reason_Code __gnu_Unwind_Backtrace(_Unwind_Trace_Fn trace, void* trace_argument, phase2_vrs* entry_vrs);
// extern _Unwind_Reason_Code __gnu_Unwind_ForcedUnwind(_Unwind_Control_Block* ucbp, _Unwind_Stop_Fn stop_fn, void* arg, phase2_vrs* ptr);
//
// /******************************************************************************/
// /**                         Unwinder helper functions                        **/
// /******************************************************************************/
//
// _Unwind_Reason_Code
// test_forced_unwind (_Unwind_Trace_Fn trace, void * trace_argument,
// 		       phase2_vrs * entry_vrs)
// {
// 	printf("arm's vrs:\n");
// 	for(uint8_t i = 0; i < 16; i++) {
// 		printf("r%d: %x\n", i, entry_vrs->core.r[i]);
// 	}
// 	return 0;
// 	// return __gnu_Unwind_ForcedUnwind(ucbp, stop_fn, stop_arg, entry_vrs);
// }
//
// _Unwind_Reason_Code unwind_stop_fn(int i, _Unwind_Action act, _Unwind_Exception_Class cls,
// 	_Unwind_Control_Block * ucbp, struct _Unwind_Context * unwind_ctx, void * arg) {
// 	uint32_t pc = _Unwind_GetIP(unwind_ctx);
// 	fprintf(stdout, "\t0x%08lx\n", pc);
// 	if (pc == (uint32_t)task_clean_up) {
// 		printf("done %d\n", __LINE__);
// 		longjmp(((struct unwind_cleanup*)arg)->env, 1);
// 		return _URC_FAILURE;
// 	}
// 	fprintf(stdout, "continuing to unwind\n");
// 	task_delay(2);
// 	return _URC_NO_REASON;
// }
//
// void exception_cleanup(_Unwind_Reason_Code code, _Unwind_Control_Block* ucb) {
// 	printf("Cleanup this damn exception %d\n", code);
// 	printf("%x\n", ucb->pr_cache.fnstart);
// 	printf("%x %x %x %x %x %x %x %x \n", ucb->exception_class[0],
// 	ucb->exception_class[1],
// 	ucb->exception_class[2],
// 	ucb->exception_class[3],
// 	ucb->exception_class[4],
// 	ucb->exception_class[5],
// 	ucb->exception_class[6],
// 	ucb->exception_class[7]);
// }

//
//
// // static inline void print_unwind_cleanup(struct unwind_cleanup* cleanup) {
// // 	static const char registers[16][4] = {
// // 		"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "sp", "lr", "pc"
// // 	};
// // 	for(size_t i = 0; i < 15; i++) {
// // 		printf("%3s: %#x\n", registers[i], cleanup->r[i]);
// // 	}
// // 	printf("cpsr: %#x\n depth: %d\n", cleanup->cpsr, cleanup->depth);
// // }
//
// /******************************************************************************/
// /**                        Unwinder "Native" Functions                       **/
// /**                                                                          **/
// /** These functions use the _Unwind_* functions directly providing our       **/
// /** helper functions                                                         **/
// /******************************************************************************/
// void backtrace_self() {
// 	int depth = 0;
// 	_Unwind_Backtrace(trace_fn, &depth);
// }
//
// void unwind_self() {
// 	int depth = 0;
// 	_Unwind_Control_Block* ucbp = __cxa_allocate_exception(sizeof *ucbp);
// 	memcpy(ucbp->exception_class, "ARM\0PROS", 8);
// 	ucbp->exception_cleanup = exception_cleanup;
// 	ucbp->unwinder_cache.reserved1 = 0;
// 	_Unwind_ForcedUnwind(ucbp, unwind_stop_fn, &depth);
// }
//
// /******************************************************************************/
// /**               Modified RTOS-Task Target Unwinder Functions               **/
// /**                                                                          **/
// /** These functions use the __gnu_Unwind_* functions providing our helper    **/
// /** functions and phase2_vrs structure based on a target task                **/
// /******************************************************************************/
void backtrace_task(task_t task) {
	phase2_vrs vrs = create_phase2_vrs(task);
  printf("Trace:\n");
  _Unwind_Control_Block ucbp;
	__gnu_Unwind_Backtrace(trace_fn, NULL, &vrs);
	printf("finished trace\n");
}
//
// void unwind_task(task_t task) {
// 	// performs a half-hearted context switch that sets up the link register and stack pointer
// 	TCB_t* tcb = (TCB_t*)task;
// 	if(tcb == NULL) {
// 		tcb = pxCurrentTCB;
// 	}
//
// 	_Unwind_Control_Block* ucbp = __cxa_allocate_exception(sizeof *ucbp);
// 	memcpy(ucbp->exception_class, "ARM\0PROS", 8);
// 	ucbp->exception_cleanup = exception_cleanup;
// 	ucbp->unwinder_cache.reserved1 = 0;
//
// 	phase2_vrs vrs = create_phase2_vrs(task);
//
// 	struct unwind_cleanup cleanup;
// 	cleanup.depth = 0;
// 	int result = setjmp(cleanup.env);
// 	printf("Howdy\n");
// 	if(result == 0) {
// 		printf("unwinding!\n");
// 		__gnu_Unwind_ForcedUnwind(ucbp, unwind_stop_fn, &cleanup, &vrs);
// 	}
//
// 	// __gnu_Unwind_ForcedUnwind(ucbp, unwind_stop_fn, &depth, &vrs);
// 	printf("finished unwinding\n");
// }
//
// // debugging function to print out important information from a task's stack
// // for figuring out its current state
// void print_stack(task_t task) {
// 	TCB_t* tcb = (TCB_t*)task;
// 	if(tcb == NULL) {
// 		tcb = pxCurrentTCB;
// 	}
//
// 	for(int i = -10 ; i < 50; i++) {
// 		printf("67 + %d/%#lx: %#lx\n", i, (unsigned long)(tcb->pxTopOfStack + 67 + i), tcb->pxTopOfStack[67 + i]);
// 	}
// }
