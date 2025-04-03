/**
 * \file pros/optical.hpp
 * \ingroup cpp-optical
 *
 * Contains prototypes for functions related to the VEX Optical sensor.
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
 * \defgroup cpp-optical VEX Optical Sensor C++ API
 */

#ifndef _PROS_OPTICAL_HPP_
#define _PROS_OPTICAL_HPP_

#include <stdbool.h>

#include <cstdint>
#include <iostream>

#include "pros/device.hpp"
#include "pros/optical.h"

namespace pros {
inline namespace v5 {
/**
 * \ingroup cpp-optical
 */
class Optical : public Device {
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
	 * \param port
	 *        The V5 port number from 1-21
	 *
	 * \b Example:
	 * \code{.cpp}
	 * pros::Optical optical(1);
	 * \endcode
	 */
	Optical(const std::uint8_t port);

	Optical(const Device& device) : Optical(device.get_port()){};


	/**
	 * Gets all optical sensors.
	 * 
	 * \return A vector of Optical sensor objects.
	 *
	 * \b Example
 	 * \code
	 * void opcontrol() {
	 *   std::vector<Optical> optical_all = pros::Optical::get_all_devices();  // All optical sensors that are connected
	 * }
 	 * \endcode
	 */
	static std::vector<Optical> get_all_devices();

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
	 * \b Example:
	 * \code{.cpp}
	 * void opcontrol() {
	 * 		pros::Optical optical(1);
	 * 		std::cout << "Hue: " << optical.get_hue() << std::endl;
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
	 * \b Example:
	 * \code{.cpp}
	 * void opcontrol() {
	 * 		pros::Optical optical(1);
	 * 		std::cout << "Saturation: " << optical.get_saturation() << std::endl;
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
	 * \b Example:
	 * \code{.cpp}
	 * void opcontrol() {
	 * 		pros::Optical optical(1);
	 * 		std::cout << "Brightness: " << optical.get_brightness() << std::endl;
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
	 * \return Proximity value if the operation was successful or PROS_ERR if
	 * the operation failed, setting errno.
	 *
	 * \b Example:
	 * \code{.cpp}
	 * void opcontrol() {
	 * 		pros::Optical optical(1);
	 * 		std::cout << "Proximity: " << optical.get_proximity() << std::endl;
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
	 * \return The Error code encountered or PROS_SUCCESS.
	 *
	 * \b Example:
	 * \code{.cpp}
	 * void initialize() {
	 * 		pros::Optical optical(1);
	 * 		optical.set_led_pwm(100);
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
	 * \b Example:
	 * \code{.cpp}
	 * void opcontrol() {
	 * 		pros::Optical optical(1);
	 *		optical.set_led_pwm(100);
	 * 		std::cout << "LED PWM: " << optical.get_led_pwm() << std::endl;
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
	 * \b Example:
	 * \code{.cpp}
	 * void opcontrol() {
	 * 		pros::Optical optical(1);
	 * 		pros::c::optical_rgb_s_t rgb = optical.get_rgb();
	 * 		while(1) {
	 * 			std::cout << "Red: " << rgb.red << std::endl;
	 * 			std::cout << "Green: " << rgb.green << std::endl;
	 * 			std::cout << "Blue: " << rgb.blue << std::endl;
	 * 			std::cout << "Brightness: " << rgb.brightness << std::endl;
	 * 			pros::delay(20);
	 * 		}
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
	 * \b Example:
	 * \code{.cpp}
	 * void opcontrol() {
	 * 		pros::Optical optical(1);
	 * 		pros::c::optical_raw_s_t raw = optical.get_raw();
	 * 		while (1) {
	 * 			std::cout << "Red: " << raw.red << std::endl;
	 * 			std::cout << "Green: " << raw.green << std::endl;
	 * 			std::cout << "Blue: " << raw.blue << std::endl;
	 * 			std::cout << "Clear: " << raw.clear << std::endl;
	 * 			pros::delay(20);
	 * 		}
	 * }
	 * \endcode
	 */
	virtual pros::c::optical_raw_s_t get_raw();

	/**
	 * Get the most recent gesture data from the sensor
	 *
	 * Gestures will be cleared after 500mS
	 *
	 * 
	 * 0 = no gesture,
	 * 1 = up (towards cable),
	 * 2 = down,
	 * 3 = right,
	 * 4 = left
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Optical Sensor
	 *
	 * \return gesture value if the operation was successful or PROS_ERR if
	 * the operation failed, setting errno.
	 *
	 * \b Example:
	 * \code{.cpp}
	 * void opcontrol() {
	 * 		pros::Optical optical(1);
	 * 		while(1) {
	 * 			std::cout << "Gesture: " << optical.get_gesture() << std::endl;
	 * 			pros::delay(20);
	 * 		}
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
	 * \b Example:
	 * \code{.cpp}
	 * void opcontrol() {
	 * 		pros::Optical optical(1);
	 *		optical.enable_gesture();
	 * 		while(1) {
	 * 			pros::c::optical_gesture_s_t gesture = optical.get_gesture_raw();
	 * 			std::cout << "Gesture raw data: " << std::endl;
	 * 			std::cout << "Up data: " << gesture.udata << std::endl;
	 * 			std::cout << "Down data: " << gesture.ddata << std::endl;
	 * 			std::cout << "Left data: " << gesture.ldata << std::endl;
	 * 			std::cout << "Right data: " << gesture.rdata << std::endl;
	 * 			std::cout << "Type: " << gesture.type << std::endl;
	 * 			std::cout << "Count: " << gesture.count << std::endl;
	 * 			std::cout << "Time: " << gesture.time << std::endl;
	 * 			pros::delay(20);
	 * 		}
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
	 * \b Example:
	 * \code{.cpp}
	 * void opcontrol() {
	 * 		pros::Optical optical(1);
	 *		optical.enable_gesture();
	 * 		while(1) {
	 * 			pros::c::optical_gesture_s_t gesture = optical.get_gesture_raw();
	 * 			std::cout << "Gesture raw data: " << std::endl;
	 * 			std::cout << "Up data: " << gesture.udata << std::endl;
	 * 			std::cout << "Down data: " << gesture.ddata << std::endl;
	 * 			std::cout << "Left data: " << gesture.ldata << std::endl;
	 * 			std::cout << "Right data: " << gesture.rdata << std::endl;
	 * 			std::cout << "Type: " << gesture.type << std::endl;
	 * 			std::cout << "Count: " << gesture.count << std::endl;
	 * 			std::cout << "Time: " << gesture.time << std::endl;
	 * 			pros::delay(20);
	 * 		}
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
	 * \b Example:
	 * \code{.cpp}
	 * void opcontrol() {
	 * 		pros::Optical optical(1);
	 *		optical.enable_gesture();
	 * 		while(1) {
	 * 			if(optical.get_gesture() != 0) {
	 * 				std::cout << "Gesture detected!"<< std::endl;
	 * 				optical.disable_gesture();
	 * 			}
	 * 			pros::delay(20);
	 * 		}
	 * }
	 * \endcode
	 */
	virtual std::int32_t disable_gesture();

	/**
	 * Get integration time (update rate) of the optical sensor in milliseconds.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Optical Sensor
	 *
	 * \return Integration time in milliseconds if the operation is successful
	 *  or PROS_ERR_F if the operation failed, setting errno.
	 */
	double get_integration_time();

	/**
	 * Set integration time (update rate) of the optical sensor in milliseconds, with
	 * minimum time being 3 ms and maximum time being 712 ms. Default is 100 ms, with the
	 * optical sensor communciating with the V5 brain every 20 ms.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as an Optical Sensor
 	 *
	 * \param time The desired integration time in milliseconds
	 * \return 1 if the operation is successful or PROS_ERR if the operation failed,
	 * setting errno.
	 */
	std::int32_t set_integration_time(double time);

	/**
	 * This is the overload for the << operator for printing to streams
	 *
	 * Prints in format(this below is all in one line with no new line):
	 * Optical [port: (port number), hue: (hue), saturation: (saturation),
	 * brightness: (brightness), proximity: (proximity), rgb: {red, green, blue}]
	 *
	 * \b Example:
	 * \code{.cpp}
	 * pros::Optical optical(1);
	 * std::cout << optical << std::endl;
	 * \endcode
	 */
	friend std::ostream& operator<<(std::ostream& os, pros::Optical& optical);

	private:
	///@}
};

namespace literals {
/**
 * Constructs a Optical sensor from a literal ending in _opt
 *
 * \return a pros::Optical for the corresponding port
 *
 * \b Example
 * \code
 * using namespace pros::literals;
 * void opcontrol() {
 *	pros::Optical opt = 2_opt; //Makes an Optical object on port 2
 * }
 * \endcode
 */
const pros::Optical operator"" _opt(const unsigned long long int o);
}  // namespace literals
}  // namespace v5
}  // namespace pros

#endif
