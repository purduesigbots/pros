/**
 * \file devices/vdml_imu.cpp
 *
 * Contains functions for interacting with the VEX Inertial sensor.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "pros/imu.hpp"
#include "vdml/vdml.h"

namespace pros {
inline namespace v5 {

std::int32_t Imu::reset(bool blocking /*= false*/) const {
	return blocking ? pros::c::imu_reset_blocking(_port) : pros::c::imu_reset(_port);
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

pros::quaternion_s_t Imu::get_quaternion() const {
	return pros::c::imu_get_quaternion(_port);
}

pros::euler_s_t Imu::get_euler() const {
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

pros::imu_gyro_s_t Imu::get_gyro_rate() const {
	return pros::c::imu_get_gyro_rate(_port);
}

pros::imu_accel_s_t Imu::get_accel() const {
	return pros::c::imu_get_accel(_port);
}

pros::ImuStatus Imu::get_status() const {
	return static_cast<pros::ImuStatus>(pros::c::imu_get_status(_port));
}

bool Imu::is_calibrating() const {
	return (int)get_status() & (int)(pros::ImuStatus::calibrating);
}

std::int32_t Imu::tare_heading() const {
	return pros::c::imu_tare_heading(_port);
}

std::int32_t Imu::tare_rotation() const {
	return pros::c::imu_tare_rotation(_port);
}

std::int32_t Imu::tare_pitch() const {
	return pros::c::imu_tare_pitch(_port);
}

std::int32_t Imu::tare_yaw() const {
	return pros::c::imu_tare_yaw(_port);
}

std::int32_t Imu::tare_roll() const {
	return pros::c::imu_tare_roll(_port);
}

std::int32_t Imu::tare_euler() const {
	return pros::c::imu_tare_euler(_port);
}

std::int32_t Imu::set_heading(double target) const {
	return pros::c::imu_set_heading(_port, target);
}

std::int32_t Imu::set_rotation(double target) const {
	return pros::c::imu_set_rotation(_port, target);
}

std::int32_t Imu::set_pitch(double target) const {
	return pros::c::imu_set_pitch(_port, target);
}

std::int32_t Imu::set_yaw(double target) const {
	return pros::c::imu_set_yaw(_port, target);
}

std::int32_t Imu::set_roll(double target) const {
	return pros::c::imu_set_roll(_port, target);
}

std::int32_t Imu::set_euler(pros::euler_s_t target) const {
	return pros::c::imu_set_euler(_port, target);
}

std::int32_t Imu::tare() const {
	return pros::c::imu_tare(_port);
}

std::ostream& operator<<(std::ostream& os, const pros::Imu& imu) {
	pros::imu_gyro_s_t gyro = imu.get_gyro_rate();
	pros::imu_accel_s_t accel = imu.get_accel();
	os << "Imu [";
	os << "port: " << imu._port;
	os << ", rotation: " << imu.get_rotation();
	os << ", heading: " << imu.get_heading();
	os << ", pitch: " << imu.get_pitch();
	os << ", roll: " << imu.get_roll();
	os << ", yaw: " << imu.get_yaw();
	os << ", gyro rate: "
	   << "{" << gyro.x << "," << gyro.y << "," << gyro.z << "}";
	os << ", get accel: "
	   << "{" << accel.x << "," << accel.y << "," << accel.z << "}";
	os << ", calibrating: " << imu.is_calibrating();
	os << "]";
	return os;
}

namespace literals {
const pros::Imu operator"" _imu(const unsigned long long int i) {
	return pros::Imu(i);
}
}  // namespace literals
}  // namespace v5
}  // namespace pros
