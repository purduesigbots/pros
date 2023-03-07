/**
 * \file devices/vdml_rotation.cpp
 *
 * Contains functions for interacting with the VEX Rotation sensor.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "pros/rotation.hpp"
#include "pros/apix.h"
#include "vdml/vdml.h"

namespace pros {
inline namespace v5 {
    
Rotation::Rotation(const std::uint8_t port, const bool reverse_flag) : Device(port) {
	pros::c::rotation_set_reversed(port, reverse_flag);
}

bool Rotation::is_installed() {
	std::uint8_t port = this->_port - 1;
    port_mutex_take(port);
    pros::c::v5_device_e_t deviceType = c::registry_get_plugged_type(port);
    port_mutex_give(port);
    if (deviceType == c::E_DEVICE_ROTATION) {
        return true;
    }
    return false;
}

std::int32_t Rotation::reset() {
	return pros::c::rotation_reset(_port);
}

std::int32_t Rotation::set_data_rate(std::uint32_t rate) const {
	return pros::c::rotation_set_data_rate(_port, rate);
}

std::int32_t Rotation::set_position(std::uint32_t position) const {
	return pros::c::rotation_set_position(_port, position);
}

std::int32_t Rotation::reset_position(void) const {
	return pros::c::rotation_reset_position(_port);
}

std::int32_t Rotation::get_position(void) const {
	return pros::c::rotation_get_position(_port);
}

std::int32_t Rotation::get_velocity(void) const {
	return pros::c::rotation_get_velocity(_port);
}

std::int32_t Rotation::get_angle(void) const {
	return pros::c::rotation_get_angle(_port);
}

std::int32_t Rotation::set_reversed(bool value) const {
	return pros::c::rotation_set_reversed(_port, value);
}

std::int32_t Rotation::reverse(void) const {
	return pros::c::rotation_reverse(_port);
}

std::int32_t Rotation::get_reversed(void) const {
	return pros::c::rotation_get_reversed(_port);
}

std::ostream& operator<<(std::ostream& os, const pros::Rotation& rotation) {
	os << "Rotation [";
	os << "port: " << rotation._port;
	os << ", position: " << rotation.get_position();
	os << ", velocity: " << rotation.get_velocity();
	os << ", angle: " << rotation.get_angle();
	os << ", reversed: " << rotation.get_reversed();
	os << "]";
	return os;
}

pros::DeviceType Rotation::get_type() const {
	return pros::DeviceType::rotation;
}
namespace literals {
const pros::Rotation operator"" _rot(const unsigned long long int r) {
    return pros::Rotation(r);
}
} // namespace literals 

}  // namespace v5
}  // namespace pros
