/**
 * pros/vision.hpp - PROS API header provides high-level user functionality
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
#ifndef _PROS_VISION_HPP_
#define _PROS_VISION_HPP_

#include "pros/vision.h"

namespace pros {
class Vision {
	public:
	Vision(uint8_t port);

	int32_t clear_led(void) const;
	int32_t get_exposure(void) const;
	vision_object_s_t get_object(const uint32_t object_id) const;
	int32_t get_object_count(void) const;
	int32_t get_white_balance(void) const;
	int32_t read_object(const uint32_t object_id, vision_object_s_t* const object_ptr) const;
	int32_t read_objects(const uint32_t object_count, vision_object_s_t* const object_arr) const;
	int32_t read_signature(const uint8_t signature_id, vision_signature_s_t* const signature_ptr) const;
	int32_t save_signature(const uint8_t signature_id, vision_signature_s_t* const signature_ptr) const;
	int32_t set_auto_white_balance(const uint8_t enable) const;
	int32_t set_exposure(const uint8_t percent) const;
	int32_t set_led(const int32_t rgb) const;
	int32_t set_white_balance(const int32_t rgb) const;

	private:
	uint8_t _port;
};
}
#endif
