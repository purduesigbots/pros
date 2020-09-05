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

ADIPort::ADIPort(std::uint8_t port, adi_port_config_e_t type) : _adi_port(port) {
	_smart_port = INTERNAL_ADI_PORT;
	adi_port_set_config( _adi_port, type);
	//_port = merge_adi_ports(_adi_port,_smart_port);
}

ADIPort::ADIPort(std::uint8_t smart_port, std::uint8_t adi_port, adi_port_config_e_t type) : _smart_port(smart_port), _adi_port(adi_port) {
	ext_adi_port_set_config(_smart_port, _adi_port, type);
	//_port = merge_adi_ports(_adi_port,_smart_port);
}

ADIPort::ADIPort(void) {
	// for use by derived classes like ADIEncoder
}

std::int32_t ADIPort::set_config(adi_port_config_e_t type) const {
	return ext_adi_port_set_config(_smart_port,_adi_port, type);
}

std::int32_t ADIPort::get_config(void) const {
	return ext_adi_port_get_config(_smart_port,_adi_port);
}

std::int32_t ADIPort::set_value(std::int32_t value) const {
	return ext_adi_port_set_value(_smart_port,_adi_port, value);
}

std::int32_t ADIPort::get_value(void) const {
	return ext_adi_port_get_value(_smart_port,_adi_port);
}

ADIAnalogIn::ADIAnalogIn(std::uint8_t port) : ADIPort(port, E_ADI_ANALOG_IN) {}
ADIAnalogIn::ADIAnalogIn(std::uint8_t smart_port, std::uint8_t adi_port) : ADIPort(smart_port, adi_port, E_ADI_ANALOG_IN) {}
 
std::int32_t ADIAnalogIn::calibrate(void) const {
	return ext_adi_analog_calibrate(_smart_port,_adi_port);
}

std::int32_t ADIAnalogIn::get_value_calibrated(void) const {
	return ext_adi_analog_read_calibrated(_smart_port,_adi_port);
}

std::int32_t ADIAnalogIn::get_value_calibrated_HR(void) const {
	return ext_adi_analog_read_calibrated_HR(_smart_port,_adi_port);
}

ADIAnalogOut::ADIAnalogOut(std::uint8_t port) : ADIPort(port, E_ADI_ANALOG_OUT) {}
ADIAnalogOut::ADIAnalogOut(std::uint8_t smart_port, std::uint8_t adi_port) : ADIPort(smart_port, adi_port, E_ADI_ANALOG_OUT) {}

ADIDigitalOut::ADIDigitalOut(std::uint8_t port, bool init_state) : ADIPort(port, E_ADI_DIGITAL_OUT) {
	set_value(init_state);
}

ADIDigitalOut::ADIDigitalOut(std::uint8_t smart_port, std::uint8_t adi_port, bool init_state)
    : ADIPort(smart_port, adi_port, E_ADI_DIGITAL_OUT) {
	set_value(init_state);
}

ADIDigitalIn::ADIDigitalIn(std::uint8_t port) : ADIPort(port, E_ADI_DIGITAL_IN) {}
ADIDigitalIn::ADIDigitalIn(std::uint8_t smart_port, std::uint8_t adi_port): ADIPort(smart_port, adi_port, E_ADI_DIGITAL_IN) {}

std::int32_t ADIDigitalIn::get_new_press(void) const {
	return ext_adi_digital_get_new_press(_smart_port,_adi_port);
}

ADIMotor::ADIMotor(std::uint8_t port) : ADIPort(port, E_ADI_LEGACY_PWM) {
	stop();
}

ADIMotor::ADIMotor(std::uint8_t smart_port, std::uint8_t adi_port) : ADIPort(smart_port, adi_port, E_ADI_LEGACY_PWM) {
	stop();
}

std::int32_t ADIMotor::stop(void) const {
	return ext_adi_motor_stop(_smart_port,_adi_port);
}

ADIEncoder::ADIEncoder(std::uint8_t port_top, std::uint8_t port_bottom, bool reversed) {
	_port = adi_encoder_init(port_top, port_bottom, reversed);
}

ADIEncoder::ADIEncoder(std::uint8_t smart_port, std::uint8_t adi_port_top, std::uint8_t adi_port_bottom,bool reversed) {
	_port = ext_adi_encoder_init(smart_port, adi_port_top, adi_port_bottom, reversed);
}

std::int32_t ADIEncoder::reset(void) const {
	return adi_encoder_reset(_port);
}

std::int32_t ADIEncoder::get_value(void) const {
	return adi_encoder_get(_port);
}

ADIUltrasonic::ADIUltrasonic(std::uint8_t port_ping, std::uint8_t port_echo) {
	_port = adi_ultrasonic_init(port_ping, port_echo);
}

ADIUltrasonic::ADIUltrasonic(std::uint8_t smart_port, std::uint8_t adi_port_ping, std::uint8_t adi_port_echo) {
	_port = ext_adi_ultrasonic_init(smart_port, adi_port_ping, adi_port_echo);
}

std::int32_t ADIUltrasonic::get_value(void) const {
	return adi_ultrasonic_get(_port);
}

ADIGyro::ADIGyro(std::uint8_t adi_port, double multiplier) {
	_port = adi_gyro_init(adi_port, multiplier);
}

ADIGyro::ADIGyro(std::uint8_t smart_port, std::uint8_t adi_port, double multiplier) {
	_port = ext_adi_gyro_init(smart_port, adi_port, multiplier);
}

double ADIGyro::get_value(void) const {
	return adi_gyro_get(_port);
}

std::int32_t ADIGyro::reset(void) const {
	return adi_gyro_reset(_port);
}
}  // namespace pros
