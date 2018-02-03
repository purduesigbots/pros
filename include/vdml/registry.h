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
 * \brief Checks whether there is a mismatch between the device registered in
 * 			a port and the device actually in that port
 *
 * Check what device type is registered in the given port and compares it to the
 * device actually plugged into that port.
 *
 * \param[in]	port	the port number to check
 *
 * \return 1 if the device registered matches the actual device, 0 if it doesnt,
 *           and PROS_ERR if an error occurs
 *
 * \exception EINVAL the port number is out of range
 */
int32_t registry_validate_binding(unsigned int port);

#ifdef __cplusplus
}
#endif

#endif /*REGISTRY_H*/
