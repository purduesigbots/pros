// This test is mostly verifying that the idle task isn't being starved

#include <stdio.h>

#include "rtos/FreeRTOS.h"
#include "rtos/task.h"

#define STACK_SIZE 0x2000

task_stack_t staticTaskStack[STACK_SIZE];
static_task_s_t staticTaskTask;
task_t staticTaskHandle;

void myStaticTask(void*);
// the task_delay(5) SHOULD NOT BE REQUIRED but FreeRTOS is buggy af
void myTask(void* ign) {
	printf("myTask %d task_notify_take\n", __LINE__);
	task_delay(10);
	task_notify_take(true, 0xffffffffUL);
	printf("myTask %d task_create_static\n", __LINE__);
	task_delay(10);
	task_create_static(myStaticTask, NULL, 8, STACK_SIZE, "My Static Task", staticTaskStack, &staticTaskTask);
	printf("myTask %d task_delete\n", __LINE__);
	task_delay(10);
	task_delete(NULL);
}

void myStaticTask(void* ign) {
	printf("myStaticTask %d task_create\n", __LINE__);
	task_delay(10);
	task_t task = task_create(myTask, NULL, 8, STACK_SIZE, "My Task");
	printf("myStaticTask %d task_notify %p\n", __LINE__, task);
	task_delay(10);
	task_notify(task);
	printf("myStaticTask %d task_delete\n", __LINE__);
	task_delay(10);
	task_delete(NULL);
}

void opcontrol() {
	task_create_static(myStaticTask, NULL, 8, STACK_SIZE, "My Static Task", staticTaskStack, &staticTaskTask);
}
