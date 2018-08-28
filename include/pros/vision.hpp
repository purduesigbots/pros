/**
 * \file pros/vision.hpp
 *
 * Contains prototypes for the VEX Vision Sensor-related functions in C++.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/vision.html to learn
 * more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_VISION_HPP_
#define _PROS_VISION_HPP_

#include "pros/vision.h"

#include <cstdint>

namespace pros {
class Vision {
	public:
	/**
	 * Create a Vision Sensor object on the given port.
	 *
	 * \param port
	 *        The V5 port number from 1-21
	 * \param zero_point
	 *        One of vision_zero_e_t to set the (0,0) coordinate for the FOV
	 */
	Vision(std::uint8_t port, vision_zero_e_t zero_point = E_VISION_ZERO_TOPLEFT);

	/**
	 * Clears the vision sensor LED color, reseting it back to its default
	 * behavior, displaying the most prominent object signature color.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the port.
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t clear_led(void) const;

	/**
	 * Gets the nth largest object according to size_id.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the port.
	 *
	 * \param size_id
	 *        The object to read from a list roughly ordered by object size
	 *        (0 is the largest item, 1 is the second largest, etc.)
	 *
	 * \return The vision_object_s_t object corresponding to the given size id, or
	 * PROS_ERR if an error occurred.
	 */
	vision_object_s_t get_by_size(const std::uint32_t size_id) const;

	/**
	 * Gets the nth largest object of the given signature according to size_id.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the port.
	 * EAGAIN - Reading the Vision Sensor failed for an unknown reason.
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
	 */
	vision_object_s_t get_by_sig(const std::uint32_t size_id, const std::uint32_t sig_id) const;

	/**
	 * Gets the exposure parameter of the Vision Sensor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the port.
	 *
	 * \return The current exposure percentage parameter from [0,100],
	 * PROS_ERR if an error occurred
	 */
	std::int32_t get_exposure(void) const;

	/**
	 * Gets the number of objects currently detected by the Vision Sensor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the port.
	 *
	 * \return The number of objects detected on the specified vision sensor.
	 * Returns PROS_ERR if the port was invalid or an error occurred.
	 */
	std::int32_t get_object_count(void) const;

	/**
	 * Get the white balance parameter of the Vision Sensor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the port.
	 *
	 * \return The current RGB white balance setting of the sensor
	 */
	std::int32_t get_white_balance(void) const;

	/**
	 * Reads up to object_count object descriptors into object_arr.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - Fewer than object_count number of objects were found.
	 * EACCES - Another resource is currently trying to access the port.
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
	 */
	std::int32_t read_by_size(const std::uint32_t size_id, const std::uint32_t object_count,
	                          vision_object_s_t* const object_arr) const;

	/**
	 * Reads up to object_count object descriptors into object_arr.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - Fewer than object_count number of objects were found.
	 * EACCES - Another resource is currently trying to access the port.
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
	 */
	std::int32_t read_by_sig(const std::uint32_t size_id, const std::uint32_t sig_id, const std::uint32_t object_count,
	                         vision_object_s_t* const object_arr) const;

	/**
	 * Enables/disables auto white-balancing on the Vision Sensor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the port.
	 *
	 * \param enabled
	 * 		    Pass 0 to disable, 1 to enable
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t set_auto_white_balance(const std::uint8_t enable) const;

	/**
	 * Sets the exposure parameter of the Vision Sensor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the port.
	 *
	 * \param percent
	 *        The new exposure percentage from [0,100]
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t set_exposure(const std::uint8_t percent) const;

	/**
	 * Sets the vision sensor LED color, overriding the automatic behavior.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the port.
	 *
	 * \param rgb
	 *        An RGB code to set the LED to
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t set_led(const std::int32_t rgb) const;

	/**
	 * Sets the white balance parameter of the Vision Sensor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the port.
	 *
	 * \param rgb
	 *        The new RGB white balance setting of the sensor
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
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
	 * EACCES - Another resource is currently trying to access the port.
	 *
	 * \param zero_point
	 *        One of vision_zero_e_t to set the (0,0) coordinate for the FOV
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t set_zero_point(vision_zero_e_t zero_point) const;

	private:
	std::uint8_t _port;
};
}  // namespace pros
#endif  // _PROS_VISION_HPP_
