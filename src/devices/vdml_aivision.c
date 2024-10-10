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

typedef struct aivision_color_s {
    uint8_t       id; //id
    uint8_t       red; //red
    uint8_t       green; //grn
    uint8_t       blue; //blu
    float         hue; //handle
    float         saturation; //hdsat
} aivision_color_s_t;

typedef struct aivision_code_s {
    uint8_t       id;
    uint8_t       length;
    int16_t       c1;
    int16_t       c2;
    int16_t       c3;
    int16_t       c4;
    int16_t       c5;
    int16_t       c6;
    int16_t       c7;
} aivision_code_s_t;

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
    return_port(port - 1, 1);
}

aivision_color_s_t aivision_get_color(uint8_t port, uint32_t id) {
    aivision_color_s_t return_color;
    if (!claim_port_try(port - 1, E_DEVICE_AIVISION)) {
		return return_color;
	}
    v5_smart_device_s_t* device = registry_get_device(port - 1);

    V5_DeviceAiVisionColor _color;
    vexDeviceAiVisionColorGet(device->device_info, id, &_color);
    return_color.red = _color.red;
    return_color.green = _color.grn;
    return_color.blue = _color.blu;
    return_color.hue = _color.hangle;
    return_color.saturation = _color.hdsat;
    return_port(port - 1, return_color);
}

int32_t aivision_get_object_count(uint8_t port) {
    claim_port_i(port - 1, E_DEVICE_AIVISION);
    int32_t result = vexDeviceAiVisionObjectCountGet(device->device_info);
    return_port(port - 1, result);
}