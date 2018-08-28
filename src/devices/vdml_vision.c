/**
 * \file devices/vdml_vision.c
 *
 * Contains functions for interacting with the V5 Vision Sensor.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ifi/v5_api.h"
#include "ifi/v5_apitypes.h"
#include "kapi.h"
#include "vdml/registry.h"
#include "vdml/vdml.h"

#define ZERO_POINT(port) registry_get_device(port)->pad[port]

static void _vision_transform_coords(uint8_t port, vision_object_s_t* object_ptr) {
	switch (ZERO_POINT(port)) {
		case E_VISION_ZERO_CENTER:
			object_ptr->left_coord -= VISION_FOV_WIDTH / 2;
			object_ptr->top_coord = (VISION_FOV_HEIGHT / 2) - object_ptr->top_coord;
			break;
		default:
			break;
	}
	object_ptr->x_middle_coord = object_ptr->left_coord + (object_ptr->width / 2);
	object_ptr->y_middle_coord = object_ptr->top_coord - (object_ptr->height / 2);
}

int32_t vision_get_object_count(uint8_t port) {
	claim_port(port - 1, E_DEVICE_VISION);
	int32_t rtn = vexDeviceVisionObjectCountGet(device->device_info);
	return_port(port - 1, rtn);
}

vision_object_s_t vision_get_by_size(uint8_t port, const uint32_t size_id) {
	vision_object_s_t rtn;
	v5_smart_device_s_t* device;
	int32_t err = claim_port_try(port - 1, E_DEVICE_VISION);
	if (err == PROS_ERR) {
		rtn.signature = VISION_OBJECT_ERR_SIG;
		goto leave;
	}
	device = registry_get_device(port - 1);
	if ((uint32_t)vexDeviceVisionObjectCountGet(device->device_info) <= size_id) {
		errno = EINVAL;
		goto leave;
	}
	err = vexDeviceVisionObjectGet(device->device_info, size_id, (V5_DeviceVisionObject*)&rtn);
	if (err == 0) {
		errno = EINVAL;
		goto leave;
	}
	_vision_transform_coords(port - 1, &rtn);

leave:
	port_mutex_give(port - 1);
	return rtn;
}

vision_object_s_t vision_get_by_sig(uint8_t port, const uint32_t size_id, const uint32_t sig_id) {
	vision_object_s_t rtn;
	v5_smart_device_s_t* device;
	uint8_t count = 0;
	int32_t object_count = 0;

	int32_t err = claim_port_try(port - 1, E_DEVICE_VISION);
	if (err == PROS_ERR) {
		errno = EINVAL;
		goto err_return;
	}
	if (sig_id > 7 || sig_id == 0) {
		errno = EINVAL;
		goto err_return;
	}
	device = registry_get_device(port - 1);
	object_count = vexDeviceVisionObjectCountGet(device->device_info);
	if ((uint32_t)object_count <= size_id) {
		errno = EINVAL;
		goto err_return;
	}

	for (uint8_t i = 0; i <= object_count; i++) {
		vision_object_s_t check;
		err = vexDeviceVisionObjectGet(device->device_info, i, (V5_DeviceVisionObject*)&check);
		if (err == PROS_ERR) {
			errno = EAGAIN;
			rtn = check;
			goto err_return;
		}
		if (check.signature == sig_id) {
			if (count == size_id) {
				rtn = check;
				_vision_transform_coords(port - 1, &rtn);
				port_mutex_give(port - 1);
				return rtn;
			}
			count++;
		}
	}

err_return:
	port_mutex_give(port - 1);
	rtn.signature = VISION_OBJECT_ERR_SIG;
	return rtn;
}

int32_t vision_read_by_size(uint8_t port, const uint32_t size_id, const uint32_t object_count,
                            vision_object_s_t* const object_arr) {
	claim_port(port - 1, E_DEVICE_VISION);
	for (uint8_t i = 0; i < object_count; i++) {
		object_arr[i].signature = VISION_OBJECT_ERR_SIG;
	}
	uint32_t c = vexDeviceVisionObjectCountGet(device->device_info);
	if (c <= size_id) {
		port_mutex_give(port - 1);
		errno = EINVAL;
		return PROS_ERR;
	} else if (c > object_count) {
		c = object_count;
	}

	for (uint32_t i = size_id; i < c; i++) {
		if (!vexDeviceVisionObjectGet(device->device_info, i, (V5_DeviceVisionObject*)(object_arr + i))) {
			break;
		}
		_vision_transform_coords(port - 1, &object_arr[i]);
	}
	return_port(port - 1, c);
}

int32_t vision_read_by_sig(uint8_t port, const uint32_t size_id, const uint32_t sig_id, const uint32_t object_count,
                           vision_object_s_t* const object_arr) {
	claim_port(port - 1, E_DEVICE_VISION);
	for (uint8_t i = 0; i < object_count; i++) {
		object_arr[i].signature = VISION_OBJECT_ERR_SIG;
	}
	uint32_t c = vexDeviceVisionObjectCountGet(device->device_info);
	if (c <= size_id) {
		errno = EINVAL;
		port_mutex_give(port - 1);
		return PROS_ERR;
	}
	if (c > object_count) {
		c = object_count;
	}

	uint8_t count = 0;
	for (uint8_t i = 0; i < c; i++) {
		vexDeviceVisionObjectGet(device->device_info, i, (V5_DeviceVisionObject*)(object_arr + i));
		if (object_arr[i].signature == sig_id) {
			if (count > size_id) {
				_vision_transform_coords(port - 1, &object_arr[i]);
			}
			count++;
		}
		if (count == object_count) break;
	}
	return_port(port - 1, count);
}

/**
 * Loads the object detection signature into the supplied pointer to memory.
 *
 * NOTE: only returns signatures set through the vision_write_signature function,
 * so this is not publicly exposed as a result.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param signature_id
 *        The signature id to read
 *
 * \return 1 if no errors occurred, PROS_ERR otherwise
 */
vision_signature_s_t vision_read_signature(uint8_t port, const uint8_t signature_id) {
	vision_signature_s_t sig;
	int32_t rtn = claim_port_try(port - 1, E_DEVICE_VISION);
	if (rtn == PROS_ERR) {
		sig.id = VISION_OBJECT_ERR_SIG;

		return sig;
	}
	if (signature_id > 7 || signature_id == 0) {
		errno = EINVAL;
		sig.id = VISION_OBJECT_ERR_SIG;
		return sig;
	}

	v5_smart_device_s_t* device = registry_get_device(port - 1);
	rtn = vexDeviceVisionSignatureGet(device->device_info, signature_id, (V5_DeviceVisionSignature*)&sig);
	if (!rtn) {
		errno = EAGAIN;
		sig.id = VISION_OBJECT_ERR_SIG;
	}
	port_mutex_give(port - 1);
	return sig;
}

/**
 * Stores the supplied object detection signature onto the vision sensor
 *
 * NOTE: only writes custom created signatures,
 * so this is not publicly exposed as a result.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param signature_id
 *        The signature id to store into
 * \param[in] signature_ptr
 *            A pointer to the signature to save
 *
 * \return 1 if no errors occured, PROS_ERR otherwise
 */
int32_t vision_write_signature(uint8_t port, const uint8_t signature_id, vision_signature_s_t* const signature_ptr) {
	claim_port(port - 1, E_DEVICE_VISION);
	if (signature_id > 8 || signature_id == 0) {
		errno = EINVAL;
		return PROS_ERR;
	}
	signature_ptr->id = signature_id;

	vexDeviceVisionSignatureSet(device->device_info, (V5_DeviceVisionSignature*)signature_ptr);
	return_port(port - 1, 1);
}

int32_t vision_set_led(uint8_t port, const int32_t rgb) {
	claim_port(port - 1, E_DEVICE_VISION);
	vexDeviceVisionLedModeSet(device->device_info, 1);
	V5_DeviceVisionRgb _rgb = {.red = COLOR2R(rgb), .blue = COLOR2B(rgb), .green = COLOR2G(rgb), .brightness = 255};
	vexDeviceVisionLedColorSet(device->device_info, _rgb);
	return_port(port - 1, 1);
}

int32_t vision_clear_led(uint8_t port) {
	claim_port(port - 1, E_DEVICE_VISION);
	vexDeviceVisionLedModeSet(device->device_info, 0);
	return_port(port - 1, 1);
}

int32_t vision_set_exposure(uint8_t port, const uint8_t percent) {
	claim_port(port - 1, E_DEVICE_VISION);
	vexDeviceVisionBrightnessSet(device->device_info, percent);
	return_port(port - 1, 1);
}

int32_t vision_get_exposure(uint8_t port) {
	claim_port(port - 1, E_DEVICE_VISION);
	int32_t rtn = vexDeviceVisionBrightnessGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t vision_set_auto_white_balance(uint8_t port, const uint8_t enable) {
	if (enable != 0 && enable != 1) {
		errno = EINVAL;
		return PROS_ERR;
	}
	claim_port(port - 1, E_DEVICE_VISION);
	vexDeviceVisionWhiteBalanceModeSet(device->device_info, enable + 1);
	return_port(port - 1, 1);
}

int32_t vision_set_white_balance(uint8_t port, const int32_t rgb) {
	claim_port(port - 1, E_DEVICE_VISION);
	vexDeviceVisionWhiteBalanceModeSet(device->device_info, 2);
	V5_DeviceVisionRgb _rgb = {.red = COLOR2R(rgb), .blue = COLOR2B(rgb), .green = COLOR2G(rgb), .brightness = 255};
	vexDeviceVisionWhiteBalanceSet(device->device_info, _rgb);
	return_port(port - 1, 1);
}

int32_t vision_get_white_balance(uint8_t port) {
	claim_port(port - 1, E_DEVICE_VISION);
	V5_DeviceVisionRgb rgb = vexDeviceVisionWhiteBalanceGet(device->device_info);
	return_port(port - 1, RGB2COLOR(rgb.red, rgb.green, rgb.blue));
}

int32_t vision_set_zero_point(uint8_t port, vision_zero_e_t zero_point) {
	claim_port(port - 1, E_DEVICE_VISION);
	device->pad[port - 1] = (uint8_t)zero_point;
	return_port(port - 1, 1);
}
