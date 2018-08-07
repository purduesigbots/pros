#include "ifi/v5_api.h"
#include "kapi.h"
#include "system/optimizers.h"

extern void vdml_initialize();
extern void vdml_background_processing();

extern void port_mutex_take_all();
extern void port_mutex_give_all();

static task_stack_t competition_task_stack[TASK_STACK_DEPTH_DEFAULT];
static static_task_s_t competition_task_buffer;
static task_t competition_task;

static task_stack_t system_daemon_task_stack[TASK_STACK_DEPTH_DEFAULT];
static static_task_s_t system_daemon_task_buffer;
static task_t system_daemon_task;

static void _disabled_task(void* ign);
static void _autonomous_task(void* ign);
static void _opcontrol_task(void* ign);
static void _competition_initialize_task(void* ign);

static void _initialize_task(void* ign);
static void _system_daemon_task(void* ign);

#define COMP_MODE_MASK 3
// libv5rts doesn't expose the symbol but gives us the prototype or return values (woops)
enum competition_states {
	E_STATE_OPCONTROL = 0,
	E_STATE_AUTON = 1,
	E_STATE_DISABLED = 2,
	E_STATE_INIT = 3,
	E_STATE_UNKNOWN
};

char task_names[4][32] = {"User Operator Control (PROS)", "User Autonomous (PROS)", "User Disabled (PROS)",
                          "User Comp. Init. (PROS)"};
task_fn_t task_fns[4] = {_opcontrol_task, _autonomous_task, _disabled_task, _competition_initialize_task};

extern void ser_output_flush(void);

// does the basic background operations that need to occur every 2ms
static inline void do_background_operations() {
	port_mutex_take_all();
	ser_output_flush();
	rtos_suspend_all();
	vexBackgroundProcessing();
	rtos_resume_all();
	vdml_background_processing();
	port_mutex_give_all();
}

static void _system_daemon_task(void* ign) {
	uint32_t time = millis();
	// Initialize status to an invalid state to force an update the first loop
	uint32_t status = (uint32_t)(1 << 8);
	uint32_t task_state;

	// XXX: Delay likely necessary for shared memory to get copied over (discovered b/c VDML would crash and burn)
	task_delay(1);

	// initialization that needs to occur with the scheduler started
	vdml_initialize();
	jinx_init();

	// start up user initialize task. once the user initialize function completes,
	// the _initialize_task will notify us and we can go into normal competition
	// monitoring mode
	competition_task = task_create_static(_initialize_task, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT,
	                                      "User Initialization (PROS)", competition_task_stack, &competition_task_buffer);

	time = millis();
	while (!task_notify_take(true, 2)) {
		// wait for initialize to finish
		do_background_operations();
	}

	while (1) {
		do_background_operations();

		if (unlikely(status != competition_get_status())) {
			// TODO: make sure vexCompetitionStatus returns a valid result. Don't know
			// what to expect yet

			// Have a new competition status, need to clean up whatever's running
			uint32_t old_status = status;
			status = competition_get_status();
			enum competition_states new_state = status & COMP_MODE_MASK;
			if (new_state & E_STATE_DISABLED) {
				new_state = E_STATE_DISABLED;
			}
			// competition initialize runs only when entering disabled and when we weren't previously
			// connected to field control
			if (new_state == E_STATE_DISABLED && !(old_status & COMPETITION_CONNECTED)) {
				new_state = E_STATE_INIT;
			}
			if (!(status & COMPETITION_CONNECTED)) {
				new_state = E_STATE_OPCONTROL;
			}

			task_state = task_get_state(competition_task);
			// delete the task only if it's in normal operation (e.g. not deleted)
			// The valid task states AREN'T deleted, invalid, or running (running means it's
			// the current task, which will never be the case)
			if (task_state == E_TASK_STATE_READY || task_state == E_TASK_STATE_BLOCKED ||
			    task_state == E_TASK_STATE_SUSPENDED) {
				task_delete(competition_task);
			}

			competition_task = task_create_static(task_fns[new_state], NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT,
			                                      task_names[new_state], competition_task_stack, &competition_task_buffer);
		}

		task_delay_until(&time, 2);
	}
}

void system_daemon_initialize() {
	system_daemon_task = task_create_static(_system_daemon_task, NULL, TASK_PRIORITY_MAX - 2, TASK_STACK_DEPTH_DEFAULT,
	                                        "PROS System Daemon", system_daemon_task_stack, &system_daemon_task_buffer);
}

// description of some cases for implementing autonomous:
// user implements autonomous w/C linkage: system daemon starts _autonomous_task which calls the user's autonomous()
// implement w/C++ linkage: sysd starts _autonomous_task calls autonomous() defined here which calls cpp_autonomous
// which calls the user's C++ linkage autonomous()
// implement no autonomous: see above, but cpp_autonomous implements a stub C++ autonomous()

// Our weak functions call C++ links of these functions, allowing users to only optionally extern "C" the task functions
extern void cpp_autonomous();
extern void cpp_initialize();
extern void cpp_opcontrol();
extern void cpp_disabled();
extern void cpp_competition_initialize();

// default implementations of the different competition modes attempt to call the C++
// linkage version of the function
__attribute__((weak)) void autonomous() {
	cpp_autonomous();
}
__attribute__((weak)) void initialize() {
	cpp_initialize();
}
__attribute__((weak)) void opcontrol() {
	cpp_opcontrol();
}
__attribute__((weak)) void disabled() {
	cpp_disabled();
}
__attribute__((weak)) void competition_initialize() {
	cpp_competition_initialize();
}

// these functions are what actually get called by the system daemon, which attempt
// to call whatever the user declares
static void _competition_initialize_task(void* ign) {
	competition_initialize();
}

static void _initialize_task(void* ign) {
	initialize();
	task_notify(system_daemon_task);
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
