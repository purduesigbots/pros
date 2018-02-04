/**
 * \file vdml.c
 *
 * \brief VDML Devices Mutex Management.
 *
 * This file ensure thread saftey for operations on motors by maintaining
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
#include "kapi.h"
#include "vdml/registry.h"
#include "vdml/vdml_adi.h"
#include "vdml/vdml_public.h"
#include <errno.h>
#include <math.h>
#include <stdio.h>

#define INTERNAL_ADI_PORT 21

#define transform_adi_port(port)                                                                                       \
	if (port >= 'a' && port <= 'h')                                                                                \
		port -= 'a';                                                                                           \
	else if (port >= 'A' && port <= 'H')                                                                           \
		port -= 'A';                                                                                           \
	else                                                                                                           \
		port--;                                                                                                \
	if (port > 7 || port < 0) {                                                                                    \
		errno = EINVAL;                                                                                        \
		return PROS_ERR;                                                                                       \
	}

int32_t adi_port_config_set(int port, adi_port_config_e_t type) {
	transform_adi_port(port);
	claim_port(INTERNAL_ADI_PORT, E_DEVICE_ADI);
	vexDeviceAdiPortConfigSet(device.device_info, port, type);
	return_port(INTERNAL_ADI_PORT, 1);
}

adi_port_config_e_t adi_port_config_get(int port) {
	transform_adi_port(port);
	claim_port(INTERNAL_ADI_PORT, E_DEVICE_ADI);
	adi_port_config_e_t rtn = (adi_port_config_e_t)vexDeviceAdiPortConfigGet(device.device_info, port);
	return_port(INTERNAL_ADI_PORT, rtn);
}

int32_t adi_value_set(int port, int32_t value) {
	transform_adi_port(port);
	claim_port(INTERNAL_ADI_PORT, E_DEVICE_ADI);
	vexDeviceAdiValueSet(device.device_info, port, value);
	return_port(INTERNAL_ADI_PORT, 1);
}

int32_t adi_value_get(int port) {
	transform_adi_port(port);
	claim_port(INTERNAL_ADI_PORT, E_DEVICE_ADI);
	int32_t rtn = vexDeviceAdiValueGet(device.device_info, port);
	return_port(INTERNAL_ADI_PORT, rtn);
}
