/**
 * \file pros/rotation.hpp
 * \ingroup cpp-rotation
 *
 * Contains prototypes for functions related to the VEX Rotation Sensor.
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
 * \defgroup cpp-rotation VEX Rotation Sensor C++ API
 */
#ifndef _PROS_ROTATION_HPP_
#define _PROS_ROTATION_HPP_

#include <cstdint>
#include <iostream>

#include "pros/device.hpp"
#include "pros/rotation.h"

namespace pros {
inline namespace v5 {
/**
 * \ingroup cpp-rotation
 */
class Rotation : public Device {
	/**
	 * \addtogroup cpp-rotation
	 * @{
	 */

	public:
	/**
	 * Constructs a new Rotation Sensor object
	 *
	 * ENXIO - The given value is not within the range of V5 ports |1-21|.
	 * ENODEV - The port cannot be configured as a Rotation Sensor
	 *
	 * \param port
	 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed Rotation Sensors.
	 *
	 * 	\b Example
	 * \code
	 * void opcontrol() {
	 * 	 pros::Rotation rotation_sensor(1); //Creates a Rotation Sensor on port 1
	 *   pros::Rotation reversed_rotation_sensor(-2); //Creates a reversed Rotation Sensor on port 2
	 * }
	 * \endcode
	 */
	Rotation(const std::int8_t port);

	Rotation(const Device& device) : Rotation(device.get_port()){};


	/**
	 * Reset the Rotation Sensor
	 *
	 * Reset the current absolute position to be the same as the
	 * Rotation Sensor angle.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Rotation Sensor
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 *  \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Rotation rotation_sensor(1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     if(master.get_analog(E_CONTROLLER_DIGITAL_X) {
	 * 	     rotation_sensor.reset();
	 *     }
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t reset();

	/**
	 * Set the Rotation Sensor's refresh interval in milliseconds.
	 *
	 * The rate may be specified in increments of 5ms, and will be rounded down to
	 * the nearest increment. The minimum allowable refresh rate is 5ms. The default
	 * rate is 10ms.
	 *
	 * As values are copied into the shared memory buffer only at 10ms intervals,
	 * setting this value to less than 10ms does not mean that you can poll the
	 * sensor's values any faster. However, it will guarantee that the data is as
	 * recent as possible.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Rotation Sensor
	 *
	 * \param rate The data refresh interval in milliseconds
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 *  \b Example
	 * \code
	 * void initialize() {
	 *   pros::Rotation rotation_sensor(1);
	 *   rotation_sensor.set_data_rate(5);
	 * }
	 * \endcode
	 */
	virtual std::int32_t set_data_rate(std::uint32_t rate) const;

	/**
	 * Set the Rotation Sensor position reading to a desired rotation value
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Rotation Sensor
	 *
	 * \param position
	 * 		  The position in terms of ticks
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 *  \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Rotation rotation_sensor(1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     if(master.get_analog(E_CONTROLLER_DIGITAL_X) {
	 * 	     rotation_sensor.set_position(600);
	 *     }
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t set_position(std::uint32_t position) const;

	/**
	 * Reset the Rotation Sensor position to 0
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Rotation Sensor
	 *
	 * \param position
	 * 		  The position in terms of ticks
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Rotation rotation_sensor(1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     if(master.get_analog(E_CONTROLLER_DIGITAL_X) {
	 * 	     rotation_sensor.reset_position();
	 *     }
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t reset_position(void) const;

	/**
	 * Gets all rotation sensors.
	 * 
	 * \return A vector of Rotation sensor objects.
	 *
	 * \b Example
 	 * \code
	 * void opcontrol() {
	 *   std::vector<Rotation> rotation_all = pros::Rotation::get_all_devices();  // All rotation sensors that are connected
	 * }
 	 * \endcode
	 */
	static std::vector<Rotation> get_all_devices();

	/**
	 * Get the Rotation Sensor's current position in centidegrees
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Rotation Sensor
	 *
	 * \return The position value or PROS_ERR if the operation failed, setting
	 * errno.
	 *
	 *  \b Example
	 * \code
	 * void opcontrol() {
	 * 	 pros::Rotation rotation_sensor(1);
	 *   while (true) {
	 *     printf("Position: %d Ticks \n", rotation_sensor.get_position());
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t get_position() const;

	/**
	 * Get the Rotation Sensor's current velocity in centidegrees per second
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Rotation Sensor
	 *
	 * \param  port
	 * 				 The V5 Rotation Sensor port number from 1-21
	 * \return The velocity value or PROS_ERR if the operation failed, setting
	 * errno.
	 *
	 *  \b Example
	 * \code
	 * void opcontrol() {
	 * 	 pros::Rotation rotation_sensor(1);
	 *   while (true) {
	 *     printf("Velocity: %d centidegrees per second \n", rotation_sensor.get_velocity));
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t get_velocity() const;

	/**
	 * Get the Rotation Sensor's current position in centidegrees
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Rotation Sensor
	 *
	 * \return The angle value or PROS_ERR if the operation failed, setting
	 * errno.
	 *
	 *  \b Example
	 * \code
	 * void opcontrol() {
	 * 	 pros::Rotation rotation_sensor(1);
	 *   while (true) {
	 *     printf("Angle: %d centidegrees \n", rotation_sensor.get_angle());
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t get_angle() const;

	/**
	 * Set the Rotation Sensor's direction reversed flag
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Rotation Sensor
	 *
	 * \param  value
	 * 				 Determines if the direction of the rotational sensor is
	 * 				 reversed or not.
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 *  \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Rotation rotation_sensor(1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     if(master.get_analog(E_CONTROLLER_DIGITAL_X) {
	 * 	     rotation_sensor.set_reversed(true); // Reverses the Rotation Sensor
	 *     }
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t set_reversed(bool value) const;

	/**
	 * Reverse the Rotation Sensor's direction.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Rotation Sensor
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 *  \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Rotation rotation_sensor(1);
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     if(master.get_analog(E_CONTROLLER_DIGITAL_X) {
	 * 	     rotation_sensor.reverse();
	 *     }
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t reverse() const;

	/**
	 * Get the Rotation Sensor's reversed flag
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Rotation Sensor
	 *
	 * \return Reversed value or PROS_ERR if the operation failed, setting
	 * errno.
	 *
	 *  \b Example
	 * \code
	 * void opcontrol() {
	 * 	 pros::Rotation rotation_sensor(1);
	 *   while (true) {
	 *     printf("Reversed: %d \n", rotation_sensor.get_reversed());
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t get_reversed() const;

	/**
	 * This is the overload for the << operator for printing to streams
	 *
	 * Prints in format(this below is all in one line with no new line):
	 * Rotation [port: rotation._port, position: (rotation position), velocity: (rotation velocity),
	 * angle: (rotation angle), reversed: (reversed boolean)]
	 *
	 * \b Example
	 * \code
	 * #define ROTATION_PORT 1
	 *
	 * void opcontrol() {
	 * 	pros::Rotation rotation_sensor(1);
	 *  while(true) {
	 *		std::cout << rotation_sensor << std::endl;
	 *  	pros::delay(20);
	 *  }
	 * }
	 * \endcode
	 */
	friend std::ostream& operator<<(std::ostream& os, const pros::Rotation& rotation);

	///@}
};

namespace literals {
/**
 * Constructs a Rotation sensor from a literal ending in _rot
 *
 * \return a pros::Rotation for the corresponding port
 *
 * \b Example
 * \code
 * using namespace pros::literals;
 * void opcontrol() {
 *	pros::Rotation rotation = 2_rot; //Makes an Motor object on port 2
 * }
 * \endcode
 */
const pros::Rotation operator"" _rot(const unsigned long long int r);
}  // namespace literals
}  // namespace v5
}  // namespace pros

#endif
