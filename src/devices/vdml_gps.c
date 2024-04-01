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

double gps_get_position_x(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_GPS);
	V5_DeviceGpsAttitude data;
	vexDeviceGpsAttitudeGet(device->device_info, &data, false);
	double rtv = data.position_x;
	return_port(port - 1, rtv);
}

double gps_get_position_y(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_GPS);
	V5_DeviceGpsAttitude data;
	vexDeviceGpsAttitudeGet(device->device_info, &data, false);
	double rtv = data.position_y;
	return_port(port - 1, rtv);
}

double gps_get_pitch(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_GPS);
	V5_DeviceGpsAttitude data;
	vexDeviceGpsAttitudeGet(device->device_info, &data, false);
	double rtv = data.pitch;
	return_port(port - 1, rtv);
}

double gps_get_roll(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_GPS);
	V5_DeviceGpsAttitude data;
	vexDeviceGpsAttitudeGet(device->device_info, &data, false);
	double rtv = data.roll;
	return_port(port - 1, rtv);
}

double gps_get_yaw(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_GPS);
	V5_DeviceGpsAttitude data;
	vexDeviceGpsAttitudeGet(device->device_info, &data, false);
	double rtv = data.yaw;
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

double gps_get_gyro_rate_x(uint8_t port) {
    claim_port_f(port - 1, E_DEVICE_GPS);
	V5_DeviceGpsRaw data;
	vexDeviceGpsRawGyroGet(device->device_info, &data);
	double rtv = data.x;
	return_port(port - 1, rtv);	
}

double gps_get_gyro_rate_y(uint8_t port) {
    claim_port_f(port - 1, E_DEVICE_GPS);
	V5_DeviceGpsRaw data;
	vexDeviceGpsRawGyroGet(device->device_info, &data);
	double rtv = data.y;
	return_port(port - 1, rtv);
}

double gps_get_gyro_rate_z(uint8_t port) {
    claim_port_f(port - 1, E_DEVICE_GPS);
	V5_DeviceGpsRaw data;
	vexDeviceGpsRawGyroGet(device->device_info, &data);
	double rtv = data.z;
	return_port(port - 1, rtv);
}

double gps_get_accel_x(uint8_t port) {
    claim_port_f(port - 1, E_DEVICE_GPS);
	V5_DeviceGpsRaw data;
	vexDeviceGpsRawAccelGet(device->device_info, &data);
	double rtv = data.x;
	return_port(port - 1, rtv);
}

double gps_get_accel_y(uint8_t port) {
    claim_port_f(port - 1, E_DEVICE_GPS);
	V5_DeviceGpsRaw data;
	vexDeviceGpsRawAccelGet(device->device_info, &data);
	double rtv = data.y;
	return_port(port - 1, rtv);
}

double gps_get_accel_z(uint8_t port) {
    claim_port_f(port - 1, E_DEVICE_GPS);
	V5_DeviceGpsRaw data;
	vexDeviceGpsRawAccelGet(device->device_info, &data);
	double rtv = data.z;
	return_port(port - 1, rtv);
}