/**
* \file pros/aivision.hpp
* \ingroup cpp-aivision
*
* Contains prototypes for the VEX AI Vision Sensor-related functions in C++.
*
* This file should not be modified by users, since it gets replaced whenever
* a kernel upgrade occurs.
*
* \copyright (c) 2017-2024, Purdue University ACM SIGBots.
* All rights reserved.
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*
* \defgroup cpp-aivision AI Vision Sensor C++ API
* \note Additional example code for this module can be found in its [Tutorial.](@ref aivision)
*/

#ifndef _PROS_AIVISION_HPP_
#define _PROS_AIVISION_HPP_

#include <cstdint>

#include "pros/aivision.h"
#include "pros/device.hpp"

namespace pros {
inline namespace v5 {
/**
* \ingroup cpp-aivision
*/
class AIVision : public Device {
	/**
	* \addtogroup cpp-aivision
	*  @{
	 */
	public:
	/**
	* Create a AI Vision Sensor object on the given port.
	*
	* This function uses the following values of errno when an error state is
	* reached:
	* ENXIO - The given value is not within the range of V5 ports (1-21).
	* ENODEV - The port cannot be configured as an AI vision sensor
	*
	* \param port
	*        The V5 port number from 1-21
	*
	* \b Example
	* \code
	* void opcontrol() {
	*   pros::AIVision ai_sensor(2); // Creates a vision sensor on port two
	* }
	* \endcode
	 */
	AIVision(const std::uint8_t port);

	AIVision(const Device& device) : AIVision(device.get_port()){};

	/**
	 * Gets all vision sensors.
	 *
	 * \return A vector of AIVision sensor objects.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   std::vector<AIVision> aivision_all = pros::AIVision::get_all_devices();  // All AI vision sensors that are connected
	 * }
	 * \endcode
	 */
	static std::vector<AIVision> get_all_devices();

	int32_t reset();
	int32_t get_enabled_detection_types();
	int32_t set_enabled_detection_types(uint8_t bits, uint8_t types_mask);
	int32_t enable_detection_types(uint8_t types_mask);
	int32_t disable_detection_types(uint8_t types_mask);
	int32_t set_tag_family(aivision_tag_family_e_t family);
	int32_t set_color(aivision_color_s_t &color);
	aivision_color_s_t get_color(uint32_t id);
	uint32_t get_class_name(int32_t id, uint8_t * class_name);
	aivision_code_s_t get_code(uint32_t id);
	uint32_t set_code(aivision_code_s_t &code);
	int32_t get_object_count();
	aivision_object_s_t get_object(uint32_t object_index);
	std::vector<aivision_object_s_t> get_all_objects();





};
}  // namespace literals
}  // namespace pros
#endif  // _PROS_VISION_HPP_
