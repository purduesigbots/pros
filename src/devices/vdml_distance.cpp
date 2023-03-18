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
#include "vdml/vdml.h"

namespace pros {
inline namespace v5 {

Distance::Distance(const std::uint8_t port) : Device(port, DeviceType::distance) {}

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


std::ostream& operator<<(std::ostream& os, pros::Distance& distance) {
	os << "Distance [";
	os << "port: " << distance.get_port();
	os << ", distance: " << distance.get();
	os << ", confidence: " << distance.get_confidence();
	os << ", object size: " << distance.get_object_size();
	os << ", object velocity: " << distance.get_object_velocity();
	os << "]";
	return os;
}

namespace literals {
const pros::Distance operator"" _dist(const unsigned long long int d) {
    return pros::Distance(d);
}
} // namespace literals
} // namespace v5
}  // namespace pros
