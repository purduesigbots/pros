/**
 * \file motors.hpp
 *
 * \brief Contains prototypes for the Smart Motor-related functions in C++.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/motors to learn more.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * LIcense, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _PROS_MOTORS_HPP_
#define _PROS_MOTORS_HPP_

#include "pros/motors.h"

namespace pros {
class Motor {
	public:
	Motor(std::uint8_t port,
	      const bool reverse = false,
	      motor_encoder_units_e_t encoder_units = E_MOTOR_ENCODER_DEGREES,
	      motor_gearset_e_t gearset = E_MOTOR_GEARSET_36);

	double get_actual_velocity(void) const;
	motor_brake_mode_e_t get_brake_mode(void) const;
	std::int32_t get_current(void) const;
	std::int32_t get_current_limit(void) const;
	std::int32_t get_current_limit_flag(void) const;
	std::int32_t get_direction(void) const;
	double get_efficiency(void) const;
	motor_encoder_units_e_t get_encoder_units(void) const;
	std::uint32_t get_faults(void) const;
	std::uint32_t get_flags(void) const;
	motor_gearset_e_t get_gearing(void) const;
	std::int32_t get_raw_position(std::uint32_t* const timestamp) const;
	std::int32_t get_temp_limit_flag(void) const;
	double get_position(void) const;
	double get_power(void) const;
	std::int32_t get_reverse(void) const;
	double get_temperature(void) const;
	double get_target(void) const;
	double get_torque(void) const;
	std::int32_t get_velocity(void) const;
	double get_voltage(void) const;
	std::int32_t get_voltage_limit(void) const;
	std::int32_t get_zero_velocity_flag(void) const;
	std::int32_t get_zero_position_flag(void) const;
	std::int32_t reset_position(void) const;
	std::int32_t set_absolute_target(const double position, const std::int32_t velocity) const;
	std::int32_t set_brake_mode(const motor_brake_mode_e_t mode) const;
	std::int32_t set_current_limit(const std::int32_t limit) const;
	std::int32_t set_encoder_units(const motor_encoder_units_e_t units) const;
	std::int32_t set_gearing(const motor_gearset_e_t gearset) const;
	std::int32_t set_position(const double position) const;
	std::int32_t set_relative_target(const double position, const std::int32_t velocity) const;
	std::int32_t set_reverse(const bool reverse) const;
	std::int32_t set_velocity(const std::int16_t velocity) const;
	std::int32_t set_voltage(const std::int16_t voltage) const;
	std::int32_t set_voltage_limit(const std::int32_t limit) const;

	private:
	std::uint8_t _port;
};
}
#endif
