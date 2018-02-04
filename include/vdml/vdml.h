/**
 * \file vdml.h
 *
 * \brief VDML Type Declarations and Constant Declarations
 *
 * This file contains all types and functions used throughout multiple VDML
 * (Vex Data Management Layer) files, as well as defines for any constants used
 * by VDML.
 *
 * \copyright (c) 2017-2018, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef VDML_H
#define VDML_H

/**
 * Macro, returns true if the port is in range, false otherwise.
 */
#define VALIDATE_PORT_NO(PORT) ((PORT) >= 0 && (PORT) < NUM_V5_PORTS)

/**
 * Macro that handles error checking, sanity checking, automatic registration,
 * and mutex taking for all of the motor wrapper functions.
 * If port is out of range, the calling function sets errno and returns.
 * If a port isn't yet registered, it registered as a motor automatically.
 * If a mutex cannot be taken, errno is set to EACCES (access denied) and returns.
 */
#define claim_port(port, new_device_type)                                                                              \
	if (!VALIDATE_PORT_NO(port)) {                                                                                 \
		errno = EINVAL;                                                                                        \
		return PROS_ERR;                                                                                       \
	}                                                                                                              \
	v5_smart_device_s_t device = registry_get_device(port);                                                        \
	if (device.device_type == E_DEVICE_NONE) {                                                                     \
		/* Register the device as a motor */                                                                   \
		kprintf("[VDML][INFO]Registering device on port %d\n", port + 1);                                      \
		registry_bind_port(port, new_device_type);                                                             \
		device = registry_get_device(port);                                                                    \
	}                                                                                                              \
	if (!port_mutex_take(port)) {                                                                                  \
		errno = EACCES;                                                                                        \
		return PROS_ERR;                                                                                       \
	}

/**
 * Macro that release the mutex for the given port and sets errno to 0 if the
 * function is an accessor wrapper whos return value is PROS_ERR or PROS_ERR_F.
 */
#define return_port(port, rtn)                                                                                         \
	port_mutex_give(port);                                                                                         \
	if (rtn == PROS_ERR || rtn == PROS_ERR_F) errno = 0;                                                           \
	return rtn;

/**
 * \brief Claims the mutex for the given port.
 *
 * Reserves the mutex for this port. Any other tasks trying to access this port
 * will block until the mutex is returned. If a higher-priortiy task attempts
 * to claim this port, the task which has the port claimed will temporarily be
 * raised to an equal priority until the mutex is given, reducing the impact of
 * the delay. See FreeRTOS documentation for more details.
 *
 * This MUST be called before any call to the v5 api to maintain thread saftey.
 *
 * \param[in] port	The port number to claim.
 *
 * \return 1 if the mutex was successfully taken, 0 if not, -1 if port is invalid.
 *
 * \exception EINVAL Port is out of range.
 */
int port_mutex_take(int port);

/**
 * \brief Returns the mutex for the given port.
 *
 * Frees the mutex for this port, allowing other tasks to continue.
 *
 * WARNING: If a mutex was claimed by a task, this MUST be called immediately
 * after the port is no longer needed by that task in order to prevent delays in
 * other tasks.
 *
 * \param[in] port The port number to free.
 *
 * \exception EINVAL Port is out of range.
 */
int port_mutex_give(int port);

#endif /*VDML_H*/
