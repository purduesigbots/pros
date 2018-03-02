/**
 * \file vdml.c
 *
 * \brief VDML Vision Sensor Driver
 *
 * This file ensure thread saftey for operations on the Vision Sensor by maintaining
 * an array of RTOS Mutexes and implementing functions to take and give them.
 *
 * \copyright (c) 2017, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "vdml/vdml.h"
#include "ifi/v5_api.h"
#include "ifi/v5_apitypes.h"
#include "kapi.h"
#include "vdml/registry.h"

int32_t vision_get_object_count(uint8_t port) {
	claim_port(port - 1, E_DEVICE_VISION);
	int32_t rtn = vexDeviceVisionObjectCountGet(device.device_info);
	return_port(port - 1, rtn);
}

int32_t vision_read_object(uint8_t port, const uint32_t object_id, vision_object_s_t* const object_ptr) {
	claim_port(port - 1, E_DEVICE_VISION);
	if (vexDeviceVisionObjectCountGet(device.device_info) <= object_id) {
		port_mutex_give(port - 1);
		errno = EINVAL;
		return PROS_ERR;
	} else {
		int32_t rtn =
		    vexDeviceVisionObjectGet(device.device_info, object_id, (V5_DeviceVisionObject*)object_ptr);
		object_ptr->x_middle_coord = object_ptr->left_coord + (object_ptr->width / 2);
		object_ptr->y_middle_coord = object_ptr->top_coord + (object_ptr->height / 2);
		return_port(port - 1, rtn);
	}
}

int32_t vision_read_objects(uint8_t port, const uint32_t object_count, vision_object_s_t* const object_arr) {
	claim_port(port - 1, E_DEVICE_VISION);
	uint32_t c = vexDeviceVisionObjectCountGet(device.device_info);
	if (c > object_count) {
		c = object_count;
	}
	for (uint32_t i = 0; i < c; i++) {
		if (!vexDeviceVisionObjectGet(device.device_info, i, (V5_DeviceVisionObject*)(object_arr + i))) {
			break;
		}
		object_arr[i].x_middle_coord = object_arr[i].left_coord + (object_arr[i].width / 2);
		object_arr[i].y_middle_coord = object_arr[i].top_coord + (object_arr[i].height / 2);
	}
	return_port(port - 1, c);
}

vision_object_s_t vision_get_object(uint8_t port, const uint32_t object_id) {
	vision_object_s_t object;
	if (vision_read_object(port, object_id, &object) == PROS_ERR) {
		object.signature = 255;
	}
	return object;
}

int32_t vision_read_signature(uint8_t port, const uint8_t signature_id, vision_signature_s_t* const signature_ptr) {
	if (signature_id > 8 || signature_id == 0) {
		errno = EINVAL;
		return PROS_ERR;
	}
	claim_port(port - 1, E_DEVICE_VISION);
	int32_t rtn =
	    vexDeviceVisionSignatureGet(device.device_info, signature_id, (V5_DeviceVisionSignature*)signature_ptr);
	if (rtn) {
		errno = EAGAIN;
		rtn = PROS_ERR;
	} else
		rtn = 1;
	return_port(port - 1, rtn);
}

int32_t vision_save_signature(uint8_t port, const uint8_t signature_id, vision_signature_s_t* const signature_ptr) {
	if (signature_id > 8 || signature_id == 0) {
		errno = EINVAL;
		return PROS_ERR;
	}
	signature_ptr->id = signature_id;
	claim_port(port - 1, E_DEVICE_VISION);
	vexDeviceVisionSignatureSet(device.device_info, (V5_DeviceVisionSignature*)signature_ptr);
	return_port(port - 1, 1);
}

int32_t vision_set_led(uint8_t port, const int32_t rgb) {
	claim_port(port - 1, E_DEVICE_VISION);
	vexDeviceVisionLedModeSet(device.device_info, 1);
	V5_DeviceVisionRgb _rgb = {
		.red = COLOR2R(rgb), .blue = COLOR2B(rgb), .green = COLOR2G(rgb), .brightness = 255
	};
	vexDeviceVisionLedColorSet(device.device_info, _rgb);
	return_port(port - 1, 1);
}

int32_t vision_clear_led(uint8_t port) {
	claim_port(port - 1, E_DEVICE_VISION);
	vexDeviceVisionLedModeSet(device.device_info, 0);
	return_port(port - 1, 1);
}

int32_t vision_set_exposure(uint8_t port, const uint8_t percent) {
	claim_port(port - 1, E_DEVICE_VISION);
	vexDeviceVisionBrightnessSet(device.device_info, percent);
	return_port(port - 1, 1);
}

int32_t vision_get_exposure(uint8_t port) {
	claim_port(port - 1, E_DEVICE_VISION);
	int32_t rtn = vexDeviceVisionBrightnessGet(device.device_info);
	return_port(port - 1, rtn);
}

int32_t vision_set_auto_white_balance(uint8_t port, const uint8_t enable) {
	if (enable != 0 && enable != 1) {
		errno = EINVAL;
		return PROS_ERR;
	}
	claim_port(port - 1, E_DEVICE_VISION);
	vexDeviceVisionWhiteBalanceModeSet(device.device_info, enable + 1);
	return_port(port - 1, 1);
}

int32_t vision_set_white_balance(uint8_t port, const int32_t rgb) {
	claim_port(port - 1, E_DEVICE_VISION);
	vexDeviceVisionWhiteBalanceModeSet(device.device_info, 2);
	V5_DeviceVisionRgb _rgb = {
		.red = COLOR2R(rgb), .blue = COLOR2B(rgb), .green = COLOR2G(rgb), .brightness = 255
	};
	vexDeviceVisionWhiteBalanceSet(device.device_info, _rgb);
	return_port(port - 1, 1);
}

int32_t vision_get_white_balance(uint8_t port) {
	claim_port(port - 1, E_DEVICE_VISION);
	V5_DeviceVisionRgb rgb = vexDeviceVisionWhiteBalanceGet(device.device_info);
	return_port(port - 1, RGB2COLOR(rgb.red, rgb.green, rgb.blue));
}
