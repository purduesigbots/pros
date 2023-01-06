/**
 * \file devices/battery.cpp
 *
 * Contains functions for interacting with the V5 Battery.
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
namespace battery {
using namespace pros::c;

double get_capacity(void) {
	return battery_get_capacity();
}

std::int32_t get_current(void) {
	return battery_get_current();
}

double get_temperature(void) {
	return battery_get_temperature();
}

std::int32_t get_voltage(void) {
	return battery_get_voltage();
}
}  // namespace battery
}  // namespace pros
