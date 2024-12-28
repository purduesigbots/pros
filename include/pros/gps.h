/**
 * \file pros/gps.h
 * \ingroup c-gps
 *
 * Contains prototypes for functions related to the VEX GPS.
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
 * \defgroup c-gps VEX GPS Sensor C API
 * \note For a pros-specific usage guide on the GPS, please check out our article [here.](@ref gps)
 */

#ifndef _PROS_GPS_H_
#define _PROS_GPS_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
namespace pros {
#endif 

/**
 * \ingroup c-gps
 */

/**
 * \addtogroup c-gps
 *  @{
 */

/**
 * \struct gps_position_s_t
 */
typedef struct __attribute__((__packed__)) gps_position_s {
	/// X Position (meters)
	double x;
	/// Y Position (meters)
	double y;
} gps_position_s_t;

/**
 * \struct gps_status_s_t
 */
typedef struct __attribute__((__packed__)) gps_status_s {
	/// X Position (meters)
	double x;
	/// Y Position (meters)
	double y;
	/// Perceived Pitch based on GPS + IMU
	double pitch;
	/// Perceived Roll based on GPS + IMU
	double roll;
	/// Perceived Yaw based on GPS + IMU
	double yaw;
} gps_status_s_t;

/**
 * \struct gps_orientation_s_t
 */
typedef struct __attribute__((__packed__)) gps_orientation_s {
	/// Perceived Pitch based on GPS + IMU
	double pitch;
	/// Perceived Roll based on GPS + IMU
	double roll;
	/// Perceived Yaw based on GPS + IMU
	double yaw;
} gps_orientation_s_t;


/**
 * \struct gps_raw_s
 */
struct gps_raw_s {
	/// Perceived Pitch based on GPS + IMU
	double x;
	/// Perceived Roll based on GPS + IMU
	double y;
	/// Perceived Yaw based on GPS + IMU
	double z;
};

/**
 * \struct gps_accel_s_t
 * 
 */
typedef struct gps_raw_s gps_accel_s_t;

/**
 * \struct gps_gyro_s_t
 * 
 */
typedef struct gps_raw_s gps_gyro_s_t;

#ifdef __cplusplus
namespace c {
#endif


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
 * 
 * \b Example
 * \code
 * #define GPS_PORT 1
 * #define X_OFFSET .225
 * #define Y_OFFSET .223
 * #define X_INITIAL 1.54
 * #define Y_INITIAL 1.14
 * #define HEADING_INITIAL 90
 * 
 * void initialize() {
 *   gps_initialize_full(GPS_PORT, X_OFFSET, Y_OFFSET, X_INITIAL, Y_INITIAL, HEADING_INITIAL);
 * }
 * \endcode
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
 * 
 * \b Example
 * \code
 * #define GPS_PORT 1
 * #define X_OFFSET -.225
 * #define Y_OFFSET .225
 * 
 * void initialize() {
 *   gps_set_offset(GPS_PORT, X_OFFSET, Y_OFFSET);
 * }
 * \endcode
 */
int32_t gps_set_offset(uint8_t port, double xOffset, double yOffset);

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
 * 
 *   while (true) {
 *     pos = gps_get_offset(GPS_PORT);
 *     screen_print(TEXT_MEDIUM, 1, "X Offset: %4d, Y Offset: %4d", pos.x, pos.y);
 *     delay(20);
 *   }
 * }
 * \endcode
 */
gps_position_s_t gps_get_offset(uint8_t port);

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
 * 
 * \b Example
 * \code
 * #define GPS_PORT 1
 * #define X_INITIAL -1.15
 * #define Y_INITIAL 1.45
 * #define HEADING_INITIAL 90
 * 
 * void initialize() {
 *   gps_set_position(GPS_PORT, X_INITIAL, Y_INITIAL, HEADING_INITIAL);
 * }
 * \endcode
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
 * 
 * \b Example
 * \code
 * #define GPS_PORT 1
 * #define GPS_DATA_RATE 5
 * 
 * void initialize() {
 *   gps_set_data_rate(GPS_PORT, GPS_DATA_RATE);
 *   while (true) {
 *     // Do something
 *   }
 * }
 * \endcode
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
 * 
 * \b Example
 * \code
 * #define GPS_PORT 1
 * 
 * void opcontrol() {
 *   double error;
 *   error = gps_get_error(GPS_PORT);
 *   screen_print(TEXT_MEDIUM, 1, "Error: %4d", error);
 * }
 * \endcode
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
 *
 * \b Example
 * \code
 * #define GPS_PORT 1
 *
 * void opcontrol() {
 *   gps_status_s_t status;
 *
 *   while (true) {
 *     status = gps_get_position_and_orientation(GPS_PORT);
 *     printf("X: %f, Y: %f, Pitch: %f, Roll: %f, Yaw: %f\n", status.x, status.y, status.pitch, status.roll, status.yaw);
 *     delay(20);
 *   }
 * }
 * \endcode
 */
gps_status_s_t gps_get_position_and_orientation(uint8_t port);

/**
 * Gets the x and y position on the field of the GPS in meters.
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
 * \return A struct (gps_position_s_t) containing values mentioned above.
 * If the operation failed, all the structure's members are filled with
 * PROS_ERR_F and errno is set.
 * 
 * \b Example
 * \code
 * #define GPS_PORT 1
 *
 * void opcontrol() {
 *   gps_position_s_t position;
 *
 *   while (true) {
 *     position = gps_get_position(GPS_PORT);
 *     printf("X: %f, Y: %f\n", position.x, position.y);
 *     delay(20);
 *   }
 * }
 * \endcode
 */
gps_position_s_t gps_get_position(uint8_t port);

/**
 * Gets the X position in meters of the robot relative to the starting position.
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
 * \return The X position in meters. If the operation failed,
 * returns PROS_ERR_F and errno is set.
 * 
 * \b Example
 * \code
 * #define GPS_PORT 1
 *
 * void opcontrol() {
 *   double pos_x;
 *
 *   while (true) {
 *     pos_x = gps_get_position_x(GPS_PORT);
 *     printf("X: %f\n", pos_x);
 *     delay(20);
 *   }
 * }
 * \endcode
 */
double gps_get_position_x(uint8_t port);

/**
 * Gets the Y position in meters of the robot relative to the starting position.
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
 * \return The Y position in meters. If the operation failed,
 * returns PROS_ERR_F and errno is set.
 * 
 * \b Example
 * \code
 * #define GPS_PORT 1
 *
 * void opcontrol() {
 *   double pos_y;
 *
 *   while (true) {
 *     pos_y = gps_get_position_y(GPS_PORT);
 *     printf("Y: %f\n", pos_y);
 *     delay(20);
 *   }
 * }
 * \endcode
 */
double gps_get_position_y(uint8_t port);

/**
 * Gets the pitch, roll, and yaw of the GPS relative to the starting orientation.
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
 * \return A struct (gps_orientation_s_t) containing values mentioned above.
 * If the operation failed, all the structure's members are filled with
 * PROS_ERR_F and errno is set.
 * 
 * \b Example
 * \code
 * #define GPS_PORT 1
 *
 * void opcontrol() {
 *   gps_orientation_s_t orientation;
 *
 *   while (true) {
 *     orientation = gps_get_orientation(GPS_PORT);
 *     printf("pitch: %f, roll: %f, yaw: %f\n", orientation.pitch, orientation.roll, orientation.yaw);
 *     delay(20);
 *   }
 * }
 * \endcode
*/
gps_orientation_s_t gps_get_orientation(uint8_t port);

/**
 * Gets the pitch of the robot in degrees relative to the starting oreintation.
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
 * \return The pitch in [0,360) degree values. If the operation failed,
 * returns PROS_ERR_F and errno is set.
 * 
 * \b Example
 * \code
 * #define GPS_PORT 1
 *
 * void opcontrol() {
 *   double pitch;
 *
 *   while (true) {
 *     pitch = gps_get_pitch(GPS_PORT);
 *     printf("pitch: %f\n", pitch);
 *     delay(20);
 *   }
 * }
 * \endcode
 */
double gps_get_pitch(uint8_t port);

/**
 * Gets the roll of the robot in degrees relative to the starting oreintation.
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
 * \return The roll in [0,360) degree values. If the operation failed,
 * returns PROS_ERR_F and errno is set.
 * 
 * \b Example
 * \code
 * #define GPS_PORT 1
 *
 * void opcontrol() {
 *   double roll;
 *
 *   while (true) {
 *     roll = gps_get_roll(GPS_PORT);
 *     printf("roll: %f\n", roll);
 *     delay(20);
 *   }
 * }
 * \endcode
 */
double gps_get_roll(uint8_t port);

/**
 * Gets the yaw of the robot in degrees relative to the starting oreintation.
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
 * \return The yaw in [0,360) degree values. If the operation failed,
 * returns PROS_ERR_F and errno is set.
 * 
 * \b Example
 * \code
 * #define GPS_PORT 1
 *
 * void opcontrol() {
 *   double yaw;
 *
 *   while (true) {
 *     yaw = gps_get_yaw(GPS_PORT);
 *     printf("yaw: %f\n", yaw);
 *     delay(20);
 *   }
 * }
 * \endcode
 */
double gps_get_yaw(uint8_t port);

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
 * 
 * \b Example
 * \code
 * #define GPS_PORT 1
 *
 * void opcontrol() {
 *   double heading;
 *
 *   while (true) {
 *     heading = gps_get_heading(GPS_PORT);
 *     printf("heading: %f\n", heading);
 *     delay(20);
 *   }
 * }
 * \endcode
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
 * 
 * \b Example
 * \code
 * #define GPS_PORT 1
 *
 * void opcontrol() {
 *   double heading_raw;
 *
 *   while (true) {
 *     heading_raw = gps_get_heading_raw(GPS_PORT);
 *     printf("heading_raw: %f\n", heading_raw);
 *     delay(20);
 *   }
 * }
 * \endcode
 */
double gps_get_heading_raw(uint8_t port);

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
 * \return A struct (gps_gyro_s_t) containing values mentioned above.
 * If the operation failed, all the
 * structure's members are filled with PROS_ERR_F and errno is set.
 *
 * \b Example
 * \code
 * #define GPS_PORT 1
 *
 * void opcontrol() {
 *   gps_gyro_s_t gyro;
 *
 *   while (true) {
 *     gyro = gps_get_gyro(GPS_PORT);
 *     printf("Gyro: %f %f %f\n", gyro.x, gyro.y, gyro.z);
 *     delay(20);
 *   }
 * }
 * \endcode
 */
gps_gyro_s_t gps_get_gyro_rate(uint8_t port);

/**
 * Get the GPS's raw gyroscope value in x-axis
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a GPS
 * EAGAIN - The sensor is still calibrating
 * 
 * \param  port
 * 				 The V5 GPS port number from 1-21
 * \return The raw gyroscope value in x-axis. If the operation fails, returns
 * PROS_ERR_F and errno is set.
 * 
 * \b Example
 * \code
 * #define GPS_PORT 1
 *
 * void opcontrol() {
 *   double gyro_x;
 *
 *   while (true) {
 *     gyro_x = gps_get_gyro_x(GPS_PORT);
 *     printf("gyro_x: %f\n", gyro_x);
 *     delay(20);
 *   }
 * }
 * \endcode
*/
double gps_get_gyro_rate_x(uint8_t port);

/**
 * Get the GPS's raw gyroscope value in y-axis
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a GPS
 * EAGAIN - The sensor is still calibrating
 * 
 * \param  port
 * 				 The V5 GPS port number from 1-21
 * \return The raw gyroscope value in y-axis. If the operation fails, returns
 * PROS_ERR_F and errno is set.
 * 
 * \b Example
 * \code
 * #define GPS_PORT 1
 *
 * void opcontrol() {
 *   double gyro_y;
 *
 *   while (true) {
 *     gyro_y = gps_get_gyro_y(GPS_PORT);
 *     printf("gyro_y: %f\n", gyro_y);
 *     delay(20);
 *   }
 * }
 * \endcode
*/
double gps_get_gyro_rate_y(uint8_t port);

/**
 * Get the GPS's raw gyroscope value in z-axis
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a GPS
 * EAGAIN - The sensor is still calibrating
 * 
 * \param  port
 * 				 The V5 GPS port number from 1-21
 * \return The raw gyroscope value in z-axis. If the operation fails, returns
 * PROS_ERR_F and errno is set.
 * 
 * \b Example
 * \code
 * #define GPS_PORT 1
 *
 * void opcontrol() {
 *   double gyro_z;
 *
 *   while (true) {
 *     gyro_z = gps_get_gyro_z(GPS_PORT);
 *     printf("gyro_z: %f\n", gyro_z);
 *     delay(20);
 *   }
 * }
 * \endcode
*/
double gps_get_gyro_rate_z(uint8_t port);

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
 * \return A struct (gps_accel_s_t) containing values mentioned above.
 * If the operation failed, all the
 * structure's members are filled with PROS_ERR_F and errno is set.
 * 
 * \b Example
 * \code
 * #define GPS_PORT 1
 *
 * void opcontrol() {
 *   gps_accel_s_t accel;
 *
 *   while (true) {
 *     accel = gps_get_accel(GPS_PORT);
 *     printf("X: %f, Y: %f, Z: %f\n", accel.x, accel.y, accel.z);
 *     delay(20);
 *   }
 * }
 * \endcode
 */
gps_accel_s_t gps_get_accel(uint8_t port);

/**
 * Get the GPS's raw accelerometer value in x-axis
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an GPS
 * EAGAIN - The sensor is still calibrating
 * 
 * \param  port
 * 				 The V5 GPS's port number from 1-21
 * \return The raw accelerometer value in x-axis. If the operation fails, returns
 * PROS_ERR_F and errno is set.
 * 
 * \b Example
 * \code
 * #define GPS_PORT 1
 *
 * void opcontrol() {
 *   double accel_x;
 *
 *   while (true) {
 *     accel_x = gps_get_accel_x(GPS_PORT);
 *     printf("accel_x: %f\n", accel_x);
 *     delay(20);
 *   }
 * }
 * \endcode
*/
double gps_get_accel_x(uint8_t port);

/**
 * Get the GPS's raw accelerometer value in y-axis
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an GPS
 * EAGAIN - The sensor is still calibrating
 * 
 * \param  port
 * 				 The V5 GPS's port number from 1-21
 * \return The raw accelerometer value in y-axis. If the operation fails, returns
 * PROS_ERR_F and errno is set.
 * 
 * \b Example
 * \code
 * #define GPS_PORT 1
 *
 * void opcontrol() {
 *   double accel_y;
 *
 *   while (true) {
 *     accel_y = gps_get_accel_y(GPS_PORT);
 *     printf("accel_y: %f\n", accel_y);
 *     delay(20);
 *   }
 * }
 * \endcode
*/
double gps_get_accel_y(uint8_t port);

/**
 * Get the GPS's raw accelerometer value in z-axis
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an GPS
 * EAGAIN - The sensor is still calibrating
 * 
 * \param  port
 * 				 The V5 GPS's port number from 1-21
 * \return The raw accelerometer value in z-axis. If the operation fails, returns
 * PROS_ERR_F and errno is set.
 * 
 * \b Example
 * \code
 * #define GPS_PORT 1
 *
 * void opcontrol() {
 *   double accel_z;
 *
 *   while (true) {
 *     accel_z = gps_get_accel_z(GPS_PORT);
 *     printf("accel_z: %f\n", accel_z);
 *     delay(20);
 *   }
 * }
 * \endcode
*/
double gps_get_accel_z(uint8_t port);

#ifdef __cplusplus
}
}
}
#endif

#endif
