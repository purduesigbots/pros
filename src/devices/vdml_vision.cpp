/**
 * \file devices/vdml_vision.cpp
 *
 * Contains functions for interacting with the V5 Vision Sensor.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "kapi.h"

namespace pros {
using namespace pros::c;

Vision::Vision(std::uint8_t port, vision_zero_e_t zero_point) : _port(port) {
	vision_set_zero_point(port, zero_point);
}

std::int32_t Vision::clear_led(void) const {
	return vision_clear_led(_port);
}

vision_object_s_t Vision::get_by_size(const std::uint32_t size_id) const {
	return vision_get_by_size(_port, size_id);
}

vision_object_s_t Vision::get_by_sig(const std::uint32_t size_id, const std::uint32_t sig_id) const {
	return vision_get_by_sig(_port, size_id, sig_id);
}

int32_t Vision::get_exposure(void) const {
	return vision_get_exposure(_port);
}

int32_t Vision::get_object_count(void) const {
	return vision_get_object_count(_port);
}

std::int32_t Vision::get_white_balance(void) const {
	return vision_get_white_balance(_port);
}

int32_t Vision::read_by_size(const std::uint32_t size_id, const std::uint32_t object_count,
                             vision_object_s_t* const object_arr) const {
	return vision_read_by_size(_port, size_id, object_count, object_arr);
}

int32_t Vision::read_by_sig(const std::uint32_t size_id, const std::uint32_t sig_id, const std::uint32_t object_count,
                            vision_object_s_t* const object_arr) const {
	return vision_read_by_sig(_port, size_id, sig_id, object_count, object_arr);
}

std::int32_t Vision::set_auto_white_balance(const std::uint8_t enable) const {
	return vision_set_auto_white_balance(_port, enable);
}

std::int32_t Vision::set_exposure(const std::uint8_t percent) const {
	return vision_set_exposure(_port, percent);
}

std::int32_t Vision::set_led(const std::int32_t rgb) const {
	return vision_set_led(_port, rgb);
}

std::int32_t Vision::set_white_balance(const std::int32_t rgb) const {
	return vision_set_white_balance(_port, rgb);
}

int32_t Vision::set_zero_point(vision_zero_e_t zero_point) const {
	return vision_set_zero_point(_port, zero_point);
}
}  // namespace pros
