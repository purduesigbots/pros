#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

#define TX_LINK_PORT 20
#define RX_LINK_PORT 10
void opcontrol() {
	printf("Init \n");
	pros::c::link_init(TX_LINK_PORT, "test", pros::E_LINK_TRANSMITTER);
	pros::c::link_init(RX_LINK_PORT, "test", pros::E_LINK_RECIEVER);
	char buf[100] = "none";
	while (true) {
		printf("Readable Size: \n");
		if(pros::c::link_readable_size(RX_LINK_PORT) > 0) {
			printf("Readable Size Actual: %d\n", pros::c::link_readable_size(RX_LINK_PORT));
			pros::c::link_read_raw(RX_LINK_PORT, buf, pros::c::link_readable_size(RX_LINK_PORT));
			pros::lcd::print(0, "Recieved Message: %s", buf);
		}
		printf("2 transmits\n");
		pros::c::link_transmit_raw(TX_LINK_PORT, (void*)" Test_MSG1 ");
		pros::c::link_transmit_raw(TX_LINK_PORT, (void*)" Test_MSG2 ");
		pros::lcd::print(2, "Errno: %d", errno); // ENODEV
		pros::lcd::print(3, "TX Device Connected?: %d", pros::c::link_connected(TX_LINK_PORT));
		pros::lcd::print(4, "RX Device Connected?: %d", pros::c::link_connected(RX_LINK_PORT));
		pros::lcd::print(5, "TX Size: %d", pros::c::link_peek_raw(TX_LINK_PORT));
		pros::delay(2500);
	}
}
