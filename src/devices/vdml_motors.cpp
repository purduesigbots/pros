/**
 * \file devices/vdml_motors.cpp
 *
 * Contains functions for interacting with the V5 Motors.
 *
 * Copyright (c) 2017-2022, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "kapi.h"
#include "pros/motors.hpp"

namespace pros {
inline namespace v5 {
using namespace pros::c;

#define empty_motor_group_check(error) \
	if (_ports.size() <= 0) {            \
		errno = EDOM;                      \
		return error;                      \
	}

#define empty_motor_group_check_vector(error, vector) \
	if (_ports.size() <= 0) {                           \
		errno = EDOM;                                     \
		vector.emplace_back(error);                       \
		return vector;                                    \
	}

Motor_Group::Motor_Group(const std::initializer_list<std::int8_t> ports, const pros::v5::Motor_Gears gearset,
                         const bool reverse, const pros::v5::Motor_Units encoder_units)
    : _ports(ports) {
	set_gearing(gearset);
	set_reversed(reverse);
	set_encoder_units(encoder_units);
}

Motor_Group::Motor_Group(const std::initializer_list<std::int8_t> ports, const pros::Color gearset_color,
                         const bool reverse, const pros::v5::Motor_Units encoder_units)
    : _ports(ports) {
	set_gearing(gearset_color);
	set_reversed(reverse);
	set_encoder_units(encoder_units);
}

Motor_Group::Motor_Group(const std::initializer_list<std::int8_t> ports, const pros::v5::Motor_Gears gearset,
                         const bool reverse)
    : _ports(ports) {
	set_gearing(gearset);
	set_reversed(reverse);
}

Motor_Group::Motor_Group(const std::initializer_list<std::int8_t> ports, const pros::Color gearset_color,
                         const bool reverse)
    : _ports(ports) {
	set_gearing(gearset_color);
	set_reversed(reverse);
}

Motor_Group::Motor_Group(const std::initializer_list<std::int8_t> ports, const pros::v5::Motor_Gears gearset)
    : _ports(ports) {
	set_gearing(gearset);
}

Motor_Group::Motor_Group(const std::initializer_list<std::int8_t> ports, const pros::Color gearset_color)
    : _ports(ports) {
	set_gearing(gearset_color);
}

Motor_Group::Motor_Group(const std::initializer_list<std::int8_t> ports, const bool reverse) : _ports(ports) {
	set_reversed(reverse);
}

Motor_Group::Motor_Group(const std::initializer_list<std::int8_t> ports) : _ports(ports) {}
Motor_Group::Motor_Group(const std::vector<std::int8_t> ports, const pros::v5::Motor_Gears gearset, const bool reverse,
                         const pros::v5::Motor_Units encoder_units)
    : _ports(ports) {
	set_gearing(gearset);
	set_reversed(reverse);
	set_encoder_units(encoder_units);
}

Motor_Group::Motor_Group(const std::vector<std::int8_t> ports, const pros::Color gearset_color, const bool reverse,
                         const pros::v5::Motor_Units encoder_units)
    : _ports(ports) {
	set_gearing(gearset_color);
	set_reversed(reverse);
	set_encoder_units(encoder_units);
}

Motor_Group::Motor_Group(const std::vector<std::int8_t> ports, const pros::v5::Motor_Gears gearset, const bool reverse)
    : _ports(ports) {
	set_gearing(gearset);
	set_reversed(reverse);
}

Motor_Group::Motor_Group(const std::vector<std::int8_t> ports, const pros::Color gearset_color, const bool reverse)
    : _ports(ports) {
	set_gearing(gearset_color);
	set_reversed(reverse);
}

Motor_Group::Motor_Group(const std::vector<std::int8_t> ports, const pros::v5::Motor_Gears gearset) : _ports(ports) {
	set_gearing(gearset);
}

Motor_Group::Motor_Group(const std::vector<std::int8_t> ports, const pros::Color gearset_color) : _ports(ports) {
	set_gearing(gearset_color);
}

Motor_Group::Motor_Group(const std::vector<std::int8_t> ports, const bool reverse) : _ports(ports) {
	set_reversed(reverse);
}

Motor_Group::Motor_Group(const std::vector<std::int8_t> ports) : _ports(ports) {}

std::int32_t Motor_Group::operator=(std::int32_t voltage) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (int i = 1; i < _ports.size(); i++) {
		motor_move(_ports[i], voltage);
	}
	return motor_move(_ports[0], voltage);
}

std::int32_t Motor_Group::move(std::int32_t voltage) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (int i = 1; i < _ports.size(); i++) {
		motor_move(_ports[i], voltage);
	}
	return motor_move(_ports[0], voltage);
}

std::int32_t Motor_Group::move_absolute(const double position, const std::int32_t velocity) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (int i = 1; i < _ports.size(); i++) {
		motor_move_absolute(_ports[i], position, velocity);
	}
	return motor_move_absolute(_ports[0], position, velocity);
}

std::int32_t Motor_Group::move_relative(const double position, const std::int32_t velocity) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (int i = 1; i < _ports.size(); i++) {
		motor_move_relative(_ports[i], position, velocity);
	}
	return motor_move_relative(_ports[0], position, velocity);
}

std::int32_t Motor_Group::move_velocity(const std::int32_t velocity) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (int i = 1; i < _ports.size(); i++) {
		motor_move_velocity(_ports[i], velocity);
	}
	return motor_move_velocity(_ports[0], velocity);
}

std::int32_t Motor_Group::move_voltage(const std::int32_t voltage) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (int i = 1; i < _ports.size(); i++) {
		motor_move_voltage(_ports[i], voltage);
	}
	return motor_move_voltage(_ports[0], voltage);
}

std::int32_t Motor_Group::brake(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (int i = 1; i < _ports.size(); i++) {
		motor_brake(_ports[i]);
	}
	return motor_brake(_ports[0]);
}

std::int32_t Motor_Group::modify_profiled_velocity(const std::int32_t velocity) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (int i = 1; i < _ports.size(); i++) {
		motor_modify_profiled_velocity(_ports[i], velocity);
	}
	return motor_modify_profiled_velocity(_ports[0], velocity);
}

double Motor_Group::get_actual_velocity(void) const {
	empty_motor_group_check(PROS_ERR_F);
	push_motor_configuration();
	return motor_get_actual_velocity(_ports[0]);
}

std::vector<double> Motor_Group::get_every_actual_velocity(void) const {
	std::vector<double> return_vector;
	empty_motor_group_check_vector(PROS_ERR_F, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back(motor_get_actual_velocity(_ports[i]));
	}
	return return_vector;
}

pros::v5::Motor_Brake Motor_Group::get_brake_mode(void) const {
	empty_motor_group_check(pros::v5::Motor_Brake::invalid);
	push_motor_configuration();
	return static_cast<pros::v5::Motor_Brake>(motor_get_brake_mode(_ports[0]));
}

std::vector<pros::v5::Motor_Brake> Motor_Group::get_every_brake_mode(void) const {
	std::vector<pros::v5::Motor_Brake> return_vector;
	empty_motor_group_check_vector(pros::v5::Motor_Brake::invalid, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back(static_cast<pros::v5::Motor_Brake>(motor_get_brake_mode(_ports[i])));
	}
	return return_vector;
}

std::int32_t Motor_Group::get_current_draw(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	return motor_get_current_draw(_ports[0]);
}
std::vector<std::int32_t> Motor_Group::get_every_current_draw(void) const {
	std::vector<std::int32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back(motor_get_current_draw(_ports[i]));
	}
	return return_vector;
}
std::int32_t Motor_Group::get_current_limit(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	return motor_get_current_limit(_ports[0]);
}
std::vector<std::int32_t> Motor_Group::get_every_current_limit(void) const {
	std::vector<std::int32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back(motor_get_current_limit(_ports[i]));
	}
	return return_vector;
}
std::int32_t Motor_Group::is_over_current(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	return motor_is_over_current(_ports[0]);
}

std::vector<std::int32_t> Motor_Group::every_is_over_current(void) const {
	std::vector<std::int32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back(motor_is_over_current(_ports[i]));
	}
	return return_vector;
}

std::int32_t Motor_Group::get_direction(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	return motor_get_direction(_ports[0]);
}
std::vector<std::int32_t> Motor_Group::get_every_direction(void) const {
	std::vector<std::int32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back(motor_get_direction(_ports[i]));
	}
	return return_vector;
}

double Motor_Group::get_efficiency(void) const {
	empty_motor_group_check(PROS_ERR_F);
	push_motor_configuration();
	return motor_get_efficiency(_ports[0]);
}
std::vector<double> Motor_Group::get_every_efficiency(void) const {
	std::vector<double> return_vector;
	empty_motor_group_check_vector(PROS_ERR_F, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back(motor_get_efficiency(_ports[i]));
	}
	return return_vector;
}

pros::v5::Motor_Units Motor_Group::get_encoder_units(void) const {
	return _encoder_units;
}

std::vector<pros::v5::Motor_Units> Motor_Group::get_every_encoder_units(void) const {
	std::vector<pros::v5::Motor_Units> return_vector;
	empty_motor_group_check_vector(pros::v5::Motor_Units::invalid, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back(static_cast<pros::v5::Motor_Units>(motor_get_encoder_units(_ports[i])));
	}
	return return_vector;
}

std::uint32_t Motor_Group::get_faults(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	return motor_get_faults(_ports[0]);
}

std::vector<std::uint32_t> Motor_Group::get_every_faults(void) const {
	std::vector<std::uint32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back(motor_get_faults(_ports[i]));
	}
	return return_vector;
}

std::uint32_t Motor_Group::get_flags(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	return motor_get_flags(_ports[0]);
}

std::vector<std::uint32_t> Motor_Group::get_every_flags(void) const {
	std::vector<std::uint32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back(motor_get_flags(_ports[i]));
	}
	return return_vector;
}

pros::v5::Motor_Gears Motor_Group::get_gearing(void) const {
	return _gearset;
}

std::vector<pros::v5::Motor_Gears> Motor_Group::get_every_gearing(void) const {
	std::vector<pros::v5::Motor_Gears> return_vector;
	empty_motor_group_check_vector(pros::v5::Motor_Gears::invalid, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back(static_cast<pros::v5::Motor_Gears>(motor_get_gearing(_ports[i])));
	}
	return return_vector;
}

std::int32_t Motor_Group::get_raw_position(std::uint32_t* const timestamp) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	return motor_get_raw_position(_ports[0], timestamp);
}

std::vector<std::int32_t> Motor_Group::get_every_raw_position(std::uint32_t* const timestamp) const {
	std::vector<std::int32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back(motor_get_raw_position(_ports[i], timestamp));
	}
	return return_vector;
}
std::int32_t Motor_Group::is_over_temp(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	return motor_is_over_temp(_ports[0]);
}

std::vector<std::int32_t> Motor_Group::every_is_over_temp(void) const {
	std::vector<std::int32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back(motor_is_over_temp(_ports[i]));
	}
	return return_vector;
}

double Motor_Group::get_position(void) const {
	empty_motor_group_check(PROS_ERR_F);
	push_motor_configuration();
	return motor_get_position(_ports[0]);
}
std::vector<double> Motor_Group::get_every_position(void) const {
	std::vector<double> return_vector;
	empty_motor_group_check_vector(PROS_ERR_F, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back(motor_get_position(_ports[i]));
	}
	return return_vector;
}

double Motor_Group::get_power(void) const {
	empty_motor_group_check(PROS_ERR_F);
	push_motor_configuration();
	return motor_get_power(_ports[0]);
}

std::vector<double> Motor_Group::get_every_power(void) const {
	std::vector<double> return_vector;
	empty_motor_group_check_vector(PROS_ERR_F, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back(motor_get_power(_ports[i]));
	}
	return return_vector;
}
std::int32_t Motor_Group::is_reversed(void) const {
	empty_motor_group_check(PROS_ERR);
	return _reverse;
}

std::vector<std::int32_t> Motor_Group::every_is_reversed(void) const {
	std::vector<std::int32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back(motor_is_reversed(_ports[i]));
	}
	return return_vector;
}

double Motor_Group::get_temperature(void) const {
	empty_motor_group_check(PROS_ERR_F);
	push_motor_configuration();
	return motor_get_temperature(_ports[0]);
}

std::vector<double> Motor_Group::get_every_temperature(void) const {
	std::vector<double> return_vector;
	empty_motor_group_check_vector(PROS_ERR_F, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back(motor_get_temperature(_ports[i]));
	}
	return return_vector;
}

double Motor_Group::get_target_position(void) const {
	empty_motor_group_check(PROS_ERR_F);
	push_motor_configuration();
	return motor_get_target_position(_ports[0]);
}
std::vector<double> Motor_Group::get_every_target_position(void) const {
	std::vector<double> return_vector;
	empty_motor_group_check_vector(PROS_ERR_F, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back(motor_get_target_position(_ports[i]));
	}
	return return_vector;
}

double Motor_Group::get_torque(void) const {
	empty_motor_group_check(PROS_ERR_F);
	push_motor_configuration();
	return motor_get_torque(_ports[0]);
}
std::vector<double> Motor_Group::get_every_torque(void) const {
	std::vector<double> return_vector;
	empty_motor_group_check_vector(PROS_ERR_F, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back(motor_get_torque(_ports[i]));
	}
	return return_vector;
}

std::int32_t Motor_Group::get_target_velocity(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	return motor_get_target_velocity(_ports[0]);
}
std::vector<std::int32_t> Motor_Group::get_every_target_velocity(void) const {
	std::vector<std::int32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back(motor_get_target_velocity(_ports[i]));
	}
	return return_vector;
}

std::int32_t Motor_Group::get_voltage(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	return motor_get_voltage(_ports[0]);
}

std::vector<std::int32_t> Motor_Group::get_every_voltage(void) const {
	std::vector<std::int32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back(motor_get_voltage(_ports[i]));
	}
	return return_vector;
}
std::int32_t Motor_Group::get_voltage_limit(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	return motor_get_voltage_limit(_ports[0]);
}

std::vector<std::int32_t> Motor_Group::get_every_voltage_limit(void) const {
	std::vector<std::int32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back(motor_get_voltage_limit(_ports[i]));
	}
	return return_vector;
}
std::uint8_t Motor_Group::get_port(void) const {
	empty_motor_group_check(PROS_ERR_BYTE);
	return (uint8_t)std::abs(_ports[0]);
}

std::vector<std::uint8_t> Motor_Group::get_every_port(void) const {
	std::vector<std::uint8_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR_BYTE, return_vector);
	push_motor_configuration();
	for (int i = 0; i < _ports.size(); i++) {
		return_vector.emplace_back((uint8_t)std::abs((_ports[i])));
	}
	return return_vector;
}

std::int32_t Motor_Group::tare_position(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (int i = 1; i < _ports.size(); i++) {
		motor_tare_position(_ports[i]);
	}
	return motor_tare_position(_ports[0]);
}

std::int32_t Motor_Group::set_brake_mode(const pros::motor_brake_mode_e_t mode) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (int i = 1; i < _ports.size(); i++) {
		motor_set_brake_mode(_ports[i], mode);
	}
	return motor_set_brake_mode(_ports[0], mode);
}

std::int32_t Motor_Group::set_brake_mode(const pros::v5::Motor_Brake mode) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (int i = 1; i < _ports.size(); i++) {
		motor_set_brake_mode(_ports[i], static_cast<pros::motor_brake_mode_e_t>(mode));
	}
	return motor_set_brake_mode(_ports[0], static_cast<pros::motor_brake_mode_e_t>(mode));
}

std::int32_t Motor_Group::set_current_limit(const std::int32_t limit) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (int i = 1; i < _ports.size(); i++) {
		motor_set_current_limit(_ports[i], limit);
	}
	return motor_set_current_limit(_ports[0], limit);
}

std::int32_t Motor_Group::set_encoder_units(const pros::motor_encoder_units_e_t units) const {
	empty_motor_group_check(PROS_ERR);
	_encoder_units = static_cast<pros::v5::Motor_Units>(units);
	for (int i = 1; i < _ports.size(); i++) {
		motor_set_encoder_units(_ports[i], units);
	}
	return motor_set_encoder_units(_ports[0], units);
}

std::int32_t Motor_Group::set_encoder_units(const pros::v5::Motor_Units units) const {
	empty_motor_group_check(PROS_ERR);
	_encoder_units = units;
	for (int i = 1; i < _ports.size(); i++) {
		motor_set_encoder_units(_ports[i], static_cast<motor_encoder_units_e_t>(units));
	}
	return motor_set_encoder_units(_ports[0], static_cast<motor_encoder_units_e_t>(units));
}

std::int32_t Motor_Group::set_gearing(const motor_gearset_e_t gearset) const {
	empty_motor_group_check(PROS_ERR);
	_gearset = static_cast<pros::v5::Motor_Gear>(gearset);
	for (int i = 1; i < _ports.size(); i++) {
		motor_set_gearing(_ports[i], gearset);
	}
	return motor_set_gearing(_ports[0], gearset);
}

std::int32_t Motor_Group::set_gearing(const pros::v5::Motor_Gear gearset) const {
	empty_motor_group_check(PROS_ERR);
	_gearset = gearset;
	for (int i = 1; i < _ports.size(); i++) {
		motor_set_gearing(_ports[i], (motor_gearset_e_t)gearset);
	}
	return motor_set_gearing(_ports[0], (motor_gearset_e_t)gearset);
}

std::int32_t Motor_Group::set_gearing(const pros::Color gearset_color) const {
	empty_motor_group_check(PROS_ERR);
	_gearset = static_cast<pros::v5::Motor_Gear>(gearset_color);
	for (int i = 1; i < _ports.size(); i++) {
		motor_set_gearing(_ports[i], (motor_gearset_e_t)_gearset);
	}
	return motor_set_gearing(_ports[0], (motor_gearset_e_t)_gearset);
}

std::int32_t Motor_Group::set_zero_position(const double position) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (int i = 1; i < _ports.size(); i++) {
		motor_set_zero_position(_ports[i], position);
	}
	return motor_set_zero_position(_ports[0], position);
}

std::int32_t Motor_Group::set_reversed(const bool reverse) const {
	empty_motor_group_check(PROS_ERR);
	_reverse = reverse;
	for (int i = 1; i < _ports.size(); i++) {
		motor_set_reversed(_ports[i], reverse ^ _ports[i] < 0);
	}

	return motor_set_reversed(_ports[0], reverse ^ _ports[0] < 0);
}

std::int32_t Motor_Group::set_voltage_limit(const std::int32_t limit) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (int i = 1; i < _ports.size(); i++) {
		motor_set_voltage_limit(_ports[i], limit);
	}
	return motor_set_voltage_limit(_ports[0], limit);
}

std::vector<std::int8_t> Motor_Group::get_ports() {
	return _ports;
}

std::int8_t Motor_Group::size() const {
	return _ports.size();
}

void Motor_Group::operator+=(Motor_Group& other) {
	for (int i = 0; i < other._ports.size(); i++) {
		_ports.emplace_back(other._ports[i]);
	}
}

void Motor_Group::append(Motor_Group& other) {
	(*this) += other;
}

void Motor_Group::erase_port(std::int8_t port) {
	int i = 0;
	while (i < _ports.size()) {
		if (std::abs(_ports[i]) == port) {
			_ports.erase(_ports.begin() + i);
		} else {
			i++;
		}
	}
}

std::ostream& operator<<(std::ostream& os, const pros::Motor& motor) {
	os << "Motor [";
	os << "port: " << motor.get_port();
	os << ", brake mode: " << (int)motor.get_brake_mode();
	os << ", current draw: " << motor.get_current_draw();
	os << ", current limit: " << motor.get_current_limit();
	os << ", direction: " << motor.get_direction();
	os << ", efficiency: " << motor.get_efficiency();
	os << ", encoder units: " << (int)motor.get_encoder_units();
	os << ", gearing: " << (int)motor.get_gearing();
	os << ", over temp: " << motor.is_over_temp();
	os << ", position: " << motor.get_position();
	os << ", reversed: " << motor.is_reversed();
	os << ", temperature: " << motor.get_temperature();
	os << ", torque: " << motor.get_torque();
	os << ", voltage: " << motor.get_voltage();
	os << "]";
	return os;
}

void Motor_Group::push_motor_configuration() const {
	_motor_group_mutex.take();
	set_gearing(_gearset);
	set_reversed(_reverse);
	set_encoder_units(_encoder_units);
	_motor_group_mutex.give();
}

Motor::Motor(const std::int8_t port) : Motor_Group({port}) {}

Motor::Motor(const std::int8_t port, const pros::v5::Motor_Gears gearset, const bool reverse,
             const pros::v5::Motor_Units encoder_units)
    : Motor_Group({port}, gearset, reverse, encoder_units) {}

Motor::Motor(const std::int8_t port, const pros::Color gearset_color, const bool reverse,
             const pros::v5::Motor_Units encoder_units)
    : Motor_Group({port}, gearset_color, reverse, encoder_units) {}

Motor::Motor(const std::int8_t port, const pros::v5::Motor_Gears gearset, const bool reverse)
    : Motor_Group({port}, gearset, reverse) {}

Motor::Motor(const std::int8_t port, const pros::Color gearset_color, const bool reverse)
    : Motor_Group({port}, gearset_color, reverse) {}

Motor::Motor(const std::int8_t port, const pros::v5::Motor_Gears gearset) : Motor_Group({port}, gearset) {}

Motor::Motor(const std::int8_t port, const pros::Color gearset_color) : Motor_Group({port}, gearset_color) {}

Motor::Motor(const std::int8_t port, const bool reverse) : Motor_Group({port}, reverse) {}

}  // namespace v5
namespace literals {
const pros::Motor operator"" _mtr(const unsigned long long int m) {
	return pros::Motor(m, false);
}
const pros::Motor operator"" _rmtr(const unsigned long long int m) {
	return pros::Motor(m, true);
}

}  // namespace literals
}  // namespace pros
