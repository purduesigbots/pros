#include "main.h"

void my_task(void* ign) {
	while (true) {
		printf("There are %d objects\n", vision_get_object_count(20));
		delay(20);
	}
}

void my_task2(void* ign) {
	while (true) {
		int left = controller_get_analog(CONTROLLER_MASTER, ANALOG_LEFT_Y);
		int right = controller_get_analog(CONTROLLER_MASTER, ANALOG_RIGHT_Y);
		motor_move(1, left);
		motor_move(11, right);
		delay(20);
	}
}

void opcontrol() {
	task_create(my_task, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "My Task");
	// task_create(my_task2, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "my Task 2");
	while (true) {
		int left = controller_get_analog(CONTROLLER_MASTER, ANALOG_LEFT_Y);
		int right = controller_get_analog(CONTROLLER_MASTER, ANALOG_RIGHT_Y);
		// printf("%d, %d\n", left, right);
		puts("Hello from opcontrol");
		motor_move(11, left);
		motor_move(1, -right);
		delay(20);
	}
}
