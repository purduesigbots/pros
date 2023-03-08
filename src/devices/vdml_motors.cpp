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

#include <stdint.h>
#include <vector>
#include <cassert>

#include "kapi.h"
#include "pros/motors.hpp"

/**
 * Macro to claim the motor group mutex with the error code being PROS_ERR
 *
 */
#define claim_mg_mutex(error)                  \
	if (!_motor_group_mutex.take(TIMEOUT_MAX)) { \
		return error;                              \
	}

/**
 * Macro to free the motor group mutex with the error value being PROS_ERR
 *
 */
#define give_mg_mutex(error)        \
	if (!_motor_group_mutex.give()) { \
		return error;                   \
	}

/**
 * Macro to loop through a function call for each motor in a motor group
 * with an error value of PROS_ERR
 *
 */
#define mg_foreach(func_call, motors, out) \
	for (int i = 0; i < _motor_count; i++) { \
		if (motors[i].func_call != PROS_ERR) { \
			out = PROS_SUCCESS;                  \
		} else {                               \
			out = PROS_ERR;                      \
		}                                      \
	}

/**
 * Macro to take the motor group mutex with a vector of error as the error value
 *
 */
#define claim_mg_mutex_vector(error)           \
	if (!_motor_group_mutex.take(TIMEOUT_MAX)) { \
		out.clear();                               \
		for (int i = 0; i < _motor_count; i++) {   \
			out.push_back(error);                    \
		}                                          \
		out.resize(_motor_count);                  \
		out.shrink_to_fit();                       \
		return out;                                \
	}

/**
 * Macro to free the motor group mutex with a vector of error as the error value
 *
 */
#define give_mg_mutex_vector(error)          \
	if (!_motor_group_mutex.give()) {          \
		out.clear();                             \
		for (int i = 0; i < _motor_count; i++) { \
			out.push_back(error);                  \
		}                                        \
	}

namespace pros {
using namespace pros::c;

Motor::Motor(const std::int8_t port, const motor_gearset_e_t gearset, const bool reverse,
             const motor_encoder_units_e_t encoder_units)
    : _port(abs(port)) {
	set_gearing(gearset);
	set_reversed(reverse);
	set_encoder_units(encoder_units);
	if (port < 0) 
		set_reversed(true);
}

Motor::Motor(const std::int8_t port, const motor_gearset_e_t gearset, const bool reverse) : _port(abs(port)) {
	set_gearing(gearset);
	set_reversed(reverse);
	if (port < 0) 
		set_reversed(true);
}

Motor::Motor(const std::int8_t port, const motor_gearset_e_t gearset) : _port(abs(port)) {
	set_gearing(gearset);
	if (port < 0) 
		set_reversed(true);
}

Motor::Motor(const std::int8_t port, const bool reverse) : _port(abs(port)) {
	set_reversed(reverse);
	if (port < 0) 
		set_reversed(true);
}

Motor::Motor(const std::int8_t port) : _port(abs(port)) {
	if (port < 0) 
		set_reversed(true);
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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	return motor_get_pos_pid(_port);
#pragma GCC diagnostic pop
}

motor_pid_full_s_t Motor::get_vel_pid(void) const {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	return motor_get_vel_pid(_port);
#pragma GCC diagnostic pop
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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	return motor_convert_pid(kf, kp, ki, kd);
#pragma GCC diagnostic pop
}

motor_pid_full_s_t Motor::convert_pid_full(double kf, double kp, double ki, double kd, double filter, double limit,
                                           double threshold, double loopspeed) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	return motor_convert_pid_full(kf, kp, ki, kd, filter, limit, threshold, loopspeed);
#pragma GCC diagnostic pop
}

std::int32_t Motor::set_pos_pid(const motor_pid_s_t pid) const {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	return motor_set_pos_pid(_port, pid);
#pragma GCC diagnostic pop
}

std::int32_t Motor::set_pos_pid_full(const motor_pid_full_s_t pid) const {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	return motor_set_pos_pid_full(_port, pid);
#pragma GCC diagnostic pop
}

std::int32_t Motor::set_vel_pid(const motor_pid_s_t pid) const {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	return motor_set_vel_pid(_port, pid);
#pragma GCC diagnostic pop
}

std::int32_t Motor::set_vel_pid_full(const motor_pid_full_s_t pid) const {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	return motor_set_vel_pid_full(_port, pid);
#pragma GCC diagnostic pop
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

Motor_Group::Motor_Group(const std::initializer_list<Motor> motors)
    : _motors(motors), _motor_group_mutex(pros::Mutex()), _motor_count(motors.size()) {
    assert(_motor_count > 0);
}

Motor_Group::Motor_Group(const std::vector<pros::Motor>& motors) 
	: _motors(motors), _motor_group_mutex(pros::Mutex()), _motor_count(motors.size()) {}

Motor_Group::Motor_Group(const std::initializer_list<std::int8_t> motor_ports) 
	: _motor_group_mutex(pros::Mutex()), _motor_count(motor_ports.size()) {
	for (auto& i : motor_ports) {
		_motors.emplace_back(i);
	}	
}

Motor_Group::Motor_Group(const std::vector<std::int8_t> motor_ports)
    : _motor_group_mutex(pros::Mutex()), _motor_count(motor_ports.size()) {
	for (auto& i: motor_ports) {
		_motors.emplace_back(i);
	}
}

std::int32_t Motor_Group::move(std::int32_t voltage) {
	claim_mg_mutex(PROS_ERR);
	std::int32_t out = PROS_SUCCESS;
	mg_foreach(move(voltage), _motors, out);
	give_mg_mutex(PROS_ERR);
	return out;
}

std::int32_t Motor_Group::operator=(std::int32_t voltage) {
	claim_mg_mutex(PROS_ERR);
	std::int32_t out = PROS_SUCCESS;
	mg_foreach(move(voltage), _motors, out);
	give_mg_mutex(PROS_ERR);
	return out;
}

pros::Motor& Motor_Group::operator[](int i) {
	if (_motors.empty()) {
		errno = ENXIO;
		return _motors[0];
	}
	if (i >= _motor_count) {
		errno = ENXIO;
		return _motors[0];
	}
	return _motors.at(i);
}

pros::Motor& Motor_Group::at(int i) {
	if (i >= _motor_count || _motors.empty()) {
		throw std::out_of_range("Out of Range! Motor_Group for at() method is out of range");
	}
	return _motors[i];
}

std::int32_t Motor_Group::size() {
	return _motor_count;
}


std::int32_t Motor_Group::move_absolute(const double position, const std::int32_t velocity) {
	claim_mg_mutex(PROS_ERR);
	std::int32_t out = PROS_SUCCESS;
	mg_foreach(move_absolute(position, velocity), _motors, out);
	give_mg_mutex(PROS_ERR);
	return out;
}

std::int32_t Motor_Group::move_relative(const double position, const std::int32_t velocity) {
	claim_mg_mutex(PROS_ERR);
	std::int32_t out = PROS_SUCCESS;
	mg_foreach(move_relative(position, velocity), _motors, out);
	give_mg_mutex(PROS_ERR);
	return out;
}

std::int32_t Motor_Group::move_velocity(const std::int32_t velocity) {
	claim_mg_mutex(PROS_ERR);
	std::int32_t out = PROS_SUCCESS;
	mg_foreach(move_velocity(velocity), _motors, out);
	give_mg_mutex(PROS_ERR);
	return out;
}

std::int32_t Motor_Group::move_voltage(const std::int32_t voltage) {
	claim_mg_mutex(PROS_ERR);
	std::int32_t out = PROS_SUCCESS;
	mg_foreach(move_voltage(voltage), _motors, out);
	give_mg_mutex(PROS_ERR);
	return out;
}

std::int32_t Motor_Group::brake(void) {
	claim_mg_mutex(PROS_ERR);
	std::int32_t out = PROS_SUCCESS;
	mg_foreach(brake(), _motors, out);
	give_mg_mutex(PROS_ERR);
	return out;
}

std::int32_t Motor_Group::set_brake_modes(motor_brake_mode_e_t mode) {
	claim_mg_mutex(PROS_ERR);
	std::int32_t out = PROS_SUCCESS;
	mg_foreach(set_brake_mode(mode), _motors, out);
	give_mg_mutex(PROS_ERR);
	return out;
}

std::int32_t Motor_Group::set_zero_position(const double position) {
	claim_mg_mutex(PROS_ERR);
	std::int32_t out = PROS_SUCCESS;
	mg_foreach(set_zero_position(position), _motors, out);
	give_mg_mutex(PROS_ERR);
	return out;
}

std::int32_t Motor_Group::set_reversed(const bool reversed) {
	claim_mg_mutex(PROS_ERR);
	std::int32_t out = PROS_SUCCESS;
	mg_foreach(set_reversed(reversed), _motors, out);
	give_mg_mutex(PROS_ERR);
	return out;
}

std::vector<double> Motor_Group::get_temperatures(void) {
	std::vector<double> out;
	claim_mg_mutex_vector(PROS_ERR_F);
	for (Motor motor : _motors) {
		out.push_back(motor.get_temperature());
	}
	give_mg_mutex_vector(PROS_ERR_F);
	return out;
}

std::int32_t Motor_Group::set_voltage_limit(const std::int32_t limit) {
	claim_mg_mutex(PROS_ERR);
	std::int32_t out = PROS_SUCCESS;
	mg_foreach(set_voltage_limit(limit), _motors, out);
	give_mg_mutex(PROS_ERR);
	return out;
}

std::int32_t Motor_Group::set_gearing(const motor_gearset_e_t gearset) {
	claim_mg_mutex(PROS_ERR);
	std::int32_t out = PROS_SUCCESS;
	mg_foreach(set_gearing(gearset), _motors, out);
	give_mg_mutex(PROS_ERR);
	return out;
}

std::int32_t Motor_Group::set_encoder_units(const motor_encoder_units_e_t units) {
	claim_mg_mutex(PROS_ERR);
	std::int32_t out = PROS_SUCCESS;
	mg_foreach(set_encoder_units(units), _motors, out);
	give_mg_mutex(PROS_ERR);
	return out;
}
std::int32_t Motor_Group::tare_position(void) {
	claim_mg_mutex(PROS_ERR);
	std::int32_t out = PROS_SUCCESS;
	mg_foreach(tare_position(), _motors, out);
	give_mg_mutex(PROS_ERR);
	return out;
}

std::vector<std::uint32_t> Motor_Group::get_voltages(void) {
	std::vector<std::uint32_t> out;
	claim_mg_mutex_vector(PROS_ERR);
	for (Motor motor : _motors) {
		out.push_back(motor.get_voltage());
	}
	give_mg_mutex_vector(PROS_ERR);
	return out;
}

std::vector<std::uint32_t> Motor_Group::get_voltage_limits(void) {
	std::vector<std::uint32_t> out;
	claim_mg_mutex_vector(PROS_ERR);
	for (Motor &motor : _motors) {	
		out.push_back(motor.get_voltage_limit());
	}
	give_mg_mutex_vector(PROS_ERR);
	return out;
}

std::vector<std::int32_t> Motor_Group::get_raw_positions(std::vector<std::uint32_t*> &timestamps) {
	// Create a vector size of the number of motors and fill it with PROS_ERR
	std::vector<std::int32_t> out(_motors.size(), PROS_ERR);
	claim_mg_mutex_vector(PROS_ERR);
	if (timestamps.empty()) {
		// If the timestamps vector is null, return a vector of PROS_ERR
		give_mg_mutex_vector(PROS_ERR);
		return out;
	}
	// Find the minimum between the number of motors and the number of timestamps
	// Use this to know how many times to iterate through the loop
	// To prevent a segfault if vector of timestamps smaller than the number of motors
	// If the timestamps vector larger than motor, the extra timestamps will be ignored
	int min_iter = std::min(_motors.size(), timestamps.size());
	for (int i = 0; i < min_iter; i++) {
		// Check timestamp is not nullptr before getting the raw position
		if (timestamps[i] != nullptr) {
			out[i] = _motors[i].get_raw_position(timestamps[i]);
		}
	}
	give_mg_mutex_vector(PROS_ERR);
	return out;
}

std::vector<double> Motor_Group::get_target_positions(void) {
	std::vector<double> out;
	claim_mg_mutex_vector(PROS_ERR_F);
	for (Motor motor : _motors) {
		out.push_back(motor.get_target_position());
	}
	give_mg_mutex_vector(PROS_ERR_F);
	return out;
}

std::vector<double> Motor_Group::get_positions(void) {
	std::vector<double> out;
	claim_mg_mutex_vector(PROS_ERR_F);
	for (Motor motor : _motors) {
		out.push_back(motor.get_position());
	}
	give_mg_mutex_vector(PROS_ERR_F);
	return out;
}

std::vector<double> Motor_Group::get_efficiencies(void) {
	std::vector<double> out;
	claim_mg_mutex_vector(PROS_ERR_F);
	for (Motor motor : _motors) {
		out.push_back(motor.get_efficiency());
	}
	give_mg_mutex_vector(PROS_ERR_F);
	return out;
}
std::vector<double> Motor_Group::get_actual_velocities(void) {
	std::vector<double> out;
	claim_mg_mutex_vector(PROS_ERR_F);
	for (Motor motor : _motors) {
		out.push_back(motor.get_actual_velocity());
	}
	give_mg_mutex_vector(PROS_ERR_F);
	return out;
}

std::vector<pros::motor_brake_mode_e_t> Motor_Group::get_brake_modes(void) {
	std::vector<pros::motor_brake_mode_e_t> out;
	claim_mg_mutex_vector(E_MOTOR_BRAKE_INVALID);
	for (Motor motor : _motors) {
		out.push_back(motor.get_brake_mode());
	}
	give_mg_mutex_vector(E_MOTOR_BRAKE_INVALID);
	return out;
}

std::vector<std::int32_t> Motor_Group::are_over_current(void) {
	std::vector<std::int32_t> out;
	claim_mg_mutex_vector(PROS_ERR);
	for (Motor motor : _motors) {
		out.push_back(motor.is_over_current());
	}
	give_mg_mutex_vector(PROS_ERR);
	return out;
}

std::vector<motor_gearset_e_t> Motor_Group::get_gearing(void) {
	std::vector<motor_gearset_e_t> out;
	claim_mg_mutex_vector(E_MOTOR_GEARSET_INVALID);
	for (Motor motor : _motors) {
		out.push_back(motor.get_gearing());
	}
	give_mg_mutex_vector(E_MOTOR_GEARSET_INVALID);
	return out;
}

std::vector<std::int32_t> Motor_Group::get_current_draws(void) {
	std::vector<std::int32_t> out;
	claim_mg_mutex_vector(PROS_ERR);
	for (Motor motor : _motors) {
		out.push_back(motor.get_current_draw());
	}
	give_mg_mutex_vector(PROS_ERR);
	return out;
}

std::vector<std::int32_t> Motor_Group::get_current_limits(void) {
	std::vector<std::int32_t> out;
	claim_mg_mutex_vector(PROS_ERR);
	for (Motor motor : _motors) {
		out.push_back(motor.get_current_limit());
	}
	give_mg_mutex_vector(PROS_ERR);
	return out;
}

std::vector<std::uint8_t> Motor_Group::get_ports(void) {
	std::vector<std::uint8_t> out;
	claim_mg_mutex_vector(PROS_ERR_BYTE);
	for (Motor motor : _motors) {
		out.push_back(motor.get_port());
	}
	give_mg_mutex_vector(PROS_ERR_BYTE);
	return out;
}

std::vector<std::int32_t> Motor_Group::get_directions(void) {
	std::vector<std::int32_t> out;
	claim_mg_mutex_vector(PROS_ERR);
	for (Motor motor : _motors) {
		out.push_back(motor.get_direction());
	}
	give_mg_mutex_vector(PROS_ERR);
	return out;
}

std::vector<std::int32_t> Motor_Group::get_target_velocities(void) {
	std::vector<std::int32_t> out;
	claim_mg_mutex_vector(PROS_ERR);
	for (Motor motor : _motors) {
		out.push_back(motor.get_target_velocity());
	}
	give_mg_mutex_vector(PROS_ERR);
	return out;
}

std::vector<std::int32_t> Motor_Group::are_over_temp(void) {
	std::vector<std::int32_t> out;
	claim_mg_mutex_vector(PROS_ERR);
	for (Motor motor : _motors) {
		out.push_back(motor.is_over_temp());
	}
	give_mg_mutex_vector(PROS_ERR);
	return out;
}

std::vector<pros::motor_encoder_units_e_t> Motor_Group::get_encoder_units(void) {
	std::vector<pros::motor_encoder_units_e_t> out;
	claim_mg_mutex_vector(E_MOTOR_ENCODER_INVALID);
	for (Motor motor : _motors) {
		out.push_back(motor.get_encoder_units());
	}
	give_mg_mutex_vector(E_MOTOR_ENCODER_INVALID);
	return out;
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
