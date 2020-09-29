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
#include "pros/distance.h"
#include "v5_api.h"
#include "vdml/registry.h"
#include "vdml/vdml.h"

#define ERROR_DISTANCE_BAD_PORT(port, err_return)                  \
	if (vexDistanceStatusGet(port) == 0x82 || vexDistanceStatusGet(port) == 0x82) { \
		errno = EAGAIN;                                                            \
		return_port(port - 1, err_return);                                         \
	}

int32_t distance_get(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_DISTANCE);
	ERROR_DISTANCE_BAD_PORT(port, PROS_ERR);
	int32_t rtn = vexDistanceDistanceGet(port - 1);
	return_port(port - 1, rtn);
}

int32_t distance_get_confidence(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_DISTANCE);
	ERROR_DISTANCE_BAD_PORT(port, PROS_ERR);
	int32_t rtn = vexDistanceConfidenceGet(port - 1);
	return_port(port - 1, rtn);
}

int32_t distance_get_object_size(uint8_t port){
	claim_port_i(port - 1, E_DEVICE_DISTANCE);
	ERROR_DISTANCE_BAD_PORT(port, PROS_ERR);
	int32_t rtn = vexDistanceObjectSizeGet(port - 1);
	return_port(port - 1, rtn);
}

double distance_get_object_velocity(uint8_t port){
	claim_port_i(port - 1, E_DEVICE_DISTANCE);
	ERROR_DISTANCE_BAD_PORT(port, PROS_ERR);
	int32_t rtn = vexDistanceObjectVelocityGet(port - 1);
	return_port(port - 1, rtn);
}
