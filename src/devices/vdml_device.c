/**
 * \file devices/vdml_device.c
 *
 * Contains functions for interacting with VEX devices.
 *
 *
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2024, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "pros/device.h"
#include "vdml/vdml.h"

v5_device_e_t get_plugged_type(uint8_t port) {
    if (!port_mutex_take(port - 1)) {                            
		errno = EACCES; 
		return E_DEVICE_UNDEFINED;                                                                           
	}
	v5_device_e_t type = registry_get_plugged_type(port - 1);
	
	return_port(port - 1, type);
}
