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

#include "pros/device.hpp"
#include "pros/gps.h"

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
	Gps(const std::uint8_t port) : Device(port, DeviceType::gps){};

	Gps(const Device& device) : Gps(device.get_port()){};

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
	explicit Gps(const std::uint8_t port, double xInitial, double yInitial, double headingInitial)
	    : Device(port, DeviceType::gps) {
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
	explicit Gps(const std::uint8_t port, double xOffset, double yOffset) : Device(port, DeviceType::gps) {
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
	explicit Gps(const std::uint8_t port, double xInitial, double yInitial, double headingInitial, double xOffset,
	             double yOffset)
	    : Device(port, DeviceType::gps) {
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

	 * Gets all GPS sensors.
	 *
	 * \return A vector of Gps sensor objects.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   std::vector<Gps> gps_all = pros::Gps::get_all_devices();  // All GPS sensors that are connected
	 * }
	 * \endcode
	 */
	static std::vector<Gps> get_all_devices();

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
	 *     status = gps.get_position_and_orientation();
	 *     printf("X: %f, Y: %f, Roll: %f, Pitch: %f, Yaw: %f\n",
	 * 	   status.x, status.y, status.roll, status.pitch, status.yaw);
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual pros::gps_status_s_t get_position_and_orientation() const;

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
	 *     printf("X: %f, Y: %f\n", position.x, position.y);
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual pros::gps_position_s_t get_position() const;

	/**
	 * Gets the X position in meters of the robot relative to the starting position.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \return The X position in meters. If the operation failed,
	 * returns PROS_ERR_F and errno is set.
	 *
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  while(true) {
	 *		double pos_x = gps.get_position_x();
	 *		printf("X: %f\n", pos_x);
	 *  	pros::delay(20);
	 *  }
	 * }
	 * \endcode
	 */
	virtual double get_position_x() const;

	/**
	 * Gets the Y position in meters of the robot relative to the starting position.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \return The Y position in meters. If the operation failed,
	 * returns PROS_ERR_F and errno is set.
	 *
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  while(true) {
	 *		double pos_y = gps.get_position_y();
	 *		printf("Y: %f\n", pos_y);
	 *  	pros::delay(20);
	 *  }
	 * }
	 * \endcode
	 */
	virtual double get_position_y() const;

	/**
	 * Gets the pitch, roll, and yaw of the GPS relative to the starting orientation.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \return A struct (gps_orientation_s_t) containing values mentioned above.
	 * If the operation failed, all the structure's members are filled with
	 * PROS_ERR_F and errno is set.
	 *
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  gps_orientation_s_t orientation;
	 *   while (true) {
	 *	 orientation = gps.get_orientation();
	 *     printf("pitch: %f, roll: %f, yaw: %f\n", orientation.pitch,
	 *         orientation.roll, orientation.yaw);
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual pros::gps_orientation_s_t get_orientation() const;

	/**
	 * Gets the pitch of the robot in degrees relative to the starting oreintation.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \return The pitch in [0,360) degree values. If the operation failed,
	 * returns PROS_ERR_F and errno is set.
	 *
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  while(true) {
	 *		double pitch = gps.get_pitch();
	 *		printf("pitch: %f\n", pitch);
	 *  	pros::delay(20);
	 *  }
	 * }
	 * \endcode
	 */
	virtual double get_pitch() const;

	/**
	 * Gets the roll of the robot in degrees relative to the starting oreintation.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \return The roll in [0,360) degree values. If the operation failed,
	 * returns PROS_ERR_F and errno is set.
	 *
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  while(true) {
	 *		double roll = gps.get_roll();
	 *		printf("roll: %f\n", roll);
	 *  	pros::delay(20);
	 *  }
	 * }
	 * \endcode
	 */
	virtual double get_roll() const;

	/**
	 * Gets the yaw of the robot in degrees relative to the starting oreintation.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \return The yaw in [0,360) degree values. If the operation failed,
	 * returns PROS_ERR_F and errno is set.
	 *
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  while(true) {
	 *		double yaw = gps.get_yaw();
	 *		printf("yaw: %f\n", yaw);
	 *  	pros::delay(20);
	 *  }
	 * }
	 * \endcode
	 */
	virtual double get_yaw() const;

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
	 * Get the GPS's raw gyroscope value in z-axis
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \return The raw gyroscope value in z-axis. If the operation fails, returns
	 * PROS_ERR_F and errno is set.
	 *
	 \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  while(true) {
	 *		double gyro_z = gps.get_gyro_z();
	 *		printf("gyro_z: %f\n", gyro_z);
	 *  	pros::delay(20);
	 *  }
	 * }
	 * \endcode
	 */
	virtual pros::gps_gyro_s_t get_gyro_rate() const;

	/**
	 * Get the GPS's raw gyroscope value in x-axis
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \return The raw gyroscope value in x-axis. If the operation fails, returns
	 * PROS_ERR_F and errno is set.
	 *
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  while(true) {
	 *		double gyro_x = gps.get_gyro_x();
	 *		printf("gyro_x: %f\n", gyro_x);
	 *  	pros::delay(20);
	 *  }
	 * }
	 * \endcode
	 */
	virtual double get_gyro_rate_x() const;

	/**
	 * Get the GPS's raw gyroscope value in y-axis
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \return The raw gyroscope value in y-axis. If the operation fails, returns
	 * PROS_ERR_F and errno is set.
	 *
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  while(true) {
	 *		double gyro_y = gps.get_gyro_y();
	 *		printf("gyro_y: %f\n", gyro_y);
	 *  	pros::delay(20);
	 *  }
	 * }
	 * \endcode
	 */
	virtual double get_gyro_rate_y() const;

	/**
	 * Get the GPS's raw gyroscope value in z-axis
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \return The raw gyroscope value in z-axis. If the operation fails, returns
	 * PROS_ERR_F and errno is set.
	 *
	 \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  while(true) {
	 *		double gyro_z = gps.get_gyro_z();
	 *		printf("gyro_z: %f\n", gyro_z);
	 *  	pros::delay(20);
	 *  }
	 * }
	 * \endcode
	*/
	virtual double get_gyro_rate_z() const;

	/**
	 * Get the GPS's raw accelerometer values
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \return The raw accelerometer values. If the operation failed, all the
	 * structure's members are filled with PROS_ERR_F and errno is set.
	 */
	virtual pros::gps_accel_s_t get_accel() const;

	/**
	 * Get the GPS's raw accelerometer value in x-axis
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \return The raw accelerometer value in x-axis. If the operation fails, returns
	 * PROS_ERR_F and errno is set.
	 *
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  while(true) {
	 *		double accel_x = gps.get_accel_x();
	 *		printf("accel_x: %f\n", accel_x);
	 *  	pros::delay(20);
	 *  }
	 * }
	 * \endcode
	 */
	virtual double get_accel_x() const;

	/**
	 * Get the GPS's raw accelerometer value in y-axis
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \return The raw accelerometer value in y-axis. If the operation fails, returns
	 * PROS_ERR_F and errno is set.
	 *
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  while(true) {
	 *		double accel_y = gps.get_accel_y();
	 *		printf("accel_y: %f\n", accel_y);
	 *  	pros::delay(20);
	 *  }
	 * }
	 * \endcode
	 */
	virtual double get_accel_y() const;

	/**
	 * Get the GPS's raw accelerometer value in z-axis
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an GPS
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \return The raw accelerometer value in z-axis. If the operation fails, returns
	 * PROS_ERR_F and errno is set.
	 *
	 * \b Example
	 * \code
	 * #define GPS_PORT 1
	 *
	 * void opcontrol() {
	 * 	Gps gps(GPS_PORT);
	 *  while(true) {
	 *		double accel_z = gps.get_accel_z();
	 *		printf("accel_z: %f\n", accel_z);
	 *  	pros::delay(20);
	 *  }
	 * }
	 * \endcode
	 */
	virtual double get_accel_z() const;

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

	/**
	 * Gets a gps sensor that is plugged in to the brain
	 *
	 * \note The first time this function is called it returns the gps sensor at the lowest port
	 * If this function is called multiple times, it will cycle through all the ports.
	 * For example, if you have 1 gps sensor on the robot
	 * this function will always return a gps sensor object for that port.
	 * If you have 2 gps sensors, all the odd numered calls to this function will return objects
	 * for the lower port number,
	 * all the even number calls will return gps objects for the higher port number
	 *
	 *
	 * This functions uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - No gps sensor is plugged into the brain
	 *
	 * \return A gps object corresponding to a port that a gps sensor is connected to the brain
	 * If no gps sensor is plugged in, it returns a gps sensor on port PROS_ERR_BYTE
	 *
	 */
	static Gps get_gps();
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

}  // namespace v5
}  // namespace pros

#endif
