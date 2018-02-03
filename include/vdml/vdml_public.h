/**
 * \file vdml_public.h
 *
 * \brief Public api declarations for vdml and registry functions
 *
 * \copyright (c) 2017-2018, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef VDML_P_H
#define VDML_P_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * \brief List of possible v5 devices
 *
 * This list contains all current V5 Devices, and mirrors V5_DeviceType from the
 * api.
 */
typedef enum v5_device_e {
	E_DEVICE_NONE = 0,
	E_DEVICE_MOTOR = 2,
	E_DEVICE_LED = 3,
	E_DEVICE_RGB = 4,
	E_DEVICE_BUMPER = 5,
	E_DEVICE_IMU = 6,
	E_DEVICE_RANGE = 7,
	E_DEVICE_RADIO = 8,
	E_DEVICE_TETHER = 9,
	E_DEVICE_BRAIN = 10,
	E_DEVICE_VISION = 11,
	E_DEVICE_ADI = 12,
	E_DEVICE_GYRO = 0x46,
	E_DEVICE_SONAR = 0x47,
	E_DEVICE_GENERIC = 128,
	E_DEVICE_UNDEFINED = 255
} v5_device_e;

/*
 * \brief Registers a device in the given port
 *
 * Registers a device of the given type in the given port into the registry, if
 * that type of device is detected to be plugged in to that port.
 *
 * \param[in]  port             the port number to register the device
 * \param[in]  device			the type of device to register
 *
 * \return 1 upon success, PROS_ERR upon failure
 *
 * \exception EINVAL Port number is out of range.
 * \exception EINVAL A different device than specified is plugged in.
 * \exception EADDRINUSE the port is already registered to another device
 */
int registry_bind_port(unsigned int port, v5_device_e device_type);

/*
 * \brief Deregisters a devices from the given port
 *
 * Removes the device registed in the given port, if there is one.
 *
 * \param[in]  port      		the port number to deregister
 *
 * \return 1 upon success, PROS_ERR upon failure
 *
 * \exception EINVAL the port number is out of range
 */
int registry_unbind_port(unsigned int port);

/*
 * \brief Checks whether there is a device mismatch among all registered ports.
 *
 * Check what device type is registered in each port and compares it to the
 * device actually plugged into each port. An error is displayed upon detection
 */
void registry_validate_all_bindings();

#ifdef __cplusplus
}
#endif  // cplusplus

#endif  // VDML_P_H
