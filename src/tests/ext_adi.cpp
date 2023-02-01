/**
 * \file tests/ext_adi.cpp
 *
 * Test code for various External ADI things
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
	pros::ADIUltrasonic ultra({1, 'a', 'b'});
	pros::ADIButton button({1, 'c'});
	pros::ADIEncoder enc({1, 5, 6}, true);
	uint32_t now = pros::millis();
	while (true) {
		pros::lcd::print(3, "%d", ultra.get_value());
		pros::lcd::print(2, "%d", button.get_value());
		pros::lcd::print(1, "%d", enc.get_value());

		pros::delay(20);
	}
}
