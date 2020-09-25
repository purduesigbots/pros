/**
 * \file devices/vdml_imu.c
 *
 * Contains functions for interacting with the VEX Inertial sensor.
 *
 * Copyright (c) 2017-2019, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <errno.h>

#include "pros/optical.h"
#include "v5_api.h"
#include "vdml/registry.h"
#include "vdml/vdml.h"

double optical_get_hue(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_OPTICAL);
	double rtn = vexDeviceOpticalHueGet(device->device_info);
	return_port(port - 1, rtn);
}

double optical_get_saturation(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_OPTICAL);
	double rtn = vexDeviceOpticalSatGet(device->device_info);
	return_port(port - 1, rtn);
}

double optical_get_brightness(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_OPTICAL);
	double rtn = vexDeviceOpticalBrightnessGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t optical_get_proximity(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_OPTICAL);
	double rtn = vexDeviceOpticalProximityGet(device->device_info);
	return_port(port - 1, rtn);
}

void optical_set_led_pwm(uint8_t port, uint8_t value) {
	claim_port_i(port - 1, E_DEVICE_OPTICAL);
	vexDeviceOpticalLedPwmSet(device->device_info, value);
	return_port(port - 1, 1);
}

int32_t optical_get_led_pwm(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_OPTICAL);
	int32_t rtn = vexDeviceOpticalLedPwmGet(device->device_info);
	return_port(port - 1, rtn);
}

#define RGB_ERR_INIT \
	{ .red = PROS_ERR_F, .green = PROS_ERR_F, .blue = PROS_ERR_F, .brightness = PROS_ERR_F }

optical_rgb_s_t optical_get_rgb(uint8_t port) {
	optical_rgb_s_t rtn = RGB_ERR_INIT;
	v5_smart_device_s_t* device;
	if (!claim_port_try(port - 1, E_DEVICE_OPTICAL)) {
		return rtn;
	}
	device = registry_get_device(port - 1);
	V5_DeviceOpticalRgb rgb;
	vexDeviceOpticalRgbGet(device->device_info, &rgb);
	rtn.red = rgb.red;
	rtn.green = rgb.green;
	rtn.blue = rgb.blue;
	rtn.brightness = rgb.brightness;
	return_port(port - 1, rtn);
}

#define RAW_ERR_INIT \
	{ .clear = PROS_ERR_F, .red = PROS_ERR_F, .green = PROS_ERR_F, .blue = PROS_ERR_F }

optical_raw_s_t optical_get_raw(uint8_t port) {
	optical_raw_s_t rtn = RGB_ERR_INIT;
	v5_smart_device_s_t* device;
	if (!claim_port_try(port - 1, E_DEVICE_OPTICAL)) {
		return rtn;
	}
	device = registry_get_device(port - 1);
	V5_DeviceOpticalRaw rgb;
	vexDeviceOpticalRawGet(device->device_info, &rgb);
	rtn.clear = rgb.clear;
	rtn.red = rgb.red;
	rtn.green = rgb.green;
	rtn.blue = rgb.blue;
	return_port(port - 1, rtn);
}

#define GESTURE_ERR_INIT                                                                                    \
	{                                                                                                         \
		.udata = PROS_ERR_F, .ddata = PROS_ERR_F, .ldata = PROS_ERR_F, .rdata = PROS_ERR_F, .type = PROS_ERR_F, \
		.pad = PROS_ERR_F, .count = PROS_ERR_F, .time = PROS_ERR_F                                              \
	}

int32_t optical_get_gesture(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_OPTICAL);
	V5_DeviceOpticalGesture gesture = GESTURE_ERR_INIT;
	int32_t rtn = vexDeviceOpticalGestureGet(device->device_info, &gesture);
	return_port(port - 1, rtn);
}

void optical_enable_gesture(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_OPTICAL);
	vexDeviceOpticalGestureEnable(device->device_info);
	return_port(port - 1, 1);
}

void optical_disable_gesture(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_OPTICAL);
	vexDeviceOpticalGestureEnable(device->device_info);
	return_port(port - 1, 1);
}
