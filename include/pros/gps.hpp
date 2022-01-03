/**
 * \file pros/gps.hpp
 *
 * Contains prototypes for functions related to the VEX GPS.
 *
 * Visit https://pros.cs.purdue.edu/v5/api/cpp/gps.html to learn
 * more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * Copyright (c) 2017-2021, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_GPS_HPP_
#define _PROS_GPS_HPP_

#include <stdbool.h>

#include <cstdint>

#include "pros/gps.h"

namespace pros {
class Gps {
	const std::uint8_t _port;

	public:
	Gps(const std::uint8_t port) : _port(port){};

	Gps(const std::uint8_t port, double xInitial, double yInitial, double headingInitial) : _port(port) {
		pros::c::gps_set_position(port, xInitial, yInitial, headingInitial);
	};

	Gps(const std::uint8_t port, double xOffset, double yOffset) : _port(port) {
		pros::c::gps_set_offset(port, xOffset, yOffset);
	};

	Gps(const std::uint8_t port, double xInitial, double yInitial, double headingInitial, double xOffset, double yOffset)
	    : _port(port) {
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
	 */
	virtual std::int32_t set_offset(double xOffset, double yOffset) const;

	/**
	 * Get the GPS's location relative to the center of turning/origin in meters.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  xOffset
	 * 				 Pointer to cartesian 4-Quadrant X offset from center of turning (meters)
	 * \param  yOffset
	 * 				 Pointer to cartesian 4-Quadrant Y offset from center of turning (meters)
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t get_offset(double* xOffset, double* yOffset) const;

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
	 */
	virtual pros::c::gps_status_s_t get_status() const;

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
	 */
	virtual pros::c::gps_gyro_s_t get_gyro_rate() const;

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
	virtual pros::c::gps_accel_s_t get_accel() const;

};  // Gps Class

using GPS = Gps;

}  // namespace pros
#endif
