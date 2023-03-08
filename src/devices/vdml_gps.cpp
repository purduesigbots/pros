/**
 * \file devices/vdml_gps.cpp
 *
 * Contains functions for interacting with the VEX GPS.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "pros/gps.hpp"

namespace pros {
inline namespace v5 {

std::int32_t Gps::initialize_full(double xInitial, double yInitial, double headingInitial, double xOffset,
                                  double yOffset) const {
	return pros::c::gps_initialize_full(_port, xInitial, yInitial, headingInitial, xOffset, yOffset);
}

std::int32_t Gps::set_offset(double xOffset, double yOffset) const {
	return pros::c::gps_set_offset(_port, xOffset, yOffset);
}

pros::gps_position_s_t Gps::get_offset() const {
	return pros::c::gps_get_offset(_port);
}

std::int32_t Gps::set_position(double xInitial, double yInitial, double headingInitial) const {
	return pros::c::gps_set_position(_port, xInitial, yInitial, headingInitial);
}

std::int32_t Gps::set_data_rate(std::uint32_t rate) const {
	return pros::c::gps_set_data_rate(_port, rate);
}

double Gps::get_error() const {
	return pros::c::gps_get_error(_port);
}

pros::gps_status_s_t Gps::get_status() const {
	return pros::c::gps_get_status(_port);
}

pros::gps_position_s_t Gps::get_position() const {
	return pros::c::gps_get_position(_port);
}

double Gps::get_heading() const {
	return pros::c::gps_get_heading(_port);
}

double Gps::get_heading_raw() const {
	return pros::c::gps_get_heading_raw(_port);
}

double Gps::get_rotation() const {
	return pros::c::gps_get_rotation(_port);
}

std::int32_t Gps::set_rotation(double target) const {
	return pros::c::gps_set_rotation(_port, target);
}

std::int32_t Gps::tare_rotation() const {
	return pros::c::gps_tare_rotation(_port);
}

pros::gps_gyro_s_t Gps::get_gyro_rate() const {
	return pros::c::gps_get_gyro_rate(_port);
}

pros::gps_accel_s_t Gps::get_accel() const {
	return pros::c::gps_get_accel(_port);
}

std::ostream& operator<<(std::ostream& os, const pros::Gps& gps) {
	pros::gps_status_s_t data = gps.get_status();
	os << "Gps [";
	os << "port: " << gps._port;
	os << ", x: " << data.x;
	os << ", y: " << data.y;
	os << ", heading: " << gps.get_heading();
	os << ", rotation: " << gps.get_rotation();
	os << "]";
	return os;
}

pros::DeviceType Gps::get_type() const {
	return pros::DeviceType::gps;
}

namespace literals {
const pros::Gps operator""_gps(const unsigned long long int g) {
	return pros::Gps(g);
}
}  // namespace literals
} // namespace v5
}  // namespace pros
