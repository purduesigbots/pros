/**
 * \file pros/device.h
 *
 * Contains functions for interacting with VEX devices.
 *
 *
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2021, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_DEVICE_H_
#define _PROS_DEVICE_H_

#include <stdint.h>

#ifdef __cplusplus
namespace pros::c {
extern "C" {
#endif

/*
 * List of possible v5 devices
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
	E_DEVICE_SERIAL = 129,
	E_DEVICE_GENERIC __attribute__((deprecated("use E_DEVICE_SERIAL instead"))) = E_DEVICE_SERIAL,
	E_DEVICE_UNDEFINED = 255
} v5_device_e_t;

/**
 * Gets the type of device on given port.
 *
 * \return The device type as an enum.
*/
v5_device_e_t get_plugged_type(uint8_t port);

#ifdef __cplusplus
} // namespace c
} // namespace pros
#endif

#endif // _PROS_DEVICE_H_
