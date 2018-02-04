#include "api.h"

void opcontrol() {
	while (true) {
		int left = controller_get_analog(CONTROLLER_MASTER, ANALOG_LEFT_Y);
		int right = controller_get_analog(CONTROLLER_MASTER, ANALOG_RIGHT_Y);
		printf("%d, %d\n", left, right);
		motor_set_velocity(1, left);
		motor_set_velocity(11, right);
		delay(20);
	}
}
