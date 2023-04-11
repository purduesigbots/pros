/**
 * \file pros/gps.hpp
 * \ingroup cpp-gps
 *
 * Contains prototypes for functions related to the VEX GPS.
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
 * \defgroup cpp-gps VEX GPS Sensor C API
 * \note For a pros-specific usage guide on the GPS, please check out our article [here.](@ref gps)
 */

#ifndef _PROS_GPS_HPP_
#define _PROS_GPS_HPP_

#include <stdbool.h>

#include <cstdint>
#include <iostream>

#include "pros/gps.h"
#include "pros/device.hpp"

namespace pros {
inline namespace v5 {
/**
 * \ingroup cpp-gps
 *  @{
 */
class Gps : public Device {
	/**
	 * \addtogroup cpp-gps
	 *  @{
	 */

	public:

	/**
	 * Creates a GPS object for the given port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param port
	 *        The V5 port number from 1-21
	 * \b Example:
	 * \code
	 * pros::Gps gps(1);
	 * \endcode
	 *
	 */
	explicit Gps(const std::uint8_t port) : Device(port, DeviceType::gps){};

	/**
	 * Creates a GPS object for the given port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param port
	 *        The V5 port number from 1-21
	 * \param xInitial
	 * 				 Cartesian 4-Quadrant X initial position (meters)
	 * \param yInitial
	 * 				 Cartesian 4-Quadrant Y initial position (meters)
	 * \param headingInitial
	 * 				 Initial heading (degrees)
	 *
	 * \b Example:
	 * \code
	 * pros::Gps gps(1, 1.30, 1.20, 90);
	 * \endcode
	 *
	 */
	explicit Gps(const std::uint8_t port, double xInitial, double yInitial, double headingInitial) : Device(port, DeviceType::gps){
		pros::c::gps_set_position(port, xInitial, yInitial, headingInitial);
	};

	/**
	 * Creates a GPS object for the given port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param port
	 *        The V5 port number from 1-21
	 * \param xOffset
	 * 				 Cartesian 4-Quadrant X offset from center of turning (meters)
	 * \param yOffset
	 * 				 Cartesian 4-Quadrant Y offset from center of turning (meters)
	 *
	 * \b Example: 
	 * \code
	 * pros::Gps gps(1, 1.30, 1.20);
	 * \endcode
	 *
	 */
	explicit Gps(const std::uint8_t port, double xOffset, double yOffset) : Device(port, DeviceType::gps){
		pros::c::gps_set_offset(port, xOffset, yOffset);
	};

	/**
	 * Creates a GPS object for the given port.
	 * 
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param port
	 *        The V5 port number from 1-21
	 * \param xInitial
	 * 				 Initial 4-Quadrant X Position, with (0,0) being at the center of the field (meters)
	 * \param yInitial
	 * 				 Initial 4-Quadrant Y Position, with (0,0) being at the center of the field (meters)
	 * \param headingInitial
	 * 				 Initial Heading, with 0 being North, 90 being East, 180 being South, and 270 being West (degrees)
	 * \param xOffset
	 * 				 Cartesian 4-Quadrant X offset from center of turning (meters)
	 * \param yOffset
	 * 				 Cartesian 4-Quadrant Y offset from center of turning (meters)
	 *
	 * \b Example:
	 * \code
	 * pros::Gps gps(1, 1.30, 1.20, 180, 1.30, 1.20);
	 * \endcode
	 *
	 */
	explicit Gps(const std::uint8_t port, double xInitial, double yInitial, double headingInitial, double xOffset, double yOffset)
	     : Device(port, DeviceType::gps){
		pros::c::gps_initialize_full(port, xInitial, yInitial, headingInitial, xOffset, yOffset);
	};

	/**
	 * Set the GPS's offset relative to the center of turning in meters,
	 * as well as its initial position.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  xOffset
	 * 				 Cartesian 4-Quadrant X offset from center of turning (meters)
	 * \param  yOffset
	 * 				 Cartesian 4-Quadrant Y offset from center of turning (meters)
	 * \param  xInitial
	 * 				 Initial 4-Quadrant X Position, with (0,0) being at the center of the field (meters)
	 * \param  yInitial
	 * 				 Initial 4-Quadrant Y Position, with (0,0) being at the center of the field (meters)
	 * \param  headingInitial
	 *  			 Heading with 0 being north on the field, in degrees [0,360) going clockwise
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT, 1.1, 1.2, 180, .4, .4);
	 *  // this is equivalent to the above line
	 *  gps.initialize_full(1.1, 1.2, 180, .4, .4);
	 *   while (true) {
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t initialize_full(double xInitial, double yInitial, double headingInitial, double xOffset,
	                                     double yOffset) const;

	/**
	 * Set the GPS's offset relative to the center of turning in meters.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  xOffset
	 * 				 Cartesian 4-Quadrant X offset from center of turning (meters)
	 * \param  yOffset
	 * 				 Cartesian 4-Quadrant Y offset from center of turning (meters)
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT, 1.1, 1.2, 180, .4, .4);
	 *  // this is equivalent to the above line
	 *  gps.set_offset(.4, .4);
	 *   while (true) {
	 *	 	delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t set_offset(double xOffset, double yOffset) const;

	/**
	* Get the GPS's cartesian location relative to the center of turning/origin in meters.
	*
	* This function uses the following values of errno when an error state is
	* reached:
	* ENXIO - The given value is not within the range of V5 ports (1-21).
	* ENODEV - The port cannot be configured as a GPS
	* EAGAIN - The sensor is still calibrating
	*
	* \param  port
	* 				 The V5 GPS port number from 1-21
	* \return A struct (gps_position_s_t) containing the X and Y values if the operation
	* failed, setting errno.
	* 
	* \b Example
	* \code
	* #define GPS_PORT 1
	* 
	* void opcontrol() {
	*   gps_position_s_t pos;
	* 	Gps gps(GPS_PORT);
	*   while (true) {
	*     pos = gps.get_offset();
	*     screen_print(TEXT_MEDIUM, 1, "X Offset: %4d, Y Offset: %4d", pos.x, pos.y);
	*     delay(20);
	*   }
	* }
	* \endcode
	*/
	virtual pros::gps_position_s_t get_offset() const;

	/**
	 * Sets the robot's location relative to the center of the field in meters.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  xInitial
	 * 				 Initial 4-Quadrant X Position, with (0,0) being at the center of the field (meters)
	 * \param  yInitial
	 * 				 Initial 4-Quadrant Y Position, with (0,0) being at the center of the field (meters)
	 * \param  headingInitial
	 *  			 Heading with 0 being north on the field, in degrees [0,360) going clockwise
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 * 
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  gps.set_position(1.3, 1.4, 180);
	 *   while (true) {
	 *     printf("X: %f, Y: %f, Heading: %f\n", gps.get_position().x, 
	 * 	   gps.get_position().y, gps.get_position().heading);
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t set_position(double xInitial, double yInitial, double headingInitial) const;

	/**
	 * Set the GPS sensor's data rate in milliseconds, only applies to IMU on GPS.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  rate
	 * 				 Data rate in milliseconds (Minimum: 5 ms)
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  gps.set_data_rate(10);
	 *   while (true) {
	 *	 	printf("X: %f, Y: %f, Heading: %f\n", gps.get_position().x,
	 * 	   gps.get_position().y, gps.get_position().heading);
	 *	 	delay(10);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t set_data_rate(std::uint32_t rate) const;

	/**
	 * Get the possible RMS (Root Mean Squared) error in meters for GPS position.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \return Possible RMS (Root Mean Squared) error in meters for GPS position.
	 * If the operation failed, returns PROS_ERR_F and errno is set.
	 *
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  double error = gps.get_error();
	 *	printf("Error: %f\n", error);
	 *  pros::delay(20);
	 * }
	 * \endcode
	 */
	virtual double get_error() const;

	/**
	 * Gets the position and roll, yaw, and pitch of the GPS.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 *
	 * \return A struct (gps_status_s_t) containing values mentioned above.
	 * If the operation failed, all the structure's members are filled with
	 * PROS_ERR_F and errno is set.
	 * 
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  gps_status_s_t status;
	 *   while (true) {
	 *     status = gps.get_status();
	 *     printf("X: %f, Y: %f, Heading: %f, Roll: %f, Pitch: %f, Yaw: %f\n",
	 * 	   status.x, status.y, status.heading, status.roll, status.pitch, status.yaw);
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual pros::gps_status_s_t get_status() const;

	/**
	 * Gets the x and y position on the field of the GPS in meters.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \return A struct (gps_position_s_t) containing values mentioned above.
	 * If the operation failed, all the structure's members are filled with
	 * PROS_ERR_F and errno is set.
	 *
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  gps_position_s_t position;
	 *   while (true) {
	 *	 position = gps.get_position();
	 *     printf("X: %f, Y: %f, Heading: %f\n", position.x, position.y,
	 * 	   position.heading);
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual pros::gps_position_s_t get_position() const;

	/**
	 * Get the heading in [0,360) degree values.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 *
	 * \return The heading in [0,360) degree values. If the operation failed,
	 * returns PROS_ERR_F and errno is set.
	 *
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  while(true) {
	 *		double heading = gps.get_heading();
	 *		printf("Heading: %f\n", heading);
	 *  	pros::delay(20);
	 *  }
	 * }
	 * \endcode
	 */
	virtual double get_heading() const;

	/**
	 * Get the heading in the max double value and min double value scale.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \return The heading in [DOUBLE_MIN, DOUBLE_MAX] values. If the operation
	 * fails, returns PROS_ERR_F and errno is set.
	 *
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  while(true) {
	 *		double heading = gps.get_heading_raw();
	 *		printf("Heading: %f\n", heading);
	 *  	pros::delay(20);
	 *  }
	 * }
	 * \endcode
	 */
	virtual double get_heading_raw() const;

	/**
	 * Gets the GPS sensor's elapsed rotation value
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \return The elased heading in degrees. If the operation fails, returns
	 * PROS_ERR_F and errno is set.
	 *
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  while(true) {
			double rotation = gps.get_rotation();
	 *		printf("Rotation: %f\n", rotation);
	 *  	pros::delay(20);
	 *  }
	 * }
	 * \endcode
	 */
	virtual double get_rotation() const;

	/**
	 * Set the GPS sensor's rotation value to target value
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  target
	 * 				 Target rotation value to set rotation value to
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  double rotation = gps.set_rotation(90);
	 *  while(true) {
	 *		printf("Rotation: %f\n", rotation);
	 *  	pros::delay(20);
	 *  }
	 * }
	 * \endcode
	 */
	virtual std::int32_t set_rotation(double target) const;

	/**
	 * Tare the GPS sensor's rotation value
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 * 
	 * \b Example: 
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  gps.tare_rotation();
	 *  while(true) {
	 * 	    Should be around 0 on first call since it was tared. 
	 *		printf("Rotation: %f\n", rotation);
	 *  	pros::delay(20);
	 *  }
	 * }
	 * \endcode
	 */
	virtual std::int32_t tare_rotation() const;

	/**
	 * Get the GPS's raw gyroscope values
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \return The raw gyroscope values. If the operation failed, all the
	 * structure's members are filled with PROS_ERR_F and errno is set.
	 *
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  while(true) {
	 *		pros::gps_gyro_s_t gyro = gps.get_gyro_rate();
	 *		printf("Gyro: %f, %f, %f\n", gyro.x, gyro.y, gyro.z);
	 *  	pros::delay(20);
	 *  }
	 * }
	 * \endcode
	 */
	virtual pros::gps_gyro_s_t get_gyro_rate() const;

	/**
	 * Get the GPS's raw accelerometer values
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 GPS's port number from 1-21
	 * \return The raw accelerometer values. If the operation failed, all the
	 * structure's members are filled with PROS_ERR_F and errno is set.
	 */
	virtual pros::gps_accel_s_t get_accel() const;

	/**
	 * This is the overload for the << operator for printing to streams
	 * 
	 * Prints in format:
	 * Gps [port: gps._port, x: (x position), y: (y position), heading: (gps heading), rotation: (gps rotation)]
	 *
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  while(true) {
	 *		std::cout << gps << std::endl;
	 *  	pros::delay(20);
	 *  }
	 * }
	 * \endcode
	 */
	friend std::ostream& operator<<(std::ostream& os, const pros::Gps& gps);

///@}
};  // Gps Class

namespace literals {
	/**
	 * Constructs a Gps object with the given port number
	 * 
	 * \b Example
	 * \code
	 * using namespace literals;
	 * 
	 * void opcontrol() {
	 * 	 pros::Gps gps = 1_gps;
	 *   while (true) {
	 *     pos = gps.get_position();
	 *     screen_print(TEXT_MEDIUM, 1, "X Position: %4d, Y Position: %4d", pos.x, pos.y);
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	const pros::Gps operator""_gps(const unsigned long long int g);
}  // namespace literals

/// @brief
/// Alias for Gps is GPS for user convenience.
using GPS = Gps;

} // namespace v5
} // namespace pros

#endif
