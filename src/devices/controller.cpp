/**
 * \file controller.cpp
 *
 * Contains functions for std::interacting with the V5 Controller.
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
Controller::Controller(controller_id_e_t id) : _id(id) {}

std::int32_t Controller::is_connected(void) {
	return controller_is_connected(_id);
}

std::int32_t Controller::get_analog(controller_analog_e_t channel) {
	return controller_get_analog(_id, channel);
}

std::int32_t Controller::get_digital(controller_digital_e_t button) {
	return controller_get_digital(_id, button);
}

std::int32_t Controller::get_digital_new_press(controller_digital_e_t button) {
	return controller_get_digital_new_press(_id, button);
}
}
