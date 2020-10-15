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
#include "api.h"

namespace pros {
std::int32_t Imu::reset() const {
	return pros::c::imu_reset(_port);
}

std::int32_t Imu::set_data_rate(std::uint32_t rate) const {
	return pros::c::imu_set_data_rate(_port, rate);
}

double Imu::get_rotation() const {
    return pros::c::imu_get_rotation(_port) + _rotation_offset;
}

double Imu::get_heading() const {
	return pros::c::imu_get_heading(_port) + _heading_offset;
}

pros::c::quaternion_s_t Imu::get_quaternion() const {
	return pros::c::imu_get_quaternion(_port);
}

pros::c::euler_s_t Imu::get_euler() const {
	return pros::c::imu_get_euler(_port);
}

double Imu::get_pitch() const {
	return get_euler().pitch + _pitch_offset;
}

double Imu::get_roll() const {
	return get_euler().roll + _roll_offset;
}

double Imu::get_yaw() const {
	return get_euler().yaw + _yaw_offset;
}

//Value reset functions:

std::int32_t Imu::reset_rotation() {
	std::int32_t current = pros::c::imu_get_rotation(_port);
	if(current == PROS_ERR){
		return PROS_ERR;
	}
	this->_rotation_offset = -current;
	return 1;
}

std::int32_t Imu::reset_heading() {
	std::int32_t current = pros::c::imu_get_heading(_port);
	if(current == PROS_ERR){
		return PROS_ERR;
	}
	this->_heading_offset = -current;
	return 1;
}

std::int32_t Imu::reset_pitch() {
	std::int32_t current = pros::c::imu_get_pitch(_port);
	if(current == PROS_ERR){
		return PROS_ERR;
	}
	this->_pitch_offset = -current;
	return 1;
}

std::int32_t Imu::reset_roll() {
	std::int32_t current = pros::c::imu_get_pitch(_port);
	if(current == PROS_ERR){
		return PROS_ERR;
	}
	this->_roll_offset = -current;
	return 1;
}

std::int32_t Imu::reset_yaw() {
	std::int32_t current = pros::c::imu_get_yaw(_port);
	if(current == PROS_ERR){
		return PROS_ERR;
	}
	this->_yaw_offset = -current;
	return 1;
}

// Current Value Setters: Set Value

std::int32_t Imu::set_heading(std::int32_t target) {
	std::int32_t current = pros::c::imu_get_heading(_port);
	if(current == PROS_ERR){
		return PROS_ERR;
	}
	this->_heading_offset = target - current;
	return 1;
}

std::int32_t Imu::set_rotation(std::int32_t target) {
	std::int32_t current = pros::c::imu_get_pitch(_port);
	if(current == PROS_ERR){
		return PROS_ERR;
	}
	this->_pitch_offset = target - current;
	return 1;
}

std::int32_t Imu::set_pitch(std::int32_t target) {
	std::int32_t current = pros::c::imu_get_pitch(_port);
	if(current == PROS_ERR){
		return PROS_ERR;
	}
	this->_roll_offset = target - current;
	return 1;
}

std::int32_t Imu::set_roll(std::int32_t target) {
	std::int32_t current = pros::c::imu_get_pitch(_port);
	if(current == PROS_ERR){
		return PROS_ERR;
	}
	this->_roll_offset = target - current;
	return 1;
}

std::int32_t Imu::set_yaw(std::int32_t target) {
	std::int32_t current = pros::c::imu_get_yaw(_port);
	if(current == PROS_ERR){
		return PROS_ERR;
	}
	this->_yaw_offset = -current;
	return 1;
}

//Current Value Getters:

std::int32_t Imu::get_heading_offset() const {
	return this->_heading_offset;
}

std::int32_t Imu::get_rotation_offset() const {
	return this->_rotation_offset;
}

std::int32_t Imu::get_roll_offset() const {
	return this->_roll_offset;
}

std::int32_t Imu::get_pitch_offset() const {
	return this->_pitch_offset;
}

std::int32_t Imu::get_yaw_offset() const {
	return this->_yaw_offset;
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
