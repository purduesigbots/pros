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

#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
namespace pros {
#endif

// Datatypes: TODO: Do proper doxygen documentation for structs

typedef struct __attribute__((__packed__)) gps_status_s {
    double pitch;
    double roll;
    double yaw;
    
    double x;
    double y;
} gps_status_s_t;

struct __attribute__((__packed__)) gps_raw_s {
	double x;
	double y;
	double z;
};

typedef gps_raw_s gps_accel_s_t;
typedef gps_raw_s gps_gyro_s_t;

#ifdef __cplusplus
namespace c {
#endif

void gps_set_origin(uint8_t port, double xOffset, double yOffset)
void gps_get_origin(uint8_t port, double* xOffset, double* yOffset);
void gps_set_position(uint8_t port, double xInitial, double yInitial, double headingInitial);
void gps_set_data_rate(uint8_t port, uint32_t rate);
double gps_get_error(uint8_t port);
gps_status_s_t gps_get_status(uint8_t port);
double gps_get_heading(uint8_t port);
double gps_get_heading_raw(uint8_t port);
double gps_get_rotation(uint8_t port);
void gps_set_rotation(uint8_t port, double target);
void gps_tare_rotation(uint8_t port);

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
}   // C extern
}   // namespace c
}   // namespace pros
#endif
