/**
 * \file vdml/vdml.h
 *
 * This file contains all types and functions used throughout multiple VDML
 * (Vex Data Management Layer) files.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "vdml/registry.h"

/**
 * Macro, returns true if the port is in range of user configurable ports,
 * false otherwise.
 */
#define VALIDATE_PORT_NO(PORT) ((PORT) >= 0 && (PORT) < NUM_V5_PORTS)

#define VALIDATE_PORT_NO_INTERNAL(PORT) ((PORT) >= 0 && (PORT) < V5_MAX_DEVICE_PORTS)

/**
 * Macro that handles error checking, sanity checking, automatic registration,
 * and mutex taking for all of the motor wrapper functions.
 * If port is out of range, the calling function sets errno and returns.
 * If a port isn't yet registered, it registered as a motor automatically.
 * If a mutex cannot be taken, errno is set to EACCES (access denied) and
 * returns.
 *
 * \param port
 *        The V5 port number from 0-20
 * \param device_type
 *        The v5_device_e_t that the port is configured as
 * \param error_code
 *        The error code that return if error checking failed
 */
#define claim_port(port, device_type, error_code)          \
  if (registry_validate_binding(port, device_type) != 0) { \
    return error_code;                                     \
  }                                                        \
  v5_smart_device_s_t* device = registry_get_device(port); \
  if (!port_mutex_take(port)) {                            \
    errno = EACCES;                                        \
    return error_code;                                     \
  }

/**
 * Function like claim_port. This macro should only be used in functions 
 * that return int32_t or enums as PROS_ERR could be returned.
 *
 * \param port
 *        The V5 port number from 0-20
 * \param device_type
 *        The v5_device_e_t that the port is configured as
 */
#define claim_port_i(port, device_type) claim_port(port, device_type, PROS_ERR)

/**
 * Function like claim_port. This macro should only be used in functions 
 * that return double or float as PROS_ERR_F could be returned.
 *
 * \param port
 *        The V5 port number from 0-20
 * \param device_type
 *        The v5_device_e_t that the port is configured as
 */
#define claim_port_f(port, device_type) claim_port(port, device_type, PROS_ERR_F)

/**
 * A function that executes claim_port and allows you to execute a block of 
 * code if an error occurs.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 0-20
 * \param device_type
 *        The v5_device_e_t that the port is configured as
 *
 * \return 1 if the operation was successful or 0 if the operation
 * failed, setting errno.
 */
int32_t claim_port_try(uint8_t port, v5_device_e_t type);

/**
 * Macro that release the mutex for the given port and sets errno to 0 if the
 * function is an accessor wrapper whos return value is PROS_ERR or PROS_ERR_F.
 *
 * \param port
 *        The V5 port number from 0-20
 * \param rtn
 *        The desired return value
 *
 * \return The rtn parameter
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
 * \param port
 *        The V5 port number to set from 0-20
 */
void vdml_set_port_error(uint8_t port);

/**
 * Sets the port's bit to 0, effectively resetting it.
 *
 * \param port
 *        The V5 port number to unset from 0-20
 */
void vdml_unset_port_error(uint8_t port);

/**
 * Gets the error bit for the port, indicating whether or not there has been an
 * error on this port.
 *
 * \param port
 *        The V5 port number to check from 0-20
 *
 * \return True if the port's bit is set, false otherwise.
 */
bool vdml_get_port_error(uint8_t port);

/**
 * Reset all ports' error bits.
 */
void vdml_reset_port_error();

/**
 * Claims the mutex for the given port.
 *
 * Reserves the mutex for this port. Any other tasks trying to access this port
 * will block until the mutex is returned. If a higher-priortiy task attempts
 * to claim this port, the task which has the port claimed will temporarily be
 * raised to an equal priority until the mutex is given, reducing the impact of
 * the delay. See FreeRTOS documentation for more details.
 *
 * This MUST be called before any call to the v5 api to maintain thread saftey.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (0-20).
 *
 * \param port
 *        The V5 port number to claim from 0-20
 *
 * \return 1 if the mutex was successfully taken, 0 if not, -1 if port is
 * invalid.
 */
int port_mutex_take(uint8_t port);

/**
 * Returns the mutex for the given port.
 *
 * Frees the mutex for this port, allowing other tasks to continue.
 *
 * WARNING: If a mutex was claimed by a task, this MUST be called immediately
 * after the port is no longer needed by that task in order to prevent delays in
 * other tasks.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (0-20).
 *
 * \param port
 *        The V5 port number to free from 0-20
 */
int port_mutex_give(uint8_t port);

/**
 * Executes port_mutex_take() for all of the V5 Smart Ports.
 */
void port_mutex_take_all();

/**
 * Executes port_mutex_give() for all of the V5 Smart Ports.
 */
void port_mutex_give_all();

/**
 * Obtains a port mutex with bounds checking for V5_MAX_PORTS (32) not user
 * exposed device ports (20). Intended for internal usage for protecting
 * thread-safety on devices such as the controller and battery
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (0-32).
 *
 * \param port
 *        The V5 port number from 0-32
 *
 * \return True if the mutex was successfully taken, false otherwise. If false
 * is returned, then errno is set with a hint about why the the mutex
 * couldn't be taken.
 */
int internal_port_mutex_take(uint8_t port);

/**
 * Returns a port mutex with bounds checking for V5_MAX_PORTS (32) not user
 * exposed device ports (20). Intended for internal usage for protecting
 * thread-safety on devices such as the controller and battery
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (0-32).
 *
 * \param port
 *        The V5 port number from 0-32
 *
 * \return True if the mutex was successfully returned, false otherwise. If
 * false is returned, then errno is set with a hint about why the mutex
 * couldn't be returned.
 */
int internal_port_mutex_give(uint8_t port);

#define V5_PORT_BATTERY 24
#define V5_PORT_CONTROLLER_1 25
#define V5_PORT_CONTROLLER_2 26
