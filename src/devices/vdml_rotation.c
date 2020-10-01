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
	ERROR_IMU_STILL_CALIBRATING(port, device, PROS_ERR);
	vexDeviceAbsEncReset(device->device_info);
	return_port(port - 1, 1);
}


