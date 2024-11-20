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
 * \ingroup c-aivision
 */

/**
 * \addtogroup c-aivision
 *  @{
 */

/// \name Macros
///Parameters given by VEX
///@{

#define AIVISION_MAX_OBJECT_COUNT     24
#define AIVISION_MODE_STATUS_OVERLAY_BIT (1 << 25)
#define AIVISION_MODE_AWB_START_BIT   (1 << 27)
#define AIVISION_MODE_TAG_SET_BIT       (1 << 29)
#define AIVISION_DETECT_TYPE_COLOR    (1 << 0)
#define AIVISION_DETECT_TYPE_CODE     (1 << 1)
#define AIVISION_DETECT_TYPE_OBJECT   (1 << 2)
#define AIVISION_DETECT_TYPE_TAG      (1 << 3)

///@}

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
namespace pros {
#endif

/**
 * \enum aivision_detect_type_e_t
 * This enumeration defines what kind of object is stored inside aivision_object_s
 */
typedef enum aivision_detected_type {
	E_AIVISION_DETECTED_COLOR = AIVISION_DETECT_TYPE_COLOR,
	E_AIVISION_DETECTED_CODE = AIVISION_DETECT_TYPE_CODE,
	E_AIVISION_DETECTED_OBJECT = AIVISION_DETECT_TYPE_OBJECT,
	E_AIVISION_DETECTED_TAG = AIVISION_DETECT_TYPE_TAG
} aivision_detected_type_e_t;

/**
 * \enum aivision_mode_type_e_t
 * This enumeration defines what kinds of objects the ai vision sensor will scan for
 */

typedef enum aivision_mode_type {
    E_AIVISION_MODE_TAGS = (1<<0),
    E_AIVISION_MODE_COLORS = (1<<1),
    E_AIVISION_MODE_OBJECTS = (1<<2),
} aivision_mode_type_e_t;

/**
 * \struct aivision_color_s_t
 * This structure contains the parameters used by the Vision Sensor to define a color
 */
typedef struct __attribute__((packed)) aivision_color_s {
    uint8_t       id;
    uint8_t       red;
    uint8_t       green;
    uint8_t       blue;
    float         hue;
    float         saturation;
} aivision_color_s_t;

typedef struct __attribute__((packed)) aivision_code_s {
    uint8_t id;
    uint8_t length;
    int16_t c1;
    int16_t c2;
    int16_t c3;
    int16_t c4;
    int16_t c5;
	  int16_t c6;
	  int16_t c7;
} aivision_code_s_t;

/**
 * \enum aivision_tag_family_e_t
 * This enumeration corresponds to a familt of AprilTags
 * @see https://april.eecs.umich.edu/software/apriltag
 */
typedef enum aivision_tag_family_e {
    TAG_CIRCLE_21H7 = 0,
    TAG_16H5 = 1,
    TAG_25H9 = 2,
    TAG_61H11 = 3
} aivision_tag_family_e_t;


/**
 * \struct aivision_object_s_t
 * This structure contains one of aivision_detected_type_e_t, stored in type
 */
typedef struct __attribute__((packed)) aivision_object_s {
    uint8_t id; //color id
    uint8_t type; //object type
    union {
        struct {
            uint16_t xoffset; //left side
            uint16_t yoffset; //top
            uint16_t width;
            uint16_t height;
            uint16_t angle; //angle, 0.1deg units
        } color;
        //april tag coordinates
        struct {
            int16_t x0;
            int16_t y0;
            int16_t x1;
            int16_t y1;
            int16_t x2;
            int16_t y2;
            int16_t x3;
            int16_t y3;
        } april_tag;
        struct {
            uint16_t xoffset; //left
            uint16_t yoffset; //top
            uint16_t width;
            uint16_t height;
            uint16_t score; //confidence
        } model;
    } object;    
} aivision_object_s_t;

#ifdef __cplusplus
namespace c {
#endif

/// \name Functions
///@{

/**
 * Resets the AI Vision sensor to the initial state.
 * @param port
 * 				The V5 port number from 1-21
 * \return 1 if the operation was successful or PROS_ERR if the operation
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
 * @param port the port from 1-21
 * @return the bitfield
 */
int32_t aivision_get_enabled_detection_types(uint8_t port);

/**
 * Modifies the types of objects the AI vision sensor is currently searching for, as per aivision_mode_type_e_t.
 * Only bits set in the bitmask will be updated to the corresponding bit in the bits.
 *
 * @param port
 * @param bits
 * @param bitmask
 * @return
 */
int32_t aivision_set_enabled_detection_types(uint8_t port, uint8_t bits, uint8_t bitmask);

/**
 * Enable detecting these types of objects, a bitmask as per aivision_mode_type_e_t
 *
 * @param port
 * @param types_mask
 * @return
 */
int32_t aivision_enable_detection_types(uint8_t port, uint8_t types_mask);

/**
 * Disable detecting these types of objects, a bitmask as per aivision_mode_type_e_t
 *
 * @param port
 * @param types_mask
 * @return
 */
int32_t aivision_disable_detection_types(uint8_t port, uint8_t types_mask);

/**
 * Sets the april tag family to detect. Only one family can be detected at a time.
 * user must separately enable the tag detection type
 * @param port
 * @param family
 * @return
 */
int32_t aivision_set_tag_family(uint8_t port, aivision_tag_family_e_t family);

/**
 * Set a color configuration that the ai vision sensor will detect
 * User must separately enable the color detection type
 * The id of the color to be set is in aivision_color_s_t
 * @param port
 * @param color
 * @return
 */
int32_t aivision_set_color(uint8_t port, aivision_color_s_t * color);

/**
 * Get a color configuration by id stored in the ai vision.
 *
 * @param port
 * @param id
 * @return
 */
aivision_color_s_t aivision_get_color(uint8_t port, uint32_t id);


uint32_t aivision_get_class_name(uint8_t port, int32_t id, uint8_t * class_name);
int32_t aivision_set_overlay(uint8_t port, bool enabled);

aivision_code_s_t aivision_get_code(uint8_t port, uint32_t id);
uint32_t aivision_set_code(uint8_t port, aivision_code_s_t code);

int32_t aivision_get_object_count(uint8_t port);
double aivision_get_temperature(uint8_t port);
aivision_object_s_t aivision_get_object(uint8_t port, uint32_t object_index);
uint32_t aivision_status(uint8_t port);
/**
 * this will need a reconfiguration of sensor after it is "done"
 * @param port
 * @return
 */
int32_t aivision_start_awb(uint8_t port);

///@}

#ifdef __cplusplus
}  // namespace c
}  // namespace pros
}
#endif

#endif  // _PROS_VISION_H_
