/**
 * \file devices/vdml_imu.c
 *
 * Contains functions for interacting with the VEX Inertial sensor.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
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

#define IMU_EULER_LIMIT 180
#define IMU_HEADING_MAX 360

#define DEGTORAD (M_PI / 180)

#define ERROR_IMU_STILL_CALIBRATING(port, device, err_return)                  \
	if (vexDeviceImuStatusGet(device->device_info) & E_IMU_STATUS_CALIBRATING) { \
		errno = EAGAIN;                                                            \
		return_port(port - 1, err_return);                                         \
	}

#define IMU_RESET_FLAG_SET_TIMEOUT 1000
#define IMU_RESET_TIMEOUT 3000 // Canonically this should be 2s, but 3s for good margin

typedef struct __attribute__ ((packed)) imu_reset_data { 
	double heading_offset;
	double rotation_offset;
	double pitch_offset;
	double yaw_offset;
	double roll_offset;
} imu_data_s_t;

int32_t imu_reset(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_IMU);
	ERROR_IMU_STILL_CALIBRATING(port, device, PROS_ERR);
	vexDeviceImuReset(device->device_info);
	// delay for vexos to set calibration flag, background processing must be called for flag
	// to be set.
	uint16_t timeoutCount = 0;
	// releasing mutex so vexBackgrounProcessing can run without being blocked.
	do {
		port_mutex_give(port - 1);
		task_delay(5);
		timeoutCount += 5;
		claim_port_i(port - 1, E_DEVICE_IMU);
		if (timeoutCount >= IMU_RESET_FLAG_SET_TIMEOUT) {
			port_mutex_give(port - 1);
			errno = EAGAIN;
			return PROS_ERR;
		}
		device = device; // suppressing compiler warning
	} while(!(vexDeviceImuStatusGet(device->device_info) & E_IMU_STATUS_CALIBRATING));
	port_mutex_give(port - 1);
	return 1;
}

int32_t imu_reset_blocking(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_IMU);
	ERROR_IMU_STILL_CALIBRATING(port, device, PROS_ERR);
	vexDeviceImuReset(device->device_info);
	// delay for vexos to set calibration flag, background processing must be called for flag
	// to be set.
	uint16_t timeoutCount = 0;
	// releasing mutex so vexBackgroundProcessing can run without being blocked.
	do {
		port_mutex_give(port - 1);
		task_delay(5);
		timeoutCount += 5;
		claim_port_i(port - 1, E_DEVICE_IMU);
		if (timeoutCount >= IMU_RESET_FLAG_SET_TIMEOUT) {
			port_mutex_give(port - 1);
			errno = EAGAIN;
			return PROS_ERR;
		}
		device = device; // suppressing compiler warning
	} while(!(vexDeviceImuStatusGet(device->device_info) & E_IMU_STATUS_CALIBRATING));
	// same concept here, we add a blocking delay for the blocking version to wait
	// until the IMU calibrating flag is cleared
	do {
		port_mutex_give(port - 1);
		task_delay(5);
		timeoutCount += 5;
		claim_port_i(port - 1, E_DEVICE_IMU);
		if (timeoutCount >= IMU_RESET_TIMEOUT) {
			port_mutex_give(port - 1);
			errno = EAGAIN;
			return PROS_ERR;
		}
		device = device; // suppressing compiler warning
	} while(vexDeviceImuStatusGet(device->device_info) & E_IMU_STATUS_CALIBRATING);
	port_mutex_give(port - 1);
	return 1;
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
	return_port(port - 1, PROS_SUCCESS);
}

double imu_get_rotation(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_IMU);
	ERROR_IMU_STILL_CALIBRATING(port, device, PROS_ERR_F);
	double rtn = vexDeviceImuHeadingGet(device->device_info) + ((imu_data_s_t*)registry_get_device(port - 1)->pad)->rotation_offset;
	return_port(port - 1, rtn);
}

double imu_get_heading(uint8_t port) {
	claim_port_f(port - 1, E_DEVICE_IMU);
	ERROR_IMU_STILL_CALIBRATING(port, device, PROS_ERR_F);
	double rtn = vexDeviceImuDegreesGet(device->device_info) + ((imu_data_s_t*)registry_get_device(port - 1)->pad)->heading_offset;
	// Restricting value to raw boundaries
	return_port(port - 1, fmod((rtn + IMU_HEADING_MAX), (double) IMU_HEADING_MAX));
}

#define QUATERNION_ERR_INIT \
	{ .x = PROS_ERR_F, .y = PROS_ERR_F, .z = PROS_ERR_F, .w = PROS_ERR_F }

quaternion_s_t imu_get_quaternion(uint8_t port) {
	quaternion_s_t rtn = QUATERNION_ERR_INIT;
	if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return rtn;
	}
	v5_smart_device_s_t* device = registry_get_device(port - 1);
	ERROR_IMU_STILL_CALIBRATING(port, device, rtn);
	euler_s_t euler;
	vexDeviceImuAttitudeGet(device->device_info, (V5_DeviceImuAttitude*)&euler);
	imu_data_s_t* data = (imu_data_s_t*)device->pad;
	// To calculate the quaternion values, we first get the euler values, add the offsets,
	// and then do the calculations.
	double roll = fmod(euler.roll + data->roll_offset, 2.0 * IMU_EULER_LIMIT);
	double yaw = fmod(euler.yaw + data->yaw_offset, 2.0 * IMU_EULER_LIMIT);
	double pitch = fmod(euler.pitch + data->pitch_offset, 2.0 * IMU_EULER_LIMIT);

	double cy = cos(DEGTORAD * yaw * 0.5);
	double sy = sin(DEGTORAD * yaw * 0.5);
	double cp = cos(DEGTORAD * pitch * 0.5);
	double sp = sin(DEGTORAD * pitch * 0.5);
	double cr = cos(DEGTORAD * roll * 0.5);
	double sr = sin(DEGTORAD * roll * 0.5); 

	rtn.w = cr * cp * cy + sr * sp * sy;
	rtn.x = sr * cp * cy - cr * sp * sy;
	rtn.y = cr * sp * cy + sr * cp * sy;
	rtn.z = cr * cp * sy - sr * sp * cy;

	return_port(port - 1, rtn);
}

#define ATTITUDE_ERR_INIT \
	{ .pitch = PROS_ERR_F, .roll = PROS_ERR_F, .yaw = PROS_ERR_F }

euler_s_t imu_get_euler(uint8_t port) {
	euler_s_t rtn = ATTITUDE_ERR_INIT;
	if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return rtn;
	}
	v5_smart_device_s_t* device = registry_get_device(port - 1);
	imu_data_s_t* data = (imu_data_s_t*)device->pad;
	ERROR_IMU_STILL_CALIBRATING(port, device, rtn);
	vexDeviceImuAttitudeGet(device->device_info, (V5_DeviceImuAttitude*)&rtn);
	rtn.pitch += data->pitch_offset;
	rtn.yaw += data->yaw_offset;
	rtn.roll += data->roll_offset;
	rtn.roll = fmod(rtn.roll, 2.0 * IMU_EULER_LIMIT);
	rtn.yaw = fmod(rtn.yaw, 2.0 * IMU_EULER_LIMIT);
	rtn.pitch = fmod(rtn.pitch, 2.0 * IMU_EULER_LIMIT);
	return_port(port - 1, rtn);
}

double imu_get_pitch(uint8_t port) {
	double rtn = PROS_ERR_F;
	if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return rtn;
	}
	euler_s_t euler_values;
	v5_smart_device_s_t* device = registry_get_device(port - 1);
	vexDeviceImuAttitudeGet(device->device_info, (V5_DeviceImuAttitude*)&euler_values);
	rtn = euler_values.pitch + ((imu_data_s_t*)registry_get_device(port - 1)->pad)->pitch_offset;
	// Restricting value to raw boundaries
	return_port(port - 1, fmod(rtn, 2.0 * IMU_EULER_LIMIT));
}

double imu_get_roll(uint8_t port) {
	double rtn = PROS_ERR_F;
	if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return rtn;
	}
	euler_s_t euler_values;
	v5_smart_device_s_t* device = registry_get_device(port - 1);
	vexDeviceImuAttitudeGet(device->device_info, (V5_DeviceImuAttitude*)&euler_values);
	rtn = euler_values.roll + ((imu_data_s_t*)registry_get_device(port - 1)->pad)->roll_offset;
	// Restricting value to raw boundaries
	return_port(port - 1, fmod(rtn, 2.0 * IMU_EULER_LIMIT));
}

double imu_get_yaw(uint8_t port) {
	double rtn = PROS_ERR_F;
	if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return rtn;
	}
	euler_s_t euler_values;
	v5_smart_device_s_t* device = registry_get_device(port - 1);
	vexDeviceImuAttitudeGet(device->device_info, (V5_DeviceImuAttitude*)&euler_values);
	rtn = euler_values.yaw + ((imu_data_s_t*)registry_get_device(port - 1)->pad)->yaw_offset;
	// Restricting value to raw boundaries
	return_port(port - 1, fmod(rtn, 2.0 * IMU_EULER_LIMIT));
}

#define RAW_IMU_ERR_INIT {.x = PROS_ERR_F, .y = PROS_ERR_F, .z = PROS_ERR_F};

imu_gyro_s_t imu_get_gyro_rate(uint8_t port) {
	imu_gyro_s_t rtn = RAW_IMU_ERR_INIT;
	if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return rtn;
	}
	v5_smart_device_s_t* device = registry_get_device(port - 1);
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
	if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return rtn;
	}
	v5_smart_device_s_t* device = registry_get_device(port - 1);
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
	if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return rtn;
	}
	v5_smart_device_s_t* device = registry_get_device(port - 1);
	rtn = vexDeviceImuStatusGet(device->device_info);
	return_port(port - 1, rtn);
}

//Reset Functions:
int32_t imu_tare(uint8_t port){
    if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return PROS_ERR;
	}
	v5_smart_device_s_t* device = registry_get_device(port - 1);
	euler_s_t euler_values;
	vexDeviceImuAttitudeGet(device->device_info, (V5_DeviceImuAttitude*)&euler_values);
	imu_data_s_t* data = (imu_data_s_t*)device->pad;
	data->rotation_offset = -vexDeviceImuHeadingGet(device->device_info);
	data->heading_offset = -vexDeviceImuDegreesGet(device->device_info);
	data->pitch_offset = -euler_values.pitch;
	data->roll_offset = -euler_values.roll;
	data->yaw_offset = -euler_values.yaw;
	return_port(port - 1, PROS_SUCCESS);
}

int32_t imu_tare_euler(uint8_t port){
    return imu_set_euler(port, (euler_s_t){0,0,0});
}

int32_t imu_tare_heading(uint8_t port){
    return imu_set_heading(port, 0);
}

int32_t imu_tare_rotation(uint8_t port){
    return imu_set_rotation(port, 0);
}

int32_t imu_tare_pitch(uint8_t port){
    return imu_set_pitch(port, 0);
}

int32_t imu_tare_roll(uint8_t port){
    return imu_set_roll(port, 0);
}

int32_t imu_tare_yaw(uint8_t port){
    return imu_set_yaw(port, 0);
}

//Setter Functions:
int32_t imu_set_rotation(uint8_t port, double target){
    if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return PROS_ERR;
	}
	v5_smart_device_s_t* device = registry_get_device(port - 1);
	ERROR_IMU_STILL_CALIBRATING(port, device, PROS_ERR);
	euler_s_t euler_values;
	vexDeviceImuAttitudeGet(device->device_info, (V5_DeviceImuAttitude*)&euler_values);
	imu_data_s_t* data = (imu_data_s_t*)device->pad;
	data->rotation_offset = target - vexDeviceImuHeadingGet(device->device_info);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t imu_set_heading(uint8_t port, double target){
    if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return PROS_ERR;
	}
	v5_smart_device_s_t* device = registry_get_device(port - 1);
	ERROR_IMU_STILL_CALIBRATING(port, device, PROS_ERR);
	euler_s_t euler_values;
	vexDeviceImuAttitudeGet(device->device_info, (V5_DeviceImuAttitude*)&euler_values);
	imu_data_s_t* data = (imu_data_s_t*)device->pad;
	if (target > IMU_HEADING_MAX) target = IMU_HEADING_MAX;
	if (target < 0) target = 0;
	data->heading_offset = target - vexDeviceImuDegreesGet(device->device_info);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t imu_set_pitch(uint8_t port, double target){
    if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return PROS_ERR;
	}
	v5_smart_device_s_t* device = registry_get_device(port - 1);
	ERROR_IMU_STILL_CALIBRATING(port, device, PROS_ERR);
	euler_s_t euler_values;
	vexDeviceImuAttitudeGet(device->device_info, (V5_DeviceImuAttitude*)&euler_values);
	imu_data_s_t* data = (imu_data_s_t*)device->pad;
	if (target > IMU_EULER_LIMIT) target = IMU_EULER_LIMIT;
	if (target < -IMU_EULER_LIMIT) target = -IMU_EULER_LIMIT;
	data->pitch_offset = target - euler_values.pitch;
	return_port(port - 1, PROS_SUCCESS);
}

int32_t imu_set_roll(uint8_t port, double target){
    if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return PROS_ERR;
	}
	v5_smart_device_s_t* device = registry_get_device(port - 1);
	ERROR_IMU_STILL_CALIBRATING(port, device, PROS_ERR);
	euler_s_t euler_values;
	vexDeviceImuAttitudeGet(device->device_info, (V5_DeviceImuAttitude*)&euler_values);
	imu_data_s_t* data = (imu_data_s_t*)device->pad;
	if (target > IMU_EULER_LIMIT) target = IMU_EULER_LIMIT;
	if (target < -IMU_EULER_LIMIT) target = -IMU_EULER_LIMIT;
	data->roll_offset = target - euler_values.roll;
	return_port(port - 1, PROS_SUCCESS);
}

int32_t imu_set_yaw(uint8_t port, double target){
    if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return PROS_ERR;
	}
	v5_smart_device_s_t* device = registry_get_device(port - 1);
	ERROR_IMU_STILL_CALIBRATING(port, device, PROS_ERR);
	euler_s_t euler_values;
	vexDeviceImuAttitudeGet(device->device_info, (V5_DeviceImuAttitude*)&euler_values);
	imu_data_s_t* data = (imu_data_s_t*)device->pad;
	data->yaw_offset = target - euler_values.yaw;
	if (target > IMU_EULER_LIMIT) target = IMU_EULER_LIMIT;
	if (target < -IMU_EULER_LIMIT) target = -IMU_EULER_LIMIT;
	return_port(port - 1, PROS_SUCCESS);
}

int32_t imu_set_euler(uint8_t port, euler_s_t target){
	if (!claim_port_try(port - 1, E_DEVICE_IMU)) {
		return PROS_ERR;
	}
	v5_smart_device_s_t* device = registry_get_device(port - 1);
	euler_s_t euler_values;
	vexDeviceImuAttitudeGet(device->device_info, (V5_DeviceImuAttitude*)&euler_values);
	imu_data_s_t* data = (imu_data_s_t*)device->pad;
	if (target.pitch > IMU_EULER_LIMIT) target.pitch = IMU_EULER_LIMIT;
	if (target.pitch < -IMU_EULER_LIMIT) target.pitch = -IMU_EULER_LIMIT;
	if (target.yaw > IMU_EULER_LIMIT) target.yaw = IMU_EULER_LIMIT;
	if (target.yaw < -IMU_EULER_LIMIT) target.yaw = -IMU_EULER_LIMIT;
	if (target.roll > IMU_EULER_LIMIT) target.roll = IMU_EULER_LIMIT;
	if (target.roll < -IMU_EULER_LIMIT) target.roll = -IMU_EULER_LIMIT;
	data->pitch_offset = target.pitch - euler_values.pitch;
	data->roll_offset = target.roll - euler_values.roll;
	data->yaw_offset = target.yaw - euler_values.yaw;
	return_port(port - 1, PROS_SUCCESS);
}
