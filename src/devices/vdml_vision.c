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

/**
 * Returns the number of objects currently detected on the Vision Sensor
 *
 * \param port
 *      The port number of the Vision Sensor
 * \return
 *      The number of objects detected on the specified vision sensor.
 *      Returns PROS_ERR if the port was invalid or an error occurred.
 */
int32_t vision_get_object_count(const int32_t port) {
	claim_port(port - 1, E_DEVICE_VISION);
	int32_t rtn = vexDeviceVisionObjectCountGet(device.device_info);
	return_port(port - 1, rtn);
}

/**
 * Copies the specified object descriptor into object_ptr.
 *
 * \param port
 *      The port number of the Vision Sensor
 * \param object_id
 *      Which object to read.
 * \param object_ptr
 *      A pointer to copy the data into
 * \return
 *      1 if the data was successfuly copied
 *      Returns PROS_ERR if the port was invalid, the object_id was out of range, or an error occurred.
 */
int32_t vision_read_object(const int32_t port, const uint32_t object_id, vision_object_s_t* object_ptr) {
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

/**
 * Reads up to object_count object descriptors into object_arr.
 *
 * \param port
 *      The port number of the Vision Sensor
 * \param object_count
 *      Which object to read.
 * \param object_arr
 *      A pointer to copy the data into
 * \return
 *      The number of object signatures copied
 *      Returns PROS_ERR if the port was invalid or an error occurred.
 */
int32_t vision_read_objects(const int32_t port, const uint32_t object_count, vision_object_s_t* object_arr) {
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

/**
 * Returns the object descriptor at object_id.
 *
 * \note This function is slightly less performant than vision_read_object since the object descriptor
 *       must be copied at the end of the function call. This may not be an issue for most users.
 *
 * \param port
 *      The port number of the Vision Sensor
 * \param object_id
 *      Which object to read.
 * \return
 *      An object descriptor. If the object_id was invalid or an error otherwise occurred, then the object
 *      signature will be set to 255.
 */
vision_object_s_t vision_get_object(const int32_t port, const uint32_t object_id) {
	vision_object_s_t object;
	if (vision_read_object(port, object_id, &object) == PROS_ERR) {
		object.signature = 255;
	}
	return object;
}

/**
 * Loads the object detection signature into the supplied pointer to memory.
 *
 * \param port
 *      The port number of the Vision Sensor
 * \param signature_id
 *      The signature id to read
 * \param signature_ptr
 *      Pointer to load the signature into
 * \return
 *      Returns 0 if no errors occurred, PROS_ERR otherwise
 */
int32_t vision_read_signature(const int32_t port, const uint8_t signature_id, vision_signature_s_t* signature_ptr) {
	if (signature_id > 8 || signature_id == 0) {
		errno = EINVAL;
		return PROS_ERR;
	}
	claim_port(port - 1, E_DEVICE_VISION);
	int32_t rtn =
	    vexDeviceVisionSignatureGet(device.device_info, signature_id, (V5_DeviceVisionSignature*)signature_ptr);
	return_port(port - 1, rtn);
}

/**
 * Saves the supplied object detection signature
 *
 * \param port
 *      The port number of the Vision Sensor
 * \param signature_id
 *      The signature id to store into
 * \param signature_ptr
 *      Pointer to the signature to save
 * \return
 *      Returns 0 if no errors occured, PROS_ERR otherwise
 */
int32_t vision_save_signature(const int32_t port, const uint8_t signature_id, vision_signature_s_t* signature_ptr) {
	if (signature_id > 8 || signature_id == 0) {
		errno = EINVAL;
		return PROS_ERR;
	}
	signature_ptr->id = signature_id;
	claim_port(port - 1, E_DEVICE_VISION);
	vexDeviceVisionSignatureSet(device.device_info, (V5_DeviceVisionSignature*)signature_ptr);
	return_port(port - 1, 0);
}

/**
 * Sets the vision sensor LED color, overriding the automatic behavior
 *
 * \param port
 *      The port number of the Vision Sensor
 * \param rgb
 *      An RGB code to set the LED to
 * \return
 *      Returns 0 if no errors occured, PROS_ERR otherwise
 */
int32_t vision_set_led(const int32_t port, const int32_t rgb) {
	claim_port(port - 1, E_DEVICE_VISION);
	vexDeviceVisionLedModeSet(device.device_info, 1);
	V5_DeviceVisionRgb _rgb = {
		.red = COLOR2R(rgb), .blue = COLOR2B(rgb), .green = COLOR2G(rgb), .brightness = 255
	};
	vexDeviceVisionLedColorSet(device.device_info, _rgb);
	return_port(port - 1, 0);
}

/**
 * Clears the vision sensor LED color, reseting it back to its default behavior,
 * displaying the most prominent object signature color
 *
 * \param port
 *      The port number of the Vision Sensor
 * \return
 *      Returns 0 if no errors occurred, PROS_ERR otherwise
 */
int32_t vision_clear_led(const int32_t port) {
	claim_port(port - 1, E_DEVICE_VISION);
	vexDeviceVisionLedModeSet(device.device_info, 0);
	return_port(port - 1, 0);
}

/**
 * Sets the exposure parameter of the Vision Sensor
 *
 * \param port
 *       The port number of the Vision Sensor
 * \param percent
 * \return
 *       Returns 0 if no erros occurred, PROS_ERR otherwise
 */
int32_t vision_set_exposure(const int32_t port, const uint8_t percent) {
	claim_port(port - 1, E_DEVICE_VISION);
	vexDeviceVisionBrightnessSet(device.device_info, percent);
	return_port(port - 1, 0);
}

/**
 * Gets the exposure parameter of the Vision Sensor
 *
 * \param port
 *       The port number of the Vision Sensor
 * \return
 *       Returns the current brightness percentage parameter from [0,100], PROS_ERR if an error occurred
 */
int32_t vision_get_exposure(const int32_t port) {
	claim_port(port - 1, E_DEVICE_VISION);
	int32_t rtn = vexDeviceVisionBrightnessGet(device.device_info);
	return_port(port - 1, rtn);
}

/**
 * Enable/disable auto white-balancing on the Vision Sensor
 *
 * \param port
 * 		The port number of the Vision Sensor
 * \param enabled
 * 		Pass 0 to disable, 1 to enable
 * \return
 *       Returns 0 if no errors occurred, PROS_ERR otherwise
 */
int32_t vision_set_auto_white_balance(const int32_t port, const uint8_t enable) {
	if (enable != 0 && enable != 1) {
		errno = EINVAL;
		return PROS_ERR;
	}
	claim_port(port - 1, E_DEVICE_VISION);
	vexDeviceVisionWhiteBalanceModeSet(device.device_info, enable + 1);
	return_port(port - 1, 0);
}

/**
 * Set the white-balance parameter manually on the Vision Sensor.
 * This function will disable auto white-balancing
 *
 * \param port
 * 		The port number of the Vision Sensor
 * \param rgb
 * 		The white balance parameter
 * \return
 * 		Returns 0 if no errros ooccurred, PROS_ERR otherwise
 */
int32_t vision_set_white_balance(const int32_t port, const int32_t rgb) {
	claim_port(port - 1, E_DEVICE_VISION);
	vexDeviceVisionWhiteBalanceModeSet(device.device_info, 2);
	V5_DeviceVisionRgb _rgb = {
		.red = COLOR2R(rgb), .blue = COLOR2B(rgb), .green = COLOR2G(rgb), .brightness = 255
	};
	vexDeviceVisionWhiteBalanceSet(device.device_info, _rgb);
	return_port(port - 1, 0);
}

/**
 * Get the white balance parameter of the Vision Sensor
 *
 * \param port
 * 		The port number of the Vision Sensor
 * \return
 * 		Returns the current RGB white balance setting of the sensor
 */
int32_t vision_get_white_balance(const int32_t port) {
	claim_port(port - 1, E_DEVICE_VISION);
	V5_DeviceVisionRgb rgb = vexDeviceVisionWhiteBalanceGet(device.device_info);
	return_port(port - 1, RGB2COLOR(rgb.red, rgb.green, rgb.blue));
}
