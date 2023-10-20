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
#include "pros/motor_group.hpp"
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
		vector.push_back(error);                      \
		return vector;                                   \
	}


MotorGroup::MotorGroup(MotorGroup& motor_group) : MotorGroup(motor_group.get_port_all()) {
}

MotorGroup::MotorGroup(const std::initializer_list<std::int8_t> ports,
                       const pros::v5::MotorGears gearset,
                       const pros::v5::MotorUnits encoder_units)
    : _ports(ports) {
	if (gearset != pros::v5::MotorGears::invalid) {
		set_gearing_all(gearset);
	}
	if (encoder_units != pros::v5::MotorUnits::invalid) {
		set_encoder_units_all(encoder_units);
	}
}

MotorGroup::MotorGroup(const std::vector<std::int8_t>& ports,
                       const pros::v5::MotorGears gearset,
                       const pros::v5::MotorUnits encoder_units)
    : _ports(ports) {
	if (gearset != pros::v5::MotorGears::invalid) {
		set_gearing_all(gearset);
	}
	if (encoder_units != pros::v5::MotorUnits::invalid) {
		set_encoder_units_all(encoder_units);
	}
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

double MotorGroup::get_actual_velocity(const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR_F);
	MotorGroup_index_check(PROS_ERR_F, index);

	return motor_get_actual_velocity(_ports[index]);
}
std::vector<double> MotorGroup::get_actual_velocity_all(void) const {
	std::vector<double> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR_F, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.push_back(motor_get_actual_velocity(*it));
	}
	return return_vector;
}

pros::v5::MotorBrake MotorGroup::get_brake_mode(const std::uint8_t index) const {
	empty_MotorGroup_check(pros::v5::MotorBrake::invalid);
	MotorGroup_index_check(pros::v5::MotorBrake::invalid, index);

	return static_cast<pros::v5::MotorBrake>(motor_get_brake_mode(_ports[index]));
}

std::vector<pros::v5::MotorBrake> MotorGroup::get_brake_mode_all(void) const {
	std::vector<pros::v5::MotorBrake> return_vector;
	empty_MotorGroup_check_vector(pros::v5::MotorBrake::invalid, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.push_back(static_cast<pros::v5::MotorBrake>(motor_get_brake_mode(*it)));
	}
	return return_vector;
}

std::int32_t MotorGroup::get_current_draw(const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);

	return motor_get_current_draw(_ports[index]);
}
std::vector<std::int32_t> MotorGroup::get_current_draw_all(void) const {
	std::vector<std::int32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.push_back(motor_get_current_draw(*it));
	}
	return return_vector;
}

std::int32_t MotorGroup::get_current_limit(const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);

	return motor_get_current_limit(_ports[index]);
}

std::vector<std::int32_t> MotorGroup::get_current_limit_all(void) const {
	std::vector<std::int32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);

	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.push_back(motor_get_current_limit(*it));
	}
	return return_vector;
}

std::int32_t MotorGroup::is_over_current(const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);
	return motor_is_over_current(_ports[index]);
}

std::vector<std::int32_t> MotorGroup::is_over_current_all(void) const {
	std::vector<std::int32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.push_back(motor_is_over_current(*it));
	}
	return return_vector;
}

std::int32_t MotorGroup::get_direction(const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);
	int ret = motor_get_direction(_ports[index]);
	ret = _ports[index] >= 0 ? ret : ret * -1;
	return ret;
}

std::vector<std::int32_t> MotorGroup::get_direction_all(void) const {
	std::vector<std::int32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		int ret = motor_get_direction(*it);
		ret = *it >= 0 ? ret : ret * -1;
		return_vector.push_back(ret);
	}
	return return_vector;
}

double MotorGroup::get_efficiency(const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR_F);
	MotorGroup_index_check(PROS_ERR_F, index);
	return motor_get_efficiency(_ports[index]);
}

std::vector<double> MotorGroup::get_efficiency_all(void) const {
	std::vector<double> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR_F, return_vector);
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.push_back(motor_get_efficiency(*it));
	}
	return return_vector;
}

pros::v5::MotorUnits MotorGroup::get_encoder_units(const std::uint8_t index) const {
	empty_MotorGroup_check(pros::v5::MotorUnits::invalid);
	MotorGroup_index_check(pros::v5::MotorUnits::invalid, index);
	return static_cast<pros::v5::MotorUnits>(motor_get_encoder_units(_ports[index]));
}

std::vector<pros::v5::MotorUnits> MotorGroup::get_encoder_units_all(void) const {
	std::vector<pros::v5::MotorUnits> return_vector;
	empty_MotorGroup_check_vector(pros::v5::MotorUnits::invalid, return_vector);
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.push_back(static_cast<pros::v5::MotorUnits>(motor_get_encoder_units(*it)));
	}
	return return_vector;
}

std::uint32_t MotorGroup::get_faults(const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);
	return motor_get_faults(_ports[index]);
}

std::vector<std::uint32_t> MotorGroup::get_faults_all(void) const {
	std::vector<std::uint32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.push_back(motor_get_faults(*it));
	}
	return return_vector;
}

std::uint32_t MotorGroup::get_flags(const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);
	return motor_get_flags(_ports[index]);
}

std::vector<std::uint32_t> MotorGroup::get_flags_all(void) const {
	std::vector<std::uint32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.push_back(motor_get_flags(*it));
	}
	return return_vector;
}

pros::v5::MotorGears MotorGroup::get_gearing(const std::uint8_t index) const {
	empty_MotorGroup_check(pros::v5::MotorGears::invalid);
	MotorGroup_index_check(pros::v5::MotorGears::invalid, index);
	return static_cast<pros::v5::MotorGears>(motor_get_gearing(_ports[index]));
}

std::vector<pros::v5::MotorGears> MotorGroup::get_gearing_all(void) const {
	std::vector<pros::v5::MotorGears> return_vector;
	empty_MotorGroup_check_vector(pros::v5::MotorGears::invalid, return_vector);
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.push_back(static_cast<pros::v5::MotorGears>(motor_get_gearing(*it)));
	}
	return return_vector;
}

std::int32_t MotorGroup::get_raw_position(std::uint32_t* const timestamp, std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);
	return motor_get_raw_position(_ports[index], timestamp);
}

std::vector<std::int32_t> MotorGroup::get_raw_position_all(std::uint32_t* const timestamp) const {
	std::vector<std::int32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.push_back(motor_get_raw_position(*it, timestamp));
	}
	return return_vector;
}

std::int32_t MotorGroup::is_over_temp(const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);
	return motor_is_over_temp(_ports[index]);
}

std::vector<std::int32_t> MotorGroup::is_over_temp_all(void) const {
	std::vector<std::int32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.push_back(motor_is_over_temp(*it));
	}
	return return_vector;
}

double MotorGroup::get_position(const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR_F);
	MotorGroup_index_check(PROS_ERR_F, index);
	return motor_get_position(_ports[index]);
}

std::vector<double> MotorGroup::get_position_all(void) const {
	std::vector<double> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR_F, return_vector);
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.push_back(motor_get_position(*it));
	}
	return return_vector;
}

double MotorGroup::get_power(const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR_F);
	MotorGroup_index_check(PROS_ERR_F, index);
	return motor_get_power(_ports[index]);
}

std::vector<double> MotorGroup::get_power_all(void) const {
	std::vector<double> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR_F, return_vector);
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.push_back(motor_get_power(*it));
	}
	return return_vector;
}

std::int32_t MotorGroup::is_reversed(const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);
	return motor_is_reversed(index);
}

std::vector<std::int32_t> MotorGroup::is_reversed_all(void) const {
	std::vector<std::int32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.push_back(motor_is_reversed(*it));
	}
	return return_vector;
}

double MotorGroup::get_temperature(const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR_F);
	MotorGroup_index_check(PROS_ERR_F, index);
	return motor_get_temperature(_ports[index]);
}

std::vector<double> MotorGroup::get_temperature_all(void) const {
	std::vector<double> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR_F, return_vector);
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.push_back(motor_get_temperature(*it));
	}
	return return_vector;
}

double MotorGroup::get_target_position(const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR_F);
	MotorGroup_index_check(PROS_ERR_F, index);
	return motor_get_target_position(_ports[index]);
}

std::vector<double> MotorGroup::get_target_position_all(void) const {
	std::vector<double> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR_F, return_vector);
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.push_back(motor_get_target_position(*it));
	}
	return return_vector;
}

double MotorGroup::get_torque(const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR_F);
	MotorGroup_index_check(PROS_ERR_F, index);
	return motor_get_torque(_ports[index]);
}

std::vector<double> MotorGroup::get_torque_all(void) const {
	std::vector<double> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR_F, return_vector);
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.push_back(motor_get_torque(*it));
	}
	return return_vector;
}

std::int32_t MotorGroup::get_target_velocity(const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);
	return motor_get_target_velocity(_ports[index]);
}

std::vector<std::int32_t> MotorGroup::get_target_velocity_all(void) const {
	std::vector<std::int32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.push_back(motor_get_target_velocity(*it));
	}
	return return_vector;
}

std::int32_t MotorGroup::get_voltage(const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);
	return motor_get_voltage(_ports[index]);
}
std::vector<std::int32_t> MotorGroup::get_voltage_all(void) const {
	std::vector<std::int32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.push_back(motor_get_voltage(*it));
	}
	return return_vector;
}

std::int32_t MotorGroup::get_voltage_limit(const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);
	return motor_get_voltage_limit(_ports[index]);
}

std::vector<std::int32_t> MotorGroup::get_voltage_limit_all(void) const {
	std::vector<std::int32_t> return_vector;
	empty_MotorGroup_check_vector(PROS_ERR, return_vector);
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		return_vector.push_back(motor_get_voltage_limit(*it));
	}
	return return_vector;
}

std::int8_t MotorGroup::get_port(const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR_BYTE);
	MotorGroup_index_check(PROS_ERR_BYTE, index);
	return _ports[index];
}

std::vector<std::int8_t> MotorGroup::get_port_all(void) const {
	return _ports;
}

std::int32_t MotorGroup::tare_position(const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);
	return motor_tare_position(_ports[index]);
}

std::int32_t MotorGroup::tare_position_all(void) const {
	empty_MotorGroup_check(PROS_ERR);
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_tare_position(*it);
	}
	return motor_tare_position(_ports[0]);
}

std::int32_t MotorGroup::set_brake_mode(const pros::motor_brake_mode_e_t mode, const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);
	return motor_set_brake_mode(_ports[index], mode);
}

std::int32_t MotorGroup::set_brake_mode(const pros::v5::MotorBrake mode, const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);

	return motor_set_brake_mode(_ports[index], static_cast<pros::motor_brake_mode_e_t>(mode));
}

std::int32_t MotorGroup::set_brake_mode_all(const pros::motor_brake_mode_e_t mode) const {
	empty_MotorGroup_check(PROS_ERR);
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_brake_mode(*it, mode);
	}
	return motor_set_brake_mode(_ports[0], mode);
}

std::int32_t MotorGroup::set_brake_mode_all(const pros::v5::MotorBrake mode) const {
	empty_MotorGroup_check(PROS_ERR);
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_brake_mode(*it, static_cast<pros::motor_brake_mode_e_t>(mode));
	}
	return motor_set_brake_mode(_ports[0], static_cast<pros::motor_brake_mode_e_t>(mode));
}
std::int32_t MotorGroup::set_current_limit(const std::int32_t limit, const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);
	return motor_set_current_limit(_ports[index], limit);
}
std::int32_t MotorGroup::set_current_limit_all(const std::int32_t limit) const {
	empty_MotorGroup_check(PROS_ERR);
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_current_limit(*it, limit);
	}
	return motor_set_current_limit(_ports[0], limit);
}
std::int32_t MotorGroup::set_encoder_units_all(const pros::motor_encoder_units_e_t units) const {
	empty_MotorGroup_check(PROS_ERR);
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_encoder_units(*it, units);
	}
	return motor_set_encoder_units(_ports[0], units);
}

std::int32_t MotorGroup::set_encoder_units_all(const pros::v5::MotorUnits units) const {
	empty_MotorGroup_check(PROS_ERR);
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_encoder_units(*it, static_cast<motor_encoder_units_e_t>(units));
	}
	return motor_set_encoder_units(_ports[0], static_cast<motor_encoder_units_e_t>(units));
}
std::int32_t MotorGroup::set_encoder_units(const pros::motor_encoder_units_e_t units, const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);
	return motor_set_encoder_units(_ports[index], units);
}

std::int32_t MotorGroup::set_encoder_units(const pros::v5::MotorUnits units, const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);
	return motor_set_encoder_units(_ports[index], static_cast<motor_encoder_units_e_t>(units));
}

std::int32_t MotorGroup::set_gearing(const motor_gearset_e_t gearset, const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);
	return motor_set_gearing(_ports[index], gearset);
}
std::int32_t MotorGroup::set_gearing(std::vector<motor_gearset_e_t> gearsets) const {
	empty_MotorGroup_check(PROS_ERR);
	for (int i = 0; i < gearsets.size(); i++){
		this->set_gearing(gearsets[i], _ports[i]);
	}
	if (gearsets.size() != _ports.size()) {
		errno = E2BIG;
	}
	return PROS_SUCCESS;
}

std::int32_t MotorGroup::set_gearing(std::vector<MotorGears> gearsets) const {
	empty_MotorGroup_check(PROS_ERR);
	for (int i = 0; i < gearsets.size(); i++){
		this->set_gearing(gearsets[i], _ports[i]);
	}
	if (gearsets.size() != _ports.size()) {
		errno = E2BIG;
	}
	return PROS_SUCCESS;
}
std::int32_t MotorGroup::set_gearing(const pros::v5::MotorGear gearset, const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);
	return motor_set_gearing(_ports[index], (motor_gearset_e_t)gearset);
}

std::int32_t MotorGroup::set_gearing_all(const motor_gearset_e_t gearset) const {
	empty_MotorGroup_check(PROS_ERR);
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_gearing(*it, gearset);
	}
	return motor_set_gearing(_ports[0], gearset);
}

std::int32_t MotorGroup::set_gearing_all(const pros::v5::MotorGear gearset) const {
	empty_MotorGroup_check(PROS_ERR);
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_gearing(*it, (motor_gearset_e_t)gearset);
	}
	return motor_set_gearing(_ports[0], (motor_gearset_e_t)gearset);
}

std::int32_t MotorGroup::set_zero_position(const double position, const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);
	return motor_set_zero_position(_ports[index], position);
}
std::int32_t MotorGroup::set_zero_position_all(const double position) const {
	empty_MotorGroup_check(PROS_ERR);
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_zero_position(*it, position);
	}
	return motor_set_zero_position(_ports[0], position);
}
std::int32_t MotorGroup::set_reversed(const bool reverse, const std::uint8_t index) {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);
	std::int8_t abs_port = std::abs(_ports[index]);
	if (reverse) {
		_ports[index] = -abs_port;
	} else {
		_ports[index] = abs_port;
	}
	return PROS_SUCCESS;
}
std::int32_t MotorGroup::set_reversed_all(const bool reverse) {
	empty_MotorGroup_check(PROS_ERR);
	for (auto it = _ports.begin(); it < _ports.end(); it++) {
		std::int8_t abs_port = std::abs(_ports[*it]);
		if (reverse) {
			_ports[*it] = -abs_port;
		} else {
			_ports[*it] = abs_port;
		}
	}
	return PROS_SUCCESS;
}
std::int32_t MotorGroup::set_voltage_limit(const std::int32_t limit, const std::uint8_t index) const {
	empty_MotorGroup_check(PROS_ERR);
	MotorGroup_index_check(PROS_ERR, index);
	return motor_set_voltage_limit(_ports[index], limit);
}
std::int32_t MotorGroup::set_voltage_limit_all(const std::int32_t limit) const {
	empty_MotorGroup_check(PROS_ERR);
	for (auto it = _ports.begin() + 1; it < _ports.end(); it++) {
		motor_set_voltage_limit(*it, limit);
	}
	return motor_set_voltage_limit(_ports[0], limit);
}

std::int8_t MotorGroup::size() const {
	return _ports.size();
}

void MotorGroup::operator+=(MotorGroup& other) {
	for (auto it = other._ports.begin(); it < other._ports.end(); it++) {
		_ports.push_back(*it);
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
}  // namespace v5
}  // namespace pros