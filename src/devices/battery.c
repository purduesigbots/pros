/**
 * \file devices/battery.c
 *
 * Contains functions for interacting with the V5 Battery.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "kapi.h"
#include "v5_api.h"
#include "vdml/vdml.h"

int32_t battery_get_voltage(void) {
	if (!internal_port_mutex_take(V5_PORT_BATTERY)) {
		errno = EACCES;
		return PROS_ERR;
	}
	double rtn = vexBatteryVoltageGet();
	internal_port_mutex_give(V5_PORT_BATTERY);
	return rtn;
}

int32_t battery_get_current(void) {
	if (!internal_port_mutex_take(V5_PORT_BATTERY)) {
		errno = EACCES;
		return PROS_ERR;
	}
	double rtn = vexBatteryCurrentGet();
	internal_port_mutex_give(V5_PORT_BATTERY);
	return rtn;
}

double battery_get_temperature(void) {
	if (!internal_port_mutex_take(V5_PORT_BATTERY)) {
		errno = EACCES;
		return PROS_ERR_F;
	}
	double rtn = vexBatteryTemperatureGet();
	internal_port_mutex_give(V5_PORT_BATTERY);
	return rtn;
}

double battery_get_capacity(void) {
	if (!internal_port_mutex_take(V5_PORT_BATTERY)) {
		errno = EACCES;
		return PROS_ERR_F;
	}
	double rtn = vexBatteryCapacityGet();
	internal_port_mutex_give(V5_PORT_BATTERY);
	return rtn;
}
