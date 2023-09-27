/**
 * \file pros/motor_group.hpp
 * \ingroup cpp-motor-group
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
 * \defgroup cpp-motor-group Motor Groups C++ API
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
	 * \addtogroup cpp-motor-group
	 *  @{
	 */
	public:

	/**
	 * Constructs a new MotorGroup object.
	 * 
	 * This function uses the following values of errno when an error state is
 	 * reached:
 	 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 	 * ENODEV - The port cannot be configured as a motor
	 * 
	 * \param port
 	 *        A initializer list of V5 port numbers from 1 to 21, or from -21 to -1 for reversed motors. 
	 * 		  A reversed motor will reverse the input or output movement functions and movement related
	 * 		  telemetry in order to produce consistant behavior with non-reversed motors
	 * 
	 * \param gearset =  pros::v5::MotorGears::green
	 * 		  Optional parameter for the gearset for the motor.
	 * 		  set to pros::v5::MotorGears::green if not specifed. 
	 * 
	 * \param encoder_units = pros::v5::MotorUnits::degrees
	 * 		  Optional parameter for the encoder units of the motor
	 * 		  set to pros::v5::MotorUnits::degrees if not specified by the user
	 * 
	 *  \b Example
 	 * \code
 	 * void opcontrol() {
	 * 	MotorGroup first_mg({1, -2}); //Creates a motor on port 1  and a reversed motor on port 2 with 
	 *  with both motors using the green gearset and degrees as the encoder units
	 *  MotorGroup rotations_mg({4, 5}, pros::v5::MotorGears::blue, pros::v5::MotorUnits::rotations);
 	 *  //Creates a motor group on ports 4 and 5 with blue motors using rotaions as the encoder units
 	 * }
	 * \endcode
	 */
	explicit MotorGroup(const std::initializer_list<std::int8_t>, const pros::v5::MotorGears gearset = pros::v5::MotorGears::green,
	                    const pros::v5::MotorUnits encoder_units = pros::v5::MotorUnits::degrees);
	/**
	 * Constructs a new MotorGroup object.
	 * 
	 * This function uses the following values of errno when an error state is
 	 * reached:
 	 * 
	 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 	 * 
	 * ENODEV - The port cannot be configured as a motor
	 * 
	 * EDOM - The motor group is empty
	 * 
	 * \param port
 	 *        A initializer list of V5 port numbers from 1 to 21, or from -21 to -1 for reversed motors. 
	 * 		  A reversed motor will reverse the input or output movement functions and movement related
	 * 		  telemetry in order to produce consistant behavior with non-reversed motors
	 * 
	 * \param gearset =  pros::v5::MotorGears::green
	 * 		  Optional parameter for the gearset for the motor.
	 * 		  set to pros::v5::MotorGears::green if not specifed. 
	 * 
	 * \param encoder_units = pros::v5::MotorUnits::degrees
	 * 		  Optional parameter for the encoder units of the motor
	 * 		  set to pros::v5::MotorUnits::degrees if not specified by the user
	 * 
	 *  \b Example
 	 * \code
 	 * void opcontrol() {
	 * 	MotorGroup first_mg({1, -2}); //Creates a motor on port 1  and a reversed motor on port 2 with 
	 *  with both motors using the green gearset and degrees as the encoder units
	 *  MotorGroup rotations_mg({4, 5}, pros::v5::MotorGears::blue, pros::v5::MotorUnits::rotations);
 	 *  //Creates a motor group on ports 4 and 5 with blue motors using rotaions as the encoder units
 	 * }
	 * \endcode
	 */
	explicit MotorGroup(const std::vector<std::int8_t>& ports, const pros::v5::MotorGears gearset = pros::v5::MotorGears::green,
	                    const pros::v5::MotorUnits encoder_units = pros::v5::MotorUnits::degrees);

	 /**
	 * Constructs a new MotorGroup object from an abstract motor.
	 * 
	 * This function uses the following values of errno when an error state is
 	 * reached:
 	 * 
	 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 	 * 
	 * ENODEV - The port cannot be configured as a motor
	 * 
	 * EDOM - The motor group is empty
	 * 
	 * \param abstract_motor
	 * 		  THe abstract motor to turn into a motor group
	 * 		  Uses abstract_motor.get_port_all() to get the vector of ports
	 * 
	 * 
	 *  \b Example
 	 * \code
 	 * void opcontrol() {
	 * 	MotorGroup first_mg({1, -2}); //Creates a motor on port 1  and a reversed motor on port 2 with 
	 *  with both motors using the green gearset and degrees as the encoder units
	 * 	AbstractMotor abs_mtr_group = first_mg;
	 * 	MotorGroup new_mg = (MotorGroup) abs_mtr_group;
 	 * }
	 * \endcode
	 */
	
	MotorGroup(AbstractMotor& abstract_motor);
	/// \name Motor movement functions
	/// These functions allow programmers to make motors move
	///@{

	/**
	 * Sets the voltage for the motor group from -128 to 127.
	 *
	 * This is designed to map easily to the input from the controller's analog
	 * stick for simple opcontrol use. The actual behavior of the motor is
	 * analogous to use of pros::Motor::move()
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - the motor group is empty
	 * 
	 * \param voltage
	 *        The new voltage from -127 to 127
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup MotorGroup ({1,3}, E_MOTOR_GEARSET_18);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t operator=(std::int32_t voltage) const;

	/**
	 * Sets the voltage for the motor group from -127 to 127.
	 *
	 * This is designed to map easily to the input from the controller's analog
	 * stick for simple opcontrol use. The actual behavior of the motor is
	 * analogous to use of motor_move()
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 *
	 * \param voltage
	 *        The new voltage from -127 to 127
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup MotorGroup ({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg.move(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y));
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t move(std::int32_t voltage) const;

	/**
	 * Sets the target absolute position for the motor group to move to.
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
	 * EDOM - the motor group has size 0
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
	 *   pros::MotorGroup mg ({1,3});
	 *   mg.move_absolute(100, 100); // Moves 100 units forward
	 *   while (!((mg.get_position() < 105) && (mg.get_position() > 95))) {
	 *     // Continue running this loop as long as the mg is not within +-5 units of its goal
	 *     pros::delay(2);
	 *   }
	 *   mg.move_absolute(100, 100); // This does not cause a movement
	 *   while (!((mg.get_position() < 105) && (mg.get_position() > 95))) {
	 *     pros::delay(2);
	 *   }
	 *   mg.tare_position();
	 *   mg.move_absolute(100, 100); // Moves 100 units forward
	 *   while (!((mg.get_position() < 105) && (mg.get_position() > 95))) {
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t move_absolute(const double position, const std::int32_t velocity) const;

	/**
	 * Sets the relative target position for the motor group to move to.
	 *
	 * This movement is relative to the current position of each motor as given in
	 * pros::MotorGroup::get_position(). Providing 10.0 as the position parameter
	 * would result in the motor moving 10 units, no matter what the
	 * current position is.
	 *
	 * \note This function simply sets the target for the motor, it does not block
	 * program execution until the movement finishes.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
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
	 *   pros::MotorGroup mg({1,3});
	 *   mg.move_relative(100, 100); // Moves 100 units forward
	 *   while (!((mg.get_position() < 105) && (mg.get_position() > 95))) {
	 *     // Continue running this loop as long as the motor is not within +-5 units of its goal
	 *     pros::delay(2);
	 *   }
	 *   mg.move_relative(100, 100); // Also moves 100 units forward
	 *   while (!((mg.get_position() < 205) && (mg.get_position() > 195))) {
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t move_relative(const double position, const std::int32_t velocity) const;

	/**
	 * Sets the velocity for the motor group.
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
	 * EDOM - The motor group is empty
	 *
	 * \param velocity
	 *        The new velocity from +-100, +-200, or +-600 depending on the
	 *        motor's gearset
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void autonomous() {
	 *   pros::MotorGroup mg({1,3});
	 *   mg.move_velocity(100);
	 *   pros::delay(1000); // Move at 100 RPM for 1 second
	 *   mg.move_velocity(0);
	 * }
	 * \endcode
	 */
	std::int32_t move_velocity(const std::int32_t velocity) const;

	/**
	 * Sets the output voltage for the motor group from -12000 to 12000 in millivolts.
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
	 *
	 * \b Example
	 * \code
	 * void autonomous() {
	 *   mg.move_voltage(12000);
	 *   pros::delay(1000); // Move at max voltage for 1 second
	 *   mg.move_voltage(0);
	 * }
	 * \endcode
	 */
	std::int32_t move_voltage(const std::int32_t voltage) const;


	/**
	 * Stops the motor group using the currently configured brake mode.
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
	 * EDOM - The motor group is empty
	* 
	* \return 1 if the operation was successful or PROS_ERR if the operation
	* failed, setting errno.
	* 
	* \b Example
	* \code 
	*  void autonomous() {
	*	Motor motor(1);
	*   mg.move_voltage(12000);
	*   pros::delay(1000); // Move at max voltage for 1 second
	*   motor.brake();
	* }
	* \endcode
	*/
	std::int32_t brake(void) const;

	/**
	 * Changes the output velocity for a profiled movement (move_absolute or
	 * move_relative). This will have no effect if the motor group is not following
	 * a profiled movement.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
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
	 * 	 pros::MotorGroup mg ({1,3});
	 *   mg.move_absolute(100, 100);
	 *   pros::delay(100);
	 *   mg.modify_profiled_velocity(0); // Stop the motor group early
	 * }
	 * \endcode
	 */
	std::int32_t modify_profiled_velocity(const std::int32_t velocity) const;
	
	/**
	 * Gets the target position set for a motor in the motor group, with a parameter
	 * for the motor index.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 * 
	 *
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 *
	 * \return The target position in its encoder units or PROS_ERR_F if the
	 * operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void autonomous() {
	 *   pros::MotorGroup mg({1,3});
	 *   mg.move_absolute(100, 100);
	 * 	// get the target position from motor at index 1. (port 3)
	 *   std::cout << "Motor Target: " << mg.get_target_position(1);
	 *   // Prints 100
	 * }
	 * \endcode
	 */
	double get_target_position(const std::uint8_t index = 0) const;

	/**
	 * Gets a vector of the the target positions set for the motor group
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The Motor group is empty
	 *
	 * \return The a vector of the target positions in its encoder units or PROS_ERR_F if the
	 * operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void autonomous() {
	 *   pros::MotorGroup mg({1,3});
	 *   mg.move_absolute(100, 100);
	 *   std::cout << "Motor Target: " << mg.get_target_position_all()[0];
	 *   // Prints 100
	 * }
	 * \endcode
	 */
	std::vector<double> get_target_position_all(void) const;

	/**
	 * Gets the velocity commanded to the motor by the user at the index specified.
	 * 
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 * EDOM - The motor group was empty
	 * 
	 * \param index Optional parameter. 
	 * 				The zero indexed index of the motor in the motor group
	 * 
	 * \return The commanded motor velocity from +-100, +-200, or +-600, or
	 * PROS_ERR if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg ({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y));
	 * 		// get the target velocity from motor at index 1. (port 3)
	 *     std::cout << "Motor Velocity: " << mg.get_target_velocity(1); 
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t get_target_velocity(const std::uint8_t index = 0) const;

	/**
	 * Gets a vector of the velocity commanded to the motor by the user
	 * 
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - THe motor group is empty
	 * 
	 * \return A vector of the commanded motor velocity from +-100, +-200, or +-600, or
	 * PROS_ERR if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg ({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y));
	 *     std::cout << "Motor Velocity: " << mg.get_target_velocity_all(); 
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<std::int32_t> get_target_velocity_all(void) const;

	///@}

	/// \name Motor telemetry functions
	/// These functions allow programmers to collect telemetry from motors
	///@{

	/**
	 * Gets the actual velocity of a motor in the motor group.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
	 * EDOM - THe motor group is empty
	 *
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 * 
	 * \param index Optional parameter. 
	 * 				The zero indexed index of the motor in the motor group 
	 *
	 * \return The motor's actual velocity in RPM or PROS_ERR_F if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   while (true) {
	 *     mg = controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_LEFT_Y);
	 *	   // get the actual velocity from motor at index 1. (port 3)
	 *     printf("Actual velocity: %lf\n", mg.get_actual_velocity(1));
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	double get_actual_velocity(const std::uint8_t index = 0) const;
	
	/**
	 * Gets a vector of the the actual velocity of each motor the motor group.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
	 * EDOM - THe motor group is empty
	 *
	 * \return A vector of the each motor's actual velocity in RPM or PROS_ERR_F if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   while (true) {
	 *     mg = controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_LEFT_Y);
	 *	   // get the target velocity from motor at index 1. (port 3)
	 *     printf("Actual velocity: %lf\n", mg.get_actual_velocity(1));
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<double> get_actual_velocity_all(void) const;

	/**
	 * Gets the current drawn by a motor in the motor group in mA.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *  
	 * EDOM - The motor group is empty
	 *
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 * 
	 * \param index Optional parameter. 
	 * 				The zero indexed index of the motor in the motor group 
	 *
	 * \return The motor's current in mA or PROS_ERR if the operation failed,
	 * setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 * 	   //Print the current draw for the motor at index 1. (port 3)
	 *     std::cout << "Motor Current Draw: " << mg.get_current_draw(1);
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t get_current_draw(const std::uint8_t index = 0) const;
	/**
	 * Gets a vector of the current drawn each motor in the motor group in mA.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *  
	 * EDOM - The motor group is empty
	 *
	 *
	 * \return A vector with each motor's current in mA or PROS_ERR if the operation failed,
	 * setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Current Draw: " << mg.get_current_draw_all();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<std::int32_t> get_current_draw_all(void) const;

	/**
	 * Gets the direction of movement for a motor in the motor group.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
	 * EDOM - The motor group is empty
	 * 
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 * 
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 * 
	 * \return 1 for moving in the positive direction, -1 for moving in the
	 * negative direction, and PROS_ERR if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 * 	   //Print the motor direction for the motor at index 1. (port 3)
	 *     std::cout << "Motor Direction: " << mg.get_direction();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t get_direction(const std::uint8_t index = 0) const;
	/**
	 * Gets a vector of the directions of movement for each motor in the motor group.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * EDOM - The motor group is empty
	 * 
	 * \return 1 for moving in the positive direction, -1 for moving in the
	 * negative direction, and PROS_ERR if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Direction: " << mg.get_direction_all()[0];
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<std::int32_t> get_direction_all(void) const;

	/**
	 * Gets the efficiency of a motor in the motor group in percent.
	 *
	 * An efficiency of 100% means that the motor is moving electrically while
	 * drawing no electrical power, and an efficiency of 0% means that the motor
	 * is drawing power but not moving.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
	 * EDOM - The motor group is empty
	 * 
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 * 
	 * 
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 *
	 * \return The motor's efficiency in percent or PROS_ERR_F if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     //Prints the efficiency of the motor at index 1 (port 3)
	 *     std::cout << "Motor Efficiency: " << mg.get_efficiency(1);
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	double get_efficiency(const std::uint8_t index = 0) const;
	/**
	 * Gets a vector of the efficiency of each motor in percent.
	 *
	 * An efficiency of 100% means that the motor is moving electrically while
	 * drawing no electrical power, and an efficiency of 0% means that the motor
	 * is drawing power but not moving.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
	 * EDOM - THe motor group is empty
	 *
	 * \return A vector containing each motor's efficiency in percent or PROS_ERR_F if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Efficiency: " << mg.get_efficiency_all()[0];
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<double> get_efficiency_all(void) const;

	/**
	 * Gets the faults experienced by a motor in the motor group.
	 *
	 * Compare this bitfield to the bitmasks in pros::motor_fault_e_t.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
	 * EDOM - The motor group is empty
	 * 
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 *
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 *
	 * \return A bitfield containing the motor's faults.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Faults: " << mg.get_faults();
	 * pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::uint32_t get_faults(const std::uint8_t index = 0) const;
	/**
	 * Gets a vector of the faults experienced by each motor in the motor group.
	 *
	 * Compare these bitfields to the bitmasks in pros::motor_fault_e_t.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * 
	 * EDOM - The motor group is empty
	 * 
	 *
	 * \return A vector containing the bitfields containing each motor's faults.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Faults: " << mg.get_faults_all();
	 * pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<std::uint32_t> get_faults_all(void) const;
	/**
	 * Gets the flags set by a motor in the motor group's operation.
	 *
	 * Compare this bitfield to the bitmasks in pros::motor_flag_e_t.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 *
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 *
	 * \return A bitfield containing the motor's flags.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Faults: " << mg.get_faults(1);
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::uint32_t get_flags(const std::uint8_t index = 0) const;
	
	/**
	 * Gets a vector of the flags set by each motor in the motor groups's operation.
	 *
	 * Compare this bitfield to the bitmasks in pros::motor_flag_e_t.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 *
	 * \return A bitfield containing the motor's flags.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Faults: " << mg.get_faults_all()[0];
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<std::uint32_t> get_flags_all(void) const;

	/**
	 * Gets the absolute position of a motor in the motor group in its encoder units.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 * 
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 *
	 * \return The motor's absolute position in its encoder units or PROS_ERR_F
	 * if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Position: " << mg.get_position(1);
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	double get_position(const std::uint8_t index = 0) const;
	/**
	 * Gets a vector of the absolute position of each motor in its encoder units.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 *
	 * \return A vector of the motor's absolute position in its encoder units or PROS_ERR_F
	 * if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Position: " << mg.get_position_all();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<double> get_position_all(void) const;

	/**
	 * Gets the power drawn by a motor in  the motor group in Watts.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 * 
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 *
	 * \return The motor's power draw in Watts or PROS_ERR_F if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Power: " << mg.get_power();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	double get_power(const std::uint8_t index = 0) const;
	/**
	 * Gets a vector of the power drawn by each motor in the motor group in Watts.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 *
	 * \return A vector of each motor's power draw in Watts or PROS_ERR_F if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Power: " << mg.get_power_all()[0];
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<double> get_power_all(void) const;
	/**
	 * Gets the raw encoder count of a motor in the motor group at a given timestamp.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 * 
	 * 
	 * \param timestamp
	 *            A pointer to a time in milliseconds for which the encoder count
	 *            will be returned. If NULL, the timestamp at which the encoder
	 *            count was read will not be supplied
	 * 
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 *
	 * \return The raw encoder count at the given timestamp or PROS_ERR if the
	 * operation failed.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   std::uint32_t now = pros::millis();
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Position: " << mg.get_raw_position(&now);
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t get_raw_position(std::uint32_t* const timestamp, const std::uint8_t index = 0) const;
	/**
	 * Gets the raw encoder count of each motor at a given timestamp.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 *
	 * \param timestamp
	 *            A pointer to a time in milliseconds for which the encoder count
	 *            will be returned. If NULL, the timestamp at which the encoder
	 *            count was read will not be supplied
	 *
	 * \return A vector of each raw encoder count at the given timestamp or PROS_ERR if the
	 * operation failed.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   std::uint32_t now = pros::millis();
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Position: " << mg.get_raw_position_all(&now)[0];
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<std::int32_t> get_raw_position_all(std::uint32_t* const timestamp) const;

	/**
	 * Gets the temperature of a motor in the motor group in degrees Celsius.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 *
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 * 
	 * \return The motor's temperature in degrees Celsius or PROS_ERR_F if the
	 * operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Temperature: " << mg.get_temperature();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	double get_temperature(const std::uint8_t index = 0) const;
	/**
	 * Gets the temperature of each motor  in the motor group in degrees Celsius.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 *
	 * \return A vector of each motor's temperature in degrees Celsius or PROS_ERR_F if the
	 * operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Temperature: " << mg.get_temperature_all()[1];
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<double> get_temperature_all(void) const;
	/**
	 * Gets the torque generated by a motor in the motor groupin Newton Meters (Nm).
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 *
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 * 
	 * \return The motor's torque in Nm or PROS_ERR_F if the operation failed,
	 * setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Torque: " << mg.get_torque();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	double get_torque(const std::uint8_t index = 0) const;
	/**
	 * Gets a vector of the torque generated by each motor in Newton Meters (Nm).
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 *
	 * \return A vector containing each motor's torque in Nm or PROS_ERR_F if the operation failed,
	 * setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Torque: " << mg.get_torque();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<double> get_torque_all(void) const;
	/**
	 * Gets the voltage delivered to a motor in the motor group in millivolts.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 * 
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 * 
	 * \return The motor's voltage in mV or PROS_ERR_F if the operation failed,
	 * setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Voltage: " << mg.get_voltage();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t get_voltage(const std::uint8_t index = 0) const;
	/**
	 * Gets a vector of the voltage delivered to each motor in millivolts.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 *
	 * \return A vector of each motor's voltage in mV or PROS_ERR_F if the operation failed,
	 * setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Motor Voltage: " << mg.get_voltage_all()[0];
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<std::int32_t> get_voltage_all(void) const;

	/**
	 * Checks if a motor in the motor group is drawing over its current limit.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 * 
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 * 
	 * \return 1 if the motor's current limit is being exceeded and 0 if the
	 * current limit is not exceeded, or PROS_ERR if the operation failed, setting
	 * errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Is the motor over its current limit?: " << mg.is_over_current();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t is_over_current(const std::uint8_t index = 0) const;
	/**
	 * Checks if each motor in the motor group is drawing over its current limit.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 *
	 * \return A vector containing the following for each motor: 1 if the motor's current limit is being exceeded and 0 if the
	 * current limit is not exceeded, or PROS_ERR if the operation failed, setting
	 * errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Is the motor over its current limit?: " << motor.is_over_current_all()[0];
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<std::int32_t> is_over_current_all(void) const;

	/**
	 * Gets the temperature limit flag for a motor in the motor group.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 * 
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 *
	 * \return 1 if the temperature limit is exceeded and 0 if the temperature is
	 * below the limit, or PROS_ERR if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Is the motor over its temperature limit?: " << motor.is_over_temp();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t is_over_temp(const std::uint8_t index = 0) const;
	/**
	 * Gets a vector with the temperature limit flag for each motor in the motor group.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 *
	 * \return 1 if the temperature limit is exceeded and 0 if the temperature is
	 * below the limit, or PROS_ERR if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     std::cout << "Is the motor over its temperature limit?: " << motor.is_over_temp();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<std::int32_t> is_over_temp_all(void) const;

	///@}

	/// \name Motor configuration functions
	/// These functions allow programmers to configure the behavior of motors
	///@{

	/**
	 * Gets the brake mode that was set for a motor in the motor group.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 * 
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 *
	 * \return One of Motor_Brake, according to what was set for the
	 * motor, or E_MOTOR_BRAKE_INVALID if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::MotorGroup mg({1,3});
	 *   mg.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	 *   std::cout << "Brake Mode: " << mg.get_brake_mode();
	 * }
	 * \endcode
	 */
	MotorBrake get_brake_mode(const std::uint8_t index = 0) const;
	/**
	 * Gets a vector with the brake mode that was set for each motor in the motor group.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 *
	 * \return A vector with one of Motor_Brake for each motor in the motor group, according to what was set for the
	 * motor, or E_MOTOR_BRAKE_INVALID if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::MotorGroup mg({1,3});
	 *   mg.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	 *   std::cout << "Brake Mode: " << mg.get_brake_mode_all()[0];
	 * }
	 * \endcode
	 */
	std::vector<MotorBrake> get_brake_mode_all(void) const;

	/**
	 * Gets the current limit for a motor in the motor group in mA.
	 *
	 * The default value is 2500 mA.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 * 
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 *
	 * \return The motor's current limit in mA or PROS_ERR if the operation failed,
	 * setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   while (true) {
	 *     std::cout << "Motor Current Limit: " << mg.get_current_limit();
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t get_current_limit(const std::uint8_t index = 0) const;
	
	/**
	 * Gets a vector of the current limit for each motor in the motor group in mA.
	 *
	 * The default value is 2500 mA.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 *
	 * \return A vector of each motor's current limit in mA or PROS_ERR if the operation failed,
	 * setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   while (true) {
	 *     std::cout << "Motor Current Limit: " << mg.get_current_limit_all()[0];
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::vector<std::int32_t> get_current_limit_all(void) const;

	/**
	 * Gets the encoder units that were set for a motor in the motor group.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 * 
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 * 
	 * \return One of Motor_Units according to what is set for the
	 * motor or E_MOTOR_ENCODER_INVALID if the operation failed.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::MotorGroup mg ({1,3}, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_COUNTS);
	 *   std::cout << "Motor Encoder Units: " << mg.get_encoder_units();
	 * }
	 * \endcode
	 */
	MotorUnits get_encoder_units(const std::uint8_t index = 0) const;
	
	/**
	 * Gets a vector of the encoder units that were set for each motor in the motor group.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 *
	 * \return A vector with the following for each motor, One of Motor_Units according to what is set for the
	 * motor or E_MOTOR_ENCODER_INVALID if the operation failed.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::MotorGroup mg ({1,3}, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_COUNTS);
	 *   std::cout << "Motor Encoder Units: " << mg.get_encoder_units_all()[0];
	 * }
	 * \endcode
	 */
	std::vector<MotorUnits> get_encoder_units_all(void) const;

	/**
	 * Gets the gearset that was set for a motor in the motor group.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 * 
	 *\param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 * 
	 * \return One of Motor_Gears according to what is set for the motor,
	 * or pros::Motor_Gears::invalid if the operation failed.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::MotorGroup mg ({1,3}, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_COUNTS);
	 *   std::cout << "Motor Gearing: " << mg.get_gearing();
	 * }
	 * \endcode
	 */
	MotorGears get_gearing(const std::uint8_t index = 0) const;
	/**
	 * Gets a vector of the gearset that was set for each motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 *
	 * 
	 * \return A vector with one of Motor_Gears according to what is set for the motor,
	 * or pros::Motor_Gears::invalid if the operation failed for each motor.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::MotorGroup mg ({1,3}, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_COUNTS);
	 *   std::cout << "Motor Gearing: " << mg.get_gearing_all()[0];
	 * }
	 * \endcode
	 */
	std::vector<MotorGears> get_gearing_all(void) const;

	/**
	 * Gets a vector with all the port numbers in the motor group.
	 * A port will be negative if the motor in the motor group is reversed
	 * 
	 * @return a vector with all the port numbers for the motor group
	 */
	std::vector<std::int8_t> get_port_all(void) const;

	/**
	 * Gets the voltage limit of a motor in the motor group set by the user.
	 *
	 * Default value is 0V, which means that there is no software limitation
	 * imposed on the voltage.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 * 
	 *\param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 *
	 * \return The motor's voltage limit in V or PROS_ERR if the operation failed,
	 * setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::MotorGroup mg({1,3});
	 *   std::cout << "Motor Voltage Limit: " << mg.get_voltage_limit(1);
	 * }
	 * \endcode
	 */
	std::int32_t get_voltage_limit(const std::uint8_t index = 0) const;

	/**
	 * Gets a vector of the voltage limit of each motor in the motor group 
	 *
	 * Default value is 0V, which means that there is no software limitation
	 * imposed on the voltage.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The Motor group is empty
	 *
	 * \return The motor's voltage limit in V or PROS_ERR if the operation failed,
	 * setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::MotorGroup mg({1,3});
	 *   std::cout << "Motor Voltage Limit: " << mg.get_voltage_limit_all()[0];
	 * }
	 * \endcode
	 */
	std::vector<std::int32_t> get_voltage_limit_all(void) const;

	/**
	 * Gets the operation direction of a motor in the motor group as set by the user.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * 
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 * 
	 *\param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 *
	 * \return 1 if the motor has been reversed and 0 if the motor was not
	 * reversed, or PROS_ERR if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::MotorGroup mg({1,3});
	 *   std::cout << "Is the motor reversed? " << motor.is_reversed();
	 *   // Prints "0"
	 * }
	 * \endcode
	 */
	std::int32_t is_reversed(const std::uint8_t index = 0) const;
	/**
	 * Gets a vector of the operation direction of each motor in the motor group as set by the user.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EDOM - The motor group is empty
	 *
	 * \return A vector conatining the following for each motor: 1 if the motor has been reversed and 0 if the motor was not
	 * reversed, or PROS_ERR if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::MotorGroup mg({1,3});
	 *   std::cout << "Is the motor reversed? " << motor.is_reversed_all()[0];
	 *   // Prints "0"
	 * }
	 * \endcode
	 */
	std::vector<std::int32_t> is_reversed_all(void) const;

	/**
	 * Sets one of Motor_Brake to a motor in the motor group. Works with the C enum
	 * and the C++ enum class.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 * 
	 * \param mode
	 *        The Motor_Brake to set for the motor
	 * 
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 * 
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::MotorGroup mg({1,3});
	 *   mg.set_brake_mode(pros::MotorBrake::brake, 1);
	 *   std::cout << "Brake Mode: " << mg.get_brake_mode();
	 * }
	 * \endcode
	 */
	std::int32_t set_brake_mode(const MotorBrake mode, const std::uint8_t index = 0) const;
	/**
	 * Sets one of Motor_Brake to a motor in the motor group. Works with the C enum
	 * and the C++ enum class.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 * 
	 * \param mode
	 *        The Motor_Brake to set for the motor
	 * 
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 * 
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::MotorGroup mg({1,3});
	 *   mg.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD, 1);
	 *   std::cout << "Brake Mode: " << mg.get_brake_mode();
	 * }
	 * \endcode
	 */
	std::int32_t set_brake_mode(const pros::motor_brake_mode_e_t mode, const std::uint8_t index = 0) const;
	/**
	 * Sets one of Motor_Brake all the motors in the motor group. Works with the C enum
	 * and the C++ enum class.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * 
	 * \param mode
	 *        The Motor_Brake to set for the motor
	 * 
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::MotorGroup mg({1,3});
	 *   mg.set_brake_mode_all(pros::MotorBrake:brake);
	 *   std::cout << "Brake Mode: " << mg.get_brake_mode();
	 * }
	 * \endcode
	 */
	std::int32_t set_brake_mode_all(const MotorBrake mode) const;
	/**
	 * Sets one of Motor_Brake to a motor in the motor group. Works with the C enum
	 * and the C++ enum class.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * 
	 * \param mode
	 *        The Motor_Brake to set for the motor
	 *
	 * 
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::MotorGroup mg({1,3});
	 *   mg.set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
	 *   std::cout << "Brake Mode: " << mg.get_brake_mode();
	 * }
	 * \endcode
	 */
	std::int32_t set_brake_mode_all(const pros::motor_brake_mode_e_t mode) const;
	/**
	 * Sets the current limit for one motor in the motor group in mA.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()
	 *
	 * \param limit
	 *        The new current limit in mA
	 * 
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 * 
	 * 
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *
	 * mg.set_current_limit(1000);
	 * while (true) {
	 *   mg = controller_get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *   // The motor will reduce its output at 1000 mA instead of the default 2500 mA
	 *   pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t set_current_limit(const std::int32_t limit, const std::uint8_t index = 0) const;
	/**
	 * Sets the current limit for every motor in the motor group in mA.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group was empty
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
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *
	 * mg.set_current_limit_all(1000);
	 * while (true) {
	 *   mg = controller_get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *   // The motor will reduce its output at 1000 mA instead of the default 2500 mA
	 *   pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t set_current_limit_all(const std::int32_t limit) const;
	/**
	 * Sets one of Motor_Units for one motor in the motor group's motor encoder. Works with the C
	 * enum and the C++ enum class.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size() 
	 *
	 * \param units
	 *        The new motor encoder units
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 * 
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::MotorGroup mg({1,3});
	 *   mg.set_encoder_units(E_MOTOR_ENCODER_DEGREES, 1);
	 *   std::cout << "Encoder Units: " << mg.get_encoder_units();
	 * }
	 * \endcode
	 */
	std::int32_t set_encoder_units(const MotorUnits units, const std::uint8_t index = 0) const;
	/**
	 * Sets one of Motor_Units for one motor in the motor group's motor encoder. Works with the C
	 * enum and the C++ enum class.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size() 
	 *
	 * \param units
	 *        The new motor encoder units
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 * 
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::MotorGroup mg({1,3});
	 *   mg.set_encoder_units(E_MOTOR_ENCODER_DEGREES, 1);
	 *   std::cout << "Encoder Units: " << mg.get_encoder_units();
	 * }
	 * \endcode
	 */
	std::int32_t set_encoder_units(const pros::motor_encoder_units_e_t units, const std::uint8_t index = 0) const;
	/**
	 * Sets one of Motor_Units for every motor in the motor group's motor encoder. Works with the C
	 * enum and the C++ enum class.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty 
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
	 *   pros::MotorGroup mg({1,3});
	 *   mg.set_encoder_units_all(E_MOTOR_ENCODER_DEGREES);
	 *   std::cout << "Encoder Units: " << mg.get_encoder_units();
	 * }
	 * \endcode
	 */
	std::int32_t set_encoder_units_all(const MotorUnits units) const;
	/**
	 * Sets one of Motor_Units for every motor in the motor group's motor encoder. Works with the C
	 * enum and the C++ enum class.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty 
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
	 *   pros::MotorGroup mg({1,3});
	 *   mg.set_encoder_units_all(E_MOTOR_ENCODER_DEGREES);
	 *   std::cout << "Encoder Units: " << mg.get_encoder_units();
	 * }
	 * \endcode
	 */
	std::int32_t set_encoder_units_all(const pros::motor_encoder_units_e_t units) const;
	/**
	 * Sets one of the gear cartridge (red, green, blue) for one motor in the motor group. Usable with
	 * the C++ enum class and the C enum.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size() 
	 *
	 * \param gearset
	 *        The new geatset of the motor
	 * 
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 * 
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::MotorGroup mg({1,3});
	 *   mg.set_gearing(pros::MotorGears::blue, 1);
	 *   std::cout << "Gearset: " << mg.get_gearing();
	 * }
	 * \endcode
	 */
	std::int32_t set_gearing(const MotorGears gearset, const std::uint8_t index = 0) const;
	/**
	 * Sets one of the gear cartridge (red, green, blue) for one motor in the motor group. Usable with
	 * the C++ enum class and the C enum.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size() 
	 *
	 * \param gearset
	 *        The new geatset of the motor
	 * 
	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 * 
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::MotorGroup mg({1,3});
	 *   mg.set_gearing(E_MOTOR_GEARSET_06, 1);
	 *   std::cout << "Gearset: " << mg.get_gearing();
	 * }
	 * \endcode
	 */
	std::int32_t set_gearing(const pros::motor_gearset_e_t gearset, const std::uint8_t index = 0) const;
	/**
	 * Sets one of the gear cartridge (red, green, blue) for one motor in the motor group. Usable with
	 * the C++ enum class and the C enum.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 *
	 * \param gearset
	 *        The new geatset of the motor
	 * 
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::MotorGroup mg({1,3});
	 *   mg.set_gearing_all(E_MOTOR_GEARSET_06);
	 *   std::cout << "Gearset: " << mg.get_gearing();
	 * }
	 * \endcode
	 */
	std::int32_t set_gearing_all(const MotorGears gearset) const;
	/**
	 * Sets one of the gear cartridge (red, green, blue) for every motor in the motor group. Usable with
	 * the C++ enum class and the C enum.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 *
	 * \param gearset
	 *        The new geatset of the motor
	 * 
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::MotorGroup mg({1,3});
	 *   mg.set_gearing_all(E_MOTOR_GEARSET_06);
	 *   std::cout << "Gearset: " << mg.get_gearing();
	 * }
	 * \endcode
	 */
	std::int32_t set_gearing_all(const pros::motor_gearset_e_t gearset) const;

	/**
	 * sets the reversal for a motor in the motor group.
	 *
	 * This will invert its movements and the values returned for its position.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size()  
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
	 *   pros::MotorGroup mg({1,3});
	 * 		//reverse the motor at index 1 (port 3)
	 *   mg.set_reversed(true, 1);
	 *   std::cout << "Is this motor reversed? " << motor.is_reversed(1);
	 * pros::delay(100);
	 * 	// unreverse the motor at index 1 (port 3)
	 *   mg.set_reversed(false, 1);
	 *   std::cout << "Is this motor reversed? " << motor.is_reversed(1);
	 * }
	 * \endcode
	 */
	std::int32_t set_reversed(const bool reverse, const std::uint8_t index = 0);
	/**
	 * Sets the reversal for all the motors in the motor group.
	 *
	 * This will invert its movements and the values returned for its position.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EDOM - The motor group is empty
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
	 *   pros::MotorGroup mg({1,3});
	 *   mg.set_reversed_all(true);
	 *   std::cout << "Is this motor reversed? " << motor.is_reversed();
	 * }
	 * \endcode
	 */
	std::int32_t set_reversed_all(const bool reverse);

	/**
	 * Sets the voltage limit for a motor in the motor group in millivolts.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size() 
	 *
	 * \param limit
	 *        The new voltage limit in Volts
	 * 
	 * 	\param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void autonomous() {
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *
	 *   mg.set_voltage_limit(10000, 1);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     // The motor on at index 1 (port 3) will not output more than 10 V
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t set_voltage_limit(const std::int32_t limit, const std::uint8_t index = 0) const;

	/**
	 * Sets the voltage limit for every motor in the motor group in millivolts.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
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
	 *   pros::MotorGroup mg({1,3});
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *
	 *   mg.set_voltage_limit_all(10000);
	 *   while (true) {
	 *     mg = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	 *     // The motor will not output more than 10 V
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t set_voltage_limit_all(const std::int32_t limit) const;

	/**
	 * Sets the position for a motor in the motor group in its encoder units.
	 *
	 * This will be the future reference point for the motor's "absolute"
	 * position.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size() 
	 * 
	 * \param position
	 *        The new reference position in its encoder units
	 * \param index Optional parameter, 0 by default.
	 * 		  The zero indexed index of the motor in the motor group
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void autonomous() {
	 *   pros::MotorGroup mg({1,3});
	 *   mg.move_absolute(100, 100); // Moves 100 units forward
	 *   mg.move_absolute(100, 100); // This does not cause a movement
	 *   mg.set_zero_position(80);
	 *   mg.set_zero_position(80, 1);
	 *   mg.move_absolute(100, 100); // Moves 80 units forward
	 * }
	 * \endcode
	 *
	 */
	std::int32_t set_zero_position(const double position, const std::uint8_t index = 0) const;
	/**
	 * Sets the position for every motor in the motor group in its encoder units.
	 *
	 * This will be the future reference point for the motor's "absolute"
	 * position.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
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
	 *   pros::MotorGroup mg({1,3});
	 *   mg.move_absolute(100, 100); // Moves 100 units forward
	 *   mg.move_absolute(100, 100); // This does not cause a movement
	 *
	 *   mg.set_zero_position_all(80);
	 *   mg.move_absolute(100, 100); // Moves 80 units forward
	 * }
	 * \endcode
	 *
	 */
	std::int32_t set_zero_position_all(const double position) const;

	/**
	 * Sets the "absolute" zero position of a motor in the motor group to its current position.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a motor
	 * EDOM - The motor group is empty
	 * EOVERFLOW - The index is greater than or equal to MotorGroup::size() 
	 *
	 * \param index Optional parameter, 0 by default.
	 * 		The zero indexed index of the motor in the motor group
	 * 
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void autonomous() {
	 *   pros::MotorGroup mg({1,3});
	 *   mg.move_absolute(100, 100); // Moves 100 units forward
	 *   mg.move_absolute(100, 100); // This does not cause a movement
	 *
	 *   mg.tare_position();
	 *   mg.tare_position(1); 
	 * 
	 *   mg.move_absolute(100, 100); // Moves 100 units forward
	 * }
	 * \endcode
	 */
	std::int32_t tare_position(const std::uint8_t index = 0) const;
	
	/**
	 * Sets the "absolute" zero position of every motor in the motor group to its current position.
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
	 *   pros::MotorGroup mg({1,3});
	 *   mg.move_absolute(100, 100); // Moves 100 units forward
	 *   mg.move_absolute(100, 100); // This does not cause a movement
	 *   mg.tare_position_all();
	 *   mg.move_absolute(100, 100); // Moves 100 units forward
	 * }
	 * \endcode
	 */
	std::int32_t tare_position_all(void) const;

	/**
	 * Returns the number of motors in the motor group
	 *
	 * \return the number of motors in the motor group
	 */
	std::int8_t size(void) const;

	/**
	 * Gets the port of a motor in the motor group
	 * 
	 * 	 * \param index Optional parameter, 0 by default.
	 * 				The zero indexed index of the motor in the motor group
	 * 
	 * \return The port of the motor at the specified index. 
	 * The return value is negative if the corresponding motor is reversed
	*/
	std::int8_t get_port(const std::uint8_t index = 0) const;

	/**
	 * Appends all the motors in the other motor group reference to this motor group
	 * 
	 * Maintains the order of the other motor group
	 *
	 */
	void operator+=(MotorGroup&);

	/**
	 * Appends all the motors in the other motor group reference to this motor group
	 * 
	 * Maintains the order of the other motor group
	 *
	 */
	void append(MotorGroup&);

	/**
	 * Removes the all motors on the port (regardless of reversal) from the motor group
	 *
	 * \param port The port to remove from the motor group
	 * 
	 */
	void erase_port(std::int8_t port);

	///@}
	private:
	/**
	 * The ordered vector of ports used by the motor group
	*/
	std::vector<std::int8_t> _ports;
	mutable pros::Mutex _MotorGroup_mutex;
};
}  // namespace v5
}  // namespace pros
#endif