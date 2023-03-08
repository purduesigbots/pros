/**
 * \file tests/gyro.c
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
	motor_pid_s_t pid = motor_convert_pid(1.0, 0.0001, 1.0, 2.0);
	lcd_print(2, "%d %d %d %d", pid.kp, pid.ki, pid.kd, pid.kf);
	motor_set_vel_pid(1, pid);
	adi_gyro_t gyro = adi_gyro_init('c', 1 / 1.8);
	motor_pid_full_s_t pidf = motor_get_vel_pid(1);
	lcd_print(3, "%d %d %d %d", pidf.kp, pidf.ki, pidf.kd, pidf.kf);
	uint32_t now = millis();
	while (true) {
		lcd_print(1, "%f", adi_gyro_get(gyro));

		if (millis() - now > 5000) {
			adi_gyro_reset(gyro);
			now = millis();
		}
		delay(20);
	}
}
