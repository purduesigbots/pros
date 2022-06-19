/**
 * \file pros/rotation.hpp
 * \ingroup cpp-rotation
 *
 * Contains prototypes for functions related to the VEX Rotation Sensor.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/rotation.html to learn
 * more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2022, Purdue University ACM SIGBots.
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

#include "pros/rotation.h"

namespace pros {
inline namespace v5 {
/**
 * \ingroup cpp-rotation
 */
class Rotation {
	/**
	 * \addtogroup cpp-rotation
	 *  @{
	 */
	const std::uint8_t _port;

	public:

	/**
	 * Constructor for the rotation sensor
	 * 
	 * \param port The V5 port number from 1-21
	 * 
	 * \b Example
	 * \code
	 * #define ROTATION_PORT 1
	 * 
	 * void initialize() {
	 *   pros::Rotation rotation_sensor(ROTATION_PORT);
	 * }
	 * \endcode
	 */
	Rotation(const std::uint8_t port) : _port(port){};

	/**
	 * Constructor for the rotation sensor
	 * 
	 * \param port The V5 port number from 1-21
	 * \param reverse_flag 
	 * 
	 * \b Example
	 * \code
	 * #define ROTATION_PORT 1
	 * 
	 * void initialize() {
	 *   pros::Rotation rotation_sensor(ROTATION_PORT, 0);
	 * }
	 * \endcode
	 */
	Rotation(const std::uint8_t port, const bool reverse_flag);

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
	 * \b Example
	 * \code
	 * #define ROTATION_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Rotation rotation_sensor(ROTATION_PORT);
	 *   while (true) {
	 *     if(controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_X)){
	 *       rotation_sensor.reverse(); //Rotation sensor recently reversed
	 *       rotation_sensor.reset(); //Reversal causes position to be multiplied by -1.
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
	 * \b Example
	 * \code
	 * #define ROTATION_PORT 1
	 * 
	 * void initialize() {
	 *   pros::Rotation rotation_sensor(ROTATION_PORT);
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
	 * \b Example
	 * \code
	 * #define ROTATION_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Rotation rotation_sensor(ROTATION_PORT);
	 *   while (true) {
	 *     printf("Tick Position: %ld \n", rotation_sensor.get_position());
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t set_position(std::uint32_t position);

	/**
	 * Reset the Rotation Sensor to a desired rotation value
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
	 * #define ROTATION_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Rotation rotation_sensor(ROTATION_PORT);
	 *   while (true) {
	 *     if(controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_X)){
	 *       rotation_sensor.reset_position();
	 *     }
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t reset_position(void);

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
	 * \b Example
	 * \code
	 * #define ROTATION_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Rotation rotation_sensor(ROTATION_PORT);
	 *   while (true) {
	 *     printf("Tick Position: %ld \n", rotation_sensor.get_position());
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t get_position();

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
	 * \return The value or PROS_ERR_F if the operation failed, setting
	 * errno.
	 * 
	 * \b Example
	 * \code
	 * #define ROTATION_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Rotation rotation_sensor(ROTATION_PORT);
	 *   while (true) {
	 *     printf("Rotational Velocity: %ld \n", rotation_sensor.get_velocity());
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t get_velocity();

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
	 * \b Example
	 * \code
	 * #define ROTATION_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Rotation rotation_sensor(ROTATION_PORT);
	 *   while (true) {
	 *     printf("Angle: %ld \n", rotation_sensor.get_angle());
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t get_angle();

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
	 * \b Example
	 * \code
	 * #define ROTATION_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Rotation rotation_sensor(ROTATION_PORT);
	 *   while (true) {
	 *     if(controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_X)){
	 *       rotation_sensor.set_reversed(true);
	 *     }
	 *   pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t set_reversed(bool value);

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
	 * \b Example
	 * \code
	 * #define ROTATION_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Rotation rotation_sensor(ROTATION_PORT);
	 *   while (true) {
	 *     if(controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_X)){
	 *       rotation_sensor.reverse();
	 *     }
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t reverse();

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
	 * \b Example
	 * \code
	 * #define ROTATION_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Rotation rotation_sensor(ROTATION_PORT);
	 *   while (true) {
	 *     printf("Rotation Sensor Reversed: %ld \n", rotation_sensor.get_reversed());
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t get_reversed();
	///@}
};
}  // namespace v5
}  // namespace pros

#endif
