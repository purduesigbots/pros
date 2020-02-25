/**
 * \file pros/imu.h
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

#ifndef _PROS_IMU_H_
#define _PROS_IMU_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
namespace pros {
namespace c {
#endif

typedef enum imu_status_e {
	E_IMU_STATUS_CALIBRATING = 0x01,
	E_IMU_STATUS_ERROR = 0xFF,  // NOTE: used for returning an error from the get_status function, not that the IMU is
	                            // necessarily in an error state
} imu_status_e_t;

typedef struct __attribute__((__packed__)) quaternion_s {
	double x;
	double y;
	double z;
	double w;
} quaternion_s_t;

struct imu_raw_s {
	double x;
	double y;
	double z;
};

typedef struct imu_raw_s imu_gyro_s_t;
typedef struct imu_raw_s imu_accel_s_t;

typedef struct __attribute__((__packed__)) euler_s {
	double pitch;
	double roll;
	double yaw;
} euler_s_t;

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
int32_t imu_reset(uint8_t port);

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
double imu_get_rotation(uint8_t port);

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
 */
euler_s_t imu_get_euler(uint8_t port);

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
double imu_get_pitch(uint8_t port);

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
double imu_get_roll(uint8_t port);

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
double imu_get_yaw(uint8_t port);

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
 * \return The raw accelerometer values. If the operation failed, all the
 * structure's members are filled with PROS_ERR_F and errno is set.
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
 * \return The Inertial Sensor's status code, or PROS_ERR if the operation
 * failed, setting errno.
 */
imu_status_e_t imu_get_status(uint8_t port);

// NOTE: not used
// void imu_set_mode(uint8_t port, uint32_t mode);
// uint32_t imu_get_mode(uint8_t port);

#ifdef __cplusplus
}
}
}
#endif

#endif
