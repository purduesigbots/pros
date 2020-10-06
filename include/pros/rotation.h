/**
 * \file pros/rotation.h
 *
 * Contains prototypes for functions related to the VEX Rotation sensor.
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


// NOTE: Descriptions of what each of these methods do may be inaccurate,
// as it's just a speculative guess as to what they actually do.

// TODO: Make revisions when proper documentation is given.

/**
 * Resets Rotational Sensor 
 *
 * Resets rotation presumably to 0.
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
 * Set the Rotation sensor to a desired rotation value.
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
 * Get the Rotation sensor's current rotational position.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Rotation Sensor
 *
 * \param  port
 * 				 The V5 Rotation Sensor port number from 1-21
 * \return The degree value or PROS_ERR_F if the operation failed, setting
 * errno.
 */
int32_t rotation_get_position(uint8_t port);

/**
 * Get the Rotation sensor's current rotational velocity.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Rotation Sensor
 *
 * \param  port
 * 				 The V5 Rotation Sensor port number from 1-21
 * \return The degree value or PROS_ERR_F if the operation failed, setting
 * errno.
 */
int32_t rotation_get_velocity(uint8_t port);

/**
 * Get the Rotation sensor's current rotational position in terms of an angle.
 *
 * TODO: Add description for what units this uses.
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Rotation Sensor
 *
 * \param  port
 * 				 The V5 Rotation Sensor port number from 1-21
 * \return The degree value or PROS_ERR_F if the operation failed, setting
 * errno.
 */
int32_t rotation_get_angle(uint8_t port);

/**
 * Sets if the rotational sensor's positive/negative direction is reversed or not.
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Rotation Sensor
 *
 * \param  port
 * 				 The V5 Rotation Sensor port number from 1-21
 * \param  value
 * 				 Determines if the direction of the rotational sensor is reversed or not.
 * 
 * \return 1 if operation succeeded or or PROS_ERR_F if the operation failed, setting
 * errno.
 */
int32_t rotation_set_reversed(uint8_t port, bool value);

/**
 * Gets if the rotational sensor's positive/negative direction is reversed or not.
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Rotation Sensor
 *
 * \param  port
 * 				 The V5 Rotation Sensor port number from 1-21
 * 
 * \return Boolean value of if the rotation sensor's direction is reversed or not.
 */
bool rotation_get_reversed(uint8_t port);

#ifdef __cplusplus
} //namespace C
} //namespace pros
} //extern "C"
#endif

#endif
