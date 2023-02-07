/**
 * \file devices/vdml_adi.cpp
 *
 * Contains functions for interacting with the V5 ADI.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "kapi.h"
#include "pros/adi.hpp"
#include "vdml/port.h"

#define MAX_LED 64

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

ADIDigitalOut::ADIDigitalOut(ext_adi_port_pair_t port_pair, bool init_state) : ADIPort(port_pair, E_ADI_DIGITAL_OUT) {
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
	return ext_adi_encoder_reset(merge_adi_ports(_smart_port, _adi_port));
}

std::int32_t ADIEncoder::get_value() const {
	return ext_adi_encoder_get(merge_adi_ports(_smart_port, _adi_port));
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
	return ext_adi_ultrasonic_get(merge_adi_ports(_smart_port, _adi_port));
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
	return ext_adi_gyro_get(merge_adi_ports(_smart_port, _adi_port));
}

std::int32_t ADIGyro::reset() const {
	return ext_adi_gyro_reset(merge_adi_ports(_smart_port, _adi_port));
}

ADIPotentiometer::ADIPotentiometer(std::uint8_t adi_port, adi_potentiometer_type_e_t potentiometer_type) : ADIAnalogIn(adi_port) { 
	std::int32_t _port = ext_adi_potentiometer_init(INTERNAL_ADI_PORT, adi_port, potentiometer_type);
	get_ports(_port, _smart_port, _adi_port);
	_smart_port++; // for inherited functions this is necessary
}

ADIPotentiometer::ADIPotentiometer(ext_adi_port_pair_t port_pair, adi_potentiometer_type_e_t potentiometer_type) : ADIAnalogIn(std::get<1>(port_pair)) { 
 	std::int32_t _port = ext_adi_potentiometer_init(port_pair.first, port_pair.second, potentiometer_type);
	get_ports(_port, _smart_port, _adi_port);
	_smart_port++; // for inherited functions this is necessary
}

double ADIPotentiometer::get_angle() const {
	uint8_t temp_smart = _smart_port - 1;
	return ext_adi_potentiometer_get_angle(merge_adi_ports(temp_smart, _adi_port));
}

ADILed::ADILed(std::uint8_t adi_port, std::uint32_t length) : ADIPort(adi_port) {
	std::int32_t _port = ext_adi_led_init(INTERNAL_ADI_PORT, adi_port);
	get_ports(_port, _smart_port, _adi_port);
	if (length < 1) {
		length = 0;
	}
	if (length > MAX_LED) {
		length = MAX_LED;
	}
	_buffer.resize(length, 0);
}

ADILed::ADILed(ext_adi_port_pair_t port_pair, std::uint32_t length) : ADIPort(std::get<1>(port_pair)) {
	std::int32_t _port = ext_adi_led_init(port_pair.first, port_pair.second);
	get_ports(_port, _smart_port, _adi_port);
	if (length < 1) {
		length = 0;
	}
	if (length > MAX_LED) {
		length = MAX_LED;
	}
	_buffer.resize(length, 0);
}

uint32_t& ADILed::operator[] (size_t index) {
	return _buffer[index];
}

std::int32_t ADILed::update() const {
	return ext_adi_led_set(merge_adi_ports(_smart_port, _adi_port), (uint32_t*)_buffer.data(), _buffer.size());
}

std::int32_t ADILed::length() {
	return _buffer.size();
}

std::int32_t ADILed::set_all(uint32_t color) {
	return ext_adi_led_set_all((adi_led_t)merge_adi_ports(_smart_port, _adi_port), (uint32_t*)_buffer.data(), _buffer.size(), color);
}

std::int32_t ADILed::set_pixel(uint32_t color, uint32_t pixel_position) {
	return ext_adi_led_set_pixel((adi_led_t)merge_adi_ports(_smart_port, _adi_port), (uint32_t*)_buffer.data(), _buffer.size(), color, pixel_position);
}

std::int32_t ADILed::clear_all() {
	return ext_adi_led_clear_all((adi_led_t)merge_adi_ports(_smart_port, _adi_port), (uint32_t*)_buffer.data(), _buffer.size());
}

std::int32_t ADILed::clear() {
	return ext_adi_led_clear_all((adi_led_t)merge_adi_ports(_smart_port, _adi_port), (uint32_t*)_buffer.data(), _buffer.size());
}

std::int32_t ADILed::clear_pixel(uint32_t pixel_position) {
	return ext_adi_led_clear_pixel((adi_led_t)merge_adi_ports(_smart_port, _adi_port), (uint32_t*)_buffer.data(), _buffer.size(), pixel_position);
}

}  // namespace pros
