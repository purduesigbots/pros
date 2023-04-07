/**
 * \file pros/vision.hpp
 * \ingroup cpp-vision
 *
 * Contains prototypes for the VEX Vision Sensor-related functions in C++.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \defgroup cpp-vision Vision Sensor C++ API
 * \note Additional example code for this module can be found in its [Tutorial.](@ref vision)
 */

#ifndef _PROS_VISION_HPP_
#define _PROS_VISION_HPP_

#include <cstdint>

#include "pros/vision.h"

namespace pros {
inline namespace v5 {
/**
 * \ingroup cpp-vision
 */
class Vision : public Device {
	/**
	 * \addtogroup cpp-vision
	 *  @{
	 */
	public:
	/**
	 * Create a Vision Sensor object on the given port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \param port
	 *        The V5 port number from 1-21
	 * \param zero_point
	 *        One of vision_zero_e_t to set the (0,0) coordinate for the FOV
	 * 
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Vision vision_sensor(1); // Creates a vision sensor on port one, with the zero point set to top left
	 * }
	 * \endcode
	 */
	explicit Vision(std::uint8_t port, vision_zero_e_t zero_point = E_VISION_ZERO_TOPLEFT);

	/**
	 * Clears the vision sensor LED color, reseting it back to its default
	 * behavior, displaying the most prominent object signature color.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 * 
	 * \b Example
 	 * \code
 	 * void initialize() {
	 *   pros::Vision vision_sensor(1);
 	 *   vision_sensor.clear_led();
  	 * }
 	 * \endcode
	 */
	std::int32_t clear_led(void) const;

	/**
	 * Creates a signature from the vision sensor utility
	 *
	 * \param id
	 *        The signature ID
	 * \param u_min
	 *        Minimum value on U axis
	 * \param u_max
	 *        Maximum value on U axis
	 * \param u_mean
	 *        Mean value on U axis
	 * \param v_min
	 *        Minimum value on V axis
	 * \param v_max
	 *        Maximum value on V axis
	 * \param v_mean
	 *        Mean value on V axis
	 * \param rgb
	 *        Scale factor
	 * \param type
	 *        Signature type
	 *
	 * \return A vision_signature_s_t that can be set using Vision::set_signature
	 * 
	 * \b Example
 	 * \code
 	 * #define VISION_PORT 1
 	 * #define EXAMPLE_SIG 1
 	 *
 	 * void opcontrol() {
	 * pros::Vision vision_sensor(VISION_PORT);
 	 * // values acquired from the vision utility
 	 * vision_signature_s_t RED_SIG =
 	 *   vision_signature_from_utility(EXAMPLE_SIG, 8973, 11143, 10058, -2119, -1053, -1586, 5.4, 0);
 	 * vision_sensor.set_signature(EXAMPLE_SIG, &RED_SIG);
 	 * while (true) {
 	 *   vision_signature_s_t rtn = vision_sensor.get_by_sig(VISION_PORT, 0, EXAMPLE_SIG);
 	 *   // Gets the largest object of the EXAMPLE_SIG signature
 	 *   printf("sig: %d", rtn.signature);
 	 *   // Prints "sig: 1"
 	 *   delay(2);
 	 *   }
 	 * }
 	 * \endcode
	 */
	static vision_signature_s_t signature_from_utility(const std::int32_t id, const std::int32_t u_min,
	                                                   const std::int32_t u_max, const std::int32_t u_mean,
	                                                   const std::int32_t v_min, const std::int32_t v_max,
	                                                   const std::int32_t v_mean, const float range,
	                                                   const std::int32_t type);

	/**
	 * Creates a color code that represents a combination of the given signature
	 * IDs.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - Fewer than two signatures have been provided or one of the
	 *          signatures is out of its [1-7] range (or 0 when omitted).
	 *
	 * \param sig_id1
	 *        The first signature id [1-7] to add to the color code
	 * \param sig_id2
	 *        The second signature id [1-7] to add to the color code
	 * \param sig_id3
	 *        The third signature id [1-7] to add to the color code
	 * \param sig_id4
	 *        The fourth signature id [1-7] to add to the color code
	 * \param sig_id5
	 *        The fifth signature id [1-7] to add to the color code
	 *
	 * \return A vision_color_code_t object containing the color code information.
	 * 
	 * \b Example
 	 * \code
 	 * #define VISION_PORT 1
 	 * #define EXAMPLE_SIG 1
 	 * #define OTHER_SIG 2
 	 * 
 	 * void opcontrol() {
	 *   pros::Vision vision_sensor(VISION_PORT);
 	 *   vision_color_code_t code1 = vision_sensor.create_color_code(EXAMPLE_SIG, OTHER_SIG);
 	 * }
 	 * \endcode
	 */
	vision_color_code_t create_color_code(const std::uint32_t sig_id1, const std::uint32_t sig_id2,
	                                      const std::uint32_t sig_id3 = 0, const std::uint32_t sig_id4 = 0,
	                                      const std::uint32_t sig_id5 = 0) const;

	/**
	 * Gets the nth largest object according to size_id.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a vision sensor
	 * EDOM - size_id is greater than the number of available objects.
	 * EAGAIN - Reading the vision sensor failed for an unknown reason.
	 *
	 * \param size_id
	 *        The object to read from a list roughly ordered by object size
	 *        (0 is the largest item, 1 is the second largest, etc.)
	 *
	 * \return The vision_object_s_t object corresponding to the given size id, or
	 * PROS_ERR if an error occurred.
	 * 
	 * \b Example
 	 * \code
 	 * #define VISION_PORT 1
 	 * 
 	 * void opcontrol() {
	 * pros::Vision vision_sensor(VISION_PORT);
 	 * while (true) {
 	 *   vision_object_s_t rtn = vision_sensor.get_by_size(0);
 	 *   // Gets the largest object
 	 *   printf("sig: %d", rtn.signature);
 	 *   delay(2);
 	 *   }
 	 * }
 	 * \endcode
	 */
	vision_object_s_t get_by_size(const std::uint32_t size_id) const;

	/**
	 * Gets the nth largest object of the given signature according to size_id.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a vision sensor
	 * EDOM - size_id is greater than the number of available objects.
	 * EINVAL - sig_id is outside the range [1-8]
	 * EAGAIN - Reading the vision sensor failed for an unknown reason.
	 *
	 * \param size_id
	 *        The object to read from a list roughly ordered by object size
	 *        (0 is the largest item, 1 is the second largest, etc.)
	 * \param signature
	 *        The vision_signature_s_t signature for which an object will be
	 *        returned.
	 *
	 * \return The vision_object_s_t object corresponding to the given signature
	 * and size_id, or PROS_ERR if an error occurred.
	 * 
	 * \b Example
 	 * \code
 	 * #define VISION_PORT 1
 	 * #define EXAMPLE_SIG 1
 	 * 
 	 * void opcontrol() {
	 * pros::Vision vision_sensor(VISION_PORT);
 	 * while (true) {
 	 *   vision_object_s_t rtn = vision_sensor.get_by_sig(0, EXAMPLE_SIG);
 	 *   // Gets the largest object of the EXAMPLE_SIG signature
 	 *   printf("sig: %d", rtn.signature);
 	 *   // Prints "sig: 1"
 	 *   delay(2);
 	 *   }
 	 * }
 	 * \endcode
	 */
	vision_object_s_t get_by_sig(const std::uint32_t size_id, const std::uint32_t sig_id) const;

	/**
	 * Gets the nth largest object of the given color code according to size_id.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a vision sensor
	 * EAGAIN - Reading the Vision Sensor failed for an unknown reason.
	 *
	 * \param size_id
	 *        The object to read from a list roughly ordered by object size
	 *        (0 is the largest item, 1 is the second largest, etc.)
	 * \param color_code
	 *        The vision_color_code_t for which an object will be returned
	 *
	 * \return The vision_object_s_t object corresponding to the given color code
	 * and size_id, or PROS_ERR if an error occurred.
	 * 
	 * \b Example
 	 * \code
 	 * #define VISION_PORT 1
 	 * #define EXAMPLE_SIG 1
 	 * #define OTHER_SIG 2
 	 * 
 	 * void opcontrol() {
	 * 	 pros::Vision vision_sensor(VISION_PORT);
 	 *   vision_color_code_t code1 = vision_sensor.create_color_code(EXAMPLE_SIG, OTHER_SIG);
 	 *   while (true) {
 	 *     vision_object_s_t rtn = vision_sensor.get_by_code(0, code1);
 	 *     // Gets the largest object
 	 *     printf("sig: %d", rtn.signature);
 	 *     delay(2);
 	 *   }
 	 * }
 	 * \endcode
	 */
	vision_object_s_t get_by_code(const std::uint32_t size_id, const vision_color_code_t color_code) const;

	/**
	 * Gets the exposure parameter of the Vision Sensor. 
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \return The current exposure parameter from [0,150],
	 * PROS_ERR if an error occurred
	 * 
	 * \b Example
 	 * \code
 	 * #define VISION_PORT 1
 	 * 
 	 * void initialize() {
	 *   pros::Vision vision_sensor(VISION_PORT);
 	 *   if (vision_sensor.get_exposure() < 50)
 	 *   vision_sensor.set_exposure(50);
 	 * }
 	 * \endcode
	 */
	std::int32_t get_exposure(void) const;

	/**
	 * Gets the number of objects currently detected by the Vision Sensor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \return The number of objects detected on the specified vision sensor.
	 * Returns PROS_ERR if the port was invalid or an error occurred.
	 * 
	 * \b Example
 	 * \code
 	 * #define VISION_PORT 1
 	 * 
 	 * void opcontrol() {
	 *   pros::Vision vision_sensor(VISION_PORT);
 	 *   while (true) {
 	 *     printf("Number of Objects Detected: %d\n", vision_sensor.get_object_count());
 	 *     delay(2);
 	 *   }
 	 * }
 	 * \endcode	
	 */
	std::int32_t get_object_count(void) const;

	/**
	 * Gets the object detection signature with the given id number.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \param signature_id
	 *        The signature id to read
	 *
	 * \return A vision_signature_s_t containing information about the signature.
	 * 
	 * \b Example
 	 * \code
 	 * #define VISION_PORT 1
 	 * #define EXAMPLE_SIG 1
 	 * 
 	 * void opcontrol() {
	 *   pros::Vision vision_sensor(VISION_PORT);
 	 *   vision_signature_s_t sig = vision_sensor.get_signature(EXAMPLE_SIG);
 	 *   vision_sensor.print_signature(sig);
 	 * }
	* \endcode
	 */
	vision_signature_s_t get_signature(const std::uint8_t signature_id) const;

	/**
	 * Get the white balance parameter of the Vision Sensor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \return The current RGB white balance setting of the sensor
	 * 
	 * \b Example
 	 * \code
 	 * #define VISION_PORT 1
 	 * #define VISION_WHITE 0xff
	 * 
 	 * void initialize() {
	 * 	 pros::Vision vision_sensor(VISION_PORT);
 	 *   if (vision_sensor.get_white_balance() != VISION_WHITE)
 	 *   vision_sensor.set_white_balance(VISION_WHITE);
 	 * }
 	 * \endcode
	 */
	std::int32_t get_white_balance(void) const;


	/**
	 * Reads up to object_count object descriptors into object_arr.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a vision sensor
	 * EDOM - size_id is greater than the number of available objects.
	 * EAGAIN - Reading the vision sensor failed for an unknown reason.
	 *
	 * \param size_id
	 *        The object to read from a list roughly ordered by object size
	 *        (0 is the largest item, 1 is the second largest, etc.)
	 * \param object_count
	 *        The number of objects to read
	 * \param[out] object_arr
	 *             A pointer to copy the objects into
	 *
	 * \return The number of object signatures copied. This number will be less than
	 * object_count if there are fewer objects detected by the vision sensor.
	 * Returns PROS_ERR if the port was invalid, an error occurred, or fewer objects
	 * than size_id were found. All objects in object_arr that were not found are
	 * given VISION_OBJECT_ERR_SIG as their signature.
	 * 
	 * \b Example
 	 * \code
 	 * #define VISION_PORT 1
	  * #define NUM_VISION_OBJECTS 4
 	 * 
 	 * void opcontrol() {
	 *   pros::Vision vision_sensor(VISION_PORT);
 	 *   vision_object_s_t object_arr[NUM_VISION_OBJECTS];
 	 *   while (true) {
 	 *     vision_sensor.read_by_size(0, NUM_VISION_OBJECTS, object_arr);
 	 *     printf("sig: %d", object_arr[0].signature);
 	 *     // Prints the signature of the largest object found
 	 *     delay(2);
 	 *   }
 	 * }
 	 * \endcode
	 */
	std::int32_t read_by_size(const std::uint32_t size_id, const std::uint32_t object_count,
	                          vision_object_s_t* const object_arr) const;

	/**
	 * Reads up to object_count object descriptors into object_arr.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a vision sensor
	 * EDOM - size_id is greater than the number of available objects.
	 * EINVAL - sig_id is outside the range [1-8]
	 * EAGAIN - Reading the vision sensor failed for an unknown reason.
	 *
	 * \param object_count
	 *        The number of objects to read
	 * \param size_id
	 *        The object to read from a list roughly ordered by object size
	 *        (0 is the largest item, 1 is the second largest, etc.)
	 * \param signature
	 *        The vision_signature_s_t signature for which an object will be
	 *        returned.
	 * \param[out] object_arr
	 *             A pointer to copy the objects into
	 *
	 * \return The number of object signatures copied. This number will be less than
	 * object_count if there are fewer objects detected by the vision sensor.
	 * Returns PROS_ERR if the port was invalid, an error occurred, or fewer objects
	 * than size_id were found. All objects in object_arr that were not found are
	 * given VISION_OBJECT_ERR_SIG as their signature.
	 * 
	 * \b Example
 	 * \code
 	 * #define VISION_PORT 1
 	 * #define EXAMPLE_SIG 1
 	 * #define NUM_VISION_OBJECTS 4
 	 * 
 	 * void opcontrol() {
	 *   pros::Vision vision_sensor(VISION_PORT);
 	 *   vision_object_s_t object_arr[NUM_VISION_OBJECTS];
 	 *   while (true) {
 	 *     vision_sensor.read_by_sig(0, EXAMPLE_SIG, NUM_VISION_OBJECTS, object_arr);
 	 *     printf("sig: %d", object_arr[0].signature);
	 *     // Prints "sig: 1"
	 *     delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t read_by_sig(const std::uint32_t size_id, const std::uint32_t sig_id, const std::uint32_t object_count,
	                         vision_object_s_t* const object_arr) const;

	/**
	 * Reads up to object_count object descriptors into object_arr.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EDOM - size_id is greater than the number of available objects.
	 * ENODEV - The port cannot be configured as a vision sensor
	 * EAGAIN - Reading the vision sensor failed for an unknown reason.
	 *
	 * \param object_count
	 *        The number of objects to read
	 * \param size_id
	 *        The object to read from a list roughly ordered by object size
	 *        (0 is the largest item, 1 is the second largest, etc.)
	 * \param color_code
	 *        The vision_color_code_t for which objects will be returned
	 * \param[out] object_arr
	 *             A pointer to copy the objects into
	 *
	 * \return The number of object signatures copied. This number will be less than
	 * object_count if there are fewer objects detected by the vision sensor.
	 * Returns PROS_ERR if the port was invalid, an error occurred, or fewer objects
	 * than size_id were found. All objects in object_arr that were not found are
	 * given VISION_OBJECT_ERR_SIG as their signature.
	 * 
	 * \b Example
 	 * \code
	 * #define VISION_PORT 1
	 * #define EXAMPLE_SIG 1
	 * #define OTHER_SIG 2
	 * #define NUM_VISION_OBJECTS 4
	 * 
	 * void opcontrol() {
	 *   pros::Vision vision_sensor(VISION_PORT);
	 *   vision_object_s_t object_arr[NUM_VISION_OBJECTS];
	 *   vision_color_code_t code1 = vision_sensor.create_color_code(EXAMPLE_SIG, OTHER_SIG, 0, 0, 0);
	 *   while (true) {
	 *     vision_sensor.read_by_code(0, code1, NUM_VISION_OBJECTS, object_arr);
	 *     printf("sig: %d", object_arr[0].signature);
	 *     // Prints the signature of the largest object found
	 *     delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	int32_t read_by_code(const std::uint32_t size_id, const vision_color_code_t color_code,
	                     const std::uint32_t object_count, vision_object_s_t* const object_arr) const;

	/**
	 * Prints the contents of the signature as an initializer list to the terminal.
	 *
	 * \param sig
	 *        The signature for which the contents will be printed
	 *
	 * \return 1 if no errors occured, PROS_ERR otherwise
	 * 
	 * \b Example
	 * \code
 	 * #define VISION_PORT 1
	 * #define EXAMPLE_SIG 1
	 * 
	 * void opcontrol() {
	 *   pros::Vision vision_sensor(VISION_PORT);
	 *   vision_signature_s_t sig = visionsensor.get_signature(EXAMPLE_SIG);
	 *   vision_print_signature(sig);
	 * }
	 * \endcode
	 */
	static std::int32_t print_signature(const vision_signature_s_t sig);

	/**
	 * Enables/disables auto white-balancing on the Vision Sensor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \param enabled
	 * 		    Pass 0 to disable, 1 to enable
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 * 
	 * \b Example
 	 * \code
 	 * #define VISION_PORT 1
	 * 
	 * void initialize() {
	 * 	 pros::Vision vision_sensor(VISION_PORT);
	 *   vision_sensor.set_auto_white_balance(true);
	 * }
	 * \endcode
	 */
	std::int32_t set_auto_white_balance(const std::uint8_t enable) const;

	/**
	 * Sets the exposure parameter of the Vision Sensor. 
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \param percent
	 *        The new exposure setting from [0,150].
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 * 
	 * \b Example
	 * \code
	 * #define VISION_PORT 1
	 * 
	 * void initialize() {
	 * 	 pros::Vision vision_sensor(VISION_PORT);
	 *   if (vision_sensor.get_exposure() < 50)
	 *   vision_sensor.set_exposure(50);
	 * }
	 * \endcode
	 */
	std::int32_t set_exposure(const std::uint8_t exposure) const;

	/**
	 * Sets the vision sensor LED color, overriding the automatic behavior.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \param rgb
	 *        An RGB code to set the LED to
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 * 
	 * \b Example
	 * \code
	 * #define VISION_PORT 1
	 * 
	 * void initialize() {
	 *	 pros::Vision vision_sensor(VISION_PORT);
	 *   vision_sensor.set_led(COLOR_BLANCHED_ALMOND);
	 * }
	 * \endcode
	 */
	std::int32_t set_led(const std::int32_t rgb) const;

	/**
	 * Stores the supplied object detection signature onto the vision sensor.
	 *
	 * NOTE: This saves the signature in volatile memory, and the signature will be
	 * lost as soon as the sensor is powered down.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a vision sensor
	 * EINVAL - sig_id is outside the range [1-8]
	 *
	 * \param signature_id
	 *        The signature id to store into
	 * \param[in] signature_ptr
	 *            A pointer to the signature to save
	 *
	 * \return 1 if no errors occured, PROS_ERR otherwise
	 * 
	 *  \b Example
	 * \code
	 * #define VISION_PORT 1
	 * #define EXAMPLE_SIG 1
	 * 
	 * void opcontrol() {
	 *   pros::Vision vision_sensor(VISION_PORT);
	 *   vision_signature_s_t sig = vision_sensor.get_signature(EXAMPLE_SIG);
	 *   sig.range = 10.0;
	 *   vision_sensor.set_signature(EXAMPLE_SIG, &sig);
	 * }
	 * \endcode
	 */
	std::int32_t set_signature(const std::uint8_t signature_id, vision_signature_s_t* const signature_ptr) const;

	/**
	 * Sets the white balance parameter of the Vision Sensor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \param rgb
	 *        The new RGB white balance setting of the sensor
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 * 
	 * \b Example
 	 * \code
	 * #define VISION_PORT 1
	 * #define VISION_WHITE 0xff
	 * 
	 * void initialize() {
	 *   pros::Vision vision_sensor(VISION_PORT);
	 *   vision_sensor.set_white_balance(VISION_WHITE);
	 * }
	 * \endcode
	 */
	std::int32_t set_white_balance(const std::int32_t rgb) const;

	/**
	 * Sets the (0,0) coordinate for the Field of View.
	 *
	 * This will affect the coordinates returned for each request for a
	 * vision_object_s_t from the sensor, so it is recommended that this function
	 * only be used to configure the sensor at the beginning of its use.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \param zero_point
	 *        One of vision_zero_e_t to set the (0,0) coordinate for the FOV
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 * 
	 * \b Example
 	 * \code
	 * #define VISION_PORT 1
	 * 
	 * void initialize() {
	 * 	 pros::Vision vision_sensor(VISION_PORT);
	 *   vision_sensor.set_zero_point(E_VISION_ZERO_CENTER);
	 * }
	 * \endcode
	 */
	std::int32_t set_zero_point(vision_zero_e_t zero_point) const;

	/**
	 * Sets the Wi-Fi mode of the Vision sensor
	 *
	 * This functions uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \param enable
	 *        Disable Wi-Fi on the Vision sensor if 0, enable otherwise (e.g. 1)
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 * 
	 *  \b Example
 	 * \code
 	 * #define VISION_PORT 1
 	 * 
 	 * void initialize() {
	 * 	 pros::Vision vision_sensor(VISION_PORT);
 	 *   vision_sensor.set_wifi_mode(0);
 	 * }
 	 * \endcode
	 */
	std::int32_t set_wifi_mode(const std::uint8_t enable) const;
  
	private:
	///@}
};
}  // namespace v5
}  // namespace pros
#endif  // _PROS_VISION_HPP_
