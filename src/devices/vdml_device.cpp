/**
 * \file pros/device.cpp
 *
 * Contains functions related to PROS devices.
 *
 * \copyright (c) 2017-2021, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "pros/device.hpp"
#include "pros/apix.h"
#include "vdml/vdml.h"

namespace pros{
inline namespace v5 {

using namespace pros::c;

bool Device::is_installed() {
    std::uint8_t port = this->_port - 1;
    port_mutex_take(port);
    pros::c::v5_device_e_t deviceType = c::registry_get_plugged_type(port);
    port_mutex_give(port);
    return deviceType != c::E_DEVICE_NONE && deviceType != c::E_DEVICE_UNDEFINED;
}

}
}

