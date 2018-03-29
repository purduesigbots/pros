/**
 * \file vdml_adi.cpp
 *
 * \brief VDML ADI functionality.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "kapi.h"
#include "pros/adi.hpp"

namespace pros {
ADIPort::ADIPort(std::uint8_t port) : _port(port) {
	adi_port_config_set(_port, E_ADI_TYPE_UNDEFINED);
}

ADIPort::ADIPort(std::uint8_t port, adi_port_config_e_t type) : _port(port) {
	adi_port_config_set(_port, type);
}
ADIPort::ADIPort() {
	// for use by derived classes like ADIEncoder
}

ADIPort::~ADIPort() {
	adi_port_config_set(_port, E_ADI_TYPE_UNDEFINED);
}

std::int32_t ADIPort::config_set(adi_port_config_e_t type) const {
	return adi_port_config_set(_port, type);
}

std::int32_t ADIPort::config_get() const {
	return adi_port_config_get(_port);
}

std::int32_t ADIPort::value_set(std::int32_t value) const {
	return adi_value_set(_port, value);
}

std::int32_t ADIPort::value_get() const {
	return adi_value_get(_port);
}

ADIAnalogIn::ADIAnalogIn(std::uint8_t port) : ADIPort(port) {
	config_set(E_ADI_ANALOG_IN);
}

ADIAnalogOut::ADIAnalogOut(std::uint8_t port) : ADIPort(port) {
	config_set(E_ADI_ANALOG_OUT);
}

std::int32_t ADIAnalogIn::calibrate() const {
	return adi_analog_calibrate(_port);
}

std::int32_t ADIAnalogIn::value_get_calibrated() const {
	return adi_analog_read_calibrated(_port);
}

std::int32_t ADIAnalogIn::value_get_calibrated_HR() const {
	return adi_analog_read_calibrated_HR(_port);
}

ADIDigitalOut::ADIDigitalOut(std::uint8_t port) : ADIPort(port) {
	config_set(E_ADI_DIGITAL_OUT);
	value_set(LOW);
}

ADIDigitalOut::ADIDigitalOut(std::uint8_t port, bool init_state) : ADIPort(port) {
	config_set(E_ADI_DIGITAL_OUT);
	value_set(init_state);
}

ADIDigitalIn::ADIDigitalIn(std::uint8_t port) : ADIPort(port) {
	config_set(E_ADI_DIGITAL_IN);
}

std::int32_t ADIDigitalIn::get_new_press() const {
	return adi_digital_get_new_press(_port);
}

ADIMotor::ADIMotor(std::uint8_t port) : ADIPort(port) {
	config_set(E_ADI_LEGACY_PWM);
	stop();
}

std::int32_t ADIMotor::stop() const {
	return adi_motor_stop(_port);
}

ADIEncoder::ADIEncoder(std::uint8_t port_bottom, std::uint8_t port_top) {
	_port = adi_encoder_init(port_bottom, port_top, false);
}

ADIEncoder::ADIEncoder(std::uint8_t port_bottom, std::uint8_t port_top, bool reversed) {
	_port = adi_encoder_init(port_bottom, port_top, reversed);
}

std::int32_t ADIEncoder::reset() const {
	return adi_encoder_reset(_port);
}

ADIUltrasonic::ADIUltrasonic(std::uint8_t port_bottom, std::uint8_t port_top) {
	_port = adi_ultrasonic_init(port_bottom, port_top);
}
}
