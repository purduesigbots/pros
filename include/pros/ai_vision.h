/**
 * \file pros/aivision.h
 * \ingroup c-aivision
 *
 * Contains prototypes for the VEX AI Vision Sensor-related functions.
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
 * \defgroup c-aivision AI Vision Sensor C API
 * \note Additional example code for this module can be found in its [Tutorial.](@ref aivision)
 */

#ifndef _PROS_AIVISION_H_
#define _PROS_AIVISION_H_

/**
 * \addtogroup c-aivision
 *  @{
 */

/// \name Macros
/// Parameters given by VEX
///@{

#define AIVISION_MAX_OBJECT_COUNT 24
#define AIVISION_MAX_CLASSNAME_COUNT 20
#define AIVISION_MODE_TAG_SET_BIT (1 << 29)

///@}

///@}

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
namespace pros {
#endif

/**
 * \addtogroup c-aivision
 *  @{
 */

/**
 * \enum aivision_detected_type_e_t
 * This enumeration defines what kind of object is stored inside the union in aivision_object_s
 */
typedef enum aivision_detected_type {
	E_AIVISION_DETECTED_COLOR = (1 << 0),
	E_AIVISION_DETECTED_CODE = (1 << 1),
	E_AIVISION_DETECTED_OBJECT = (1 << 2),
	E_AIVISION_DETECTED_TAG = (1 << 3)
} aivision_detected_type_e_t;

/**
 * \enum aivision_mode_type_e_t
 * This enumeration defines what kinds of objects the ai vision sensor will scan for:
 * tags (april tags), colors (user defined colors), and objects (game elements), and all (all objects)
 */

typedef enum aivision_mode_type {
	E_AIVISION_MODE_TAGS = (1 << 0),
	E_AIVISION_MODE_COLORS = (1 << 1),
	E_AIVISION_MODE_OBJECTS = (1 << 2),
	E_AIVISION_MODE_COLOR_MERGE = (1 << 4),
	E_AIVISION_MODE_ALL = (1 << 0) | (1 << 1) | (1 << 2),
} aivision_mode_type_e_t;

/**
 * \struct aivision_color_s_t
 * This structure contains the parameters used by the AI Vision Sensor to define a color. hue_range and saturation_range
 * are ranges for hue and saturation that are acceptable.
 * For example, if a large hue range is specified for a blue color, colors that are more magenta or teal may be detected
 * as "blue".
 */
typedef struct aivision_color_s {
	uint8_t id;             /**< id of color descriptor, can range from 1-7 */
	uint8_t red;            /**< red value of color */
	uint8_t green;          /**< green value of color */
	uint8_t blue;           /**< blue value of color */
	float hue_range;        /**< range by which detected color's hue can vary from the base color, can range from 1-40 */
	float saturation_range; /**< range by which detected color's saturation can vary from base color, can range from 0.1-1 */
} aivision_color_s_t;

/**
 * \struct aivision_code_s_t
 * This structure contains the parameters used by the AI Vision sensor to define a code.
 * Codes are a combination of color descriptors, and tells the AI Vision sensor to merge objects
 * close to each other that belong to the given color descriptors into a single object that matches
 * the code descriptor.
 * Codes must use at least 2, and no greater than 5, color descriptors.
 */
typedef struct aivision_code_s {
	uint8_t id;     /**< id of code descriptor, can range from 1-5 */
	uint8_t length; /**< number of color descriptors used by this code. */
	int16_t c1;     /**< id of first color descriptor */
	int16_t c2;     /**< id of second color descriptor */
	int16_t c3;     /**< id of third color descriptor */
	int16_t c4;     /**< id of fourth color descriptor */
	int16_t c5;     /**< id of fifth color descriptor */
} aivision_code_s_t;

/**
 * \enum aivision_tag_family_e_t
 * This enumeration corresponds to a family of AprilTags.
 * \see https://april.eecs.umich.edu/software/apriltag
 */
typedef enum aivision_tag_family_e {
	TAG_CIRCLE_21H7 = 0,
	TAG_16H5 = 1,
	TAG_25H9 = 2,
	TAG_61H11 = 3
} aivision_tag_family_e_t;

/**
 * \struct aivision_object_color_s_t
 * This structure contains a detected color.
 */
typedef struct __attribute__((packed)) aivision_object_color_s {
	uint16_t xoffset;  // left edge (from camera's view)
	uint16_t yoffset;  // top edge
	uint16_t width;
	uint16_t height;
	uint16_t angle;  // angle, in tenths of a degree
} aivision_object_color_s_t;

/**
 * \struct aivision_object_tag_s_t
 * This structure contains a detected tag.
 */
typedef struct __attribute__((packed)) aivision_object_tag_s {
	int16_t x0;
	int16_t y0;
	int16_t x1;
	int16_t y1;
	int16_t x2;
	int16_t y2;
	int16_t x3;
	int16_t y3;
} aivision_object_tag_s_t;

typedef struct __attribute__((packed)) aivision_object_element_s {
	uint16_t xoffset;  // left
	uint16_t yoffset;  // top
	uint16_t width;
	uint16_t height;
	uint16_t score;  // confidence that this struct is
} aivision_object_element_s_t;
/**
 * \struct aivision_object_s_t
 * This structure contains one of aivision_detected_type_e_t, stored in type
 *
 * If the object is a color, id stores the color's id
 * If the object is an April Tag, id stores the tag's id
 * If the object is an AI model element, id stores the element id as per
 * https://api.vex.com/v5/home/cpp/AiVision/AiObjdesc.html
 */
typedef struct __attribute__((packed)) aivision_object_s {
	uint8_t id;    // object id
	uint8_t type;  // object type
	union {
		aivision_object_color_s_t color;
		aivision_object_tag_s_t tag;
		aivision_object_element_s_t element;
	} object;
} aivision_object_s_t;
/// @}

#ifdef __cplusplus
namespace c {
#endif

/**
 * \addtogroup c-aivision
 *  @{
 */
/// \name Functions

/**
 * Resets the AI Vision sensor to the initial state.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 *
 * \param port The V5 port number from 1-21
 *
 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 *
 * \b Example
 * \code
 * #define AIVISION_PORT 1
 * void initialize() {
 *   aivision_reset(AIVISION_PORT);
 * }
 * \endcode
 */
int32_t aivision_reset(uint8_t port);

/**
 * Returns a bitfield of the types of objects the AI vision sensor is currently searching for,
 * as per aivision_mode_type_e_t.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 *
 * \param port The V5 port number from 1-21
 * \return the bitfield if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 *
 * \b Example
 * \code
 * #define AIVISION_PORT 1
 * void initialize() {
 *   aivision_get_enabled_detection_types(AIVISION_PORT);
 * }
 * \endcode
 */
int32_t aivision_get_enabled_detection_types(uint8_t port);

/**
 * Modifies the types of objects the AI vision sensor is currently searching for, as per aivision_mode_type_e_t.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 *
 * \b Example
 * aivision_set_enabled_detection_types(1, 0b010, 0b101) would disable the detection of tags and objects,
 * and leave the setting of colors alone.
 *
 * \param port The V5 port number from 1-21
 * \param bits the bits to set
 * \param bitmask the bitmask to apply
 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t aivision_set_enabled_detection_types(uint8_t port, uint8_t bits, uint8_t bitmask);

/**
 * Enable detecting these types of objects, a bitmask as per aivision_mode_type_e_t.
 * Enabling any given type of object will not disable the detection of other objects.
 * This must be done explicitly.
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
 *   // start or continue looking for AI model objects
 * 	 aivision_enable_detection_types(AIVISION_PORT, aivision_mode_type_e_t::E_AIVISION_MODE_OBJECTS);
 * }
 * \endcode
 *
 * \param port The V5 port number from 1-21
 * \param types_mask The types to enable
 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t aivision_enable_detection_types(uint8_t port, uint8_t types_mask);

/**
 * Disable detecting these types of objects, a bitmask as per aivision_mode_type_e_t.
 * Disabling any given type of object will not affect the detection of other objects.
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
 *   // stop looking for AI model objects (competition elements, for example)
 * 	 aivision_disable_detection_types(AIVISION_PORT, aivision_mode_type_e_t::E_AIVISION_MODE_OBJECTS);
 * }
 * \endcode
 *
 * \param port The V5 port number from 1-21
 * \param types_mask The types to enable
 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t aivision_disable_detection_types(uint8_t port, uint8_t types_mask);

/**
 * Sets the april tag family to detect. Use this function will override the enabled apriltag
 * detection family.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 *
 * \param port The V5 port number from 1-21
 * \param family the tag family to configure the AI Vision sensor to detect
 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t aivision_set_tag_family_override(uint8_t port, aivision_tag_family_e_t family);

/**
 * Sets the april tag family to detect. Use this function will allow multiple apriltags
 * to be detected.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 *
 * \param port The V5 port number from 1-21
 * \param family the tag family to configure the AI Vision sensor to detect
 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t aivision_set_tag_family(uint8_t port, aivision_tag_family_e_t family);

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
 * \param port The V5 port number from 1-21
 * \param color the color to configure the AI Vision sensor to detect
 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
 * failed, setting errno
 */
int32_t aivision_set_color(uint8_t port, const aivision_color_s_t* color);

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
 * \param port The V5 port number from 1-21
 * \param id the id of color from 1-7
 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
 * failed, setting errno
 */
aivision_color_s_t aivision_get_color(uint8_t port, uint32_t id);

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
 *
 * \param port The V5 port number from 1-21
 * \param id the id of the class name from 0-(AIVISION_MAX_CLASSNAME_COUNT - 1)
 * \param class_name a string of length >=20 to store the classname.
 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
 * failed, setting errno
 */
int32_t aivision_get_class_name(uint8_t port, int32_t id, uint8_t* class_name);

/**
 * Enable or disable the bounding box overlay the AI Vision sensor outputs on the USB port.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 *
 * \param port The V5 port number from 1-21
 * \param enabled if the overlay is enabled or disabled
 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
 * failed, setting errno
 */
int32_t aivision_set_usb_bounding_box_overlay(uint8_t port, bool enabled);

/**
 * Runs auto white balance to adjust to different lighting conditions.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 *
 * \param port The V5 port number from 1-21
 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
 * failed, setting errno
 */
int32_t aivision_start_awb(uint8_t port);

/**
 * Get a code that the AI vision sensor has stored.
 *
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 *
 * \param port The V5 port number from 1-21
 * \param id The id from 1-5
 * \return the code, or a struct with an invalid ID if the operation failed, setting errno
 */
aivision_code_s_t aivision_get_code(uint8_t port, uint32_t id);

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
 * \param port The V5 port number from 1-21
 * \param code The code to set
 * \return PROS_SUCCESS if the operation was successful or PROS_ERR if the operation
 * failed, setting errno
 */
int32_t aivision_set_code(uint8_t port, const aivision_code_s_t* wcode);

/**
 * Get the current number of objects detected by the AI vision sensor.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 *
 * \param port The V5 port number from 1-21
 * \return the number of objects if the operation was successful or PROS_ERR if the operation failed, setting errno
 */
int32_t aivision_get_object_count(uint8_t port);

/**
 * Get the detected object at a given object index; there are aivision_get_object_count objects and the index starts
 * from 0.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 *
 * @param port The V5 port number from 1-21
 * @param object_index the object index
 * @return the detected object if the operation was successful or an invalid object type if the operation failed,
 * setting errno
 */
aivision_object_s_t aivision_get_object(uint8_t port, uint32_t object_index);

/**
 * Get the current reported temperature of the AI Vision sensor in degrees Celsius.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 *
 * \param port The V5 port number from 1-21
 * \return the temperature if the operation was successful or PROS_ERR_F if the operation failed, setting errno
 */
double aivision_get_temperature(uint8_t port);

///@}

#ifdef __cplusplus
}  // namespace c
}  // namespace pros
}
#endif

#endif  // _PROS_VISION_H_