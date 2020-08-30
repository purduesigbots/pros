/**
 * \file devices/vdml_adi.cpp
 *
 * Contains functions for interacting with the V5 ADI.
 *
 * Copyright (c) 2017-2020, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "kapi.h"
#include "pros/ext_adi.hpp"

namespace pros {
using namespace pros::c;

EXT_ADIPort::EXT_ADIPort(std::uint8_t smart_port, std::uint8_t adi_port, adi_port_config_e_t type) : _smart_port(smart_port), _adi_port(adi_port) {
 	ext_adi_port_set_config(_smart_port, _adi_port, type);
}

EXT_ADIPort::EXT_ADIPort(void) {
	// for use by derived classes like ADIEncoder
}

std::int32_t EXT_ADIPort::set_config(adi_port_config_e_t type) const {
	return ext_adi_port_set_config(_smart_port, _adi_port, type);
}

std::int32_t EXT_ADIPort::get_config(void) const {
	return ext_adi_port_get_config(_smart_port, _adi_port);
}

std::int32_t EXT_ADIPort::set_value(std::int32_t value) const {
	return ext_adi_port_set_value(_smart_port, _adi_port, value);
}

std::int32_t EXT_ADIPort::get_value(void) const {
	return ext_adi_port_get_value(_smart_port, _adi_port);
}

EXT_ADIAnalogIn::EXT_ADIAnalogIn(std::uint8_t smart_port, std::uint8_t adi_port) : EXT_ADIPort(smart_port, adi_port, E_ADI_ANALOG_IN) {}

 
EXT_ADIAnalogOut::EXT_ADIAnalogOut(std::uint8_t smart_port, std::uint8_t adi_port) : EXT_ADIPort(smart_port, adi_port, E_ADI_ANALOG_OUT) {}

std::int32_t EXT_ADIAnalogIn::calibrate(void) const {
	return ext_adi_analog_calibrate(_smart_port, _adi_port);
}

std::int32_t EXT_ADIAnalogIn::get_value_calibrated(void) const {
	return ext_adi_analog_read_calibrated(_smart_port, _adi_port);
}

std::int32_t EXT_ADIAnalogIn::get_value_calibrated_HR(void) const {
	return ext_adi_analog_read_calibrated_HR(_smart_port, _adi_port);
}

EXT_ADIDigitalOut::EXT_ADIDigitalOut(std::uint8_t smart_port, std::uint8_t adi_port, bool init_state)
    : EXT_ADIPort(smart_port, adi_port, E_ADI_DIGITAL_OUT) {
	set_value(init_state);
}

EXT_ADIDigitalIn::EXT_ADIDigitalIn(std::uint8_t smart_port, std::uint8_t adi_port)
    : EXT_ADIPort(smart_port, adi_port, E_ADI_DIGITAL_IN) {}

std::int32_t EXT_ADIDigitalIn::get_new_press(void) const {
	return ext_adi_digital_get_new_press(_smart_port, _adi_port);
}

EXT_ADIMotor::EXT_ADIMotor(std::uint8_t smart_port, std::uint8_t adi_port) : EXT_ADIPort(smart_port, adi_port, E_ADI_LEGACY_PWM) {
	stop();
}

std::int32_t EXT_ADIMotor::stop(void) const {
	return ext_adi_motor_stop(_smart_port, _adi_port);
}

EXT_ADIEncoder::EXT_ADIEncoder(std::uint8_t smart_port, std::uint8_t adi_port_top, std::uint8_t adi_port_bottom,
                       bool reversed) {
	_port = ext_adi_encoder_init(smart_port, adi_port_top, adi_port_bottom, reversed);
	get_ports(_port,_smart_port,_adi_port);
}

std::int32_t EXT_ADIEncoder::reset(void) const {
	return ext_adi_encoder_reset(_port);
}

std::int32_t EXT_ADIEncoder::get_value(void) const {
	return ext_adi_encoder_get(_port);
}

EXT_ADIUltrasonic::EXT_ADIUltrasonic(std::uint8_t smart_port, std::uint8_t adi_port_ping, std::uint8_t adi_port_echo) {
	_port = ext_adi_ultrasonic_init(smart_port, adi_port_ping, adi_port_echo);
	get_ports(_port,_smart_port,_adi_port);
}

EXT_ADIGyro::EXT_ADIGyro(std::uint8_t smart_port, std::uint8_t adi_port, double multiplier) {
	_port = ext_adi_gyro_init(smart_port, adi_port, multiplier);
	get_ports(_port,_smart_port,_adi_port);
}

double EXT_ADIGyro::get_value(void) const {
	return ext_adi_gyro_get(_port);
}

std::int32_t EXT_ADIGyro::reset(void) const {
	return ext_adi_gyro_reset(_port);
}
}  // namespace pros
