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
#include "pros/adi.hpp"

namespace pros {
using namespace pros::c;

ADIPort::ADIPort(std::uint8_t adi_port, adi_port_config_e_t type)
    : _adi_port(adi_port), _smart_port(INTERNAL_ADI_PORT) {
	ext_adi_port_set_config(_adi_port, _smart_port, type);
}

ADIPort::ADIPort(ext_adi_port_pair_t port, adi_port_config_e_t type) : _adi_port(port.first), _smart_port(port.second) {
	ext_adi_port_set_config(_adi_port, _smart_port, type);
}

ADIPort::ADIPort() {
	// for use by derived classes like ADIEncoder
}

std::int32_t ADIPort::set_config(adi_port_config_e_t type) const {
	return ext_adi_port_set_config(_adi_port, _smart_port, type);
}

std::int32_t ADIPort::get_config() const {
	return ext_adi_port_get_config(_adi_port, _smart_port);
}

std::int32_t ADIPort::set_value(std::int32_t value) const {
	return ext_adi_port_set_value(_adi_port, _smart_port, value);
}

std::int32_t ADIPort::get_value() const {
	return ext_adi_port_get_value(_adi_port, _smart_port);
}

ADIAnalogIn::ADIAnalogIn(std::uint8_t adi_port) : ADIPort(adi_port, E_ADI_ANALOG_IN) {}
ADIAnalogIn::ADIAnalogIn(ext_adi_port_pair_t port) : ADIPort(port, E_ADI_ANALOG_IN) {}

std::int32_t ADIAnalogIn::calibrate() const {
	return ext_adi_analog_calibrate(_adi_port, _smart_port);
}

std::int32_t ADIAnalogIn::get_value_calibrated() const {
	return ext_adi_analog_read_calibrated(_adi_port, _smart_port);
}

std::int32_t ADIAnalogIn::get_value_calibrated_HR() const {
	return ext_adi_analog_read_calibrated_HR(_adi_port, _smart_port);
}

ADIAnalogOut::ADIAnalogOut(std::uint8_t adi_port) : ADIPort(adi_port, E_ADI_ANALOG_OUT) {}
ADIAnalogOut::ADIAnalogOut(ext_adi_port_pair_t port) : ADIPort(port, E_ADI_ANALOG_OUT) {}

ADIDigitalIn::ADIDigitalIn(std::uint8_t adi_port) : ADIPort(adi_port, E_ADI_DIGITAL_IN) {}
ADIDigitalIn::ADIDigitalIn(ext_adi_port_pair_t port) : ADIPort(port, E_ADI_DIGITAL_IN) {}

std::int32_t ADIDigitalIn::get_new_press() const {
	return ext_adi_digital_get_new_press(_adi_port, _smart_port);
}

ADIDigitalOut::ADIDigitalOut(std::uint8_t adi_port, bool init_state) : ADIPort(adi_port, E_ADI_DIGITAL_OUT) {
	set_value(init_state);
}

ADIDigitalOut::ADIDigitalOut(ext_adi_port_pair_t port, bool init_state) : ADIPort(port, E_ADI_DIGITAL_OUT) {
	set_value(init_state);
}

ADIMotor::ADIMotor(std::uint8_t adi_port) : ADIPort(adi_port, E_ADI_LEGACY_PWM) {
	stop();
}

ADIMotor::ADIMotor(ext_adi_port_pair_t port) : ADIPort(port, E_ADI_LEGACY_PWM) {
	stop();
}

std::int32_t ADIMotor::stop() const {
	return ext_adi_motor_stop(_adi_port, _smart_port);
}

ADIEncoder::ADIEncoder(std::uint8_t adi_port_top, std::uint8_t adi_port_bottom, bool reversed) {
	std::int32_t _port = ext_adi_encoder_init(adi_port_top, adi_port_bottom, INTERNAL_ADI_PORT, reversed);
	get_ports(_port, _smart_port, _adi_port);
}

ADIEncoder::ADIEncoder(ext_adi_port_tuple_t port, bool reversed) {
	std::int32_t _port = ext_adi_encoder_init(std::get<0>(port), std::get<1>(port), std::get<2>(port), reversed);
	get_ports(_port, _smart_port, _adi_port);
}

std::int32_t ADIEncoder::reset() const {
	return adi_encoder_reset(merge_adi_ports(_adi_port, _smart_port));
}

std::int32_t ADIEncoder::get_value() const {
	return adi_encoder_get(merge_adi_ports(_adi_port, _smart_port));
}

ADIUltrasonic::ADIUltrasonic(std::uint8_t adi_port_ping, std::uint8_t adi_port_echo) {
	std::int32_t _port = ext_adi_ultrasonic_init(adi_port_ping, adi_port_echo, INTERNAL_ADI_PORT);
	get_ports(_port, _smart_port, _adi_port);
}

ADIUltrasonic::ADIUltrasonic(ext_adi_port_tuple_t port) {
	std::int32_t _port = ext_adi_ultrasonic_init(std::get<0>(port), std::get<1>(port), std::get<2>(port));
	get_ports(_port, _smart_port, _adi_port);
}

std::int32_t ADIUltrasonic::get_value() const {
	return adi_ultrasonic_get(merge_adi_ports(_adi_port, _smart_port));
}

ADIGyro::ADIGyro(std::uint8_t adi_port, double multiplier) {
	std::int32_t _port = ext_adi_gyro_init(adi_port, INTERNAL_ADI_PORT, multiplier);
	get_ports(_port, _smart_port, _adi_port);
}

ADIGyro::ADIGyro(ext_adi_port_pair_t port, double multiplier) {
	std::int32_t _port = ext_adi_gyro_init(port.first, port.second, multiplier);
	get_ports(_port, _smart_port, _adi_port);
}

double ADIGyro::get_value() const {
	return adi_gyro_get(merge_adi_ports(_adi_port, _smart_port));
}

std::int32_t ADIGyro::reset() const {
	return adi_gyro_reset(merge_adi_ports(_adi_port, _smart_port));
}
}  // namespace pros
