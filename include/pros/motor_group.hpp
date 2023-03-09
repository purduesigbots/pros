/**
 * \file pros/motors.hpp
 * \ingroup cpp-motors
 *
 * Contains prototypes for the V5 Motor-related functions.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/motors.html to learn
 * more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \defgroup cpp-motors Motors C++ API
 * \note Additional example code for this module can be found in its [Tutorial](@ref motors).
 */

#ifndef _PROS_MOTOR_GROUP_HPP_
#define _PROS_MOTOR_GROUP_HPP_

#include <cstdint>
#include <iostream>

#include "pros/abstract_motor.hpp"
#include "pros/colors.hpp"
#include "pros/device.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "rtos.hpp"

namespace pros {
inline namespace v5 {
class MotorGroup : public virtual AbstractMotor {
	/**
	 * \addtogroup cpp-motors
	 *  @{
	 */
	public:
	explicit MotorGroup(const std::initializer_list<std::int8_t>, const pros::v5::Motor_Gears gearset = pros::v5::Motor_Gears::green,
	                    const pros::v5::Motor_Units encoder_units = pros::v5::Motor_Units::degrees);
	explicit MotorGroup(const std::vector<std::int8_t>& ports, const pros::v5::Motor_Gears gearset = pros::v5::Motor_Gears::green,
	                    const pros::v5::Motor_Units encoder_units = pros::v5::Motor_Units::degrees);
	std::int32_t operator=(const std::int32_t);

	/// \name Motor movement functions
	/// These functions allow programmers to make motors move
	///@{

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
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup MotorGroup ({1}, E_MOTOR_GEARSET_18);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t operator=(std::int32_t voltage) const;

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
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup MotorGroup ({1});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor.move(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y));
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t move(std::int32_t voltage) const;

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
	 *
	 * \b Example
	 * \code
	 * void autonomous() {
	 *   pros::Motor motor (1);
	 *   motor.move_absolute(100, 100); // Moves 100 units forward
	 *   while (!((motor.get_position() < 105) && (motor.get_position() > 95))) {
	 *     // Continue running this loop as long as the motor is not within +-5 units of its goal
	 *     pros::delay(2);
	 *   }
	 *   motor.move_absolute(100, 100); // This does not cause a movement
	 *   while (!((motor.get_position() < 105) && (motor.get_position() > 95))) {
	 *     pros::delay(2);
	 *   }
	 *   motor.tare_position();
	 *   motor.move_absolute(100, 100); // Moves 100 units forward
	 *   while (!((motor.get_position() < 105) && (motor.get_position() > 95))) {
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t move_absolute(const double position, const std::int32_t velocity) const;

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
	 *
	 * \b Example
	 * \code
	 * void autonomous() {
	 *   pros::Motor motor (1);
	 *   motor.move_relative(100, 100); // Moves 100 units forward
	 *   while (!((motor.get_position() < 105) && (motor.get_position() > 95))) {
	 *     // Continue running this loop as long as the motor is not within +-5 units of its goal
	 *     pros::delay(2);
	 *   }
	 *   motor.move_relative(100, 100); // Also moves 100 units forward
	 *   while (!((motor.get_position() < 205) && (motor.get_position() > 195))) {
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t move_relative(const double position, const std::int32_t velocity) const;

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
	 *
	 * \b Example
	 * \code
	 * void autonomous() {
	 *   pros::Motor motor (1);
	 *   motor.move_velocity(100);
	 *   pros::delay(1000); // Move at 100 RPM for 1 second
	 *   motor.move_velocity(0);
	 * }
	 * \endcode
	 */
	std::int32_t move_velocity(const std::int32_t velocity) const;

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
	 *
	 * \b Example
	 * \code
	 * void autonomous() {
	 *   motor.move_voltage(12000);
	 *   pros::delay(1000); // Move at max voltage for 1 second
	 *   motor.move_voltage(0);
	 * }
	 * \endcode
	 */
	std::int32_t move_voltage(const std::int32_t voltage) const;

	std::int32_t brake(void) const;

	std::int32_t modify_profiled_velocity(const std::int32_t velocity) const;
	/**
	 * Gets the target position set for the motor by the user, with a parameter
	 * for the motor index.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param index The index of the motor to get the target position of.
	 *
	 * \return The target position in its encoder units or PROS_ERR_F if the
	 * operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void autonomous() {
	 *   pros::Motor motor (1);
	 *   motor.move_absolute(100, 100);
	 *   std::cout << "Motor Target: " << motor.get_target_position();
	 *   // Prints 100
	 * }
	 * \endcode
	 */
	double get_target_position(const std::uint8_t index) const;
	std::vector<double> get_target_position_all(void) const;

	/**
	 * Gets the velocity commanded to the motor by the user.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return The commanded motor velocity from +-100, +-200, or +-600, or
	 * PROS_ERR if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y));
	 *     std::cout << "Motor Velocity: " << motor.get_target_velocity();
	 *     // Prints the value of E_CONTROLLER_ANALOG_LEFT_Y
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t get_target_velocity(const std::uint8_t index = 0) const;
	std::vector<std::int32_t> get_target_velocity_all(void) const;

	///@}

	/// \name Motor telemetry functions
	/// These functions allow programmers to collect telemetry from motors
	///@{

	/**
	 * Gets the actual velocity of the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return The motor's actual velocity in RPM or PROS_ERR_F if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   while (true) {
	 *     motor = controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_LEFT_Y);
	 *     printf("Actual velocity: %lf\n", motor.get_actual_velocity());
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	double get_actual_velocity(const std::uint8_t index = 0) const;
	std::vector<double> get_actual_velocity_all(void) const;

	/**
	 * Gets the current drawn by the motor in mA.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return The motor's current in mA or PROS_ERR if the operation failed,
	 * setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Current Draw: " << motor.get_current_draw();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t get_current_draw(const std::uint8_t index = 0) const;
	std::vector<std::int32_t> get_current_draw_all(void) const;

	/**
	 * Gets the direction of movement for the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return 1 for moving in the positive direction, -1 for moving in the
	 * negative direction, and PROS_ERR if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Direction: " << motor.get_direction();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t get_direction(const std::uint8_t index = 0) const;
	std::vector<std::int32_t> get_direction_all(void) const;

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
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Efficiency: " << motor.get_efficiency();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	double get_efficiency(const std::uint8_t index = 0) const;
	std::vector<double> get_efficiency_all(void) const;

	/**
	 * Gets the faults experienced by the motor.
	 *
	 * Compare this bitfield to the bitmasks in pros::motor_fault_e_t.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param port
	 *        The V5 port number from 1-21
	 *
	 * \return A bitfield containing the motor's faults.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Faults: " << motor.get_faults();pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::uint32_t get_faults(const std::uint8_t index = 0) const;
	std::vector<std::uint32_t> get_faults_all(void) const;
	/**
	 * Gets the flags set by the motor's operation.
	 *
	 * Compare this bitfield to the bitmasks in pros::motor_flag_e_t.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param port
	 *        The V5 port number from 1-21
	 *
	 * \return A bitfield containing the motor's flags.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Faults: " << motor.get_faults();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::uint32_t get_flags(const std::uint8_t index = 0) const;
	std::vector<std::uint32_t> get_flags_all(void) const;

	/**
	 * Gets the absolute position of the motor in its encoder units.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return The motor's absolute position in its encoder units or PROS_ERR_F
	 * if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Position: " << motor.get_position();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	double get_position(const std::uint8_t index = 0) const;
	std::vector<double> get_position_all(void) const;

	/**
	 * Gets the power drawn by the motor in Watts.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return The motor's power draw in Watts or PROS_ERR_F if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Power: " << motor.get_power();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	double get_power(const std::uint8_t index = 0) const;
	std::vector<double> get_power_all(void) const;
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
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   std::uint32_t now = pros::millis();
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Position: " << motor.get_raw_position(&now);
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t get_raw_position(std::uint32_t* const timestamp, const std::uint8_t index = 0) const;
	std::vector<std::int32_t> get_raw_position_all(std::uint32_t* const timestamp) const;

	/**
	 * Gets the temperature of the motor in degrees Celsius.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return The motor's temperature in degrees Celsius or PROS_ERR_F if the
	 * operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Temperature: " << motor.get_temperature();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	double get_temperature(const std::uint8_t index = 0) const;
	std::vector<double> get_temperature_all(void) const;
	/**
	 * Gets the torque generated by the motor in Newton Meters (Nm).
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return The motor's torque in Nm or PROS_ERR_F if the operation failed,
	 * setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Torque: " << motor.get_torque();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	double get_torque(const std::uint8_t index = 0) const;
	std::vector<double> get_torque_all(void) const;
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
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Voltage: " << motor.get_voltage();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t get_voltage(const std::uint8_t index = 0) const;
	std::vector<std::int32_t> get_voltage_all(void) const;

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
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Is the motor over its current limit?: " << motor.is_over_current();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t is_over_current(const std::uint8_t index = 0) const;
	std::vector<std::int32_t> is_over_current_all(void) const;

	/**
	 * Gets the temperature limit flag for the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return 1 if the temperature limit is exceeded and 0 if the temperature is
	 * below the limit, or PROS_ERR if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Is the motor over its temperature limit?: " << motor.is_over_temp();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t is_over_temp(const std::uint8_t index = 0) const;
	std::vector<std::int32_t> is_over_temp_all(void) const;

	///@}

	/// \name Motor configuration functions
	/// These functions allow programmers to configure the behavior of motors
	///@{

	/**
	 * Gets the brake mode that was set for the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return One of Motor_Brake, according to what was set for the
	 * motor, or E_MOTOR_BRAKE_INVALID if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::Motor motor (1);
	 *   motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	 *   std::cout << "Brake Mode: " << motor.get_brake_mode();
	 * }
	 * \endcode
	 */
	Motor_Brake get_brake_mode(const std::uint8_t index = 0) const;
	std::vector<Motor_Brake> get_brake_mode_all(void) const;

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
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   while (true) {
	 *     std::cout << "Motor Current Limit: " << motor.get_current_limit();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t get_current_limit(const std::uint8_t index = 0) const;
	std::vector<std::int32_t> get_current_limit_all(void) const;

	/**
	 * Gets the encoder units that were set for the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return One of Motor_Units according to what is set for the
	 * motor or E_MOTOR_ENCODER_INVALID if the operation failed.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::Motor motor (1, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_COUNTS);
	 *   std::cout << "Motor Encoder Units: " << motor.get_encoder_units();
	 * }
	 * \endcode
	 */
	Motor_Units get_encoder_units(const std::uint8_t index = 0) const;
	std::vector<Motor_Units> get_encoder_units_all(void) const;

	/**
	 * Gets the gearset that was set for the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return One of Motor_Gears according to what is set for the motor,
	 * or pros::Motor_Gears::invalid if the operation failed.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::Motor motor (1, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_COUNTS);
	 *   std::cout << "Motor Gearing: " << motor.get_gearing();
	 * }
	 * \endcode
	 */
	Motor_Gears get_gearing(const std::uint8_t index = 0) const;
	std::vector<Motor_Gears> get_gearing_all(void) const;

	/**
	 * @brief Gets returns a vector with all the port numbers in the motor group.
	 * (ALL THE PORTS WILL BE POSITIVE)
	 * Use get_ports if you want to get the information on reversal.
	 *
	 * @return std::vector<std::uint8_t>
	 */
	std::vector<std::int8_t> get_port_all(void) const;

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
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::Motor motor (1);
	 *   std::cout << "Motor Voltage Limit: " << motor.get_voltage_limit();
	 * }
	 * \endcode
	 */
	std::int32_t get_voltage_limit(const std::uint8_t index = 0) const;
	std::vector<std::int32_t> get_voltage_limit_all(void) const;

	/**
	 * Gets the operation direction of the motor as set by the user.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return 1 if the motor has been reversed and 0 if the motor was not
	 * reversed, or PROS_ERR if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::Motor motor (1);
	 *   std::cout << "Is the motor reversed? " << motor.is_reversed();
	 *   // Prints "0"
	 * }
	 * \endcode
	 */
	std::int32_t is_reversed(const std::uint8_t index = 0) const;
	std::vector<std::int32_t> is_reversed_all(void) const;

	/**
	 * Sets one of Motor_Brake to the motor. Works with the C enum
	 * and the C++ enum class.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param mode
	 *        The Motor_Brake to set for the motor
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::Motor motor (1);
	 *   motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	 *   std::cout << "Brake Mode: " << motor.get_brake_mode();
	 * }
	 * \endcode
	 */
	std::int32_t set_brake_mode(const Motor_Brake mode, const std::uint8_t index = 0) const;
	std::int32_t set_brake_mode(const pros::motor_brake_mode_e_t mode, const std::uint8_t index = 0) const;
	std::int32_t set_brake_mode_all(const Motor_Brake mode) const;
	std::int32_t set_brake_mode_all(const pros::motor_brake_mode_e_t mode) const;
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
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *
	 * motor.set_current_limit(1000);
	 * while (true) {
	 *   motor = controller_get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *   // The motor will reduce its output at 1000 mA instead of the default 2500 mA
	 *   pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t set_current_limit(const std::int32_t limit, const std::uint8_t index = 0) const;
	std::int32_t set_current_limit_all(const std::int32_t limit) const;
	/**
	 * Sets one of Motor_Units for the motor encoder. Works with the C
	 * enum and the C++ enum class.
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
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::Motor motor (1);
	 *   motor.set_encoder_units(E_MOTOR_ENCODER_DEGREES);
	 *   std::cout << "Encoder Units: " << motor.get_encoder_units();
	 * }
	 * \endcode
	 */
	std::int32_t set_encoder_units(const Motor_Units units, const std::uint8_t index = 0) const;
	std::int32_t set_encoder_units(const pros::motor_encoder_units_e_t units, const std::uint8_t index = 0) const;
	std::int32_t set_encoder_units_all(const Motor_Units units) const;
	std::int32_t set_encoder_units_all(const pros::motor_encoder_units_e_t units) const;
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
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::Motor motor (1);
	 *   motor.set_gearing(E_MOTOR_GEARSET_06);
	 *   std::cout << "Gearset: " << motor.get_gearing();
	 * }
	 * \endcode
	 */
	std::int32_t set_gearing(const Motor_Gears gearset, const std::uint8_t index = 0) const;
	std::int32_t set_gearing(const pros::motor_gearset_e_t gearset, const std::uint8_t index = 0) const;
	std::int32_t set_gearing_all(const Motor_Gears gearset) const;
	std::int32_t set_gearing_all(const pros::motor_gearset_e_t gearset) const;

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
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::Motor motor (1);
	 *   motor.set_reversed(true);
	 *   std::cout << "Is this motor reversed? " << motor.is_reversed();
	 * }
	 * \endcode
	 */
	std::int32_t set_reversed(const bool reverse, const std::uint8_t index = 0);
	std::int32_t set_reversed_all(const bool reverse);

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
	 *
	 * \b Example
	 * \code
	 * void autonomous() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *
	 *   motor.set_voltage_limit(10000);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     // The motor will not output more than 10 V
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t set_voltage_limit(const std::int32_t limit, const std::uint8_t index = 0) const;
	std::int32_t set_voltage_limit_all(const std::int32_t limit) const;

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
	 *
	 * \b Example
	 * \code
	 * void autonomous() {
	 *   pros::Motor motor (1);
	 *   motor.move_absolute(100, 100); // Moves 100 units forward
	 *   motor.move_absolute(100, 100); // This does not cause a movement
	 *
	 *   motor.set_zero_position(80);
	 *   motor.move_absolute(100, 100); // Moves 80 units forward
	 * }
	 * \endcode
	 *
	 */
	std::int32_t set_zero_position(const double position, const std::uint8_t index = 0) const;
	std::int32_t set_zero_position_all(const double position) const;

	/**
	 * Sets the "absolute" zero position of the motor to its current position.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void autonomous() {
	 *   pros::Motor motor (1);
	 *   motor.move_absolute(100, 100); // Moves 100 units forward
	 *   motor.move_absolute(100, 100); // This does not cause a movement
	 *
	 *   motor.tare_position();
	 *   motor.move_absolute(100, 100); // Moves 100 units forward
	 * }
	 * \endcode
	 */
	std::int32_t tare_position(const std::uint8_t index = 0) const;
	std::int32_t tare_position_all(void) const;

	/**
	 * @brief Returns the number of objects
	 *
	 * @return std::int8_t
	 */
	std::int8_t size(void) const;
	std::int8_t get_port(const std::uint8_t index = 0) const;

	/**
	 * @brief Appends the other motor group reference to this motor group
	 *
	 */
	void operator+=(MotorGroup&);

	/**
	 * @brief Appends the other motor group reference to this motor group
	 *
	 */
	void append(MotorGroup&);

	/**
	 * @brief Removes the port (and it's reversed )
	 *
	 * @param port
	 */
	void erase_port(std::int8_t port);

	/**
	 * This is the overload for the << operator for printing to streams
	 *
	 * Prints in format:
	 * Motor [port: (motor port), brake mode: (brake mode), current draw: (current draw),
	 * current limit: (current limit), direction: (direction), efficiency: (efficiency),
	 * encoder units: (encoder units), gearing: (gearing), is over temp: (over temp),
	 * position: (position), reversed: (reversed boolean), temperature: (temperature),
	 * torque: (torque), voltage: (voltage)]
	 */
	friend std::ostream& operator<<(std::ostream& os, pros::MotorGroup& motor);
	///@}
	private:
	std::vector<std::int8_t> _ports;
	mutable pros::Mutex _MotorGroup_mutex;
};
}  // namespace v5
}  // namespace pros
#endif