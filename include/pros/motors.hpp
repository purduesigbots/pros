/**
 * \file pros/motors.hpp
 * \ingroup cpp-motors
 *
 * Contains prototypes for the V5 Motor-related functions.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2024, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \defgroup cpp-motors Motors C++ API
 * \note Additional example code for this module can be found in its [Tutorial](@ref motors).
 */

#ifndef _PROS_MOTORS_HPP_
#define _PROS_MOTORS_HPP_

#include <cstdint>
#include <iostream>

#include "pros/abstract_motor.hpp"
#include "pros/device.hpp"
#include "pros/motors.h"
#include "rtos.hpp"

namespace pros {
inline namespace v5 {

class Motor : public AbstractMotor, public Device {
	public:
	/**
	 * \addtogroup cpp-motors
	 *  @{
	 */

	/**
	 * Constructs a new Motor object.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports |1-21|.
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param port
	 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors.
	 * 		  A reversed motor will reverse the input or output movement functions and movement related
	 * 		  telemetry in order to produce consistant behavior with non-reversed motors
	 *
	 * \param gearset = pros::v5::MotorGears::green
	 * 		  Optional parameter for the gearset for the motor.
	 * 		  Does not explicitly set the gearset if not specified or if the gearset is invalid
	 *
	 * \param encoder_units = pros::v5::MotorUnits::degrees
	 * 		  Optional parameter for the encoder units of the motor
	 * 		  Does not explicitly set the gearset if not specified or if the gearset is invalid
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 * 	Motor first_motor(1); //Creates a motor on port 1 without altering gearset or encoder units
	 *  Motor reversed_motor(-2); //Creates a reversed motor on port 1 port 1 without altering gearset or encoder units
	 *  Motor blue_motor(3, pros::v5::MotorGears::blue); //Creates a motor on port 3 with blue gear set
	 *  Motor rotations_motor(4, pros::v5::MotorGears::green, pros::v5::MotorUnits::rotations); //port 4 w/ rotations
	 *
	 * }
	 * \endcode
	 *
	 */
	Motor(const std::int8_t port, const pros::v5::MotorGears gearset = pros::v5::MotorGears::invalid,
	      const pros::v5::MotorUnits encoder_units = pros::v5::MotorUnits::invalid);

	Motor(const Device& device) : Motor(device.get_port()){};

	/// \name Motor movement functions
	/// These functions allow programmers to make motors move
	///@{

	/**
	 * Sets the voltage for the motor from -127 to 127.
	 *
	 * This is designed to map easily to the input from the controller's analog
	 * stick for simple opcontrol use. The actual behavior of the motor is
	 * analogous to use of motor_move().
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
	 *   pros::Motor Motor (1);
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
	 * would result in the motor moving clockwise 10 units (counter clockwise if reversed),
	 * no matter what the current position is.
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
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 *  void autonomous() {
	 *  Motor motor(1);
	 *   motor.move_voltage(12000);
	 *   pros::delay(1000); // Move at max voltage for 1 second
	 *   motor.brake();
	 * }
	 * \endcode
	 */
	std::int32_t brake(void) const;

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
	 *
	 * \b Example
	 * \code
	 * void autonomous() {
	 * 	 pros::Motor motor (1);
	 *   motor.move_absolute(100, 100);
	 *   pros::delay(100);
	 *   motor.modify_profiled_velocity(0); // Stop the motor early
	 * }
	 * \endcode
	 */
	std::int32_t modify_profiled_velocity(const std::int32_t velocity) const;

	///@}

	/// \name Motor telemetry functions
	/// These functions allow programmers to collect telemetry from motors
	///@{

	/**
	 * Gets the target position set for the motor by the user
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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
	double get_target_position(const std::uint8_t index = 0) const;

	/**
	 * Gets the velocity commanded to the motor by the user at the index specified.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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

	/**
	 * Gets the actual velocity of the motor.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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

	/**
	 * Gets the current drawn by the motor in mA.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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

	/**
	 * Gets the direction of movement for the motor.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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

	/**
	 * Gets the efficiency of the motor in percent.
	 *
	 * An efficiency of 100% means that the motor is moving electrically while
	 * drawing no electrical power, and an efficiency of 0% means that the motor
	 * is drawing power but not moving.
	 *
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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

	/**
	 * Gets the faults experienced by the motor.
	 *
	 * Compare this bitfield to the bitmasks in pros::motor_fault_e_t.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
	 *
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

	/**
	 * Gets the flags set by the motor's operation.
	 *
	 * Compare this bitfield to the bitmasks in pros::motor_flag_e_t.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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

	/**
	 * Gets the absolute position of the motor in its encoder units.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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

	/**
	 * Gets the power drawn by the motor in Watts.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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

	/**
	 * Gets the raw encoder count of the motor at a given timestamp.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 *
	 * \param timestamp
	 *            A pointer to a time in milliseconds for which the encoder count
	 *            will be returned. If NULL, the timestamp at which the encoder
	 *            count was read will not be supplied
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
	 *
	 *
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

	/**
	 * Gets the temperature of the motor in degrees Celsius.

	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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

	/**
	 * Gets the torque generated by the motor in Newton Meters (Nm).
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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

	/**
	 * Gets the voltage delivered to the motor in millivolts.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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

	/**
	 * Checks if the motor is drawing over its current limit.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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

	/**
	 * Gets the temperature limit flag for the motor.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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

	///@}

	/// \name Motor configuration functions
	/// These functions allow programmers to configure the behavior of motors
	///@{

	/**
	 * Gets the brake mode that was set for the motor.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
	 *
	 * \return One of MotorBrake, according to what was set for the
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
	MotorBrake get_brake_mode(const std::uint8_t index = 0) const;

	/**
	 * Gets the current limit for the motor in mA.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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

	/**
	 * Gets the encoder units that were set for the motor.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
	 *
	 * \return One of MotorUnits according to what is set for the
	 * motor or E_MOTOR_ENCODER_INVALID if the operation failed.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::Motor motor (1, E_MOTOR_GEARSET_06, E_MOTOR_ENCODER_COUNTS);
	 *   std::cout << "Motor Encoder Units: " << motor.get_encoder_units();
	 * }
	 * \endcode
	 */
	MotorUnits get_encoder_units(const std::uint8_t index = 0) const;

	/**
	 * Gets the gearset that was set for the motor.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
	 *
	 * \return One of MotorGears according to what is set for the motor,
	 * or pros::MotorGears::invalid if the operation failed.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::Motor motor (1, E_MOTOR_GEARSET_06, E_MOTOR_ENCODER_COUNTS);
	 *   std::cout << "Motor Gearing: " << motor.get_gearing();
	 * }
	 * \endcode
	 */
	MotorGears get_gearing(const std::uint8_t index = 0) const;

	/**
	 * Gets the voltage limit set by the user.
	 *
	 * Default value is 0V, which means that there is no software limitation
	 * imposed on the voltage.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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

	/**
	 * Gets whether the motor is reversed or not
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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

	/**
	 * Gets the type of the motor
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the type of.
	 * 		  By default index is 0, and will return an error for a non-zero index
	 *
	 * \return One of MotorType according to the type of the motor,
	 * or pros::MotorType::invalid if the operation failed.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::Motor motor (1, E_MOTOR_GEARSET_06, E_MOTOR_ENCODER_COUNTS);
	 *   std::cout << "Motor Type: " << motor.get_type();
	 * }
	 * \endcode
	 */
	MotorType get_type(const std::uint8_t index = 0) const;

	/**
	 * Sets one of Motor_Brake to the motor.
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 *
	 * \param mode
	 *        The MotorBrake to set for the motor
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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
	std::int32_t set_brake_mode(const MotorBrake mode, const std::uint8_t index = 0) const;
	/**
	 * Sets one of MotorBrake to the motor.
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 *
	 * \param mode
	 *        The MotorBrake to set for the motor
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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
	std::int32_t set_brake_mode(const pros::motor_brake_mode_e_t mode, const std::uint8_t index = 0) const;
	/**
	 * Sets the current limit for the motor in mA.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param limit
	 *        The new current limit in mA
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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
	std::int32_t set_encoder_units(const MotorUnits units, const std::uint8_t index = 0) const;
	/**
	 * Sets one of MotorUnits for the motor encoder. Works with the C
	 * enum and the C++ enum class.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
	 *
	 * * \param units
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
	std::int32_t set_encoder_units(const pros::motor_encoder_units_e_t units, const std::uint8_t index = 0) const;
	/**
	 * Sets one of the gear cartridge (red, green, blue) for the motor. Usable with
	 * the C++ enum class and the C enum.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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
	std::int32_t set_gearing(const MotorGears gearset, const std::uint8_t index = 0) const;

	/**
	 * Sets one of the gear cartridge (red, green, blue) for the motor. Usable with
	 * the C++ enum class and the C enum.

	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param gearset
	 *        The new motor gearset
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
	 *
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
	std::int32_t set_gearing(const pros::motor_gearset_e_t gearset, const std::uint8_t index = 0) const;

	/**
	 * Sets the reverse flag for the motor.
	 *
	 * This will invert its movements and the values returned for its position.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param reverse
	 *        True reverses the motor, false is default direction
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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

	/**
	 * Sets the position for the motor in its encoder units.
	 *
	 * This will be the future reference point for the motor's "absolute"
	 * position.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param position
	 *        The new reference position in its encoder units
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
	 *
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
	 *   motor.move_absolute(100, 100); // Moves 20 units forward
	 * }
	 * \endcode
	 *
	 */
	std::int32_t set_zero_position(const double position, const std::uint8_t index = 0) const;

	/**
	 * Sets the "absolute" zero position of the motor to its current position.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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

	/**
	 * Gets the number of motors.
	 *
	 * \return Always returns 1
	 *
	 */
	std::int8_t size(void) const;

	/**
	 * Gets all motors.
	 *
	 * \return A vector of Motor objects.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   std::vector<Motor> motor_all = pros::Motor::get_all_devices();  // All motors that are connected
	 * }
	 * \endcode
	 */
	static std::vector<Motor> get_all_devices();

	/**
	 * gets the port number of the motor
	 *
	 * \return The signed port of the motor. (negative if the motor is reversed)
	 *
	 */
	std::int8_t get_port(const std::uint8_t index = 0) const;

	///@}

	/// \name Additional motor functions
	/// These functions allow for motors and motor groups to be used interchangeably
	///@{
	/**
	 * Gets a vector containing the target position set for the motor by the user
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 *
	 * \return A vector containing the target position in its encoder units or PROS_ERR_F if the
	 * operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void autonomous() {
	 *   pros::Motor motor (1);
	 *   motor.move_absolute(100, 100);
	 *   std::cout << "Motor Target: " << motor.get_target_position_all()[0];
	 *   // Prints 100
	 * }
	 * \endcode
	 */
	std::vector<double> get_target_position_all(void) const;

	/**
	 * Gets a vector containing the velocity commanded to the motor by the user
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return A vector containing the commanded motor velocity from +-100,
	 * +-200, or +-600, or PROS_ERR if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y));
	 *     std::cout << "Motor Velocity: " << motor.get_target_velocity_all()[0];
	 *     // Prints the value of E_CONTROLLER_ANALOG_LEFT_Y
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<std::int32_t> get_target_velocity_all(void) const;

	/**
	 * Gets a vector containing the actual velocity commanded of the motor
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return A vector containing the motor's actual velocity in RPM or PROS_ERR_F
	 * if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y));
	 *     std::cout << "Motor Velocity: " << motor.get_actual_velocity_all()[0];
	 *     // Prints the value of E_CONTROLLER_ANALOG_LEFT_Y
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<double> get_actual_velocity_all(void) const;

	/**
	 * Gets a vector containing the current drawn by the motor in mA.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 *
	 * \return A vector containing the motor's current in mA or PROS_ERR if the operation failed,
	 * setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Current Draw: " << motor.get_current_draw_all()[0];
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<std::int32_t> get_current_draw_all(void) const;

	/**
	 * Gets a vector containing the direction of movement for the motor.
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
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Direction: " << motor.get_direction_all()[0];
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<std::int32_t> get_direction_all(void) const;

	/**
	 * Gets a vector containing the efficiency of the motor in percent.
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
	 * \return A vector containing The motor's efficiency in percent or PROS_ERR_F if the operation
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
	std::vector<double> get_efficiency_all(void) const;

	/**
	 * Gets a vector of the faults experienced by the motor.
	 *
	 * Compare this bitfield to the bitmasks in pros::motor_fault_e_t.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
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
	 *     std::cout << "Motor Faults: " << motor.get_faults_all()[0];
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<std::uint32_t> get_faults_all(void) const;

	/**
	 * Gets a vector of the flags set by the motor's operation.
	 *
	 * Compare this bitfield to the bitmasks in pros::motor_flag_e_t.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
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
	 *     std::cout << "Motor Faults: " << motor.get_faults_all()[0];
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<std::uint32_t> get_flags_all(void) const;

	/**
	 * Gets a vector containing the absolute position of the motor in its encoder units.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor

	 *
	 * \return A vector containing the motor's absolute position in its encoder units or PROS_ERR_F
	 * if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Position: " << motor.get_position_all()[0];
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<double> get_position_all(void) const;

	/**
	 * Gets a vector containing the power drawn by the motor in Watts.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return A vector containing the motor's power draw in Watts or PROS_ERR_F if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Power: " << motor.get_power_all()[0];
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<double> get_power_all(void) const;

	/**
	 * Gets a vector of the raw encoder count of the motor at a given timestamp.
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
	 * \return A vector containing the raw encoder count at the given timestamp or PROS_ERR if the
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
	std::vector<std::int32_t> get_raw_position_all(std::uint32_t* const timestamp) const;

	/**
	 * Gets a vector of the temperature of the motor in degrees Celsius.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return A vector contaioning the motor's temperature in degrees Celsius
	 * or PROS_ERR_F if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Temperature: " << motor.get_temperature_all()[0];
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<double> get_temperature_all(void) const;

	/**
	 * Gets a vector of the torque generated by the motor in Newton Meters (Nm).
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return A vector containing the motor's torque in Nm or PROS_ERR_F if the operation failed,
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
	std::vector<double> get_torque_all(void) const;

	/**
	 * Gets a vector of the voltage delivered to the motor in millivolts.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 *
	 * \return A vector of the motor's voltage in mV or PROS_ERR_F if the operation failed,
	 * setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Voltage: " << motor.get_voltage_all()[0];
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<std::int32_t> get_voltage_all(void) const;

	/**
	 * Checks if the motor is drawing over its current limit.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return A vector containing 1 if the motor's current limit is being exceeded and 0 if the
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
	 *     std::cout << "Is the motor over its current limit?: " << motor.is_over_current_all()[0];
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<std::int32_t> is_over_current_all(void) const;

	/**
	 * Gets the temperature limit flag for the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return A vector containing 1 if the temperature limit is exceeded and 0 if the temperature is
	 * below the limit, or PROS_ERR if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Is the motor over its temperature limit?: " << motor.is_over_temp_all();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<std::int32_t> is_over_temp_all(void) const;

	/**
	 * Gets a vector containing the brake mode that was set for the motor.
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
	std::vector<MotorBrake> get_brake_mode_all(void) const;

	/**
	 * Gets a vector containing the current limit for the motor in mA.
	 *
	 * The default value is 2500 mA.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return A vector containing the motor's current limit in mA or PROS_ERR if the operation failed,
	 * setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Motor motor (1);
	 *   while (true) {
	 *     std::cout << "Motor Current Limit: " << motor.get_current_limit_all()[0];
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<std::int32_t> get_current_limit_all(void) const;

	/**
	 * Gets a vector containing the encoder units that were set for the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return A vector containing One of Motor_Units according to what is set for the
	 * motor or E_MOTOR_ENCODER_INVALID if the operation failed.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::Motor motor (1, E_MOTOR_GEARSET_06, E_MOTOR_ENCODER_COUNTS);
	 *   std::cout << "Motor Encoder Units: " << motor.get_encoder_units_all()[0];
	 * }
	 * \endcode
	 */
	std::vector<MotorUnits> get_encoder_units_all(void) const;

	/**
	 * Gets a vector containing the gearset that was set for the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return A vector containing one of Motor_Gears according to what is set for the motor,
	 * or pros::Motor_Gears::invalid if the operation failed.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::Motor motor (1, E_MOTOR_GEARSET_06, E_MOTOR_ENCODER_COUNTS);
	 *   std::cout << "Motor Gearing: " << motor.get_gearing_all()[0];
	 * }
	 * \endcode
	 */
	std::vector<MotorGears> get_gearing_all(void) const;

	/**
	 * Gets returns a vector with all the port numbers in the motor group.
	 *
	 * \return A vector containing the signed port of the motor. (negative if the motor is reversed)
	 */
	std::vector<std::int8_t> get_port_all(void) const;

	/**
	 * Gets a vector of the voltage limit set by the user.
	 *
	 * Default value is 0V, which means that there is no software limitation
	 * imposed on the voltage.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return A vector containing the motor's voltage limit in V or PROS_ERR if the operation failed,
	 * setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::Motor motor (1);
	 *   std::cout << "Motor Voltage Limit: " << motor.get_voltage_limit_all()[0];
	 * }
	 * \endcode
	 */
	std::vector<std::int32_t> get_voltage_limit_all(void) const;

	/**
	 * Gets a vector containg whether the motor is reversed or not
	 *
	 * \return A vector containing 1 if the motor has been reversed and 0 if the motor was not
	 * reversed, or PROS_ERR if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::Motor motor (1);
	 *   std::cout << "Is the motor reversed? " << motor.is_reversed_all()[0];
	 *   // Prints "0"
	 * }
	 * \endcode
	 */
	std::vector<std::int32_t> is_reversed_all(void) const;

	/**
	 * Gets a vector containing the type of the motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \return A vector containing one of MotorType according to the type of the motor,
	 * or pros::MotorType::invalid if the operation failed.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::Motor motor (1, E_MOTOR_GEARSET_06, E_MOTOR_ENCODER_COUNTS);
	 *   std::cout << "Motor Type: " << motor.get_type_all()[0];
	 * }
	 * \endcode
	 */
	std::vector<MotorType> get_type_all(void) const;

	/**
	 * Sets one of Motor_Brake to the motor.
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
	 *   motor.set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
	 *   std::cout << "Brake Mode: " << motor.get_brake_mode();
	 * }
	 * \endcode
	 */
	std::int32_t set_brake_mode_all(const MotorBrake mode) const;

	/**
	 * Sets one of Motor_Brake to the motor.
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
	 *   motor.set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
	 *   std::cout << "Brake Mode: " << motor.get_brake_mode();
	 * }
	 * \endcode
	 */
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
	 * motor.set_current_limit_all(1000);
	 * while (true) {
	 *   motor = controller_get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *   // The motor will reduce its output at 1000 mA instead of the default 2500 mA
	 *   pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t set_current_limit_all(const std::int32_t limit) const;

	/**
	 * Sets one of Motor_Units for the motor encoder. Works with the C
	 * enum and the C++ enum class.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * * \param units
	 *        The new motor encoder units
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::Motor motor (1);
	 *   motor.set_encoder_units_all(E_MOTOR_ENCODER_DEGREES);
	 *   std::cout << "Encoder Units: " << motor.get_encoder_units();
	 * }
	 * \endcode
	 */
	std::int32_t set_encoder_units_all(const MotorUnits units) const;

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
	 *   motor.set_encoder_units_all(E_MOTOR_ENCODER_DEGREES);
	 *   std::cout << "Encoder Units: " << motor.get_encoder_units();
	 * }
	 * \endcode
	 */
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
	 *   motor.set_gearing_all(E_MOTOR_GEARSET_06);
	 *   std::cout << "Gearset: " << motor.get_gearing();
	 * }
	 * \endcode
	 */
	std::int32_t set_gearing_all(const MotorGears gearset) const;

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
	 *   motor.set_gearing_all(E_MOTOR_GEARSET_06);
	 *   std::cout << "Gearset: " << motor.get_gearing();
	 * }
	 * \endcode
	 */
	std::int32_t set_gearing_all(const pros::motor_gearset_e_t gearset) const;

	/**
	 * Sets the reverse flag for the motor.
	 *
	 * This will invert its movements and the values returned for its position.
	 *
	 *
	 * \param reverse
	 *        True reverses the motor, false is default direction
	 *
	 * \return 1
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::Motor motor (1);
	 *   motor.set_reversed_all(true);
	 *   std::cout << "Is this motor reversed? " << motor.is_reversed();
	 * }
	 * \endcode
	 */
	std::int32_t set_reversed_all(const bool reverse);

	/**
	 * Sets the voltage limit for the motor in Volts.
	 *
	 * \note This is one of many Motor functions that takes in an optional index parameter.
	 * 		 This parameter can be ignored by most users but exists to give a shared base class
	 * 		 for motors and motor groups
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 *
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EOVERFLOW - The index is non 0
	 *
	 * \param limit
	 *        The new voltage limit in Volts
	 *
	 * \param index Optional parameter.
	 * 		  The zero-indexed index of the motor to get the target position of.
	 * 		  By default index is 0, and will return an error for a non-zero index
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
	 *   motor.set_voltage_limit_all(10000);
	 *   while (true) {
	 *     motor = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     // The motor will not output more than 10 V
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
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
	 *   motor.set_zero_position_all(80);
	 *   motor.move_absolute(100, 100); // Moves 20 units forward
	 * }
	 * \endcode
	 *
	 */
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
	 *   motor.tare_position_all();
	 *   motor.move_absolute(100, 100); // Moves 100 units forward
	 * }
	 * \endcode
	 */
	std::int32_t tare_position_all(void) const;

	///@}

	private:
	/**
	 * The port of the motor. Negative ports indicate that the motor is reversed
	 */
	std::int8_t _port;
};
namespace literals {
/**
 * Constructs a Motor from a literal ending in _mtr
 *
 * \return a pros::Motor for the corresponding port
 *
 * \b Example
 * \code
 * using namespace pros::literals;
 * void opcontrol() {
 *	pros::Motor motor = 2_mtr; //Makes an Motor object on port 2
 * }
 * \endcode
 */
const pros::Motor operator"" _mtr(const unsigned long long int m);
/**
 * Constructs a reversed Motor from a literal ending in _rmtr
 *
 * \return a pros::Motor for the corresponding port that is reversed
 *
 * \b Example
 * \code
 * using namespace pros::literals;
 * void opcontrol() {
 *	pros::motor motor = 2_rmtr; //Makes an reversed Motor object on port 2
 * }
 * \endcode
 */
const pros::Motor operator"" _rmtr(const unsigned long long int m);
}  // namespace literals
}  // namespace v5
}  // namespace pros
#endif  // _PROS_MOTORS_HPP_
