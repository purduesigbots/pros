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

/**
 * \brief Background processing function for the VDML system.
 *
 * This function should be called by the system daemon approximately every
 * 2 milliseconds.
 *
 * Updates the registry type array, detecting what devices are actually
 * plugged in according to the system, then compares that with the registry
 * records. For each port and condition, the following actions are performed:
 *
 * |      Type      |
 * |Actual |Registry|    |          Action          |
 * |None   |None    | -> |Nothing                   |
 * |Type A |None    | -> |Register Type A           |
 * |None   |Type A  | -> |Warn of unplugged device  |
 * |Type A |Type A  | -> |Nothing                   |
 * |Type A |Type B  | -> |Warn of mismatched device |
 *
 * On warnings, no operation is performed except for a printf. A printf notifes
 * the user that a device is being automatically registered.
 */
void vdml_background_processing() {
	// Refresh actual device types
	registry_update_types();

	// Validate the ports. Warn if mismatch.
	for (int i = 0; i < NUM_V5_PORTS; i++) {
		// Take the mutex for the port
		port_mutex_take(i);
		if (!registry_validate_binding(i)) {
			// If what's registered is different from what's plugged in...
			v5_device_e registered_t = registry_get_bound_type(i);
			v5_device_e actual_t = registry_get_plugged_type(i);

			if (registered_t == E_DEVICE_NONE) {
				// Device isn't registered, but stuff is plugged in.
				// Auto register.
				kprintf("[VDML-BG][INFO] Auto-register port %d\n", i++);
				registry_bind_port(i, actual_t);
			} else if (actual_t == E_DEVICE_NONE) {
				// Device is registered, but nothing is plugged in.
				// Print warning.
				kprintf("[VDML][WARNING] No device in port %d\n", i++);
			} else {
				// One device is registered, but another is plugged in.
				// Print warning.
				kprintf("[VDML][ERROR] Device type mismatch in port %d\n", i++);
			}
		}
		port_mutex_give(i);
	}
}
