/**
 * \file vdml.c
 *
 * \brief VDML Devices Mutex Management.
 *
 * This file ensure thread saftey for operations on motors by maintaining
 * an array of RTOS Mutexes and implementing functions to take and give them.
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
#include "pros/motors.h"
#include "vdml/registry.h"
#include "vdml/vdml.h"

#define MOTOR_MOVE_RANGE 127
#define MOTOR_VOLTAGE_RANGE 12000

typedef union motor_data {
	struct {
		V5_DeviceMotorPid pos, vel;
	} pid_data;
	uint8_t raw[24];
} motor_data_s_t;

static V5_DeviceMotorPid get_pos_pid(uint8_t port) {
	motor_data_s_t data;
	data.raw[0] = registry_get_device(port)->pad[port * 4];
	data.raw[1] = registry_get_device(port)->pad[port * 4 + 1];
	data.raw[2] = registry_get_device(port)->pad[port * 4 + 2];
	data.raw[3] = registry_get_device(port)->pad[port * 4 + 3];
	data.raw[4] = registry_get_device(port)->pad[port * 4 + 4];
	data.raw[5] = registry_get_device(port)->pad[port * 4 + 5];
	data.raw[6] = registry_get_device(port)->pad[port * 4 + 6];
	data.raw[7] = registry_get_device(port)->pad[port * 4 + 7];
	data.raw[8] = registry_get_device(port)->pad[port * 4 + 8];
	data.raw[9] = registry_get_device(port)->pad[port * 4 + 9];
	data.raw[10] = registry_get_device(port)->pad[port * 4 + 10];
	data.raw[11] = registry_get_device(port)->pad[port * 4 + 11];
	return data.pid_data.pos;
}

static void set_pos_pid(uint8_t port, V5_DeviceMotorPid pos) {
	motor_data_s_t data;
	data.pid_data.pos = pos;
	registry_get_device(port)->pad[port * 4] = data.raw[0];
	registry_get_device(port)->pad[port * 4 + 1] = data.raw[1];
	registry_get_device(port)->pad[port * 4 + 2] = data.raw[2];
	registry_get_device(port)->pad[port * 4 + 3] = data.raw[3];
	registry_get_device(port)->pad[port * 4 + 4] = data.raw[4];
	registry_get_device(port)->pad[port * 4 + 5] = data.raw[5];
	registry_get_device(port)->pad[port * 4 + 6] = data.raw[6];
	registry_get_device(port)->pad[port * 4 + 7] = data.raw[7];
	registry_get_device(port)->pad[port * 4 + 8] = data.raw[8];
	registry_get_device(port)->pad[port * 4 + 9] = data.raw[9];
	registry_get_device(port)->pad[port * 4 + 10] = data.raw[10];
	registry_get_device(port)->pad[port * 4 + 11] = data.raw[11];
}

static V5_DeviceMotorPid get_vel_pid(uint8_t port) {
	motor_data_s_t data;
	data.raw[12] = registry_get_device(port)->pad[port * 4 + 12];
	data.raw[13] = registry_get_device(port)->pad[port * 4 + 13];
	data.raw[14] = registry_get_device(port)->pad[port * 4 + 14];
	data.raw[15] = registry_get_device(port)->pad[port * 4 + 15];
	data.raw[16] = registry_get_device(port)->pad[port * 4 + 16];
	data.raw[17] = registry_get_device(port)->pad[port * 4 + 17];
	data.raw[18] = registry_get_device(port)->pad[port * 4 + 18];
	data.raw[19] = registry_get_device(port)->pad[port * 4 + 19];
	data.raw[20] = registry_get_device(port)->pad[port * 4 + 20];
	data.raw[21] = registry_get_device(port)->pad[port * 4 + 21];
	data.raw[22] = registry_get_device(port)->pad[port * 4 + 22];
	data.raw[23] = registry_get_device(port)->pad[port * 4 + 23];
	return data.pid_data.vel;
}

static void set_vel_pid(uint8_t port, V5_DeviceMotorPid vel) {
	motor_data_s_t data;
	data.pid_data.vel = vel;
	registry_get_device(port)->pad[port * 4 + 12] = data.raw[12];
	registry_get_device(port)->pad[port * 4 + 13] = data.raw[13];
	registry_get_device(port)->pad[port * 4 + 14] = data.raw[14];
	registry_get_device(port)->pad[port * 4 + 15] = data.raw[15];
	registry_get_device(port)->pad[port * 4 + 16] = data.raw[16];
	registry_get_device(port)->pad[port * 4 + 17] = data.raw[17];
	registry_get_device(port)->pad[port * 4 + 18] = data.raw[18];
	registry_get_device(port)->pad[port * 4 + 19] = data.raw[19];
	registry_get_device(port)->pad[port * 4 + 20] = data.raw[20];
	registry_get_device(port)->pad[port * 4 + 21] = data.raw[21];
	registry_get_device(port)->pad[port * 4 + 22] = data.raw[22];
	registry_get_device(port)->pad[port * 4 + 23] = data.raw[23];
}

// Motor functions

int32_t motor_move(uint8_t port, const int8_t voltage) {
	// Remap the input voltage range to the motor voltage
	// scale to [0, 256] -> [0, 24000] -> [-100, 100]
	int32_t command = (((voltage + MOTOR_MOVE_RANGE) * (MOTOR_VOLTAGE_RANGE)) / (MOTOR_MOVE_RANGE));
	command -= MOTOR_VOLTAGE_RANGE;  // readjust back to [-100, 100]
	return motor_move_voltage(port, command);
}

int32_t motor_move_absolute(uint8_t port, const double position, const int32_t velocity) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorAbsoluteTargetSet(device->device_info, position, velocity);
	return_port(port - 1, 1);
}

int32_t motor_move_relative(uint8_t port, const double position, const int32_t velocity) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorRelativeTargetSet(device->device_info, position, velocity);
	return_port(port - 1, 1);
}

int32_t motor_move_velocity(uint8_t port, const int32_t velocity) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorVelocitySet(device->device_info, velocity);
	return_port(port - 1, 1);
}

int32_t motor_move_voltage(uint8_t port, const int32_t voltage) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorVoltageSet(device->device_info, voltage);
	return_port(port - 1, 1);
}

int32_t motor_modify_profiled_velocity(uint8_t port, const int32_t velocity) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorVelocityUpdate(device->device_info, velocity);
	return_port(port - 1, 1);
}

double motor_get_target_position(uint8_t port) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	double rtn = vexDeviceMotorTargetGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_get_target_velocity(uint8_t port) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	int32_t rtn = vexDeviceMotorVelocityGet(device->device_info);
	return_port(port - 1, rtn);
}

// Telemetry functions

double motor_get_actual_velocity(uint8_t port) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	double rtn = vexDeviceMotorActualVelocityGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_get_current_draw(uint8_t port) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	int32_t rtn = vexDeviceMotorCurrentGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_get_direction(uint8_t port) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	int32_t rtn = vexDeviceMotorDirectionGet(device->device_info);
	return_port(port - 1, rtn);
}

double motor_get_efficiency(uint8_t port) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	double rtn = vexDeviceMotorEfficiencyGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_is_over_current(uint8_t port) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	int rtn = vexDeviceMotorCurrentLimitFlagGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_is_over_temp(uint8_t port) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	int rtn = vexDeviceMotorOverTempFlagGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_is_stopped(uint32_t port) {
	errno = ENOSYS;
	return PROS_ERR;
	claim_port(port - 1, E_DEVICE_MOTOR);
	int rtn = vexDeviceMotorZeroVelocityFlagGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_get_zero_position_flag(uint32_t port) {
	errno = ENOSYS;
	return PROS_ERR;
	claim_port(port - 1, E_DEVICE_MOTOR);
	int rtn = vexDeviceMotorZeroPositionFlagGet(device->device_info);
	return_port(port - 1, rtn);
}

uint32_t motor_get_faults(uint8_t port) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	uint32_t rtn = vexDeviceMotorFaultsGet(device->device_info);
	return_port(port - 1, rtn);
}

uint32_t motor_get_flags(uint8_t port) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	uint32_t rtn = vexDeviceMotorFlagsGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_get_raw_position(uint8_t port, uint32_t* const timestamp) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	int32_t rtn = vexDeviceMotorPositionRawGet(device->device_info, timestamp);
	return_port(port - 1, rtn);
}

double motor_get_position(uint8_t port) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	double rtn = vexDeviceMotorPositionGet(device->device_info);
	return_port(port - 1, rtn);
}

double motor_get_power(uint8_t port) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	double rtn = vexDeviceMotorPowerGet(device->device_info);
	return_port(port - 1, rtn);
}

double motor_get_temperature(uint8_t port) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	double rtn = vexDeviceMotorTemperatureGet(device->device_info);
	return_port(port - 1, rtn);
}

double motor_get_torque(uint8_t port) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	double rtn = vexDeviceMotorTorqueGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_get_voltage(uint8_t port) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	int32_t rtn = vexDeviceMotorVoltageGet(device->device_info);
	return_port(port - 1, rtn);
}

// Config functions

int32_t motor_set_zero_position(uint8_t port, const double position) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorPositionSet(device->device_info, position);
	return_port(port - 1, 1);
}

int32_t motor_tare_position(uint8_t port) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorPositionReset(device->device_info);
	return_port(port - 1, 1);
}

int32_t motor_set_brake_mode(uint8_t port, const motor_brake_mode_e_t mode) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorBrakeModeSet(device->device_info, (V5MotorBrakeMode)mode);
	return_port(port - 1, 1);
}

int32_t motor_set_current_limit(uint8_t port, const int32_t limit) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorCurrentLimitSet(device->device_info, limit);
	return_port(port - 1, 1);
}

int32_t motor_set_encoder_units(uint8_t port, const motor_encoder_units_e_t units) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorEncoderUnitsSet(device->device_info, (V5MotorEncoderUnits)units);
	return_port(port - 1, 1);
}

int32_t motor_set_gearing(uint8_t port, const motor_gearset_e_t gearset) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorGearingSet(device->device_info, (V5MotorGearset)gearset);
	return_port(port - 1, 1);
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
	claim_port(port - 1, E_DEVICE_MOTOR);
	V5_DeviceMotorPid out;
	out.kf = pid.kf;
	out.kp = pid.kp;
	out.ki = pid.ki;
	out.kd = pid.kd;
	set_pos_pid(port, out);
	vexDeviceMotorPositionPidSet(device->device_info, &out);
	return_port(port - 1, 1);
}

int32_t motor_set_pos_pid_full(uint8_t port, const motor_pid_full_s_t pid) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	V5_DeviceMotorPid out;
	out.kf = pid.kf;
	out.kp = pid.kp;
	out.ki = pid.ki;
	out.kd = pid.kd;
	out.filter = pid.filter;
	out.limit = pid.limit;
	out.threshold = pid.threshold;
	out.loopspeed = pid.loopspeed;
	set_pos_pid(port, out);
	vexDeviceMotorPositionPidSet(device->device_info, &out);
	return_port(port - 1, 1);
}

int32_t motor_set_vel_pid(uint8_t port, const motor_pid_s_t pid) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	V5_DeviceMotorPid out;
	out.kf = pid.kf;
	out.kp = pid.kp;
	out.ki = pid.ki;
	out.kd = pid.kd;
	set_vel_pid(port, out);
	vexDeviceMotorPositionPidSet(device->device_info, &out);
	return_port(port - 1, 1);
}

int32_t motor_set_vel_pid_full(uint8_t port, const motor_pid_full_s_t pid) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	V5_DeviceMotorPid out;
	out.kf = pid.kf;
	out.kp = pid.kp;
	out.ki = pid.ki;
	out.kd = pid.kd;
	out.filter = pid.filter;
	out.limit = pid.limit;
	out.threshold = pid.threshold;
	out.loopspeed = pid.loopspeed;
	set_vel_pid(port, out);
	vexDeviceMotorPositionPidSet(device->device_info, &out);
	return_port(port - 1, 1);
}

int32_t motor_set_reversed(uint8_t port, const bool reverse) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorReverseFlagSet(device->device_info, reverse);
	return_port(port - 1, 1);
}

int32_t motor_set_voltage_limit(uint8_t port, const int32_t limit) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	vexDeviceMotorVoltageLimitSet(device->device_info, limit);
	return_port(port - 1, 1);
}

motor_brake_mode_e_t motor_get_brake_mode(uint8_t port) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	V5MotorBrakeMode rtn = vexDeviceMotorBrakeModeGet(device->device_info);
	return_port(port - 1, (motor_brake_mode_e_t)rtn);
}

int32_t motor_get_current_limit(uint8_t port) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	int32_t rtn = vexDeviceMotorCurrentLimitGet(device->device_info);
	return_port(port - 1, rtn);
}

motor_encoder_units_e_t motor_get_encoder_units(uint8_t port) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	V5MotorEncoderUnits rtn = vexDeviceMotorEncoderUnitsGet(device->device_info);
	return_port(port - 1, (motor_encoder_units_e_t)rtn);
}

motor_gearset_e_t motor_get_gearing(uint8_t port) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	V5MotorGearset rtn = vexDeviceMotorGearingGet(device->device_info);
	return_port(port - 1, (motor_gearset_e_t)rtn);
}

motor_pid_full_s_t motor_get_pos_pid(uint8_t port) {
	motor_pid_full_s_t rtn;
	int32_t err = claim_port_try(port - 1, E_DEVICE_MOTOR);
	if (err == PROS_ERR) {
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
	V5_DeviceMotorPid pid = get_pos_pid(port);
	rtn.kf = pid.kf;
	rtn.kp = pid.kp;
	rtn.ki = pid.ki;
	rtn.kd = pid.kd;
	rtn.filter = pid.filter;
	rtn.limit = pid.limit;
	rtn.threshold = pid.threshold;
	rtn.loopspeed = pid.loopspeed;
	port_mutex_give(port - 1);
	return rtn;
}

motor_pid_full_s_t motor_get_vel_pid(uint8_t port) {
	motor_pid_full_s_t rtn;
	int32_t err = claim_port_try(port - 1, E_DEVICE_MOTOR);
	if (err == PROS_ERR) {
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
	V5_DeviceMotorPid pid = get_vel_pid(port);
	rtn.kf = pid.kf;
	rtn.kp = pid.kp;
	rtn.ki = pid.ki;
	rtn.kd = pid.kd;
	rtn.filter = pid.filter;
	rtn.limit = pid.limit;
	rtn.threshold = pid.threshold;
	rtn.loopspeed = pid.loopspeed;
	port_mutex_give(port - 1);
	return rtn;
}

int32_t motor_is_reversed(uint8_t port) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	int rtn = vexDeviceMotorReverseFlagGet(device->device_info);
	return_port(port - 1, rtn);
}

int32_t motor_get_voltage_limit(uint8_t port) {
	claim_port(port - 1, E_DEVICE_MOTOR);
	int32_t rtn = vexDeviceMotorVoltageLimitGet(device->device_info);
	return_port(rtn, port - 1);
}
