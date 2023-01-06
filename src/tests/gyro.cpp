/**
 * \file tests/gyro.cpp
 *
 * Test code for the gyroscope driver
 *
 * NOTE: There should also be a call to the constructor for the gyroscope object
 * in initialize() for calibration to occur before the opcontrol code.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "main.h"
void opcontrol() {
	pros::Motor left_mtr(1);
	pros::motor_pid_s_t pid = pros::Motor::convert_pid(1.0, 0.0001, 1.0, 2.0);
	pros::lcd::print(2, "%d %d %d %d", pid.kp, pid.ki, pid.kd, pid.kf);
	left_mtr.set_vel_pid(pid);
	pros::ADIGyro gyro('c', 1 / 1.8);
	pros::motor_pid_full_s_t pidf = left_mtr.get_vel_pid();
	pros::lcd::print(3, "%d %d %d %d", pidf.kp, pidf.ki, pidf.kd, pidf.kf);
	uint32_t now = pros::millis();
	while (true) {
		pros::lcd::print(1, "%f", gyro.get_value());

		if (pros::millis() - now > 5000) {
			gyro.reset();
			now = pros::millis();
		}
		pros::delay(20);
	}
}
