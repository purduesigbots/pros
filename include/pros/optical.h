/**
 * \file pros/optical.h
 * \ingroup c-optical
 *
 * Contains prototypes for functions related to the VEX Optical sensor.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * \defgroup c-optical VEX Optical Sensor C API
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

/**
 * \ingroup c-optical
 */

/**
 * \addtogroup c-optical
 *  @{
 */

/**
 * \enum optical_direction_e_t
 */
typedef enum optical_direction_e { NO_GESTURE = 0,
	/// The direction indicating an upward gesture.
	UP = 1,
	/// The direction indicating a downward gesture.
	DOWN = 2,
	/// The direction indicating a rightward gesture.
	RIGHT = 3,
	/// The direction indicating a leftward gesture.
	LEFT = 4,
	ERROR = PROS_ERR 
} optical_direction_e_t;

/**
 * \struct optical_rgb_s_t
 * The RGB and Brightness values for the optical sensor.
 */
typedef struct optical_rgb_s {
	double red;
	double green;
	double blue;
	double brightness;
} optical_rgb_s_t;

/**
 * \struct optical_raw_s_t
 * The RGB and clear values for the optical sensor.
 */
typedef struct optical_raw_s {
	uint32_t clear;
	uint32_t red;
	uint32_t green;
	uint32_t blue;
} optical_raw_s_t;

/**
 * \struct optical_gesture_s_t
 * This structure contains the raw gesture data.
 */
typedef struct optical_gesture_s {
	uint8_t udata;  ///Up data
	uint8_t ddata;  ///Down data
	uint8_t ldata;  ///Left data
	uint8_t rdata;  ///Right data
	uint8_t type;   ///Type of gesture
	uint8_t pad;    ///Padding
	uint16_t count; ///Number of gestures
	uint32_t time;  ///Time since gesture recognized
} optical_gesture_s_t;

/**
 * \name Functions
 * @{
 */

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
 * 
 * \b Example
 * \code
 * #define OPTICAL_PORT 1
 * 
 * void opcontrol() {
 *   while (true) {
 *     printf("Hue value: %lf \n", optical_get_hue(OPTICAL_PORT));
 *     delay(20);
 *   }
 * }
 * \endcode
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
 * 
 * \b Example
 * \code
 * #define OPTICAL_PORT 1
 * 
 * void opcontrol() {
 *   while (true) {
 *     printf("Saturation value: %lf \n", optical_get_saturation(OPTICAL_PORT));
 *     delay(20);
 *   }
 * }
 * \endcode
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
 * 
 * \b Example
 * \code
 * #define OPTICAL_PORT 1
 * 
 * void opcontrol() {
 *   while (true) {
 *     printf("Brightness value: %lf \n", optical_get_brightness(OPTICAL_PORT));
 *     delay(20);
 *   }
 * }
 * \endcode
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
 * 
 * \b Example
 * \code
 * #define OPTICAL_PORT 1
 * 
 * void opcontrol() {
 *   while (true) {
 *     printf("Proximity value: %d \n", optical_get_proximity(OPTICAL_PORT));
 *     delay(20);
 *   }
 * }
 * \endcode
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
 * 
 * \b Example
 * \code
 * #define OPTICAL_PORT 1
 * 
 * void opcontrol() {
 *   while (true) {
 *     optical_set_led_pwm(OPTICAL_PORT, 50);
 *     delay(20);
 *   }
 * }
 * \endcode
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
 * 
 * \b Example
 * \code
 * #define OPTICAL_PORT 1
 * 
 * void opcontrol() {
 *   while (true) {
 *     printf("PWM Value: %d \n", optical_get_led_pwm(OPTICAL_PORT));
 *     delay(20);
 *   }
 * }
 * \endcode
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
 * 
 * \b Example
 * \code
 * #define OPTICAL_PORT 1
 * 
 * optical_rgb_s_t RGB_values;
 * void opcontrol() {
 *   while (true) {
 *     RGB_values = optical_get_rgb(OPTICAL_PORT);
 *     printf("Red value: %lf \n", RGB_values.red);
 *     printf("Green value: %lf \n", RGB_values.green);
 *     printf("Blue value: %lf \n", RGB_values.blue);
 *     printf("Brightness value: %lf \n", RGB_values.brightness);
 *     delay(20);
 *   }
 * }
 * \endcode
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
 * 
 * \b Example
 * \code
 * #define OPTICAL_PORT 1
 * 
 * optical_raw_s_t raw_values;
 * void opcontrol() {
 * while (true) {
 *   raw_values = optical_get_raw(OPTICAL_PORT);
 *   printf("Red value: %ld \n", raw_values.red);
 *   printf("Green value: %ld \n", raw_values.green);
 *   printf("Blue value: %ld \n", raw_values.blue);
 *   printf("Clear value: %ld \n", raw_values.clear);
 *   delay(20);
 *   }
 * }
 * \endcode
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
 * 
 * \b Example
 * \code
 * #define OPTICAL_PORT 1
 * 
 * optical_direction_e_t gesture;
 * void opcontrol() {
 *   while (true) {
 *     gesture = optical_get_gesture(OPTICAL_PORT);
 *     printf("Gesture value: %d \n", gesture);
 *     delay(20);
 *   }
 * }
 * \endcode
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
 * 
 * \b Example
 * \code
 * #define OPTICAL_PORT 1
 * 
 * optical_gesture_s_t raw_gesture;
 * void opcontrol() {
 *   while (true) {
 *     raw_gesture = optical_get_gesture_raw(OPTICAL_PORT);
 *     printf("Up data: %u \n", raw_gesture.udata);
 *     printf("Down data: %u \n", raw_gesture.ddata);
 *     printf("Left data: %u \n", raw_gesture.ldata);
 *     printf("Right data: %u \n", raw_gesture.rdata);
 *     printf("Type: %u \n", raw_gesture.type);
 *     printf("Count: %u \n", raw_gesture.count);
 *     printf("Time: %lu \n", raw_gesture.time);
 *     delay(20);
 *   }
 * }
 * \endcode
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
 * 
 * \b Example
 * \code
 * #define OPTICAL_PORT 1
 * 
 * void opcontrol() {
 *   while (true) {
 *     optical_enable_gesture(OPTICAL_PORT);
 *     delay(20);
 *   }
 * }
 * \endcode
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
 * 
 * \b Example
 * \code
 * #define OPTICAL_PORT 1
 * 
 * void opcontrol() {
 *   while (true) {
 *     optical_disable_gesture(OPTICAL_PORT);
 *     delay(20);
 *   }
 * }
 * \endcode
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

///@}

///@}

#ifdef __cplusplus
}
}
}
#endif

#endif
