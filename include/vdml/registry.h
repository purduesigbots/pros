/**
 * \file registry.h
 *
 * \brief Contains constant, type, and funciton declarations for the VDML Registry
 *
 * This file contains the standard header info for the VDML (Vex Data Management Layer)
 * registry, including constant definitions/defines, type definitions, and forward
 * function declarations.
 *
 * \copyright (c) 2017-2018, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef REGISTRY_H
#define REGISTRY_H

#include "ifi/v5_api.h"
#include "vdml/vdml.h"
#include "vdml/vdml_public.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	v5_device_e device_type;
	V5_DeviceT device_info;
} v5_smart_device_s_t;

/*
 * \brief Detects the devices that are plugged in.
 *
 * Wrapper function for vexDeviceGetStatus.
 * Pulls the type names of plugged-in devices and stores them in the buffer
 * registry_types.
 */
void registry_update_types();

/*
 * \brief Returns the information on the device registered to the port.
 *
 * \param[in] port the port to check
 *
 * \return A struct containing the device type and the info needed for api
 *         functions
 *
 * \except EINVAL the port number is out of range.
 */
v5_smart_device_s_t registry_get_device(unsigned int port);

/*
 * \brief Returns the type of device registered to the port.
 *
 * \param[in] port the port to check
 *
 * \return The type of device that is registered into the port (NOT what is
 *         plugged in)
 *
 * \exception EINVAL the port number is out of range
 */
v5_device_e registry_get_bound_type(unsigned int port);

/*
 * \brief Returns the type of the device plugged into the port.
 *
 * \param[in] port the port to check
 *
 * \return The type of device that is plugged into the port (NOT what is
 *         registered)
 *
 * \exception EINVAL the port number is out of range.
 */
v5_device_e registry_get_plugged_type(unsigned int port);

/*
 * \brief Checks whether there is a discrepancy between the binding of the port
 *        and what is actually plugged in.
 *
 * If a device is plugged in but not registered, registers the port.
 * If a device is not plugged in and a device is registered, warns the user.
 * If one type of device is registered but another is plugged in, warns the user.
 *
 * \param[in] port the port number to check
 * \param[in] expected_t the expected type (i.e., the type of function being
 *            called. If E_DEVICE_NONE, indicates that background processing is
 *            calling this, and a mismatch will only occur if there is an actual
 *            discrepancy between what is registered and what is plugged in.
 *
 * \return 0 if the device registered matches the device plugged and the
 *         expected device matches both of those or is E_DEVICE_NONE, 1 if the
 *         a registered device is not plugged in, and 2 if there is a mismatch.
 *         PROS_ERR on exception.
 *
 * \exception EINVAL the port number is out of range
 */
int32_t registry_validate_binding(unsigned int port, v5_device_e expected_t);

#ifdef __cplusplus
}
#endif

#endif /*REGISTRY_H*/
