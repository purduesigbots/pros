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

#include "pros/imu.h"
#include "pros/imu.hpp"

namespace pros {
std::int32_t Imu::reset() const {
	return pros::c::imu_reset(_port);
}

std::int32_t Imu::set_data_rate(std::uint32_t rate) const {
	return pros::c::imu_set_data_rate(_port, rate);
}

double Imu::get_rotation() const {
    return pros::c::imu_get_rotation(_port);
}

double Imu::get_heading() const {
	return pros::c::imu_get_heading(_port);
}

pros::c::quaternion_s_t Imu::get_quaternion() const {
	return pros::c::imu_get_quaternion(_port);
}

pros::c::euler_s_t Imu::get_euler() const {
	return pros::c::imu_get_euler(_port);
}

double Imu::get_pitch() const {
	return get_euler().pitch;
}

double Imu::get_roll() const {
	return get_euler().roll;
}

double Imu::get_yaw() const {
	return get_euler().yaw;
}

pros::c::imu_gyro_s_t Imu::get_gyro_rate() const {
	return pros::c::imu_get_gyro_rate(_port);
}

pros::c::imu_accel_s_t Imu::get_accel() const {
	return pros::c::imu_get_accel(_port);
}

pros::c::imu_status_e_t Imu::get_status() const {
	return pros::c::imu_get_status(_port);
}

bool Imu::is_calibrating() const {
	return get_status() & pros::c::E_IMU_STATUS_CALIBRATING;
}
}  // namespace pros
