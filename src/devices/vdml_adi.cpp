/**
 * \file devices/vdml_adi.cpp
 *
 * Contains functions for interacting with the V5 ADI.
 *
 * Copyright (c) 2017-2022, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "kapi.h"
#include "pros/adi.hpp"
#include "vdml/port.h"

namespace pros {
namespace adi {
using namespace pros::c;

Port::Port(std::uint8_t adi_port, adi_port_config_e_t type)
    : _smart_port(INTERNAL_ADI_PORT), _adi_port(adi_port) {
	ext_adi_port_set_config(_smart_port, _adi_port, type);
}

Port::Port(ext_adi_port_pair_t port_pair, adi_port_config_e_t type)
    : _smart_port(port_pair.first), _adi_port(port_pair.second) {
	ext_adi_port_set_config(_smart_port, _adi_port, type);
}

std::int32_t Port::set_config(adi_port_config_e_t type) const {
	return ext_adi_port_set_config(_smart_port, _adi_port, type);
}

std::int32_t Port::get_config() const {
	return ext_adi_port_get_config(_smart_port, _adi_port);
}

std::int32_t Port::set_value(std::int32_t value) const {
	return ext_adi_port_set_value(_smart_port, _adi_port, value);
}

std::int32_t Port::get_value() const {
	return ext_adi_port_get_value(_smart_port, _adi_port);
}

AnalogIn::AnalogIn(std::uint8_t adi_port) : Port(adi_port, E_ADI_ANALOG_IN) {}
AnalogIn::AnalogIn(ext_adi_port_pair_t port_pair) : Port(port_pair, E_ADI_ANALOG_IN) {}

std::int32_t AnalogIn::calibrate() const {
	return ext_adi_analog_calibrate(_smart_port, _adi_port);
}

std::int32_t AnalogIn::get_value_calibrated() const {
	return ext_adi_analog_read_calibrated(_smart_port, _adi_port);
}

std::int32_t AnalogIn::get_value_calibrated_HR() const {
	return ext_adi_analog_read_calibrated_HR(_smart_port, _adi_port);
}

AnalogOut::AnalogOut(std::uint8_t adi_port) : Port(adi_port, E_ADI_ANALOG_OUT) {}
AnalogOut::AnalogOut(ext_adi_port_pair_t port_pair) : Port(port_pair, E_ADI_ANALOG_OUT) {}

DigitalIn::DigitalIn(std::uint8_t adi_port) : Port(adi_port, E_ADI_DIGITAL_IN) {}
DigitalIn::DigitalIn(ext_adi_port_pair_t port_pair) : Port(port_pair, E_ADI_DIGITAL_IN) {}

std::int32_t DigitalIn::get_new_press() const {
	return ext_adi_digital_get_new_press(_smart_port, _adi_port);
}

DigitalOut::DigitalOut(std::uint8_t adi_port, bool init_state) : Port(adi_port, E_ADI_DIGITAL_OUT) {
	set_value(init_state);
}

DigitalOut::DigitalOut(ext_adi_port_pair_t port_pair, bool init_state) : ADIPort(port_pair, E_ADI_DIGITAL_IN) {
	set_value(init_state);
}

Motor::Motor(std::uint8_t adi_port) : Port(adi_port, E_ADI_LEGACY_PWM) {
	stop();
}

Motor::Motor(ext_adi_port_pair_t port_pair) : Port(port_pair, E_ADI_LEGACY_PWM) {
	stop();
}

std::int32_t Motor::stop() const {
	return ext_adi_motor_stop(_smart_port, _adi_port);
}

Encoder::Encoder(std::uint8_t adi_port_top, std::uint8_t adi_port_bottom, bool reversed) : Port(adi_port_top) {
	std::int32_t _port = ext_adi_encoder_init(INTERNAL_ADI_PORT, adi_port_top, adi_port_bottom, reversed);
	get_ports(_port, _smart_port, _adi_port);
}

Encoder::Encoder(ext_adi_port_tuple_t port_tuple, bool reversed) : Port(std::get<1>(port_tuple)) {
	std::int32_t _port =
	    ext_adi_encoder_init(std::get<0>(port_tuple), std::get<1>(port_tuple), std::get<2>(port_tuple), reversed);
	get_ports(_port, _smart_port, _adi_port);

}

std::int32_t Encoder::reset() const {
	return adi_encoder_reset(merge_adi_ports(_smart_port, _adi_port));
}

std::int32_t Encoder::get_value() const {
	return adi_encoder_get(merge_adi_ports(_smart_port, _adi_port));
}

Ultrasonic::Ultrasonic(std::uint8_t adi_port_ping, std::uint8_t adi_port_echo) : Port(adi_port_ping) {
	std::int32_t _port = ext_adi_ultrasonic_init(INTERNAL_ADI_PORT, adi_port_ping, adi_port_echo);
	get_ports(_port, _smart_port, _adi_port);
}

Ultrasonic::Ultrasonic(ext_adi_port_tuple_t port_tuple) : Port(std::get<1>(port_tuple)) {
	std::int32_t _port =
	    ext_adi_ultrasonic_init(std::get<0>(port_tuple), std::get<1>(port_tuple), std::get<2>(port_tuple));
	get_ports(_port, _smart_port, _adi_port);
}

std::int32_t Ultrasonic::get_value() const {
	return adi_ultrasonic_get(merge_adi_ports(_smart_port, _adi_port));
}

Gyro::Gyro(std::uint8_t adi_port, double multiplier) : Port(adi_port) {
	std::int32_t _port = ext_adi_gyro_init(INTERNAL_ADI_PORT, adi_port, multiplier);
	get_ports(_port, _smart_port, _adi_port);
}

Gyro::Gyro(ext_adi_port_pair_t port_pair, double multiplier) : ADIPort(std::get<1>(port_pair)) {
	std::int32_t _port = ext_adi_gyro_init(port_pair.first, port_pair.second, multiplier);
	get_ports(_port, _smart_port, _adi_port);
}

double Gyro::get_value() const {
	return adi_gyro_get(merge_adi_ports(_smart_port, _adi_port));
}

std::int32_t Gyro::reset() const {
	return adi_gyro_reset(merge_adi_ports(_smart_port, _adi_port));
}

Potentiometer::Potentiometer(std::uint8_t adi_port, adi_potentiometer_type_e_t potentiometer_type) : AnalogIn(adi_port) { 
	std::int32_t _port = ext_adi_potentiometer_init(INTERNAL_ADI_PORT, adi_port, potentiometer_type);
	get_ports(_port, _smart_port, _adi_port);
	_smart_port++; // for inherited functions this is necessary
}

Potentiometer::Potentiometer(ext_adi_port_pair_t port_pair, adi_potentiometer_type_e_t potentiometer_type) : AnalogIn(std::get<1>(port_pair)) { 
 	std::int32_t _port = ext_adi_potentiometer_init(port_pair.first, port_pair.second, potentiometer_type);
	get_ports(_port, _smart_port, _adi_port);
	_smart_port++; // for inherited functions this is necessary
}

double Potentiometer::get_angle() const {
	uint8_t temp_smart = _smart_port - 1;
	return ext_adi_potentiometer_get_angle(merge_adi_ports(temp_smart, _adi_port));
}
}  // namespace adi
}  // namespace pros
