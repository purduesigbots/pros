/**
 * \file devices/vdml_serial.c
 *
 * Contains functions for interacting with V5 Generic Serial devices.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <errno.h>
#include <math.h>
#include <stdio.h>

#include "kapi.h"
#include "pros/serial.h"
#include "v5_api.h"
#include "vdml/registry.h"
#include "vdml/vdml.h"

// Control function

int32_t serial_enable(uint8_t port) {
	/**
	 * claim_port_i(port - 1, E_DEVICE_SERIAL) is not used because it requires
	 * the port to already be of the requested type in VEXos, which will not yet
	 * be the case for generic serial as vexDeviceGenericSerialEnable is what
	 * switches the port into the correct mode
	 */
	if (!VALIDATE_PORT_NO(port - 1)) {
		errno = EINVAL;
		return PROS_ERR;
	}
	v5_smart_device_s_t* device = registry_get_device(port - 1);
	if (!port_mutex_take(port - 1)) {
		errno = EACCES;
		return PROS_ERR;
	}
	vexDeviceGenericSerialEnable(device->device_info, 0);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t serial_set_baudrate(uint8_t port, int32_t baudrate) {
	claim_port_i(port - 1, E_DEVICE_SERIAL);
	vexDeviceGenericSerialBaudrate(device->device_info, baudrate);
	return_port(port - 1, PROS_SUCCESS);
}

int32_t serial_flush(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_SERIAL);
	vexDeviceGenericSerialFlush(device->device_info);
	return_port(port - 1, PROS_SUCCESS);
}

// Telemetry functions

int32_t serial_get_read_avail(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_SERIAL);
	int32_t rtn = vexDeviceGenericSerialReceiveAvail(device->device_info);
	return_port(port - 1, rtn);
}

int32_t serial_get_write_free(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_SERIAL);
	int32_t rtn = vexDeviceGenericSerialWriteFree(device->device_info);
	return_port(port - 1, rtn);
}

// Read functions

int32_t serial_peek_byte(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_SERIAL);
	int32_t rtn = vexDeviceGenericSerialPeekChar(device->device_info);
	return_port(port - 1, rtn);
}

int32_t serial_read_byte(uint8_t port) {
	claim_port_i(port - 1, E_DEVICE_SERIAL);
	int32_t rtn = vexDeviceGenericSerialReadChar(device->device_info);
	return_port(port - 1, rtn);
}

int32_t serial_read(uint8_t port, uint8_t* buffer, int32_t length) {
	claim_port_i(port - 1, E_DEVICE_SERIAL);
	int32_t rtn = vexDeviceGenericSerialReceive(device->device_info, buffer, length);
	return_port(port - 1, rtn);
}

// Write functions

int32_t serial_write_byte(uint8_t port, uint8_t buffer) {
	claim_port_i(port - 1, E_DEVICE_SERIAL);
	int32_t rtn = vexDeviceGenericSerialWriteChar(device->device_info, buffer);
	if (rtn == -1) {
		errno = EIO;
		return_port(port - 1, PROS_ERR);
	}
	return_port(port - 1, rtn);
}

int32_t serial_write(uint8_t port, uint8_t* buffer, int32_t length) {
	claim_port_i(port - 1, E_DEVICE_SERIAL);
	int32_t rtn = vexDeviceGenericSerialTransmit(device->device_info, buffer, length);
	if (rtn == -1) {
		errno = EIO;
		return_port(port - 1, PROS_ERR);
	}
	return_port(port - 1, rtn);
}
