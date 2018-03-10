/**
 * \file motors.hpp
 *
 * \brief Contains prototypes for the motor-related thread-safe wrapper
 * functions.
 *
 * This file contains the header info for the functions used to modify the
 * status of vex motors.
 *
 * \copyright (c) 2017-2018, Purdue University ACM SIGBots.
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
	Motor(uint8_t port,
	      const bool reverse = false,
	      motor_encoder_units_e_t encoder_units = E_MOTOR_ENCODER_DEGREES,
	      motor_gearset_e_t gearset = E_MOTOR_GEARSET_36);

	double get_actual_velocity(void);
	motor_brake_mode_e_t get_brake_mode(void);
	int32_t get_current(void);
	int32_t get_current_limit(void);
	int32_t get_current_limit_flag(void);
	int32_t get_direction(void);
	double get_efficiency(void);
	motor_encoder_units_e_t get_encoder_units(void);
	uint32_t get_faults(void);
	uint32_t get_flags(void);
	motor_gearset_e_t get_gearing(void);
	int32_t get_raw_position(uint32_t* const timestamp);
	int32_t get_temp_limit_flag(void);
	double get_position(void);
	double get_power(void);
	int32_t get_reverse(void);
	double get_temperature(void);
	double get_target(void);
	double get_torque(void);
	int32_t get_velocity(void);
	double get_voltage(void);
	int32_t get_voltage_limit(void);
	int32_t get_zero_velocity_flag(void);
	int32_t get_zero_position_flag(void);
	int32_t reset_position(void);
	int32_t set_absolute_target(const double position, const int32_t velocity);
	int32_t set_brake_mode(const motor_brake_mode_e_t mode);
	int32_t set_current_limit(const int32_t limit);
	int32_t set_encoder_units(const motor_encoder_units_e_t units);
	int32_t set_gearing(const motor_gearset_e_t gearset);
	int32_t set_position(const double position);
	int32_t set_relative_target(const double position, const int32_t velocity);
	int32_t set_reverse(const bool reverse);
	int32_t set_velocity(const int16_t velocity);
	int32_t set_voltage(const int16_t voltage);
	int32_t set_voltage_limit(const int32_t limit);

	private:
	uint8_t _port;
};
}
#endif
