/**
 * \file devices/vdml_gps.cpp
 *
 * Contains functions for interacting with the VEX GPS.
 *
 * \copyright Copyright (c) 2017-2024, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "pros/device.h"
#include "pros/gps.hpp"
#include "vdml/vdml.h"

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

std::vector<Gps> Gps::get_all_devices() {
	std::vector<Device> matching_devices {Device::get_all_devices(DeviceType::gps)};
	std::vector<Gps> return_vector;
	for (auto device : matching_devices) {
		return_vector.push_back(device);
	}
	return return_vector;
}


double Gps::get_error() const {
	return pros::c::gps_get_error(_port);
}

pros::gps_status_s_t Gps::get_position_and_orientation() const {
	return pros::c::gps_get_position_and_orientation(_port);
}

pros::gps_position_s_t Gps::get_position() const {
	return pros::c::gps_get_position(_port);
}

double Gps::get_position_x() const {
    return pros::c::gps_get_position_x(_port);
}

double Gps::get_position_y() const {
    return pros::c::gps_get_position_y(_port);
}

pros::gps_orientation_s_t Gps::get_orientation() const {
    return pros::c::gps_get_orientation(_port);
}

double Gps::get_pitch() const {
    return pros::c::gps_get_pitch(_port);
}

double Gps::get_roll() const {
    return pros::c::gps_get_roll(_port);
}

double Gps::get_yaw() const {
    return pros::c::gps_get_yaw(_port);
}

double Gps::get_heading() const {
	return pros::c::gps_get_heading(_port);
}

double Gps::get_heading_raw() const {
	return pros::c::gps_get_heading_raw(_port);
}

pros::gps_gyro_s_t Gps::get_gyro_rate() const {
	return pros::c::gps_get_gyro_rate(_port);
}

double Gps::get_gyro_rate_x() const {
    return pros::c::gps_get_gyro_rate_x(_port);
}

double Gps::get_gyro_rate_y() const {
	return pros::c::gps_get_gyro_rate_y(_port);
}

double Gps::get_gyro_rate_z() const {
	return pros::c::gps_get_gyro_rate_z(_port);
}

pros::gps_accel_s_t Gps::get_accel() const {
	return pros::c::gps_get_accel(_port);
}

double Gps::get_accel_x() const {
    return pros::c::gps_get_accel_x(_port);
}

double Gps::get_accel_y() const {
	return pros::c::gps_get_accel_y(_port);
}

double Gps::get_accel_z() const {
	return pros::c::gps_get_accel_z(_port);
}

std::ostream& operator<<(std::ostream& os, const pros::Gps& gps) {
	pros::gps_status_s_t data = gps.get_position_and_orientation();
	os << "Gps [";
	os << "port: " << gps._port;
	os << ", x: " << data.x;
	os << ", y: " << data.y;
	os << ", heading: " << gps.get_heading();
	os << "]";
	return os;
}

pros::Gps pros::Gps::get_gps() {
	static int curr_gps_port = 0;
	curr_gps_port = curr_gps_port % 21;
	for (int i = 0; i < 21; i++) {
		if (registry_get_device(curr_gps_port)->device_type == pros::c::E_DEVICE_GPS) {
			curr_gps_port++;
			return Gps(curr_gps_port);
		}
		curr_gps_port++;
		curr_gps_port = curr_gps_port % 21;
	}
	errno = ENODEV;
	return Gps(PROS_ERR_BYTE);
}

namespace literals {
const pros::Gps operator""_gps(const unsigned long long int g) {
	return pros::Gps(g);
}
}  // namespace literals
}  // namespace v5
}  // namespace pros
