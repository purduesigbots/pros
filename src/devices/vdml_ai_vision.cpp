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

#include "pros/ai_vision.hpp"
#include "pros/device.h"
#include "vdml/vdml.h"

namespace pros {
inline namespace v5 {
using namespace pros::c;

AivisionModeType operator|(AivisionModeType lhs, AivisionModeType rhs) {
	return static_cast<AivisionModeType>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

AIVision::AIVision(const uint8_t port) : Device(port, DeviceType::aivision) {
	// empty constructor
}

std::vector<AIVision> AIVision::get_all_devices() {
	std::vector<Device> matching_devices{Device::get_all_devices(DeviceType::aivision)};

	std::vector<AIVision> return_vector;
	for (const auto& device : matching_devices) {
		return_vector.push_back(device);
	}
	return return_vector;
}

bool AIVision::is_type(AivisionDetectType type, const aivision_object_s_t& object) {
	return static_cast<uint8_t>(type) == object.type;
}

int32_t AIVision::reset() {
	return c::aivision_reset(this->_port);
}

int32_t AIVision::get_enabled_detection_types() {
	return c::aivision_get_enabled_detection_types(this->_port);
}

int32_t AIVision::enable_detection_types(AivisionModeType types_mask) {
	return c::aivision_enable_detection_types(this->_port, static_cast<uint8_t>(types_mask));
}

int32_t AIVision::disable_detection_types(AivisionModeType types_mask) {
	return c::aivision_disable_detection_types(this->_port, static_cast<uint8_t>(types_mask));
}

int32_t AIVision::set_tag_family(AivisionTagFamily family) {
	return c::aivision_set_tag_family(this->_port, static_cast<aivision_tag_family_e_t>(family));
}

int32_t AIVision::set_color(pros::aivision_color_s_t& color) {
	return c::aivision_set_color(this->_port, &color);
}

aivision_color_s_t AIVision::get_color(uint32_t id) {
	return c::aivision_get_color(this->_port, id);
}

int32_t AIVision::get_object_count() {
	return c::aivision_get_object_count(this->_port);
}

aivision_object_s_t AIVision::get_object(uint32_t object_index) {
	return c::aivision_get_object(this->_port, object_index);
}

std::vector<aivision_object_s_t> AIVision::get_all_objects() {
	int32_t count = this->get_object_count();
	if (count < 0 || count == PROS_ERR) {
		return {};
	}
	std::vector<aivision_object_s_t> objects = std::vector<aivision_object_s_t>(count);
	for (size_t idx = 0; idx < count; idx++) {
		objects.emplace_back(this->get_object(idx));
	}
	return objects;
}

uint32_t AIVision::get_class_name(int32_t id, uint8_t* class_name) {
	return c::aivision_get_class_name(this->_port, id, class_name);
}

aivision_code_s_t AIVision::get_code(uint32_t id) {
	return c::aivision_get_code(this->_port, id);
}

uint32_t AIVision::set_code(pros::aivision_code_s_t& code) {
	return c::aivision_set_code(this->_port, &code);
}

}  // namespace v5
}  // namespace pros