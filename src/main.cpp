#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {

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
using namespace pros;
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
    Imu imutest(4);
	while (true) {
		pros::lcd::print(0, "Rotation: %f", imutest.get_rotation());
		pros::lcd::print(1, "Heading: %f", imutest.get_heading());
		pros::lcd::print(2, "pitch: %f", pros::c::imu_get_pitch(4));
		pros::lcd::print(3, "yaw: %f", pros::c::imu_get_yaw(4));
		pros::lcd::print(4, "roll: %f", pros::c::imu_get_roll(4));
/*
		pros::lcd::print(5, "pitch offset: %f", pros::c::imu_get_pitch_offset(4));
		pros::lcd::print(6, "yaw offset: %f", pros::c::imu_get_yaw_offset(4));
		pros::lcd::print(7, "roll offset: %f", pros::c::imu_get_roll_offset(4));
*/
		if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_L1)){
			imutest.set_rotation(120);
		}
		if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_L2)){
			imutest.set_pitch(120);
		}
		if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_UP)){
			imutest.set_heading(120);
		}
		if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN)){
			imutest.set_yaw(120);
		}
		if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_LEFT)){
			imutest.set_roll(120);
			//pros::c::imu_set_roll(4,120);
			//imutest.reset();
		}
		if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_RIGHT)){
			imutest.reset();
		}
		if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_A
		)){
			imutest.reset();
		}
		if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_R1)){
			imutest.reset_rotation();
		}
		if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_R2)){
			imutest.reset_pitch();
		}
		if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_X
		)){
			imutest.reset_heading();
		}
				if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_Y
		)){
			imutest.reset_roll();
		}
				if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_B
		)){
			imutest.reset_roll();
		}
		pros::delay(20);
	}
}
