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
    pros::DeviceType plugged_device_type = (pros::DeviceType)pros::c::registry_get_plugged_type(zero_indexed_port);
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

template <typename T>
std::vector<T> Device::get_all_devices() {
	DeviceType device_type {};

	if(std::is_same<T,Motor>::value)
		device_type = DeviceType::motor;
	else if (std::is_same<T,Rotation>::value)
		device_type = DeviceType::motor;
	else if (std::is_same<T,IMU>::value)
		device_type = DeviceType::imu;
	else if (std::is_same<T,Distance>::value)
		device_type = DeviceType::distance;
	else if (std::is_same<T,Vision>::value)
		device_type = DeviceType::vision;
	else if (std::is_same<T,Optical>::value)
		device_type = DeviceType::optical;
	else if (std::is_same<T,Gps>::value)
		device_type = DeviceType::gps;
	else if (std::is_same<T,Serial>::value)
		device_type = DeviceType::serial;
	else
		device_type = DeviceType::undefined;

	std::vector<T> device_list {};

	for (std::uint8_t curr_port = 0; curr_port < 21; ++curr_port) {
		if (!port_mutex_take(curr_port)) {                            
			errno = EACCES;
			continue;
		}

        DeviceType type = (DeviceType) c::registry_get_plugged_type(curr_port);
		if (device_type == type) {
			device_list.push_back(T {curr_port});
		}

		return_port(curr_port, type);
	}

	return device_list;
}

Device::Device(const std::uint8_t port) : _port(port) {}


}  // namespace v5
}  // namespace pros
