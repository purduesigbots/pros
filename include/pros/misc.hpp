/**
 * pros/misc.hpp - PROS API header provides high-level user functionality
 *
 * Contains miscellaneous declarations for use by typical VEX programmers using PROS.
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

	int32_t is_connected(void);
	int32_t get_analog(controller_analog_e_t channel);
	int32_t get_digital(controller_digital_e_t button);
	int32_t get_digital_new_press(controller_digital_e_t button);

	private:
	controller_id_e_t _id;
};

namespace battery {
	double get_capacity(void);
	double get_current(void);
	double get_temperature(void);
	double get_voltage(void);
}
}

#endif
