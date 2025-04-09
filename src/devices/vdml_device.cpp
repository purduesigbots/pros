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
 * \copyright (c) 2017-2024, Purdue University ACM SIGBots.
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
    pros::DeviceType plugged_device_type = (pros::DeviceType)pros::c::registry_get_plugged_type(zero_indexed_port);
	return_port(zero_indexed_port, _deviceType == plugged_device_type);
}

std::uint8_t Device::get_port(void) const {
	return _port;
}

pros::DeviceType Device::get_plugged_type() const {
	return(get_plugged_type(_port));
}

pros::DeviceType Device::get_plugged_type(std::uint8_t port) {
	if (!port_mutex_take(port - 1)) {                            
		errno = EACCES; 
		return DeviceType::undefined;                                                                           
	}
	DeviceType type = (DeviceType) pros::c::registry_get_plugged_type(port - 1);
	
	return_port(port - 1, type);
}

std::vector<Device> Device::get_all_devices(pros::DeviceType device_type) {
	std::vector<Device> device_list {};

	for (std::uint8_t curr_port = 0; curr_port < 21; ++curr_port) {
		if (!port_mutex_take(curr_port)) {                            
			errno = EACCES;
			continue;
		}

		pros::DeviceType type = (DeviceType) pros::c::registry_get_plugged_type(curr_port);
		if (device_type == type) {;
			device_list.push_back(Device {static_cast<std::uint8_t>(curr_port + 1)});
		}
		port_mutex_give(curr_port);
	}
	return device_list;
}

Device::Device(const std::uint8_t port) : _port(port) {}


}  // namespace v5
}  // namespace pros
