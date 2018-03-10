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
Vision::Vision(uint8_t port) : _port(port) {}

int32_t Vision::clear_led(void) {
	return vision_clear_led(_port);
}

int32_t Vision::get_exposure(void) {
	return vision_get_exposure(_port);
}

vision_object_s_t Vision::get_object(const uint32_t object_id) {
	return vision_get_object(_port, object_id);
}

int32_t Vision::get_object_count(void) {
	return vision_get_object_count(_port);
}

int32_t Vision::get_white_balance(void) {
	return vision_get_white_balance(_port);
}

int32_t Vision::read_object(const uint32_t object_id, vision_object_s_t* const object_ptr) {
	return vision_read_object(_port, object_id, object_ptr);
}

int32_t Vision::read_objects(const uint32_t object_count, vision_object_s_t* const object_arr) {
	return vision_read_objects(_port, object_count, object_arr);
}

int32_t Vision::read_signature(const uint8_t signature_id, vision_signature_s_t* const signature_ptr) {
	return vision_read_signature(_port, signature_id, signature_ptr);
}

int32_t Vision::save_signature(const uint8_t signature_id, vision_signature_s_t* const signature_ptr) {
	return vision_save_signature(_port, signature_id, signature_ptr);
}

int32_t Vision::set_auto_white_balance(const uint8_t enable) {
	return vision_set_auto_white_balance(_port, enable);
}

int32_t Vision::set_exposure(const uint8_t percent) {
	return vision_set_exposure(_port, percent);
}

int32_t Vision::set_led(const int32_t rgb) {
	return vision_set_led(_port, rgb);
}

int32_t Vision::set_white_balance(const int32_t rgb) {
	return vision_set_white_balance(_port, rgb);
}
}
