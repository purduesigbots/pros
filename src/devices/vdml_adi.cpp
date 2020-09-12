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
#include "vdml/port.h"

namespace pros {
using namespace pros::c;

ADIPort::ADIPort(std::uint8_t adi_port, adi_port_config_e_t type)
    : _smart_port(INTERNAL_ADI_PORT), _adi_port(adi_port) {
	ext_adi_port_set_config(_smart_port, _adi_port, type);
}

ADIPort::ADIPort(ext_adi_port_pair_t port_pair, adi_port_config_e_t type)
    : _smart_port(port_pair.first), _adi_port(port_pair.second) {
	ext_adi_port_set_config(_smart_port, _adi_port, type);
}

std::int32_t ADIPort::set_config(adi_port_config_e_t type) const {
	return ext_adi_port_set_config(_smart_port, _adi_port, type);
}

std::int32_t ADIPort::get_config() const {
	return ext_adi_port_get_config(_smart_port, _adi_port);
}

std::int32_t ADIPort::set_value(std::int32_t value) const {
	return ext_adi_port_set_value(_smart_port, _adi_port, value);
}

std::int32_t ADIPort::get_value() const {
	return ext_adi_port_get_value(_smart_port, _adi_port);
}

ADIAnalogIn::ADIAnalogIn(std::uint8_t adi_port) : ADIPort(adi_port, E_ADI_ANALOG_IN) {}
ADIAnalogIn::ADIAnalogIn(ext_adi_port_pair_t port_pair) : ADIPort(port_pair, E_ADI_ANALOG_IN) {}

std::int32_t ADIAnalogIn::calibrate() const {
	return ext_adi_analog_calibrate(_smart_port, _adi_port);
}

std::int32_t ADIAnalogIn::get_value_calibrated() const {
	return ext_adi_analog_read_calibrated(_smart_port, _adi_port);
}

std::int32_t ADIAnalogIn::get_value_calibrated_HR() const {
	return ext_adi_analog_read_calibrated_HR(_smart_port, _adi_port);
}

ADIAnalogOut::ADIAnalogOut(std::uint8_t adi_port) : ADIPort(adi_port, E_ADI_ANALOG_OUT) {}
ADIAnalogOut::ADIAnalogOut(ext_adi_port_pair_t port_pair) : ADIPort(port_pair, E_ADI_ANALOG_OUT) {}

ADIDigitalIn::ADIDigitalIn(std::uint8_t adi_port) : ADIPort(adi_port, E_ADI_DIGITAL_IN) {}
ADIDigitalIn::ADIDigitalIn(ext_adi_port_pair_t port_pair) : ADIPort(port_pair, E_ADI_DIGITAL_IN) {}

std::int32_t ADIDigitalIn::get_new_press() const {
	return ext_adi_digital_get_new_press(_smart_port, _adi_port);
}

ADIDigitalOut::ADIDigitalOut(std::uint8_t adi_port, bool init_state) : ADIPort(adi_port, E_ADI_DIGITAL_OUT) {
	set_value(init_state);
}

ADIDigitalOut::ADIDigitalOut(ext_adi_port_pair_t port_pair, bool init_state) : ADIPort(port_pair, E_ADI_DIGITAL_IN) {
	set_value(init_state);
}

ADIMotor::ADIMotor(std::uint8_t adi_port) : ADIPort(adi_port, E_ADI_LEGACY_PWM) {
	stop();
}

ADIMotor::ADIMotor(ext_adi_port_pair_t port_pair) : ADIPort(port_pair, E_ADI_LEGACY_PWM) {
	stop();
}

std::int32_t ADIMotor::stop() const {
	return ext_adi_motor_stop(_smart_port, _adi_port);
}

ADIEncoder::ADIEncoder(std::uint8_t adi_port_top, std::uint8_t adi_port_bottom, bool reversed) : ADIPort(adi_port_top) {
	std::int32_t _port = ext_adi_encoder_init(INTERNAL_ADI_PORT, adi_port_top, adi_port_bottom, reversed);
	get_ports(_port, _smart_port, _adi_port);
}

ADIEncoder::ADIEncoder(ext_adi_port_tuple_t port_tuple, bool reversed) : ADIPort(std::get<1>(port_tuple)) {
	std::int32_t _port =
	    ext_adi_encoder_init(std::get<0>(port_tuple), std::get<1>(port_tuple), std::get<2>(port_tuple), reversed);
	get_ports(_port, _smart_port, _adi_port);

}

std::int32_t ADIEncoder::reset() const {
	return adi_encoder_reset(merge_adi_ports(_smart_port, _adi_port));
}

std::int32_t ADIEncoder::get_value() const {
	return adi_encoder_get(merge_adi_ports(_smart_port, _adi_port));
}

ADIUltrasonic::ADIUltrasonic(std::uint8_t adi_port_ping, std::uint8_t adi_port_echo) : ADIPort(adi_port_ping) {
	std::int32_t _port = ext_adi_ultrasonic_init(INTERNAL_ADI_PORT, adi_port_ping, adi_port_echo);
	get_ports(_port, _smart_port, _adi_port);
}

ADIUltrasonic::ADIUltrasonic(ext_adi_port_tuple_t port_tuple) : ADIPort(std::get<1>(port_tuple)) {
	std::int32_t _port =
	    ext_adi_ultrasonic_init(std::get<0>(port_tuple), std::get<1>(port_tuple), std::get<2>(port_tuple));
	get_ports(_port, _smart_port, _adi_port);
}

std::int32_t ADIUltrasonic::get_value() const {
	return adi_ultrasonic_get(merge_adi_ports(_smart_port, _adi_port));
}

ADIGyro::ADIGyro(std::uint8_t adi_port, double multiplier) : ADIPort(adi_port) {
	std::int32_t _port = ext_adi_gyro_init(INTERNAL_ADI_PORT, adi_port, multiplier);
	get_ports(_port, _smart_port, _adi_port);
}

ADIGyro::ADIGyro(ext_adi_port_pair_t port_pair, double multiplier) : ADIPort(std::get<1>(port_pair)) {
	std::int32_t _port = ext_adi_gyro_init(port_pair.first, port_pair.second, multiplier);
	get_ports(_port, _smart_port, _adi_port);
}

double ADIGyro::get_value() const {
	return adi_gyro_get(merge_adi_ports(_smart_port, _adi_port));
}

std::int32_t ADIGyro::reset() const {
	return adi_gyro_reset(merge_adi_ports(_smart_port, _adi_port));
}
}  // namespace pros
