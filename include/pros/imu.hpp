/**
 * \file pros/imu.hpp
 * \ingroup cpp-imu
 *
 * Contains prototypes for functions related to the VEX Inertial sensor.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2024, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \defgroup cpp-imu VEX Inertial Sensor C++ API
 */
#ifndef _PROS_IMU_HPP_
#define _PROS_IMU_HPP_

#include <cstdint>
#include <iostream>

#include "pros/device.hpp"
#include "pros/imu.h"

namespace pros {
/**
 * \ingroup cpp-imu
 * */

/**
 * \addtogroup cpp-imu
 *  @{
 */

/**
 * \enum Imu_Status
 * @brief Indicates IMU status.
 */

enum class ImuStatus {
	ready = 0,
	/** The IMU is calibrating */
	calibrating = 19,
	/** Used to indicate that an error state was reached in the imu_get_status function,\
	not that the IMU is necessarily in an error state */
	error = 0xFF,
};

inline namespace v5 {
/**
 * \ingroup cpp-imu
 */
class Imu : public Device {
	/**
	 * \addtogroup cpp-imu
	 * @{
	 */

	public:
	/**
	 * Creates an Imu object for the given port
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 *
	 * \param port
	 *        The V5 Inertial Sensor port number from 1-21
	 *
	 * \b Example
	 * \code
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Do something with the sensor data
	 *   }
	 * }
	 * \endcode
	 */


	Imu(const std::uint8_t port) : Device(port, DeviceType::imu){};

	Imu(const Device& device) : Imu(device.get_port()){};

	/**
	 * Gets a IMU sensor that is plugged in to the brain
	 *
	 * \note The first time this function is called it returns the IMU sensor at the lowest port
	 * If this function is called multiple times, it will cycle through all the ports.
	 * For example, if you have 1 IMU sensor on the robot
	 * this function will always return a IMU sensor object for that port.
	 * If you have 2 IMU sensors, all the odd numered calls to this function will return objects
	 * for the lower port number,
	 * all the even number calls will return IMU objects for the higher port number
	 *
	 *
	 * This functions uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - No IMU sensor is plugged into the brain
	 *
	 * \return A IMU object corresponding to a port that a IMU sensor is connected to the brain
	 * If no IMU sensor is plugged in, it returns a IMU sensor on port PROS_ERR_BYTE
	 *
	 */
	static Imu get_imu();
	/**
	 * Calibrate IMU
	 *
	 * Calibration takes approximately 2 seconds and blocks during this period if
	 * the blocking param is true, with a timeout for this operation being set a 3
	 * seconds as a safety margin. This function also blocks until the IMU
	 * status flag is set properly to E_IMU_STATUS_CALIBRATING, with a minimum
	 * blocking time of 5ms and a timeout of 1 second if it's never set.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 * EAGAIN - The sensor is already calibrating, or time out setting the status flag.
	 *
	 * \param blocking
	 *			Whether this function blocks during calibration.
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   // Block until calibration is complete
	 *   imu.reset(true);
	 * }
	 * \endcode
	 */
	virtual std::int32_t reset(bool blocking = false) const;
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Set the refresh rate to 5ms
	 *     std::int32_t status = imu.set_data_rate(5);
	 *     delay(20);
	 *
	 *     // Check if the operation was successful
	 *     if (status == PROS_ERR) {
	 *       // Do something with the error
	 *     }
	 *
	 *     // Do something with the sensor data
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t set_data_rate(std::uint32_t rate) const;


	/**
	 * Gets all IMU sensors.
	 * 
	 * \return A vector of Imu sensor objects.
	 *
	 * \b Example
 	 * \code
	 * void opcontrol() {
	 *   std::vector<Imu> imu_all = pros::Imu::get_all_devices();  // All IMU sensors that are connected
	 * }
 	 * \endcode
	 */

	static std::vector<Imu> get_all_devices();

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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *    // Get the total number of degrees the sensor has spun
	 *    printf("Total rotation: %f\n", imu.get_rotation());
	 *    delay(20);
	 *   }
	 * }
	 * \endcode
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Get the sensor's heading
	 *     printf("Heading: %f\n", imu.get_heading());
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Get the sensor's quaternion
	 *     pros::quaternion_s_t quat = imu.get_quaternion();
	 *     cout << "Quaternion: " << quat.w << ", " << quat.x << ", " << quat.y << ", " << quat.z << endl;
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual pros::quaternion_s_t get_quaternion() const;
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Get the sensor's Euler angles
	 *     pros::euler_s_t euler = imu.get_euler();
	 *     cout << "Euler: " << euler.roll << ", " << euler.pitch << ", " << euler.yaw << endl;
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual pros::euler_s_t get_euler() const;
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Get the sensor's pitch
	 *     printf("Pitch: %f\n", imu.get_pitch());
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Get the sensor's roll
	 *     printf("Roll: %f\n", imu.get_roll());
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Get the sensor's yaw
	 *     printf("Yaw: %f\n", imu.get_yaw());
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Get the sensor's raw gyroscope values
	 *     pros::imu_gyro_s_t gyro = imu.get_gyro_rate();
	 *     cout << "Gyro: " << gyro.x << ", " << gyro.y << ", " << gyro.z << endl;
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual pros::imu_gyro_s_t get_gyro_rate() const;
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Set the sensor's rotation value to 10
	 *     imu.set_rotation(10);
	 *     delay(20);
	 *
	 *     // Do something with sensor
	 *
	 *     // Reset the sensor's rotation value to 0
	 *     imu.tare_rotation();
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Set the sensor's heading value to 10
	 *     imu.set_heading(10);
	 *     delay(20);
	 *
	 *     // Do something with sensor
	 *
	 *     // Reset the sensor's heading value to 0
	 *     imu.tare_heading();
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Set the sensor's pitch value to 10
	 *     imu.set_pitch(10);
	 *     delay(20);
	 *
	 *     // Do something with sensor
	 *
	 *     // Reset the sensor's pitch value to 0
	 *     imu.tare_pitch();
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Set the sensor's yaw value to 10
	 *     imu.set_yaw(10);
	 *     delay(20);
	 *
	 *     // Do something with sensor
	 *
	 *     // Reset the sensor's yaw value to 0
	 *     imu.tare_yaw();
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Set the sensor's roll value to 10
	 *     imu.set_roll(10);
	 *     delay(20);
	 *
	 *     // Do something with sensor
	 *
	 *     // Reset the sensor's roll value to 0
	 *     imu.tare_roll();
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Reset all values of the sensor to 0
	 *     imu.tare();
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Reset all euler values of the sensor to 0
	 *     imu.tare_euler();
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Set the sensor's heading value to 10
	 *     imu.set_heading(10);
	 *     delay(20);
	 *
	 *     // Do something with sensor
	 *   }
	 * }
	 * \endcode
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Set the sensor's rotation value to 10
	 *     imu.set_rotation(10);
	 *     delay(20);
	 *
	 *     // Do something with sensor
	 *   }
	 * }
	 * \endcode
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Set the sensor's yaw value to 10
	 *     imu.set_yaw(10);
	 *     delay(20);
	 *
	 *     // Do something with sensor
	 *   }
	 * }
	 * \endcode
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Set the sensor's pitch value to 10
	 *     imu.set_pitch(10);
	 *     delay(20);
	 *
	 *     // Do something with sensor
	 *   }
	 * }
	 * \endcode
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Set the sensor's roll value to 100
	 *     imu.set_roll(100);
	 *     delay(20);
	 *
	 *     // Do something with sensor
	 *   }
	 * }
	 * \endcode
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Set the sensor's euler values to 50
	 *     imu.set_euler(50);
	 *     delay(20);
	 *
	 *     // Do something with sensor
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t set_euler(const pros::euler_s_t target) const;
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Get the sensor's raw accelerometer values
	 *     pros::imu_accel_s_t accel = imu.get_accel();
	 *     printf("x: %f, y: %f, z: %f\n", accel.x, accel.y, accel.z);
	 *     delay(20);
	 *
	 *     // Do something with sensor
	 *   }
	 * }
	 * \endcode
	 */
	virtual pros::imu_accel_s_t get_accel() const;
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
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Get the sensor's status
	 *     pros::ImuStatus status = imu.get_status();
	 *     cout << "Status: " << status << endl;
	 *     delay(20);
	 *
	 *     // Do something with sensor
	 *   }
	 * }
	 * \endcode
	 */
	virtual pros::ImuStatus get_status() const;
	/**
	 * Check whether the IMU is calibrating
	 *
	 * \return true if the V5 Inertial Sensor is calibrating or false
	 * false if it is not.
	 *
	 * \b Example
	 * \code
	 *
	 * #define IMU_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::Imu imu(IMU_PORT);
	 *
	 *   while (true) {
	 *     // Calibrate the sensor
	 *     imu.reset();
	 *     delay(20);
	 *
	 *     // Check if the sensor is calibrating
	 *     if (imu.is_calibrating()) {
	 *       printf("Calibrating...\n");
	 *     }
	 *
	 *     // Do something with sensor
	 *   }
	 * }
	 * \endcode
	 */
	virtual bool is_calibrating() const;
	/**
	 * Returns the physical orientation of the IMU
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Inertial Sensor
	 *
	 * \param  port
	 * 				 The V5 Inertial Sensor port number from 1-21
	 * \returns The physical orientation of the Inertial Sensor or PROS_ERR if an error occured.
	 *
	 */
	virtual imu_orientation_e_t get_physical_orientation() const;

	/**
	 * This is the overload for the << operator for printing to streams
	 *
	 * Prints in format(this below is all in one line with no new line):
	 * Imu [port: imu._port, rotation: (rotation), heading: (heading),
	 * pitch: (pitch angle), roll: (roll angle), yaw: (yaw angle),
	 * gyro rate: {x,y,z}, get accel: {x,y,z}, calibrating: (calibrating boolean)]
	 */
	friend std::ostream& operator<<(std::ostream& os, const pros::Imu& imu);

	///@}
};

namespace literals {
/**
 * Constructs a Imu from a literal ending in _imu via calling the constructor
 *
 * \return a pros::Imu for the corresponding port
 *
 * \b Example
 * \code
 * using namespace pros::literals;
 * void opcontrol() {
 *	pros::Imu imu = 2_imu; //Makes an IMU object on port 2
 * }
 * \endcode
 */
const pros::Imu operator"" _imu(const unsigned long long int i);
}  // namespace literals

using IMU = Imu;
}  // namespace v5
}  // namespace pros

#endif
