/**
 * \file devices/vdml_aivision.c
 *
 * Contains functions for interacting with the V5 Vision Sensor.
 *
 * \copyright Copyright (c) 2017-2024, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */


#include "v5_api.h"
#include "vdml/registry.h"
#include "vdml/vdml.h"

// No header file yet :)

#define AIVISION_MODE_SET_MASK (1 << 29)

#define AIVISION_DETECT_TYPE_COLOR    (1 << 0)
#define AIVISION_DETECT_TYPE_CODE     (1 << 1)
#define AIVISION_DETECT_TYPE_OBJECT   (1 << 2)
#define AIVISION_DETECT_TYPE_TAG      (1 << 3)

typedef struct __attribute__((packed)) aivision_color_s {
    uint8_t       id; //id
    uint8_t       red; //red
    uint8_t       green; //grn
    uint8_t       blue; //blu
    float         hue; //hangle -- range of hue that sensor will accept (i.e. higher = less sensitive)
    float         saturation; //hdsat -- range of saturation that sensor will accept (i.e. higher = less sensitive)
} aivision_color_s_t;

typedef struct __attribute__((packed)) aivision_code_s {
    uint8_t id;
    uint8_t length;
    int16_t c1;
    int16_t c2;
    int16_t c3; //optional
    int16_t c4; //optional
    int16_t c5; //optional
    int32_t reserved; //do not use -- reserved
} aivision_code_s_t;

typedef enum aivision_tag_family_e {
    TAG_CIRCLE_21H7 = 0,
    TAG_16H5 = 1,
    TAG_25H9 = 2,
    TAG_61H11 = 3
} aivision_tag_family_e_t;

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

int32_t aivision_set_color(uint8_t port, aivision_color_s_t *color) {
    claim_port_i(port - 1, E_DEVICE_AIVISION);
    V5_DeviceAiVisionColor _color;
    _color.id = color->id;
    _color.red = color->red;
    _color.grn = color->green;
    _color.blu = color->blue;
    _color.hangle = color->hue;
    _color.hdsat = color->saturation;
    vexDeviceAiVisionColorSet(device->device_info, &_color);
    return_port(port - 1, PROS_SUCCESS);
}

aivision_color_s_t aivision_get_color(uint8_t port, uint32_t id) {
    aivision_color_s_t return_color;
    if (!claim_port_try(port - 1, E_DEVICE_AIVISION)) {
        return return_color;
    }
    v5_smart_device_s_t* device = registry_get_device(port - 1);

    vexDeviceAiVisionColorGet(device->device_info, id, &return_color);
    return_port(port - 1, return_color);
}

int32_t aivision_get_object_count(uint8_t port) {
    claim_port_i(port - 1, E_DEVICE_AIVISION);
    int32_t result = vexDeviceAiVisionObjectCountGet(device->device_info);
    return_port(port - 1, result);
}

double aivision_get_temperature(uint8_t port) {
    claim_port_i(port - 1, E_DEVICE_AIVISION);
    double result = vexDeviceAiVisionTemperatureGet(device->device_info);
    return_port(port - 1, result);
}

int32_t aivision_set_tag_family(uint8_t port, aivision_tag_family_e_t family) {
    claim_port_i(port - 1, E_DEVICE_AIVISION);
    uint32_t tag_family_flag = (uint32_t) family << 16;
    vexDeviceAiVisionModeSet(device->device_info, tag_family_flag | AIVISION_MODE_SET_MASK);
    return_port(port - 1, PROS_SUCCESS);
}

int32_t aivision_reset(uint8_t port) {
    claim_port_i(port - 1, E_DEVICE_AIVISION);
    vexDeviceAiVisionReset(device->device_info);
    return_port(port - 1, PROS_SUCCESS);
}

aivision_object_s_t aivision_get_object(uint8_t port, uint32_t object_index) {
    aivision_object_s_t result;
    if (!claim_port_try(port - 1, E_DEVICE_AIVISION)) {
        return result;
    }
    v5_smart_device_s_t* device = registry_get_device(port - 1);
    vexDeviceAiVisionObjectGet(device->device_info, object_index, &result);
    return_port(port - 1, result);
}

aivision_code_s_t aivision_set_color_code(uint8_t port) {
    aivision_code_s_t result;
    if (!claim_port_try(port - 1, E_DEVICE_AIVISION)) {
        return result;
    }
    v5_smart_device_s_t* device = registry_get_device(port - 1);
    vexDeviceAiVisionColorSet(device->device_info, &result);
    return_port(port - 1, result);
}