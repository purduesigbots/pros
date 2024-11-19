/**
 * \file pros/distance.h
 * \ingroup c-distance
 *
 * Contains prototypes for functions related to the VEX Distance sensor.
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
 * \defgroup c-distance VEX Distance Sensor C API
 */

#ifndef _PROS_DISTANCE_H_
#define _PROS_DISTANCE_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
namespace pros {
namespace c {
#endif

/**
 * \ingroup c-distance
 */

/**
 * \addtogroup c-distance
 *  @{
 */

/**
 * Get the currently measured distance from the sensor in mm
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Distance Sensor
 *
 * \param port The V5 Distance Sensor port number from 1-21
 * \return The distance value or PROS_ERR if the operation failed, setting
 * errno.
 * 
 * \b Example
 * \code
 * #define DISTANCE_PORT 1
 * 
 * void opcontrol() {
 *   while (true) {
 *     printf("Distance Value: %d mm\n", distance_get(DISTANCE_PORT));
 *     delay(20);
 *   }
 * }
 * \endcode
 */
int32_t distance_get(uint8_t port);

/**
 * Get the confidence in the distance reading
 *
 * This is a value that has a range of 0 to 63. 63 means high confidence,
 * lower values imply less confidence. Confidence is only available
 * when distance is > 200mm (the value 10 is returned in this scenario).
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Distance Sensor
 *
 * \param  port The V5 Distance Sensor port number from 1-21
 * \return The confidence value or PROS_ERR if the operation failed, setting
 * errno.
 * 
 * \b Example
 * \code
 * #define DISTANCE_PORT 1
 * 
 * void opcontrol() {
 *   while (true) {
 *     printf("Distance Confidence Value: %d\n", distance_get_confidence(DISTANCE_PORT));
 *     delay(20);
 *   }
 * }
 * \endcode
 */
int32_t distance_get_confidence(uint8_t port);

/**
 * Get the current guess at relative object size
 *
 * This is a value that has a range of 0 to 400.
 * A 18" x 30" grey card will return a value of approximately 75
 * in typical room lighting.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Distance Sensor
 *
 * \param  port The V5 Distance Sensor port number from 1-21
 * \return The size value or PROS_ERR if the operation failed, setting
 * errno.
 * 
 * \b Example
 * \code
 * #define DISTANCE_PORT 1
 * 
 * void opcontrol() {
 *   while (true) {
 *     printf("Distance Object Size: %d\n", distance_get_object_size(DISTANCE_PORT));
 *     delay(20);
 *   }
 * }
 * \endcode
 */
int32_t distance_get_object_size(uint8_t port);

/**
 * Get the object velocity in m/s
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Distance Sensor
 *
 * \param  port The V5 Distance Sensor port number from 1-21
 * \return The velocity value or PROS_ERR if the operation failed, setting
 * errno.
 * 
 * \b Example
 * \code
 * #define DISTANCE_PORT 1
 * 
 * void opcontrol() {
 *   while (true) {
 *     printf("Distance Object Velocity: %f\n", distance_get_object_velocity(DISTANCE_PORT));
 *     delay(20);
 *   }
 * }
 * \endcode
 */
double distance_get_object_velocity(uint8_t port);

///@}

#ifdef __cplusplus
}
}
}
#endif

#endif
