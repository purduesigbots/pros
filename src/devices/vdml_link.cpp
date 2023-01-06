/**
 * \file vdml_link.cpp
 *
 * \brief Contains source code for functions related to the robot to robot communications.
 *
 * Visit https://pros.cs.purdue.edu/v5/api/c/link.html to learn
 * more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 * 
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "pros/link.hpp"

namespace pros {
    Link::Link(const std::uint8_t port, const std::string link_id, link_type_e_t type, bool ov) : _port(port) {
        (ov) ? pros::c::link_init_override(_port, link_id.c_str(), type) : pros::c::link_init(_port, link_id.c_str(), type);
    }

    bool Link::connected() {
        return pros::c::link_connected(_port);
    }

    std::uint32_t Link::raw_receivable_size() {
        return pros::c::link_raw_receivable_size(_port);
    }

    std::uint32_t Link::raw_transmittable_size() {
        return pros::c::link_raw_transmittable_size(_port);
    }

    std::uint32_t Link::transmit_raw(void* data, std::uint16_t data_size) {
        return pros::c::link_transmit_raw(_port, data, data_size);
    }

    std::uint32_t Link::receive_raw(void* dest, std::uint16_t data_size) {
        return pros::c::link_receive_raw(_port, dest, data_size);
    }

    std::uint32_t Link::transmit(void* data, std::uint16_t data_size) {
        return pros::c::link_transmit(_port, data, data_size);
    }

    std::uint32_t Link::receive(void* dest, std::uint16_t data_size) {
        return pros::c::link_receive(_port, dest, data_size);
    }

    std::uint32_t Link::clear_receive_buf() {
        return pros::c::link_clear_receive_buf(_port);
    }
}
