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

#define empty_MotorGroup_check(error) \
	if (_ports.size() <= 0) {           \
		errno = EDOM;                     \
		return error;                     \
	}
#define MotorGroup_index_check(error, index) \
	if (_ports.size() <= index || index < 0) { \
		errno = EOVERFLOW;                       \
		return error;                            \
	}

#define empty_MotorGroup_check_vector(error, vector) \
	if (_ports.size() <= 0) {                          \
		errno = EDOM;                                    \
		vector.emplace_back(error);                      \
		return vector;                                   \
	}

MotorGroup::MotorGroup(const std::initializer_list<std::int8_t> ports,
                       const pros::v5::Motor_Gears gearset = pros::v5::Motor_Gears::green, const bool reverse = false,
                       const pros::v5::Motor_Units encoder_units = pros::v5::Motor_Units::degrees)
    : _ports(ports) {
	set_gearing(gearset);
	set_reversed(reverse);
	set_encoder_units(encoder_units);
}

MotorGroup::MotorGroup(const std::vector<std::int8_t>& ports,
                       const pros::v5::Motor_Gears gearset = pros::v5::Motor_Gears::green, const bool reverse = false,
                       const pros::v5::Motor_Units encoder_units = pros::v5::Motor_Units::degrees)
    : _ports(ports) {
	set_gearing(gearset);
	set_reversed(reverse);
	set_encoder_units(encoder_units);
}

std::int32_t MotorGroup::operator=(std::int32_t voltage) const {
	empty_MotorGroup_check(PROS_ERR);
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_move(*it, voltage);
	}
	return motor_move(_ports[0], voltage);
}

std::int32_t MotorGroup::move(std::int32_t voltage) const {
	empty_MotorGroup_check(PROS_ERR);

	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_move(*it, voltage);
	}
	return motor_move(_ports[0], voltage);
}

std::int32_t MotorGroup::move_absolute(const double position, const std::int32_t velocity) const {
	empty_MotorGroup_check(PROS_ERR);

	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_move_absolute(*it, position, velocity);
	}
	return motor_move_absolute(_ports[0], position, velocity);
}

std::int32_t MotorGroup::move_relative(const double position, const std::int32_t velocity) const {
	empty_MotorGroup_check(PROS_ERR);

	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_move_relative(*it, position, velocity);
	}
	return motor_move_relative(_ports[0], position, velocity);
}

std::int32_t MotorGroup::move_velocity(const std::int32_t velocity) const {
	empty_MotorGroup_check(PROS_ERR);

	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_move_velocity(*it, velocity);
	}
	return motor_move_velocity(_ports[0], velocity);
}

std::int32_t MotorGroup::move_voltage(const std::int32_t voltage) const {
	empty_MotorGroup_check(PROS_ERR);

	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_move_voltage(*it, voltage);
	}
	return motor_move_voltage(_ports[0], voltage);
}

std::int32_t MotorGroup::brake(void) const {
	empty_MotorGroup_check(PROS_ERR);

	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_brake(*it);
	}
	return motor_brake(_ports[0]);
}

std::int32_t MotorGroup::modify_profiled_velocity(const std::int32_t velocity) const {
	empty_MotorGroup_check(PROS_ERR);

	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_modify_profiled_velocity(*it, velocity);
	}
	return motor_modify_profiled_velocity(_ports[0], velocity);
}

double MotorGroup::get_actual_velocity(void) const {
	empty_MotorGroup_check(PROS_ERR_F);

	return motor_get_actual_velocity(_ports[0]);
}

double MotorGroup::get_actual_velocity(std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR_F);
	MotorGroup_index_check(PROS_ERR_F, index);

	return motor_get_actual_velocity(_ports[index]);
}
std::vector<double> MotorGroup::get_every_actual_velocity(void) const {
	std::vector<double> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR_F, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_actual_velocity(*it));
	}
	return return_vector;
}

pros::v5::Motor_Brake MotorGroup::get_brake_mode(void) const {
	empty_MotorGroup_check(pros::v5::Motor_Brake::invalid);

	return static_cast<pros::v5::Motor_Brake>(motor_get_brake_mode(_ports[0]));
}
pros::v5::Motor_Brake MotorGroup::get_brake_mode(std::uint8_t index) const {
	empty_MotorGroup_check(pros::v5::Motor_Brake::invalid);
	MotorGroup_index_check(pros::v5::Motor_Brake::invalid, index);

	return static_cast<pros::v5::Motor_Brake>(motor_get_brake_mode(_ports[index]));
}

std::vector<pros::v5::Motor_Brake> MotorGroup::get_every_brake_mode(void) const {
	std::vector<pros::v5::Motor_Brake> return_vector;
	empty_MotorGroup_check_vector(pros::v5::Motor_Brake::invalid, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(static_cast<pros::v5::Motor_Brake>(motor_get_brake_mode(*it)));
	}
	return return_vector;
}

std::int32_t MotorGroup::get_current_draw(void) const {
	empty_MotorGroup_check(PROS_ERR);

	return motor_get_current_draw(_ports[0]);
}
std::int32_t MotorGroup::get_current_draw(std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);

	return motor_get_current_draw(_ports[index]);
}
std::vector<std::int32_t> MotorGroup::get_every_current_draw(void) const {
	std::vector<std::int32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_current_draw(*it));
	}
	return return_vector;
}
std::int32_t MotorGroup::get_current_limit(void) const {
	empty_MotorGroup_check(PROS_ERR);

	return motor_get_current_limit(_ports[0]);
}
std::int32_t MotorGroup::get_current_limit(std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);

	return motor_get_current_limit(_ports[index]);
}
std::vector<std::int32_t> MotorGroup::get_every_current_limit(void) const {
	std::vector<std::int32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_current_limit(*it));
	}
	return return_vector;
}
std::int32_t MotorGroup::is_over_current(void) const {
	empty_MotorGroup_check(PROS_ERR);

	return motor_is_over_current(_ports[0]);
}
std::int32_t MotorGroup::is_over_current(std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);

	return motor_is_over_current(_ports[index]);
}

std::vector<std::int32_t> MotorGroup::every_is_over_current(void) const {
	std::vector<std::int32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_is_over_current(*it));
	}
	return return_vector;
}

std::int32_t MotorGroup::get_direction(void) const {
	empty_MotorGroup_check(PROS_ERR);

	return motor_get_direction(_ports[0]);
}
std::int32_t MotorGroup::get_direction(std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);

	return motor_get_direction(_ports[index]);
}
std::vector<std::int32_t> MotorGroup::get_every_direction(void) const {
	std::vector<std::int32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_direction(*it));
	}
	return return_vector;
}

double MotorGroup::get_efficiency(void) const {
	empty_MotorGroup_check(PROS_ERR_F);

	return motor_get_efficiency(_ports[0]);
}
double MotorGroup::get_efficiency(std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR_F);
	MotorGroup_index_check(PROS_ERR_F, index);

	return motor_get_efficiency(_ports[index]);
}
std::vector<double> MotorGroup::get_every_efficiency(void) const {
	std::vector<double> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR_F, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_efficiency(*it));
	}
	return return_vector;
}

pros::v5::Motor_Units MotorGroup::get_encoder_units(void) const {
	return _encoder_units;
}
pros::v5::Motor_Units MotorGroup::get_encoder_units(std::uint8_t index) const {
	empty_MotorGroup_check(pros::v5::Motor_Units::invalid);
	MotorGroup_index_check(pros::v5::Motor_Units::invalid, index);

	return static_cast<pros::v5::Motor_Units>(motor_get_encoder_units(_ports[index]));
}

std::vector<pros::v5::Motor_Units> MotorGroup::get_every_encoder_units(void) const {
	std::vector<pros::v5::Motor_Units> return_vector;
	empty_MotorGroup_check_vector(pros::v5::Motor_Units::invalid, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(static_cast<pros::v5::Motor_Units>(motor_get_encoder_units(*it)));
	}
	return return_vector;
}

std::uint32_t MotorGroup::get_faults(void) const {
	empty_MotorGroup_check(PROS_ERR);

	return motor_get_faults(_ports[0]);
}
std::uint32_t MotorGroup::get_faults(std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);

	return motor_get_faults(_ports[index]);
}

std::vector<std::uint32_t> MotorGroup::get_every_faults(void) const {
	std::vector<std::uint32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_faults(*it));
	}
	return return_vector;
}

std::uint32_t MotorGroup::get_flags(void) const {
	empty_MotorGroup_check(PROS_ERR);

	return motor_get_flags(_ports[0]);
}
std::uint32_t MotorGroup::get_flags(std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);

	return motor_get_flags(_ports[index]);
}

std::vector<std::uint32_t> MotorGroup::get_every_flags(void) const {
	std::vector<std::uint32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_flags(*it));
	}
	return return_vector;
}

pros::v5::Motor_Gears MotorGroup::get_gearing(void) const {
	return _gearset;
}

pros::v5::Motor_Gears MotorGroup::get_gearing(std::uint8_t index) const {
	empty_MotorGroup_check(pros::v5::Motor_Gears::invalid);
	MotorGroup_index_check(pros::v5::Motor_Gears::invalid, index);

	return static_cast<pros::v5::Motor_Gears>(motor_get_gearing(_ports[index]));
}
std::vector<pros::v5::Motor_Gears> MotorGroup::get_every_gearing(void) const {
	std::vector<pros::v5::Motor_Gears> return_vector;
	empty_MotorGroup_check_vector(pros::v5::Motor_Gears::invalid, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(static_cast<pros::v5::Motor_Gears>(motor_get_gearing(*it)));
	}
	return return_vector;
}

std::int32_t MotorGroup::get_raw_position(std::uint32_t* const timestamp) const {
	empty_MotorGroup_check(PROS_ERR);

	return motor_get_raw_position(_ports[0], timestamp);
}
std::int32_t MotorGroup::get_raw_position(std::uint32_t* const timestamp, std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);

	return motor_get_raw_position(_ports[index], timestamp);
}

std::vector<std::int32_t> MotorGroup::get_every_raw_position(std::uint32_t* const timestamp) const {
	std::vector<std::int32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_raw_position(*it, timestamp));
	}
	return return_vector;
}
std::int32_t MotorGroup::is_over_temp(void) const {
	empty_MotorGroup_check(PROS_ERR);

	return motor_is_over_temp(_ports[0]);
}
std::int32_t MotorGroup::is_over_temp(std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);

	return motor_is_over_temp(_ports[index]);
}

std::vector<std::int32_t> MotorGroup::every_is_over_temp(void) const {
	std::vector<std::int32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_is_over_temp(*it));
	}
	return return_vector;
}

double MotorGroup::get_position(void) const {
	empty_MotorGroup_check(PROS_ERR_F);

	return motor_get_position(_ports[0]);
}
double MotorGroup::get_position(std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR_F);
	MotorGroup_index_check(PROS_ERR_F, index);

	return motor_get_position(_ports[index]);
}
std::vector<double> MotorGroup::get_every_position(void) const {
	std::vector<double> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR_F, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_position(*it));
	}
	return return_vector;
}

double MotorGroup::get_power(void) const {
	empty_MotorGroup_check(PROS_ERR_F);

	return motor_get_power(_ports[0]);
}

double MotorGroup::get_power(std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR_F);
	MotorGroup_index_check(PROS_ERR_F, index);

	return motor_get_power(_ports[index]);
}

std::vector<double> MotorGroup::get_every_power(void) const {
	std::vector<double> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR_F, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_power(*it));
	}
	return return_vector;
}
std::int32_t MotorGroup::is_reversed(void) const {
	empty_MotorGroup_check(PROS_ERR);
	return _reverse;
}

std::int32_t MotorGroup::is_reversed(std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);

	return motor_is_reversed(index);
}
std::vector<std::int32_t> MotorGroup::every_is_reversed(void) const {
	std::vector<std::int32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_is_reversed(*it));
	}
	return return_vector;
}

double MotorGroup::get_temperature(void) const {
	empty_MotorGroup_check(PROS_ERR_F);

	return motor_get_temperature(_ports[0]);
}
double MotorGroup::get_temperature(std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR_F);
	MotorGroup_index_check(PROS_ERR_F, index);

	return motor_get_temperature(_ports[index]);
}

std::vector<double> MotorGroup::get_every_temperature(void) const {
	std::vector<double> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR_F, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_temperature(*it));
	}
	return return_vector;
}

double MotorGroup::get_target_position(void) const {
	empty_MotorGroup_check(PROS_ERR_F);

	return motor_get_target_position(_ports[0]);
}

double MotorGroup::get_target_position(std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR_F);
	MotorGroup_index_check(PROS_ERR_F, index);

	return motor_get_target_position(_ports[index]);
}

std::vector<double> MotorGroup::get_every_target_position(void) const {
	std::vector<double> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR_F, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_target_position(*it));
	}
	return return_vector;
}

double MotorGroup::get_torque(void) const {
	empty_MotorGroup_check(PROS_ERR_F);

	return motor_get_torque(_ports[0]);
}
double MotorGroup::get_torque(std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR_F);
	MotorGroup_index_check(PROS_ERR_F, index);

	return motor_get_torque(_ports[index]);
}
std::vector<double> MotorGroup::get_every_torque(void) const {
	std::vector<double> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR_F, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_torque(*it));
	}
	return return_vector;
}

std::int32_t MotorGroup::get_target_velocity(void) const {
	empty_MotorGroup_check(PROS_ERR);

	return motor_get_target_velocity(_ports[0]);
}
std::int32_t MotorGroup::get_target_velocity(std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);

	return motor_get_target_velocity(_ports[index]);
}
std::vector<std::int32_t> MotorGroup::get_every_target_velocity(void) const {
	std::vector<std::int32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_target_velocity(*it));
	}
	return return_vector;
}

std::int32_t MotorGroup::get_voltage(void) const {
	empty_MotorGroup_check(PROS_ERR);

	return motor_get_voltage(_ports[0]);
}
std::int32_t MotorGroup::get_voltage(std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);

	return motor_get_voltage(_ports[index]);
}
std::vector<std::int32_t> MotorGroup::get_every_voltage(void) const {
	std::vector<std::int32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_voltage(*it));
	}
	return return_vector;
}
std::int32_t MotorGroup::get_voltage_limit(void) const {
	empty_MotorGroup_check(PROS_ERR);

	return motor_get_voltage_limit(_ports[0]);
}
std::int32_t MotorGroup::get_voltage_limit(std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);

	return motor_get_voltage_limit(_ports[index]);
}
std::vector<std::int32_t> MotorGroup::get_every_voltage_limit(void) const {
	std::vector<std::int32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_voltage_limit(*it));
	}
	return return_vector;
}

std::vector<std::uint8_t> MotorGroup::get_every_port(void) const {
	std::vector<std::uint8_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR_BYTE, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back((uint8_t)std::abs((*it)));
	}
	return return_vector;
}

std::int32_t MotorGroup::tare_position(void) const {
	empty_MotorGroup_check(PROS_ERR);

	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_tare_position(*it);
	}
	return motor_tare_position(_ports[0]);
}

std::int32_t MotorGroup::set_brake_mode(const pros::motor_brake_mode_e_t mode) const {
	empty_MotorGroup_check(PROS_ERR);

	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_brake_mode(*it, mode);
	}
	return motor_set_brake_mode(_ports[0], mode);
}

std::int32_t MotorGroup::set_brake_mode(const pros::v5::Motor_Brake mode) const {
	empty_MotorGroup_check(PROS_ERR);

	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_brake_mode(*it, static_cast<pros::motor_brake_mode_e_t>(mode));
	}
	return motor_set_brake_mode(_ports[0], static_cast<pros::motor_brake_mode_e_t>(mode));
}

std::int32_t MotorGroup::set_current_limit(const std::int32_t limit) const {
	empty_MotorGroup_check(PROS_ERR);

	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_current_limit(*it, limit);
	}
	return motor_set_current_limit(_ports[0], limit);
}

std::int32_t MotorGroup::set_encoder_units(const pros::motor_encoder_units_e_t units) {
	empty_MotorGroup_check(PROS_ERR);
	_encoder_units = static_cast<pros::v5::Motor_Units>(units);
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_encoder_units(*it, units);
	}
	return motor_set_encoder_units(_ports[0], units);
}

std::int32_t MotorGroup::set_encoder_units(const pros::v5::Motor_Units units) {
	empty_MotorGroup_check(PROS_ERR);
	_encoder_units = units;
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_encoder_units(*it, static_cast<motor_encoder_units_e_t>(units));
	}
	return motor_set_encoder_units(_ports[0], static_cast<motor_encoder_units_e_t>(units));
}

std::int32_t MotorGroup::set_gearing(const motor_gearset_e_t gearset) {
	empty_MotorGroup_check(PROS_ERR);
	_gearset = static_cast<pros::v5::Motor_Gear>(gearset);
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_gearing(*it, gearset);
	}
	return motor_set_gearing(_ports[0], gearset);
}

std::int32_t MotorGroup::set_gearing(const pros::v5::Motor_Gear gearset) {
	empty_MotorGroup_check(PROS_ERR);
	_gearset = gearset;
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_gearing(*it, (motor_gearset_e_t)gearset);
	}
	return motor_set_gearing(_ports[0], (motor_gearset_e_t)gearset);
}

std::int32_t MotorGroup::set_zero_position(const double position) const {
	empty_MotorGroup_check(PROS_ERR);

	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_zero_position(*it, position);
	}
	return motor_set_zero_position(_ports[0], position);
}

std::int32_t MotorGroup::set_reversed(const bool reverse) {
	empty_MotorGroup_check(PROS_ERR);
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_reversed(*it, reverse ^ ((*it) < 0));
	}

	return motor_set_reversed(_ports[0], reverse ^ (_ports[0] < 0));
}

std::int32_t MotorGroup::set_voltage_limit(const std::int32_t limit) const {
	empty_MotorGroup_check(PROS_ERR);

	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_voltage_limit(*it, limit);
	}
	return motor_set_voltage_limit(_ports[0], limit);
}

std::vector<std::int8_t> MotorGroup::get_ports() const {
	return _ports;
}

std::int8_t MotorGroup::size() const {
	return _ports.size();
}

void MotorGroup::operator+=(MotorGroup& other) {
	for (auto it = other._ports.begin(); it < other._ports.end(); it++) {
		_ports.emplace_back(*it);
	}
}

void MotorGroup::append(MotorGroup& other) {
	(*this) += other;
}

void MotorGroup::erase_port(std::int8_t port) {
	auto it = _ports.begin();
	while (it < _ports.end()) {
		if (std::abs(*it) == std::abs(port)) {
			_ports.erase(it);
		} else {
			it++;
		}
	}
}

std::ostream& operator<<(std::ostream& os, pros::Motor& motor) {
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

Motor::Motor(const std::int8_t port, const pros::v5::Motor_Gears gearset = pros::v5::Motor_Gears::green,
             const bool reverse = false, const pros::v5::Motor_Units encoder_units)
    : _port(port), Device(port) {
	set_gearing(gearset);
	set_reversed(reverse);
	set_encoder_units(encoder_units);
}

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

std::int32_t Motor::brake(void) const {
	return motor_brake(_port);
}

std::int32_t Motor::modify_profiled_velocity(const std::int32_t velocity) const {
	return motor_modify_profiled_velocity(_port, velocity);
}

double Motor::get_actual_velocity(void) const {
	return motor_get_actual_velocity(_port);
}

double Motor::get_actual_velocity(std::uint8_t index) const {
	// TODO Decide what to do for out of bounds checks for motors
	return motor_get_actual_velocity(_port);
}
std::vector<double> Motor::get_every_actual_velocity(void) const {
	std::vector<double> return_vector;
	return_vector.emplace_back(motor_get_actual_velocity(_port));
	return return_vector;
}

pros::v5::Motor_Brake Motor::get_brake_mode(void) const {
	return static_cast<pros::v5::Motor_Brake>(motor_get_brake_mode(_port));
}
pros::v5::Motor_Brake Motor::get_brake_mode(std::uint8_t index) const {
	// TODO Decide what to do for out of bounds checks for motors
	return static_cast<pros::v5::Motor_Brake>(motor_get_brake_mode(_port));
}

std::vector<pros::v5::Motor_Brake> Motor::get_every_brake_mode(void) const {
	std::vector<pros::v5::Motor_Brake> return_vector;
	return_vector.emplace_back(static_cast<pros::v5::Motor_Brake>(motor_get_brake_mode(_port)));
	return return_vector;
}

std::int32_t Motor::get_current_draw(void) const {
	return motor_get_current_draw(_port);
}
std::int32_t Motor::get_current_draw(std::uint8_t index) const {
	// TODO Decide what to do for out of bounds checks for motors
	return motor_get_current_draw(_port);
}
std::vector<std::int32_t> Motor::get_every_current_draw(void) const {
	std::vector<std::int32_t> return_vector;
	return_vector.emplace_back(motor_get_current_draw(_port));
	return return_vector;
}
std::int32_t Motor::get_current_limit(void) const {
	return motor_get_current_limit(_port);
}
std::int32_t Motor::get_current_limit(std::uint8_t index) const {
	// TODO Decide what to do for out of bounds checks for motors
	return motor_get_current_limit(_port);
}
std::vector<std::int32_t> Motor::get_every_current_limit(void) const {
	std::vector<std::int32_t> return_vector;
	return_vector.emplace_back(motor_get_current_limit(_port));
	return return_vector;
}
std::int32_t Motor::is_over_current(void) const {
	return motor_is_over_current(_port);
}
std::int32_t Motor::is_over_current(std::uint8_t index) const {
	// TODO Decide what to do for out of bounds checks for motors

	return motor_is_over_current(0);
}

std::vector<std::int32_t> Motor::every_is_over_current(void) const {
	std::vector<std::int32_t> return_vector;
	empty_Motor_check_vector(PROS_ERR, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_is_over_current(*it));
	}
	return return_vector;
}

std::int32_t Motor::get_direction(void) const {
	return motor_get_direction(_port);
}
std::int32_t Motor::get_direction(std::uint8_t index) const {
	Motor_index_check(PROS_ERR, index);

	return motor_get_direction(_port);
}
std::vector<std::int32_t> Motor::get_every_direction(void) const {
	std::vector<std::int32_t> return_vector;
	empty_Motor_check_vector(PROS_ERR, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.emplace_back(motor_get_direction(*it));
	}
	return return_vector;
}

double Motor::get_efficiency(void) const {
	return motor_get_efficiency(_port);
}
double Motor::get_efficiency(std::uint8_t index) const {
	static_cast<pros::v5::Motor_Units>(motor_get_encoder_units(_port)) return motor_get_efficiency(_port);
}
std::vector<double> Motor::get_every_efficiency(void) const {
	std::vector<double> return_vector;
	return_vector.emplace_back(motor_get_efficiency(_port));
	return return_vector;
}

pros::v5::Motor_Units Motor::get_encoder_units(void) const {
	return static_cast<pros::v5::Motor_Units>(motor_get_encoder_units(_port));
}
pros::v5::Motor_Units Motor::get_encoder_units(std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return static_cast<pros::v5::Motor_Units>(motor_get_encoder_units(_port));
}

std::vector<pros::v5::Motor_Units> Motor::get_every_encoder_units(void) const {
	std::vector<pros::v5::Motor_Units> return_vector;
	return_vector.emplace_back(static_cast<pros::v5::Motor_Units>(motor_get_encoder_units(_port)));
	return return_vector;
}

std::uint32_t Motor::get_faults(void) const {
	return motor_get_faults(_port);
}
std::uint32_t Motor::get_faults(std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_get_faults(_port);
}

std::vector<std::uint32_t> Motor::get_every_faults(void) const {
	std::vector<std::uint32_t> return_vector;
	return_vector.emplace_back(motor_get_faults(_port));
	return return_vector;
}

std::uint32_t Motor::get_flags(void) const {
	return motor_get_flags(_port);
}
std::uint32_t Motor::get_flags(std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_get_flags(_port);
}

std::vector<std::uint32_t> Motor::get_every_flags(void) const {
	std::vector<std::uint32_t> return_vector;
	return_vector.emplace_back(motor_get_flags(_port));
	return return_vector;
}

pros::v5::Motor_Gears Motor::get_gearing(void) const {
	return motor_get_gearing(_port);
}

pros::v5::Motor_Gears Motor::get_gearing(std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return static_cast<pros::v5::Motor_Gears>(motor_get_gearing(_port));
}
std::vector<pros::v5::Motor_Gears> Motor::get_every_gearing(void) const {
	std::vector<pros::v5::Motor_Gears> return_vector;
	return_vector.emplace_back(static_cast<pros::v5::Motor_Gears>(motor_get_gearing(_port)));
	return return_vector;
}

std::int32_t Motor::get_raw_position(std::uint32_t* const timestamp) const {
	return motor_get_raw_position(_port, timestamp);
}
std::int32_t Motor::get_raw_position(std::uint32_t* const timestamp, std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_get_raw_position(_port, timestamp);
}

std::vector<std::int32_t> Motor::get_every_raw_position(std::uint32_t* const timestamp) const {
	std::vector<std::int32_t> return_vector;
	return_vector.emplace_back(motor_get_raw_position(*it, timestamp));
	return return_vector;
}
std::int32_t Motor::is_over_temp(void) const {
	return motor_is_over_temp(_port);
}
std::int32_t Motor::is_over_temp(std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_is_over_temp(_port);
}

std::vector<std::int32_t> Motor::every_is_over_temp(void) const {
	std::vector<std::int32_t> return_vector;
	return_vector.emplace_back(motor_is_over_temp(_port));
	return return_vector;
}

double Motor::get_position(void) const {
	return motor_get_position(_port);
}
double Motor::get_position(std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_get_position(_port);
}
std::vector<double> Motor::get_every_position(void) const {
	std::vector<double> return_vector;
	return_vector.emplace_back(motor_get_position(_port));
	return return_vector;
}

double Motor::get_power(void) const {
	return motor_get_power(_port);
}

double Motor::get_power(std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_get_power(_port);
}

std::vector<double> Motor::get_every_power(void) const {
	std::vector<double> return_vector;
	return_vector.emplace_back(motor_get_power(_port));
	return return_vector;
}
std::int32_t Motor::is_reversed(void) const {
	return _reverse;
}

std::int32_t Motor::is_reversed(std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_is_reversed(index);
}
std::vector<std::int32_t> Motor::every_is_reversed(void) const {
	std::vector<std::int32_t> return_vector;
	return_vector.emplace_back(motor_is_reversed(_port));
	return return_vector;
}

double Motor::get_temperature(void) const {
	return motor_get_temperature(_port);
}
double Motor::get_temperature(std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_get_temperature(_port);
}

std::vector<double> Motor::get_every_temperature(void) const {
	std::vector<double> return_vector;
	return_vector.emplace_back(motor_get_temperature(_port));
	return return_vector;
}

double Motor::get_target_position(void) const {
	return motor_get_target_position(_port);
}

double Motor::get_target_position(std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_get_target_position(_port);
}

std::vector<double> Motor::get_every_target_position(void) const {
	std::vector<double> return_vector;
	return_vector.emplace_back(motor_get_target_position(_port));
	return return_vector;
}

double Motor::get_torque(void) const {
	empty_Motor_check(PROS_ERR_F);

	return motor_get_torque(_port);
}
double Motor::get_torque(std::uint8_t index) const {
	return motor_get_torque(_port);
}
std::vector<double> Motor::get_every_torque(void) const {
	std::vector<double> return_vector;
	return_vector.emplace_back(motor_get_torque(_port));
	return return_vector;
}

std::int32_t Motor::get_target_velocity(void) const {
	return motor_get_target_velocity(_port);
}
std::int32_t Motor::get_target_velocity(std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_get_target_velocity(_port);
}
std::vector<std::int32_t> Motor::get_every_target_velocity(void) const {
	std::vector<std::int32_t> return_vector;
	return_vector.emplace_back(motor_get_target_velocity(_port));
	return return_vector;
}

std::int32_t Motor::get_voltage(void) const {
	return motor_get_voltage(_port);
}
std::int32_t Motor::get_voltage(std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_get_voltage(_port);
}
std::vector<std::int32_t> Motor::get_every_voltage(void) const {
	std::vector<std::int32_t> return_vector;
	return_vector.emplace_back(motor_get_voltage(_port));
	return return_vector;
}
std::int32_t Motor::get_voltage_limit(void) const {
	return motor_get_voltage_limit(_port);
}
std::int32_t Motor::get_voltage_limit(std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_get_voltage_limit(_port);
}
std::vector<std::int32_t> Motor::get_every_voltage_limit(void) const {
	std::vector<std::int32_t> return_vector;
	return_vector.emplace_back(motor_get_voltage_limit(_port));
	return return_vector;
}

std::vector<std::uint8_t> Motor::get_every_port(void) const {
	std::vector<std::uint8_t> return_vector;
	return_vector.emplace_back((uint8_t)std::abs((_port)));
	return return_vector;
}

std::int32_t Motor::tare_position(void) const {
	return motor_tare_position(_port);
}

std::int32_t Motor::set_brake_mode(const pros::motor_brake_mode_e_t mode) const {
	return motor_set_brake_mode(_port, mode);
}

std::int32_t Motor::set_brake_mode(const pros::v5::Motor_Brake mode) const {
	return motor_set_brake_mode(_port, static_cast<pros::motor_brake_mode_e_t>(mode));
}

std::int32_t Motor::set_current_limit(const std::int32_t limit) const {
	return motor_set_current_limit(_port, limit);
}

std::int32_t Motor::set_encoder_units(const pros::motor_encoder_units_e_t units) {
	return motor_set_encoder_units(_port, units);
}

std::int32_t Motor::set_encoder_units(const pros::v5::Motor_Units units) {
	return motor_set_encoder_units(_port, static_cast<motor_encoder_units_e_t>(units));
}

std::int32_t Motor::set_gearing(const motor_gearset_e_t gearset) {
	return motor_set_gearing(_port, gearset);
}

std::int32_t Motor::set_gearing(const pros::v5::Motor_Gear gearset) {
	return motor_set_gearing(_port, static_cast<motor_gearset_e_t>(gearset));
}

std::int32_t Motor::set_zero_position(const double position) const {
	return motor_set_zero_position(_port, position);
}

std::int32_t Motor::set_reversed(const bool reverse) {
	return motor_set_reversed(_port, reverse);
}

std::int32_t Motor::set_voltage_limit(const std::int32_t limit) const {
	return motor_set_voltage_limit(_port, limit);
}

std::vector<std::int8_t> Motor::get_ports() const {
	return _ports;
}

std::int8_t Motor::size() const {
	return _ports.size();
}
DeviceType Motor::get_type() const {
	return DeviceType::motor;
}

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
