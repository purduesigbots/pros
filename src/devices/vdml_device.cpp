/**
 * \file devices/vdml_device.cpp
 *
 * Contains functions for interacting with the VEX Smart Deivce.
 *
 * Copyright (c) 2017-2022, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "pros/apix.h"
#include "pros/device.hpp"

#define VALIDATE_PORT_NO(PORT) ((PORT) >= 0 && (PORT) < NUM_V5_PORTS)

namespace pros {

Device::Device(const std::uint8_t port, pros::c::v5_device_e_t type): _port(port), _type(type) {
	if (!VALIDATE_PORT_NO(port - 1)) {
		errno = ENXIO;
	} else if (pros::c::registry_get_plugged_type(port - 1) != _type) {
		errno = ENODEV;
	}
}

std::uint8_t Device::get_port() {
	return _port;
}

pros::c::v5_device_e_t Device::get_type() {
	return _type;
}
}  // namespace pros