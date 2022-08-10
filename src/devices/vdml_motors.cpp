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

#include <vector>

/**
 * Macro to claim the motor group mutex with the error code being PROS_ERR
 * 
 */
#define take_motor_group_mutex_int 						 \
	if (!_motor_group_mutex.take(TIMEOUT_MAX)) { \
		return PROS_ERR;													 \
	}																						 

/**
 * Macro to free the motor group mutex with the error value being PROS_ERR
 * 
 */
#define give_motor_group_mutex_int						 \
	if (!_motor_group_mutex.give()) {            \
		return PROS_ERR;                           \
	}

/**
 * Macro to loop through a function call for each motor in a motor group
 * with an error value of PROS_ERR
 * 
 */
#define motor_group_loop(function_call)                       \
	for(Motor motor : _motors) {                                \
		if (out != PROS_ERR && motor.function_call != PROS_ERR) { \
			out = 1;                                                \
		}                                                         \
		else {                                                    \
			out = PROS_ERR;                                         \
		}                                                         \
	}

/**
 * Macro to take the motor group mutex with a vector of error as the error value
 * 
 */
#define take_motor_group_mutex_vector(error) \
if (!_motor_group_mutex.take(TIMEOUT_MAX)) { \
		out.clear();														 \
		for (Motor m : _motors) {								 \
			out.push_back(error);						       \
		}																				 \
		out.resize(_motors.size());							 \
		out.shrink_to_fit();                     \
		return out;                              \
	}

/**
 * Macro to free the motor group mutex with a vector of error as the error value
 * 
 */
#define give_motor_group_mutex_vector(error) \
	if (!_motor_group_mutex.give()) {					 \
		out.clear();														 \
		for (Motor m : _motors) {								 \
			out.push_back(error);									 \
		}																				 \
	}

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
Motor_Group::Motor_Group(const std::initializer_list<Motor> motors) : _motors(motors), _motor_group_mutex(pros::Mutex()) {}

std::int32_t Motor_Group::move(std::int32_t voltage) {
	take_motor_group_mutex_int;
	std::int32_t out = 0;
	motor_group_loop(move(voltage));
	give_motor_group_mutex_int;
	return out;
}

std::int32_t Motor_Group::operator=(std::int32_t voltage) {
	take_motor_group_mutex_int;
	std::int32_t out = 0;
	motor_group_loop(move(voltage));
	give_motor_group_mutex_int;
	return out;
}

std::int32_t Motor_Group::move_absolute(const double position, const std::int32_t velocity) {
	take_motor_group_mutex_int;
	std::int32_t out = 0;
	motor_group_loop(move_absolute(position, velocity));
	give_motor_group_mutex_int;
	return out;
}

std::int32_t Motor_Group::move_relative(const double position, const std::int32_t velocity) {
take_motor_group_mutex_int;
	std::int32_t out = 0;
	motor_group_loop(move_relative(position, velocity));
	give_motor_group_mutex_int;
	return out;
}

std::int32_t Motor_Group::move_velocity(const std::int32_t velocity) {
	take_motor_group_mutex_int;
	std::int32_t out = 0;
	motor_group_loop(move_velocity(velocity));
	give_motor_group_mutex_int;
	return out;
}

std::int32_t Motor_Group::move_voltage(const std::int32_t voltage) {
	take_motor_group_mutex_int;
	std::int32_t out = 0;
	motor_group_loop(move_voltage(voltage));
	give_motor_group_mutex_int;
	return out;
}

std::int32_t Motor_Group::brake(void) {
	take_motor_group_mutex_int;
	std::int32_t out = 0;
	motor_group_loop(brake());
	give_motor_group_mutex_int;
	return out;
}

std::int32_t Motor_Group::set_zero_position(const double position) {
	take_motor_group_mutex_int;
	std::int32_t out = 0;
	motor_group_loop(set_zero_position(position));
	give_motor_group_mutex_int;
	return out;
}

std::int32_t Motor_Group::set_reversed(const bool reversed) {
	take_motor_group_mutex_int;
	std::int32_t out = 0;
	motor_group_loop(set_reversed(reversed));
	give_motor_group_mutex_int;
	return out;
}

std::int32_t Motor_Group::set_voltage_limit(const std::int32_t limit) {
	take_motor_group_mutex_int;
	std::int32_t out = 0;
	motor_group_loop(set_voltage_limit(limit));
	give_motor_group_mutex_int;
	return out;
}

std::int32_t Motor_Group::set_gearing(const motor_gearset_e_t gearset) {
	take_motor_group_mutex_int;
	std::int32_t out = 0;
	motor_group_loop(set_gearing(gearset));
	give_motor_group_mutex_int;
	return out;
}

std::int32_t Motor_Group::set_encoder_units(const motor_encoder_units_e_t units) {
	take_motor_group_mutex_int;
	std::int32_t out = 0;
	motor_group_loop(set_encoder_units(units));
	give_motor_group_mutex_int;
	return out;
}

std::vector<double> Motor_Group::get_target_positions(void) {
	std::vector<double> out;
	take_motor_group_mutex_vector(PROS_ERR_F);
	for(Motor motor : _motors) {
		double temp = motor.get_target_position();
		if (temp == PROS_ERR_F) {
			out.clear();
			for (Motor m : _motors) {
			out.push_back(PROS_ERR_F);
			}
			break;
		}
		else {
			out.push_back(temp);
		}
	}
	out.resize(_motors.size());
	out.shrink_to_fit();
	give_motor_group_mutex_vector(PROS_ERR_F);
	return out;
}
std::vector<double> Motor_Group::get_efficiencies(void) {
	std::vector<double> out;
	take_motor_group_mutex_vector(PROS_ERR_F);
	for(Motor motor : _motors) {
		double temp = motor.get_efficiency();
		if (temp == PROS_ERR_F) {
			out.clear();
			for (Motor m : _motors) {
			out.push_back(PROS_ERR_F);
			}
			break;
		}
		else {
			out.push_back(temp);
		}
	}
	out.resize(_motors.size());
	out.shrink_to_fit();
	give_motor_group_mutex_vector(PROS_ERR_F);
	return out;
}
std::vector<double> Motor_Group::get_actual_velocities(void) {
	std::vector<double> out;
	take_motor_group_mutex_vector(PROS_ERR_F);
	for(Motor motor : _motors) {
		double temp = motor.get_target_position();
		if (temp == PROS_ERR_F) {
			out.clear();
			for (Motor m : _motors) {
				out.push_back(PROS_ERR_F);
			}
			break;
		}
		else {
			out.push_back(temp);
		}
	}
	out.resize(_motors.size());
	out.shrink_to_fit();
	give_motor_group_mutex_vector(PROS_ERR_F);
	return out;
}

std::vector<pros::motor_brake_mode_e_t> Motor_Group::get_brake_modes(void) {
	std::vector<pros::motor_brake_mode_e_t> out;
	take_motor_group_mutex_vector(E_MOTOR_BRAKE_INVALID);
	for(Motor motor : _motors) {
		pros::motor_brake_mode_e_t temp = motor.get_brake_mode();
		if (temp == E_MOTOR_BRAKE_INVALID) {
			out.clear();
			for (Motor m : _motors) {
			out.push_back(E_MOTOR_BRAKE_INVALID);
			}
			break;
		}
		else {
			out.push_back(temp);
		}
	}
	out.resize(_motors.size());
	out.shrink_to_fit();
	give_motor_group_mutex_vector(E_MOTOR_BRAKE_INVALID);
	return out;
}
std::vector<std::int32_t> Motor_Group::are_over_current(void) {
	std::vector<std::int32_t> out;
	take_motor_group_mutex_vector(PROS_ERR);
	for(Motor motor : _motors) {
		std::int32_t temp = motor.is_over_current();
		if (temp == PROS_ERR) {
			out.clear();
			for (Motor m : _motors) {
				out.push_back(PROS_ERR);
			}
			break;
		} else {
			out.push_back(PROS_ERR);
		}
	}
	out.resize(_motors.size());
	out.shrink_to_fit();
	give_motor_group_mutex_vector(PROS_ERR);
	return out;
}
std::vector<std::int32_t> Motor_Group::get_current_draws(void) {
	std::vector<std::int32_t> out;
	take_motor_group_mutex_vector(PROS_ERR);
	for(Motor motor : _motors) {
		std::int32_t temp = motor.get_current_draw();
		if (temp == PROS_ERR) {
			out.clear();
			for (Motor m : _motors) {
				out.push_back(PROS_ERR);
			}
			break;
		} else {
			out.push_back(PROS_ERR);
		}
	}
	out.resize(_motors.size());
	out.shrink_to_fit();
	give_motor_group_mutex_vector(PROS_ERR);
	return out;
}

std::vector<std::int32_t> Motor_Group::get_current_limits(void) {
	std::vector<std::int32_t> out;
	take_motor_group_mutex_vector(PROS_ERR);
	for(Motor motor : _motors) {
		std::int32_t temp = motor.get_current_limit();
		if (temp == PROS_ERR) {
			out.clear();
			for (Motor m : _motors) {
				out.push_back(PROS_ERR);
			}
			break;
		}
		else {
			out.push_back(temp);
		}
	}
	out.resize(_motors.size());
	out.shrink_to_fit();
	give_motor_group_mutex_vector(PROS_ERR);
	return out;
}

std::vector<std::uint8_t> Motor_Group::get_ports(void) {
	std::vector<std::uint8_t> out;
	for(Motor motor : _motors) {
		out.push_back(motor.get_port());
	}
	out.resize(_motors.size());
	out.shrink_to_fit();
	return out;
}

std::vector<std::int32_t> Motor_Group::get_directions(void) {
	std::vector<std::int32_t> out;
	take_motor_group_mutex_vector(PROS_ERR);
	for(Motor motor : _motors) {
		std::int32_t temp = motor.get_direction();
		if (temp == PROS_ERR) {
			out.clear();
			for (Motor m : _motors) {
				out.push_back(PROS_ERR);
			}
			break;
		} 
		else {
			out.push_back(temp);
		}
	}
	out.resize(_motors.size());
	out.shrink_to_fit();
	give_motor_group_mutex_vector(PROS_ERR);
	return out;
}

std::vector<std::int32_t> Motor_Group::get_target_velocities(void) {
	std::vector<std::int32_t> out;
	take_motor_group_mutex_vector(PROS_ERR);
	for(Motor motor : _motors) {
		std::int32_t temp = motor.get_target_velocity();
		if (temp == PROS_ERR) {
			out.clear();
			for (Motor m : _motors) {
				out.push_back(PROS_ERR);
			}
			break;
		} 
		else {
			out.push_back(temp);
		}
	}
	out.resize(_motors.size());
	out.shrink_to_fit();
	give_motor_group_mutex_vector(PROS_ERR);
	return out;
}

std::vector<std::int32_t> Motor_Group::are_over_temp(void) {
	std::vector<std::int32_t> out;
	take_motor_group_mutex_vector(PROS_ERR);
	for(Motor motor : _motors) {
		std::int32_t temp = motor.is_over_temp();
		if (temp == PROS_ERR) {
			out.clear();
			for (Motor m : _motors) {
				out.push_back(PROS_ERR);
			}
			break;
		} 
		else {
			out.push_back(temp);
		}
	}
	out.resize(_motors.size());
	out.shrink_to_fit();
	give_motor_group_mutex_vector(PROS_ERR);
	return out;
}

std::vector<pros::motor_encoder_units_e_t> Motor_Group::get_encoder_units(void) {
	std::vector<pros::motor_encoder_units_e_t> out;

	take_motor_group_mutex_vector(E_MOTOR_ENCODER_INVALID);
	for(Motor motor : _motors) {
		pros::motor_encoder_units_e_t temp = motor.get_encoder_units();
		if (temp == E_MOTOR_ENCODER_INVALID) {
			out.clear();
			for (Motor m : _motors) {
				out.push_back(E_MOTOR_ENCODER_INVALID);
			}
			break;
		} else {
			out.push_back(temp);
		}
	}
	out.resize(_motors.size());
	out.shrink_to_fit();
	give_motor_group_mutex_vector(E_MOTOR_ENCODER_INVALID);
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
