/**
 * \file pros/optical.hpp
 * \ingroup cpp-optical
 *
 * Contains prototypes for functions related to the VEX Optical sensor.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/imu.html to learn
 * more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2022, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * \defgroup cpp-optical VEX Optical Sensor C++ API
 */

#ifndef _PROS_OPTICAL_HPP_
#define _PROS_OPTICAL_HPP_

#include <stdbool.h>

#include <cstdint>

#include "pros/optical.h"

namespace pros {
inline namespace v5 {
/**
 * \ingroup cpp-optical
 */
class Optical {
	/**
	 * \addtogroup cpp-optical
	 *  @{
	 */

	public:
	/**
	 * Creates an Optical Sensor object for the given port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Optical Sensor
	 *
	 * \param port The V5 port number from 1-21
	 * 
	 * \b Example
	 * \code
	 * #define OPTICAL_PORT 1
	 * 
	 * void initialize() {
	 *   pros::Optical optical_sensor(OPTICAL_PORT);
	 * }
	 * \endcode
	 */
	explicit Optical(const std::uint8_t port);

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
	 * \return hue value if the operation was successful or PROS_ERR_F if the operation
	 * failed, setting errno.
	 * 
	 * \b Example
	 * \code
	 * #define OPTICAL_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Optical optical_sensor(OPTICAL_PORT);
	 *   while (true) {
	 *     printf("Hue value: %lf \n", optical_sensor.get_hue());
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual double get_hue();

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
	 * \return saturation value if the operation was successful or PROS_ERR_F if
	 * the operation failed, setting errno.
	 * 
	 * \b Example
	 * \code
	 * #define OPTICAL_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Optical optical_sensor(OPTICAL_PORT);
	 *   while (true) {
	 *     printf("Saturation value: %lf \n", optical_sensor.get_saturation());
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual double get_saturation();

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
	 * \return brightness value if the operation was successful or PROS_ERR_F if
	 * the operation failed, setting errno.
	 * 
	 * \b Example
	 * \code
	 * #define OPTICAL_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Optical optical_sensor(OPTICAL_PORT);
	 *   while (true) {
	 *     printf("Brightness value: %lf \n", optical_sensor.get_brightness());
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual double get_brightness();

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
	 * \return proximity value if the operation was successful or PROS_ERR if
	 * the operation failed, setting errno.
	 * 
	 * \b Example
	 * \code
	 * #define OPTICAL_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Optical optical_sensor(OPTICAL_PORT);
	 *   while (true) {
	 *     printf("Proximity value: %ld \n", optical_sensor.get_proximity());
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t get_proximity();

	/**
	 * Set the pwm value of the White LED on the sensor
	 *
	 * value that ranges from 0 to 100
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Optical Sensor
	 *
	 * \return The Error code encountered
	 * 
	 * \b Example
	 * \code
	 * #define OPTICAL_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Optical optical_sensor(OPTICAL_PORT);
	 *   while (true) {
	 *     optical_sensor.set_led_pwm(50);
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t set_led_pwm(uint8_t value);

	/**
	 * Get the pwm value of the White LED on the sensor
	 *
	 * value that ranges from 0 to 100
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Optical Sensor
	 *
	 * \return LED pwm value if the operation was successful or PROS_ERR if
	 * the operation failed, setting errno.
	 * 
	 * \b Example
	 * \code
	 * #define OPTICAL_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Optical optical_sensor(OPTICAL_PORT);
	 *   while (true) {
	 *     printf("LED PWM: %d \n", optical_sensor.get_led_pwm());
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t get_led_pwm();

	/**
	 * Get the processed RGBC data from the sensor
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Optical Sensor
	 *
	 * \return rgb value if the operation was successful or an optical_rgb_s_t 
	 * with all fields set to PROS_ERR if the operation failed, setting errno.
	 * 
	 * \b Example
	 * \code
	 * #define OPTICAL_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Optical optical_sensor(OPTICAL_PORT);
	 *   pros::c::optical_rgb_s_t rgb_value;
	 *   while (true) {
	 *     rgb_value = optical_sensor.get_rgb();
	 *     printf("Red value: %lf \n", rgb_value.red);
	 *     printf("Green value: %lf \n", rgb_value.green);
	 *     printf("Blue value: %lf \n", rgb_value.blue);
	 *     printf("Clear value: %lf \n", rgb_value.clear);
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual pros::c::optical_rgb_s_t get_rgb();

	/**
	 * Get the raw un-processed RGBC data from the sensor
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Optical Sensor
	 *
	 * \return raw rgb value if the operation was successful or an optical_raw_s_t 
	 * with all fields set to PROS_ERR if the operation failed, setting errno.
	 * 
	 * \b Example
	 * \code
	 * #define OPTICAL_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Optical optical_sensor(OPTICAL_PORT);
	 *   pros::c::optical_raw_s_t raw_values;
	 *   while (true) {
	 *     raw_values = optical_sensor.get_raw();
	 *     printf("Red value: %ld \n", raw_values.red);
	 *     printf("Green value: %ld \n", raw_values.green);
	 *     printf("Blue value: %ld \n", raw_values.blue);
	 *     printf("Clear value: %ld \n", raw_values.clear);
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual pros::c::optical_raw_s_t get_raw();

	/**
	 * Get the most recent gesture data from the sensor
	 *
	 * Gestures will be cleared after 500mS
	 * \n 0 = no gesture
	 * \n 1 = up (towards cable)
	 * \n 2 = down
	 * \n 3 = right
	 * \n 4 = left
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Optical Sensor
	 *
	 * \return gesture value if the operation was successful or PROS_ERR if
	 * the operation failed, setting errno.
	 * 
	 * \b Example
	 * \code
	 * #define OPTICAL_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Optical optical_sensor(OPTICAL_PORT);
	 *   while (true) {
	 *     printf("Direction: %d \n", optical_sensor.get_gesture());
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual pros::c::optical_direction_e_t get_gesture();

	/**
	 * Get the most recent raw gesture data from the sensor
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Optical Sensor
	 *
	 * \return gesture value if the operation was successful or an optical_gesture_s_t 
	 * with all fields set to PROS_ERR if the operation failed, setting errno.
	 * 
	 * \b Example
	 * \code
	 * #define OPTICAL_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Optical optical_sensor(OPTICAL_PORT);
	 *   pros::c::optical_gesture_s_t raw_gesture;
	 *   while (true) {
	 *     raw_gesture = optical_sensor.get_gesture_raw();
	 *     printf("Up data: %u \n", raw_gesture.udata);
	 *     printf("Down data: %u \n", raw_gesture.ddata);
	 *     printf("Left data: %u \n", raw_gesture.ldata);
	 *     printf("Right data: %u \n", raw_gesture.rdata);
	 *     printf("Type: %u \n", raw_gesture.type);
	 *     printf("Count: %u \n", raw_gesture.count);
	 *     printf("Time: %u \n", raw_gesture.time);
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual pros::c::optical_gesture_s_t get_gesture_raw();

	/**
	 * Enable gesture detection on the sensor
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Optical Sensor
	 *
	 * \return 1 if the operation is successful or PROS_ERR if the operation failed,
 	 * setting errno.
	  * 
	 * \b Example
	 * \code
	 * #define OPTICAL_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Optical optical_sensor(OPTICAL_PORT);
	 *   while (true) {
	 *     optical_sensor.enable_gesture();
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t enable_gesture();

	/**
	 * Disable gesture detection on the sensor
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Optical Sensor
	 *
	 * \return 1 if the operation is successful or PROS_ERR if the operation failed,
 	 * setting errno.
	  * 
	 * \b Example
	 * \code
	 * #define OPTICAL_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Optical optical_sensor(OPTICAL_PORT);
	 *   while (true) {
	 *     optical_sensor.disable_gesture();
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::int32_t disable_gesture();

	/**
	 * Gets the port number of the Optical Sensor.
	 *
	 * \return The Optical Sensor's port number.
	 * 
	 * \b Example
	 * \code
	 * #define OPTICAL_PORT 1
	 * 
	 * void opcontrol() {
	 *   pros::Optical optical_sensor(OPTICAL_PORT);
	 *   while (true) {
	 *     printf("Port number: %u \n", optical_sensor.get_port());
	 *     pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	virtual std::uint8_t get_port();

	private:
	const std::uint8_t _port;
	///@}
};
}  // namespace v5
}  // namespace pros

#endif // _PROS_OPTICAL_HPP_
