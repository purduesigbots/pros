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

namespace pros {
    
Rotation::Rotation(const std::uint8_t port, const bool reverse_flag) : _port(port) {
	pros::c::rotation_set_reversed(port, reverse_flag);
}

std::int32_t Rotation::reset() {
	return pros::c::rotation_reset(_port);
}

std::int32_t Rotation::set_data_rate(std::uint32_t rate) const {
	return pros::c::rotation_set_data_rate(_port, rate);
}

std::int32_t Rotation::set_position(std::uint32_t position) {
	return pros::c::rotation_set_position(_port, position);
}

std::int32_t Rotation::reset_position(void) {
	return pros::c::rotation_reset_position(_port);
}

std::int32_t Rotation::get_position(void) {
    return pros::c::rotation_get_position(_port);
}

std::int32_t Rotation::get_velocity(void) {
    return pros::c::rotation_get_velocity(_port);
}

std::int32_t Rotation::get_angle(void) {
    return pros::c::rotation_get_angle(_port);
}

std::int32_t Rotation::set_reversed(bool value) {
    return pros::c::rotation_set_reversed(_port, value);
}

std::int32_t Rotation::reverse(void) {
    return pros::c::rotation_reverse(_port);
}

std::int32_t Rotation::get_reversed(void) {
    return pros::c::rotation_get_reversed(_port);
}

}  // namespace pros
