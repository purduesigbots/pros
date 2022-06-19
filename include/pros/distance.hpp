/**
 * \file pros/distance.hpp
 * \ingroup cpp-distance
 *
 * Contains prototypes for the V5 Distance Sensor-related functions.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/distance.html to learn
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
 * \defgroup cpp-distance VEX Distance Sensor C++ API
 */

#ifndef _PROS_DISTANCE_HPP_
#define _PROS_DISTANCE_HPP_

#include <cstdint>

#include "pros/distance.h"

namespace pros {
inline namespace v5 {
/**
 * \ingroup cpp-distance
 */
class Distance {
	/**
	 * \addtogroup cpp-distance
	 *  @{
 	*/
	public:
	/**
	 * Creates a Distance Sensor object for the given port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a Distance Sensor
	 *
	 * \param port The V5 port number from 1-21
	 * 
	 * \b Example
	 * \code
	 * #define DISTANCE_PORT 1
	 * 
	 * void initialize() {
	 *   pros::Distance distance_sensor(DISTANCE_PORT);
	 * }
	 * \endcode
	 */
	explicit Distance(const std::uint8_t port);

	/**
	 * Get the currently measured distance from the sensor in mm
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Distance Sensor
	 *
	 * \return The distance value or PROS_ERR if the operation failed, setting
	 * errno.
	 * 
	 * \b Example
	 * \code
	 * #define DISTANCE_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Distance distance_sensor(DISTANCE_PORT);
	 *   while (true) {
	 *     printf("Distance: %d mm\n", distance_sensor.get());
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t get();

	/**
	 * Get the confidence in the distance reading
	 *
	 * This is a value that has a range of 0 to 63. 63 means high confidence,
	 * lower values imply less confidence. Confidence is only available
	 * when distance is > 200mm.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Distance Sensor
	 *
	 * \return The confidence value or PROS_ERR if the operation failed, setting
	 * errno.
	 * 
	 * \b Example
	 * \code
	 * #define DISTANCE_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Distance distance_sensor(DISTANCE_PORT);
	 *   while (true) {
	 *     printf("Distance Confidence Value: %d\n", distance_sensor.get_confidence(DISTANCE_PORT));
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t get_confidence();

	/**
	 * Get the current guess at relative object size
	 *
	 * This is a value that has a range of 0 to 400.
	 * A 18" x 30" grey card will return a value of approximately 75
	 * in typical room lighting.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Distance Sensor
	 *
	 * \return The size value or PROS_ERR if the operation failed, setting
	 * errno.
	 * 
	 * \b Example
	 * \code
	 * #define DISTANCE_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Distance distance_sensor(DISTANCE_PORT);
	 *   while (true) {
	 *     printf("Distance Object Size: %d\n", distance_sensor.get_object_size(DISTANCE_PORT));
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t get_object_size();

	/**
	 * Get the object velocity in m/s
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Distance Sensor
	 *
	 * \return The velocity value or PROS_ERR if the operation failed, setting
	 * errno.
	 * \b Example
	 * \code
	 * #define DISTANCE_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Distance distance_sensor(DISTANCE_PORT);
	 *   while (true) {
	 *     printf("Distance Object Velocity: %d\n", distance_sensor.get_object_velocity(DISTANCE_PORT));
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual double get_object_velocity();

	/**
	 * Gets the port number of the distance sensor.
	 *
	 * \return The distance sensor's port number.
	 * 
	 * \b Example
	 * \code
	 * #define DISTANCE_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Distance distance_sensor(DISTANCE_PORT);
	 *   printf("Distance Sensor Port: %d\n", distance_sensor.get_port());
	 * }
	 * \endcode
	 */
	std::uint8_t get_port();

	private:
	const std::uint8_t _port;
	///@}
};

}  // namespace v5
}  // namespace pros

#endif
