/**
 * \file devices/vdml_vision.cpp
 *
 * Contains functions for interacting with the V5 Vision Sensor.
 *
 * \copyright Copyright (c) 2017-2024, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "kapi.h"
#include "vdml/vdml.h"

namespace pros {
inline namespace v5 {
using namespace pros::c;

Vision::Vision(std::uint8_t port, vision_zero_e_t zero_point) : Device(port, DeviceType::vision) {
	vision_set_zero_point(port, zero_point);
}

std::int32_t Vision::clear_led(void) const {
	return vision_clear_led(_port);
}

vision_signature_s_t Vision::signature_from_utility(const std::int32_t id, const std::int32_t u_min,
                                                    const std::int32_t u_max, const std::int32_t u_mean,
                                                    const std::int32_t v_min, const std::int32_t v_max,
                                                    const std::int32_t v_mean, const float range,
                                                    const std::int32_t type) {
	return c::vision_signature_from_utility(id, u_min, u_max, u_mean, v_min, v_max, v_mean, range, type);
}

vision_color_code_t Vision::create_color_code(const std::uint32_t sig_id1, const std::uint32_t sig_id2,
                                              const std::uint32_t sig_id3, const std::uint32_t sig_id4,
                                              const std::uint32_t sig_id5) const {
	return vision_create_color_code(_port, sig_id1, sig_id2, sig_id3, sig_id4, sig_id5);
}

std::vector<Vision> Vision::get_all_devices() {

	std::vector<Device> matching_devices{Device::get_all_devices(DeviceType::vision)};

	std::vector<Vision> return_vector;
	for (auto device : matching_devices) {
		return_vector.push_back(device);
	}
	return return_vector;
}

vision_object_s_t Vision::get_by_size(const std::uint32_t size_id) const {
	return vision_get_by_size(_port, size_id);
}

vision_object_s_t Vision::get_by_sig(const std::uint32_t size_id, const std::uint32_t sig_id) const {
	return vision_get_by_sig(_port, size_id, sig_id);
}

vision_object_s_t Vision::get_by_code(const std::uint32_t size_id, const vision_color_code_t color_code) const {
	return vision_get_by_code(_port, size_id, color_code);
}

int32_t Vision::get_exposure(void) const {
	return vision_get_exposure(_port);
}

int32_t Vision::get_object_count(void) const {
	return vision_get_object_count(_port);
}

std::int32_t Vision::get_white_balance(void) const {
	return vision_get_white_balance(_port);
}

int32_t Vision::read_by_size(const std::uint32_t size_id, const std::uint32_t object_count,
                             vision_object_s_t* const object_arr) const {
	return vision_read_by_size(_port, size_id, object_count, object_arr);
}

int32_t Vision::read_by_sig(const std::uint32_t size_id, const std::uint32_t sig_id, const std::uint32_t object_count,
                            vision_object_s_t* const object_arr) const {
	return vision_read_by_sig(_port, size_id, sig_id, object_count, object_arr);
}

int32_t Vision::read_by_code(const std::uint32_t size_id, const vision_color_code_t color_code,
                             const std::uint32_t object_count, vision_object_s_t* const object_arr) const {
	return vision_read_by_code(_port, size_id, color_code, object_count, object_arr);
}

vision_signature_s_t Vision::get_signature(const std::uint8_t signature_id) const {
	return vision_get_signature(_port, signature_id);
}

std::int32_t Vision::print_signature(const vision_signature_s_t sig) {
	return vision_print_signature(sig);
}

std::int32_t Vision::set_signature(const std::uint8_t signature_id, vision_signature_s_t* const signature_ptr) const {
	return vision_set_signature(_port, signature_id, signature_ptr);
}

std::int32_t Vision::set_auto_white_balance(const std::uint8_t enable) const {
	return vision_set_auto_white_balance(_port, enable);
}

std::int32_t Vision::set_exposure(const std::uint8_t exposure) const {
	return vision_set_exposure(_port, exposure);
}

std::int32_t Vision::set_led(const std::int32_t rgb) const {
	return vision_set_led(_port, rgb);
}

std::int32_t Vision::set_white_balance(const std::int32_t rgb) const {
	return vision_set_white_balance(_port, rgb);
}

std::int32_t Vision::set_zero_point(vision_zero_e_t zero_point) const {
	return vision_set_zero_point(_port, zero_point);
}

std::int32_t Vision::set_wifi_mode(const std::uint8_t enable) const {
	return vision_set_wifi_mode(_port, enable);
}
Vision Vision::get_vision() {
	static int curr_vision_port = 0;
	curr_vision_port = curr_vision_port % 21;
	for (int i = 0; i < 21; i++) {
		if (registry_get_device(curr_vision_port)->device_type == pros::c::E_DEVICE_VISION) {
			curr_vision_port++;
			return Vision(curr_vision_port);
		}
		curr_vision_port++;
		curr_vision_port = curr_vision_port % 21;
	}
	errno = ENODEV;
	return Vision(PROS_ERR_BYTE);
}

}  // namespace v5
namespace literals {
const pros::Vision operator"" _vis(const unsigned long long int m) {
	return Vision(m);
}

}  // namespace literals
}  // namespace pros
