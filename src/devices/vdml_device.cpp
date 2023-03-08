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
#include "vdml/vdml.h"

namespace pros{
inline namespace v5 {

bool Device::is_installed() {
    std::uint8_t port = _port - 1;
    port_mutex_take(port);
    DeviceType deviceType = (DeviceType) c::registry_get_plugged_type(port);
    port_mutex_give(port);
    return _deviceType == deviceType;
}

}
}

