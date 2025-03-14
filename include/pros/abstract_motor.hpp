/**
 * \file abstract_motor.hpp
 * \ingroup cpp-abstract-motor
 *
 * Contains prototypes for AbstractMotor, the abstract base class of both
 * motors and motor groups. Abstract motors cannot be directly constructed, but
 * you can use motors and motor groups as abstract motors.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright Copyright (c) 2017-2024, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_ABSTRACT_MOTORS_HPP_
#define _PROS_ABSTRACT_MOTORS_HPP_

#include <cstdint>
#include <iostream>

#include "pros/device.hpp"
#include "pros/motors.h"
#include "rtos.hpp"

namespace pros {
inline namespace v5 {

/**
 * \enum MotorBrake
 * Indicates the current 'brake mode' of a motor.
 */
enum class MotorBrake {
	coast = 0, ///< Motor coasts when stopped, traditional behavior
	brake = 1, ///< Motor brakes when stopped
	hold = 2, ///< Motor actively holds position when stopped
	invalid = INT32_MAX ///< Invalid brake mode
};

/**
 * \enum MotorEncoderUnits
 * Indicates the units used by the motor encoders.
 */
enum class MotorEncoderUnits {
	degrees = 0, ///< Position is recorded as angle in degrees as a floating point number
	deg = 0, ///< Position is recorded as angle in degrees as a floating point number
	rotations = 1, ///< Position is recorded as angle in rotations as a floating point number
	counts = 2, ///< Position is recorded as raw encoder ticks as a whole number
	invalid = INT32_MAX ///< Invalid motor encoder units
};

// Alias for MotorEncoderUnits
using MotorUnits = MotorEncoderUnits;

enum class MotorGears {
	ratio_36_to_1 = 0, ///< 36:1, 100 RPM, Red gear set
	red = ratio_36_to_1, ///< 36:1, 100 RPM, Red gear set
	rpm_100 = ratio_36_to_1, ///< 36:1, 100 RPM, Red gear set
	ratio_18_to_1 = 1, ///< 18:1, 200 RPM, Green gear set
	green = ratio_18_to_1, ///< 18:1, 200 RPM, Green gear set
	rpm_200 = ratio_18_to_1, ///< 18:1, 200 RPM, Green gear set
	ratio_6_to_1 = 2, ///< 6:1, 600 RPM, Blue gear set
	blue = ratio_6_to_1, ///< 6:1, 600 RPM, Blue gear set
	rpm_600 = ratio_6_to_1, ///< 6:1, 600 RPM, Blue gear set
	invalid = INT32_MAX ///< Error return code
};

/**
 * \enum MotorType
 * Indicates the type of a motor
 */
enum class MotorType {
	v5 = 0, ///< 11w motor
	exp = 1, ///< 5.5w motor
	invalid = INT32_MAX ///< Error return code
};


// Provide Aliases for MotorGears
using MotorGearset = MotorGears;
using MotorCart = MotorGears;
using MotorCartridge = MotorGears;
using MotorGear = MotorGears;

/**
 * \ingroup cpp-abstract-motor
 */
class AbstractMotor {
	/**
	 * \addtogroup cpp-abstract-motor
	 *  @{
	 */
	public:
	/// \name Motor movement functions
	/// These functions allow programmers to make motors move
	///@{

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
	virtual std::int32_t move(std::int32_t voltage) const = 0;

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
	virtual std::int32_t move_absolute(const double position, const std::int32_t velocity) const = 0;

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
	virtual std::int32_t move_relative(const double position, const std::int32_t velocity) const = 0;

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
	virtual std::int32_t move_velocity(const std::int32_t velocity) const = 0;

	/**
	 * Sets the output voltage for the motor from -12000 to 12000 in millivolts.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param port
	 *        The V5 port number from 1-21
	 * \param voltage
	 *        The new voltage value from -12000 to 12000
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t move_voltage(const std::int32_t voltage) const = 0;

	/**
	 * Stops the motor using the currently configured brake mode.
	 * 
	 * This function sets motor velocity to zero, which will cause it to act
	 * according to the set brake mode. If brake mode is set to MOTOR_BRAKE_HOLD,
	 * this function may behave differently than calling move_absolute(0)
	 * or motor_move_relative(0).
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 */
	virtual std::int32_t brake(void) const = 0;

	/**
	 * Changes the output velocity for a profiled movement (motor_move_absolute or
	 * motor_move_relative). This will have no effect if the motor is not following
	 * a profiled movement.
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
	virtual std::int32_t modify_profiled_velocity(const std::int32_t velocity) const = 0;

	/**
	 * Gets the target position set for the motor by the user, with a parameter
	 * for the motor index.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return The target position in its encoder units or PROS_ERR_F if the
	 * operation failed, setting errno.
	 */
	virtual double get_target_position(const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector containing the target position(s) set for the motor(s) by the user
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 *
	 * \return A vector containing the target position(s) in its encoder units or PROS_ERR_F if the
	 * operation failed, setting errno.
	 */
	virtual std::vector<double> get_target_position_all(void) const = 0;

	/**
	 * Gets the velocity commanded to the motor by the user.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
	 * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return The commanded motor velocity from +-100, +-200, or +-600, or
	 * PROS_ERR if the operation failed, setting errno.
	 */
	virtual std::int32_t get_target_velocity(const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector containing the velocity/velocities commanded to the motor(s) by the user
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return A vector containing the commanded motor velocity/velocities from +-100, 
	 * +-200, or +-600, or PROS_ERR if the operation failed, setting errno.
	 */
	virtual std::vector<std::int32_t> get_target_velocity_all(void) const = 0;

	///@}

	/// \name Motor telemetry functions
	/// \addtogroup cpp-motor-telemetry
	/// These functions allow programmers to collect telemetry from motors
	///@{

	/**
	 * Gets the actual velocity of the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return The motor's actual velocity in RPM or PROS_ERR_F if the operation
	 * failed, setting errno.
	 */
	virtual double get_actual_velocity(const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector containing the actual velocity/velocities of the motor(s)
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return A vector containing the motor's/motors' actual velocity/velocities in RPM or PROS_ERR_F
	 * if the operation failed, setting errno.
	 */	
	virtual std::vector<double> get_actual_velocity_all(void) const = 0;

	/**
	 * Gets the current drawn by the motor in mA.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return The motor's current in mA or PROS_ERR if the operation failed,
	 * setting errno.
	 */
	virtual std::int32_t get_current_draw(const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector containing the current(s) drawn by the motor(s) in mA.
	 * 
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * 
	 * ENODEV - The port cannot be configured as a motor
	 * 
	 * 
	 * \return A vector conatining the motor's/motors' current(s) in mA or PROS_ERR if the operation failed,
	 * setting errno.
	 */
	virtual std::vector<std::int32_t> get_current_draw_all(void) const = 0;

	/**
	 * Gets the direction of movement for the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return 1 for moving in the positive direction, -1 for moving in the
	 * negative direction, and PROS_ERR if the operation failed, setting errno.
	 */
	virtual std::int32_t get_direction(const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector containing the direction(s) of movement for the motor(s).
	 *
	 * 
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * 
	 * ENODEV - The port cannot be configured as a motor
	 * 
	 * 
	 * \return A vector containing 1 for moving in the positive direction, -1 for moving in the
	 * negative direction, and PROS_ERR if the operation failed, setting errno.
	 */
	virtual std::vector<std::int32_t> get_direction_all(void) const = 0;

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
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return The motor's efficiency in percent or PROS_ERR_F if the operation
	 * failed, setting errno.
	 */
	virtual double get_efficiency(const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector containing the efficiency/efficiencies of the motor(s) in percent.
	 *
	 * An efficiency of 100% means that the motor is moving electrically while
	 * drawing no electrical power, and an efficiency of 0% means that the motor
	 * is drawing power but not moving.
	 * 
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * 
	 * ENODEV - The port cannot be configured as a motor
	 * 
	 *
	 * \return A vector containing the motor's/motors' efficiency/efficiencies in percent or PROS_ERR_F if the operation
	 * failed, setting errno.
	 */
	virtual std::vector<double> get_efficiency_all(void) const = 0;

	/**
	 * Gets the faults experienced by the motor.
	 *
	 * Compare this bitfield to the bitmasks in pros::motor_fault_e_t.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return A bitfield containing the motor's faults.
	 */
	virtual std::uint32_t get_faults(const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector of the faults experienced by the motor(s).
	 *
	 * Compare this bitfield to the bitmasks in pros::motor_fault_e_t.
	 * 
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * 
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return A bitfield containing the motor's/motors' faults.
	 */
	virtual std::vector<std::uint32_t> get_faults_all(void) const = 0;
	/**
	 * Gets the flags set by the motor's operation.
	 *
	 * Compare this bitfield to the bitmasks in pros::motor_flag_e_t.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return A bitfield containing the motor's flags.
	 */
	virtual std::uint32_t get_flags(const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector of the flags set by the motor's/motors' operation.
	 *
	 * Compare this bitfield to the bitmasks in pros::motor_flag_e_t.
	 * 
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * 
	 * ENODEV - The port cannot be configured as a motor
	 *
	 *
	 * \return A bitfield containing the motor's/motors' flags.
	 */
	virtual std::vector<std::uint32_t> get_flags_all(void) const = 0;

	/**
	 * Gets the absolute position of the motor in its encoder units.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return The motor's absolute position in its encoder units or PROS_ERR_F
	 * if the operation failed, setting errno.
	 */
	virtual double get_position(const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector containing the absolute position(s) of the motor(s) in its encoder units.
	 * 
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * 
	 * ENODEV - The port cannot be configured as a motor
	 
	 *
	 * \return A vector containing the motor's/motors' absolute position(s) in its encoder units or PROS_ERR_F
	 * if the operation failed, setting errno.
	 */
	virtual std::vector<double> get_position_all(void) const = 0;

	/**
	 * Gets the power drawn by the motor in Watts.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return The motor's power draw in Watts or PROS_ERR_F if the operation
	 * failed, setting errno.
	 */
	virtual double get_power(const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector containing the power(s) drawn by the motor(s) in Watts.
	 * 
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * 
	 * ENODEV - The port cannot be configured as a motor
	 * 
	 * \return A vector containing the motor's/motors' power draw in Watts or PROS_ERR_F if the operation
	 * failed, setting errno.
	 */
	virtual std::vector<double> get_power_all(void) const = 0;

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
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return The raw encoder count at the given timestamp or PROS_ERR if the
	 * operation failed.
	 */
	virtual std::int32_t get_raw_position(std::uint32_t* const timestamp, const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector of the raw encoder count(s) of the motor(s) at a given timestamp.
	 *
	 * 
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * 
	 * ENODEV - The port cannot be configured as a motor
	 * 
	 * \param timestamp
	 *            A pointer to a time in milliseconds for which the encoder count
	 *            will be returned. If NULL, the timestamp at which the encoder
	 *            count was read will not be supplied
	 * 
	 * \return A vector containing the raw encoder count(s) at the given timestamp or PROS_ERR if the
	 * operation failed.
	 */
	virtual std::vector<std::int32_t> get_raw_position_all(std::uint32_t* const timestamp) const = 0;

	/**
	 * Gets the temperature of the motor in degrees Celsius.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return The motor's temperature in degrees Celsius or PROS_ERR_F if the
	 * operation failed, setting errno.
	 */
	virtual double get_temperature(const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector of the temperature(s) of the motor(s) in degrees Celsius.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return A vector containing the motor's/motors' temperature(s) in degrees Celsius or PROS_ERR_F if the
	 * operation failed, setting errno.
	 */
	virtual std::vector<double> get_temperature_all(void) const = 0;
	/**
	 * Gets the torque generated by the motor in Newton Meters (Nm).
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return The motor's torque in Nm or PROS_ERR_F if the operation failed,
	 * setting errno.
	 */
	virtual double get_torque(const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector of the torque(s) generated by the motor(s) in Newton Meters (Nm).
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return A vector containing the motor's/motors' torque(s) in Nm or PROS_ERR_F if the operation failed,
	 * setting errno.
	 */
	virtual std::vector<double> get_torque_all(void) const = 0;
	/**
	 * Gets the voltage delivered to the motor in millivolts.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return The motor's voltage in mV or PROS_ERR_F if the operation failed,
	 * setting errno.
	 */
	virtual std::int32_t get_voltage(const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector of the voltage(s) delivered to the motor(s) in millivolts.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return A vector containing the motor's/motors' voltage(s) in mV or PROS_ERR_F if the operation failed,
	 * setting errno.
	 */
	virtual std::vector<std::int32_t> get_voltage_all(void) const = 0;

	/**
	 * Checks if the motor is drawing over its current limit.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return 1 if the motor's current limit is being exceeded and 0 if the
	 * current limit is not exceeded, or PROS_ERR if the operation failed, setting
	 * errno.
	 */
	virtual std::int32_t is_over_current(const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector of whether each motor is drawing over its current limit.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return 1 if the motor's current limit is being exceeded and 0 if the
	 * current limit is not exceeded, or PROS_ERR if the operation failed, setting
	 * errno.
	 */
	virtual std::vector<std::int32_t> is_over_current_all(void) const = 0;

	/**
	 * Gets the temperature limit flag for the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return 1 if the temperature limit is exceeded and 0 if the temperature is
	 * below the limit, or PROS_ERR if the operation failed, setting errno.
	 */
	virtual std::int32_t is_over_temp(const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector of the temperature limit flag(s) for the motor(s).
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return 1 if the temperature limit is exceeded and 0 if the temperature is
	 * below the limit, or PROS_ERR if the operation failed, setting errno.
	 */
	virtual std::vector<std::int32_t> is_over_temp_all(void) const = 0;

	///@}

	/// \name Motor configuration functions
	/// \addtogroup cpp-motor-configuration
	/// These functions allow programmers to configure the behavior of motors
	///@{

	/**
	 * Gets the brake mode that was set for the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return One of MotorBrake, according to what was set for the
	 * motor, or E_MOTOR_BRAKE_INVALID if the operation failed, setting errno.
	 */
	virtual MotorBrake get_brake_mode(const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector of the brake mode(s) that was set for the motor(s).
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return A vector containing MotorBrake(s), according to what was set for the
	 * motor(s), or E_MOTOR_BRAKE_INVALID if the operation failed, setting errno.
	 */
	virtual std::vector<MotorBrake> get_brake_mode_all(void) const = 0;

	/**
	 * Gets the current limit for the motor in mA.
	 *
	 * The default value is 2500 mA.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return The motor's current limit in mA or PROS_ERR if the operation failed,
	 * setting errno.
	 */
	virtual std::int32_t get_current_limit(const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector of the current limit(s) for the motor(s) in mA.
	 *
	 * The default value is 2500 mA.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return A vector containing the motor's/motors' current limit(s) in mA or PROS_ERR if the operation failed,
	 * setting errno.
	 */
	virtual std::vector<std::int32_t> get_current_limit_all(void) const = 0;

	/**
	 * Gets the encoder units that were set for the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return One of MotorUnits according to what is set for the
	 * motor or E_MOTOR_ENCODER_INVALID if the operation failed.
	 */
	virtual MotorUnits get_encoder_units(const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector of the encoder units that were set for the motor(s).
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return A vector of MotorUnits according to what is set for the
	 * motor(s) or E_MOTOR_ENCODER_INVALID if the operation failed.
	 */
	virtual std::vector<MotorUnits> get_encoder_units_all(void) const = 0;

	/**
	 * Gets the gearset that was set for the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return One of MotorGears according to what is set for the motor,
	 * or pros::MotorGears::invalid if the operation failed.
	 */
	virtual MotorGears get_gearing(const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector of the gearset(s) that was/were set for the motor(s).
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return A vector of MotorGears according to what is set for the motor(s),
	 * or pros::MotorGears::invalid if the operation failed.
	 */
	virtual std::vector<MotorGears> get_gearing_all(void) const = 0;

	/**
	 * @brief Gets returns a vector with all the port numbers in the motor group.
	 *
	 * @return std::vector<std::int8_t>
	 */
	virtual std::vector<std::int8_t> get_port_all(void) const = 0;

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
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return The motor's voltage limit in V or PROS_ERR if the operation failed,
	 * setting errno.
	 */
	virtual std::int32_t get_voltage_limit(const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector of the voltage limit(s) set by the user.
	 *
	 * Default value is 0V, which means that there is no software limitation
	 * imposed on the voltage.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return A vector containing the motor's/motors' voltage limit(s) in V or PROS_ERR if the operation failed,
	 * setting errno.
	 */
	virtual std::vector<std::int32_t> get_voltage_limit_all(void) const = 0;

	/**
	 * Gets the operation direction of the motor as set by the user.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return 1 if the motor has been reversed and 0 if the motor was not
	 * reversed, or PROS_ERR if the operation failed, setting errno.
	 */
	virtual std::int32_t is_reversed(const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector of the operation direction(s) of the motor(s) as set by the user.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return 1 if the motor has been reversed and 0 if the motor was not
	 * reversed, or PROS_ERR if the operation failed, setting errno.
	 */
	virtual std::vector<std::int32_t> is_reversed_all(void) const = 0;

	/**
	 * Gets the type of the motor
	 * 
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return One of MotorType according to the type of the motor,
	 * or pros::MotorType::invalid if the operation failed
	 */
	virtual MotorType get_type(const std::uint8_t index = 0) const = 0;

	/**
	 * Gets a vector of the type(s) of the motor(s).
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
	 * \return A vector of MotorType according to the type(s) of the motor(s),
	 * or pros::MotorType::invalid if the operation failed.
	 */
	virtual std::vector<MotorType> get_type_all(void) const = 0;

	/**
	 * Sets one of MotorBrake to the motor. Works with the C enum
	 * and the C++ enum class.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param mode
	 *        The MotorBrake to set for the motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t set_brake_mode(const MotorBrake mode, const std::uint8_t index = 0) const = 0;
	virtual std::int32_t set_brake_mode(const pros::motor_brake_mode_e_t mode, const std::uint8_t index = 0) const = 0;
	virtual std::int32_t set_brake_mode_all(const MotorBrake mode) const = 0;
	virtual std::int32_t set_brake_mode_all(const pros::motor_brake_mode_e_t mode) const = 0;
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
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t set_current_limit(const std::int32_t limit, const std::uint8_t index = 0) const = 0;
	virtual std::int32_t set_current_limit_all(const std::int32_t limit) const = 0;
	/**
	 * Sets one of MotorUnits for the motor encoder. Works with the C
	 * enum and the C++ enum class.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param units
	 *        The new motor encoder units
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t set_encoder_units(const MotorUnits units, const std::uint8_t index = 0) const = 0;
	virtual std::int32_t set_encoder_units(const pros::motor_encoder_units_e_t units, const std::uint8_t index = 0) const = 0;
	virtual std::int32_t set_encoder_units_all(const MotorUnits units) const = 0;
	virtual std::int32_t set_encoder_units_all(const pros::motor_encoder_units_e_t units) const = 0;
	/**
	 * Sets one of the gear cartridge (red, green, blue) for the motor. Usable with
	 * the C++ enum class and the C enum.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param gearset
	 *        The new motor gearset
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t set_gearing(const MotorGears gearset, const std::uint8_t index = 0) const = 0;
	virtual std::int32_t set_gearing(const pros::motor_gearset_e_t gearset, const std::uint8_t index = 0) const = 0;
	virtual std::int32_t set_gearing_all(const MotorGears gearset) const = 0;
	virtual std::int32_t set_gearing_all(const pros::motor_gearset_e_t gearset) const = 0;

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
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t set_reversed(const bool reverse, const std::uint8_t index = 0) = 0;
	virtual std::int32_t set_reversed_all(const bool reverse) = 0;

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
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t set_voltage_limit(const std::int32_t limit, const std::uint8_t index = 0) const = 0;
	virtual std::int32_t set_voltage_limit_all(const std::int32_t limit) const = 0;

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
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t set_zero_position(const double position, const std::uint8_t index = 0) const = 0;
	virtual std::int32_t set_zero_position_all(const double position) const = 0;

	/**
	 * Sets the "absolute" zero position of the motor to its current position.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
     * \param index Optional parameter. 
     *           The index of the motor to get the target position of.
     *           By default index is 0, and will return an error for an out of bounds index
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t tare_position(const std::uint8_t index = 0) const = 0;
	virtual std::int32_t tare_position_all(void) const = 0;
	virtual std::int8_t get_port(const std::uint8_t index = 0) const = 0;
	/**
	 * @brief Returns the number of objects
	 *
	 * @return std::int8_t
	 */
	virtual std::int8_t size(void) const = 0;

	///@}
	private:
};

} // namespace v5
} // namespace pros

///@}

#endif
