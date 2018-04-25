#include <iostream>
#include "main.h"

void my_task(void* str) {
	pros::Vision vision(20);
	while (true) {
		std::cout << (char*)str << std::endl;
		std::cout << vision.get_object_count() << std::endl;
		pros::delay(20);
	}
}

void opcontrol() {
	{
		std::string s = "Hello world";
		pros::Task t(my_task, (void*)(s.c_str()));
	}
	while (true) {
		pros::Motor(11).move(pros::Controller(E_CONTROLLER_MASTER).get_analog(E_CONTROLLER_ANALOG_LEFT_Y));
		pros::Motor(1).move(-pros::Controller(E_CONTROLLER_MASTER).get_analog(E_CONTROLLER_ANALOG_RIGHT_Y));
		std::puts("Hello from opcontrol");
		pros::Task::delay(20);
	}
}
