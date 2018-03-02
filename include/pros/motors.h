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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Indicates the current 'brake mode' of a motor.
 */
typedef enum motor_brake_mode_e {
	E_MOTOR_BRAKE_COAST = 0,  // Motor coasts when stopped, traditional behavior
	E_MOTOR_BRAKE_BRAKE = 1,  // Motor brakes when stopped
	E_MOTOR_BRAKE_HOLD = 2,   // Motor actively holds position when stopped
	E_MOTOR_BRAKE_INVALID = INT32_MAX
} motor_brake_mode_e_t;

/**
 * Indicates the units used by the motor encoders.
 */
typedef enum motor_encoder_units_e {
	E_MOTOR_ENCODER_DEGREES = 0,
	E_MOTOR_ENCODER_ROTATIONS = 1,
	E_MOTOR_ENCODER_COUNTS = 2,
	E_MOTOR_ENCODER_INVALID = INT32_MAX
} motor_encoder_units_e_t;

/**
 * Indicates the current internal gear ratio of a motor.
 */
typedef enum motor_gearset_e {
	E_MOTOR_GEARSET_36 = 0,  // 36:1, 100 RPM, Red gear set
	E_MOTOR_GEARSET_18 = 1,  // 18:1, 200 RPM, Green gear set
	E_MOTOR_GEARSET_06 = 2,  // 6:1, 600 RPM, Blue gear set
	E_MOTOR_GEARSET_INVALID = INT32_MAX
} motor_gearset_e_t;

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

/**
 * \brief Gets the actual velocity of the motor
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The motor's actual velocity in motor_encoder_units_e_t per second
 *         or PROS_ERR_F if the operation failed, setting errno.
 */
double motor_get_actual_velocity(uint8_t port);

/**
 * \brief Gets the brake mode of the motor
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return One of motor_brake_mode_e_t, according to what was set for the motor,
 *          or E_MOTOR_BRAKE_INVALID if the operation failed, setting errno.
 */
motor_brake_mode_e_t motor_get_brake_mode(uint8_t port);

/**
 * \brief Gets the current drawn by the motor in mA
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The motor's current in mA or PROS_ERR if the operation failed,
 *         setting errno.
 */
int32_t motor_get_current(uint8_t port);

/**
 * \brief Gets the current limit for the motor in mA
 *
 * The default value is 2500 mA.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The motor's current limit in mA or PROS_ERR if the operation failed,
 *         setting errno.
 */
int32_t motor_get_current_limit(uint8_t port);

/**
 * \brief Gets the current limit flag for the motor
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return 1 if the motor's current limit is being exceeded and 0 if the current
 *         limit is not exceeded, or PROS_ERR if the operation failed, setting
 *         errno.
 */
int32_t motor_get_current_limit_flag(uint8_t port);

/**
 * \brief Gets the direction of movement for the motor
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return 1 for moving in the positive direction, -1 for moving in the
 *         negative direction, and PROS_ERR if the operation failed,
 *         setting errno.
 */
int32_t motor_get_direction(uint8_t port);

/**
 * \brief Gets the efficiency of the motor in percent
 *
 * An efficiency of 100% means that the motor is moving electrically while
 * drawing no electrical power, and an efficiency of 0% means that the motor
 * is drawing power but not moving.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The motor's efficiency in percent or PROS_ERR_F if the operation
 *         failed, setting errno.
 */
double motor_get_efficiency(uint8_t port);

/**
 * \brief Gets the encoder units set for the motor
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return One of motor_encoder_units_e_t according to what is set for the motor
 *         or E_MOTOR_ENCODER_INVALID if the operation failed.
 */
motor_encoder_units_e_t motor_get_encoder_units(uint8_t port);

/**
 * \brief Gets the faults experienced by the motor
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return A currently unknown bitfield containing the motor's faults.
 *         0b00000100 = Current Limit Hit
 */
uint32_t motor_get_faults(uint8_t port);

/**
 * \brief Gets the flags set by the motor's operation
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return A currently unknown bitfield containing the motor's flags. These seem
 *         to be unrelated to the individual motor_get_specific_flag functions
 */
uint32_t motor_get_flags(uint8_t port);

/**
 * \brief Gets the gearset that was set for the motor
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return One of motor_gearset_e_t according to what is set for the motor,
 *         or E_GEARSET_INVALID if the operation failed.
 */
motor_gearset_e_t motor_get_gearing(uint8_t port);

/**
 * \brief Gets the raw encoder count of the motor at a given timestamp
 *
 * \param port
 *        The V5 port number from 1-21
 * \param timestamp
 *        A pointer to a time in milliseconds for which the
 *        encoder count will be returned
 *
 * \return The raw encoder count at the given timestamp or PROS_ERR if the
 *         operation failed.
 */
int32_t motor_get_raw_position(uint8_t port, uint32_t* const timestamp);

/**
 * \brief Gets the temperature limit flag for the motor
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return 1 if the temperature limit is exceeded and 0 if the the
 *         temperature is below the limit, or PROS_ERR if the operation failed,
 *         setting errno.
 */
int32_t motor_get_temp_limit_flag(uint8_t port);

/**
 * \brief Gets the absolute position of the motor in its encoder units.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The motor's absolute position in its encoder units or PROS_ERR_F
 *         if the operation failed, setting errno.
 */
double motor_get_position(uint8_t port);

/**
 * \brief Gets the power drawn by the motor in Watts
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The motor's power draw in Watts or PROS_ERR_F if the operation
 *         failed, setting errno.
 */
double motor_get_power(uint8_t port);

/**
 * \brief Gets the operation direction of the motor as set by the user
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return 1 if the motor has been reversed and 0 if the motor was not reversed,
 *         or PROS_ERR if the operation failed, setting errno.
 */
int32_t motor_get_reverse(uint8_t port);

/**
 * \brief Gets the temperature of the motor in degrees Celsius
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The motor's temperature in degrees Celsius or PROS_ERR_F if the
 *         operation failed, setting errno.
 */
double motor_get_temperature(uint8_t port);

/**
 * \brief Gets the target position set for the motor by the user
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The target position in its encoder units or PROS_ERR_F if the
 *         operation failed, setting errno.
 */
double motor_get_target(uint8_t port);

/**
 * \brief Gets the torque generated by the motor in NM
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The motor's torque in NM or PROS_ERR_F if the operation failed,
 *         setting errno.
 */
double motor_get_torque(uint8_t port);

/**
 * \brief Gets the velocity commanded to the motor by the user
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The commanded motor velocity from -128 to 127 or PROS_ERR if the
 *         operation failed, setting errno.
 */
int32_t motor_get_velocity(uint8_t port);

/**
 * \brief Gets the voltage delivered to the motor in V
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The motor's voltage in V or PROS_ERR_F if the operation failed,
 *         setting errno.
 */
double motor_get_voltage(uint8_t port);

/**
 * \brief Gets the voltage limit set by the user
 *
 * Default value is 0V, which I presume means that the limit does not exist.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The motor's voltage limit in V or PROS_ERR if the operation failed,
 *         setting errno.
 */
int32_t motor_get_voltage_limit(uint8_t port);

/**
 * \brief Gets the zero velocity flag for the motor
 *
 * \note Currently does not work
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return 1 if the motor is not moving and 0 if the motor is moving, or PROS_ERR
 *           if the operation failed, setting errno.
 */
int32_t motor_get_zero_velocity_flag(uint8_t port);

/**
 * \brief Gets the zero position flag for the motor
 *
 * \note Currently does not work
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return 1 if the motor is at zero absolute position and 0 if the motor has
 *         moved from its absolute zero, or PROS_ERR if the operation failed
 *         setting errno.
 */
int32_t motor_get_zero_position_flag(uint8_t port);

/**
 * \brief Sets the "absolute" zero position of the motor to its current position
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation failed,
 *           setting errno.
 */
int32_t motor_reset_position(uint8_t port);

/**
 * \brief Sets the target absolute position for the motor to move to.
 *
 * This movement is relative to the position of the motor when initialized or
 * the position when it was most recently reset with motor_reset_position.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param position
 *        The absolute position to move to in the motor's encoder units
 * \param velocity
 *        The maximum allowable velocity for the movement
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation failed,
 *         setting errno.
 */
int32_t motor_set_absolute_target(uint8_t port, const double position, const int32_t velocity);

/**
 * \brief Sets one of motor_brake_mode_e_t to the motor
 *
 * \param port
 *        The V5 port number from 1-21
 * \param mode
 *        The motor_brake_mode_e_t to set for the motor
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation failed,
 *         setting errno.
 */
int32_t motor_set_brake_mode(uint8_t port, const motor_brake_mode_e_t mode);

/**
 * \brief Sets the current limit for the motor in mA
 *
 * \param port
 *        The V5 port number from 1-21
 * \param limit
 *        The new current limit in mA
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation failed,
 *         setting errno.
 */
int32_t motor_set_current_limit(uint8_t port, const int32_t limit);

/**
 * \brief Sets one of motor_encoder_units_e_t for the motor encoder
 *
 * \param port
 *        The V5 port number from 1-21
 * \param units
 *        The new motor encoder units
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation failed,
 *         setting errno.
 */
int32_t motor_set_encoder_units(uint8_t port, const motor_encoder_units_e_t units);

/**
 * \brief Sets one of motor_gearset_e_t for the motor
 *
 * \param port
 *        The V5 port number from 1-21
 * \param gearset
 *        The new motor gearset
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation failed,
 *         setting errno.
 */
int32_t motor_set_gearing(uint8_t port, const motor_gearset_e_t gearset);

/**
 * \brief Sets the position for the motor in its encoder units.
 *
 * This will be the future reference point for the motor's "absolute" position.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param position
 *        The new reference position in its encoder units
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation failed,
 *         setting errno.
 */
int32_t motor_set_position(uint8_t port, const double position);

/**
 * \brief Sets the relative target position for the motor to move to.
 *
 * This movement is relative to the current position of the motor as given in
 * motor_get_position.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param position
 *        The relative position to move to in the motor's encoder units
 * \param velocity
 *        The maximum allowable velocity for the movement
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation failed,
 *         setting errno.
 */
int32_t motor_set_relative_target(uint8_t port, const double position, const int32_t velocity);

/**
 * \brief Sets the reverse flag for the motor.
 *
 * This will invert its movements and the values returned for its position.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param reverse
 *        True reverses the motor, false is default
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation failed,
 *         setting errno.
 */
int32_t motor_set_reverse(uint8_t port, const bool reverse);

/**
 * \brief Sets the velocity for the motor from -128 to 127.
 *
 * This velocity corresponds to different actual speeds depending on the gearset
 * used for the motor. The velocity is held with PID to ensure consistent speed,
 * as opposed to setting the motor's voltage.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param velocity
 *        The new motor velocity from -127 to 127
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation failed,
 *         setting errno.
 */
int32_t motor_set_velocity(uint8_t port, const int16_t velocity);

/**
 * \brief Sets the voltage for the motor from -128 to 127.
 *
 * This voltage is controlled by PWM, and does not immediately correspond to
 * the value returned by motor_get_voltage (which is in Volts)
 *
 * \param port
 *        The V5 port number from 1-21
 * \param voltage
 *        The new PWM value from -127 to 127
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation failed,
 *         setting errno.
 */
int32_t motor_set_voltage(uint8_t port, const int16_t voltage);

/**
 * \brief Sets the voltage limit for the motor in Volts
 *
 * \param port
 *        The V5 port number from 1-21
 * \param limit
 *        The new voltage limit in Volts
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation failed,
 *         setting errno.
 */
int32_t motor_set_voltage_limit(uint8_t port, const int32_t limit);

#ifdef __cplusplus
}
#endif

#endif
