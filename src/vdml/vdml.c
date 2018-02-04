#include "vdml/vdml.h"
#include "ifi/v5_api.h"
#include "kapi.h"
#include "vdml/registry.h"
#include "vdml/vdml_public.h"
#include <errno.h>
#include <stdio.h>

extern void registry_init();
extern void port_mutex_init();

mutex_t port_mutexes[NUM_V5_PORTS];            // Mutexes for each port
static_sem_s_t port_mutex_bufs[NUM_V5_PORTS];  // Stack mem for rtos

/**
 * \brief Shorcut to initialize all of VDML (mutexes and register)
 */
void vdml_initialize() {
	port_mutex_init();
	registry_init();
}

/**
 * \brief Initializes the mutexes for the motor ports.
 *
 * Initializes a static array of FreeRTOS mutexes to protect against race
 * conditions. For example, we don't want the Background processing task to run
 * at the same time that we set a motor, because bad information may be
 * returned, or worse.
 */
void port_mutex_init() {
	for (int i = 0; i < NUM_V5_PORTS; i++) {
		port_mutexes[i] = mutex_create_static(&(port_mutex_bufs[i]));
	}
}

int port_mutex_take(int port) {
	if (port < 0 || port > NUM_V5_PORTS) {
		errno = EINVAL;
		return PROS_ERR;
	}
	return mutex_take(port_mutexes[port], TIMEOUT_MAX);
}

int port_mutex_give(int port) {
	if (port < 0 || port > NUM_V5_PORTS) {
		errno = EINVAL;
		return PROS_ERR;
	}
	return mutex_give(port_mutexes[port]);
}

void port_mutex_take_all() {
	for (int i = 0; i < NUM_V5_PORTS; i++) {
		port_mutex_take(i);
	}
}

void port_mutex_give_all() {
	for (int i = 0; i < NUM_V5_PORTS; i++) {
		port_mutex_give(i);
	}
}

void vdml_set_port_error(int port) {
	if (VALIDATE_PORT_NO(port)) {
		port_errors |= (1 << port);
	}
}

void vdml_unset_port_error(int port) {
	if (VALIDATE_PORT_NO(port)) {
		port_errors &= ~(1 << port);
	}
}

bool vdml_get_port_error(int port) {
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
 * \brief Background processing function for the VDML system.
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
int cycle = 0;
void vdml_background_processing() {
	cycle++;
	if (cycle % 5000 == 0) {
		vdml_reset_port_error();
	}

	// Refresh actual device types
	registry_update_types();

	// Validate the ports. Warn if mismatch.
	for (int i = 0; i < NUM_V5_PORTS; i++) {
		registry_validate_binding(i, E_DEVICE_NONE);
	}
}
