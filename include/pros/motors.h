/**
 * \file pros/motors.h
 *
 * Contains prototypes for the Smart Motor-related functions.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/motors to learn more.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_MOTORS_H_
#define _PROS_MOTORS_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
namespace pros {
namespace c {
#endif

/******************************************************************************/
/**                         Motor movement functions                         **/
/**                                                                          **/
/**          These functions allow programmers to make motors move           **/
/******************************************************************************/

/**
 * Sets the voltage for the motor from -127 to 127.
 *
 * This is designed to map easily to the input from the controller's analog
 * stick for simple opcontrol use. The actual behavior of the motor is analogous
 * to use of motor_move_voltage(), or motorSet() from the PROS 2 API.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param voltage
 *        The new motor voltage from -127 to 127
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed,
 *         setting errno.
 */
int32_t motor_move(uint8_t port, const int8_t voltage);

/**
 * Sets the target absolute position for the motor to move to.
 *
 * This movement is relative to the position of the motor when initialized or
 * the position when it was most recently reset with motor_set_zero_position().
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param position
 *        The absolute position to move to in the motor's encoder units
 * \param velocity
 *        The maximum allowable velocity for the movement in RPM
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed,
 *         setting errno.
 */
int32_t motor_move_absolute(uint8_t port, const double position, const int32_t velocity);

/**
 * Sets the relative target position for the motor to move to.
 *
 * This movement is relative to the current position of the motor as given in
 * motor_get_position(). Providing 10.0 as the position parameter would result
 * in the motor
 * moving clockwise 10 units, no matter what the current position is.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param position
 *        The relative position to move to in the motor's encoder units
 * \param velocity
 *        The maximum allowable velocity for the movement in RPM
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed,
 *         setting errno.
 */
int32_t motor_move_relative(uint8_t port, const double position, const int32_t velocity);

/**
 * Sets the velocity for the motor.
 *
 * This velocity corresponds to different actual speeds depending on the gearset
 * used for the motor. This results in a range of +-100 for E_MOTOR_GEARSET_36,
 * +-200 for E_MOTOR_GEARSET_18, and +-600 for E_MOTOR_GEARSET_6. The velocity
 * is held with PID to ensure consistent speed, as opposed to setting the
 * motor's
 * voltage.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param velocity
 *        The new motor velocity from +-100, +-200, or +-600 depending on the
 * motor's
 *        gearset
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed,
 *         setting errno.
 */
int32_t motor_move_velocity(uint8_t port, const int32_t velocity);

/**
 * Sets the output voltage for the motor from -12000 to 12000 in millivolts
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param voltage
 *        The new voltage value from -12000 to 12000
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed,
 *         setting errno.
 */
int32_t motor_move_voltage(uint8_t port, const int32_t voltage);

/**
 * Gets the target position set for the motor by the user.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The target position in its encoder units or PROS_ERR_F if the
 *         operation failed, setting errno.
 */
double motor_get_target_position(uint8_t port);

/**
 * Gets the velocity commanded to the motor by the user.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The commanded motor velocity from +-100, +-200, or +-600, or PROS_ERR
 *         if the operation failed, setting errno.
 */
int32_t motor_get_target_velocity(uint8_t port);

/******************************************************************************/
/**                        Motor telemetry functions                         **/
/**                                                                          **/
/**    These functions allow programmers to collect telemetry from motors    **/
/******************************************************************************/

/**
 * Gets the actual velocity of the motor.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The motor's actual velocity in motor_encoder_units_e_t per second
 *         or PROS_ERR_F if the operation failed, setting errno.
 */
double motor_get_actual_velocity(uint8_t port);

/**
 * Gets the current drawn by the motor in mA.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The motor's current in mA or PROS_ERR if the operation failed,
 *         setting errno.
 */
int32_t motor_get_current_draw(uint8_t port);

/**
 * Gets the direction of movement for the motor.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
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
 * Gets the efficiency of the motor in percent.
 *
 * An efficiency of 100% means that the motor is moving electrically while
 * drawing no electrical power, and an efficiency of 0% means that the motor
 * is drawing power but not moving.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The motor's efficiency in percent or PROS_ERR_F if the operation
 *         failed, setting errno.
 */
double motor_get_efficiency(uint8_t port);

/**
 * Checks if the motor is drawing over its current limit.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return 1 if the motor's current limit is being exceeded and 0 if the current
 *         limit is not exceeded, or PROS_ERR if the operation failed, setting
 *         errno.
 */
int32_t motor_is_over_current(uint8_t port);

/**
 * Checks if the motor's temperature is above its limit.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return 1 if the temperature limit is exceeded and 0 if the the
 *         temperature is below the limit, or PROS_ERR if the operation failed,
 *         setting errno.
 */
int32_t motor_is_over_temp(uint8_t port);

/**
 * Checks if the motor is stopped.
 *
 * \note Although this function forwards data from the motor, the motor
 * presently
 * does not provide any value. This function returns PROS_ERR with errno set to
 * ENOSYS.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return 1 if the motor is not moving, 0 if the motor is moving, or PROS_ERR
 *         if the operation failed, setting errno
 */
int32_t motor_is_stopped(uint32_t port);

/**
 * Checks if the motor is at its zero position.
 *
 * \note Although this function forwards data from the motor, the motor
 * presently
 * does not provide any value. This function returns PROS_ERR with errno set to
 * ENOSYS.
 *
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return 1 if the motor is at zero absolute position, 0 if the motor has
 *         moved from its absolute zero, or PROS_ERR if the operation failed,
 *         setting errno
 */
int32_t motor_get_zero_position_flag(uint32_t port);

/**
 * Gets the faults experienced by the motor.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return A currently unknown bitfield containing the motor's faults.
 *         0b00000100 = Current Limit Hit
 */
uint32_t motor_get_faults(uint8_t port);

/**
 * Gets the flags set by the motor's operation.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return A currently unknown bitfield containing the motor's flags. These seem
 *         to be unrelated to the individual motor_get_specific_flag functions
 */
uint32_t motor_get_flags(uint8_t port);

/**
 * Gets the raw encoder count of the motor at a given timestamp.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param timestamp[in]
 *        A pointer to a time in milliseconds for which the encoder count will
 * be returned.
 *        If NULL, the timestamp at which the encoder count was read will not be
 * supplied
 *
 * \return The raw encoder count at the given timestamp or PROS_ERR if the
 *         operation failed.
 */
int32_t motor_get_raw_position(uint8_t port, uint32_t* const timestamp);

/**
 * Gets the absolute position of the motor in its encoder units.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The motor's absolute position in its encoder units or PROS_ERR_F
 *         if the operation failed, setting errno.
 */
double motor_get_position(uint8_t port);

/**
 * Gets the power drawn by the motor in Watts.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The motor's power draw in Watts or PROS_ERR_F if the operation
 *         failed, setting errno.
 */
double motor_get_power(uint8_t port);

/**
 * Gets the temperature of the motor in degrees Celsius.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The motor's temperature in degrees Celsius or PROS_ERR_F if the
 *         operation failed, setting errno.
 */
double motor_get_temperature(uint8_t port);

/**
 * Gets the torque generated by the motor in Newton Metres (Nm).
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The motor's torque in NM or PROS_ERR_F if the operation failed,
 *         setting errno.
 */
double motor_get_torque(uint8_t port);

/**
 * Gets the voltage delivered to the motor in millivolts.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The motor's voltage in V or PROS_ERR_F if the operation failed,
 *         setting errno.
 */
int32_t motor_get_voltage(uint8_t port);

/******************************************************************************/
/**                      Motor configuration functions                       **/
/**                                                                          **/
/**  These functions allow programmers to configure the behavior of motors   **/
/******************************************************************************/

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

/**
 * Sets the position for the motor in its encoder units.
 *
 * This will be the future reference point for the motor's "absolute" position.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param position
 *        The new reference position in its encoder units
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed,
 *         setting errno.
 */
int32_t motor_set_zero_position(uint8_t port, const double position);

/**
 * Sets the "absolute" zero position of the motor to its current position.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed,
 *           setting errno.
 */
int32_t motor_tare_position(uint8_t port);

/**
 * Sets one of motor_brake_mode_e_t to the motor.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param mode
 *        The motor_brake_mode_e_t to set for the motor
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed,
 *         setting errno.
 */
int32_t motor_set_brake_mode(uint8_t port, const motor_brake_mode_e_t mode);

/**
 * Sets the current limit for the motor in mA.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param limit
 *        The new current limit in mA
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed,
 *         setting errno.
 */
int32_t motor_set_current_limit(uint8_t port, const int32_t limit);

/**
 * Sets one of motor_encoder_units_e_t for the motor encoder.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param units
 *        The new motor encoder units
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed,
 *         setting errno.
 */
int32_t motor_set_encoder_units(uint8_t port, const motor_encoder_units_e_t units);

/**
 * Sets one of motor_gearset_e_t for the motor.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param gearset
 *        The new motor gearset
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed,
 *         setting errno.
 */
int32_t motor_set_gearing(uint8_t port, const motor_gearset_e_t gearset);

/**
 * Sets the reverse flag for the motor.
 *
 * This will invert its movements and the values returned for its position.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param reverse
 *        True reverses the motor, false is default
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed,
 *         setting errno.
 */
int32_t motor_set_reversed(uint8_t port, const bool reverse);

/**
 * Sets the voltage limit for the motor in Volts.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param limit
 *        The new voltage limit in Volts
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed,
 *         setting errno.
 */
int32_t motor_set_voltage_limit(uint8_t port, const int32_t limit);

/**
 * Gets the brake mode that was set for the motor.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return One of motor_brake_mode_e_t, according to what was set for the motor,
 *          or E_MOTOR_BRAKE_INVALID if the operation failed, setting errno.
 */
motor_brake_mode_e_t motor_get_brake_mode(uint8_t port);

/**
 * Gets the current limit for the motor in mA.
 *
 * The default value is 2500 mA.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The motor's current limit in mA or PROS_ERR if the operation failed,
 *         setting errno.
 */
int32_t motor_get_current_limit(uint8_t port);

/**
 * Gets the encoder units that were set for the motor.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return One of motor_encoder_units_e_t according to what is set for the motor
 *         or E_MOTOR_ENCODER_INVALID if the operation failed.
 */
motor_encoder_units_e_t motor_get_encoder_units(uint8_t port);

/**
 * Gets the gearset that was set for the motor.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return One of motor_gearset_e_t according to what is set for the motor,
 *         or E_GEARSET_INVALID if the operation failed.
 */
motor_gearset_e_t motor_get_gearing(uint8_t port);

/**
 * Gets the operation direction of the motor as set by the user.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return 1 if the motor has been reversed and 0 if the motor was not reversed,
 *         or PROS_ERR if the operation failed, setting errno.
 */
int32_t motor_is_reversed(uint8_t port);

/**
 * Gets the voltage limit set by the user.
 *
 * Default value is 0V, which means that there is no software limitation imposed
 * on the voltage.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The motor's voltage limit in V or PROS_ERR if the operation failed,
 *         setting errno.
 */
int32_t motor_get_voltage_limit(uint8_t port);

#ifdef __cplusplus
}
}
}
#endif

#endif  // _PROS_MOTORS_H_
