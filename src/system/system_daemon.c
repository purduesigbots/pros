#include "system/system.h"
#include "ifi/v5_api.h"
#include "rtos/task.h"
#include "system/comp_state.h"
#include "system/optimizers.h"

static task_stack_t competition_task_stack[TASK_STACK_DEPTH_DEFAULT];
static static_task_s_t competition_task_buffer;
static task_t competition_task;

static task_stack_t system_daemon_task_stack[TASK_STACK_DEPTH_DEFAULT];
static static_task_s_t system_daemon_task_buffer;
static task_t system_daemon_task;

static void _disabled_task(void* ign);
static void _autonomous_task(void* ign);
static void _opcontrol_task(void* ign);

static void _initialize_task(void* ign);
static void _system_daemon_task(void* ign);

// libv5rts doesn't expose the symbol but gives us the prototype or return values (woops)
#define DISABLED_STATE 0
#define AUTONOMOUS_STATE 1
#define OPCONTROL_STATE 2
__attribute__((weak)) uint32_t vexCompetitionStatus(void) {
	return OPCONTROL_STATE;
}

char task_names[3][32] = { "PROS Disabled (user)", "PROS Autonomous (user)", "PROS Operator Control (user)" };

task_fn_t task_fns[3] = { _disabled_task, _autonomous_task, _opcontrol_task };

static void _system_daemon_task(void* ign) {
	uint32_t time = millis();
	// Initialize status to an invalid state to force an update the first loop
	uint32_t status = (uint32_t)-1;
	uint32_t task_state;

	competition_task =
	    task_create_static(_initialize_task, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT,
	                       "PROS Initialization (user)", competition_task_stack, &competition_task_buffer);

	task_suspend(NULL);  // _initialize_task will resume us

	time = millis();
	while (1) {
		taskENTER_CRITICAL();
		{
			vexBackgroundProcessing();  // TODO: figure out how much stack space this requires
		}
		taskEXIT_CRITICAL();

		if (unlikely(status != vexCompetitionStatus())) {
			// TODO: make sure vexCompetitionStatus returns a valid result. Don't know
			// what to expect yet

			// Have a new competition status, need to clean up whatever's running
			status = vexCompetitionStatus();
			task_state = task_get_state(competition_task);
			// delete the task only if it's validly running. will never be
			// E_TASK_STATE_RUNNING
			if (task_state == E_TASK_STATE_READY || task_state == E_TASK_STATE_BLOCKED ||
			    task_state == E_TASK_STATE_SUSPENDED) {
				task_delete(competition_task);
			}

			competition_task =
			    task_create_static(task_fns[status], NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT,
			                       task_names[status], competition_task_stack, &competition_task_buffer);
		}

		task_delay_until(&time, 2);
	}
}

void system_daemon_initialize() {
	system_daemon_task =
	    task_create_static(_system_daemon_task, NULL, TASK_PRIORITY_MAX - 2, TASK_STACK_DEPTH_DEFAULT,
	                       "PROS System Daemon", system_daemon_task_stack, &system_daemon_task_buffer);
}

__attribute__((weak)) void autonomous() {}
__attribute__((weak)) void initialize() {}
__attribute__((weak)) void opcontrol() {}
__attribute__((weak)) void disabled() {}

static void _initialize_task(void* ign) {
	initialize();
	task_resume(system_daemon_task);
}
static void _autonomous_task(void* ign) {
	autonomous();
}
static void _opcontrol_task(void* ign) {
	opcontrol();
}
static void _disabled_task(void* ign) {
	disabled();
}
