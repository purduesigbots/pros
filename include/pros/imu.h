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

#define IMU_MINIMUM_DATA_RATE 5

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
int32_t imu_calibrate(uint8_t port);


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

//reset functions: (TODO: Finish Documentation)
int32_t imu_reset_heading(uint8_t port);
int32_t imu_reset_rotation(uint8_t port);

//Offset Getter Functions (TODO: Finish Documentation)
double imu_get_heading_offset(uint8_t port);
double imu_get_rotation_offset(uint8_t port);
double imu_get_pitch_offset(uint8_t port);
double imu_get_yaw_offset(uint8_t port);
double imu_get_roll_offset(uint8_t port);

//Setter Functions
void imu_set_heading_offset(uint8_t port, double current);
void imu_set_rotation_offset(uint8_t port, double current);
void imu_set_pitch_offset(uint8_t port, double current);
void imu_set_yaw_offset(uint8_t port, double current);
void imu_set_roll_offset(uint8_t port, double current);

#ifdef __cplusplus
}
}
}
#endif

#endif
