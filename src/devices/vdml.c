/**
 * \file devices/vdml.c
 *
 * VDML - VEX Data Management Layer
 *
 * VDML ensures thread saftey for operations on smart devices by maintaining
 * an array of RTOS Mutexes and implementing functions to take and give them.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "vdml/vdml.h"
#include "kapi.h"
#include "v5_api.h"
#include "vdml/registry.h"

#include <errno.h>
#include <stdio.h>

/**
 * Bitmap to indicate if a port has had an error printed or not.
 */
int32_t port_errors;

extern void registry_init();
extern void port_mutex_init();

int32_t claim_port_try(uint8_t port, v5_device_e_t type) {
	if (!VALIDATE_PORT_NO(port)) {
		errno = ENXIO;
		return 0;
	}
	if (registry_validate_binding(port, type) != 0) {
		return 0;
	}
	if (!port_mutex_take(port)) {
		errno = EACCES;
		return 0;
	}
	return 1;
}

/**
 * We have V5_MAX_DEVICE_PORTS so that we can do thread safety on things like
 * controllers, batteries which are sort of like smart devices internally to the
 * V5
 */
mutex_t port_mutexes[V5_MAX_DEVICE_PORTS];            // Mutexes for each port
static_sem_s_t port_mutex_bufs[V5_MAX_DEVICE_PORTS];  // Stack mem for rtos

/**
 * Shorcut to initialize all of VDML (mutexes and register)
 */
void vdml_initialize() {
	port_mutex_init();
	registry_init();
}

/**
 * Initializes the mutexes for the motor ports.
 *
 * Initializes a static array of FreeRTOS mutexes to protect against race
 * conditions. For example, we don't want the Background processing task to run
 * at the same time that we set a motor, because bad information may be
 * returned, or worse.
 */
void port_mutex_init() {
	for (int i = 0; i < V5_MAX_DEVICE_PORTS; i++) {
		port_mutexes[i] = mutex_create_static(&(port_mutex_bufs[i]));
	}
}

int port_mutex_take(uint8_t port) {
	if (port >= V5_MAX_DEVICE_PORTS) {
		errno = ENXIO;
		return PROS_ERR;
	}
	return xTaskGetSchedulerState() != taskSCHEDULER_RUNNING || mutex_take(port_mutexes[port], TIMEOUT_MAX);
}

int internal_port_mutex_take(uint8_t port) {
	if (port >= V5_MAX_DEVICE_PORTS) {
		errno = ENXIO;
		return PROS_ERR;
	}
	return mutex_take(port_mutexes[port], TIMEOUT_MAX);
}

static inline char* print_num(char* buff, int num) {
	*buff++ = (num / 10) + '0';
	*buff++ = (num % 10) + '0';
	return buff;
}

int port_mutex_give(uint8_t port) {
	if (port >= V5_MAX_DEVICE_PORTS) {
		errno = ENXIO;
		return PROS_ERR;
	}
	return xTaskGetSchedulerState() != taskSCHEDULER_RUNNING || mutex_give(port_mutexes[port]);
}

int internal_port_mutex_give(uint8_t port) {
	if (port >= V5_MAX_DEVICE_PORTS) {
		errno = ENXIO;
		return PROS_ERR;
	}
	return mutex_give(port_mutexes[port]);
}

void port_mutex_take_all() {
	for (int i = 0; i < V5_MAX_DEVICE_PORTS; i++) {
		port_mutex_take(i);
	}
}

void port_mutex_give_all() {
	for (int i = 0; i < V5_MAX_DEVICE_PORTS; i++) {
		port_mutex_give(i);
	}
}

void vdml_set_port_error(uint8_t port) {
	if (VALIDATE_PORT_NO(port)) {
		port_errors |= (1 << port);
	}
}

void vdml_unset_port_error(uint8_t port) {
	if (VALIDATE_PORT_NO(port)) {
		port_errors &= ~(1 << port);
	}
}

bool vdml_get_port_error(uint8_t port) {
	if (VALIDATE_PORT_NO(port)) {
		return (port_errors >> port) & 1;
	} else {
		return false;
	}
}

void vdml_reset_port_error() {
	port_errors = 0;
}

/**
 * Background processing function for the VDML system.
 *
 * This function should be called by the system daemon approximately every
 * 2 milliseconds.
 *
 * Updates the registry type array, detecting what devices are actually
 * plugged in according to the system, then compares that with the registry
 * records.
 *
 * On warnings, no operation is performed.
 */
void vdml_background_processing() {
	static int32_t last_port_errors = 0;
	static int cycle = 0;
	cycle++;
	if (cycle % 5000 == 0) {
		vdml_reset_port_error();
		last_port_errors = 0;
	}

	// Refresh actual device types
	registry_update_types();

	// Validate the ports. Warn if mismatch.
	uint8_t error_arr[NUM_V5_PORTS];
	int num_errors = 0;
	int mismatch_errors = 0;
	for (int i = 0; i < NUM_V5_PORTS; i++) {
		error_arr[i] = registry_validate_binding(i, E_DEVICE_NONE);
		if (error_arr[i] != 0) num_errors++;
		if (error_arr[i] == 2) mismatch_errors++;
	}
	// Every 50 ms
	if (cycle % 50 == 0) {
		if (last_port_errors == port_errors) {
			goto end_render_errors;
		}
		char line[50];
		char* line_ptr = line;
		if (num_errors == 0)
			line[0] = (char)0;
		else if (num_errors <= 6) {
			// If we have 1-6 total errors (unplugged + mismatch), we can
			// display a line indicating the ports where these errors occur
			strcpy(line_ptr, "PORTS");
			line_ptr += 5;  // 5 is length of "PORTS"
			if (mismatch_errors != 0) {
				strcpy(line_ptr, " MISMATCHED: ");
				line_ptr += 13;  // 13 is length of previous string
				for (int i = 0; i < NUM_V5_PORTS; i++) {
					if (error_arr[i] == 2) {
						line_ptr = print_num(line_ptr, i + 1);
						*line_ptr++ = ',';
					}
				}
				line_ptr--;
			}
			if (num_errors != mismatch_errors) {
				strcpy(line_ptr, " UNPLUGGED: ");
				line_ptr += 12;  // 12 is length of previous string
				for (int i = 0; i < NUM_V5_PORTS; i++) {
					if (error_arr[i] == 1) {
						line_ptr = print_num(line_ptr, i + 1);
						*line_ptr++ = ',';
					}
				}
				line_ptr--;
			}
		} else {
			/* If we have > 6 errors, we display the following:
			 * PORT ERRORS: 1..... 6..... 11..... 16.....
			 * where each . represents a port. A '.' indicates
			 * there is no error on that port, a 'U' indicates
			 * the registry expected a device there but there isn't
			 * one, and a 'M' indicates the plugged in devices doesn't
			 * match what we expect. The numbers are just a visual reference
			 * to aid in determining what ports have errors.
			 */
			strcpy(line_ptr, "PORT ERRORS:");
			line_ptr += 12;  // 12 is length of previous string
			for (int i = 0; i < NUM_V5_PORTS; i++) {
				if (i % 5 == 0) {
					*line_ptr++ = ' ';
					line_ptr = print_num(line_ptr, i + 1);
				}
				switch (error_arr[i]) {
					case 0:
						*line_ptr++ = '.';
						break;
					case 1:
						*line_ptr++ = 'U';
						break;
					case 2:
						*line_ptr++ = 'M';
						break;
					// Should never happen
					default:
						*line_ptr++ = '?';
						break;
				}
			}
		}
		// Null terminate the string
		*line_ptr = '\0';
		display_error(line);
	end_render_errors:
		last_port_errors = port_errors;
	}
}
