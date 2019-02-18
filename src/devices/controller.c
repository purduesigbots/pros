/**
 * \file devices/controller.c
 *
 * Contains functions for interacting with the V5 Controller.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdio.h>

#include "kapi.h"
#include "v5_api.h"
#include "vdml/vdml.h"

#define CONTROLLER_MAX_COLS 15

// From enum in misc.h
#define NUM_BUTTONS 12

typedef struct controller_data {
	bool button_pressed[NUM_BUTTONS];
} controller_data_s_t;

bool get_button_pressed(int port, int button) {
	return ((controller_data_s_t*)registry_get_device_internal(port)->pad)->button_pressed[button];
}

void set_button_pressed(int port, int button, bool state) {
	controller_data_s_t* data = (controller_data_s_t*)registry_get_device_internal(port)->pad;
	data->button_pressed[button] = state;
}

int32_t controller_is_connected(controller_id_e_t id) {
	uint8_t port;
	switch (id) {
		case E_CONTROLLER_MASTER:
			port = V5_PORT_CONTROLLER_1;
			break;
		case E_CONTROLLER_PARTNER:
			port = V5_PORT_CONTROLLER_2;
			break;
		default:
			errno = EINVAL;
			return PROS_ERR;
	}
	if (!internal_port_mutex_take(port)) {
		errno = EACCES;
		return PROS_ERR;
	}
	int32_t rtn = vexControllerConnectionStatusGet(id);
	internal_port_mutex_give(port);
	return rtn;
}

int32_t controller_get_analog(controller_id_e_t id, controller_analog_e_t channel) {
	uint8_t port;
	switch (id) {
		case E_CONTROLLER_MASTER:
			port = V5_PORT_CONTROLLER_1;
			break;
		case E_CONTROLLER_PARTNER:
			port = V5_PORT_CONTROLLER_2;
			break;
		default:
			errno = EINVAL;
			return PROS_ERR;
	}
	if (!internal_port_mutex_take(port)) {
		errno = EACCES;
		return PROS_ERR;
	}
	int32_t rtn = vexControllerGet(id, channel);
	internal_port_mutex_give(port);
	return rtn;
}

int32_t controller_get_battery_capacity(controller_id_e_t id) {
	uint8_t port;
	switch (id) {
		case E_CONTROLLER_MASTER:
			port = V5_PORT_CONTROLLER_1;
			break;
		case E_CONTROLLER_PARTNER:
			port = V5_PORT_CONTROLLER_2;
			break;
		default:
			errno = EINVAL;
			return PROS_ERR;
	}
	if (!internal_port_mutex_take(port)) {
		errno = EACCES;
		return PROS_ERR;
	}
	int32_t rtn = vexControllerGet(id, BatteryCapacity);
	internal_port_mutex_give(port);
	return rtn;
}

int32_t controller_get_battery_level(controller_id_e_t id) {
	uint8_t port;
	switch (id) {
		case E_CONTROLLER_MASTER:
			port = V5_PORT_CONTROLLER_1;
			break;
		case E_CONTROLLER_PARTNER:
			port = V5_PORT_CONTROLLER_2;
			break;
		default:
			errno = EINVAL;
			return PROS_ERR;
	}
	if (!internal_port_mutex_take(port)) {
		errno = EACCES;
		return PROS_ERR;
	}
	int32_t rtn = vexControllerGet(id, BatteryLevel);
	internal_port_mutex_give(port);
	return rtn;
}

int32_t controller_get_digital(controller_id_e_t id, controller_digital_e_t button) {
	uint8_t port;
	switch (id) {
		case E_CONTROLLER_MASTER:
			port = V5_PORT_CONTROLLER_1;
			break;
		case E_CONTROLLER_PARTNER:
			port = V5_PORT_CONTROLLER_2;
			break;
		default:
			errno = EINVAL;
			return PROS_ERR;
	}
	if (!internal_port_mutex_take(port)) {
		errno = EACCES;
		return PROS_ERR;
	}
	// the buttons enum starts at 4, the correct place for the libv5rts
	int32_t rtn = vexControllerGet(id, button);
	internal_port_mutex_give(port);
	return rtn;
}

int32_t controller_get_digital_new_press(controller_id_e_t id, controller_digital_e_t button) {
	int32_t pressed = controller_get_digital(id, button);
	uint8_t port;
	switch (id) {
		case E_CONTROLLER_MASTER:
			port = V5_PORT_CONTROLLER_1;
			break;
		case E_CONTROLLER_PARTNER:
			port = V5_PORT_CONTROLLER_2;
			break;
		default:
			errno = EINVAL;
			return PROS_ERR;
	}
	if (!internal_port_mutex_take(port)) {
		errno = EACCES;
		return PROS_ERR;
	}
	uint8_t button_num = button - E_CONTROLLER_DIGITAL_L1;

	if (!pressed) {
		set_button_pressed(port, button_num, false);
	}
	if (pressed && !get_button_pressed(port, button_num)) {
		// button is currently pressed and was not detected as being pressed during
		// last check
		set_button_pressed(port, button_num, true);
		internal_port_mutex_give(port);
		return true;
	} else {
		internal_port_mutex_give(port);
		return false;  // button is not pressed or was already detected
	}
}

int32_t controller_set_text(controller_id_e_t id, uint8_t line, uint8_t col, const char* str) {
	uint8_t port;
	switch (id) {
		case E_CONTROLLER_MASTER:
			port = V5_PORT_CONTROLLER_1;
			break;
		case E_CONTROLLER_PARTNER:
			port = V5_PORT_CONTROLLER_2;
			break;
		default:
			errno = EINVAL;
			return PROS_ERR;
	}
	if (!internal_port_mutex_take(port)) {
		errno = EACCES;
		return PROS_ERR;
	}
	line++;
	if (col >= CONTROLLER_MAX_COLS)
		col = CONTROLLER_MAX_COLS;
	else
		col++;

	char* buf = strndup(str, CONTROLLER_MAX_COLS + 1);

	vexControllerTextSet(id, line, col, buf);
	free(buf);
	internal_port_mutex_give(port);
	return 1;
}

int32_t controller_print(controller_id_e_t id, uint8_t line, uint8_t col, const char* fmt, ...) {
	uint8_t port;
	switch (id) {
		case E_CONTROLLER_MASTER:
			port = V5_PORT_CONTROLLER_1;
			break;
		case E_CONTROLLER_PARTNER:
			port = V5_PORT_CONTROLLER_2;
			break;
		default:
			errno = EINVAL;
			return PROS_ERR;
	}
	if (!internal_port_mutex_take(port)) {
		errno = EACCES;
		return PROS_ERR;
	}
	line++;
	if (col >= CONTROLLER_MAX_COLS)
		col = CONTROLLER_MAX_COLS;
	else
		col++;

	va_list args;
	va_start(args, fmt);
	char* buf = (char*)malloc(CONTROLLER_MAX_COLS + 1);
	vsnprintf(buf, CONTROLLER_MAX_COLS + 1, fmt, args);

	vexControllerTextSet(id, line, col, buf);
	free(buf);
	va_end(args);

	internal_port_mutex_give(port);
	return 1;
}

int32_t controller_clear_line(controller_id_e_t id, uint8_t line) {
	uint8_t port;
	switch (id) {
		case E_CONTROLLER_MASTER:
			port = V5_PORT_CONTROLLER_1;
			break;
		case E_CONTROLLER_PARTNER:
			port = V5_PORT_CONTROLLER_2;
			break;
		default:
			errno = EINVAL;
			return PROS_ERR;
	}
	if (!internal_port_mutex_take(port)) {
		errno = EACCES;
		return PROS_ERR;
	}
	line++;

	const char* const blank = "                 ";
	vexControllerTextSet(port, line, 0, blank);
	internal_port_mutex_give(port);
	return 1;
}

int32_t controller_clear(controller_id_e_t id) {
	for (int i = 0; i < 3; i++) controller_clear_line(id, i);
	return 1;
}

int32_t controller_rumble(controller_id_e_t id, const char* rumble_pattern) {
	return controller_set_text(id, 3, 0, rumble_pattern);
}

uint8_t competition_get_status(void) {
	return vexCompetitionStatus();
}
