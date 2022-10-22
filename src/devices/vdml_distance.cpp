/**
 * \file devices/vdml_distance.cpp
 *
 * Contains functions for interacting with the VEX Distance sensor.
 *
 * Copyright (c) 2017-2022, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "pros/distance.hpp"

namespace pros {
inline namespace v5{ 
	
Distance::Distance(const std::uint8_t port) : _port(port) {}

std::int32_t Distance::get() const {
	push_distance_configuration();
	return pros::c::distance_get(_port);
}

std::int32_t Distance::get_confidence() const {
	push_distance_configuration();
	return pros::c::distance_get_confidence(_port);
}

std::int32_t Distance::get_object_size() const {
	push_distance_configuration();
	return pros::c::distance_get_object_size(_port);
}

double Distance::get_object_velocity() const {
	push_distance_configuration();
	return pros::c::distance_get_object_velocity(_port);
}

std::uint8_t Distance::get_port() const {
	return _port;
}

}
}  // namespace pros