/**
 * \file devices/vdml_adi.c
 *
 * Contains functions for interacting with the V5 ADI.
 *
 * Copyright (c) 2017-2020, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "vdml/registry.h"
#include "vdml/vdml.h"

adi_port_config_e_t adi_port_get_config(uint8_t port) {
	return ext_adi_port_get_config(INTERNAL_ADI_PORT, port);
}

int32_t adi_port_get_value(uint8_t port) {
	return ext_adi_port_get_value(INTERNAL_ADI_PORT, port);
}

int32_t adi_port_set_config(uint8_t port, adi_port_config_e_t type) {
	return ext_adi_port_set_config(INTERNAL_ADI_PORT, port, type);
}

int32_t adi_port_set_value(uint8_t port, int32_t value) {
	return ext_adi_port_set_value(INTERNAL_ADI_PORT, port, value);
}

int32_t adi_analog_calibrate(uint8_t port) {
	return ext_adi_analog_calibrate(INTERNAL_ADI_PORT, port);
}

int32_t adi_analog_read(uint8_t port) {
	return ext_adi_analog_read(INTERNAL_ADI_PORT, port);
}

int32_t adi_analog_read_calibrated(uint8_t port) {
	return ext_adi_analog_read_calibrated(INTERNAL_ADI_PORT, port);
}

int32_t adi_analog_read_calibrated_HR(uint8_t port) {
	return ext_adi_analog_read_calibrated_HR(INTERNAL_ADI_PORT, port);
}

int32_t adi_digital_read(uint8_t port) {
	return ext_adi_digital_read(INTERNAL_ADI_PORT, port);
}

int32_t adi_digital_get_new_press(uint8_t port) {
	return ext_adi_digital_get_new_press(INTERNAL_ADI_PORT, port);
}

int32_t adi_digital_write(uint8_t port, bool value) {
	return ext_adi_digital_write(INTERNAL_ADI_PORT, port, value);
}

int32_t adi_pin_mode(uint8_t port, uint8_t mode) {
	return ext_adi_pin_mode(INTERNAL_ADI_PORT, port, mode);
}

int32_t adi_motor_set(uint8_t port, int8_t speed) {
	return ext_adi_motor_set(INTERNAL_ADI_PORT, port, speed);
}

int32_t adi_motor_get(uint8_t port) {
	return ext_adi_motor_get(INTERNAL_ADI_PORT, port);
}

int32_t adi_motor_stop(uint8_t port) {
	return ext_adi_motor_stop(INTERNAL_ADI_PORT, port);
}

adi_encoder_t adi_encoder_init(uint8_t port_top, uint8_t port_bottom, bool reverse) {
	return (adi_encoder_t)ext_adi_encoder_init(INTERNAL_ADI_PORT, port_top, port_bottom, reverse);
}

int32_t adi_encoder_get(adi_encoder_t enc) {
	return ext_adi_encoder_get((ext_adi_encoder_t)enc);
}

int32_t adi_encoder_reset(adi_encoder_t enc) {
	return ext_adi_encoder_reset((ext_adi_encoder_t)enc);
}

int32_t adi_encoder_shutdown(adi_encoder_t enc) {
	return ext_adi_encoder_shutdown((ext_adi_encoder_t)enc);
}

adi_ultrasonic_t adi_ultrasonic_init(uint8_t port_ping, uint8_t port_echo) {
	return (adi_ultrasonic_t)ext_adi_ultrasonic_init(INTERNAL_ADI_PORT, port_ping, port_echo);
}

int32_t adi_ultrasonic_get(adi_ultrasonic_t ult) {
	return ext_adi_ultrasonic_get((ext_adi_ultrasonic_t)ult);
}

int32_t adi_ultrasonic_shutdown(adi_ultrasonic_t ult) {
	return ext_adi_ultrasonic_shutdown((ext_adi_ultrasonic_t)ult);
}

adi_gyro_t adi_gyro_init(uint8_t adi_port, double multiplier) {
	return (adi_gyro_t)ext_adi_gyro_init(INTERNAL_ADI_PORT, adi_port, multiplier);
}

double adi_gyro_get(adi_gyro_t gyro) {
	return ext_adi_gyro_get((ext_adi_gyro_t)gyro);
}

int32_t adi_gyro_reset(adi_gyro_t gyro) {
	return ext_adi_gyro_reset((ext_adi_gyro_t)gyro);
}

int32_t adi_gyro_shutdown(adi_gyro_t gyro) {
	return ext_adi_gyro_shutdown((ext_adi_gyro_t)gyro);
}