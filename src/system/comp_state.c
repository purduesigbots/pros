#include "api.h"
#include "main.h"

#include "competition.h"

static void _autonomous_task(void* ign) { autonomous(); }

static void _initialize_task(void* ign) { initialize(); }

static void _opcontrol_task(void* ign) { opcontrol(); }

void competition_initialize()
{
	xTaskCreate(_opcontrol_task, NULL, "Operator Control",
	            TASK_PRIO_DEFAULT, TASK_STACK_DEPTH_DEFAULT);
}

__attribute__((weak)) void autonomous() {}
__attribute__((weak)) void initialize() {}
__attribute__((weak)) void opcontrol() {}