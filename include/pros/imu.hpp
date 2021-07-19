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
 * Copyright (c) 2017-2021, Purdue University ACM SIGBots.
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
	 * Calibration takes approximately 2 seconds, but this function only blocks
 	 * until the IMU status flag is set properly to E_IMU_STATUS_CALIBRATING.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is already calibrating
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t reset() const;
	/**
	* Set the Inertial Sensor's refresh interval in milliseconds.
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
	* ENODEV - The port cannot be configured as an Inertial Sensor
	* EAGAIN - The sensor is still calibrating
	*
	* \param rate The data refresh interval in milliseconds
	* \return 1 if the operation was successful or PROS_ERR if the operation
	* failed, setting errno.
	*/
	virtual std::int32_t set_data_rate(std::uint32_t rate) const;
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
	 * This value is bounded by [0,360). Clockwise rotations are represented with
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
	 * Get the Inertial Sensor's pitch angle bounded by (-180,180)
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
	 * Get the Inertial Sensor's roll angle bounded by (-180,180)
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
	 * Get the Inertial Sensor's yaw angle bounded by (-180,180)
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
	 * Resets the current reading of the Inertial Sensor's rotation to zero
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t tare_rotation() const;
	/**
	 * Resets the current reading of the Inertial Sensor's heading to zero
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t tare_heading() const;
	/**
	 * Resets the current reading of the Inertial Sensor's pitch to zero
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t tare_pitch() const;
	/**
	 * Resets the current reading of the Inertial Sensor's yaw to zero
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t tare_yaw() const;
	/**
	 * Resets the current reading of the Inertial Sensor's roll to zero
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t tare_roll() const;
	/**
	 * Resets all 5 values of the Inertial Sensor to 0.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t tare() const;
	/**
	 * Reset all 3 euler values of the Inertial Sensor to 0.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t tare_euler() const;
	/**
	 * Sets the current reading of the Inertial Sensor's heading to target value
	 * Target will default to 360 if above 360 and default to 0 if below 0.
	 * 
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \param  target
	 * 				 Target value for the heading value to be set to
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t set_heading(const double target) const;
	/**
	 * Sets the current reading of the Inertial Sensor's rotation to target value
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \param  target
	 * 				 Target value for the rotation value to be set to
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t set_rotation(const double target) const;
	/**
	 * Sets the current reading of the Inertial Sensor's yaw to target value
	 * Will default to +/- 180 if target exceeds +/- 180.
	 * 
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \param  target
	 * 				 Target value for yaw value to be set to
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t set_yaw(const double target) const;
	/**
	 * Sets the current reading of the Inertial Sensor's pitch to target value
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \param  target
	 * 				 Target value for the pitch value to be set to
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t set_pitch(const double target) const;
	/**
	 * Sets the current reading of the Inertial Sensor's roll to target value
	 * Will default to +/- 180 if target exceeds +/- 180.
	 * 
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \param  target
	 * 				 Target euler values for the euler values to be set to
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t set_roll(const double target) const;
	/**
	 * Sets the current reading of the Inertial Sensor's euler values to
	 * target euler values. Will default to +/- 180 if target exceeds +/- 180.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is still calibrating
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \param  target
	 * 				 Target euler values for the euler values to be set to
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t set_euler(const pros::c::euler_s_t target) const;
	/**
	 * Get the Inertial Sensor's raw accelerometer values
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

using IMU = Imu;

}  // namespace pros

#endif
