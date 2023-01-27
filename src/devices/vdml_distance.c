/**
 * \file devices/vdml_distance.c
 *
 * Contains functions for interacting with the VEX Distance sensor.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <errno.h>

#include "pros/distance.h"
#include "v5_api.h"
#include "vdml/registry.h"
#include "vdml/vdml.h"

#define ERROR_DISTANCE_BAD_PORT(device, err_return)                 \
	if (!(vexDeviceDistanceStatusGet(device->device_info) == 0x82 ||  \
	      vexDeviceDistanceStatusGet(device->device_info) == 0x86)) { \
		errno = EAGAIN;                                                 \
		return_port(port - 1, err_return);                              \
	}

int32_t distance_get(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_DISTANCE);
	ERROR_DISTANCE_BAD_PORT(device, PROS_ERR);
	int32_t rtn = vexDeviceDistanceDistanceGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t distance_get_confidence(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_DISTANCE);
	ERROR_DISTANCE_BAD_PORT(device, PROS_ERR);
	int32_t rtn = vexDeviceDistanceConfidenceGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t distance_get_object_size(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_DISTANCE);
	ERROR_DISTANCE_BAD_PORT(device, PROS_ERR);
	int32_t rtn = vexDeviceDistanceObjectSizeGet(device->device_info);
	return_port(port - 1, rtn);
}

double distance_get_object_velocity(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_DISTANCE);
	ERROR_DISTANCE_BAD_PORT(device, PROS_ERR);
	double rtn = vexDeviceDistanceObjectVelocityGet(device->device_info);
	return_port(port - 1, rtn);
}
