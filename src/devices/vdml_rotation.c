/**
 * \file devices/vdml_rotation.c
 *
 * Contains functions for interacting with the VEX Rotation sensor.
 *
 * Copyright (c) 2017-2019, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <errno.h>
#include "pros/rotation.h"
#include "v5_api.h"
#include "vdml/registry.h"
#include "vdml/vdml.h"

int32_t rotation_reset(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_DISTANCE);
	vexDeviceAbsEncReset(device->device_info);
	return_port(port - 1, 1);
}

int32_t rotation_set_position(uint8_t port, uint32_t position) {
	claim_port_i(port - 1, E_DEVICE_DISTANCE);
	vexDeviceAbsEncPositionSet(device->device_info, position);
	return_port(port - 1, 1);
}

int32_t rotation_get_position(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_DISTANCE);
	vexDeviceAbsEncPositionGet(device->device_info);
	return_port(port - 1, 1);
}

int32_t rotation_get_velocity(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_DISTANCE);
	vexDeviceAbsEncVelocityGet(device->device_info);
	return_port(port - 1, 1);
}

int32_t rotation_get_angle(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_DISTANCE);
	vexDeviceAbsEncAngleGet(device->device_info);
	return_port(port - 1, 1);
}

int32_t rotation_set_reversed(uint8_t port, bool value) {
	claim_port_i(port - 1, E_DEVICE_DISTANCE);
	vexDeviceAbsReverseFlagSet(device->device_info, value);
	return_port(port - 1, 1);
}

int32_t rotation_get_reversed(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_DISTANCE);
	vexDeviceAbsReverseFlagGet(device->device_info);
	return_port(port - 1, 1);
}