/**
 * \file pros/rotation.h
 *
 * Contains prototypes for functions related to the VEX Rotation Sensor.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/rotation.html to learn
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

#ifndef _PROS_ROTATION_H_
#define _PROS_ROTATION_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
namespace pros {
namespace c {
#endif

/**
 * Reset Rotation Sensor 
 *
 * Reset the current absolute position to be the same as the
 * Rotation Sensor angle.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Rotation Sensor
 *
 * \param port
 *        The V5 Rotation Sensor port number from 1-21
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t rotation_reset(uint8_t port);

/**
 * Set the Rotation Sensor position reading to a desired rotation value
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Rotation Sensor
 *
 * \param port
 *		  The V5 Rotation Sensor port number from 1-21
 * \param position 
 * 		  The position in terms of ticks
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t rotation_set_position(uint8_t port, uint32_t position);

/**
 * Reset the Rotation Sensor position to 0
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Rotation Sensor
 *
 * \param port
 *		  The V5 Rotation Sensor port number from 1-21

 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t rotation_reset_position(uint8_t port);

/**
 * Get the Rotation Sensor's current position in centidegrees
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Rotation Sensor
 *
 * \param  port
 * 				 The V5 Rotation Sensor port number from 1-21
 * \return The position value or PROS_ERR_F if the operation failed, setting
 * errno.
 */
int32_t rotation_get_position(uint8_t port);

/**
 * Get the Rotation Sensor's current velocity in centidegrees per second
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Rotation Sensor
 *
 * \param  port
 * 				 The V5 Rotation Sensor port number from 1-21
 * \return The velocity value or PROS_ERR_F if the operation failed, setting
 * errno.
 */
int32_t rotation_get_velocity(uint8_t port);

/**
 * Get the Rotation Sensor's current angle in centidegrees (0-36000)
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Rotation Sensor
 *
 * \param  port
 * 				 The V5 Rotation Sensor port number from 1-21
 * \return The angle value (0-36000) or PROS_ERR_F if the operation failed, setting
 * errno.
 */
int32_t rotation_get_angle(uint8_t port);

/**
 * Set the Rotation Sensor's direction reversed flag
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Rotation Sensor
 *
 * \param  port
 * 				 The V5 Rotation Sensor port number from 1-21
 * \param  value
 * 				 Determines if the direction of the Rotation Sensor is reversed or not.
 * 
 * \return 1 if operation succeeded or PROS_ERR if the operation failed, setting
 * errno.
 */
int32_t rotation_set_reversed(uint8_t port, bool value);

/**
 * Reverse the Rotation Sensor's direction
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Rotation Sensor
 *
 * \param  port
 * 				 The V5 Rotation Sensor port number from 1-21
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t rotation_reverse(uint8_t port);

/**
 * Get the Rotation Sensor's reversed flag
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Rotation Sensor
 *
 * \param  port
 * 				 The V5 Rotation Sensor port number from 1-21
 * 
 * \return Boolean value of if the Rotation Sensor's direction is reversed or not
 * or PROS_ERR if the operation failed, setting errno.
 */
int32_t rotation_get_reversed(uint8_t port);

#ifdef __cplusplus
} //namespace C
} //namespace pros
} //extern "C"
#endif

#endif
