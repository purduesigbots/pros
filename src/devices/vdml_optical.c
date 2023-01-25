/**
 * \file devices/vdml_optical.c
 *
 * Contains functions for interacting with the VEX Optical sensor.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
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

// Source for these figures: 
// https://www.vexforum.com/t/v5-optical-sensor-refresh-rate/109632/9
#define MIN_INTEGRATION_TIME 3 // ms
#define MAX_INTEGRATION_TIME 712 // ms

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

int32_t optical_set_led_pwm(uint8_t port, uint8_t value) {
	claim_port_i(port - 1, E_DEVICE_OPTICAL);
	vexDeviceOpticalLedPwmSet(device->device_info, value);
	return_port(port - 1, PROS_SUCCESS);
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
	{ .clear = PROS_ERR, .red = PROS_ERR, .green = PROS_ERR, .blue = PROS_ERR }

optical_raw_s_t optical_get_raw(uint8_t port) {
	optical_raw_s_t rtn = RAW_ERR_INIT;
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

optical_direction_e_t optical_get_gesture(uint8_t port) {
	claim_port(port - 1, E_DEVICE_OPTICAL, OPT_GESTURE_ERR);
	optical_direction_e_t rtn = vexDeviceOpticalGestureGet(device->device_info, NULL);
	return_port(port - 1, rtn);
}

#define GESTURE_ERR_INIT                                                                                    \
	{                                                                                                         \
		.udata = OPT_GESTURE_ERR, .ddata = OPT_GESTURE_ERR, .ldata = OPT_GESTURE_ERR, .rdata = OPT_GESTURE_ERR, \
		.type = OPT_GESTURE_ERR, .pad = OPT_GESTURE_ERR, .count = OPT_COUNT_ERR, .time = OPT_TIME_ERR           \
	}

optical_gesture_s_t optical_get_gesture_raw(uint8_t port) {
	optical_gesture_s_t rtn = GESTURE_ERR_INIT;
	v5_smart_device_s_t* device;
	if (!claim_port_try(port - 1, E_DEVICE_OPTICAL)) {
		return rtn;
	}
	device = registry_get_device(port - 1);
	V5_DeviceOpticalGesture gesture;
	vexDeviceOpticalGestureGet(device->device_info, &gesture);
	rtn.udata = gesture.udata;
	rtn.ddata = gesture.ddata;
	rtn.ldata = gesture.ldata;
	rtn.rdata = gesture.rdata;
	rtn.type = gesture.type;
	rtn.pad = gesture.pad;
	rtn.count = gesture.count;
	rtn.time = gesture.time;
	return_port(port - 1, rtn);
}

int32_t optical_enable_gesture(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_OPTICAL);
	vexDeviceOpticalGestureEnable(device->device_info);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t optical_disable_gesture(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_OPTICAL);
	vexDeviceOpticalGestureDisable(device->device_info);
	return_port(port - 1, PROS_SUCCESS);
}

double optical_get_integration_time(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_OPTICAL);
	double rtv = vexDeviceOpticalIntegrationTimeGet(device->device_info);
	return_port(port - 1, rtv);
}

int32_t optical_set_integration_time(uint8_t port, double time) {
	claim_port_i(port - 1, E_DEVICE_OPTICAL);
	// going to set the time to minimum of 3 ms, 3 ms is possible but impractical.
	time = time < MIN_INTEGRATION_TIME ? MIN_INTEGRATION_TIME : time;
	// also boundary limit max integration time too
	time = time > MAX_INTEGRATION_TIME ? MAX_INTEGRATION_TIME : time;

	vexDeviceOpticalIntegrationTimeSet(device->device_info, time);
	return_port(port - 1, PROS_SUCCESS);
}
