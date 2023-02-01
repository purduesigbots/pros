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
 * \copyright (c) 2017-2023, Purdue University ACM SIGBots.
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
#include <iostream>

#include "pros/device.hpp"
#include "pros/distance.h"

namespace pros {
inline namespace v5 {
/**
 * \ingroup cpp-distance
 */
class Distance : public Device {
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
	 * \param port
	 *        The V5 port number from 1-21
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
	 */
	virtual double get_object_velocity();

    /**
     * This is the overload for the << operator for printing to streams
     *
     * Prints in format(this below is all in one line with no new line):
     * Distance [port: (port number), distance: (distance), confidence: (confidence),
     * object size: (object size), object velocity: (object velocity)]
     */
	friend std::ostream& operator<<(std::ostream& os, pros::Distance& distance);

	/**
     * Returns the type of device
     *
	 */
	pros::DeviceType get_type() const;

	private:
	///@}
};

namespace literals {
const pros::Distance operator"" _dist(const unsigned long long int d);
}  // namespace literals
}
}  // namespace pros

#endif
