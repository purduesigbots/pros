/**
 * \file pros/imubl.h
 *
 * IMU Backend Layer header for non-threadsafe functions for reset
 * functions that interact with the registry.
 *
 * Copyright (c) 2017-2020, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_IMUBL_H_
#define _PROS_IMUBL_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
namespace pros {
namespace c {
#endif

/**
 * Get the Euler angles representing the Inertial Sensor's orientation,
 * with this version being unthread safe.
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
euler_s_t imu_get_euler_bl(uint8_t port);

/**
 * Get registry stored offset between the true and corrected value
 * of the heading value.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Inertial Sensor
 * EAGAIN - The sensor is still calibrating
 *
 * \param  port
 * 				 The V5 Inertial Sensor port number from 1-21
 * \return The offset value or PROS_ERR_F if the operation failed, setting
 * errno.
 */
double imu_get_heading_offset(uint8_t port);

/**
 * Get registry stored offset between the true and corrected value
 * of the rotation value.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Inertial Sensor
 * EAGAIN - The sensor is still calibrating
 *
 * \param  port
 * 				 The V5 Inertial Sensor port number from 1-21
 * \return The offset value or PROS_ERR_F if the operation failed, setting
 * errno.
 */
double imu_get_rotation_offset(uint8_t port);

/**
 * Get registry stored offset between the true and corrected value
 * of the pitch value.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Inertial Sensor
 * EAGAIN - The sensor is still calibrating
 *
 * \param  port
 * 				 The V5 Inertial Sensor port number from 1-21
 * \return The offset value or PROS_ERR_F if the operation failed, setting
 * errno.
 */
double imu_get_pitch_offset(uint8_t port);

/**
 * Get registry stored offset between the true and corrected value
 * of the yaw value.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Inertial Sensor
 * EAGAIN - The sensor is still calibrating
 *
 * \param  port
 * 				 The V5 Inertial Sensor port number from 1-21
 * \return The offset value or PROS_ERR_F if the operation failed, setting
 * errno.
 */
double imu_get_yaw_offset(uint8_t psort);

/**
 * Get registry stored offset between the true and corrected value
 * of the roll value.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Inertial Sensor
 * EAGAIN - The sensor is still calibrating
 *
 * \param  port
 * 				 The V5 Inertial Sensor port number from 1-21
 * \return The offset value or PROS_ERR_F if the operation failed, setting
 * errno.
 */
double imu_get_roll_offset(uint8_t port);

/**
 * Set registry stored offset between the true and corrected value
 * of the heading value.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Inertial Sensor
 * EAGAIN - The sensor is still calibrating
 *
 * \param  port
 * 				 The V5 Inertial Sensor port number from 1-21
 */
void imu_set_heading_offset(uint8_t port, double target);

/**
 * Set registry stored offset between the true and corrected value
 * of the rotation value.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Inertial Sensor
 * EAGAIN - The sensor is still calibrating
 *
 * \param  port
 * 				 The V5 Inertial Sensor port number from 1-21
 */
void imu_set_rotation_offset(uint8_t port, double target);

/**
 * Set registry stored offset between the true and corrected value
 * of the pitch value.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Inertial Sensor
 * EAGAIN - The sensor is still calibrating
 *
 * \param  port
 * 				 The V5 Inertial Sensor port number from 1-21
 */
void imu_set_pitch_offset(uint8_t port, double target);

/**
 * Set registry stored offset between the true and corrected value
 * of the yaw value.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Inertial Sensor
 * EAGAIN - The sensor is still calibrating
 *
 * \param  port
 * 				 The V5 Inertial Sensor port number from 1-21
 */
void imu_set_yaw_offset(uint8_t port, double target);

/**
 * Set registry stored offset between the true and corrected value
 * of the roll value.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Inertial Sensor
 * EAGAIN - The sensor is still calibrating
 *
 * \param  port
 * 				 The V5 Inertial Sensor port number from 1-21
 */
void imu_set_roll_offset(uint8_t port, double target);

#ifdef __cplusplus
}
}
}
#endif

#endif
