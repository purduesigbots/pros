/**
 * \file devices/vdml_rotation.cpp
 *
 * Contains functions for interacting with the VEX Rotation sensor.
 *
 * Copyright (c) 2017-2022, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "pros/rotation.hpp"

namespace pros {
inline namespace v5 {

Rotation::Rotation(const std::uint8_t port, const bool reverse_flag) : _port(port) {
	pros::c::rotation_init_reverse(port, reverse_flag);
}

std::int32_t Rotation::reset() const {
	push_rotation_configuration();
	return pros::c::rotation_reset(_port);
}

std::int32_t Rotation::set_data_rate(std::uint32_t rate) const {
	push_rotation_configuration();
	return pros::c::rotation_set_data_rate(_port, rate);
}

std::int32_t Rotation::set_position(std::uint32_t position) const {
	push_rotation_configuration();
	return pros::c::rotation_set_position(_port, position);
}

std::int32_t Rotation::reset_position(void) const {
	push_rotation_configuration();
	return pros::c::rotation_reset_position(_port);
}

std::int32_t Rotation::get_position(void) const {
	push_rotation_configuration();
	return pros::c::rotation_get_position(_port);
}

std::int32_t Rotation::get_velocity(void) const {
	push_rotation_configuration();
	return pros::c::rotation_get_velocity(_port);
}

std::int32_t Rotation::get_angle(void) const {
	push_rotation_configuration();
	return pros::c::rotation_get_angle(_port);
}

std::int32_t Rotation::set_reversed(bool value) const {
	_reverse_flag = value;
	return pros::c::rotation_set_reversed(_port, value);
}

std::int32_t Rotation::reverse(void) const {
	push_rotation_configuration();
	return pros::c::rotation_reverse(_port);
}

std::int32_t Rotation::get_reversed(void) const {
	return _reverse_flag;
}

void Rotation::push_rotation_configuration() const {
	_rotation_mutex.take();
	set_reversed(_reverse_flag);
	_rotation_mutex.give();
}

pros::Rotation& pros::Rotation::operator=(const pros::Rotation& other) {
  _port = other._port;
  _reverse_flag = other._reverse_flag;
  return *this;
}

}  // namespace v5
}  // namespace pros
