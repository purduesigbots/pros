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

std::int32_t Gps::initialize_full(double xInitial, double yInitial, double headingInitial, double xOffset,
                                  double yOffset) const {
	return pros::c::gps_initialize_full(_port, xInitial, yInitial, headingInitial, xOffset, yOffset);
}

std::int32_t Gps::set_offset(double xOffset, double yOffset) const {
	return pros::c::gps_set_offset(_port, xOffset, yOffset);
}

std::int32_t Gps::get_offset(double* xOffset, double* yOffset) const {
	return pros::c::gps_get_offset(_port, xOffset, yOffset);
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

pros::c::gps_status_s_t Gps::get_status() const {
	return pros::c::gps_get_status(_port);
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

pros::c::gps_gyro_s_t Gps::get_gyro_rate() const {
	return pros::c::gps_get_gyro_rate(_port);
}

pros::c::gps_accel_s_t Gps::get_accel() const {
	return pros::c::gps_get_accel(_port);
}

}  // namespace pros
