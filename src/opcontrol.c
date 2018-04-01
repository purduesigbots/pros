#include "main.h"

void opcontrol() {
	while (true) {
		int left = controller_get_analog(CONTROLLER_MASTER, ANALOG_LEFT_Y);
		int right = controller_get_analog(CONTROLLER_MASTER, ANALOG_RIGHT_Y);
		printf("%d, %d\n", left, right);
		motor_move(1, left);
		motor_move(2, right);
		delay(20);
	}
}
