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
namespace adi{
using namespace pros::c;

Port::Port(std::uint8_t port, adi_port_config_e_t type) : _port(port) {
	adi_port_set_config(_port, type);
}

Port::Port(void) {
	// for use by derived classes like Encoder
}

std::int32_t Port::set_config(adi_port_config_e_t type) const {
	return adi_port_set_config(_port, type);
}

std::int32_t Port::get_config(void) const {
	return adi_port_get_config(_port);
}

std::int32_t Port::set_value(std::int32_t value) const {
	return adi_port_set_value(_port, value);
}

std::int32_t Port::get_value(void) const {
	return adi_port_get_value(_port);
}

AnalogIn::AnalogIn(std::uint8_t port) : Port(port, E_ADI_ANALOG_IN) {}

AnalogOut::AnalogOut(std::uint8_t port) : Port(port, E_ADI_ANALOG_OUT) {}

std::int32_t AnalogIn::calibrate(void) const {
	return adi_analog_calibrate(_port);
}

std::int32_t AnalogIn::get_value_calibrated(void) const {
	return adi_analog_read_calibrated(_port);
}

std::int32_t AnalogIn::get_value_calibrated_HR(void) const {
	return adi_analog_read_calibrated_HR(_port);
}

DigitalOut::DigitalOut(std::uint8_t port, bool init_state) : Port(port, E_ADI_DIGITAL_OUT) {
	set_value(init_state);
}

DigitalIn::DigitalIn(std::uint8_t port) : Port(port, E_ADI_DIGITAL_IN) {}

std::int32_t DigitalIn::get_new_press(void) const {
	return adi_digital_get_new_press(_port);
}

Motor::Motor(std::uint8_t port) : Port(port, E_ADI_LEGACY_PWM) {
	stop();
}

std::int32_t Motor::stop(void) const {
	return adi_motor_stop(_port);
}

Encoder::Encoder(std::uint8_t port_top, std::uint8_t port_bottom, bool reversed) {
	_port = adi_encoder_init(port_top, port_bottom, reversed);
}

std::int32_t Encoder::reset(void) const {
	return adi_encoder_reset(_port);
}

std::int32_t Encoder::get_value(void) const {
	return adi_encoder_get(_port);
}

Ultrasonic::Ultrasonic(std::uint8_t port_ping, std::uint8_t port_echo) {
	_port = adi_ultrasonic_init(port_ping, port_echo);
}

Gyro::Gyro(std::uint8_t port, double multiplier) {
	_port = adi_gyro_init(port, multiplier);
}

double Gyro::get_value(void) const {
	return adi_gyro_get(_port);
}

std::int32_t Gyro::reset(void) const {
	return adi_gyro_reset(_port);
}
}  // namespace adi
}  // namespace pros
