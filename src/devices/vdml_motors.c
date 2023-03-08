/**
 * \file devices/vdml_motors.c
 *
 * Contains functions for interacting with the V5 Motors.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <errno.h>
#include <math.h>
#include <stdio.h>

#include "kapi.h"
#include "pros/motors.h"
#include "v5_api.h"
#include "vdml/registry.h"
#include "vdml/vdml.h"

#define MOTOR_MOVE_RANGE 127
#define MOTOR_VOLTAGE_RANGE 12000

typedef struct motor_data {
	V5_DeviceMotorPid pos_pid, vel_pid;
} motor_data_s_t;

static V5_DeviceMotorPid get_pos_pid(uint8_t port) {
	return ((motor_data_s_t*)registry_get_device(port)->pad)->pos_pid;
}

static void set_pos_pid(uint8_t port, V5_DeviceMotorPid pos) {
	motor_data_s_t* data = (motor_data_s_t*)registry_get_device(port)->pad;
	data->pos_pid = pos;
}

static V5_DeviceMotorPid get_vel_pid(uint8_t port) {
	return ((motor_data_s_t*)registry_get_device(port)->pad)->vel_pid;
}

static void set_vel_pid(uint8_t port, V5_DeviceMotorPid vel) {
	motor_data_s_t* data = (motor_data_s_t*)registry_get_device(port)->pad;
	data->vel_pid = vel;
}

// Movement functions

int32_t motor_move(uint8_t port, int32_t voltage) {
	if (voltage > 127) {
		voltage = 127;
	} else if (voltage < -127) {
		voltage = -127;
	}

	// Remap the input voltage range to the motor voltage
	// scale to [-127, 127] -> [-12000, 12000]
	int32_t command = (((voltage + MOTOR_MOVE_RANGE) * (MOTOR_VOLTAGE_RANGE)) / (MOTOR_MOVE_RANGE));
	command -= MOTOR_VOLTAGE_RANGE;
	return motor_move_voltage(port, command);
}

int32_t motor_brake(uint8_t port) {
	return motor_move_velocity(port, 0);
}

int32_t motor_move_absolute(uint8_t port, const double position, const int32_t velocity) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorAbsoluteTargetSet(device->device_info, position, velocity);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t motor_move_relative(uint8_t port, const double position, const int32_t velocity) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorRelativeTargetSet(device->device_info, position, velocity);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t motor_move_velocity(uint8_t port, const int32_t velocity) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorVelocitySet(device->device_info, velocity);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t motor_move_voltage(uint8_t port, const int32_t voltage) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorVoltageSet(device->device_info, voltage);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t motor_modify_profiled_velocity(uint8_t port, const int32_t velocity) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorVelocityUpdate(device->device_info, velocity);
	return_port(port - 1, PROS_SUCCESS);
}

double motor_get_target_position(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_MOTOR);
	double rtn = vexDeviceMotorTargetGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_get_target_velocity(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	int32_t rtn = vexDeviceMotorVelocityGet(device->device_info);
	return_port(port - 1, rtn);
}

// Telemetry functions

double motor_get_actual_velocity(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_MOTOR);
	double rtn = vexDeviceMotorActualVelocityGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_get_current_draw(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	int32_t rtn = vexDeviceMotorCurrentGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_get_direction(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	int32_t rtn = vexDeviceMotorDirectionGet(device->device_info);
	return_port(port - 1, rtn);
}

double motor_get_efficiency(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_MOTOR);
	double rtn = vexDeviceMotorEfficiencyGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_is_over_current(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	int rtn = vexDeviceMotorCurrentLimitFlagGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_is_over_temp(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	int rtn = vexDeviceMotorOverTempFlagGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_is_stopped(uint8_t port) {
	errno = ENOSYS;
	return PROS_ERR;
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	int rtn = vexDeviceMotorZeroVelocityFlagGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_get_zero_position_flag(uint8_t port) {
	errno = ENOSYS;
	return PROS_ERR;
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	int rtn = vexDeviceMotorZeroPositionFlagGet(device->device_info);
	return_port(port - 1, rtn);
}

uint32_t motor_get_faults(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	uint32_t rtn = vexDeviceMotorFaultsGet(device->device_info);
	return_port(port - 1, rtn);
}

uint32_t motor_get_flags(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	uint32_t rtn = vexDeviceMotorFlagsGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_get_raw_position(uint8_t port, uint32_t* const timestamp) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	int32_t rtn = vexDeviceMotorPositionRawGet(device->device_info, timestamp);
	return_port(port - 1, rtn);
}

double motor_get_position(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_MOTOR);
	double rtn = vexDeviceMotorPositionGet(device->device_info);
	return_port(port - 1, rtn);
}

double motor_get_power(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_MOTOR);
	double rtn = vexDeviceMotorPowerGet(device->device_info);
	return_port(port - 1, rtn);
}

double motor_get_temperature(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_MOTOR);
	double rtn = vexDeviceMotorTemperatureGet(device->device_info);
	return_port(port - 1, rtn);
}

double motor_get_torque(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_MOTOR);
	double rtn = vexDeviceMotorTorqueGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_get_voltage(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	int32_t rtn = vexDeviceMotorVoltageGet(device->device_info);
	return_port(port - 1, rtn);
}

// Config functions

int32_t motor_set_zero_position(uint8_t port, const double position) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorPositionSet(device->device_info, position);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t motor_tare_position(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorPositionReset(device->device_info);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t motor_set_brake_mode(uint8_t port, const motor_brake_mode_e_t mode) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorBrakeModeSet(device->device_info, (V5MotorBrakeMode)mode);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t motor_set_current_limit(uint8_t port, const int32_t limit) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorCurrentLimitSet(device->device_info, limit);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t motor_set_encoder_units(uint8_t port, const motor_encoder_units_e_t units) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorEncoderUnitsSet(device->device_info, (V5MotorEncoderUnits)units);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t motor_set_gearing(uint8_t port, const motor_gearset_e_t gearset) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorGearingSet(device->device_info, (V5MotorGearset)gearset);
	return_port(port - 1, PROS_SUCCESS);
}

motor_pid_full_s_t motor_convert_pid_full(double kf, double kp, double ki, double kd, double filter, double limit,
                                          double threshold, double loopspeed) {
	motor_pid_full_s_t rtn;
	rtn.kf = roundf(kf * 16);
	rtn.kp = roundf(kp * 16);
	rtn.ki = roundf(ki * 16);
	rtn.kd = roundf(kd * 16);
	rtn.filter = roundf(filter * 16);
	rtn.limit = roundf(limit * 16);
	rtn.threshold = roundf(threshold * 16);
	rtn.loopspeed = roundf(loopspeed * 16);
	return rtn;
}

motor_pid_s_t motor_convert_pid(double kf, double kp, double ki, double kd) {
	motor_pid_s_t rtn;
	rtn.kf = roundf(kf * 16);
	rtn.kp = roundf(kp * 16);
	rtn.ki = roundf(ki * 16);
	rtn.kd = roundf(kd * 16);
	return rtn;
}

int32_t motor_set_pos_pid(uint8_t port, const motor_pid_s_t pid) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	V5_DeviceMotorPid out;
	out.kf = pid.kf;
	out.kp = pid.kp;
	out.ki = pid.ki;
	out.kd = pid.kd;
	set_pos_pid(port - 1, out);
	vexDeviceMotorPositionPidSet(device->device_info, &out);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t motor_set_pos_pid_full(uint8_t port, const motor_pid_full_s_t pid) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	V5_DeviceMotorPid out;
	out.kf = pid.kf;
	out.kp = pid.kp;
	out.ki = pid.ki;
	out.kd = pid.kd;
	out.filter = pid.filter;
	out.limit = pid.limit;
	out.threshold = pid.threshold;
	out.loopspeed = pid.loopspeed;
	set_pos_pid(port - 1, out);
	vexDeviceMotorPositionPidSet(device->device_info, &out);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t motor_set_vel_pid(uint8_t port, const motor_pid_s_t pid) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	V5_DeviceMotorPid out;
	out.kf = pid.kf;
	out.kp = pid.kp;
	out.ki = pid.ki;
	out.kd = pid.kd;
	set_vel_pid(port - 1, out);
	vexDeviceMotorPositionPidSet(device->device_info, &out);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t motor_set_vel_pid_full(uint8_t port, const motor_pid_full_s_t pid) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	V5_DeviceMotorPid out;
	out.kf = pid.kf;
	out.kp = pid.kp;
	out.ki = pid.ki;
	out.kd = pid.kd;
	out.filter = pid.filter;
	out.limit = pid.limit;
	out.threshold = pid.threshold;
	out.loopspeed = pid.loopspeed;
	set_vel_pid(port - 1, out);
	vexDeviceMotorPositionPidSet(device->device_info, &out);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t motor_set_reversed(uint8_t port, const bool reverse) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorReverseFlagSet(device->device_info, reverse);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t motor_set_voltage_limit(uint8_t port, const int32_t limit) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorVoltageLimitSet(device->device_info, limit);
	return_port(port - 1, PROS_SUCCESS);
}

motor_brake_mode_e_t motor_get_brake_mode(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	V5MotorBrakeMode rtn = vexDeviceMotorBrakeModeGet(device->device_info);
	return_port(port - 1, (motor_brake_mode_e_t)rtn);
}

int32_t motor_get_current_limit(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	int32_t rtn = vexDeviceMotorCurrentLimitGet(device->device_info);
	return_port(port - 1, rtn);
}

motor_encoder_units_e_t motor_get_encoder_units(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	V5MotorEncoderUnits rtn = vexDeviceMotorEncoderUnitsGet(device->device_info);
	return_port(port - 1, (motor_encoder_units_e_t)rtn);
}

motor_gearset_e_t motor_get_gearing(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	V5MotorGearset rtn = vexDeviceMotorGearingGet(device->device_info);
	return_port(port - 1, (motor_gearset_e_t)rtn);
}

motor_pid_full_s_t motor_get_pos_pid(uint8_t port) {
	motor_pid_full_s_t rtn;
	if (!claim_port_try(port - 1, E_DEVICE_MOTOR)) {
		// Set all fields to lowest
		rtn.kf = 0;
		rtn.kp = 0;
		rtn.ki = 0;
		rtn.kd = 0;
		rtn.filter = 0;
		rtn.limit = 0;
		rtn.threshold = 0;
		rtn.loopspeed = 0;
		return rtn;
	}
	V5_DeviceMotorPid pid = get_pos_pid(port - 1);
	rtn.kf = pid.kf;
	rtn.kp = pid.kp;
	rtn.ki = pid.ki;
	rtn.kd = pid.kd;
	rtn.filter = pid.filter;
	rtn.limit = pid.limit;
	rtn.threshold = pid.threshold;
	rtn.loopspeed = pid.loopspeed;
	return_port(port - 1, rtn);
}

motor_pid_full_s_t motor_get_vel_pid(uint8_t port) {
	motor_pid_full_s_t rtn;
	if (!claim_port_try(port - 1, E_DEVICE_MOTOR)) {
		// Set all fields to lowest
		rtn.kf = 0;
		rtn.kp = 0;
		rtn.ki = 0;
		rtn.kd = 0;
		rtn.filter = 0;
		rtn.limit = 0;
		rtn.threshold = 0;
		rtn.loopspeed = 0;
		return rtn;
	}
	V5_DeviceMotorPid pid = get_vel_pid(port - 1);
	rtn.kf = pid.kf;
	rtn.kp = pid.kp;
	rtn.ki = pid.ki;
	rtn.kd = pid.kd;
	rtn.filter = pid.filter;
	rtn.limit = pid.limit;
	rtn.threshold = pid.threshold;
	rtn.loopspeed = pid.loopspeed;
	return_port(port - 1, rtn);
}

int32_t motor_is_reversed(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	int rtn = vexDeviceMotorReverseFlagGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_get_voltage_limit(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_MOTOR);
	int32_t rtn = vexDeviceMotorVoltageLimitGet(device->device_info);
	return_port(port - 1, rtn);
}
