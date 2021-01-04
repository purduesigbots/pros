/**
 * \file devices/vdml_motors.cpp
 *
 * Contains functions for interacting with the V5 Motors.
 *
 * Copyright (c) 2017-2020, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "kapi.h"
#include "pros/motors.hpp"

namespace pros {
using namespace pros::c;

Motor::Motor(const std::uint8_t port, const motor_gearset_e_t gearset, const bool reverse,
             const motor_encoder_units_e_t encoder_units)
    : _port(port) {
	set_gearing(gearset);
	set_reversed(reverse);
	set_encoder_units(encoder_units);
}

Motor::Motor(const std::uint8_t port, const motor_gearset_e_t gearset, const bool reverse) : _port(port) {
	set_gearing(gearset);
	set_reversed(reverse);
}

Motor::Motor(const std::uint8_t port, const motor_gearset_e_t gearset) : _port(port) {
	set_gearing(gearset);
}

Motor::Motor(const std::uint8_t port, const bool reverse) : _port(port) {
	set_reversed(reverse);
}

Motor::Motor(const std::uint8_t port) : _port(port) {}

std::int32_t Motor::operator=(std::int32_t voltage) const {
	return motor_move(_port, voltage);
}

std::int32_t Motor::move(std::int32_t voltage) const {
	return motor_move(_port, voltage);
}

std::int32_t Motor::move_absolute(const double position, const std::int32_t velocity) const {
	return motor_move_absolute(_port, position, velocity);
}

std::int32_t Motor::move_relative(const double position, const std::int32_t velocity) const {
	return motor_move_relative(_port, position, velocity);
}

std::int32_t Motor::move_velocity(const std::int32_t velocity) const {
	return motor_move_velocity(_port, velocity);
}

std::int32_t Motor::move_voltage(const std::int32_t voltage) const {
	return motor_move_voltage(_port, voltage);
}

std::int32_t Motor::modify_profiled_velocity(const std::int32_t velocity) const {
	return motor_modify_profiled_velocity(_port, velocity);
}

double Motor::get_actual_velocity(void) const {
	return motor_get_actual_velocity(_port);
}

motor_brake_mode_e_t Motor::get_brake_mode(void) const {
	return motor_get_brake_mode(_port);
}

std::int32_t Motor::get_current_draw(void) const {
	return motor_get_current_draw(_port);
}

std::int32_t Motor::get_current_limit(void) const {
	return motor_get_current_limit(_port);
}

std::int32_t Motor::is_over_current(void) const {
	return motor_is_over_current(_port);
}

std::int32_t Motor::get_direction(void) const {
	return motor_get_direction(_port);
}

double Motor::get_efficiency(void) const {
	return motor_get_efficiency(_port);
}

motor_encoder_units_e_t Motor::get_encoder_units(void) const {
	return motor_get_encoder_units(_port);
}

std::uint32_t Motor::get_faults(void) const {
	return motor_get_faults(_port);
}

std::uint32_t Motor::get_flags(void) const {
	return motor_get_flags(_port);
}

motor_gearset_e_t Motor::get_gearing(void) const {
	return motor_get_gearing(_port);
}

motor_pid_full_s_t Motor::get_pos_pid(void) const {
	return motor_get_pos_pid(_port);
}

motor_pid_full_s_t Motor::get_vel_pid(void) const {
	return motor_get_vel_pid(_port);
}

std::int32_t Motor::get_raw_position(std::uint32_t* const timestamp) const {
	return motor_get_raw_position(_port, timestamp);
}

std::int32_t Motor::is_over_temp(void) const {
	return motor_is_over_temp(_port);
}

std::int32_t Motor::is_stopped(void) const {
	return motor_is_stopped(_port);
}

std::int32_t Motor::get_zero_position_flag(void) const {
	return motor_get_zero_position_flag(_port);
}

double Motor::get_position(void) const {
	return motor_get_position(_port);
}

double Motor::get_power(void) const {
	return motor_get_power(_port);
}

std::int32_t Motor::is_reversed(void) const {
	return motor_is_reversed(_port);
}

double Motor::get_temperature(void) const {
	return motor_get_temperature(_port);
}

double Motor::get_target_position(void) const {
	return motor_get_target_position(_port);
}

double Motor::get_torque(void) const {
	return motor_get_torque(_port);
}

std::int32_t Motor::get_target_velocity(void) const {
	return motor_get_target_velocity(_port);
}

std::int32_t Motor::get_voltage(void) const {
	return motor_get_voltage(_port);
}

std::int32_t Motor::get_voltage_limit(void) const {
	return motor_get_voltage_limit(_port);
}

std::uint8_t Motor::get_port(void) const {
	return _port;
}

std::int32_t Motor::tare_position(void) const {
	return motor_tare_position(_port);
}

std::int32_t Motor::set_brake_mode(const motor_brake_mode_e_t mode) const {
	return motor_set_brake_mode(_port, mode);
}

std::int32_t Motor::set_current_limit(const std::int32_t limit) const {
	return motor_set_current_limit(_port, limit);
}

std::int32_t Motor::set_encoder_units(const motor_encoder_units_e_t units) const {
	return motor_set_encoder_units(_port, units);
}

std::int32_t Motor::set_gearing(const motor_gearset_e_t gearset) const {
	return motor_set_gearing(_port, gearset);
}

motor_pid_s_t Motor::convert_pid(double kf, double kp, double ki, double kd) {
	return motor_convert_pid(kf, kp, ki, kd);
}

motor_pid_full_s_t Motor::convert_pid_full(double kf, double kp, double ki, double kd, double filter, double limit,
                                           double threshold, double loopspeed) {
	return motor_convert_pid_full(kf, kp, ki, kd, filter, limit, threshold, loopspeed);
}

std::int32_t Motor::set_pos_pid(const motor_pid_s_t pid) const {
	return motor_set_pos_pid(_port, pid);
}

std::int32_t Motor::set_pos_pid_full(const motor_pid_full_s_t pid) const {
	return motor_set_pos_pid_full(_port, pid);
}

std::int32_t Motor::set_vel_pid(const motor_pid_s_t pid) const {
	return motor_set_vel_pid(_port, pid);
}

std::int32_t Motor::set_vel_pid_full(const motor_pid_full_s_t pid) const {
	return motor_set_vel_pid_full(_port, pid);
}

std::int32_t Motor::set_zero_position(const double position) const {
	return motor_set_zero_position(_port, position);
}

std::int32_t Motor::set_reversed(const bool reverse) const {
	return motor_set_reversed(_port, reverse);
}

std::int32_t Motor::set_voltage_limit(const std::int32_t limit) const {
	return motor_set_voltage_limit(_port, limit);
}

namespace literals {
const pros::Motor operator"" _mtr(const unsigned long long int m) {
	return pros::Motor(m, false);
}
const pros::Motor operator"" _rmtr(const unsigned long long int m) {
	return pros::Motor(m, true);
}
}  // namespace literals
}  // namespace pros
