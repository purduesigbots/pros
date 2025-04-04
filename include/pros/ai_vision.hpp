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
#include <type_traits>
#include <expected>

#include "pros/ai_vision.h"
#include "pros/device.hpp"

namespace pros {
inline namespace v5 {

/**
 * \enum AivisionDetectType
 * \ingroup cpp-aivision
 * Enum class for describing detection type of objects detected by the AI Vision Sensor.
 */
enum class AivisionDetectType : uint8_t {
	color = (1 << 0), /**< object was detected based on color descriptor */
	code = (1 << 1), /**< object was detected based on code descriptor */
	object = (1 << 2), /**< object was detected using AI model */
	tag = (1 << 3) /**< object was detected as an AprilTag */
};

/**
 * \enum AivisionModeType
 * \ingroup cpp-aivision
 * Enum class for enabling/disabling detection types of AI Vision Sensor.
 */
enum class AivisionModeType : uint8_t {
	tags = (1 << 0), /**< AprilTag detection */
	colors = (1 << 1), /**< color and code detection */
	objects = (1 << 2), /**< AI model object detection */
	color_merge = (1 << 4), /**< merge adjacent color detections */
	all = (1 << 0) | (1 << 1) | (1 << 2),
};

/**
 * \enum AivisionTagFamily
 * \ingroup cpp-aivision
 * Enum class for describing family of apriltags to detect.
 */
enum class AivisionTagFamily { tag_21H7 = 0, tag_16H5 = 1, tag_25H9 = 2, tag_61H11 = 3 };

/**
 * \ingroup cpp-aivision
 */
class AIVision : public Device {
	/**
	 * \addtogroup cpp-aivision
	 *  @{
	 */
	public:

	using Color = aivision_color_s_t;
	using Code = aivision_code_s_t;
	using Object = aivision_object_s_t;

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
	explicit AIVision(const std::uint8_t port);

	AIVision(const Device& device) : AIVision(device.get_port()){};

	/**
	 * Gets all vision sensors.
	 *
	 * \return A vector of AIVision sensor objects.
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   std::vector<AIVision> aivision_all = pros::AIVision::get_all_devices();  // All AI vision sensors that are
	 * connected
	 * }
	 * \endcode
	 */
	static std::vector<AIVision> get_all_devices();

	/**
	 * Check if the dected type is the same as the given type.
	 *
	 * \return true if the type is the same, false otherwise
	 *
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::AIVision aivision(1);
	 *   pros::AIVision::Object object = aivision.get_object(0);
	 *   if (AIVision::is_type(AivisionDetectType::color, object)) {
	 *       printf("is color\n");
	 *   } else if (AIVision::is_type(AivisionDetectType::object, object)) {
	 *       printf("is object\n");
	 *   } else if (AIVision::is_type(AivisionDetectType::code, object)) {
	 *       printf("is code\n");
	 *   } else if (AIVision::is_type(AivisionDetectType::tag, object)) {
	 *       printf("is tag\n");
	 *   } else {
	 *       printf("unknown\n");
	 *   }
	 * }
	 * \endcode
	 */
	static bool is_type(const Object& object, AivisionDetectType type);

	/**
	 * Resets the AI Vision sensor to the initial state.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a vision sensor
	 *

	 *
	 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * #define AIVISION_PORT 1
	 * void initialize() {
	 *   pros::AIVision aivision(AIVISION_PORT);
	 *   aivision.reset();
	 * }
	 * \endcode
	 */
	int32_t reset();

	/**
	 * Returns a bitfield of the types of objects the AI vision sensor is currently searching for,
	 * as per AivisionModeType.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a vision sensor
	 *

	 * \return the bitfield if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * #define AIVISION_PORT 1
	 * void initialize() {
	 *   pros::AIVision aivision(AIVISION_PORT);
	 *   int32_t enabled_types = aivision.get_enabled_detection_types();
	 *   printf("is tag: %d\n", enabled_types | AivisionModeType::tags);
	 * }
	 * \endcode
	 */
	int32_t get_enabled_detection_types();

	/**
	 * Enable detecting these types of objects, a bitmask as per aivision_mode_type_e_t.
	 * Enabling any given type of object will not disable the detection of other objects.
	 * This must be done explicitly.
	 *
	 * For this function you must use bitwise or to combine the types you want to enable.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \b Example
	 * \code
	 * #define AIVISION_PORT 1
	 * void initialize() {
	 *   pros::AIVision aivision(AIVISION_PORT);
	 *   // start or continue looking for AI model objects
	 *   // enable aivision to look for tags and objects
	 * 	 aivision.enable_detection_types(AivisionModeType::tags | AivisionModeType::objects);
	 * }
	 * \endcode
	 *
	 * \param types_mask The types to enable
	 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	int32_t enable_detection_types(AivisionModeType types_mask);

	/**
	 * Enable detecting these types of objects, a bitmask as per aivision_mode_type_e_t.
	 * Enabling any given type of object will not disable the detection of other objects.
	 * This must be done explicitly.
	 *
	 * For this function you can use comma separated values to combine the types you want to enable.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \b Example
	 * \code
	 * #define AIVISION_PORT 1
	 * void initialize() {
	 *   pros::AIVision aivision(AIVISION_PORT);
	 *   // start or continue looking for AI model objects
	 *   // enable aivision to look for tags and objects
	 * 	 aivision.enable_detection_types(AivisionModeType::tags, AivisionModeType::objects);
	 * }
	 * \endcode
	 *
	 * \param types_mask The types to enable
	 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	template <class... Flags>
	  requires((std::conjunction_v<std::is_same<AivisionModeType, Flags>...>))
	int32_t enable_detection_types(Flags... flags) {
		auto types_mask = (static_cast<uint8_t>(flags) | ...);
		return c::aivision_enable_detection_types(this->_port, static_cast<uint8_t>(types_mask));
	}

	/**
	 * Disable detecting these types of objects, a bitmask as per aivision_mode_type_e_t.
	 * Disabling any given type of object will not affect the detection of other objects.
	 *
	 * For this function you must use bitwise or to combine the types you want to disable.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \b Example
	 * \code
	 * #define AIVISION_PORT 1
	 * void initialize() {
	 *   pros::AIVision aivision(AIVISION_PORT);
	 *   // stop looking for AI model objects (competition elements, for example)
	 *   // disable aivision to look for tags and objects
	 * 	 aivision.disable_detection_types(AivisionModeType::tags | AivisionModeType::objects);
	 * }
	 * \endcode
	 *
	 * \param types_mask The types to enable
	 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	int32_t disable_detection_types(AivisionModeType types_mask);

	/**
	 * Disable detecting these types of objects, a bitmask as per aivision_mode_type_e_t.
	 * Disabling any given type of object will not affect the detection of other objects.
	 *
	 * For this function you can use comma separated values to combine the types you want to disable.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \b Example
	 * \code
	 * #define AIVISION_PORT 1
	 * void initialize() {
	 *   pros::AIVision aivision(AIVISION_PORT);
	 *   // stop looking for AI model objects (competition elements, for example)
	 *   // disable aivision to look for tags and objects
	 * 	 aivision.disable_detection_types(AivisionModeType::tags | AivisionModeType::objects);
	 * }
	 * \endcode
	 *
	 * \param types_mask The types to enable
	 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	template <class... Flags>
	  requires((std::conjunction_v<std::is_same<AivisionModeType, Flags>...>))
	int32_t disable_detection_types(Flags... flags) {
		auto types_mask = (static_cast<uint8_t>(flags) | ...);
		return c::aivision_disable_detection_types(this->_port, static_cast<uint8_t>(types_mask));
	}

	/**
	 * Sets the april tag family to detect.
	 * If override is true, the AI vision sensor will only look for the given family.
	 * Otherwise, it will add the given tag to the list of enabled tags.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \code
	 * #define AIVISION_PORT 1
	 * void initialize() {
	 *   pros::AIVision aivision(AIVISION_PORT);
	 *   // set the only tag family to look for to 21H7
	 *   aivision.set_tag_family(AivisionTagFamily::tag_21H7);
	 *   // add 16H5 to the list of enabled tag families
	 *   aivision.set_tag_family(AivisionTagFamily::tag_16H5);
	 *   // set the only tag family to look for to 25H9
	 *   aivision.set_tag_family(AivisionTagFamily::tag_25H9, true);
	 * }
	 * \endcode
	 *
	 * \param family the tag family to configure the AI Vision sensor to detect
	 * \param override if true, the given family will be set as the only enabled tag family.
	 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	int32_t set_tag_family(AivisionTagFamily family, bool override = false);

	/**
	 * Set a color configuration that the AI vision sensor will detect.
	 * The color detection type must be separately enabled.
	 * If a color with the same ID already is stored in the sensor, it will be overwritten.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \code
	 * #define AIVISION_PORT 1
	 * void opcontrol() {
	 *   pros::AIVision aivision(AIVISION_PORT);
	 *   AIVision::Color color = {1, 207, 19, 25, 10.00, 0.20};
	 *   aivision.set_color(color);
	 * }
	 * \endcode
	 *
	 * \param color the color to configure the AI Vision sensor to detect
	 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno
	 */
	int32_t set_color(const Color& color);

	/**
	 * Get a color configuration that the AI vision sensor has stored.
	 * If you attempt to get a color configuration that has not been previously used, the
	 * behavior is not defined.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 *
	 * #define AIVISION_PORT 1
	 * void opcontrol() {
	 *  pros::AIVision aivision(AIVISION_PORT);
	 *  AIVision::Code color = aivision.get_color(0);
	 *  printf("id: %d, red: %d, green: %d, blue: %d, hue_range: %f, saturation_range: %f\n",
	 *  color.id, color.red, color.green, color.blue, color.hue_range, color.saturation_range);
	 * }
	 *
	 * \param id the id of color from 1-7
	 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno
	 */
	AIVision::Color get_color(uint32_t id);

	/**
	 * Set a code that the AI vision sensor will detect for.
	 * The id of the code is stored in the aivision_code_s_t struct. If there is already a code
	 * stored in the AI vision sensor with the id, this function will overwrite.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \code
	 *
	 * #define AIVISION_PORT 1
	 * void opcontrol() {
	 *  pros::AIVision aivision(AIVISION_PORT);
	 *  AIVision::Code code = {1, 207, 19, 25, 10.00, 0.20};
	 *  aivision.set_code(code);
	 * }
	 *
	 * \endcode
	 *
	 * \param code The code to set
	 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno
	 */
	uint32_t set_code(const Code& code);

	/**
	 * Get a code that the AI vision sensor has stored.
	 *
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \code
	 *
	 * #define AIVISION_PORT 1
	 * void opcontrol() {
	 *  pros::AIVision aivision(AIVISION_PORT);
	 *  AIVision::Code code = aivision.get_code(0);
	 *  printf("id: %d, length: %d, c1: %d, c2: %d, c3: %d, c4: %d, c5: %d\n",
	 *  code.id, code.length, code.c1, code.c2, code.c3, code.c4, code.c5);
	 *  )
	 * }
	 *
	 * \endcode
	 *
	 * \param id The id from 1-5
	 * \return the code, or a struct with an invalid ID if the operation failed, setting errno
	 */
	AIVision::Code get_code(uint32_t id);

	/**
	 * Runs auto white balance to adjust to different lighting conditions.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno
	 */
	int32_t start_awb();

	/**
	 * Get a class name that the AI vision sensor has stored.
	 * The AI Vision sensor may not correctly report classnames for the first several hundred milliseconds
	 * of being plugged in.
	 * By passing in -1 for the id, the function will return the number of class names the AI vision sensor reports.
	 * For other values of id, the function return value is undefined
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a vision sensor
	 * \code
	 * #define AIVISION_PORT 1
	 * void opcontrol() {
	 *  pros::AIVision aivision(AIVISION_PORT);
	 *  char* class_name = new char[21];
	 *  aivision.get_class_name(0, class_name);
	 *  printf("%s\n", class_name);
	 *  delete[] class_name;
	 * }
	 *
	 * \endcode
	 *
	 * \param id the id of the class name from 0-(AIVISION_MAX_CLASSNAME_COUNT - 1)
	 * \param class_name a string of length >=20 to store the classname.
	 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno
	 */
	int32_t get_class_name(int32_t id, char* class_name);

	/**
	 * Get a class name that the AI vision sensor has stored.
	 * The AI Vision sensor may not correctly report classnames for the first several hundred milliseconds
	 * of being plugged in.
	 * By passing in -1 for the id, the function will return the number of class names the AI vision sensor reports.
	 * For other values of id, the function return value is undefined
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a vision sensor
	 * \code
	 * #define AIVISION_PORT 1
	 * void opcontrol() {
	 *  pros::AIVision aivision(AIVISION_PORT);
	 *  auto name = aivision.get_class_name(1);
	 *
	 *  if(name.has_value()) {
	 *	  printf("Class name: %s\n", name.value().c_str());
	 *  } else {
	 *	  printf("Error: %ld\n", errno);
	 *  }
	 * }
	 *
	 * \endcode
	 *
	 * \param id the id of the class name from 0-(AIVISION_MAX_CLASSNAME_COUNT - 1)
	 * \return the class name string in std::optional if the operation was successful
	 * or an empty optional if the operation failed, setting errno
	 */
	std::optional<std::string> get_class_name(int32_t id);

	/**
	 * Get the current number of objects detected by the AI vision sensor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \code
	 * #define AIVISION_PORT 1
	 * void opcontrol() {
	 *  pros::AIVision aivision(AIVISION_PORT);
	 *  int32_t object_count = aivision.get_object_count();
	 *  printf("%d\n", object_count);
	 * }
	 * \endcode
	 *
	 * \return the number of objects if the operation was successful or PROS_ERR if the operation failed, setting errno
	 */
	int32_t get_object_count();

	/**
	 * Get the detected object at a given object index; there are aivision_get_object_count objects and the index starts
	 * from 0.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \code
	 * #define AIVISION_PORT 1
	 * void opcontrol() {
	 *  pros::AIVision aivision(AIVISION_PORT);
	 *  int32_t object_count = aivision.get_object_count();
	 *  for (int i = 0; i < object_count; i++) {
	 *  	pros::AIVision::Object object = aivision.get_object(i);
	 *  	printf("Object %d: %d\n", i, object.type);
	 *  }
	 * }
	 *
	 * \endcode
	 *
	 * @param object_index the object index
	 * @return the detected object if the operation was successful or an invalid object type if the operation failed,
	 * setting errno
	 */
	Object get_object(uint32_t object_index);

	/**
	 * Get all detected objects in a vector.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a vision sensor
	 *
	 * \code
	 * #define AIVISION_PORT 1
	 * void opcontrol() {
	 *  pros::AIVision aivision(AIVISION_PORT);
	 *  auto objects = aivision.get_all_objects();
	 *  for (const auto& object : objects) {
	 *    printf("Object %d: %d\n", object.id, object.type);
	 *  }
	 * }
	 * \endcode
	 *
	 * @return a vector of all detected objects
	 */
	std::vector<Object> get_all_objects();
	/// @}
};
}  // namespace v5
}  // namespace pros
#endif  // _PROS_VISION_HPP_
