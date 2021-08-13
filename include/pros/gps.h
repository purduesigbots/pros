/**
 * \file pros/gps.h
 *
 * Contains prototypes for functions related to the VEX GPS.
 *
 * Visit https://pros.cs.purdue.edu/v5/api/c/gps.html to learn
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

#ifndef _PROS_GPS_H_
#define _PROS_GPS_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
namespace pros {
namespace c {
#endif

typedef struct __attribute__((__packed__)) gps_status_s {
	double x;      ///< X Position (meters)
	double y;      ///< Y Position (meters)
	double pitch;  ///< Percieved Pitch based on GPS + IMU
	double roll;   ///< Percieved Roll based on GPS + IMU
	double yaw;    ///< Percieved Yaw based on GPS + IMU
} gps_status_s_t;

struct gps_raw_s {
	double x;  ///< Percieved Pitch based on GPS + IMU
	double y;  ///< Percieved Roll based on GPS + IMU
	double z;  ///< Percieved Yaw based on GPS + IMU
};

typedef struct gps_raw_s gps_accel_s_t;
typedef struct gps_raw_s gps_gyro_s_t;

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
 * \param  port
 * 				 The V5 GPS port number from 1-21
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
int32_t gps_initialize_full(uint8_t port, double xInitial, double yInitial, double headingInitial, double xOffset,
                            double yOffset);

/**
 * Set the GPS's offset relative to the center of turning in meters.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a GPS
 * EAGAIN - The sensor is still calibrating
 *
 * \param  port
 * 				 The V5 GPS port number from 1-21
 * \param  xOffset
 * 				 Cartesian 4-Quadrant X offset from center of turning (meters)
 * \param  yOffset
 * 				 Cartesian 4-Quadrant Y offset from center of turning (meters)
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t gps_set_offset(uint8_t port, double xOffset, double yOffset);

/**
 * Get the GPS's location relative to the center of turning/origin in meters.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a GPS
 * EAGAIN - The sensor is still calibrating
 *
 * \param  port
 * 				 The V5 GPS port number from 1-21
 * \param  xOffset
 * 				 Pointer to cartesian 4-Quadrant X offset from center of turning (meters)
 * \param  yOffset
 * 				 Pointer to cartesian 4-Quadrant Y offset from center of turning (meters)
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t gps_get_offset(uint8_t port, double* xOffset, double* yOffset);

/**
 * Sets the robot's location relative to the center of the field in meters.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a GPS
 * EAGAIN - The sensor is still calibrating
 *
 * \param  port
 * 				 The V5 GPS port number from 1-21
 * \param  xInitial
 * 				 Initial 4-Quadrant X Position, with (0,0) being at the center of the field (meters)
 * \param  yInitial
 * 				 Initial 4-Quadrant Y Position, with (0,0) being at the center of the field (meters)
 * \param  headingInitial
 *  			 Heading with 0 being north on the field, in degrees [0,360) going clockwise
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t gps_set_position(uint8_t port, double xInitial, double yInitial, double headingInitial);

/**
 * Set the GPS sensor's data rate in milliseconds, only applies to IMU on GPS.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a GPS
 * EAGAIN - The sensor is still calibrating
 *
 * \param  port
 * 				 The V5 GPS port number from 1-21
 * \param  rate
 * 				 Data rate in milliseconds (Minimum: 5 ms)
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t gps_set_data_rate(uint8_t port, uint32_t rate);

/**
 * Get the possible RMS (Root Mean Squared) error in meters for GPS position.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a GPS
 * EAGAIN - The sensor is still calibrating
 *
 * \param  port
 * 				 The V5 GPS port number from 1-21
 *
 * \return Possible RMS (Root Mean Squared) error in meters for GPS position.
 * If the operation failed, returns PROS_ERR_F and errno is set.
 */
double gps_get_error(uint8_t port);

/**
 * Gets the position and roll, yaw, and pitch of the GPS.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a GPS
 * EAGAIN - The sensor is still calibrating
 *
 * \param  port
 * 				 The V5 GPS port number from 1-21
 *
 * \return A struct (gps_status_s_t) containing values mentioned above.
 * If the operation failed, all the structure's members are filled with
 * PROS_ERR_F and errno is set.
 */
gps_status_s_t gps_get_status(uint8_t port);

/**
 * Get the heading in [0,360) degree values.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a GPS
 * EAGAIN - The sensor is still calibrating
 *
 * \param  port
 * 				 The V5 GPS port number from 1-21
 *
 * \return The heading in [0,360) degree values. If the operation failed,
 * returns PROS_ERR_F and errno is set.
 */
double gps_get_heading(uint8_t port);

/**
 * Get the heading in the max double value and min double value scale.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a GPS
 * EAGAIN - The sensor is still calibrating
 *
 * \param  port
 * 				 The V5 GPS port number from 1-21
 *
 * \return The heading in [DOUBLE_MIN, DOUBLE_MAX] values. If the operation
 * fails, returns PROS_ERR_F and errno is set.
 */
double gps_get_heading_raw(uint8_t port);

/**
 * Gets the GPS sensor's elapsed rotation value
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a GPS
 * EAGAIN - The sensor is still calibrating
 *
 * \param  port
 * 				 The V5 GPS port number from 1-21
 * \return The elased heading in degrees. If the operation fails, returns
 * PROS_ERR_F and errno is set.
 */
double gps_get_rotation(uint8_t port);

/**
 * Set the GPS sensor's rotation value to target value
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a GPS
 * EAGAIN - The sensor is still calibrating
 *
 * \param  port
 * 				 The V5 GPS port number from 1-21
 * \param  target
 * 				 Target rotation value to set rotation value to
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t gps_set_rotation(uint8_t port, double target);

/**
 * Tare the GPS sensor's rotation value
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a GPS
 * EAGAIN - The sensor is still calibrating
 *
 * \param  port
 * 				 The V5 GPS port number from 1-21
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t gps_tare_rotation(uint8_t port);

/**
 * Get the GPS's raw gyroscope values
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a GPS
 * EAGAIN - The sensor is still calibrating
 *
 * \param  port
 * 				 The V5 GPS port number from 1-21
 * \return The raw gyroscope values. If the operation failed, all the
 * structure's members are filled with PROS_ERR_F and errno is set.
 */
gps_gyro_s_t gps_get_gyro_rate(uint8_t port);

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
gps_accel_s_t gps_get_accel(uint8_t port);

#ifdef __cplusplus
}
}
}
#endif

#endif
