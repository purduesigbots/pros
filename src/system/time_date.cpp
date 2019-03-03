/**
 * \file system/time_date.cpp
 *
 * Wrappers for the VEXos date and time functionality.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "kapi.h"
#include "v5_api.h"

namespace pros {
date_s_t get_date(void) {
	return pros::c::get_date();
}

time_s_t get_time(void) {
	return pros::c::get_time();
}
}  // namespace pros
