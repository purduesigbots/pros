/**
 * \file tests/adi.cpp
 *
 * Test code for various ADI things
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
	pros::ADIGyro gyro ('c', 1/1.8);
	pros::ADIDigitalIn dig (4);
	pros::ADIEncoder enc (5, 6, true);
	uint32_t now = pros::millis();
 	while (true) {
 		if (dig.get_new_press())
			pros::lcd::print(4, "here");
		pros::lcd::print(2, "%d", enc.get_value());
		pros::lcd::print(1, "%f", gyro.get_value());

 		if (pros::millis() - now > 5000) {
 			gyro.reset();
 			now = pros::millis();
 		}

 		pros::delay(20);
 	}
 }
