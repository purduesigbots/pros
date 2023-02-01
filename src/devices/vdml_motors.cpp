/**
 * \file devices/vdml_motors.cpp
 *
 * Contains functions for interacting with the V5 Motors.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
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
#define motor_group_index_check(error, index) \
	if (_ports.size() <= index || index < 0) {  \
		errno = EOVERFLOW;                        \
		return error;                             \
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
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_move(*it, voltage);
	}
	return motor_move(_ports[0], voltage);
}

std::int32_t Motor_Group::move(std::int32_t voltage) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_move(*it, voltage);
	}
	return motor_move(_ports[0], voltage);
}

std::int32_t Motor_Group::move_absolute(const double position, const std::int32_t velocity) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_move_absolute(*it, position, velocity);
	}
	return motor_move_absolute(_ports[0], position, velocity);
}

std::int32_t Motor_Group::move_relative(const double position, const std::int32_t velocity) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_move_relative(*it, position, velocity);
	}
	return motor_move_relative(_ports[0], position, velocity);
}

std::int32_t Motor_Group::move_velocity(const std::int32_t velocity) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_move_velocity(*it, velocity);
	}
	return motor_move_velocity(_ports[0], velocity);
}

std::int32_t Motor_Group::move_voltage(const std::int32_t voltage) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_move_voltage(*it, voltage);
	}
	return motor_move_voltage(_ports[0], voltage);
}

std::int32_t Motor_Group::brake(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_brake(*it);
	}
	return motor_brake(_ports[0]);
}

std::int32_t Motor_Group::modify_profiled_velocity(const std::int32_t velocity) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_modify_profiled_velocity(*it, velocity);
	}
	return motor_modify_profiled_velocity(_ports[0], velocity);
}

double Motor_Group::get_actual_velocity(void) const {
	empty_motor_group_check(PROS_ERR_F);
	push_motor_configuration();
	return motor_get_actual_velocity(_ports[0]);
}

double Motor_Group::get_actual_velocity(std::uint8_t index) const {
	empty_motor_group_check(PROS_ERR_F);
	motor_group_index_check(PROS_ERR_F, index);
	push_motor_configuration();
	return motor_get_actual_velocity(_ports[index]);
}
std::vector<double> Motor_Group::get_every_actual_velocity(void) const {
	std::vector<double> return_vector;
	empty_motor_group_check_vector(PROS_ERR_F, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_actual_velocity(*it));
	}
	return return_vector;
}

pros::v5::Motor_Brake Motor_Group::get_brake_mode(void) const {
	empty_motor_group_check(pros::v5::Motor_Brake::invalid);
	push_motor_configuration();
	return static_cast<pros::v5::Motor_Brake>(motor_get_brake_mode(_ports[0]));
}
pros::v5::Motor_Brake Motor_Group::get_brake_mode(std::uint8_t index) const {
	empty_motor_group_check(pros::v5::Motor_Brake::invalid);
	motor_group_index_check(pros::v5::Motor_Brake::invalid, index);
	push_motor_configuration();
	return static_cast<pros::v5::Motor_Brake>(motor_get_brake_mode(_ports[index]));
}

std::vector<pros::v5::Motor_Brake> Motor_Group::get_every_brake_mode(void) const {
	std::vector<pros::v5::Motor_Brake> return_vector;
	empty_motor_group_check_vector(pros::v5::Motor_Brake::invalid, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(static_cast<pros::v5::Motor_Brake>(motor_get_brake_mode(*it)));
	}
	return return_vector;
}

std::int32_t Motor_Group::get_current_draw(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	return motor_get_current_draw(_ports[0]);
}
std::int32_t Motor_Group::get_current_draw(std::uint8_t index) const {
	empty_motor_group_check(PROS_ERR);
	motor_group_index_check(PROS_ERR, index);
	push_motor_configuration();
	return motor_get_current_draw(_ports[index]);
}
std::vector<std::int32_t> Motor_Group::get_every_current_draw(void) const {
	std::vector<std::int32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_current_draw(*it));
	}
	return return_vector;
}
std::int32_t Motor_Group::get_current_limit(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	return motor_get_current_limit(_ports[0]);
}
std::int32_t Motor_Group::get_current_limit(std::uint8_t index) const {
	empty_motor_group_check(PROS_ERR);
	motor_group_index_check(PROS_ERR, index);
	push_motor_configuration();
	return motor_get_current_limit(_ports[index]);
}
std::vector<std::int32_t> Motor_Group::get_every_current_limit(void) const {
	std::vector<std::int32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_current_limit(*it));
	}
	return return_vector;
}
std::int32_t Motor_Group::is_over_current(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	return motor_is_over_current(_ports[0]);
}
std::int32_t Motor_Group::is_over_current(std::uint8_t index) const {
	empty_motor_group_check(PROS_ERR);
	motor_group_index_check(PROS_ERR, index);
	push_motor_configuration();
	return motor_is_over_current(_ports[index]);
}

std::vector<std::int32_t> Motor_Group::every_is_over_current(void) const {
	std::vector<std::int32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_is_over_current(*it));
	}
	return return_vector;
}

std::int32_t Motor_Group::get_direction(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	return motor_get_direction(_ports[0]);
}
std::int32_t Motor_Group::get_direction(std::uint8_t index) const {
	empty_motor_group_check(PROS_ERR);
	motor_group_index_check(PROS_ERR, index);
	push_motor_configuration();
	return motor_get_direction(_ports[index]);
}
std::vector<std::int32_t> Motor_Group::get_every_direction(void) const {
	std::vector<std::int32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_direction(*it));
	}
	return return_vector;
}

double Motor_Group::get_efficiency(void) const {
	empty_motor_group_check(PROS_ERR_F);
	push_motor_configuration();
	return motor_get_efficiency(_ports[0]);
}
double Motor_Group::get_efficiency(std::uint8_t index) const {
	empty_motor_group_check(PROS_ERR_F);
	motor_group_index_check(PROS_ERR_F, index);
	push_motor_configuration();
	return motor_get_efficiency(_ports[index]);
}
std::vector<double> Motor_Group::get_every_efficiency(void) const {
	std::vector<double> return_vector;
	empty_motor_group_check_vector(PROS_ERR_F, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_efficiency(*it));
	}
	return return_vector;
}

pros::v5::Motor_Units Motor_Group::get_encoder_units(void) const {
	return _encoder_units;
}
pros::v5::Motor_Units Motor_Group::get_encoder_units(std::uint8_t index) const {
	empty_motor_group_check(pros::v5::Motor_Units::invalid);
	motor_group_index_check(pros::v5::Motor_Units::invalid, index);
	push_motor_configuration();
	return static_cast<pros::v5::Motor_Units>(motor_get_encoder_units(_ports[index]));
}

std::vector<pros::v5::Motor_Units> Motor_Group::get_every_encoder_units(void) const {
	std::vector<pros::v5::Motor_Units> return_vector;
	empty_motor_group_check_vector(pros::v5::Motor_Units::invalid, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(static_cast<pros::v5::Motor_Units>(motor_get_encoder_units(*it)));
	}
	return return_vector;
}

std::uint32_t Motor_Group::get_faults(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	return motor_get_faults(_ports[0]);
}
std::uint32_t Motor_Group::get_faults(std::uint8_t index) const {
	empty_motor_group_check(PROS_ERR);
	motor_group_index_check(PROS_ERR, index);
	push_motor_configuration();
	return motor_get_faults(_ports[index]);
}

std::vector<std::uint32_t> Motor_Group::get_every_faults(void) const {
	std::vector<std::uint32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_faults(*it));
	}
	return return_vector;
}

std::uint32_t Motor_Group::get_flags(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	return motor_get_flags(_ports[0]);
}
std::uint32_t Motor_Group::get_flags(std::uint8_t index) const {
	empty_motor_group_check(PROS_ERR);
	motor_group_index_check(PROS_ERR, index);
	push_motor_configuration();
	return motor_get_flags(_ports[index]);
}

std::vector<std::uint32_t> Motor_Group::get_every_flags(void) const {
	std::vector<std::uint32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_flags(*it));
	}
	return return_vector;
}

pros::v5::Motor_Gears Motor_Group::get_gearing(void) const {
	return _gearset;
}

pros::v5::Motor_Gears Motor_Group::get_gearing(std::uint8_t index) const {
	empty_motor_group_check(pros::v5::Motor_Gears::invalid);
	motor_group_index_check(pros::v5::Motor_Gears::invalid, index);
	push_motor_configuration();
	return static_cast<pros::v5::Motor_Gears>(motor_get_gearing(_ports[index]));
}
std::vector<pros::v5::Motor_Gears> Motor_Group::get_every_gearing(void) const {
	std::vector<pros::v5::Motor_Gears> return_vector;
	empty_motor_group_check_vector(pros::v5::Motor_Gears::invalid, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(static_cast<pros::v5::Motor_Gears>(motor_get_gearing(*it)));
	}
	return return_vector;
}

std::int32_t Motor_Group::get_raw_position(std::uint32_t* const timestamp) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	return motor_get_raw_position(_ports[0], timestamp);
}
std::int32_t Motor_Group::get_raw_position(std::uint32_t* const timestamp, std::uint8_t index) const {
	empty_motor_group_check(PROS_ERR);
	motor_group_index_check(PROS_ERR, index);
	push_motor_configuration();
	return motor_get_raw_position(_ports[index], timestamp);
}

std::vector<std::int32_t> Motor_Group::get_every_raw_position(std::uint32_t* const timestamp) const {
	std::vector<std::int32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_raw_position(*it, timestamp));
	}
	return return_vector;
}
std::int32_t Motor_Group::is_over_temp(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	return motor_is_over_temp(_ports[0]);
}
std::int32_t Motor_Group::is_over_temp(std::uint8_t index) const {
	empty_motor_group_check(PROS_ERR);
	motor_group_index_check(PROS_ERR, index);
	push_motor_configuration();
	return motor_is_over_temp(_ports[index]);
}

std::vector<std::int32_t> Motor_Group::every_is_over_temp(void) const {
	std::vector<std::int32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_is_over_temp(*it));
	}
	return return_vector;
}

double Motor_Group::get_position(void) const {
	empty_motor_group_check(PROS_ERR_F);
	push_motor_configuration();
	return motor_get_position(_ports[0]);
}
double Motor_Group::get_position(std::uint8_t index) const {
	empty_motor_group_check(PROS_ERR_F);
	motor_group_index_check(PROS_ERR_F, index);
	push_motor_configuration();
	return motor_get_position(_ports[index]);
}
std::vector<double> Motor_Group::get_every_position(void) const {
	std::vector<double> return_vector;
	empty_motor_group_check_vector(PROS_ERR_F, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_position(*it));
	}
	return return_vector;
}

double Motor_Group::get_power(void) const {
	empty_motor_group_check(PROS_ERR_F);
	push_motor_configuration();
	return motor_get_power(_ports[0]);
}

double Motor_Group::get_power(std::uint8_t index) const {
	empty_motor_group_check(PROS_ERR_F);
	motor_group_index_check(PROS_ERR_F, index);
	push_motor_configuration();
	return motor_get_power(_ports[index]);
}

std::vector<double> Motor_Group::get_every_power(void) const {
	std::vector<double> return_vector;
	empty_motor_group_check_vector(PROS_ERR_F, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_power(*it));
	}
	return return_vector;
}
std::int32_t Motor_Group::is_reversed(void) const {
	empty_motor_group_check(PROS_ERR);
	return _reverse;
}

std::int32_t Motor_Group::is_reversed(std::uint8_t index) const {
	empty_motor_group_check(PROS_ERR);
	motor_group_index_check(PROS_ERR, index);
	push_motor_configuration();
	return motor_is_reversed(index);
}
std::vector<std::int32_t> Motor_Group::every_is_reversed(void) const {
	std::vector<std::int32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_is_reversed(*it));
	}
	return return_vector;
}

double Motor_Group::get_temperature(void) const {
	empty_motor_group_check(PROS_ERR_F);
	push_motor_configuration();
	return motor_get_temperature(_ports[0]);
}
double Motor_Group::get_temperature(std::uint8_t index) const {
	empty_motor_group_check(PROS_ERR_F);
	motor_group_index_check(PROS_ERR_F, index);
	push_motor_configuration();
	return motor_get_temperature(_ports[index]);
}

std::vector<double> Motor_Group::get_every_temperature(void) const {
	std::vector<double> return_vector;
	empty_motor_group_check_vector(PROS_ERR_F, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_temperature(*it));
	}
	return return_vector;
}

double Motor_Group::get_target_position(void) const {
	empty_motor_group_check(PROS_ERR_F);
	push_motor_configuration();
	return motor_get_target_position(_ports[0]);
}

double Motor_Group::get_target_position(std::uint8_t index) const {
	empty_motor_group_check(PROS_ERR_F);
	motor_group_index_check(PROS_ERR_F, index);
	push_motor_configuration();
	return motor_get_target_position(_ports[index]);
}

std::vector<double> Motor_Group::get_every_target_position(void) const {
	std::vector<double> return_vector;
	empty_motor_group_check_vector(PROS_ERR_F, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_target_position(*it));
	}
	return return_vector;
}

double Motor_Group::get_torque(void) const {
	empty_motor_group_check(PROS_ERR_F);
	push_motor_configuration();
	return motor_get_torque(_ports[0]);
}
double Motor_Group::get_torque(std::uint8_t index) const {
	empty_motor_group_check(PROS_ERR_F);
	motor_group_index_check(PROS_ERR_F, index);
	push_motor_configuration();
	return motor_get_torque(_ports[index]);
}
std::vector<double> Motor_Group::get_every_torque(void) const {
	std::vector<double> return_vector;
	empty_motor_group_check_vector(PROS_ERR_F, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_torque(*it));
	}
	return return_vector;
}

std::int32_t Motor_Group::get_target_velocity(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	return motor_get_target_velocity(_ports[0]);
}
std::int32_t Motor_Group::get_target_velocity(std::uint8_t index) const {
	empty_motor_group_check(PROS_ERR);
	motor_group_index_check(PROS_ERR, index);
	push_motor_configuration();
	return motor_get_target_velocity(_ports[index]);
}
std::vector<std::int32_t> Motor_Group::get_every_target_velocity(void) const {
	std::vector<std::int32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_target_velocity(*it));
	}
	return return_vector;
}

std::int32_t Motor_Group::get_voltage(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	return motor_get_voltage(_ports[0]);
}
std::int32_t Motor_Group::get_voltage(std::uint8_t index) const {
	empty_motor_group_check(PROS_ERR);
	motor_group_index_check(PROS_ERR, index);
	push_motor_configuration();
	return motor_get_voltage(_ports[index]);
}
std::vector<std::int32_t> Motor_Group::get_every_voltage(void) const {
	std::vector<std::int32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_voltage(*it));
	}
	return return_vector;
}
std::int32_t Motor_Group::get_voltage_limit(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	return motor_get_voltage_limit(_ports[0]);
}
std::int32_t Motor_Group::get_voltage_limit(std::uint8_t index) const {
	empty_motor_group_check(PROS_ERR);
	motor_group_index_check(PROS_ERR, index);
	push_motor_configuration();
	return motor_get_voltage_limit(_ports[index]);
}
std::vector<std::int32_t> Motor_Group::get_every_voltage_limit(void) const {
	std::vector<std::int32_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_voltage_limit(*it));
	}
	return return_vector;
}
std::uint8_t Motor_Group::get_port(void) const {
	empty_motor_group_check(PROS_ERR_BYTE);
	return (uint8_t)std::abs(_ports[0]);
}
std::uint8_t Motor_Group::get_port(std::uint8_t index) const {
	empty_motor_group_check(PROS_ERR_BYTE);
	motor_group_index_check(PROS_ERR_BYTE, index);
	return (uint8_t)(_ports[index]);
}
std::vector<std::uint8_t> Motor_Group::get_every_port(void) const {
	std::vector<std::uint8_t> return_vector;
	empty_motor_group_check_vector(PROS_ERR_BYTE, return_vector);
	push_motor_configuration();
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back((uint8_t)std::abs((*it)));
	}
	return return_vector;
}

std::int32_t Motor_Group::tare_position(void) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_tare_position(*it);
	}
	return motor_tare_position(_ports[0]);
}

std::int32_t Motor_Group::set_brake_mode(const pros::motor_brake_mode_e_t mode) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_brake_mode(*it, mode);
	}
	return motor_set_brake_mode(_ports[0], mode);
}

std::int32_t Motor_Group::set_brake_mode(const pros::v5::Motor_Brake mode) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_brake_mode(*it, static_cast<pros::motor_brake_mode_e_t>(mode));
	}
	return motor_set_brake_mode(_ports[0], static_cast<pros::motor_brake_mode_e_t>(mode));
}

std::int32_t Motor_Group::set_current_limit(const std::int32_t limit) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_current_limit(*it, limit);
	}
	return motor_set_current_limit(_ports[0], limit);
}

std::int32_t Motor_Group::set_encoder_units(const pros::motor_encoder_units_e_t units) {
	empty_motor_group_check(PROS_ERR);
	_encoder_units = static_cast<pros::v5::Motor_Units>(units);
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_encoder_units(*it, units);
	}
	return motor_set_encoder_units(_ports[0], units);
}

std::int32_t Motor_Group::set_encoder_units(const pros::v5::Motor_Units units) {
	empty_motor_group_check(PROS_ERR);
	_encoder_units = units;
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_encoder_units(*it, static_cast<motor_encoder_units_e_t>(units));
	}
	return motor_set_encoder_units(_ports[0], static_cast<motor_encoder_units_e_t>(units));
}

std::int32_t Motor_Group::set_gearing(const motor_gearset_e_t gearset) {
	empty_motor_group_check(PROS_ERR);
	_gearset = static_cast<pros::v5::Motor_Gear>(gearset);
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_gearing(*it, gearset);
	}
	return motor_set_gearing(_ports[0], gearset);
}

std::int32_t Motor_Group::set_gearing(const pros::v5::Motor_Gear gearset) {
	empty_motor_group_check(PROS_ERR);
	_gearset = gearset;
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_gearing(*it, (motor_gearset_e_t)gearset);
	}
	return motor_set_gearing(_ports[0], (motor_gearset_e_t)gearset);
}

std::int32_t Motor_Group::set_gearing(const pros::Color gearset_color) {
	empty_motor_group_check(PROS_ERR);
	_gearset = static_cast<pros::v5::Motor_Gear>(gearset_color);
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_gearing(*it, (motor_gearset_e_t)_gearset);
	}
	return motor_set_gearing(_ports[0], (motor_gearset_e_t)_gearset);
}

std::int32_t Motor_Group::set_zero_position(const double position) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_zero_position(*it, position);
	}
	return motor_set_zero_position(_ports[0], position);
}

std::int32_t Motor_Group::set_reversed(const bool reverse) {
	empty_motor_group_check(PROS_ERR);
	_reverse = reverse;
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_reversed(*it, reverse ^ ((*it) < 0));
	}

	return motor_set_reversed(_ports[0], reverse ^ (_ports[0] < 0));
}

std::int32_t Motor_Group::set_voltage_limit(const std::int32_t limit) const {
	empty_motor_group_check(PROS_ERR);
	push_motor_configuration();
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_voltage_limit(*it, limit);
	}
	return motor_set_voltage_limit(_ports[0], limit);
}

std::vector<std::int8_t> Motor_Group::get_ports() const {
	return _ports;
}

std::int8_t Motor_Group::size() const {
	return _ports.size();
}

void Motor_Group::operator+=(Motor_Group& other) {
	for (auto it = other._ports.begin(); it < other._ports.end(); it++) {
		_ports.emplace_back(*it);
	}
}

void Motor_Group::append(Motor_Group& other) {
	(*this) += other;
}

void Motor_Group::erase_port(std::int8_t port) {
	auto it = _ports.begin();
	while (it < _ports.end()) {
		if (std::abs(*it) == port) {
			_ports.erase(it);
		} else {
			it++;
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
	for (auto it = _ports.begin() + 0; it < _ports.end(); it++) {
		motor_set_gearing(*it, (motor_gearset_e_t)_gearset);
		motor_set_reversed(*it, _reverse ^ ((*it) < 0));
		motor_set_encoder_units(*it, (motor_encoder_units_e_t)_encoder_units);
	}
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
