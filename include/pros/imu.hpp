/**
 * \file pros/imu.hpp
 *
 * Contains prototypes for functions related to the VEX Inertial sensor.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/imu.html to learn
 * more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * Copyright (c) 2017-2020, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _PROS_IMU_HPP_
#define _PROS_IMU_HPP_

#include <cstdint>
#include "pros/imu.h"

namespace pros {
class Imu {
	const std::uint8_t _port;

	public:
	Imu(const std::uint8_t port) : _port(port){};

	/**
	 * Calibrate IMU
	 *
	 * This takes approximately 2 seconds, and is a non-blocking operation.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is already calibrating
	 *
	 * \param port
	 *        The V5 Inertial Sensor port number from 1-21
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t reset() const;
	/**
	 * Get the total number of degrees the Inertial Sensor has spun about the z-axis
	 *
	 * This value is theoretically unbounded. Clockwise rotations are represented
	 * with positive degree values, while counterclockwise rotations are represented
	 * with negative ones.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \return The degree value or PROS_ERR_F if the operation failed, setting
	 * errno.
	 */
	virtual double get_rotation() const;
	/**
	 * Get the Inertial Sensor's heading relative to the initial direction of its
	 * x-axis
	 *
	 * This value is bounded by (-360,360). Clockwise rotations are represented with
	 * positive degree values, while counterclockwise rotations are represented with
	 * negative ones.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \return The degree value or PROS_ERR_F if the operation failed, setting
	 * errno.
	 */
	virtual double get_heading() const;
	/**
	 * Get a quaternion representing the Inertial Sensor's orientation
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \return The quaternion representing the sensor's orientation. If the
	 * operation failed, all the quaternion's members are filled with PROS_ERR_F and
	 * errno is set.
	 */
	virtual pros::c::quaternion_s_t get_quaternion() const;
	/**
	 * Get the Euler angles representing the Inertial Sensor's orientation
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \return The Euler angles representing the sensor's orientation. If the
	 * operation failed, all the structure's members are filled with PROS_ERR_F and
	 * errno is set.
	 */
	virtual pros::c::euler_s_t get_euler() const;
	/**
	 * Get the Inertial Sensor's pitch angle
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \return The pitch angle, or PROS_ERR_F if the operation failed, setting
	 * errno.
	 */
	virtual double get_pitch() const;
	/**
	 * Get the Inertial Sensor's roll angle
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \return The roll angle, or PROS_ERR_F if the operation failed, setting errno.
	 */
	virtual double get_roll() const;
	/**
	 * Get the Inertial Sensor's yaw angle
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \return The yaw angle, or PROS_ERR_F if the operation failed, setting errno.
	 */
	virtual double get_yaw() const;
	/**
	 * Get the Inertial Sensor's raw gyroscope values
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \return The raw gyroscope values. If the operation failed, all the
	 * structure's members are filled with PROS_ERR_F and errno is set.
	 */
	virtual pros::c::imu_gyro_s_t get_gyro_rate() const;
	/**
	 * Get the Inertial Sensor's raw acceleroneter values
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \return The raw accelerometer values. If the operation failed, all the
	 * structure's members are filled with PROS_ERR_F and errno is set.
	 */
	virtual pros::c::imu_accel_s_t get_accel() const;
	/**
	 * Get the Inertial Sensor's status
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \return The Inertial Sensor's status code, or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual pros::c::imu_status_e_t get_status() const;
	/**
	 * Check whether the IMU is calibrating
	 *
	 * \return true if the V5 Inertial Sensor is calibrating or false
	 * false if it is not.
	 */
	virtual bool is_calibrating() const;
};
}  // namespace pros

#endif
