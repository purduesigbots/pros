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

ext_adi_port_tuple_t Port::get_port() const {
	return std::make_tuple(_smart_port, _adi_port, PROS_ERR_BYTE);
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

std::ostream& operator<<(std::ostream& os, pros::adi::AnalogIn& analog_in) {
	os << "AnalogIn [";
	os << "smart_port: " << +analog_in._smart_port;
	os << ", adi_port: " << ((analog_in._adi_port > 10) ? analog_in._adi_port : +analog_in._adi_port);
	os << ", value calibrated: " << analog_in.get_value_calibrated();
	os << ", value calibrated HR: " << analog_in.get_value_calibrated_HR();
	os << ", value: " << analog_in.get_value();
	os << "]";

	return os;
}

AnalogOut::AnalogOut(std::uint8_t adi_port) : Port(adi_port, E_ADI_ANALOG_OUT) {}
AnalogOut::AnalogOut(ext_adi_port_pair_t port_pair) : Port(port_pair, E_ADI_ANALOG_OUT) {}

std::ostream& operator<<(std::ostream& os, pros::adi::AnalogOut& analog_out) {
	os << "AnalogOut [";
	os << "smart_port: " << +analog_out._smart_port;
	os << ", adi_port: " << ((analog_out._adi_port > 10) ? analog_out._adi_port : +analog_out._adi_port);
	os << ", value: " << analog_out.get_value();
	os << "]";

	return os;
}

DigitalIn::DigitalIn(std::uint8_t adi_port) : Port(adi_port, E_ADI_DIGITAL_IN) {}
DigitalIn::DigitalIn(ext_adi_port_pair_t port_pair) : Port(port_pair, E_ADI_DIGITAL_IN) {}

std::int32_t DigitalIn::get_new_press() const {
	return ext_adi_digital_get_new_press(_smart_port, _adi_port);
}

std::ostream& operator<<(std::ostream& os, pros::adi::DigitalIn& digital_in) {
	os << "DigitalIn [";
	os << "smart_port: " << +digital_in._smart_port;
	os << ", adi_port: " << ((digital_in._adi_port > 10) ? digital_in._adi_port : +digital_in._adi_port);
	os << ", value: " << digital_in.get_value();
	os << "]";

	return os;
}

DigitalOut::DigitalOut(std::uint8_t adi_port, bool init_state) : Port(adi_port, E_ADI_DIGITAL_OUT) {
	set_value(init_state);
}

DigitalOut::DigitalOut(ext_adi_port_pair_t port_pair, bool init_state) : ADIPort(port_pair, E_ADI_DIGITAL_IN) {
	set_value(init_state);
}

std::ostream& operator<<(std::ostream& os, pros::adi::DigitalOut& digital_out) {
	os << "DigitalOut [";
	os << "smart_port: " << +digital_out._smart_port;
	os << ", adi_port: " << ((digital_out._adi_port > 10) ? digital_out._adi_port : +digital_out._adi_port);
	os << ", value: " << digital_out.get_value();
	os << "]";

	return os;
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

Encoder::Encoder(std::uint8_t adi_port_top, std::uint8_t adi_port_bottom, bool reversed) : Port(adi_port_top),  _port_pair(adi_port_top, adi_port_bottom) {
	std::int32_t _port = ext_adi_encoder_init(INTERNAL_ADI_PORT, adi_port_top, adi_port_bottom, reversed);
	get_ports(_port, _smart_port, _adi_port);
}

Encoder::Encoder(ext_adi_port_tuple_t port_tuple, bool reversed) : Port(std::get<1>(port_tuple)), _port_pair(std::get<1>(port_tuple), std::get<2>(port_tuple)) {
	std::int32_t _port =
	    ext_adi_encoder_init(std::get<0>(port_tuple), std::get<1>(port_tuple), std::get<2>(port_tuple), reversed);
	get_ports(_port, _smart_port, _adi_port);
}

std::int32_t Encoder::reset() const {
	return ext_adi_encoder_reset(merge_adi_ports(_smart_port, _adi_port));
}

std::int32_t Encoder::get_value() const {
	return ext_adi_encoder_get(merge_adi_ports(_smart_port, _adi_port));
}

ext_adi_port_tuple_t ADIEncoder::get_port() const {
	return std::make_tuple(_smart_port, std::get<0>(_port_pair), std::get<1>(_port_pair));
}

std::ostream& operator<<(std::ostream& os, pros::adi::Encoder& encoder) {
	os << "Encoder [";
	os << "smart_port: " << +encoder._smart_port;
	os << ", adi_port: " << ((encoder._adi_port > 10) ? encoder._adi_port : +encoder._adi_port);
	os << ", value: " << encoder.get_value();
	os << "]";

	return os;
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
	return ext_adi_ultrasonic_get(merge_adi_ports(_smart_port, _adi_port));
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
	return ext_adi_gyro_get(merge_adi_ports(_smart_port, _adi_port));
}

std::int32_t Gyro::reset() const {
	return ext_adi_gyro_reset(merge_adi_ports(_smart_port, _adi_port));
}

Potentiometer::Potentiometer(std::uint8_t adi_port, adi_potentiometer_type_e_t potentiometer_type) : AnalogIn(adi_port) { 
	std::int32_t _port = ext_adi_potentiometer_init(INTERNAL_ADI_PORT, adi_port, potentiometer_type);
	get_ports(_port, _smart_port, _adi_port);
}

Potentiometer::Potentiometer(ext_adi_port_pair_t port_pair, adi_potentiometer_type_e_t potentiometer_type) : AnalogIn(std::get<1>(port_pair)) { 
 	std::int32_t _port = ext_adi_potentiometer_init(port_pair.first, port_pair.second, potentiometer_type);
	get_ports(_port, _smart_port, _adi_port);
}

double Potentiometer::get_angle() const {
	uint8_t temp_smart = _smart_port - 1;
	return ext_adi_potentiometer_get_angle(merge_adi_ports(temp_smart, _adi_port));
}

Led::Led(std::uint8_t adi_port, std::uint32_t length) : Port(adi_port) {
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

Led::Led(ext_adi_port_pair_t port_pair, std::uint32_t length) : Port(std::get<1>(port_pair)) {
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

uint32_t& Led::operator[] (size_t index) {
	return _buffer[index];
}

std::int32_t Led::update() const {
	return ext_adi_led_set(merge_adi_ports(_smart_port, _adi_port), (uint32_t*)_buffer.data(), _buffer.size());
}

std::int32_t Led::length() {
	return _buffer.size();
}

std::int32_t Led::set_all(uint32_t color) {
	return ext_adi_led_set_all((adi_led_t)merge_adi_ports(_smart_port, _adi_port), (uint32_t*)_buffer.data(), _buffer.size(), color);
}

std::int32_t Led::set_pixel(uint32_t color, uint32_t pixel_position) {
	return ext_adi_led_set_pixel((adi_led_t)merge_adi_ports(_smart_port, _adi_port), (uint32_t*)_buffer.data(), _buffer.size(), color, pixel_position);
}

std::int32_t Led::clear_all() {
	return ext_adi_led_clear_all((adi_led_t)merge_adi_ports(_smart_port, _adi_port), (uint32_t*)_buffer.data(), _buffer.size());
}

std::int32_t Led::clear() {
	return ext_adi_led_clear_all((adi_led_t)merge_adi_ports(_smart_port, _adi_port), (uint32_t*)_buffer.data(), _buffer.size());
}

std::int32_t Led::clear_pixel(uint32_t pixel_position) {
	return ext_adi_led_clear_pixel((adi_led_t)merge_adi_ports(_smart_port, _adi_port), (uint32_t*)_buffer.data(), _buffer.size(), pixel_position);
}

Pneumatics::Pneumatics(std::uint8_t adi_port, bool start_extended, bool extended_is_low)
	: DigitalOut(adi_port), state(start_extended ^ extended_is_low), extended_is_low(extended_is_low) {
	set_value(state);
}

Pneumatics::Pneumatics(ext_adi_port_pair_t port_pair, bool start_extended, bool extended_is_low) 
	: DigitalOut(port_pair), state(start_extended ^ extended_is_low), extended_is_low(extended_is_low) {
	set_value(state);
}

std::int32_t Pneumatics::extend() {
	bool old_state = state;

	state = !extended_is_low;
	if(set_value(state) == PROS_ERR) {
		return PROS_ERR;
	}

	return state != old_state;
}

std::int32_t Pneumatics::retract() {
	bool old_state = state;

	state = extended_is_low;
	if(set_value(state) == PROS_ERR) {
		return PROS_ERR;
	}

	return state != old_state;
}

std::int32_t Pneumatics::toggle() {
	state = !state;
	return set_value(state);
}

bool Pneumatics::is_extended() const {
	return state ^ extended_is_low;
}

std::ostream& operator<<(std::ostream& os, pros::adi::Potentiometer& potentiometer) {
	os << "Potentiometer [";
	os << "smart_port: " << +potentiometer._smart_port;
	os << ", adi_port: " << ((potentiometer._adi_port > 10) ? potentiometer._adi_port : +potentiometer._adi_port);
	os << ", value: " << potentiometer.get_value();
	os << ", value calibrated: " << potentiometer.get_value_calibrated();
	os << ", angle: " << potentiometer.get_angle();
	os << "]";

	return os;
}
}  // namespace adi
}  // namespace pros
