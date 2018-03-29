/**
 * \file pros/vision.h
 *
 * \brief Contains prototypes for the VEX Vision Sensor-related functions in C++.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/vision to learn more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
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
	Vision(std::uint8_t port);

	std::int32_t clear_led(void) const;
	std::int32_t get_exposure(void) const;
	vision_object_s_t get_object(const std::uint32_t object_id) const;
	std::int32_t get_object_count(void) const;
	std::int32_t get_white_balance(void) const;
	std::int32_t read_object(const std::uint32_t object_id, vision_object_s_t* const object_ptr) const;
	std::int32_t read_objects(const std::uint32_t object_count, vision_object_s_t* const object_arr) const;
	std::int32_t read_signature(const std::uint8_t signature_id, vision_signature_s_t* const signature_ptr) const;
	std::int32_t save_signature(const std::uint8_t signature_id, vision_signature_s_t* const signature_ptr) const;
	std::int32_t set_auto_white_balance(const std::uint8_t enable) const;
	std::int32_t set_exposure(const std::uint8_t percent) const;
	std::int32_t set_led(const std::int32_t rgb) const;
	std::int32_t set_white_balance(const std::int32_t rgb) const;

	private:
	std::uint8_t _port;
};
}
#endif
