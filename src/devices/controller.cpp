/**
 * \file devices/controller.cpp
 *
 * Contains functions for interacting with the V5 Controller, as well as the
 * competition control functions.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "kapi.h"

namespace pros {
using namespace pros::c;

Controller::Controller(pros::controller_id_e_t id) : _id(id) {}

std::int32_t Controller::is_connected(void) {
	return controller_is_connected(_id);
}

std::int32_t Controller::get_analog(pros::controller_analog_e_t channel) {
	return controller_get_analog(_id, channel);
}

std::int32_t Controller::get_battery_capacity(void) {
	return controller_get_battery_capacity(_id);
}

std::int32_t Controller::get_battery_level(void) {
	return controller_get_battery_level(_id);
}

std::int32_t Controller::get_digital(pros::controller_digital_e_t button) {
	return controller_get_digital(_id, button);
}

std::int32_t Controller::get_digital_new_press(pros::controller_digital_e_t button) {
	return controller_get_digital_new_press(_id, button);
}

std::int32_t Controller::set_text(std::uint8_t line, std::uint8_t col, const char* str) {
	return controller_set_text(_id, line, col, str);
}

std::int32_t Controller::set_text(std::uint8_t line, std::uint8_t col, const std::string& str) {
	return controller_set_text(_id, line, col, str.c_str());
}

std::int32_t Controller::clear_line(std::uint8_t line) {
	return controller_clear_line(_id, line);
}

std::int32_t Controller::clear(void) {
	return controller_clear(_id);
}

std::int32_t Controller::rumble(const char* rumble_pattern) {
	return controller_rumble(_id, rumble_pattern);
}

namespace competition {
std::uint8_t get_status(void) {
	return competition_get_status();
}

std::uint8_t is_autonomous(void) {
	return competition_is_autonomous();
}

std::uint8_t is_connected(void) {
	return competition_is_connected();
}

std::uint8_t is_disabled(void) {
	return competition_is_disabled();
}
}  // namespace competition
}  // namespace pros
