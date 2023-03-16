/**
 * \file devices/vdml_device.cpp
 *
 * Base class for all smart devices.
 *
 *
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2021, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "pros/device.hpp"
#include "vdml/vdml.h"

namespace pros {
inline namespace v5 {

bool Device::is_installed() {
    std::uint8_t zero_indexed_port = _port - 1;
    port_mutex_take(zero_indexed_port);
    pros::DeviceType plugged_device_type = (pros::DeviceType)pros::c::registry_get_plugged_type(port);
	return_port(zero_indexed_port, _deviceType == plugged_device_type);
}

std::uint8_t Device::get_port(void) {
	return _port;
}

pros::DeviceType Device::get_plugged_type() const {
	if (!port_mutex_take(_port - 1)) {                            
		errno = EACCES; 
		return DeviceType::undefined;                                                                           
	}
	DeviceType type = (DeviceType) pros::c::registry_get_plugged_type(_port - 1);
	
	return_port(_port - 1, type);
}

Device::Device(const std::uint8_t port) : _port(port) {}


}  // namespace v5
}  // namespace pros
