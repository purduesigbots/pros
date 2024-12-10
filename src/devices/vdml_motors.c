/**
 * \file devices/vdml_motors.c
 *
 * Contains functions for interacting with the V5 Motors.
 *
 * \copyright Copyright (c) 2017-2024, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <errno.h>
#include <math.h>
#include <stdio.h>

#include "kapi.h"
#include "pros/colors.h"
#include "pros/motors.h"
#include "pros/apix.h"
#include "v5_api.h"
#include "vdml/registry.h"
#include "vdml/vdml.h"

#define MOTOR_MOVE_RANGE 127
#define MOTOR_VOLTAGE_RANGE 12000

// Movement functions

int32_t motor_move(int8_t port, int32_t voltage) {
	if (voltage > 127) {
		voltage = 127;
	} else if (voltage < -127) {
		voltage = -127;
	}

	if (port < 0) {
		voltage = -voltage;
	}

	port = abs(port);
	// Remap the input voltage range to the motor voltage
	// scale to [-127, 127] -> [-12000, 12000]
	int32_t command = (((voltage + MOTOR_MOVE_RANGE) * (MOTOR_VOLTAGE_RANGE)) / (MOTOR_MOVE_RANGE));
	command -= MOTOR_VOLTAGE_RANGE;
	return motor_move_voltage(port, command);
}

int32_t motor_brake(int8_t port) {
	return motor_move_velocity(port, 0);
}

int32_t motor_move_absolute(int8_t port, double position, int32_t velocity) {
	uint8_t abs_port = abs(port);
	claim_port_i(abs_port - 1, E_DEVICE_MOTOR);
	if (port < 0) position = -position;
	vexDeviceMotorAbsoluteTargetSet(device->device_info, position, velocity);
	return_port(abs_port - 1, PROS_SUCCESS);
}

int32_t motor_move_relative(int8_t port, double position, int32_t velocity) {
	uint8_t abs_port = abs(port);
	claim_port_i(abs_port - 1, E_DEVICE_MOTOR);
	if (port < 0) position = -position;
	vexDeviceMotorRelativeTargetSet(device->device_info, position, velocity);
	return_port(abs_port - 1, PROS_SUCCESS);
}

int32_t motor_move_velocity(int8_t port, int32_t velocity) {
	uint8_t abs_port = abs(port);
	claim_port_i(abs_port - 1, E_DEVICE_MOTOR);
	if (port < 0) velocity = -velocity;
	vexDeviceMotorVelocitySet(device->device_info, velocity);
	return_port(abs_port - 1, PROS_SUCCESS);
}

int32_t motor_move_voltage(int8_t port, int32_t voltage) {
	uint8_t abs_port = abs(port);
	claim_port_i(abs_port - 1, E_DEVICE_MOTOR);
	if (port < 0) voltage = -voltage;
	vexDeviceMotorVoltageSet(device->device_info, voltage);
	return_port(abs_port - 1, PROS_SUCCESS);
}

int32_t motor_modify_profiled_velocity(int8_t port, int32_t velocity) {
	uint8_t abs_port = abs(port);
	claim_port_i(abs_port - 1, E_DEVICE_MOTOR);
	if (port < 0) velocity = -velocity;
	vexDeviceMotorVelocityUpdate(device->device_info, velocity);
	return_port(abs_port - 1, PROS_SUCCESS);
}

double motor_get_target_position(int8_t port) {
	uint8_t abs_port = abs(port);
	claim_port_f(abs_port - 1, E_DEVICE_MOTOR);
	double rtn = vexDeviceMotorTargetGet(device->device_info);
	if (port < 0) rtn = -rtn;
	return_port(abs_port - 1, rtn);
}

int32_t motor_get_target_velocity(int8_t port) {
	uint8_t abs_port = abs(port);
	claim_port_i(abs_port - 1, E_DEVICE_MOTOR);
	int32_t rtn = vexDeviceMotorVelocityGet(device->device_info);
	if (port < 0) rtn = -rtn;
	return_port(abs_port - 1, rtn);
}

// Telemetry functions

double motor_get_actual_velocity(int8_t port) {
	uint8_t abs_port = abs(port);
	claim_port_f(abs_port - 1, E_DEVICE_MOTOR);
	double rtn = vexDeviceMotorActualVelocityGet(device->device_info);
	if (port < 0) rtn = -rtn;
	return_port(abs_port - 1, rtn);
}

int32_t motor_get_current_draw(int8_t port) {
	port = abs(port);
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	int32_t rtn = vexDeviceMotorCurrentGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_get_direction(int8_t port) {
	uint8_t abs_port = abs(port);
	claim_port_i(abs_port - 1, E_DEVICE_MOTOR);
	int32_t rtn = vexDeviceMotorDirectionGet(device->device_info);
	if (port < 0) rtn = -rtn;
	return_port(abs_port - 1, rtn);
}

double motor_get_efficiency(int8_t port) {
	port = abs(port);
	claim_port_f(port - 1, E_DEVICE_MOTOR);
	double rtn = vexDeviceMotorEfficiencyGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_is_over_current(int8_t port) {
	port = abs(port);
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	int rtn = vexDeviceMotorCurrentLimitFlagGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_is_over_temp(int8_t port) {
	port = abs(port);
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	int rtn = vexDeviceMotorOverTempFlagGet(device->device_info);
	return_port(port - 1, rtn);
}

uint32_t motor_get_faults(int8_t port) {
	port = abs(port);
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	uint32_t rtn = vexDeviceMotorFaultsGet(device->device_info);
	return_port(port - 1, rtn);
}

uint32_t motor_get_flags(int8_t port) {
	port = abs(port);
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	uint32_t rtn = vexDeviceMotorFlagsGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_get_raw_position(int8_t port, uint32_t* const timestamp) {
	uint8_t abs_port = abs(port);
	claim_port_i(abs_port - 1, E_DEVICE_MOTOR);
	int32_t rtn = vexDeviceMotorPositionRawGet(device->device_info, timestamp);
	if (port < 0) rtn = -rtn;
	return_port(abs_port - 1, rtn);
}

double motor_get_position(int8_t port) {
	uint8_t abs_port = abs(port);
	claim_port_f(abs_port - 1, E_DEVICE_MOTOR);
	double rtn = vexDeviceMotorPositionGet(device->device_info);
	if (port < 0) rtn = -rtn;
	return_port(abs_port - 1, rtn);
}

double motor_get_power(int8_t port) {
	port = abs(port);
	claim_port_f(port - 1, E_DEVICE_MOTOR);
	double rtn = vexDeviceMotorPowerGet(device->device_info);
	return_port(port - 1, rtn);
}

double motor_get_temperature(int8_t port) {
	port = abs(port);
	claim_port_f(port - 1, E_DEVICE_MOTOR);
	double rtn = vexDeviceMotorTemperatureGet(device->device_info);
	return_port(port - 1, rtn);
}

double motor_get_torque(int8_t port) {
	port = abs(port);
	claim_port_f(port - 1, E_DEVICE_MOTOR);
	double rtn = vexDeviceMotorTorqueGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_get_voltage(int8_t port) {
	uint8_t abs_port = abs(port);
	claim_port_i(abs_port - 1, E_DEVICE_MOTOR);
	int32_t rtn = vexDeviceMotorVoltageGet(device->device_info);
	if (port < 0) rtn = -rtn;
	return_port(abs_port - 1, rtn);
}

// Config functions

int32_t motor_set_zero_position(int8_t port, const double position) {
	port = abs(port);
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorPositionSet(device->device_info, position);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t motor_tare_position(int8_t port) {
	port = abs(port);
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorPositionReset(device->device_info);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t motor_set_brake_mode(int8_t port, const motor_brake_mode_e_t mode) {
	port = abs(port);
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorBrakeModeSet(device->device_info, (V5MotorBrakeMode)mode);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t motor_set_current_limit(int8_t port, const int32_t limit) {
	port = abs(port);
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorCurrentLimitSet(device->device_info, limit);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t motor_set_encoder_units(int8_t port, const motor_encoder_units_e_t units) {
	port = abs(port);
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorEncoderUnitsSet(device->device_info, (V5MotorEncoderUnits)units);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t motor_set_gearing(int8_t port, const motor_gearset_e_t gearset) {
	port = abs(port);
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorGearingSet(device->device_info, (V5MotorGearset)gearset);
  return_port(port - 1, PROS_SUCCESS);
}

int32_t motor_set_reversed(int8_t port, const bool reverse) {
	port = abs(port);
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorReverseFlagSet(device->device_info, reverse);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t motor_set_voltage_limit(int8_t port, const int32_t limit) {
	port = abs(port);
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorVoltageLimitSet(device->device_info, limit);
	return_port(port - 1, PROS_SUCCESS);
}

motor_brake_mode_e_t motor_get_brake_mode(int8_t port) {
	port = abs(port);
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	V5MotorBrakeMode rtn = vexDeviceMotorBrakeModeGet(device->device_info);
	return_port(port - 1, (motor_brake_mode_e_t)rtn);
}

int32_t motor_get_current_limit(int8_t port) {
	port = abs(port);
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	int32_t rtn = vexDeviceMotorCurrentLimitGet(device->device_info);
	return_port(port - 1, rtn);
}

motor_encoder_units_e_t motor_get_encoder_units(int8_t port) {
	port = abs(port);
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	V5MotorEncoderUnits rtn = vexDeviceMotorEncoderUnitsGet(device->device_info);
	return_port(port - 1, (motor_encoder_units_e_t)rtn);
}

motor_gearset_e_t motor_get_gearing(int8_t port) {
	port = abs(port);
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	V5MotorGearset rtn = vexDeviceMotorGearingGet(device->device_info);
	return_port(port - 1, (motor_gearset_e_t)rtn);
}

int32_t motor_is_reversed(int8_t port) {
	port = abs(port);
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	int rtn = vexDeviceMotorReverseFlagGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_get_voltage_limit(int8_t port) {
	port = abs(port);
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	int32_t rtn = vexDeviceMotorVoltageLimitGet(device->device_info);
	return_port(port - 1, rtn);
}
