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
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);

// pros::Motor left_mtr_1(1);
// 	pros::Motor left_mtr_2(2);
// 	pros::Motor left_mtr_3(3);

	pros::Motor right_mtr_1(8);
	pros::Motor right_mtr_2(9);
	pros::Motor right_mtr_3(10);
	 pros::Motor_Group left_mg({1, 2, 3});
	pros::Motor_Group right_mg({right_mtr_1, right_mtr_2, right_mtr_3});
	left_mg.set_reversed(true);
	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		int left = master.get_analog(ANALOG_LEFT_Y);
		int right = master.get_analog(ANALOG_RIGHT_Y);
		// pros::lcd::print(3, "%d", left);
		left_mg = left;
		pros::lcd::print(5, "Right Ports: %d %d %d", left_mg.get_current_limits()[0], left_mg.get_current_limits()[1], left_mg.get_current_limits()[2]);
		pros::lcd::print(4, "Left Ports: %d %d %d", left_mg.get_ports()[0], left_mg.get_ports()[1], left_mg.get_ports()[2]);
		//pros::controller_digital_e_t button;
		// button = DIGITAL_A;
		// if (master.get_digital(button)) {
		// 	left_mg.set_reversed(false);
		// }
		// pros::controller_digital_e_t button2;
		// button2 = DIGITAL_B;
		// if (master.get_digital(button2)) {
		// 	left_mg.set_reversed(true);
		// }
		// left_mg.move_voltage(10000);
		
		// right_mg = right;
		pros::delay(20);
	}
}
