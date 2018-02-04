/**
 * \file vdml_motors.h
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

#ifndef VDML_MOTORS_H
#define VDML_MOTORS_H

#include <stdbool.h>
#include <stdint.h>

/**
 * Represents the current 'brake mode' of a motor. It is unknown/undocumented
 * how this functions. Mirrors _V5_MotorBrakeMode
 */
typedef enum motor_brake_mode_e {
	E_MOTOR_BRAKE_COAST = 0,
	E_MOTOR_BRAKE_BRAKE = 1,
	E_MOTOR_BRAKE_HOLD = 2,
	E_MOTOR_BRAKE_INVALID = INT32_MAX
} motor_brake_mode_e;

/**
 * Indicates the units used by the motor encoders. Mirrors V5MotorEncoderUnits.
 */
typedef enum motor_encoder_units_e {
	E_MOTOR_ENCODER_DEGREES = 0,
	E_MOTOR_ENCODER_ROTATIONS = 1,
	E_MOTOR_ENCODER_COUNTS = 2,
	E_MOTOR_ENCODER_INVALID = INT32_MAX
} motor_encoder_units_e;

/**
 * Indicates the current internal gear ratio of a motor. Mirrors
 * _V5MotorGearset.
 */
typedef enum motor_gearset_e {
	E_MOTOR_GEARSET_36 = 0,
	E_MOTOR_GEARSET_18 = 1,
	E_MOTOR_GEARSET_06 = 2,
	E_MOTOR_GEARSET_INVALID = INT32_MAX
} motor_gearset_e;

/*
 * For the following functions:
 * If they don't need a return value (e.g. a setter):
 * 		They will return PROS_ERR upon failure and 1 upons success
 * If they should return a bool (e.g. motor_get_over_temp_flag):
 *		They will return 1 for true, 0 for false, and PROS_ERR upon failure
 * If they should return a 32-bit integer (e.g. a getter):
 *		They will return their value. If an error occurs, they will return
 *		PROS_ERR. If their actual return value should be PROS_ERR (INT32_MAX)
 *		then they will set errno = 0 to indicate no error has occured.
 * If they should return a double (e.g. a getter):
 *		They will return their value, or PROS_ERROR_F upon failure
 *
 * Upon returning PROS_ERR or PROS_ERR_F, errno will be set to indicate the
 * type of error. Again, some functions may return PROS_ERR as a valid value;
 * in this case, errno will be set to 0
 */

int32_t motor_set_velocity(int port, int16_t velocity);
int32_t motor_set_voltage(int port, int16_t voltage);
int32_t motor_get_velocity(int port);
double motor_get_actual_velocity(int port);
int32_t motor_get_direction(int port);
int32_t motor_set_current_limit(int port, int32_t limit);
int32_t motor_get_current_limit(int port);
int32_t motor_set_voltage_limit(int port, int32_t limit);
int32_t motor_get_voltage_limit(int port);
int32_t motor_get_current(int port);
double motor_get_voltage(int port);
double motor_get_power(int port);
double motor_get_torque(int port);
double motor_get_efficiency(int port);
double motor_get_temperature(int port);
int32_t motor_get_over_temp(int port);
int32_t motor_get_over_current_limit(int port);
uint32_t motor_get_faults(int port);
int32_t motor_get_zero_velocity(int port);
int32_t motor_get_zero_position(int port);
uint32_t motor_get_flags(int port);
int32_t motor_set_reverse(int port, bool reverse);
int32_t motor_get_reverse(int port);
int32_t motor_encoder_set_units(int port, motor_encoder_units_e units);
motor_encoder_units_e motor_encoder_get_units(int port);
int32_t motor_set_brake_mode(int port, motor_brake_mode_e mode);
motor_brake_mode_e motor_get_brake_mode(int port);
int32_t motor_set_position(int port, double position);
double motor_get_position(int port);
int32_t motor_get_raw_position(int port, uint32_t* timestamp);
int32_t motor_reset_position(int port);
double motor_get_target(int port);
int32_t motor_set_absolute_target(int port, double position, int32_t velocity);
int32_t motor_set_relative_target(int port, double position, int32_t velocity);
int32_t motor_set_gearing(int port, motor_gearset_e gearset);
motor_gearset_e motor_get_gearing(int port);

#endif
