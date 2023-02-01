/**
 * \file devices/vdml_gps.c
 *
 * Contains functions for interacting with the VEX GPS.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <errno.h>

#include "pros/gps.h"
#include "v5_api.h"
#include "vdml/registry.h"
#include "vdml/vdml.h"

#define GPS_MINIMUM_DATA_RATE 5

#define GPS_STATUS_ERR_INIT \
	{ .x = PROS_ERR_F, .y = PROS_ERR_F, .roll = PROS_ERR_F, .pitch = PROS_ERR_F, .yaw = PROS_ERR_F }

#define GPS_RAW_ERR_INIT \
	{ .x = PROS_ERR_F, .y = PROS_ERR_F, .z = PROS_ERR_F }

int32_t gps_initialize_full(uint8_t port, double xInitial, double yInitial, double headingInitial, double xOffset,
                            double yOffset) {
	claim_port_i(port - 1, E_DEVICE_GPS);
	vexDeviceGpsOriginSet(device->device_info, xOffset, yOffset);
	vexDeviceGpsInitialPositionSet(device->device_info, xInitial, yInitial, headingInitial);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t gps_set_offset(uint8_t port, double xOffset, double yOffset) {
	claim_port_i(port - 1, E_DEVICE_GPS);
	vexDeviceGpsOriginSet(device->device_info, xOffset, yOffset);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t gps_get_offset(uint8_t port, double* xOffset, double* yOffset) {
	claim_port_i(port - 1, E_DEVICE_GPS);
	vexDeviceGpsOriginGet(device->device_info, xOffset, yOffset);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t gps_set_position(uint8_t port, double xInitial, double yInitial, double headingInitial) {
	claim_port_i(port - 1, E_DEVICE_GPS);
	vexDeviceGpsInitialPositionSet(device->device_info, xInitial, yInitial, headingInitial);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t gps_set_data_rate(uint8_t port, uint32_t rate) {
	claim_port_i(port - 1, E_DEVICE_GPS);

	// rate is not less than 5ms, and rounded down to nearest increment of 5
	if (rate < GPS_MINIMUM_DATA_RATE) {
		rate = GPS_MINIMUM_DATA_RATE;
	} else {
		rate -= rate % GPS_MINIMUM_DATA_RATE;
	}

	vexDeviceGpsDataRateSet(device->device_info, rate);
	return_port(port - 1, PROS_SUCCESS);
}

double gps_get_error(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_GPS);
	double rtv = vexDeviceGpsErrorGet(device->device_info);
	return_port(port - 1, rtv);
}

gps_status_s_t gps_get_status(uint8_t port) {
	gps_status_s_t rtv = GPS_STATUS_ERR_INIT;
	if (!claim_port_try(port - 1, E_DEVICE_GPS)) {
		return rtv;
	}
	v5_smart_device_s_t* device = registry_get_device(port - 1);
	V5_DeviceGpsAttitude data;
	vexDeviceGpsAttitudeGet(device->device_info, &data, false);
	rtv.x = data.position_x;
	rtv.y = data.position_y;
	rtv.pitch = data.pitch;
	rtv.roll = data.roll;
	rtv.yaw = data.yaw;
	return_port(port - 1, rtv);
}

double gps_get_heading(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_GPS);
	double rtv = vexDeviceGpsDegreesGet(device->device_info);
	return_port(port - 1, rtv);
}

double gps_get_heading_raw(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_GPS);
	double rtv = vexDeviceGpsHeadingGet(device->device_info);
	return_port(port - 1, rtv);
}

double gps_get_rotation(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_GPS);
	double rtv = vexDeviceGpsRotationGet(device->device_info);
	return_port(port - 1, rtv);
}

int32_t gps_set_rotation(uint8_t port, double target) {
	claim_port_i(port - 1, E_DEVICE_GPS);
	vexDeviceGpsRotationSet(device->device_info, target);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t gps_tare_rotation(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_GPS);
	vexDeviceGpsRotationSet(device->device_info, 0);
	return_port(port - 1, PROS_SUCCESS);
}

gps_gyro_s_t gps_get_gyro_rate(uint8_t port) {
	gps_gyro_s_t rtv = GPS_RAW_ERR_INIT;
	if (!claim_port_try(port - 1, E_DEVICE_GPS)) {
		return rtv;
	}
	v5_smart_device_s_t* device = registry_get_device(port - 1);
	V5_DeviceGpsRaw data;
	vexDeviceGpsRawGyroGet(device->device_info, &data);
	rtv.x = data.x;
	rtv.y = data.y;
	rtv.z = data.z;
	return_port(port - 1, rtv);
}

gps_accel_s_t gps_get_accel(uint8_t port) {
	gps_accel_s_t rtv = GPS_RAW_ERR_INIT;
	if (!claim_port_try(port - 1, E_DEVICE_GPS)) {
		return rtv;
	}
	v5_smart_device_s_t* device = registry_get_device(port - 1);
	V5_DeviceGpsRaw data;
	vexDeviceGpsRawAccelGet(device->device_info, &data);
	rtv.x = data.x;
	rtv.y = data.y;
	rtv.z = data.z;
	return_port(port - 1, rtv);
}
