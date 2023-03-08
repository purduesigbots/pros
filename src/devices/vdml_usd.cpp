/**
 * \file devices/usd.cpp
 *
 * Contains functions for interacting with the SD card.
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
namespace usd {
using namespace pros::c;

std::int32_t is_installed(void) {
	return usd_is_installed();
}

}  // namespace usd
}  // namespace pros
