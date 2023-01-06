/**
 * \file devices/vdml_vision.c
 *
 * Contains functions for interacting with the V5 Vision Sensor.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "kapi.h"
#include "v5_api.h"
#include "v5_apitypes.h"
#include "vdml/registry.h"
#include "vdml/vdml.h"

typedef struct vision_data {
	vision_zero_e_t zero_point;
} vision_data_s_t;

static vision_zero_e_t get_zero_point(uint8_t port) {
	return ((vision_data_s_t*)registry_get_device(port)->pad)->zero_point;
}

static void set_zero_point(uint8_t port, vision_zero_e_t zero_point) {
	vision_data_s_t* data = (vision_data_s_t*)registry_get_device(port)->pad;
	data->zero_point = zero_point;
}

static void _vision_transform_coords(uint8_t port, vision_object_s_t* object_ptr) {
	switch (get_zero_point(port)) {
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
	claim_port_i(port - 1, E_DEVICE_VISION);
	int32_t rtn = vexDeviceVisionObjectCountGet(device->device_info);
	return_port(port - 1, rtn);
}

vision_object_s_t vision_get_by_size(uint8_t port, const uint32_t size_id) {
	vision_object_s_t rtn;
	v5_smart_device_s_t* device;
	if (!claim_port_try(port - 1, E_DEVICE_VISION)) {
		rtn.signature = VISION_OBJECT_ERR_SIG;
		return rtn;
	}
	device = registry_get_device(port - 1);
	if ((uint32_t)vexDeviceVisionObjectCountGet(device->device_info) <= size_id) {
		errno = EDOM;
		rtn.signature = VISION_OBJECT_ERR_SIG;
		goto leave;
	}
	if (vexDeviceVisionObjectGet(device->device_info, size_id, (V5_DeviceVisionObject*)&rtn) == 0) {
		errno = EAGAIN;
		rtn.signature = VISION_OBJECT_ERR_SIG;
		goto leave;
	}
	_vision_transform_coords(port - 1, &rtn);

leave:
	port_mutex_give(port - 1);
	return rtn;
}

vision_object_s_t _vision_get_by_sig(uint8_t port, const uint32_t size_id, const uint32_t sig_id) {
	vision_object_s_t rtn;
	rtn.signature = VISION_OBJECT_ERR_SIG;
	v5_smart_device_s_t* device;
	uint8_t count = 0;
	int32_t object_count = 0;

	if (!claim_port_try(port - 1, E_DEVICE_VISION)) {
		goto err_return_no_mutex;
	}

	device = registry_get_device(port - 1);
	object_count = vexDeviceVisionObjectCountGet(device->device_info);
	if ((uint32_t)object_count <= size_id) {
		errno = EDOM;
		goto err_return;
	}

	for (uint8_t i = 0; i <= object_count; i++) {
		vision_object_s_t check;
		if (vexDeviceVisionObjectGet(device->device_info, i, (V5_DeviceVisionObject*)&check) == PROS_ERR) {
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
	errno = EDOM;  // we read through all the objects and none matched sig_id and size_id

err_return:
	port_mutex_give(port - 1);
err_return_no_mutex:
	rtn.signature = VISION_OBJECT_ERR_SIG;
	return rtn;
}

vision_object_s_t vision_get_by_sig(uint8_t port, const uint32_t size_id, const uint32_t sig_id) {
	if (sig_id > 7 || sig_id == 0) {
		errno = EINVAL;
		vision_object_s_t rtn;
		rtn.signature = VISION_OBJECT_ERR_SIG;
		return rtn;
	}
	return _vision_get_by_sig(port, size_id, sig_id);
}

vision_object_s_t vision_get_by_code(uint8_t port, const uint32_t size_id, const vision_color_code_t color_code) {
	return _vision_get_by_sig(port, size_id, color_code);
}

int32_t vision_read_by_size(uint8_t port, const uint32_t size_id, const uint32_t object_count,
                            vision_object_s_t* const object_arr) {
	claim_port_i(port - 1, E_DEVICE_VISION);
	for (uint8_t i = 0; i < object_count; i++) {
		object_arr[i].signature = VISION_OBJECT_ERR_SIG;
	}
	uint32_t c = vexDeviceVisionObjectCountGet(device->device_info);
	if (c <= size_id) {
		port_mutex_give(port - 1);
		errno = EDOM;
		return PROS_ERR;
	} else if (c > object_count + size_id) {
		c = object_count + size_id;
	}

	for (uint32_t i = size_id; i < c; i++) {
		if (!vexDeviceVisionObjectGet(device->device_info, i, (V5_DeviceVisionObject*)(object_arr + i))) {
			errno = EAGAIN;
			object_arr[i].signature = VISION_OBJECT_ERR_SIG;
			break;
		}
		_vision_transform_coords(port - 1, &object_arr[i]);
	}
	return_port(port - 1, c);
}

int32_t _vision_read_by_sig(uint8_t port, const uint32_t size_id, const uint32_t sig_id, const uint32_t object_count,
                            vision_object_s_t* const object_arr) {
	claim_port_i(port - 1, E_DEVICE_VISION);
	for (uint8_t i = 0; i < object_count; i++) {
		object_arr[i].signature = VISION_OBJECT_ERR_SIG;
	}
	uint32_t c = vexDeviceVisionObjectCountGet(device->device_info);
	if (c <= size_id) {
		errno = EDOM;
		port_mutex_give(port - 1);
		return PROS_ERR;
	}
	if (c > object_count) {
		c = object_count;
	}

	uint32_t j = 0;                    // track how many objects we've placed into object_arr
	uint32_t seen = 0;                 // track how many objects we've seen matching sig_id
	for (uint8_t i = 0; i < c; i++) {  // loop through all objects on sensor
		// place i-th object on vision sensor on j-th position in object_arr
		if (!vexDeviceVisionObjectGet(device->device_info, i, (V5_DeviceVisionObject*)(object_arr + j))) {
			errno = EAGAIN;
			object_arr[i].signature = VISION_OBJECT_ERR_SIG;
			goto rtn;
		}
		// check if this (j-th) object matches sig_id
		if (object_arr[j].signature == sig_id) {
			seen++;
			if (seen >= size_id) {  // have we seen enough objects to start adding to object_arr?
				// if so, transform the coords and "commit" it by incrementing j
				_vision_transform_coords(port - 1, &object_arr[j]);
				j++;
				if (j == object_count) goto rtn;
			}
		}
	}
	errno = EDOM;  // read through all objects and couldn't find enough objects matching filter parameters
rtn:
	return_port(port - 1, j);
}

int32_t vision_read_by_sig(uint8_t port, const uint32_t size_id, const uint32_t sig_id, const uint32_t object_count,
                           vision_object_s_t* const object_arr) {
	if (sig_id > 7 || sig_id == 0) {
		errno = EINVAL;
		for (uint8_t i = 0; i < object_count; i++) {
			object_arr[i].signature = VISION_OBJECT_ERR_SIG;
		}
		return PROS_ERR;
	}
	return _vision_read_by_sig(port, size_id, sig_id, object_count, object_arr);
}

int32_t vision_read_by_code(uint8_t port, const uint32_t size_id, const vision_color_code_t color_code,
                            const uint32_t object_count, vision_object_s_t* const object_arr) {
	return _vision_read_by_sig(port, size_id, color_code, object_count, object_arr);
}

vision_signature_s_t vision_get_signature(uint8_t port, const uint8_t signature_id) {
	vision_signature_s_t sig;
	sig.id = VISION_OBJECT_ERR_SIG;
	if (signature_id > 7 || signature_id == 0) {
		errno = EINVAL;
		return sig;
	}
	int32_t rtn = claim_port_try(port - 1, E_DEVICE_VISION);
	if (!rtn) {
		return sig;
	}
	v5_smart_device_s_t* device = registry_get_device(port - 1);
	rtn = vexDeviceVisionSignatureGet(device->device_info, signature_id, (V5_DeviceVisionSignature*)&sig);
	if (!rtn || !sig._pad[0]) {  // sig._pad[0] is flags, will be set to 1 if data is valid and signatures are sent
		errno = EAGAIN;
		sig.id = VISION_OBJECT_ERR_SIG;
	}
	port_mutex_give(port - 1);
	return sig;
}

int32_t vision_set_signature(uint8_t port, const uint8_t signature_id, vision_signature_s_t* const signature_ptr) {
	if (signature_id > 7 || signature_id == 0) {
		errno = EINVAL;
		return PROS_ERR;
	}
	signature_ptr->id = signature_id;

	claim_port_i(port - 1, E_DEVICE_VISION);
	vexDeviceVisionSignatureSet(device->device_info, (V5_DeviceVisionSignature*)signature_ptr);
	return_port(port - 1, PROS_SUCCESS);
}

vision_signature_s_t vision_signature_from_utility(const int32_t id, const int32_t u_min, const int32_t u_max,
                                                   const int32_t u_mean, const int32_t v_min, const int32_t v_max,
                                                   const int32_t v_mean, const float range, const int32_t type) {
	vision_signature_s_t sig = {0};
	sig.id = id;
	sig.range = range;
	sig.u_min = u_min;
	sig.u_max = u_max;
	sig.u_mean = u_mean;
	sig.v_min = v_min;
	sig.v_max = v_max;
	sig.v_mean = v_mean;
	sig.type = type;
	return sig;
}

vision_color_code_t vision_create_color_code(uint8_t port, const uint32_t sig_id1, const uint32_t sig_id2,
                                             const uint32_t sig_id3, const uint32_t sig_id4, const uint32_t sig_id5) {
	uint16_t id = 0;
	if (!sig_id1 || !sig_id2 || sig_id1 > 7 || sig_id2 > 7 || sig_id3 > 7 || sig_id4 > 7 || sig_id5 > 7) {
		// Need to at least have two signatures to make a color code, and they all
		// must be in the range [0-7]
		errno = EINVAL;
		id = VISION_OBJECT_ERR_SIG;
		return id;
	}

	const uint32_t sigs[5] = {sig_id1, sig_id2, sig_id3, sig_id4, sig_id5};
	for (size_t i = 0; i < 5 && sigs[i]; i++) {
		register const uint32_t sig_id = sigs[i];
		id = (id << 3) | sig_id;

		vision_signature_s_t stored_sig = vision_get_signature(port, sig_id);
		if (stored_sig.type != E_VISION_OBJECT_COLOR_CODE) {
			stored_sig.type = E_VISION_OBJECT_COLOR_CODE;
			vision_set_signature(port, sig_id, &stored_sig);
		}
	}

	return id;
}

int32_t vision_set_led(uint8_t port, const int32_t rgb) {
	claim_port_i(port - 1, E_DEVICE_VISION);
	vexDeviceVisionLedModeSet(device->device_info, 1);
	V5_DeviceVisionRgb _rgb = {.red = COLOR2R(rgb), .blue = COLOR2B(rgb), .green = COLOR2G(rgb), .brightness = 255};
	vexDeviceVisionLedColorSet(device->device_info, _rgb);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t vision_clear_led(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_VISION);
	vexDeviceVisionLedModeSet(device->device_info, 0);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t vision_set_exposure(uint8_t port, const uint8_t percent) {
	claim_port_i(port - 1, E_DEVICE_VISION);
	// This translation comes from VEX to match the brightness represented in vision utility
	vexDeviceVisionBrightnessSet(device->device_info, (((int)((percent * 100) + 50)) / 255));
	return_port(port - 1, PROS_SUCCESS);
}

int32_t vision_get_exposure(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_VISION);
	// This translation comes from VEX to match the brightness represented in vision utility
	int32_t rtn = ((vexDeviceVisionBrightnessGet(device->device_info) * 255) + 50) / 100;
	return_port(port - 1, rtn);
}

int32_t vision_set_auto_white_balance(uint8_t port, const uint8_t enable) {
	if (enable != 0 && enable != 1) {
		errno = EINVAL;
		return PROS_ERR;
	}
	claim_port_i(port - 1, E_DEVICE_VISION);
	vexDeviceVisionWhiteBalanceModeSet(device->device_info, enable + 1);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t vision_set_white_balance(uint8_t port, const int32_t rgb) {
	claim_port_i(port - 1, E_DEVICE_VISION);
	vexDeviceVisionWhiteBalanceModeSet(device->device_info, 2);
	V5_DeviceVisionRgb _rgb = {.red = COLOR2R(rgb), .blue = COLOR2B(rgb), .green = COLOR2G(rgb), .brightness = 255};
	vexDeviceVisionWhiteBalanceSet(device->device_info, _rgb);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t vision_get_white_balance(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_VISION);
	V5_DeviceVisionRgb rgb = vexDeviceVisionWhiteBalanceGet(device->device_info);
	return_port(port - 1, RGB2COLOR(rgb.red, rgb.green, rgb.blue));
}

int32_t vision_set_zero_point(uint8_t port, vision_zero_e_t zero_point) {
	if (!VALIDATE_PORT_NO(port - 1)) {
		errno = ENXIO;
		return PROS_ERR;
	}
	if (registry_validate_binding(port - 1, E_DEVICE_VISION) != 0) {
		errno = ENODEV;
		return PROS_ERR;
	}
	if (!port_mutex_take(port - 1)) {
		errno = EACCES;
		return PROS_ERR;
	}
	set_zero_point(port - 1, zero_point);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t vision_set_wifi_mode(uint8_t port, const uint8_t enable) {
	claim_port_i(port - 1, E_DEVICE_VISION);
	vexDeviceVisionWifiModeSet(device->device_info, !!enable);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t vision_print_signature(const vision_signature_s_t sig) {
	printf("\n\npros::vision_signature_s_t SIG_%d = {", sig.id);
	printf("%d, {%d, %d, %d}, %f, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld};\n\n", sig.id, sig._pad[0], sig._pad[1],
	       sig._pad[2], sig.range, sig.u_min, sig.u_max, sig.u_mean, sig.v_min, sig.v_max, sig.v_mean, sig.rgb, sig.type);
	return 1;
}
