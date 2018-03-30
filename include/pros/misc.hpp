/**
 * \file pros/misc.hpp
 *
 * \brief Contains miscellaneous declarations such as controller and competition
 * functions in C++
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/misc to learn more.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 * All rights reservered.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _PROS_MISC_HPP_
#define _PROS_MISC_HPP_

#include "pros/misc.h"

namespace pros {
class Controller {
	public:
	Controller(controller_id_e_t id);

	std::int32_t is_connected(void);
	std::int32_t get_analog(controller_analog_e_t channel);
	std::int32_t get_digital(controller_digital_e_t button);
	std::int32_t get_digital_new_press(controller_digital_e_t button);

	private:
	controller_id_e_t _id;
};

namespace battery {
	double get_capacity(void);
	double get_current(void);
	double get_temperature(void);
	double get_voltage(void);
}

namespace competition {
	std::uint8_t get_status(void);
	std::uint8_t is_autonomous(void);
	std::uint8_t is_connected(void);
	std::uint8_t is_disabled(void);
}
}

#endif
