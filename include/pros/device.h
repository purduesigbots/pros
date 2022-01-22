/**
 * \file pros/device.h
 *
 * Contains prototypes for interfacing with the VEX Smart Device.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/index.html to learn more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * Copyright (c) 2017-2022, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_DEVICE_H_
#define _PROS_DEVICE_H_

#include <stdbool.h>
#include <stdint.h>
#ifndef PROS_ERR
#define PROS_ERR (INT32_MAX)
#endif

#ifdef __cplusplus
extern "C" {
namespace pros {
namespace c {
#endif

/*
 * Represents the port type for a V5 Device.
 *
 * This list contains all current V5 Devices, and mirrors V5_DeviceType from the
 * api.
 */
typedef enum v5_device_e {
	E_DEVICE_NONE = 0,

	E_DEVICE_MOTOR = 2,
	E_DEVICE_ROTATION = 4,
	E_DEVICE_IMU = 6,
	E_DEVICE_DISTANCE = 7,
	E_DEVICE_RADIO = 8,
	E_DEVICE_VISION = 11,
	E_DEVICE_ADI = 12,
	E_DEVICE_OPTICAL = 16,
	E_DEVICE_GPS = 20,

	E_DEVICE_GENERIC = 129,

	E_DEVICE_UNDEFINED = 255,
	E_DEVICE_ERR = PROS_ERR
} v5_device_e_t;

#ifdef PROS_USE_SIMPLE_NAMES
#ifdef __cplusplus
#define DEVICE_NONE pros::c::E_DEVICE_NONE
#define DEVICE_MOTOR pros::c::E_DEVICE_MOTOR
#define DEVICE_ROTATION pros::c::E_DEVICE_ROTATION
#define DEVICE_IMU pros::c::E_DEVICE_IMU
#define DEVICE_DISTANCE pros::c::E_DEVICE_DISTANCE
#define DEVICE_RADIO pros::c::E_DEVICE_RADIO
#define DEVICE_VISION pros::c::E_DEVICE_VISION 
#define DEVICE_ADI pros::c::E_DEVICE_ADI 
#define DEVICE_OPTICAL pros::c::E_DEVICE_OPTICAL 
#define DEVICE_GPS pros::c::E_DEVICE_GPS 
#define DEVICE_GENERIC pros::c::E_DEVICE_GENERIC
#define DEVICE_UNDEFINED pros::c::E_DEVICE_UNDEFINED
#define DEVICE_ERR pros::c::E_DEVICE_ERR
#else
#define DEVICE_NONE E_DEVICE_NONE
#define DEVICE_MOTOR E_DEVICE_MOTOR
#define DEVICE_ROTATION E_DEVICE_ROTATION
#define DEVICE_IMU E_DEVICE_IMU
#define DEVICE_DISTANCE E_DEVICE_DISTANCE
#define DEVICE_RADIO E_DEVICE_RADIO
#define DEVICE_VISION E_DEVICE_VISION 
#define DEVICE_ADI E_DEVICE_ADI 
#define DEVICE_OPTICAL E_DEVICE_OPTICAL 
#define DEVICE_GPS E_DEVICE_GPS 
#define DEVICE_GENERIC E_DEVICE_GENERIC
#define DEVICE_UNDEFINED E_DEVICE_UNDEFINED
#define DEVICE_ERR E_DEVICE_ERR
#endif
#endif

#ifdef __cplusplus
}  // namespace c
}  // namespace pros
}
#endif

#endif  // _PROS_DEVICE_H_