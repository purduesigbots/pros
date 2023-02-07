/**
 * \file devices/vdml_serial.cpp
 *
 * Contains functions for interacting with V5 Generic Serial devices.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "kapi.h"
#include "pros/serial.hpp"

namespace pros {
using namespace pros::c;

Serial::Serial(std::uint8_t port, std::int32_t baudrate) : _port(port) {
	serial_enable(port);
	set_baudrate(baudrate);
}

Serial::Serial(std::uint8_t port) : _port(port) {
	serial_enable(port);
}

std::int32_t Serial::set_baudrate(std::int32_t baudrate) const {
	return serial_set_baudrate(_port, baudrate);
}

std::int32_t Serial::flush() const {
	return serial_flush(_port);
}

std::int32_t Serial::get_read_avail() const {
	return serial_get_read_avail(_port);
}

std::int32_t Serial::get_write_free() const {
	return serial_get_write_free(_port);
}

std::uint8_t Serial::get_port() const {
	return _port;
}

std::int32_t Serial::peek_byte() const {
	return serial_peek_byte(_port);
}

std::int32_t Serial::read_byte() const {
	return serial_read_byte(_port);
}

std::int32_t Serial::read(std::uint8_t* buffer, std::int32_t length) const {
	return serial_read(_port, buffer, length);
}

std::int32_t Serial::write_byte(std::uint8_t buffer) const {
	return serial_write_byte(_port, buffer);
}

std::int32_t Serial::write(std::uint8_t* buffer, std::int32_t length) const {
	return serial_write(_port, buffer, length);
}

namespace literals {
const pros::Serial operator"" _ser(const unsigned long long int m) {
	return pros::Serial(m);
}
}  // namespace literals
}  // namespace pros
