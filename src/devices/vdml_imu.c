/**
 * \file devices/vdml_imu.c
 *
 * Contains functions for interacting with the VEX Inertial sensor.
 *
 * Copyright (c) 2017-2019, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <errno.h>
#include "pros/imu.h"
#include "v5_api.h"
#include "vdml/registry.h"
#include "vdml/vdml.h"

#define ERROR_IMU_STILL_CALIBRATING(port, device, err_return)                  \
	if (vexDeviceImuStatusGet(device->device_info) & E_IMU_STATUS_CALIBRATING) { \
		errno = EAGAIN;                                                            \
		return_port(port - 1, err_return);                                         \
	}

int32_t imu_reset(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_IMU);
	ERROR_IMU_STILL_CALIBRATING(port, device, PROS_ERR);
	vexDeviceImuReset(device->device_info);
	return_port(port - 1, 1);
}

int32_t imu_set_data_rate(uint8_t port, uint32_t rate) {
	claim_port_i(port - 1, E_DEVICE_IMU);
	ERROR_IMU_STILL_CALIBRATING(port, device, PROS_ERR);

	// rate is not less than 5ms, and rounded down to nearest increment of 5
	if (rate < IMU_MINIMUM_DATA_RATE) {
		rate = IMU_MINIMUM_DATA_RATE;
	} else {
		rate -= rate % IMU_MINIMUM_DATA_RATE;
	}

	vexDeviceImuDataRateSet(device->device_info, rate);
	return_port(port - 1, 1);
}

double imu_get_rotation(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_IMU);
	ERROR_IMU_STILL_CALIBRATING(port, device, PROS_ERR_F);
	double rtn = vexDeviceImuHeadingGet(device->device_info);
	return_port(port - 1, rtn);
}

double imu_get_heading(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_IMU);
	ERROR_IMU_STILL_CALIBRATING(port, device, PROS_ERR_F);
	double rtn = vexDeviceImuDegreesGet(device->device_info);
	return_port(port - 1, rtn);
}

#define QUATERNION_ERR_INIT \
	{ .x = PROS_ERR_F, .y = PROS_ERR_F, .z = PROS_ERR_F, .w = PROS_ERR_F }

quaternion_s_t imu_get_quaternion(uint8_t port) {
	quaternion_s_t rtn = QUATERNION_ERR_INIT;
	v5_smart_device_s_t* device;
	if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return rtn;
	}
	device = registry_get_device(port - 1);
	ERROR_IMU_STILL_CALIBRATING(port, device, rtn);
	// HACK: vexos represents quaternions as {a,b,c,d} and we want them in
	// {x,y,z,w}. we don't know how exactly the quaternion data is filled, so best
	// just to manually shuffle stuff into the right places here
	V5_DeviceImuQuaternion qt;
	vexDeviceImuQuaternionGet(device->device_info, &qt);
	rtn.x = qt.b;
	rtn.y = qt.c;
	rtn.z = qt.d;
	rtn.w = qt.a;
	return_port(port - 1, rtn);
}

#define ATTITUDE_ERR_INIT \
	{ .pitch = PROS_ERR_F, .roll = PROS_ERR_F, .yaw = PROS_ERR_F }

euler_s_t imu_get_euler(uint8_t port) {
	euler_s_t rtn = ATTITUDE_ERR_INIT;
	v5_smart_device_s_t* device;
	if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return rtn;
	}
	device = registry_get_device(port - 1);
	ERROR_IMU_STILL_CALIBRATING(port, device, rtn);
	vexDeviceImuAttitudeGet(device->device_info, (V5_DeviceImuAttitude*)&rtn);
	return_port(port - 1, rtn);
}

double imu_get_pitch(uint8_t port) {
	return imu_get_euler(port).pitch;
}

double imu_get_roll(uint8_t port) {
	return imu_get_euler(port).roll;
}

double imu_get_yaw(uint8_t port) {
	return imu_get_euler(port).yaw;
}

#define RAW_IMU_ERR_INIT {.x = PROS_ERR_F, .y = PROS_ERR_F, .z = PROS_ERR_F};

imu_gyro_s_t imu_get_gyro_rate(uint8_t port) {
	imu_gyro_s_t rtn = RAW_IMU_ERR_INIT;
	v5_smart_device_s_t* device;
	if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return rtn;
	}
	device = registry_get_device(port - 1);
	ERROR_IMU_STILL_CALIBRATING(port, device, rtn);
	// NOTE: `V5_DeviceImuRaw` has the same form as a quaternion, but this call
	// never fills the `w` field, so we make a dummy quaternion container and copy
	// the (x,y,z) part into the return struct
	quaternion_s_t dummy;
	vexDeviceImuRawGyroGet(device->device_info, (V5_DeviceImuRaw*)&dummy);
	rtn.x = dummy.x;
	rtn.y = dummy.y;
	rtn.z = dummy.z;
	return_port(port - 1, rtn);
}

imu_accel_s_t imu_get_accel(uint8_t port) {
	imu_accel_s_t rtn = RAW_IMU_ERR_INIT;
	v5_smart_device_s_t* device;
	if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return rtn;
	}
	device = registry_get_device(port - 1);
	ERROR_IMU_STILL_CALIBRATING(port, device, rtn);
	// NOTE: this is the same as `imu_get_raw_gyro`
	quaternion_s_t dummy;
	vexDeviceImuRawAccelGet(device->device_info, (V5_DeviceImuRaw*)&dummy);
	rtn.x = dummy.x;
	rtn.y = dummy.y;
	rtn.z = dummy.z;
	return_port(port - 1, rtn);
}

imu_status_e_t imu_get_status(uint8_t port) {
	imu_status_e_t rtn = E_IMU_STATUS_ERROR;
	v5_smart_device_s_t* device;
	if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return rtn;
	}
	device = registry_get_device(port - 1);
	rtn = vexDeviceImuStatusGet(device->device_info);
	return_port(port - 1, rtn);
}
