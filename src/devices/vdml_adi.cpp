/**
 * \file vdml_adi.cpp
 *
 * \brief VDML ADI functionality.
 *
 * \copyright (c) 2018, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <errno.h>
#include <math.h>
#include <stdio.h>

#include "ifi/v5_api.h"
#include "kapi.h"
#include "pros/adi.h"
#include "pros/adi.hpp"
#include "vdml/registry.h"
#include "vdml/vdml.h"

ADIPort::ADIPort(uint8_t port) : _port(port) {
	adi_port_config_set(_port, E_ADI_TYPE_UNDEFINED);
}

ADIPort::ADIPort(uint8_t port, adi_port_config_e_t type) : _port(port) {
	adi_port_config_set(_port, type);
}

ADIPort::ADIPort(char port) : _port(port) {
	adi_port_config_set(_port, E_ADI_TYPE_UNDEFINED);
}

ADIPort::ADIPort(char port, adi_port_config_e_t type) : _port(port) {
	adi_port_config_set(_port, type);
}

ADIPort::~ADIPort() {
	adi_port_config_set(_port, E_ADI_TYPE_UNDEFINED);
}

int32_t ADIPort::config_set(adi_port_config_e_t type) {
	return adi_port_config_set(_port, type);
}

int32_t ADIPort::config_get() {
	return adi_port_config_get(_port);
}

int32_t ADIPort::value_set(int32_t value) {
	return adi_value_set(_port, value);
}

int32_t ADIPort::value_get() {
	return adi_value_get(_port);
}

ADIAnalogIn::ADIAnalogIn(uint8_t port) : ADIPort(port) {
	config_set(E_ADI_ANALOG_IN);
}

ADIAnalogIn::ADIAnalogIn(char port) : ADIPort(port) {
	config_set(E_ADI_ANALOG_IN);
}

int32_t ADIAnalogIn::calibrate() {
	return adi_analog_calibrate(_port);
}

int32_t ADIAnalogIn::value_get_calibrated() {
	return adi_analog_read_calibrated(_port);
}

int32_t ADIAnalogIn::value_get_calibrated_HR() {
	return adi_analog_read_calibrated_HR(_port);
}

ADIDigitalOut::ADIDigitalOut(uint8_t port) : ADIPort(port) {
	config_set(E_ADI_DIGITAL_OUT);
	value_set(LOW);
}

ADIDigitalOut::ADIDigitalOut(uint8_t port, bool init_state) : ADIPort(port) {
	config_set(E_ADI_DIGITAL_OUT);
	value_set(init_state);
}

ADIDigitalOut::ADIDigitalOut(char port) : ADIPort(port) {
	config_set(E_ADI_DIGITAL_OUT);
	value_set(LOW);
}

ADIDigitalOut::ADIDigitalOut(char port, bool init_state) : ADIPort(port) {
	config_set(E_ADI_DIGITAL_OUT);
	value_set(init_state);
}

ADIDigitalIn::ADIDigitalIn(uint8_t port) : ADIPort(port) {
	config_set(E_ADI_DIGITAL_IN);
}

ADIMotor::ADIMotor(uint8_t port) : ADIPort(port) {
	config_set(E_ADI_LEGACY_PWM);
	stop();
}

ADIDigitalIn::ADIDigitalIn(char port) : ADIPort(port) {
	config_set(E_ADI_DIGITAL_IN);
}

ADIMotor::ADIMotor(char port) : ADIPort(port) {
	config_set(E_ADI_LEGACY_PWM);
	stop();
}

int32_t ADIMotor::stop() {
	return adi_motor_stop(_port);
}

ADIEncoder::ADIEncoder(uint8_t port_bottom, uint8_t port_top) {
	_port = adi_encoder_init(port_bottom, port_top, false);
}

ADIEncoder::ADIEncoder(uint8_t port_bottom, uint8_t port_top, bool reversed) {
	_port = adi_encoder_init(port_bottom, port_top, reversed);
}

ADIEncoder::ADIEncoder(char port_bottom, char port_top) {
	_port = adi_encoder_init(port_bottom, port_top, false);
}

ADIEncoder::ADIEncoder(char port_bottom, char port_top, bool reversed) {
	_port = adi_encoder_init(port_bottom, port_top, reversed);
}

int32_t ADIEncoder::reset() {
	return adi_encoder_reset(_port);
}

ADIUltrasonic::ADIUltrasonic(uint8_t port_bottom, uint8_t port_top) {
	_port = adi_ultrasonic_init(port_bottom, port_top);
}

ADIUltrasonic::ADIUltrasonic(char port_bottom, char port_top) {
	_port = adi_ultrasonic_init(port_bottom, port_top);
}
