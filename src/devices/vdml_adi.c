/**
 * \file vdml_adi.c
 *
 * \brief VDML ADI functionality.
 *
 * This file ensure thread saftey for operations on motors by maintaining
 * an array of RTOS Mutexes and implementing functions to take and give them.
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
#include "vdml/registry.h"
#include "vdml/vdml.h"

#define INTERNAL_ADI_PORT 21

#define ADI_MOTOR_MAX_SPEED 127
#define ADI_MOTOR_MIN_SPEED -128

#define NUM_MAX_TWOWIRE 4

typedef union adi_data {
	struct {
		int32_t calib;
	} analog_data;
	struct {
		bool was_pressed;
	} digital_data;
} adi_data_s_t;

adi_data_s_t adi_data[NUM_ADI_PORTS];

bool encoder_reversed[NUM_MAX_TWOWIRE];

#define transform_adi_port(port)                                                                                       \
	if (port >= 'a' && port <= 'h')                                                                                \
		port -= 'a';                                                                                           \
	else if (port >= 'A' && port <= 'H')                                                                           \
		port -= 'A';                                                                                           \
	else                                                                                                           \
		port--;                                                                                                \
	if (port > 7 || port < 0) {                                                                                    \
		errno = EINVAL;                                                                                        \
		return PROS_ERR;                                                                                       \
	}

#define validate_type(port, type)                                                                                      \
	adi_port_config_e_t config = _adi_port_config_get(port);                                                       \
	if (config != type) {                                                                                          \
		return PROS_ERR;                                                                                       \
	}

#define validate_analog(port)                                                                                          \
	adi_port_config_e_t config = _adi_port_config_get(port);                                                       \
	if (config != E_ADI_ANALOG_IN && config != E_ADI_LEGACY_POT && config != E_ADI_LEGACY_LINE_SENSOR &&           \
	    config != E_ADI_LEGACY_LIGHT_SENSOR && config != E_ADI_LEGACY_ACCELEROMETER &&                             \
	    config != E_ADI_SMART_POT) {                                                                               \
		errno = EINVAL;                                                                                        \
		return PROS_ERR;                                                                                       \
	}

#define validate_digital_in(port)                                                                                      \
	adi_port_config_e_t config = _adi_port_config_get(port);                                                       \
	if (config != E_ADI_DIGITAL_IN && config != E_ADI_LEGACY_BUTTON && config != E_ADI_SMART_BUTTON) {             \
		errno = EINVAL;                                                                                        \
		return PROS_ERR;                                                                                       \
	}

#define validate_motor(port)                                                                                           \
	adi_port_config_e_t config = _adi_port_config_get(port);                                                       \
	if (config != E_ADI_LEGACY_PWM && config != E_ADI_LEGACY_SERVO) {                                              \
		errno = EINVAL;                                                                                        \
		return PROS_ERR;                                                                                       \
	}

#define validate_twowire(port_top, port_bottom)                                                                        \
	if (abs(port_top - port_bottom) > 1) {                                                                         \
		errno = EINVAL;                                                                                        \
		return PROS_ERR;                                                                                       \
	}                                                                                                              \
	int port;                                                                                                      \
	if (port_top < port_bottom)                                                                                    \
		port = port_top;                                                                                       \
	else if (port_bottom < port_top)                                                                               \
		port = port_bottom;                                                                                    \
	else                                                                                                           \
		return PROS_ERR;                                                                                       \
	if (port % 2 == 1) {                                                                                           \
		return PROS_ERR;                                                                                       \
	}

static inline int32_t _adi_port_config_set(uint8_t port, adi_port_config_e_t type) {
	claim_port(INTERNAL_ADI_PORT, E_DEVICE_ADI);
	vexDeviceAdiPortConfigSet(device.device_info, port, type);
	return_port(INTERNAL_ADI_PORT, 1);
}

static inline adi_port_config_e_t _adi_port_config_get(uint8_t port) {
	claim_port(INTERNAL_ADI_PORT, E_DEVICE_ADI);
	adi_port_config_e_t rtn = (adi_port_config_e_t)vexDeviceAdiPortConfigGet(device.device_info, port);
	return_port(INTERNAL_ADI_PORT, rtn);
}

static inline int32_t _adi_value_set(uint8_t port, int32_t value) {
	claim_port(INTERNAL_ADI_PORT, E_DEVICE_ADI);
	vexDeviceAdiValueSet(device.device_info, port, value);
	return_port(INTERNAL_ADI_PORT, 1);
}

static inline int32_t _adi_value_get(uint8_t port) {
	claim_port(INTERNAL_ADI_PORT, E_DEVICE_ADI);
	int32_t rtn = vexDeviceAdiValueGet(device.device_info, port);
	return_port(INTERNAL_ADI_PORT, rtn);
}

int32_t adi_port_config_set(uint8_t port, adi_port_config_e_t type) {
	transform_adi_port(port);
	return _adi_port_config_set(port, type);
}

adi_port_config_e_t adi_port_config_get(uint8_t port) {
	transform_adi_port(port);
	return _adi_port_config_get(port);
}

int32_t adi_value_set(uint8_t port, int32_t value) {
	transform_adi_port(port);
	return _adi_value_set(port, value);
}

int32_t adi_value_get(uint8_t port) {
	transform_adi_port(port);
	return _adi_value_get(port);
}

int32_t adi_analog_calibrate(uint8_t port) {
	transform_adi_port(port);
	validate_analog(port);
	uint32_t total = 0, i;
	for (i = 0; i < 512; i++) {
		total += _adi_value_get(port);
		task_delay(1);
	}
	adi_data[port - 1].analog_data.calib = (int32_t)((total + 16) >> 5);
	return ((int32_t)((total + 256) >> 9));
}

int32_t adi_analog_read(uint8_t port) {
	transform_adi_port(port);
	validate_analog(port);
	return _adi_value_get(port);
}

int32_t adi_analog_read_calibrated(uint8_t port) {
	transform_adi_port(port);
	validate_analog(port);
	return (_adi_value_get(port) - (adi_data[port - 1].analog_data.calib >> 4));
}

int32_t adi_analog_read_calibrated_HR(uint8_t port) {
	transform_adi_port(port);
	validate_analog(port);
	return ((_adi_value_get(port) << 4) - adi_data[port - 1].analog_data.calib);
}

int32_t adi_digital_read(uint8_t port) {
	transform_adi_port(port);
	validate_digital_in(port);
	return _adi_value_get(port);
}

int32_t digital_get_new_press(uint8_t port) {
	transform_adi_port(port);
	int32_t pressed = _adi_value_get(port);

	if (!pressed)  // buttons is not currently pressed
		adi_data[port].digital_data.was_pressed = false;

	if (pressed && !adi_data[port].digital_data.was_pressed) {
		// button is currently pressed and was not detected as being pressed during last check
		adi_data[port].digital_data.was_pressed = true;
		return true;
	} else
		return false;  // button is not pressed or was already detected
}

int32_t adi_digital_write(uint8_t port, const bool value) {
	transform_adi_port(port);
	validate_type(port, E_ADI_DIGITAL_OUT);
	return _adi_value_set(port, (int32_t)value);
}

int32_t adi_pin_mode(uint8_t port, const uint8_t mode) {
	switch (mode) {
	case INPUT:
		adi_port_config_set(port, E_ADI_DIGITAL_IN);
		break;
	case OUTPUT:
		adi_port_config_set(port, E_ADI_DIGITAL_OUT);
		break;
	case INPUT_ANALOG:
		adi_port_config_set(port, E_ADI_ANALOG_IN);
		break;
	case OUTPUT_ANALOG:
		adi_port_config_set(port, E_ADI_ANALOG_OUT);
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

	return _adi_value_set(port, speed);
}

int32_t adi_motor_get(uint8_t port) {
	transform_adi_port(port);
	validate_motor(port);
	return (_adi_value_get(port) - ADI_MOTOR_MAX_SPEED);
}

int32_t adi_motor_stop(uint8_t port) {
	validate_motor(port);
	return _adi_value_set(port, 0);
}

adi_encoder_t adi_encoder_init(uint8_t port_top, uint8_t port_bottom, const bool reverse) {
	transform_adi_port(port_top);
	transform_adi_port(port_bottom);
	validate_twowire(port_top, port_bottom);
	encoder_reversed[(port - 1) / 2] = reverse;

	if (_adi_port_config_set(port, E_ADI_LEGACY_ENCODER)) {
		return port;
	} else {
		return PROS_ERR;
	}
}

int32_t adi_encoder_get(adi_encoder_t enc) {
	validate_type(enc, E_ADI_LEGACY_ENCODER);
	if (encoder_reversed[(enc - 1) / 2]) return (-_adi_value_get(enc));
	return _adi_value_get(enc);
}

int32_t adi_encoder_reset(adi_encoder_t enc) {
	validate_type(enc, E_ADI_LEGACY_ENCODER);
	return _adi_value_set(enc, 0);
}

int32_t adi_encoder_shutdown(adi_encoder_t enc) {
	validate_type(enc, E_ADI_LEGACY_ENCODER);
	return _adi_port_config_set(enc, E_ADI_TYPE_UNDEFINED);
}

adi_ultrasonic_t adi_ultrasonic_init(uint8_t port_echo, uint8_t port_ping) {
	transform_adi_port(port_echo);
	transform_adi_port(port_ping);
	validate_twowire(port_echo, port_ping);
	if (port != port_echo) return PROS_ERR;

	if (_adi_port_config_set(port, E_ADI_LEGACY_ULTRASONIC)) {
		return port;
	} else {
		return PROS_ERR;
	}
}

int32_t adi_ultrasonic_get(adi_ultrasonic_t ult) {
	validate_type(ult, E_ADI_LEGACY_ULTRASONIC);
	return _adi_value_get(ult);
}

int32_t adi_ultrasonic_shutdown(adi_ultrasonic_t ult) {
	validate_type(ult, E_ADI_LEGACY_ULTRASONIC);
	return _adi_port_config_set(ult, E_ADI_TYPE_UNDEFINED);
}
