/**
 * \file devices/vdml_adi.c
 *
 * Contains functions for interacting with the V5 ADI.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
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
#include "vdml/registry.h"
#include "vdml/vdml.h"

#define INTERNAL_ADI_PORT 21

#define ADI_MOTOR_MAX_SPEED 127
#define ADI_MOTOR_MIN_SPEED -128

#define NUM_MAX_TWOWIRE 4

// Theoretical calibration time is 1024ms, but in practice this seemed to be the
// actual time that it takes.
#define GYRO_CALIBRATION_TIME 1300

typedef union adi_data {
	struct {
		int32_t calib;
	} analog_data;
	struct {
		bool was_pressed;
	} digital_data;
	struct {
		bool reversed;
	} encoder_data;
	struct __attribute__((packed)) {
		double multiplier;
		double tare_value;
	} gyro_data;
} adi_data_s_t;

static int32_t get_analog_calib(uint8_t port) {
	adi_data_s_t* const adi_data = &((adi_data_s_t*)(registry_get_device(INTERNAL_ADI_PORT)->pad))[port];
	return adi_data->analog_data.calib;
}

static void set_analog_calib(uint8_t port, int32_t calib) {
	port_mutex_take(port);
	adi_data_s_t* const adi_data = &((adi_data_s_t*)(registry_get_device(INTERNAL_ADI_PORT)->pad))[port];
	adi_data->analog_data.calib = calib;
	port_mutex_give(port);
}

static bool get_digital_pressed(uint8_t port) {
	adi_data_s_t* const adi_data = &((adi_data_s_t*)(registry_get_device(INTERNAL_ADI_PORT)->pad))[port];
	return adi_data->digital_data.was_pressed;
}

static void set_digital_pressed(uint8_t port, bool val) {
	port_mutex_take(port);
	adi_data_s_t* const adi_data = &((adi_data_s_t*)(registry_get_device(INTERNAL_ADI_PORT)->pad))[port];
	adi_data->digital_data.was_pressed = val;
	port_mutex_give(port);
}

static bool get_encoder_reversed(uint8_t port) {
	adi_data_s_t* const adi_data = &((adi_data_s_t*)(registry_get_device(INTERNAL_ADI_PORT)->pad))[port];
	return adi_data->encoder_data.reversed;
}

static void set_encoder_reversed(uint8_t port, bool val) {
	port_mutex_take(port);
	adi_data_s_t* const adi_data = &((adi_data_s_t*)(registry_get_device(INTERNAL_ADI_PORT)->pad))[port];
	adi_data->encoder_data.reversed = val;
	port_mutex_give(port);
}

static double get_gyro_multiplier(uint8_t port) {
	adi_data_s_t* const adi_data = &((adi_data_s_t*)(registry_get_device(INTERNAL_ADI_PORT)->pad))[port];
	return adi_data->gyro_data.multiplier;
}

static double get_gyro_tare(uint8_t port) {
	adi_data_s_t* const adi_data = &((adi_data_s_t*)(registry_get_device(INTERNAL_ADI_PORT)->pad))[port];
	return adi_data->gyro_data.tare_value;
}

static void set_gyro_multiplier(uint8_t port, double mult) {
	port_mutex_take(port);
	adi_data_s_t* const adi_data = &((adi_data_s_t*)(registry_get_device(INTERNAL_ADI_PORT)->pad))[port];
	adi_data->gyro_data.multiplier = mult;
	port_mutex_give(port);
}

static void set_gyro_tare(uint8_t port, double tare) {
	port_mutex_take(port);
	adi_data_s_t* const adi_data = &((adi_data_s_t*)(registry_get_device(INTERNAL_ADI_PORT)->pad))[port];
	adi_data->gyro_data.tare_value = tare;
	port_mutex_give(port);
}

#define transform_adi_port(port)       \
	if (port >= 'a' && port <= 'h')      \
		port -= 'a';                       \
	else if (port >= 'A' && port <= 'H') \
		port -= 'A';                       \
	else                                 \
		port--;                            \
	if (port > 7 || port < 0) {          \
		errno = EINVAL;                    \
		return PROS_ERR;                   \
	}

#define validate_type(port, type)                          \
	adi_port_config_e_t config = _adi_port_get_config(port); \
	if (config != type) {                                    \
		return PROS_ERR;                                       \
	}

#define validate_type_f(port, type)                        \
	adi_port_config_e_t config = _adi_port_get_config(port); \
	if (config != type) {                                    \
		return PROS_ERR_F;                                     \
	}

#define validate_motor(port)                                        \
	adi_port_config_e_t config = _adi_port_get_config(port);          \
	if (config != E_ADI_LEGACY_PWM && config != E_ADI_LEGACY_SERVO) { \
		errno = EINVAL;                                                 \
		return PROS_ERR;                                                \
	}

#define validate_twowire(port_top, port_bottom) \
	if (abs(port_top - port_bottom) > 1) {        \
		errno = EINVAL;                             \
		return PROS_ERR;                            \
	}                                             \
	int port;                                     \
	if (port_top < port_bottom)                   \
		port = port_top;                            \
	else if (port_bottom < port_top)              \
		port = port_bottom;                         \
	else                                          \
		return PROS_ERR;                            \
	if (port % 2 == 1) {                          \
		return PROS_ERR;                            \
	}

static inline int32_t _adi_port_set_config(uint8_t port, adi_port_config_e_t type) {
	claim_port(INTERNAL_ADI_PORT, E_DEVICE_ADI);
	vexDeviceAdiPortConfigSet(device->device_info, port, type);
	return_port(INTERNAL_ADI_PORT, 1);
}

static inline adi_port_config_e_t _adi_port_get_config(uint8_t port) {
	claim_port(INTERNAL_ADI_PORT, E_DEVICE_ADI);
	adi_port_config_e_t rtn = (adi_port_config_e_t)vexDeviceAdiPortConfigGet(device->device_info, port);
	return_port(INTERNAL_ADI_PORT, rtn);
}

static inline int32_t _adi_port_set_value(uint8_t port, int32_t value) {
	claim_port(INTERNAL_ADI_PORT, E_DEVICE_ADI);
	vexDeviceAdiValueSet(device->device_info, port, value);
	return_port(INTERNAL_ADI_PORT, 1);
}

static inline int32_t _adi_port_get_value(uint8_t port) {
	claim_port(INTERNAL_ADI_PORT, E_DEVICE_ADI);
	int32_t rtn = vexDeviceAdiValueGet(device->device_info, port);
	return_port(INTERNAL_ADI_PORT, rtn);
}

int32_t adi_port_set_config(uint8_t port, adi_port_config_e_t type) {
	transform_adi_port(port);
	return _adi_port_set_config(port, type);
}

adi_port_config_e_t adi_port_get_config(uint8_t port) {
	transform_adi_port(port);
	return _adi_port_get_config(port);
}

int32_t adi_port_set_value(uint8_t port, int32_t value) {
	transform_adi_port(port);
	return _adi_port_set_value(port, value);
}

int32_t adi_port_get_value(uint8_t port) {
	transform_adi_port(port);
	return _adi_port_get_value(port);
}

int32_t adi_analog_calibrate(uint8_t port) {
	transform_adi_port(port);
	validate_type(port, E_ADI_ANALOG_IN);
	uint32_t total = 0, i;
	for (i = 0; i < 512; i++) {
		total += _adi_port_get_value(port);
		task_delay(1);
	}
	set_analog_calib(port, (int32_t)((total + 16) >> 5));
	return ((int32_t)((total + 256) >> 9));
}

int32_t adi_analog_read(uint8_t port) {
	transform_adi_port(port);
	validate_type(port, E_ADI_ANALOG_IN);
	return _adi_port_get_value(port);
}

int32_t adi_analog_read_calibrated(uint8_t port) {
	transform_adi_port(port);
	validate_type(port, E_ADI_ANALOG_IN);
	return (_adi_port_get_value(port) - (get_analog_calib(port) >> 4));
}

int32_t adi_analog_read_calibrated_HR(uint8_t port) {
	transform_adi_port(port);
	validate_type(port, E_ADI_ANALOG_IN);
	return ((_adi_port_get_value(port) << 4) - get_analog_calib(port));
}

int32_t adi_digital_read(uint8_t port) {
	transform_adi_port(port);
	validate_type(port, E_ADI_DIGITAL_IN);
	return _adi_port_get_value(port);
}

int32_t adi_digital_get_new_press(uint8_t port) {
	transform_adi_port(port);
	int32_t pressed = _adi_port_get_value(port);

	if (!pressed)  // buttons is not currently pressed
		set_digital_pressed(port, false);

	if (pressed && !get_digital_pressed(port)) {
		// button is currently pressed and was not detected as being pressed during last check
		set_digital_pressed(port, true);
		return true;
	} else
		return false;  // button is not pressed or was already detected
}

int32_t adi_digital_write(uint8_t port, const bool value) {
	transform_adi_port(port);
	validate_type(port, E_ADI_DIGITAL_OUT);
	return _adi_port_set_value(port, (int32_t)value);
}

int32_t adi_pin_mode(uint8_t port, uint8_t mode) {
	switch (mode) {
		case INPUT:
			adi_port_set_config(port, E_ADI_DIGITAL_IN);
			break;
		case OUTPUT:
			adi_port_set_config(port, E_ADI_DIGITAL_OUT);
			break;
		case INPUT_ANALOG:
			adi_port_set_config(port, E_ADI_ANALOG_IN);
			break;
		case OUTPUT_ANALOG:
			adi_port_set_config(port, E_ADI_ANALOG_OUT);
			break;
		default:
			errno = EINVAL;
			return PROS_ERR;
	};
	return 1;
}

int32_t adi_motor_set(uint8_t port, int8_t speed) {
	transform_adi_port(port);
	validate_motor(port);
	if (speed > ADI_MOTOR_MAX_SPEED)
		speed = ADI_MOTOR_MAX_SPEED;
	else if (speed < ADI_MOTOR_MIN_SPEED)
		speed = ADI_MOTOR_MIN_SPEED;

	return _adi_port_set_value(port, speed);
}

int32_t adi_motor_get(uint8_t port) {
	transform_adi_port(port);
	validate_motor(port);
	return (_adi_port_get_value(port) - ADI_MOTOR_MAX_SPEED);
}

int32_t adi_motor_stop(uint8_t port) {
	validate_motor(port);
	return _adi_port_set_value(port, 0);
}

adi_encoder_t adi_encoder_init(uint8_t port_top, uint8_t port_bottom, const bool reverse) {
	transform_adi_port(port_top);
	transform_adi_port(port_bottom);
	validate_twowire(port_top, port_bottom);
	set_encoder_reversed(port, reverse);

	if (_adi_port_set_config(port, E_ADI_LEGACY_ENCODER)) {
		return port;
	} else {
		return PROS_ERR;
	}
}

int32_t adi_encoder_get(adi_encoder_t enc) {
	validate_type(enc, E_ADI_LEGACY_ENCODER);
	if (get_encoder_reversed(enc)) return (-_adi_port_get_value(enc));
	return _adi_port_get_value(enc);
}

int32_t adi_encoder_reset(adi_encoder_t enc) {
	validate_type(enc, E_ADI_LEGACY_ENCODER);
	return _adi_port_set_value(enc, 0);
}

int32_t adi_encoder_shutdown(adi_encoder_t enc) {
	validate_type(enc, E_ADI_LEGACY_ENCODER);
	return _adi_port_set_config(enc, E_ADI_TYPE_UNDEFINED);
}

adi_ultrasonic_t adi_ultrasonic_init(uint8_t port_ping, uint8_t port_echo) {
	transform_adi_port(port_ping);
	transform_adi_port(port_echo);
	validate_twowire(port_ping, port_echo);
	if (port != port_ping) return PROS_ERR;

	if (_adi_port_set_config(port, E_ADI_LEGACY_ULTRASONIC)) {
		return port;
	} else {
		return PROS_ERR;
	}
}

int32_t adi_ultrasonic_get(adi_ultrasonic_t ult) {
	validate_type(ult, E_ADI_LEGACY_ULTRASONIC);
	return _adi_port_get_value(ult);
}

int32_t adi_ultrasonic_shutdown(adi_ultrasonic_t ult) {
	validate_type(ult, E_ADI_LEGACY_ULTRASONIC);
	return _adi_port_set_config(ult, E_ADI_TYPE_UNDEFINED);
}

adi_gyro_t adi_gyro_init(uint8_t port, double multiplier) {
	transform_adi_port(port);
	if (multiplier == 0) multiplier = 1;
	set_gyro_multiplier(port, multiplier);
	set_gyro_tare(port, 0);

	adi_port_config_e_t config = _adi_port_get_config(port);
	if (config == E_ADI_LEGACY_GYRO) {
		// port has already been calibrated, no need to do that again
		return port;
	}

	int status = _adi_port_set_config(port, E_ADI_LEGACY_GYRO);
	if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)
		// If the scheduler is currently running (meaning that this is not called
		// from a global constructor, for example) then delay for the duration of
		// the calibration time in VexOS.
		delay(GYRO_CALIBRATION_TIME);

	if (status)
		return port;
	else
		return PROS_ERR;
}

double adi_gyro_get(adi_gyro_t gyro) {
	validate_type_f(gyro, E_ADI_LEGACY_GYRO);
	double rtn = (double)_adi_port_get_value(gyro);
	rtn -= get_gyro_tare(gyro);
	rtn *= get_gyro_multiplier(gyro);
	return rtn;
}

int32_t adi_gyro_reset(adi_gyro_t gyro) {
	validate_type(gyro, E_ADI_LEGACY_GYRO);
	set_gyro_tare(gyro, _adi_port_get_value(gyro));
	return 1;
}

int32_t adi_gyro_shutdown(adi_gyro_t gyro) {
	validate_type(gyro, E_ADI_LEGACY_GYRO);
	return _adi_port_set_config(gyro, E_ADI_TYPE_UNDEFINED);
}
