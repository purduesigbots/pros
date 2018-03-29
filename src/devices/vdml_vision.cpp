/**
 * pros/vision.cpp
 *
 * See https://pros.cs.purdue.edu/v5/tutorials/multitasking to learn more.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 * All rights reservered.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "kapi.h"

namespace pros {
Vision::Vision(std::uint8_t port) : _port(port) {}

std::int32_t Vision::clear_led(void) const {
	return vision_clear_led(_port);
}

std::int32_t Vision::get_exposure(void) const {
	return vision_get_exposure(_port);
}

vision_object_s_t Vision::get_object(const std::uint32_t object_id) const {
	return vision_get_object(_port, object_id);
}

std::int32_t Vision::get_object_count(void) const {
	return vision_get_object_count(_port);
}

std::int32_t Vision::get_white_balance(void) const {
	return vision_get_white_balance(_port);
}

std::int32_t Vision::read_object(const std::uint32_t object_id, vision_object_s_t* const object_ptr) const {
	return vision_read_object(_port, object_id, object_ptr);
}

std::int32_t Vision::read_objects(const std::uint32_t object_count, vision_object_s_t* const object_arr) const {
	return vision_read_objects(_port, object_count, object_arr);
}

std::int32_t Vision::read_signature(const std::uint8_t signature_id, vision_signature_s_t* const signature_ptr) const {
	return vision_read_signature(_port, signature_id, signature_ptr);
}

std::int32_t Vision::save_signature(const std::uint8_t signature_id, vision_signature_s_t* const signature_ptr) const {
	return vision_save_signature(_port, signature_id, signature_ptr);
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
}
