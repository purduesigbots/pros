/**
 * \file devices/vdml_imu.cpp
 *
 * Contains functions for interacting with the VEX Inertial sensor.
 *
 * Copyright (c) 2017-2019, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "pros/imu.hpp"

namespace pros {
std::int32_t Imu::reset() const {
	return pros::c::imu_reset(_port);
}

double Imu::get_heading() const {
	return pros::c::imu_get_heading(_port);
}

double Imu::get_degrees() const {
	return pros::c::imu_get_degrees(_port);
}

pros::c::quaternion_s_t Imu::get_quaternion() const {
	return pros::c::imu_get_quaternion(_port);
}

pros::c::attitude_s_t Imu::get_attitude() const {
	return pros::c::imu_get_attitude(_port);
}

pros::c::imu_s_t Imu::get_raw_gyro() const {
	return pros::c::imu_get_raw_gyro(_port);
}

pros::c::imu_s_t Imu::get_raw_accel() const {
	return pros::c::imu_get_raw_accel(_port);
}

pros::c::imu_status_e_t Imu::get_status() const {
	return pros::c::imu_get_status(_port);
}

bool Imu::is_calibrating() const {
	return get_status() & pros::c::E_IMU_STATUS_CALIBRATING;
}
}  // namespace pros
