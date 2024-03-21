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

	pros::Gps gps1(1);

	//Gps(const std::uint8_t port, double xInitial, double yInitial, double headingInitial);

	//Gps(const std::uint8_t port, double xOffset, double yOffset);

	//Gps(const std::uint8_t port, double xInitial, double yInitial, double headingInitial, double xOffset, double yOffset);

	while (true) {

		// test: 

        double offset_x = gps1.get_offset_x();
        double offset_y = gps1.get_offset_y();
		double error = gps1.get_error();

		double position_x = gps1.get_position_x();
		double position_y = gps1.get_position_y();
		double pitch = gps1.get_pitch();
		double roll = gps1.get_roll();
		double yaw = gps1.get_yaw();

		double heading = gps1.get_heading();
		double heading_yaw = gps1.get_heading_raw();

		double gyro_rate_x = gps1.get_gyro_rate_x();
		double gyro_rate_y = gps1.get_gyro_rate_y();
		double gyro_rate_z = gps1.get_gyro_rate_z();
        double accel_x = gps1.get_accel_x();
        double accel_y = gps1.get_accel_y();
        double accel_z = gps1.get_accel_z();

		/*

		pros::lcd::print(3, "offsetX:%f\n", offset_x);
		pros::lcd::print(4, "offsetY:%f\n", offset_y);
		pros::lcd::print(5, "error:%f\n", error);
		pros::lcd::print(6, "heading:%f\n", heading);
		pros::lcd::print(7, "headingYaw:%f\n", heading_yaw);

        */


		/*

		pros::lcd::print(3, "positionX:%f\n", position_x);
		pros::lcd::print(4, "positionY:%f\n", position_y);
        pros::lcd::print(5, "pitch:%f\n", pitch);
        pros::lcd::print(6, "roll:%f\n", roll);
        pros::lcd::print(7, "yaw:%f\n", yaw);

		*/
        

		/*
		
        pros::lcd::print(3, "gyroX:%f\n", gyro_rate_x);
        pros::lcd::print(4, "gyroY:%f\n", gyro_rate_y);
        pros::lcd::print(5, "gyroZ:%f\n", gyro_rate_z);
        pros::lcd::print(6,"accelX:%f\n", accel_x);
        pros::lcd::print(7,"accelY:%f\n", accel_y);
        pros::lcd::print(8,"accelZ:%f\n", accel_z);
		
		*/

		pros::delay(20);
	}
}
