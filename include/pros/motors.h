/**
 * \file pros/motors.h
 *
 * Contains prototypes for the V5 Motor-related functions.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/motors.html to learn
 * more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
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
 * failed, setting errno.
 */
int32_t motor_move(uint8_t port, int32_t voltage);

/**
 * Sets the target absolute position for the motor to move to.
 *
 * This movement is relative to the position of the motor when initialized or
 * the position when it was most recently reset with motor_set_zero_position().
 *
 * \note This function simply sets the target for the motor, it does not block
 * program execution until the movement finishes.
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
 * failed, setting errno.
 */
int32_t motor_move_absolute(uint8_t port, const double position, const int32_t velocity);

/**
 * Sets the relative target position for the motor to move to.
 *
 * This movement is relative to the current position of the motor as given in
 * motor_get_position(). Providing 10.0 as the position parameter would result
 * in the motor moving clockwise 10 units, no matter what the current position
 * is.
 *
 * \note This function simply sets the target for the motor, it does not block
 * program execution until the movement finishes.
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
 * failed, setting errno.
 */
int32_t motor_move_relative(uint8_t port, const double position, const int32_t velocity);

/**
 * Sets the velocity for the motor.
 *
 * This velocity corresponds to different actual speeds depending on the gearset
 * used for the motor. This results in a range of +-100 for E_MOTOR_GEARSET_36,
 * +-200 for E_MOTOR_GEARSET_18, and +-600 for E_MOTOR_GEARSET_6. The velocity
 * is held with PID to ensure consistent speed, as opposed to setting the
 * motor's voltage.
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
 *        motor's gearset
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
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
 * failed, setting errno.
 */
int32_t motor_move_voltage(uint8_t port, const int32_t voltage);

/**
 * Changes the output velocity for a profiled movement (motor_move_absolute or
 * motor_move_relative). This will have no effect if the motor is not following
 * a profiled movement.
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
 *        motor's gearset
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t motor_modify_profiled_velocity(uint8_t port, const int32_t velocity);

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
 * operation failed, setting errno.
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
 * if the operation failed, setting errno.
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
 * \return The motor's actual velocity in RPM or PROS_ERR_F if the operation
 * failed, setting errno.
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
 * setting errno.
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
 * negative direction, or PROS_ERR if the operation failed, setting errno.
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
 * failed, setting errno.
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
 * limit is not exceeded, or PROS_ERR if the operation failed, setting errno.
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
 * \return 1 if the temperature limit is exceeded and 0 if the the temperature
 * is below the limit, or PROS_ERR if the operation failed, setting errno.
 */
int32_t motor_is_over_temp(uint8_t port);

/**
 * Checks if the motor is stopped.
 *
 * \note Although this function forwards data from the motor, the motor
 * presently does not provide any value. This function returns PROS_ERR with
 * errno set to ENOSYS.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return 1 if the motor is not moving, 0 if the motor is moving, or PROS_ERR
 * if the operation failed, setting errno
 */
int32_t motor_is_stopped(uint32_t port);

/**
 * Checks if the motor is at its zero position.
 *
 * \note Although this function forwards data from the motor, the motor
 * presently does not provide any value. This function returns PROS_ERR with
 * errno set to ENOSYS.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return 1 if the motor is at zero absolute position, 0 if the motor has
 * moved from its absolute zero, or PROS_ERR if the operation failed,
 * setting errno
 */
int32_t motor_get_zero_position_flag(uint32_t port);

#ifdef __cplusplus
}  // namespace c
#endif

typedef enum motor_fault_e {
	E_MOTOR_FAULT_NO_FAULTS = 0x00,
	E_MOTOR_FAULT_MOTOR_OVER_TEMP = 0x01,  // Analogous to motor_is_over_temp()
	E_MOTOR_FAULT_DRIVER_FAULT = 0x02,     // Indicates a motor h-bridge fault
	E_MOTOR_FAULT_OVER_CURRENT = 0x04,     // Analogous to motor_is_over_current()
	E_MOTOR_FAULT_DRV_OVER_CURRENT = 0x08  // Indicates an h-bridge over current
} motor_fault_e_t;

#ifdef PROS_USE_SIMPLE_NAMES
#ifdef __cplusplus
#define MOTOR_FAULT_NO_FAULTS pros::E_MOTOR_FAULT_NO_FAULTS
#define MOTOR_FAULT_MOTOR_OVER_TEMP pros::E_MOTOR_FAULT_MOTOR_OVER_TEMP
#define MOTOR_FAULT_DRIVER_FAULT pros::E_MOTOR_FAULT_DRIVER_FAULT
#define MOTOR_FAULT_OVER_CURRENT pros::E_MOTOR_FAULT_DRV_OVER_CURRENT
#define MOTOR_FAULT_DRV_OVER_CURRENT pros::E_MOTOR_FAULT_DRV_OVER_CURRENT
#else
#define MOTOR_FAULT_NO_FAULTS E_MOTOR_FAULT_NO_FAULTS
#define MOTOR_FAULT_MOTOR_OVER_TEMP E_MOTOR_FAULT_MOTOR_OVER_TEMP
#define MOTOR_FAULT_DRIVER_FAULT E_MOTOR_FAULT_DRIVER_FAULT
#define MOTOR_FAULT_OVER_CURRENT E_MOTOR_FAULT_DRV_OVER_CURRENT
#define MOTOR_FAULT_DRV_OVER_CURRENT E_MOTOR_FAULT_DRV_OVER_CURRENT
#endif
#endif

#ifdef __cplusplus
namespace c {
#endif

/**
 * Gets the faults experienced by the motor.
 *
 * Compare this bitfield to the bitmasks in motor_fault_e_t.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return A bitfield containing the motor's faults.
 */
uint32_t motor_get_faults(uint8_t port);

#ifdef __cplusplus
}  // namespace c
#endif

typedef enum motor_flag_e {
	E_MOTOR_FLAGS_NONE = 0x00,
	E_MOTOR_FLAGS_BUSY = 0x01,           // Cannot currently communicate to the motor
	E_MOTOR_FLAGS_ZERO_VELOCITY = 0x02,  // Analogous to motor_is_stopped()
	E_MOTOR_FLAGS_ZERO_POSITION = 0x04   // Analogous to motor_get_zero_position_flag()
} motor_flag_e_t;

#ifdef PROS_USE_SIMPLE_NAMES
#ifdef __cplusplus
#define MOTOR_FLAGS_NONE pros::E_MOTOR_FLAGS_NONE
#define MOTOR_FLAGS_BUSY pros::E_MOTOR_FLAGS_BUSY
#define MOTOR_FLAGS_ZERO_VELOCITY pros::E_MOTOR_FLAGS_ZERO_VELOCITY
#define MOTOR_FLAGS_ZERO_POSITION pros::E_MOTOR_FLAGS_ZERO_POSITION
#else
#define MOTOR_FLAGS_NONE E_MOTOR_FLAGS_NONE
#define MOTOR_FLAGS_BUSY E_MOTOR_FLAGS_BUSY
#define MOTOR_FLAGS_ZERO_VELOCITY E_MOTOR_FLAGS_ZERO_VELOCITY
#define MOTOR_FLAGS_ZERO_POSITION E_MOTOR_FLAGS_ZERO_POSITION
#endif
#endif

#ifdef __cplusplus
namespace c {
#endif

/**
 * Gets the flags set by the motor's operation.
 *
 * Compare this bitfield to the bitmasks in motor_flag_e_t.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return A bitfield containing the motor's flags.
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
 * \param[in] timestamp
 *            A pointer to a time in milliseconds for which the encoder count
 *            will be returned. If NULL, the timestamp at which the encoder
 *            count was read will not be supplied
 *
 * \return The raw encoder count at the given timestamp or PROS_ERR if the
 * operation failed.
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
 * if the operation failed, setting errno.
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
 * failed, setting errno.
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
 * operation failed, setting errno.
 */
double motor_get_temperature(uint8_t port);

/**
 * Gets the torque generated by the motor in Newton Meters (Nm).
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The motor's torque in Nm or PROS_ERR_F if the operation failed,
 * setting errno.
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
 * \return The motor's voltage in mV or PROS_ERR_F if the operation failed,
 * setting errno.
 */
int32_t motor_get_voltage(uint8_t port);

/******************************************************************************/
/**                      Motor configuration functions                       **/
/**                                                                          **/
/**  These functions allow programmers to configure the behavior of motors   **/
/******************************************************************************/

#ifdef __cplusplus
}  // namespace c
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
	E_MOTOR_ENCODER_DEGREES = 0,    // Position is recorded as angle in degrees
	                                // as a floating point number
	E_MOTOR_ENCODER_ROTATIONS = 1,  // Position is recorded as angle in rotations
	                                // as a floating point number
	E_MOTOR_ENCODER_COUNTS = 2,     // Position is recorded as raw encoder ticks
	                                // as a whole number
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

#ifdef PROS_USE_SIMPLE_NAMES
#ifdef __cplusplus
#define MOTOR_BRAKE_COAST pros::E_MOTOR_BRAKE_COAST
#define MOTOR_BRAKE_BRAKE pros::E_MOTOR_BRAKE_BRAKE
#define MOTOR_BRAKE_HOLD pros::E_MOTOR_BRAKE_HOLD
#define MOTOR_BRAKE_INVALID pros::E_MOTOR_BRAKE_INVALID
#define MOTOR_ENCODER_DEGREES pros::E_MOTOR_ENCODER_DEGREES
#define MOTOR_ENCODER_ROTATIONS pros::E_MOTOR_ENCODER_ROTATIONS
#define MOTOR_ENCODER_COUNTS pros::E_MOTOR_ENCODER_COUNTS
#define MOTOR_ENCODER_INVALID pros::E_MOTOR_ENCODER_INVALID
#define MOTOR_GEARSET_36 pros::E_MOTOR_GEARSET_36
#define MOTOR_GEARSET_18 pros::E_MOTOR_GEARSET_18
#define MOTOR_GEARSET_06 pros::E_MOTOR_GEARSET_06
#define MOTOR_GEARSET_6 pros::E_MOTOR_GEARSET_06
#define MOTOR_GEARSET_INVALID pros::E_MOTOR_GEARSET_INVALID
#else
#define MOTOR_BRAKE_COAST E_MOTOR_BRAKE_COAST
#define MOTOR_BRAKE_BRAKE E_MOTOR_BRAKE_BRAKE
#define MOTOR_BRAKE_HOLD E_MOTOR_BRAKE_HOLD
#define MOTOR_BRAKE_INVALID E_MOTOR_BRAKE_INVALID
#define MOTOR_ENCODER_DEGREES E_MOTOR_ENCODER_DEGREES
#define MOTOR_ENCODER_ROTATIONS E_MOTOR_ENCODER_ROTATIONS
#define MOTOR_ENCODER_COUNTS E_MOTOR_ENCODER_COUNTS
#define MOTOR_ENCODER_INVALID E_MOTOR_ENCODER_INVALID
#define MOTOR_GEARSET_36 E_MOTOR_GEARSET_36
#define MOTOR_GEARSET_18 E_MOTOR_GEARSET_18
#define MOTOR_GEARSET_06 E_MOTOR_GEARSET_06
#define MOTOR_GEARSET_6 E_MOTOR_GEARSET_06
#define MOTOR_GEARSET_INVALID E_MOTOR_GEARSET_INVALID
#endif
#endif

/**
 * Holds the information about a Motor's position or velocity PID controls.
 *
 * These values are in 4.4 format, meaning that a value of 0x20 represents 2.0,
 * 0x21 represents 2.0625, 0x22 represents 2.125, etc.
 */
typedef struct motor_pid_full_s {
	uint8_t kf;         // The feedforward constant
	uint8_t kp;         // The proportional constant
	uint8_t ki;         // The integral constants
	uint8_t kd;         // The derivative constant
	uint8_t filter;     // A constant used for filtering the profile acceleration
	uint16_t limit;     // The integral limit
	uint8_t threshold;  // The threshold for determining if a position movement has
	                    // reached its goal. This has no effect for velocity PID
	                    // calculations.
	uint8_t loopspeed;  // The rate at which the PID computation is run in ms
} motor_pid_full_s_t;

/**
 * Holds just the constants for a Motor's position or velocity PID controls.
 *
 * These values are in 4.4 format, meaning that a value of 0x20 represents 2.0,
 * 0x21 represents 2.0625, 0x22 represents 2.125, etc.
 */
typedef struct motor_pid_s {
	uint8_t kf;  // The feedforward constant
	uint8_t kp;  // The proportional constant
	uint8_t ki;  // The integral constants
	uint8_t kd;  // The derivative constant
} motor_pid_s_t;

#ifdef __cplusplus
namespace c {
#endif

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
 * failed, setting errno.
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
 * failed, setting errno.
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
 * failed, setting errno.
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
 * failed, setting errno.
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
 * failed, setting errno.
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
 * failed, setting errno.
 */
int32_t motor_set_gearing(uint8_t port, const motor_gearset_e_t gearset);

/**
 * Takes in floating point values and returns a properly formatted pid struct.
 * The motor_pid_s_t struct is in 4.4 format, i.e. 0x20 is 2.0, 0x21 is 2.0625,
 * etc.
 * This function will convert the floating point values to the nearest 4.4
 * value.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param kf
 *        The feedforward constant
 * \param kp
 *        The proportional constant
 * \param ki
 *        The integral constant
 * \param kd
 *        The derivative constant
 *
 * \return A motor_pid_s_t struct formatted properly in 4.4.
 */
motor_pid_s_t motor_convert_pid(double kf, double kp, double ki, double kd);

/**
 * Takes in floating point values and returns a properly formatted pid struct.
 * The motor_pid_s_t struct is in 4.4 format, i.e. 0x20 is 2.0, 0x21 is 2.0625,
 * etc.
 * This function will convert the floating point values to the nearest 4.4
 * value.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param kf
 *        The feedforward constant
 * \param kp
 *        The proportional constant
 * \param ki
 *        The integral constant
 * \param kd
 *        The derivative constant
 * \param filter
 *        A constant used for filtering the profile acceleration
 * \param limit
 *        The integral limit
 * \param threshold
 *        The threshold for determining if a position movement has reached its
 *        goal. This has no effect for velocity PID calculations.
 * \param loopspeed
 *        The rate at which the PID computation is run in ms
 *
 * \return A motor_pid_s_t struct formatted properly in 4.4.
 */
motor_pid_full_s_t motor_convert_pid_full(double kf, double kp, double ki, double kd, double filter, double limit,
                                          double threshold, double loopspeed);

/**
 * Sets one of motor_pid_s_t for the motor. This intended to just modify the
 * main PID constants.
 *
 * Only non-zero values of the struct will change the existing motor constants.
 *
 * \note This feature is in beta, it is advised to use caution when modifying
 * the PID values. The motor could be damaged by particularly large constants.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param pid
 *        The new motor PID constants
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t motor_set_pos_pid(uint8_t port, const motor_pid_s_t pid);

/**
 * Sets one of motor_pid_full_s_t for the motor.
 *
 * Only non-zero values of the struct will change the existing motor constants.
 *
 * \note This feature is in beta, it is advised to use caution when modifying
 * the PID values. The motor could be damaged by particularly large constants.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param pid
 *        The new motor PID constants
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t motor_set_pos_pid_full(uint8_t port, const motor_pid_full_s_t pid);

/**
 * Sets one of motor_pid_s_t for the motor. This intended to just modify the
 * main PID constants.
 *
 * Only non-zero values of the struct will change the existing motor constants.
 *
 * \note This feature is in beta, it is advised to use caution when modifying
 * the PID values. The motor could be damaged by particularly large constants.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param pid
 *        The new motor PID constants
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t motor_set_vel_pid(uint8_t port, const motor_pid_s_t pid);

/**
 * Sets one of motor_pid_full_s_t for the motor.
 *
 * Only non-zero values of the struct will change the existing motor constants.
 *
 * \note This feature is in beta, it is advised to use caution when modifying
 * the PID values. The motor could be damaged by particularly large constants.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param pid
 *        The new motor PID constants
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t motor_set_vel_pid_full(uint8_t port, const motor_pid_full_s_t pid);

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
 * failed, setting errno.
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
 * failed, setting errno.
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
 * or E_MOTOR_BRAKE_INVALID if the operation failed, setting errno.
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
 * setting errno.
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
 * or E_MOTOR_ENCODER_INVALID if the operation failed.
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
 * or E_GEARSET_INVALID if the operation failed.
 */
motor_gearset_e_t motor_get_gearing(uint8_t port);

/**
 * Gets the position PID that was set for the motor. This function will return
 * zero for all of the parameters if the motor_set_pos_pid() or
 * motor_set_pos_pid_full() functions have not been used.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * Additionally, in an error state all values of the returned struct are set
 * to their negative maximum values.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return A motor_pid_full_s_t containing the position PID constants last set
 * to the given motor
 */
motor_pid_full_s_t motor_get_pos_pid(uint8_t port);

/**
 * Gets the velocity PID that was set for the motor. This function will return
 * zero for all of the parameters if the motor_set_vel_pid() or
 * motor_set_vel_pid_full() functions have not been used.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * Additionally, in an error state all values of the returned struct are set
 * to their negative maximum values.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return A motor_pid_full_s_t containing the velocity PID constants last set
 * to the given motor
 */
motor_pid_full_s_t motor_get_vel_pid(uint8_t port);

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
 * or PROS_ERR if the operation failed, setting errno.
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
 * setting errno.
 */
int32_t motor_get_voltage_limit(uint8_t port);

#ifdef __cplusplus
}  // namespace c
}  // namespace pros
}
#endif

#endif  // _PROS_MOTORS_H_
