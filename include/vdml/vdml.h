/**
 * \file vdml.h
 *
 * \brief VDML Type Declarations and Constant Declarations
 *
 * This file contains all types and functions used throughout multiple VDML
 * (Vex Data Management Layer) files, as well as defines for any constants used
 * by VDML.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef VDML_H
#define VDML_H

#include <stdbool.h>
#include <stdint.h>
#include "vdml/registry.h"

/**
 * Macro, returns true if the port is in range, false otherwise.
 */
#define VALIDATE_PORT_NO(PORT) ((PORT) >= 0 && (PORT) < NUM_V5_PORTS)

/**
 * Macro that handles error checking, sanity checking, automatic registration,
 * and mutex taking for all of the motor wrapper functions.
 * If port is out of range, the calling function sets errno and returns.
 * If a port isn't yet registered, it registered as a motor automatically.
 * If a mutex cannot be taken, errno is set to EACCES (access denied) and
 * returns.
 */
#define claim_port(port, device_type)                      \
	if (!VALIDATE_PORT_NO(port)) {                           \
		errno = EINVAL;                                        \
		return PROS_ERR;                                       \
	}                                                        \
	if (registry_validate_binding(port, device_type) != 0) { \
		errno = EINVAL;                                        \
		return PROS_ERR;                                       \
	}                                                        \
	v5_smart_device_s_t* device = registry_get_device(port); \
	if (!port_mutex_take(port)) {                            \
		errno = EACCES;                                        \
		return PROS_ERR;                                       \
	}

/**
 * A function that executes claim_port for functions that do not return an
 * int32_t
 *
 * Returns 1 upon success, PROS_ERR upon failure
 */
int32_t claim_port_try(uint8_t port, v5_device_e_t type);

/**
 * Macro that release the mutex for the given port and sets errno to 0 if the
 * function is an accessor wrapper whos return value is PROS_ERR or PROS_ERR_F.
 */
#define return_port(port, rtn) \
	port_mutex_give(port);       \
	return rtn;

/**
 * Bitmap to indicate if a port has had an error printed or not.
 */
extern int32_t port_errors;

/**
 * Sets the port's bit to 1, indicating there has already been an error on this
 * port.
 *
 * \param[in] port The port to set.
 */
void vdml_set_port_error(uint8_t port);

/**
 * Sets the port's bit to 0, effectively resetting it.
 *
 * \param[in] port The port to unset.
 */
void vdml_unset_port_error(uint8_t port);

/**
 * Gets the error bit for the port, indicating whether or not there has been an
 * error on this port.
 *
 * \param[in] port The port to check
 *
 * \return True if the port's bit is set, false otherwise.
 */
bool vdml_get_port_error(uint8_t port);

/**
 * Reset all ports' error bits.
 */
void vdml_reset_port_error();

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
 * \return 1 if the mutex was successfully taken, 0 if not, -1 if port is
 * invalid.
 *
 * \exception EINVAL Port is out of range.
 */
int port_mutex_take(uint8_t port);

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
int port_mutex_give(uint8_t port);

void port_mutex_take_all();

void port_mutex_give_all();

/**
 * Obtains a port mutex with bounds checking for V5_MAX_PORTS (32) not user
 * exposed
 * device ports (22). Intended for internal usage for protecting thread-safety
 * on
 * devices such as the controller and battery
 */
int internal_port_mutex_take(uint8_t port);

/**
 * Returns a port mutex with bounds checking for V5_MAX_PORTS (32) not user
 * exposed
 * device ports (22). Intended for internal usage for protecting thread-safety
 * on
 * devices such as the controller and battery
 */
int internal_port_mutex_give(uint8_t port);

#define V5_PORT_BATTERY 24
#define V5_PORT_CONTROLLER_1 25
#define V5_PORT_CONTROLLER_2 26

#endif /*VDML_H*/
