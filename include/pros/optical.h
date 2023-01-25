/**
 * \file pros/optical.h
 *
 * Contains prototypes for functions related to the VEX Optical sensor.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/imu.html to learn
 * more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_OPTICAL_H_
#define _PROS_OPTICAL_H_

#include <stdbool.h>
#include <stdint.h>
#include "error.h"

#define OPT_GESTURE_ERR (INT8_MAX)
#define OPT_COUNT_ERR (INT16_MAX)
#define OPT_TIME_ERR PROS_ERR

#ifdef __cplusplus
extern "C" {
namespace pros {
namespace c {
#endif


typedef enum optical_direction_e { 
	NO_GESTURE = 0, 
	UP = 1, 
	DOWN = 2, 
	RIGHT = 3, 
	LEFT = 4, 
	ERROR = PROS_ERR 
} optical_direction_e_t;

typedef struct optical_rgb_s {
	double red;
	double green;
	double blue;
	double brightness;
} optical_rgb_s_t;

typedef struct optical_raw_s {
	uint32_t clear;
	uint32_t red;
	uint32_t green;
	uint32_t blue;
} optical_raw_s_t;

typedef struct optical_gesture_s {
	uint8_t udata;
	uint8_t ddata;
	uint8_t ldata;
	uint8_t rdata;
	uint8_t type;
	uint8_t pad;
	uint16_t count;
	uint32_t time;
} optical_gesture_s_t;

/**
 * Get the detected color hue
 *
 * This is not available if gestures are being detected. Hue has a
 * range of 0 to 359.999
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Optical Sensor
 *
 * \param port
 *        The V5 Optical Sensor port number from 1-21
 * \return hue value if the operation was successful or PROS_ERR_F if the operation
 * failed, setting errno.
 */
double optical_get_hue(uint8_t port);

/**
 * Get the detected color saturation
 *
 * This is not available if gestures are being detected. Saturation has a
 * range of 0 to 1.0
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Optical Sensor
 *
 * \param port
 *        The V5 Optical Sensor port number from 1-21
 * \return saturation value if the operation was successful or PROS_ERR_F if
 * the operation failed, setting errno.
 */
double optical_get_saturation(uint8_t port);

/**
 * Get the detected color brightness
 *
 * This is not available if gestures are being detected. Brightness has a
 * range of 0 to 1.0
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Optical Sensor
 *
 * \param port
 *        The V5 Optical Sensor port number from 1-21
 * \return brightness value if the operation was successful or PROS_ERR_F if
 * the operation failed, setting errno.
 */
double optical_get_brightness(uint8_t port);

/**
 * Get the detected proximity value
 *
 * This is not available if gestures are being detected. proximity has
 * a range of 0 to 255.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Optical Sensor
 *
 * \param port
 *        The V5 Optical Sensor port number from 1-21
 * \return poximity value if the operation was successful or PROS_ERR if
 * the operation failed, setting errno.
 */
int32_t optical_get_proximity(uint8_t port);

/**
 * Set the pwm value of the White LED
 *
 * value that ranges from 0 to 100
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Optical Sensor
 *
 * \param port
 *        The V5 Optical Sensor port number from 1-21
 * \return 1 if the operation is successful or PROS_ERR if the operation failed,
 * setting errno.
 */
int32_t optical_set_led_pwm(uint8_t port, uint8_t value);

/**
 * Get the pwm value of the White LED
 *
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Optical Sensor
 *
 * \param port
 *        The V5 Optical Sensor port number from 1-21
 * \return LED pwm value that ranges from 0 to 100 if the operation was 
 * successful or PROS_ERR if the operation failed, setting errno.
 */
int32_t optical_get_led_pwm(uint8_t port);

/**
 * Get the processed RGBC data from the sensor
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Optical Sensor
 *
 * \param port
 *        The V5 Optical Sensor port number from 1-21
 * \return rgb value if the operation was successful or an optical_rgb_s_t with
 * all fields set to PROS_ERR if the operation failed, setting errno.
 */
optical_rgb_s_t optical_get_rgb(uint8_t port);

/**
 * Get the raw, unprocessed RGBC data from the sensor
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Optical Sensor
 *
 * \param port
 *        The V5 Optical Sensor port number from 1-21
 * \return raw rgb value if the operation was successful or an optical_raw_s_t 
 * with all fields set to PROS_ERR if the operation failed, setting errno.
 */
optical_raw_s_t optical_get_raw(uint8_t port);

/**
 * Get the most recent gesture data from the sensor
 *
 * Gestures will be cleared after 500mS
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Optical Sensor
 *
 * \param port
 *        The V5 Optical Sensor port number from 1-21
 * \return gesture value if the operation was successful or PROS_ERR if
 * the operation failed, setting errno.
 */
optical_direction_e_t optical_get_gesture(uint8_t port);

/**
 * Get the most recent raw gesture data from the sensor
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Optical Sensor
 *
 * \param port
 *        The V5 Optical Sensor port number from 1-21
 * \return gesture value if the operation was successful or an optical_gesture_s_t 
 * with all fields set to PROS_ERR if the operation failed, setting errno.
 */
optical_gesture_s_t optical_get_gesture_raw(uint8_t port);

/**
 * Enable gesture detection on the sensor
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Optical Sensor
 *
 * \param port
 *        The V5 Optical Sensor port number from 1-21
 * \return 1 if the operation is successful or PROS_ERR if the operation failed,
 * setting errno.
 */
int32_t optical_enable_gesture(uint8_t port);

/**
 * Disable gesture detection on the sensor
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Optical Sensor
 *
 * \param port
 *        The V5 Optical Sensor port number from 1-21
 * \return 1 if the operation is successful or PROS_ERR if the operation failed,
 * setting errno.
 */
int32_t optical_disable_gesture(uint8_t port);

/**
 * Get integration time (update rate) of the optical sensor in milliseconds, with
 * minimum time being 
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Optical Sensor
 *
 * \param port
 *        The V5 Optical Sensor port number from 1-21
 * \return Integration time in milliseconds if the operation is successful 
 * or PROS_ERR if the operation failed, setting errno.
 */
double optical_get_integration_time(uint8_t port);

/**
 * Set integration time (update rate) of the optical sensor in milliseconds.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as an Optical Sensor
 *
 * \param port
 *        The V5 Optical Sensor port number from 1-21
 * \param time
 *        The desired integration time in milliseconds
 * \return 1 if the operation is successful or PROS_ERR if the operation failed,
 * setting errno.
 */
int32_t optical_set_integration_time(uint8_t port, double time);

#ifdef __cplusplus
}
}
}
#endif

#endif
