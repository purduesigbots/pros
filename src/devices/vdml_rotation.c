/**
 * \file devices/vdml_rotation.c
 *
 * Contains functions for interacting with the VEX Rotation sensor.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
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

#define ROTATION_RESET_TIMEOUT 1000

int32_t rotation_reset(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_ROTATION);
	vexDeviceAbsEncReset(device->device_info);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t rotation_set_data_rate(uint8_t port, uint32_t rate) {
	claim_port_i(port - 1, E_DEVICE_ROTATION);

	// rate is not less than 5ms, and rounded down to nearest increment of 5
	if (rate < ROTATION_MINIMUM_DATA_RATE) {
		rate = ROTATION_MINIMUM_DATA_RATE;
	} else {
		rate -= rate % ROTATION_MINIMUM_DATA_RATE;
	}

	vexDeviceAbsEncDataRateSet(device->device_info, rate);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t rotation_reset_position(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_ROTATION);
	vexDeviceAbsEncPositionSet(device->device_info, 0);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t rotation_set_position(uint8_t port, uint32_t position) {
	claim_port_i(port - 1, E_DEVICE_ROTATION);
	vexDeviceAbsEncPositionSet(device->device_info, position);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t rotation_get_position(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_ROTATION);
	int32_t rtn = vexDeviceAbsEncPositionGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t rotation_get_velocity(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_ROTATION);
	int32_t rtn = vexDeviceAbsEncVelocityGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t rotation_get_angle(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_ROTATION);
	int32_t rtn = vexDeviceAbsEncAngleGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t rotation_set_reversed(uint8_t port, bool value) {
	claim_port_i(port - 1, E_DEVICE_ROTATION);
	vexDeviceAbsEncReverseFlagSet(device->device_info, value);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t rotation_init_reverse(uint8_t port, bool reverse_flag) {
	claim_port_i(port - 1, E_DEVICE_ROTATION);
    uint16_t timeoutCount = 0;
    // releasing mutex so vexBackgrounProcessing can run without being blocked.
    do {
        port_mutex_give(port - 1);
        task_delay(5);
        timeoutCount += 5;
        claim_port_i(port - 1, E_DEVICE_ROTATION);
        if (timeoutCount >= ROTATION_RESET_TIMEOUT) {
            port_mutex_give(port - 1);
            errno = EAGAIN;
            return PROS_ERR;
        }
        device = device; // suppressing compiler warning
    } while(vexDeviceAbsEncStatusGet(device->device_info) == 0);
    vexAbsEncReverseFlagSet(port - 1, reverse_flag);
    return_port(port - 1, 1)
}

int32_t rotation_reverse(uint8_t port){
	claim_port_i(port - 1, E_DEVICE_ROTATION);
	vexDeviceAbsEncReverseFlagSet(device->device_info, !vexDeviceAbsEncReverseFlagGet(device->device_info));
	return_port(port - 1, PROS_SUCCESS);
}

int32_t rotation_get_reversed(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_ROTATION);
	int32_t rtn = vexDeviceAbsEncReverseFlagGet(device->device_info);
	return_port(port - 1, rtn);
}