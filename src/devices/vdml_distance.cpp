/**
 * \file devices/vdml_distance.cpp
 *
 * Contains functions for interacting with the VEX Distance sensor.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "pros/distance.hpp"

namespace pros {

Distance::Distance(const std::uint8_t port) : _port(port) {}

std::int32_t Distance::get() {
	return pros::c::distance_get(_port);
}

std::int32_t Distance::get_confidence() {
	return pros::c::distance_get_confidence(_port);
}

std::int32_t Distance::get_object_size() {
	return pros::c::distance_get_object_size(_port);
}

double Distance::get_object_velocity() {
	return pros::c::distance_get_object_velocity(_port);
}

std::uint8_t Distance::get_port() {
	return _port;
}
}  // namespace pros