/**
 * \file pros/motors.hpp
 *
 * Contains prototypes for the V5 Motor-related functions.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/motors.html to learn
 * more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2021, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_MOTORS_HPP_
#define _PROS_MOTORS_HPP_

#include <cstdint>
#include <initializer_list>
#include <vector>

#include "pros/motors.h"
#include "pros/rtos.hpp"

namespace pros {
class Motor {
	public:
	/**
	 * Creates a Motor object for the given port and specifications.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param port
	 *        The V5 port number from 1-21
	 * \param gearset
	 *        The motor's gearset
	 * \param reverse
	 *        True reverses the motor, false is default
	 * \param encoder_units
	 *        The motor's encoder units
	 */
	explicit Motor(const std::int8_t port, const motor_gearset_e_t gearset, const bool reverse,
	               const motor_encoder_units_e_t encoder_units);

	explicit Motor(const std::int8_t port, const motor_gearset_e_t gearset, const bool reverse);

	explicit Motor(const std::int8_t port, const motor_gearset_e_t gearset);

	explicit Motor(const std::int8_t port, const bool reverse);

	explicit Motor(const std::int8_t port);

	/****************************************************************************/
	/**                         Motor movement functions                       **/
	/**                                                                        **/
	/**          These functions allow programmers to make motors move         **/
	/****************************************************************************/
	/**
	 * Sets the voltage for the motor from -128 to 127.
	 *
	 * This is designed to map easily to the input from the controller's analog
	 * stick for simple opcontrol use. The actual behavior of the motor is
	 * analogous to use of pros::Motor::move(), or motorSet from the PROS 2 API.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param voltage
	 *        The new motor voltage from -127 to 127
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t operator=(std::int32_t voltage) const;

	/**
	 * Sets the voltage for the motor from -127 to 127.
	 *
	 * This is designed to map easily to the input from the controller's analog
	 * stick for simple opcontrol use. The actual behavior of the motor is
	 * analogous to use of motor_move(), or motorSet() from the PROS 2 API.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param voltage
	 *        The new motor voltage from -127 to 127
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t move(std::int32_t voltage) const;

	/**
	 * Sets the target absolute position for the motor to move to.
	 *
	 * This movement is relative to the position of the motor when initialized or
	 * the position when it was most recently reset with
	 * pros::Motor::set_zero_position().
	 *
	 * \note This function simply sets the target for the motor, it does not block
	 * program execution until the movement finishes.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param position
	 *        The absolute position to move to in the motor's encoder units
	 * \param velocity
	 *        The maximum allowable velocity for the movement in RPM
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t move_absolute(const double position, const std::int32_t velocity) const;

	/**
	 * Sets the relative target position for the motor to move to.
	 *
	 * This movement is relative to the current position of the motor as given in
	 * pros::Motor::motor_get_position(). Providing 10.0 as the position parameter
	 * would result in the motor moving clockwise 10 units, no matter what the
	 * current position is.
	 *
	 * \note This function simply sets the target for the motor, it does not block
	 * program execution until the movement finishes.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param position
	 *        The relative position to move to in the motor's encoder units
	 * \param velocity
	 *        The maximum allowable velocity for the movement in RPM
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t move_relative(const double position, const std::int32_t velocity) const;

	/**
	 * Sets the velocity for the motor.
	 *
	 * This velocity corresponds to different actual speeds depending on the
	 * gearset used for the motor. This results in a range of +-100 for
	 * E_MOTOR_GEARSET_36, +-200 for E_MOTOR_GEARSET_18, and +-600 for
	 * E_MOTOR_GEARSET_6. The velocity is held with PID to ensure consistent
	 * speed, as opposed to setting the motor's voltage.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param velocity
	 *        The new motor velocity from -+-100, +-200, or +-600 depending on the
	 *        motor's gearset
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t move_velocity(const std::int32_t velocity) const;

	/**
	 * Sets the output voltage for the motor from -12000 to 12000 in millivolts.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param voltage
	 *        The new voltage value from -12000 to 12000
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t move_voltage(const std::int32_t voltage) const;

	/**
	 * Stops the motor using the currently configured brake mode.
	 *
	 * This function sets motor velocity to zero, which will cause it to act
	 * according to the set brake mode. If brake mode is set to MOTOR_BRAKE_HOLD,
	 * this function may behave differently than calling move_absolute(0)
	 * or move_relative(0).
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t brake(void) const;

	/**
	 * Changes the output velocity for a profiled movement (motor_move_absolute()
	 * or motor_move_relative()). This will have no effect if the motor is not
	 * following a profiled movement.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param velocity
	 *        The new motor velocity from +-100, +-200, or +-600 depending on the
	 *        motor's gearset
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t modify_profiled_velocity(const std::int32_t velocity) const;

	/**
	 * Gets the target position set for the motor by the user.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return The target position in its encoder units or PROS_ERR_F if the
	 * operation failed, setting errno.
	 */
	virtual double get_target_position(void) const;

	/**
	 * Gets the velocity commanded to the motor by the user.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return The commanded motor velocity from +-100, +-200, or +-600, or
	 * PROS_ERR if the operation failed, setting errno.
	 */
	virtual std::int32_t get_target_velocity(void) const;

	/****************************************************************************/
	/**                        Motor telemetry functions                       **/
	/**                                                                        **/
	/**    These functions allow programmers to collect telemetry from motors  **/
	/****************************************************************************/

	/**
	 * Gets the actual velocity of the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return The motor's actual velocity in RPM or PROS_ERR_F if the operation
	 * failed, setting errno.
	 */
	virtual double get_actual_velocity(void) const;

	/**
	 * Gets the current drawn by the motor in mA.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return The motor's current in mA or PROS_ERR if the operation failed,
	 * setting errno.
	 */
	virtual std::int32_t get_current_draw(void) const;

	/**
	 * Gets the direction of movement for the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return 1 for moving in the positive direction, -1 for moving in the
	 * negative direction, and PROS_ERR if the operation failed, setting errno.
	 */
	virtual std::int32_t get_direction(void) const;

	/**
	 * Gets the efficiency of the motor in percent.
	 *
	 * An efficiency of 100% means that the motor is moving electrically while
	 * drawing no electrical power, and an efficiency of 0% means that the motor
	 * is drawing power but not moving.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return The motor's efficiency in percent or PROS_ERR_F if the operation
	 * failed, setting errno.
	 */
	virtual double get_efficiency(void) const;

	/**
	 * Checks if the motor is drawing over its current limit.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return 1 if the motor's current limit is being exceeded and 0 if the
	 * current limit is not exceeded, or PROS_ERR if the operation failed, setting
	 * errno.
	 */
	virtual std::int32_t is_over_current(void) const;

	/**
	 * Checks if the motor is stopped.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \note Although this function forwards data from the motor, the motor
	 * presently does not provide any value. This function returns PROS_ERR with
	 * errno set to ENOSYS.
	 *
	 * \return 1 if the motor is not moving, 0 if the motor is moving, or PROS_ERR
	 * if the operation failed, setting errno
	 */
	virtual std::int32_t is_stopped(void) const;

	/**
	 * Checks if the motor is at its zero position.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \note Although this function forwards data from the motor, the motor
	 * presently does not provide any value. This function returns PROS_ERR with
	 * errno set to ENOSYS.
	 *
	 * \return 1 if the motor is at zero absolute position, 0 if the motor has
	 * moved from its absolute zero, or PROS_ERR if the operation failed, setting
	 * errno
	 */
	virtual std::int32_t get_zero_position_flag(void) const;

	/**
	 * Gets the faults experienced by the motor.
	 *
	 * Compare this bitfield to the bitmasks in pros::motor_fault_e_t.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return A bitfield containing the motor's faults.
	 */
	virtual std::uint32_t get_faults(void) const;

	/**
	 * Gets the flags set by the motor's operation.
	 *
	 * Compare this bitfield to the bitmasks in pros::motor_flag_e_t.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return A bitfield containing the motor's flags.
	 */
	virtual std::uint32_t get_flags(void) const;

	/**
	 * Gets the raw encoder count of the motor at a given timestamp.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param[in] timestamp
	 *            A pointer to a time in milliseconds for which the encoder count
	 *            will be returned. If NULL, the timestamp at which the encoder
	 *            count was read will not be supplied
	 *
	 * \return The raw encoder count at the given timestamp or PROS_ERR if the
	 * operation failed.
	 */
	virtual std::int32_t get_raw_position(std::uint32_t* const timestamp) const;

	/**
	 * Gets the temperature limit flag for the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return 1 if the temperature limit is exceeded and 0 if the temperature is
	 * below the limit, or PROS_ERR if the operation failed, setting errno.
	 */
	virtual std::int32_t is_over_temp(void) const;

	/**
	 * Gets the absolute position of the motor in its encoder units.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return The motor's absolute position in its encoder units or PROS_ERR_F
	 * if the operation failed, setting errno.
	 */
	virtual double get_position(void) const;

	/**
	 * Gets the power drawn by the motor in Watts.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return The motor's power draw in Watts or PROS_ERR_F if the operation
	 * failed, setting errno.
	 */
	virtual double get_power(void) const;

	/**
	 * Gets the temperature of the motor in degrees Celsius.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return The motor's temperature in degrees Celsius or PROS_ERR_F if the
	 * operation failed, setting errno.
	 */
	virtual double get_temperature(void) const;

	/**
	 * Gets the torque generated by the motor in Newton Meters (Nm).
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return The motor's torque in Nm or PROS_ERR_F if the operation failed,
	 * setting errno.
	 */
	virtual double get_torque(void) const;

	/**
	 * Gets the voltage delivered to the motor in millivolts.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return The motor's voltage in mV or PROS_ERR_F if the operation failed,
	 * setting errno.
	 * 
	 */
	virtual std::int32_t get_voltage(void) const;

	/****************************************************************************/
	/**                      Motor configuration functions                     **/
	/**                                                                        **/
	/**  These functions allow programmers to configure the behavior of motors **/
	/****************************************************************************/

	/**
	 * Sets the position for the motor in its encoder units.
	 *
	 * This will be the future reference point for the motor's "absolute"
	 * position.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param position
	 *        The new reference position in its encoder units
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t set_zero_position(const double position) const;

	/**
	 * Sets the "absolute" zero position of the motor to its current position.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t tare_position(void) const;

	/**
	 * Sets one of motor_brake_mode_e_t to the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param mode
	 *        The motor_brake_mode_e_t to set for the motor
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t set_brake_mode(const motor_brake_mode_e_t mode) const;

	/**
	 * Sets the current limit for the motor in mA.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param limit
	 *        The new current limit in mA
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t set_current_limit(const std::int32_t limit) const;

	/**
	 * Sets one of motor_encoder_units_e_t for the motor encoder.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param units
	 *        The new motor encoder units
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t set_encoder_units(const motor_encoder_units_e_t units) const;

	/**
	 * Sets one of motor_gearset_e_t for the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param gearset
	 *        The new motor gearset
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t set_gearing(const motor_gearset_e_t gearset) const;

	/**
	 * Takes in floating point values and returns a properly formatted pid struct.
	 * The motor_pid_s_t struct is in 4.4 format, i.e. 0x20 is 2.0, 0x21 is
	 * 2.0625, etc.
	 * This function will convert the floating point values to the nearest 4.4
	 * value.
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
	[[deprecated(
	    "Changing these values is not supported by VEX and may lead to permanent motor damage.")]] static motor_pid_s_t
	convert_pid(double kf, double kp, double ki, double kd);

	/**
	 * Takes in floating point values and returns a properly formatted pid struct.
	 * The motor_pid_s_t struct is in 4.4 format, i.e. 0x20 is 2.0, 0x21 is
	 * 2.0625, etc.
	 * This function will convert the floating point values to the nearest 4.4
	 * value.
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
	[[deprecated(
	    "Changing these values is not supported by VEX and may lead to permanent motor "
	    "damage.")]] static motor_pid_full_s_t
	convert_pid_full(double kf, double kp, double ki, double kd, double filter, double limit, double threshold,
	                 double loopspeed);

	/**
	 * Sets one of motor_pid_s_t for the motor. This intended to just modify the
	 * main PID constants.
	 *
	 * Only non-zero values of the struct will change the existing motor
	 * constants.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param pid
	 *        The new motor PID constants
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	[[deprecated(
	    "Changing these values is not supported by VEX and may lead to permanent motor damage.")]] virtual std::int32_t
	set_pos_pid(const motor_pid_s_t pid) const;

	/**
	 * Sets one of motor_pid_full_s_t for the motor.
	 *
	 * Only non-zero values of the struct will change the existing motor
	 * constants.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param pid
	 *        The new motor PID constants
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	[[deprecated(
	    "Changing these values is not supported by VEX and may lead to permanent motor damage.")]] virtual std::int32_t
	set_pos_pid_full(const motor_pid_full_s_t pid) const;

	/**
	 * Sets one of motor_pid_s_t for the motor. This intended to just modify the
	 * main PID constants.
	 *
	 * Only non-zero values of the struct will change the existing motor
	 * constants.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param pid
	 *        The new motor PID constants
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	[[deprecated(
	    "Changing these values is not supported by VEX and may lead to permanent motor damage.")]] virtual std::int32_t
	set_vel_pid(const motor_pid_s_t pid) const;

	/**
	 * Sets one of motor_pid_full_s_t for the motor.
	 *
	 * Only non-zero values of the struct will change the existing motor
	 * constants.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param pid
	 *        The new motor PID constants
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	[[deprecated(
	    "Changing these values is not supported by VEX and may lead to permanent motor damage.")]] virtual std::int32_t
	set_vel_pid_full(const motor_pid_full_s_t pid) const;

	/**
	 * Sets the reverse flag for the motor.
	 *
	 * This will invert its movements and the values returned for its position.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param reverse
	 *        True reverses the motor, false is default
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t set_reversed(const bool reverse) const;

	/**
	 * Sets the voltage limit for the motor in Volts.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param limit
	 *        The new voltage limit in Volts
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t set_voltage_limit(const std::int32_t limit) const;

	/**
	 * Gets the brake mode that was set for the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return One of motor_brake_mode_e_t, according to what was set for the
	 * motor, or E_MOTOR_BRAKE_INVALID if the operation failed, setting errno.
	 */
	virtual motor_brake_mode_e_t get_brake_mode(void) const;

	/**
	 * Gets the current limit for the motor in mA.
	 *
	 * The default value is 2500 mA.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return The motor's current limit in mA or PROS_ERR if the operation failed,
	 * setting errno.
	 */
	virtual std::int32_t get_current_limit(void) const;

	/**
	 * Gets the encoder units that were set for the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return One of motor_encoder_units_e_t according to what is set for the
	 * motor or E_MOTOR_ENCODER_INVALID if the operation failed.
	 */
	virtual motor_encoder_units_e_t get_encoder_units(void) const;

	/**
	 * Gets the gearset that was set for the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return One of motor_gearset_e_t according to what is set for the motor,
	 * or E_GEARSET_INVALID if the operation failed.
	 */
	virtual motor_gearset_e_t get_gearing(void) const;

	/**
	 * Gets the position PID that was set for the motor. This function will return
	 * zero for all of the parameters if the motor_set_pos_pid() or
	 * motor_set_pos_pid_full() functions have not been used.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * Additionally, in an error state all values of the returned struct are set
	 * to their negative maximum values.
	 *
	 * \return A motor_pid_full_s_t containing the position PID constants last set
	 * to the given motor
	 */
	[[deprecated(
	    "Changing these values is not supported by VEX and may lead to permanent motor "
	    "damage.")]] virtual motor_pid_full_s_t
	get_pos_pid(void) const;

	/**
	 * Gets the velocity PID that was set for the motor. This function will return
	 * zero for all of the parameters if the motor_set_vel_pid() or
	 * motor_set_vel_pid_full() functions have not been used.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * Additionally, in an error state all values of the returned struct are set
	 * to their negative maximum values.
	 *
	 * \return A motor_pid_full_s_t containing the velocity PID constants last set
	 * to the given motor
	 */
	[[deprecated(
	    "Changing these values is not supported by VEX and may lead to permanent motor "
	    "damage.")]] virtual motor_pid_full_s_t
	get_vel_pid(void) const;

	/**
	 * Gets the operation direction of the motor as set by the user.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return 1 if the motor has been reversed and 0 if the motor was not
	 * reversed, or PROS_ERR if the operation failed, setting errno.
	 */
	virtual std::int32_t is_reversed(void) const;

	/**
	 * Gets the voltage limit set by the user.
	 *
	 * Default value is 0V, which means that there is no software limitation
	 * imposed on the voltage.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return The motor's voltage limit in V or PROS_ERR if the operation failed,
	 * setting errno.
	 */
	virtual std::int32_t get_voltage_limit(void) const;

	/**
	 * Gets the port number of the motor.
	 *
	 * \return The motor's port number.
	 */
	virtual std::uint8_t get_port(void) const;

	private:
	const std::uint8_t _port;
};

class Motor_Group {
	public:
	Motor_Group(const std::initializer_list<Motor> motors);
	explicit Motor_Group(const std::vector<pros::Motor>& motors);
	explicit Motor_Group(const std::initializer_list<std::int8_t> motor_ports);
	explicit Motor_Group(const std::vector<std::int8_t> motor_ports); // Pass by value to preserve ABI

	/****************************************************************************/
	/**                      Motor Group movement functions                    **/
	/**                                                                        **/
	/**       These functions allow programmers to make motor groups move      **/
	/****************************************************************************/
	/**
	 * Sets the voltage for all the motors in the motor group from -128 to 127.
	 *
	 * This is designed to map easily to the input from the controller's analog
	 * stick for simple opcontrol use. The actual behavior of the motor is
	 * analogous to use of pros::Motor::move() on each motor individually
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - One of the ports cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \param voltage
	 *        The new motor voltage from -127 to 127
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t operator=(std::int32_t);

	/**
	 * Sets the voltage for the motors in the motor group from -127 to 127.
	 *
	 * This is designed to map easily to the input from the controller's analog
	 * stick for simple opcontrol use. The actual behavior of the motor is
	 * analogous to use of motor_move(), or motorSet() from the
	 * PROS 2 API on each motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \param voltage
	 *        The new motor voltage from -127 to 127
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t move(std::int32_t voltage);

	/**
	 * Sets the target absolute position for the motors to move to.
	 *
	 * This movement is relative to the position of the motors when initialized or
	 * the position when it was most recently reset with
	 * pros::Motor::set_zero_position().
	 *
	 * \note This function simply sets the target for the motors, it does not block
	 * program execution until the movement finishes.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \param position
	 *        The absolute position to move to in the motors' encoder units
	 * \param velocity
	 *        The maximum allowable velocity for the movement in RPM
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t move_absolute(const double position, const std::int32_t velocity);

	/**
	 * Sets the relative target position for the motor to move to.
	 *
	 * This movement is relative to the current position of the motor as given in
	 * pros::Motor::motor_get_position(). Providing 10.0 as the position parameter
	 * would result in the motor moving clockwise 10 units, no matter what the
	 * current position is.
	 *
	 * \note This function simply sets the target for the motor, it does not block
	 * program execution until the movement finishes.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \param position
	 *        The relative position to move to in the motor's encoder units
	 * \param velocity
	 *        The maximum allowable velocity for the movement in RPM
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t move_relative(const double position, const std::int32_t velocity);

	/**
	 * Sets the velocity for the motors.
	 *
	 * This velocity corresponds to different actual speeds depending on the
	 * gearset used for the motor. This results in a range of +-100 for
	 * E_MOTOR_GEARSET_36, +-200 for E_MOTOR_GEARSET_18, and +-600 for
	 * E_MOTOR_GEARSET_6. The velocity is held with PID to ensure consistent
	 * speed, as opposed to setting the motor's voltage.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \param velocity
	 *        The new motor velocity from -+-100, +-200, or +-600 depending on the
	 *        motor's gearset
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t move_velocity(const std::int32_t velocity);

	/**
	 * Sets the output voltage for the motors from -12000 to 12000 in millivolts.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \param voltage
	 *        The new voltage value from -12000 to 12000
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t move_voltage(const std::int32_t voltage);

	/**
	 * Stops the motor using the currently configured brake mode.
	 *
	 * This function sets motor velocity to zero, which will cause it to act
	 * according to the set brake mode. If brake mode is set to MOTOR_BRAKE_HOLD,
	 * this function may behave differently than calling move_absolute(0)
	 * or move_relative(0).
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t brake(void);

	/* 
	 * Gets the voltages delivered to the motors in millivolts.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 * 
	 * \return The voltage of the motor in millivolts or PROS_ERR_F if the operation
	 * failed, setting errno.
	 * 
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor_Group motors({1, 2});
	 *   std::vector<std::uint32_t> voltages;
	 *   while (true) {
	 * 	   voltages = motors.get_voltages();
	 * 
	 *     for (uint32_t i = 0; i < voltages.size(); i++) {
	 * 	     printf("Voltages: %ld\n", voltages[i]);
	 *     }
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 * 
	 */
	std::vector<std::uint32_t> get_voltages(void);

	/* 
	 * Get the voltage limits of the motors set by the user.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 * 
	 * \return The voltage limit of the motor in millivolts or PROS_ERR_F if the operation
	 * failed, setting errno.
	 * 
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor_Group motors({1, 2});
	 *   std::vector<std::uint32_t> voltage_limits;
	 *   while (true) {
	 * 	   voltage_limits = motors.get_voltage_limits();
	 * 
	 *     for (uint32_t i = 0; i < voltage_limits.size(); i++) {
	 * 	     printf("Voltage Limits: %ld\n", voltage_limits[i]);
	 *     }
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<std::uint32_t> get_voltage_limits(void);

	/* 
	 * Gets the raw encoder positions of a motor group at a given timestamp.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 * 	
	 * \return A vector of the raw encoder positions of the motors in the motor group
	 * based on the timestamps passed in. If a timestamp is not found for a motor, the
	 * value at that index will be PROS_ERR.
	 * 
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor_Group motors({1, 2});
	 *   std::vector<std::uint32_t*> timestamps;
	 *   std::vector<std::int32_t> positions;
	 * 	 std::uint32_t temp = 0;
	 *   std::uint32_t temp2 = 0;
	 *   timestamps.push_back(&temp);
	 *   timestamps.push_back(&temp2);
	 * 
	 *   while (true) {
	 *     positions = motors.get_raw_positions(timestamps);
	 * 
	 *     printf("Position: %ld, Time: %ln\n", positions[0], timestamps[0]);
     *	   printf("Position: %ld, Time: %ln\n", positions[1], timestamps[1]);
	 *
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<std::int32_t> get_raw_positions(std::vector<std::uint32_t*> &timestamps);
	/****************************************************************************/
	/**                      Motor configuration functions                     **/
	/**                                                                        **/
	/** These functions let programmers configure the behavior of motor groups **/
	/****************************************************************************/

	/**
	 * Indexes Motor in the Motor_Group in the same way as an array.
	 * 
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - Out of bounds on indexing the motor groups.
	 * 
	 * \param i
	 *        The index value in the motor group.
	 *
	 * \return the appropriate Motor reference or the erno if the operation
	 *  failed
	 */ 
	pros::Motor& operator[](int i);


	/**
	 * Indexes Motor in the Motor_Group.
	 * 
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * Throws an std::out_of_range error when indexing out of range
	 * 
	 * \param i
	 *        The index value in the motor group.
	 *
	 * \return the appropriate Motor reference.
	 */ 	
	pros::Motor& at(int i);

	/**
	 * Indexes Motor in the Motor_Group in the same way as an array.
	 * 
	 * \return the size of the vector containing motors
	 */ 
	std::int32_t size();

	/**
	 * Sets the position for the motor in its encoder units.
	 *
	 * This will be the future reference point for the motors' "absolute"
	 * position.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \param position
	 *        The new reference position in its encoder units
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t set_zero_position(const double position);
	/**
	 * Sets one of motor_brake_mode_e_t to the motor group.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \param mode
	 *        The motor_brake_mode_e_t to set for the motor
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t set_brake_modes(motor_brake_mode_e_t mode);

	/**
	 * Sets the reverse flag for all the motors in the motor group.
	 *
	 * This will invert its movements and the values returned for its position.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \param reverse
	 *        True reverses the motor, false is default
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t set_reversed(const bool reversed);

	/**
	 * Sets the voltage limit for all the motors in Volts.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \param limit
	 *        The new voltage limit in Volts
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t set_voltage_limit(const std::int32_t limit);
	/**
	 * Sets one of motor_gearset_e_t for all the motors in the motor group.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \param gearset
	 *        The new motor gearset
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t set_gearing(const motor_gearset_e_t gearset);

	/**
	 * Sets one of motor_encoder_units_e_t for the all the motor encoders
	 * in the motor group.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \param units
	 *        The new motor encoder units
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t set_encoder_units(const motor_encoder_units_e_t units);

	/**
	 * Sets the "absolute" zero position of the motor group to its current position.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t tare_position(void);

	/****************************************************************************/
	/**                        Motor telemetry functions                       **/
	/**                                                                        **/
	/** These functions let programmers to collect telemetry from motor groups **/
	/****************************************************************************/
	/**
	 * Gets the actual velocity of each motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \return A vector with the each motor's actual velocity in RPM in the order
	 * or a vector filled with PROS_ERR_F if the operation failed, setting errno.
	 */
	std::vector<double> get_actual_velocities(void);

	/**
	 * Gets the velocity commanded to the motor by the user.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return A vector filled with The commanded motor velocities from
	 *  +-100, +-200, or +-600, or a vector filled with PROS_ERR if the operation
	 *  failed, setting errno.
	 */
	std::vector<std::int32_t> get_target_velocities(void);

	/**
	 * Gets the target position set for the motor by the user.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \return A vector filled with the target position in its encoder units
	 * or a vector filled with PROS_ERR_F if the operation failed, setting errno.
	 */
	std::vector<double> get_target_positions(void);

	/**
	 * Gets the absolute position of the motor in its encoder units.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return The motor's absolute position in its encoder units or PROS_ERR_F
	 * if the operation failed, setting errno.
	 */
	std::vector<double> get_positions(void);
	/**
	 * Gets the efficiency of the motors in percent.
	 *
	 * An efficiency of 100% means that the motor is moving electrically while
	 * drawing no electrical power, and an efficiency of 0% means that the motor
	 * is drawing power but not moving.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \return A vector filled with the motor's efficiency in percent
	 * or a vector filled with PROS_ERR_F if the operation failed, setting errno.
	 */
	std::vector<double> get_efficiencies(void);

	/**
	 * Checks if the motors are drawing over its current limit.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \return 1 if the motor's current limit is being exceeded and 0 if the
	 * current limit is not exceeded, or PROS_ERR if the operation failed, setting
	 * errno.
	 */
	std::vector<std::int32_t> are_over_current(void);

	/**
	 * Gets the temperature limit flag for the motors.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return A vector with for each motor a 1 if the temperature limit is
	 * exceeded and 0 if the temperature is below the limit,
	 * or a vector filled with PROS_ERR if the operation failed, setting errno.
	 */
	std::vector<std::int32_t> are_over_temp(void);

	/**
	 * Gets the brake mode that was set for the motors.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \return A Vector with for each motor one of motor_brake_mode_e_t,
	 * according to what was set for the motor, or a vector filled with
	 * E_MOTOR_BRAKE_INVALID if the operation failed, setting errno.
	 */
	std::vector<pros::motor_brake_mode_e_t> get_brake_modes(void);

	/**
	 * Gets the gearset that was set for the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \return One of motor_gearset_e_t according to what is set for the motor,
	 * or E_GEARSET_INVALID if the operation failed.
	 */
	std::vector<motor_gearset_e_t> get_gearing(void);

	/**
	 * Gets the current drawn by each motor in mA.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \return A vector containing each motor's current in mA
	 * or a vector filled with PROS_ERR if the operation failed, setting errno.
	 */
	std::vector<std::int32_t> get_current_draws(void);

	/**
	 * Gets the current limit for each motor in mA.
	 *
	 * The default value is 2500 mA.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \return A vector with each motor's current limit in mA or a vector filled
	 * with PROS_ERR if the operation failed, setting errno.
	 */
	std::vector<std::int32_t> get_current_limits(void);

	/**
	 * Gets the port number of each motor.
	 *
	 * \return a vector with each motor's port number.
	 */
	std::vector<std::uint8_t> get_ports(void);
	/**
	 * Gets the direction of movement for the motors.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return 1 for moving in the positive direction, -1 for moving in the
	 * negative direction, and PROS_ERR if the operation failed, setting errno.
	 */
	std::vector<std::int32_t> get_directions(void);

	/**
	 * Gets the encoder units that were set for each motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \return A vector filled with one of motor_encoder_units_e_t for each motor
	 * according to what is set for the motor or a vector filled with
	 * E_MOTOR_ENCODER_INVALID if the operation failed.
	 */
	std::vector<pros::motor_encoder_units_e_t> get_encoder_units(void);
	
	/**
	 * Gets the encoder units that were set for each motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EACCESS - The Motor group mutex can't be taken or given
	 *
	 * \return The vector filled with motors' temperature in degrees Celsius or PROS_ERR_F if the
 	 * operation failed, setting errno.
	 */
	virtual std::vector<double> get_temperatures(void);

	private:
	std::vector<Motor> _motors;
	pros::Mutex _motor_group_mutex;
	std::uint8_t _motor_count;
};

using MotorGroup = Motor_Group; //alias

namespace literals {
const pros::Motor operator"" _mtr(const unsigned long long int m);
const pros::Motor operator"" _rmtr(const unsigned long long int m);
}  // namespace literals
}  // namespace pros
#endif  // _PROS_MOTORS_HPP_
