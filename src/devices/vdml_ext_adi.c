/**
 * \file devices/vdml_ext_adi.c
 *
 * Contains functions for interacting with the V5 3-Wire Expander.
 *
 * Copyright (c) 2017-2020, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <errno.h>
#include <math.h>
#include <stdio.h>

#include "kapi.h"
#include "pros/adi.h"
#include "pros/ext_adi.h"
#include "v5_api.h"
#include "vdml/port.h"
#include "vdml/registry.h"
#include "vdml/vdml.h"

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

#define transform_adi_port(port)       \
	if (port >= 'a' && port <= 'h')      \
		port -= 'a';                       \
	else if (port >= 'A' && port <= 'H') \
		port -= 'A';                       \
	else                                 \
		port--;                            \
	if (port > 7 || port < 0) {          \
		errno = ENXIO;                     \
		return PROS_ERR;                   \
	}

#define validate_type(device, port, type)                                                                 \
	adi_port_config_e_t config = (adi_port_config_e_t)vexDeviceAdiPortConfigGet(device->device_info, port); \
	if (config != type) {                                                                                   \
		errno = EADDRINUSE;                                                                                   \
		return PROS_ERR;                                                                                      \
	}

#define validate_motor(device, port)                                                                      \
	adi_port_config_e_t config = (adi_port_config_e_t)vexDeviceAdiPortConfigGet(device->device_info, port); \
	if (config != E_ADI_LEGACY_PWM && config != E_ADI_LEGACY_SERVO) {                                       \
		errno = EADDRINUSE;                                                                                   \
		return PROS_ERR;                                                                                      \
	}

/*
 * Validates 3 things:
 * - Ports next to each other
 * - Both ports are initialized and valid (individually)
 *
 * Returns PROS_ERR if one of these is false.
 */
#define validate_twowire(port_top, port_bottom) \
	if (abs(port_top - port_bottom) > 1) {        \
		errno = ENXIO;                              \
		return PROS_ERR;                            \
	}                                             \
	int port;                                     \
	if (port_top < port_bottom)                   \
		port = port_top;                            \
	else if (port_bottom < port_top)              \
		port = port_bottom;                         \
	else {                                        \
		errno = EINVAL;                             \
		return PROS_ERR;                            \
	}                                             \
	if (port % 2 == 1) {                          \
		errno = EINVAL;                             \
		return PROS_ERR;                            \
	}

adi_port_config_e_t ext_adi_port_get_config(uint8_t smart_port, uint8_t adi_port) {
	transform_adi_port(adi_port);
	claim_port_i(smart_port - 1, E_DEVICE_ADI);
	adi_port_config_e_t rtn = (adi_port_config_e_t)vexDeviceAdiPortConfigGet(device->device_info, adi_port);
	return_port(smart_port - 1, rtn);
}

int32_t ext_adi_port_get_value(uint8_t smart_port, uint8_t adi_port) {
	transform_adi_port(adi_port);
	claim_port_i(smart_port - 1, E_DEVICE_ADI);
	int32_t rtn = vexDeviceAdiValueGet(device->device_info, adi_port);
	return_port(smart_port - 1, rtn);
}

int32_t ext_adi_port_set_config(uint8_t smart_port, uint8_t adi_port, adi_port_config_e_t type) {
	transform_adi_port(adi_port);
	claim_port_i(smart_port - 1, E_DEVICE_ADI);
	vexDeviceAdiPortConfigSet(device->device_info, adi_port, (V5_AdiPortConfiguration)type);
	return_port(smart_port - 1, 1);
}

int32_t ext_adi_port_set_value(uint8_t smart_port, uint8_t adi_port, int32_t value) {
	transform_adi_port(adi_port);
	claim_port_i(smart_port - 1, E_DEVICE_ADI);
	vexDeviceAdiValueSet(device->device_info, adi_port, value);
	return_port(smart_port - 1, 1);
}

int32_t ext_adi_analog_calibrate(uint8_t smart_port, uint8_t adi_port) {
	transform_adi_port(adi_port);
	claim_port_i(smart_port - 1, E_DEVICE_ADI);
	validate_type(device, adi_port, E_ADI_ANALOG_IN);
	uint32_t total = 0;
	for (uint32_t i = 0; i < 512; i++) {
		total += vexDeviceAdiValueGet(device->device_info, adi_port);
		task_delay(1);  // TODO: If smart ports (and the ADI) only update every 10ms, this really only reads 56 samples,
		                // maybe change to a 10ms
	}
	adi_data_s_t* const adi_data = &((adi_data_s_t*)(device->pad))[adi_port];
	adi_data->analog_data.calib = (int32_t)((total + 16) >> 5);
	return_port(smart_port - 1, (int32_t)((total + 256) >> 9));
}

int32_t ext_adi_analog_read(uint8_t smart_port, uint8_t adi_port) {
	transform_adi_port(adi_port);
	claim_port_i(smart_port - 1, E_DEVICE_ADI);
	validate_type(device, adi_port, E_ADI_ANALOG_IN);
	int32_t rtn = vexDeviceAdiValueGet(device->device_info, adi_port);
	return_port(smart_port - 1, rtn);
}

int32_t ext_adi_analog_read_calibrated(uint8_t smart_port, uint8_t adi_port) {
	transform_adi_port(adi_port);
	claim_port_i(smart_port - 1, E_DEVICE_ADI);
	validate_type(device, adi_port, E_ADI_ANALOG_IN);
	adi_data_s_t* const adi_data = &((adi_data_s_t*)(device->pad))[adi_port];
	int32_t rtn = vexDeviceAdiValueGet(device->device_info, adi_port) - (adi_data->analog_data.calib >> 4);
	return_port(smart_port - 1, rtn);
}

int32_t ext_adi_analog_read_calibrated_HR(uint8_t smart_port, uint8_t adi_port) {
	transform_adi_port(adi_port);
	claim_port_i(smart_port - 1, E_DEVICE_ADI);
	validate_type(device, adi_port, E_ADI_ANALOG_IN);
	adi_data_s_t* const adi_data = &((adi_data_s_t*)(device->pad))[adi_port];
	int32_t rtn = (vexDeviceAdiValueGet(device->device_info, adi_port) << 4) - adi_data->analog_data.calib;
	return_port(smart_port - 1, rtn);
}

int32_t ext_adi_digital_read(uint8_t smart_port, uint8_t adi_port) {
	transform_adi_port(adi_port);
	claim_port_i(smart_port - 1, E_DEVICE_ADI);
	validate_type(device, adi_port, E_ADI_DIGITAL_IN);
	int32_t rtn = vexDeviceAdiValueGet(device->device_info, adi_port);
	return_port(smart_port - 1, rtn);
}

int32_t ext_adi_digital_get_new_press(uint8_t smart_port, uint8_t adi_port) {
	transform_adi_port(adi_port);
	claim_port_i(smart_port - 1, E_DEVICE_ADI);
	validate_type(device, adi_port, E_ADI_DIGITAL_IN);

	adi_data_s_t* const adi_data = &((adi_data_s_t*)(device->pad))[adi_port];

	int32_t pressed = vexDeviceAdiValueGet(device->device_info, adi_port);

	if (!pressed)
		adi_data->digital_data.was_pressed = false;
	else if (!adi_data->digital_data.was_pressed) {
		// Button is currently pressed and was not detected as being pressed during last check
		adi_data->digital_data.was_pressed = true;
		return_port(smart_port - 1, true);
	}

	return_port(smart_port - 1, false);
}

int32_t ext_adi_digital_write(uint8_t smart_port, uint8_t adi_port, bool value) {
	transform_adi_port(adi_port);
	claim_port_i(smart_port - 1, E_DEVICE_ADI);
	validate_type(device, adi_port, E_ADI_DIGITAL_OUT);
	vexDeviceAdiValueSet(device->device_info, adi_port, (int32_t)value);
	return_port(smart_port - 1, 1);
}

int32_t ext_adi_pin_mode(uint8_t smart_port, uint8_t adi_port, uint8_t mode) {
	switch (mode) {
		case INPUT:
			ext_adi_port_set_config(smart_port, adi_port, E_ADI_DIGITAL_IN);
			break;
		case OUTPUT:
			ext_adi_port_set_config(smart_port, adi_port, E_ADI_DIGITAL_OUT);
			break;
		case INPUT_ANALOG:
			ext_adi_port_set_config(smart_port, adi_port, E_ADI_ANALOG_IN);
			break;
		case OUTPUT_ANALOG:
			ext_adi_port_set_config(smart_port, adi_port, E_ADI_ANALOG_OUT);
			break;
		default:
			errno = EINVAL;
			return PROS_ERR;
	};
	return 1;
}

int32_t ext_adi_motor_set(uint8_t smart_port, uint8_t adi_port, int8_t speed) {
	transform_adi_port(adi_port);
	claim_port_i(smart_port - 1, E_DEVICE_ADI);
	validate_motor(device, adi_port);
	if (speed > ADI_MOTOR_MAX_SPEED)
		speed = ADI_MOTOR_MAX_SPEED;
	else if (speed < ADI_MOTOR_MIN_SPEED)
		speed = ADI_MOTOR_MIN_SPEED;
	vexDeviceAdiValueSet(device->device_info, adi_port, speed);
	return_port(smart_port - 1, 1);
}

int32_t ext_adi_motor_get(uint8_t smart_port, uint8_t adi_port) {
	transform_adi_port(adi_port);
	claim_port_i(smart_port - 1, E_DEVICE_ADI);
	validate_motor(device, adi_port);
	int32_t rtn = vexDeviceAdiValueGet(device->device_info, adi_port) - ADI_MOTOR_MAX_SPEED;
	return_port(smart_port - 1, rtn);
}

int32_t ext_adi_motor_stop(uint8_t smart_port, uint8_t adi_port) {
	return ext_adi_motor_set(smart_port, adi_port, 0);
}

ext_adi_encoder_t ext_adi_encoder_init(uint8_t smart_port, uint8_t adi_port_top, uint8_t adi_port_bottom,
                                       bool reverse) {
	transform_adi_port(adi_port_top);
	transform_adi_port(adi_port_bottom);
	claim_port_i(smart_port - 1, E_DEVICE_ADI);
	validate_twowire(adi_port_top, adi_port_bottom);

	adi_data_s_t* const adi_data = &((adi_data_s_t*)(device->pad))[port];
	adi_data->encoder_data.reversed = reverse;
	vexDeviceAdiPortConfigSet(device->device_info, port, E_ADI_LEGACY_ENCODER);
	return_port(smart_port - 1, merge_adi_ports(smart_port - 1, port + 1));
}

int32_t ext_adi_encoder_get(ext_adi_encoder_t enc) {
	uint8_t smart_port, adi_port;
	get_ports(enc, smart_port, adi_port);
	transform_adi_port(adi_port);
	claim_port_i(smart_port, E_DEVICE_ADI);
	validate_type(device, adi_port, E_ADI_LEGACY_ENCODER);

	int32_t rtn;
	adi_data_s_t* const adi_data = &((adi_data_s_t*)(device->pad))[adi_port];
	if (adi_data->encoder_data.reversed)
		rtn = -vexDeviceAdiValueGet(device->device_info, adi_port);
	else
		rtn = vexDeviceAdiValueGet(device->device_info, adi_port);
	return_port(smart_port, rtn);
}

int32_t ext_adi_encoder_reset(ext_adi_encoder_t enc) {
	uint8_t smart_port, adi_port;
	get_ports(enc, smart_port, adi_port);
	transform_adi_port(adi_port);
	claim_port_i(smart_port, E_DEVICE_ADI);
	validate_type(device, adi_port, E_ADI_LEGACY_ENCODER);

	vexDeviceAdiValueSet(device->device_info, adi_port, 0);
	return_port(smart_port, 1);
}

int32_t ext_adi_encoder_shutdown(ext_adi_encoder_t enc) {
	uint8_t smart_port, adi_port;
	get_ports(enc, smart_port, adi_port);
	transform_adi_port(adi_port);
	claim_port_i(smart_port, E_DEVICE_ADI);
	validate_type(device, adi_port, E_ADI_LEGACY_ENCODER);

	vexDeviceAdiPortConfigSet(device->device_info, adi_port, E_ADI_TYPE_UNDEFINED);
	return_port(smart_port, 1);
}

ext_adi_ultrasonic_t ext_adi_ultrasonic_init(uint8_t smart_port, uint8_t adi_port_ping, uint8_t adi_port_echo) {
	transform_adi_port(adi_port_ping);
	transform_adi_port(adi_port_echo);
	validate_twowire(adi_port_ping, adi_port_echo);
	if (port != adi_port_ping) {
		errno = EINVAL;
		return PROS_ERR;
	}

	claim_port_i(smart_port - 1, E_DEVICE_ADI);
	vexDeviceAdiPortConfigSet(device->device_info, port, E_ADI_LEGACY_ULTRASONIC);
	return_port(smart_port - 1, merge_adi_ports(smart_port - 1, port + 1));
}

int32_t ext_adi_ultrasonic_get(ext_adi_ultrasonic_t ult) {
	uint8_t smart_port, adi_port;
	get_ports(ult, smart_port, adi_port);
	transform_adi_port(adi_port);
	claim_port_i(smart_port, E_DEVICE_ADI);
	validate_type(device, adi_port, E_ADI_LEGACY_ULTRASONIC);

	int32_t rtn = vexDeviceAdiValueGet(device->device_info, adi_port);
	return_port(smart_port, rtn);
}

int32_t ext_adi_ultrasonic_shutdown(ext_adi_ultrasonic_t ult) {
	uint8_t smart_port, adi_port;
	get_ports(ult, smart_port, adi_port);
	transform_adi_port(adi_port);
	claim_port_i(smart_port, E_DEVICE_ADI);
	validate_type(device, adi_port, E_ADI_LEGACY_ULTRASONIC);

	vexDeviceAdiPortConfigSet(device->device_info, adi_port, E_ADI_TYPE_UNDEFINED);
	return_port(smart_port, 1);
}

ext_adi_gyro_t ext_adi_gyro_init(uint8_t smart_port, uint8_t adi_port, double multiplier) {
	transform_adi_port(adi_port);
	claim_port_i(smart_port - 1, E_DEVICE_ADI);

	if (multiplier == 0) multiplier = 1;
	adi_data_s_t* const adi_data = &((adi_data_s_t*)(device->pad))[adi_port];
	adi_data->gyro_data.multiplier = multiplier;
	adi_data->gyro_data.tare_value = 0;

	adi_port_config_e_t config = vexDeviceAdiPortConfigGet(device->device_info, adi_port);
	if (config == E_ADI_LEGACY_GYRO) {
		// Port has already been calibrated, no need to do that again
		return_port(smart_port - 1, merge_adi_ports(smart_port - 1, adi_port + 1));
	}

	vexDeviceAdiPortConfigSet(device->device_info, adi_port, E_ADI_LEGACY_GYRO);
	if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING) {
		// If the scheduler is currently running (meaning that this is not called
		// from a global constructor, for example) then delay for the duration of
		// the calibration time in VexOS.
		delay(GYRO_CALIBRATION_TIME);
	}
	return_port(smart_port - 1, merge_adi_ports(smart_port - 1, adi_port + 1));
}

// Internal wrapper for adi_gyro_get to get around transform_adi_port, claim_port_i, validate_type and return_port
// possibly returning PROS_ERR, not PROS_ERR_F
int32_t _ext_adi_gyro_get(ext_adi_gyro_t gyro, double* out) {
	uint8_t smart_port, adi_port;
	get_ports(gyro, smart_port, adi_port);
	transform_adi_port(adi_port);
	claim_port_i(smart_port, E_DEVICE_ADI);
	validate_type(device, adi_port, E_ADI_LEGACY_GYRO);

	double rtn = (double)vexDeviceAdiValueGet(device->device_info, adi_port);
	adi_data_s_t* const adi_data = &((adi_data_s_t*)(device->pad))[adi_port];
	rtn -= adi_data->gyro_data.tare_value;
	rtn *= adi_data->gyro_data.multiplier;
	*out = rtn;
	return_port(smart_port, 1);
}

double ext_adi_gyro_get(ext_adi_gyro_t gyro) {
	double rtn;
	if (_ext_adi_gyro_get(gyro, &rtn) == PROS_ERR)
		return PROS_ERR_F;
	else
		return rtn;
}

int32_t ext_adi_gyro_reset(ext_adi_gyro_t gyro) {
	uint8_t smart_port, adi_port;
	get_ports(gyro, smart_port, adi_port);
	transform_adi_port(adi_port);
	claim_port_i(smart_port, E_DEVICE_ADI);
	validate_type(device, adi_port, E_ADI_LEGACY_GYRO);

	adi_data_s_t* const adi_data = &((adi_data_s_t*)(device->pad))[adi_port];
	adi_data->gyro_data.tare_value = vexDeviceAdiValueGet(device->device_info, adi_port);
	return_port(smart_port, 1);
}

int32_t ext_adi_gyro_shutdown(ext_adi_gyro_t gyro) {
	uint8_t smart_port, adi_port;
	get_ports(gyro, smart_port, adi_port);
	transform_adi_port(adi_port);
	claim_port_i(smart_port, E_DEVICE_ADI);
	validate_type(device, adi_port, E_ADI_LEGACY_GYRO);

	vexDeviceAdiPortConfigSet(device->device_info, adi_port, E_ADI_TYPE_UNDEFINED);
	return_port(smart_port, 1);
}
