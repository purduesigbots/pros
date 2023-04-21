/**
 * \file pros/imu.h
 * \ingroup c-imu
 *
 * Contains prototypes for functions related to the VEX Inertial sensor.
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
 * \defgroup c-imu VEX Inertial Sensor C API
 */

#ifndef _PROS_IMU_H_
#define _PROS_IMU_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
namespace pros {
#endif

/**
 * \ingroup c-imu
 * */

/**
 * \addtogroup c-imu
 *  @{
 */

/**
 * \enum imu_status_e_t
 * @brief Indicates IMU status.
 */
typedef enum imu_status_e {
	/** The IMU is calibrating */
	E_IMU_STATUS_CALIBRATING = 0x01,
	/** Used to indicate that an error state was reached in the imu_get_status function,\
	not that the IMU is necessarily in an error state */
	E_IMU_STATUS_ERROR = 0xFF,
} imu_status_e_t;

/**
 * \struct quaternion_s_t
 */
typedef struct __attribute__((__packed__)) quaternion_s {
	double x;
	double y;
	double z;
	double w;
} quaternion_s_t;

/**
 * \struct imu_raw_s
 *
 */
struct imu_raw_s {
	double x;
	double y;
	double z;
};

/**
 * \struct imu_gyro_s_t
 *
 */
typedef struct imu_raw_s imu_gyro_s_t;

/**
 * \struct imu_accel_s_t
 *
 */
typedef struct imu_raw_s imu_accel_s_t;

/**
 * \struct euler_s_t
 *
 */
typedef struct __attribute__((__packed__)) euler_s {
	double pitch;
	double roll;
	double yaw;
} euler_s_t;

#ifdef __cplusplus
namespace c {
#endif
/**
 * \def IMU_MINIMUM_DATA_RATE
 */
#ifdef PROS_USE_SIMPLE_NAMES
#ifdef __cplusplus
#define IMU_STATUS_CALIBRATING pros::E_IMU_STATUS_CALIBRATING
#define IMU_STATUS_ERROR pros::E_IMU_STATUS_ERROR
#else
#define IMU_STATUS_CALIBRATING E_IMU_STATUS_CALIBRATING
#define IMU_STATUS_ERROR E_IMU_STATUS_ERROR
#endif
#endif

#define IMU_MINIMUM_DATA_RATE 5

/**
 * Calibrate IMU
 *
 * Calibration takes approximately 2 seconds, but this function only blocks
 * until the IMU status flag is set properly to E_IMU_STATUS_CALIBRATING,
 * with a minimum blocking time of 5ms.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Inertial Sensor
 * EAGAIN - The sensor is already calibrating, or time out setting the status flag.
 *
 * \param port
 *        The V5 Inertial Sensor port number from 1-21
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed setting errno.
 *
 * \b Example
 * \code
 * #define IMU_PORT 1
 *
 * void initialize() {
 *   imu_reset(IMU_PORT);
 *   int time = millis();
 *   int iter = 0;
 *   while (imu_get_status(IMU_PORT) & E_IMU_STATUS_CALIBRATING) {
 *     printf("IMU calibrating... %d\n", iter);
 *     iter += 10;
 *     delay(10);
 *   }
 *   // should print about 2000 ms
 *   printf("IMU is done calibrating (took %d ms)\n", iter - time);
 * }
 * \endcode
 */
int32_t imu_reset(uint8_t port);

/**
 * Calibrate IMU and Blocks while Calibrating
 *
 * Calibration takes approximately 2 seconds and blocks during this period,
 * with a timeout for this operation being set a 3 seconds as a safety margin.
 * Like the other reset function, this function also blocks until the IMU
 * status flag is set properly to E_IMU_STATUS_CALIBRATING, with a minimum
 * blocking time of 5ms and a timeout of 1 second if it's never set.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Inertial Sensor
 * EAGAIN - The sensor is already calibrating, or time out setting the status flag.
 *
 * \param port
 *        The V5 Inertial Sensor port number from 1-21
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed (timing out or port claim failure), setting errno.
 */
int32_t imu_reset_blocking(uint8_t port);
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
 * \param port
 *		  The V5 Inertial Sensor port number from 1-21
 * \param rate The data refresh interval in milliseconds
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 *
 * \b Example
 * \code
 *
 * \endcode
 */
int32_t imu_set_data_rate(uint8_t port, uint32_t rate);

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
 * #define IMU_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     printf("IMU get rotation: %f degrees\n", imu_get_rotation(IMU_PORT));
 *     delay(20);
 *   }
 * }
 * \endcode
 */
double imu_get_rotation(uint8_t port);

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
 * #define IMU_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     printf("IMU get heading: %f degrees\n", imu_get_heading(IMU_PORT));
 *     delay(20);
 *   }
 * }
 * \endcode
 */
double imu_get_heading(uint8_t port);

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
 * #define IMU_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     quaternion_s_t qt = imu_get_quaternion(IMU_PORT);
 *     printf("IMU quaternion: {x: %f, y: %f, z: %f, w: %f}\n", qt.x, qt.y, qt.z, qt.w);
 *     delay(20);
 *   }
 * }
 * \endcode
 */
quaternion_s_t imu_get_quaternion(uint8_t port);

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
 * #define IMU_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     euler_s_t eu = imu_get_euler(IMU_PORT);
 *     printf("IMU euler angles: {pitch: %f, roll: %f, yaw: %f}\n", eu.pitch, eu.roll, eu.yaw);
 *     delay(20);
 *   }
 * }
 * \endcode
 */
euler_s_t imu_get_euler(uint8_t port);

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
 * \return The pitch angle, or PROS_ERR_F if the operation failed, setting
 * errno.
 *
 * \b Example
 * \code
 * #define IMU_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     printf("IMU pitch: %f\n", imu_get_pitch(IMU_PORT));
 *     delay(20);
 *   }
 * }
 * \endcode
 */
imu_gyro_s_t imu_get_gyro_rate(uint8_t port);

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
 * \return The roll angle, or PROS_ERR_F if the operation failed, setting errno.
 *
 * \b Example
 * \code
 * #define IMU_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     printf("IMU roll: %f\n", imu_get_roll(IMU_PORT));
 *     delay(20);
 *   }
 * }
 * \endcode
 */
imu_accel_s_t imu_get_accel(uint8_t port);

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
 * \return The yaw angle, or PROS_ERR_F if the operation failed, setting errno.
 *
 * \b Example
 * \code
 * #define IMU_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     printf("IMU yaw: %f\n", imu_get_yaw(IMU_PORT));
 *     delay(20);
 *   }
 * }
 * \endcode
 */
imu_status_e_t imu_get_status(uint8_t port);

// Value set functions:
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
 * \return The raw gyroscope values. If the operation failed, all the
 * structure's members are filled with PROS_ERR_F and errno is set.
 *
 * \b Example
 * \code
 * #define IMU_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     int32_t val = imu_set_euler(IMU_PORT, {45, 60, 90});
 *     printf("IMU : {gyro vals: %d}\n", val);
 *     delay(20);
 *   }
 * }
 * \endcode
 */
int32_t imu_set_euler(uint8_t port, euler_s_t target);

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
 * \return The raw accelerometer values. If the operation failed, all the
 * structure's members are filled with PROS_ERR_F and errno is set.
 *
 * \b Example
 * \code
 * #define IMU_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     imu_accel_s_t accel = imu_get_accel(IMU_PORT);
 *     printf("IMU accel values: {x: %f, y: %f, z: %f}\n", accel.x, accel.y, accel.z);
 *     delay(20);
 *   }
 * }
 * \endcode
 */
double imu_get_pitch(uint8_t port);

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
 * \return The Inertial Sensor's status code, or PROS_ERR if the operation
 * failed, setting errno.
 *
 * \b Example
 * \code
 * #define IMU_PORT 1
 *
 * void initialize() {
 *   imu_reset(IMU_PORT);
 *   int time = millis();
 *   int iter = 0;
 *   while (imu_get_status(IMU_PORT) & E_IMU_STATUS_CALIBRATING) {
 *     printf("IMU calibrating... %d\n", iter);
 *     iter += 10;
 *     delay(10);
 *   }
 *   // should print about 2000 ms
 *   printf("IMU is done calibrating (took %d ms)\n", iter - time);
 * }
 * \endcode
 */
double imu_get_roll(uint8_t port);

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
double imu_get_yaw(uint8_t port);

// NOTE: not used
// void imu_set_mode(uint8_t port, uint32_t mode);
// uint32_t imu_get_mode(uint8_t port);

/**
 * \name Value Reset Functions
 * @{
 */

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
 * #define IMU_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     if(controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_X)){
 *       imu_tare_heading(IMU_PORT);
 *     }
 *   pros::delay(20);
 *   }
 * }
 * \endcode
 */
int32_t imu_tare_heading(uint8_t port);

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
 * #define IMU_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     if(controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_X)){
 *       imu_tare_rotation(IMU_PORT);
 *     }
 *   pros::delay(20);
 *   }
 * }
 * \endcode
 */
int32_t imu_tare_rotation(uint8_t port);

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
 * #define IMU_PORT 1void opcontrol() {
 *   while (true) {
 *     if(controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_X)){
 *       imu_tare_pitch(IMU_PORT);
 *     }
 *   pros::delay(20);
 *   }
 * }
 * \endcode
 */
int32_t imu_tare_pitch(uint8_t port);

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
 * #define IMU_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     if(controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_X)){
 *       imu_tare_roll(IMU_PORT);
 *     }
 *   pros::delay(20);
 *   }
 * }
 * \endcode
 */
int32_t imu_tare_roll(uint8_t port);

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
 * #define IMU_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     if(controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_X)){
 *       imu_tare_yaw(IMU_PORT);
 *     }
 *   pros::delay(20);
 *   }
 * }
 * \endcode
 */
int32_t imu_tare_yaw(uint8_t port);

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
 * #define IMU_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     if(controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_X)){
 *       imu_tare_euler(IMU_PORT);
 *     }
 *   pros::delay(20);
 *   }
 * }
 * \endcode
 */
int32_t imu_tare_euler(uint8_t port);

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
 * #define IMU_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     if(controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_X)){
 *       imu_tare(IMU_PORT);
 *     }
 *   pros::delay(20);
 *   }
 * }
 * \endcode
 */
int32_t imu_tare(uint8_t port);

/** @} */

/**
 * \name Value Set Functions
 * @{
 */

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
 * #define IMU_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     if(controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_X)){
 *       imu_set_euler(IMU_PORT, {45,45,45});
 *     }
 *     pros::delay(20);
 *   }
 * }
 * \endcode
 */
int32_t imu_set_euler(uint8_t port, euler_s_t target);

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
 * #define IMU_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     if(controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_X)){
 *       imu_set_rotation(IMU_PORT, 45);
 *     }
 *     pros::delay(20);
 *   }
 * }
 * \endcode
 */
int32_t imu_set_rotation(uint8_t port, double target);

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
 * #define IMU_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     if(controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_X)){
 *       imu_set_heading(IMU_PORT, 45);
 *     }
 *     pros::delay(20);
 *   }
 * }
 * \endcode
 */
int32_t imu_set_heading(uint8_t port, double target);

/**
 * Sets the current reading of the Inertial Sensor's pitch to target value
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
 * 				 Target value for the pitch value to be set to
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 *
 * \b Example
 * \code
 * #define IMU_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     if(controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_X)){
 *       imu_set_pitch(IMU_PORT, 45);
 *     }
 *     pros::delay(20);
 *   }
 * }
 * \endcode
 */
int32_t imu_set_pitch(uint8_t port, double target);

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
 * 				 Target value for the roll value to be set to
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 *
 * \b Example
 * \code
 * #define IMU_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     if(controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_X)){
 *       imu_set_roll(IMU_PORT, 45);
 *     }
 *     pros::delay(20);
 *   }
 * }
 * \endcode
 */
int32_t imu_set_roll(uint8_t port, double target);

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
 * 				 Target value for the yaw value to be set to
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 *
 * \b Example
 * \code
 * #define IMU_PORT 1void opcontrol() {
 *
 * while (true) {
 *   if(controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_X)){
 *     imu_set_yaw(IMU_PORT, 45);
 *   }
 *   pros::delay(20);
 *   }
 * }
 * \endcode
 */
int32_t imu_set_yaw(uint8_t port, double target);

/** @} */

/** @} */

#ifdef __cplusplus
}
}
}
#endif

#endif
