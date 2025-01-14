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
#include "pros/ai_vision.h"
#include "v5_api.h"
#include "vdml/registry.h"
#include "vdml/vdml.h"

#define AIVISION_COLOR_ERR_INIT                                                                                        \
	{   .id = PROS_ERR_BYTE, .red = PROS_ERR_BYTE, .green = PROS_ERR_BYTE, .blue = PROS_ERR_BYTE,                      \
        .hue_range = PROS_ERR_F, .saturation_range = PROS_ERR_F                                                        \
	}

#define AIVISION_CODE_ERR_INIT                                                                                         \
	{   .id = PROS_ERR_BYTE, .length = PROS_ERR_BYTE, .c1 = PROS_ERR_2_BYTE, .c2 = PROS_ERR_2_BYTE,                    \
		.c3 = PROS_ERR_2_BYTE, .c4 = PROS_ERR_2_BYTE, .c5 = PROS_ERR_2_BYTE                                            \
	}

#define AIVISION_OBJECT_ERR_INIT \
	{ .id = PROS_ERR_BYTE, .type = PROS_ERR_BYTE }

int32_t aivision_reset(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_AIVISION);
	vexDeviceAiVisionReset(device->device_info);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t aivision_set_enabled_detection_types(uint8_t port, uint8_t bits, uint8_t bitmask) {
	claim_port_i(port - 1, E_DEVICE_AIVISION);
	vexDeviceAiVisionEnableSet(device->device_info, bits, bitmask);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t aivision_get_enabled_detection_types(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_AIVISION);
	uint8_t enabled_detection_types = vexDeviceAiVisionEnableGet(device->device_info);
	return_port(port - 1, enabled_detection_types);
}

int32_t aivision_enable_detection_types(uint8_t port, uint8_t types_mask) {
	claim_port_i(port - 1, E_DEVICE_AIVISION);
	vexDeviceAiVisionEnableSet(device->device_info, types_mask, types_mask);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t aivision_disable_detection_types(uint8_t port, uint8_t types_mask) {
	claim_port_i(port - 1, E_DEVICE_AIVISION);
	vexDeviceAiVisionEnableSet(device->device_info, 0, types_mask);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t aivision_get_class_name(uint8_t port, int32_t id, uint8_t* class_name) {
	claim_port_i(port - 1, E_DEVICE_AIVISION);
	vexDeviceAiVisionClassNameGet(device->device_info, id, class_name);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t aivision_set_color(uint8_t port, aivision_color_s_t* color) {
	claim_port_i(port - 1, E_DEVICE_AIVISION);
	V5_DeviceAiVisionColor _color;
	_color.id = color->id;
	_color.red = color->red;
	_color.grn = color->green;
	_color.blu = color->blue;
	_color.hangle = color->hue_range;
	_color.hdsat = color->saturation_range;
	vexDeviceAiVisionColorSet(device->device_info, &_color);
	return_port(port - 1, PROS_SUCCESS);
}

aivision_color_s_t aivision_get_color(uint8_t port, uint32_t id) {
	aivision_color_s_t color = AIVISION_COLOR_ERR_INIT;
	if (!claim_port_try(port - 1, E_DEVICE_AIVISION)) {
		return color;
	}
	V5_DeviceAiVisionColor _color;
	v5_smart_device_s_t* device = registry_get_device(port - 1);
	vexDeviceAiVisionColorGet(device->device_info, id, &_color);
	color.id = _color.id;
	color.red = _color.red;
	color.green = _color.grn;
	color.blue = _color.blu;
	color.hue_range = _color.hangle;
	color.saturation_range = _color.hdsat;
	return_port(port - 1, color);
}

int32_t aivision_get_object_count(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_AIVISION);
	int32_t result = vexDeviceAiVisionObjectCountGet(device->device_info);
	return_port(port - 1, result);
}

double aivision_get_temperature(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_AIVISION);
	double result = vexDeviceAiVisionTemperatureGet(device->device_info);
	return_port(port - 1, result);
}

int32_t aivision_set_tag_family(uint8_t port, aivision_tag_family_e_t family) {
	claim_port_i(port - 1, E_DEVICE_AIVISION);
	uint32_t tag_family_flag = (uint32_t)family << 16;
	vexDeviceAiVisionModeSet(device->device_info, tag_family_flag | AIVISION_MODE_TAG_SET_BIT);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t aivision_set_usb_bounding_box_overlay(uint8_t port, bool enabled) {
	claim_port_i(port - 1, E_DEVICE_AIVISION);
	uint32_t mode = vexDeviceAiVisionStatusGet(device->device_info);
	if (enabled) {
		mode &= 0x7F;
	} else {
		mode = (mode != 0) | 0x80;
	}
	mode = (mode << 8) | (1 << 25);

	vexDeviceAiVisionModeSet(device->device_info, mode);
	return_port(port - 1, PROS_SUCCESS);
}

aivision_object_s_t aivision_get_object(uint8_t port, uint32_t object_index) {
	aivision_object_s_t result = AIVISION_OBJECT_ERR_INIT;

    if (!claim_port_try(port - 1, E_DEVICE_AIVISION)) {
        return result;
    }
    v5_smart_device_s_t* device = registry_get_device(port - 1);
    vexDeviceAiVisionObjectGet(device->device_info, object_index, (V5_DeviceAiVisionObject*)&result);
    return_port(port - 1, result);
}

aivision_code_s_t aivision_get_code(uint8_t port, uint32_t id) {
	aivision_code_s_t code = AIVISION_CODE_ERR_INIT;
	if (!claim_port_try(port - 1, E_DEVICE_AIVISION)) {
		return code;
	}
	v5_smart_device_s_t* device = registry_get_device(port - 1);
	V5_DeviceAiVisionCode _code;
	vexDeviceAiVisionCodeGet(device->device_info, id, &_code);
	code.id = _code.id;
	code.length = _code.len;
	code.c1 = _code.c1;
	code.c2 = _code.c2;
	code.c3 = _code.c3;
	code.c4 = _code.c4;
	code.c5 = _code.c5;
	return_port(port - 1, code);
}

int32_t aivision_set_code(uint8_t port, aivision_code_s_t* code) {
	claim_port_i(port - 1, E_DEVICE_AIVISION);
	V5_DeviceAiVisionCode _code;
	_code.id = code->id;
	_code.len = code->length;
	_code.c1 = code->c1;
	_code.c2 = code->c2;
	_code.c3 = code->c3;
	_code.c4 = code->c4;
	_code.c5 = code->c5;
	vexDeviceAiVisionCodeSet(device->device_info, &_code);
	return_port(port - 1, PROS_SUCCESS);
}
