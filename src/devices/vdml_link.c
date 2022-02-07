/**
 * \file vdml_link.c
 *
 * \brief Contains source code for functions related to the robot to robot communications.
 *
 * Visit https://pros.cs.purdue.edu/v5/api/c/link.html to learn
 * more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * Copyright (c) 2017-2022, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

 #include "pros/link.h"
 #include "kapi.h"
 #include "pros/apix.h"
 #include "vdml/vdml.h"
 #include <string.h>

#define FIFO_SIZE           512 // VEX Defined TX/RX Queue Size
#define PROTOCOL_SIZE         4 // Protocol Size 

static uint8_t start_byte = 0x33;

uint32_t link_init(uint8_t port, char* link_id, link_type_e_t type) {
    // not used because it's a generic serial device 
    // claim_port_i(port - 1, E_DEVICE_RADIO);
	if (!VALIDATE_PORT_NO(port - 1)) {
		errno = EINVAL;
		return PROS_ERR;
	}
    // by default, the vexlink radio in the lower port should be by default E_DEVICE_NONE
    // or, the only radio on the brain
    v5_device_e_t plugged_device = registry_get_plugged_type(port - 1);
    if(plugged_device != E_DEVICE_NONE && plugged_device != E_DEVICE_RADIO) {
        return PROS_ERR;
    }
	v5_smart_device_s_t* device = registry_get_device(port - 1);
	if (!port_mutex_take(port - 1)) {
		errno = EACCES;
		return PROS_ERR;
	}
    vexDeviceGenericRadioConnection(device->device_info, link_id, type, false);
    return_port(port - 1, 1);
}

uint32_t link_init_override(uint8_t port, char* link_id, link_type_e_t type) {
    // not used because it's a generic serial device 
    // claim_port_i(port - 1, E_DEVICE_RADIO);
	if (!VALIDATE_PORT_NO(port - 1)) {
		errno = EINVAL;
		return PROS_ERR;
	}
    // by default, the vexlink radio in the lower port should be by default E_DEVICE_NONE
    // or, the only radio on the brain
    v5_device_e_t plugged_device = registry_get_plugged_type(port - 1);
    if(plugged_device != E_DEVICE_NONE && plugged_device != E_DEVICE_RADIO) {
        return PROS_ERR;
    }
	v5_smart_device_s_t* device = registry_get_device(port - 1);
	if (!port_mutex_take(port - 1)) {
		errno = EACCES;
		return PROS_ERR;
	}
    vexDeviceGenericRadioConnection(device->device_info, link_id, type, true);
    return_port(port - 1, 1);
}

bool link_connected(uint8_t port) {
    claim_port(port - 1, E_DEVICE_SERIAL, false);
    bool rtv = vexDeviceGenericRadioLinkStatus(device->device_info); 
    return_port(port - 1, rtv);
}

uint32_t link_raw_readable_size(uint8_t port) {
    claim_port_i(port - 1, E_DEVICE_SERIAL);
    uint32_t rtv = vexDeviceGenericRadioReceiveAvail(device->device_info); 
    return_port(port - 1, rtv);
}

uint32_t link_read_raw(uint8_t port, void* dest, uint32_t size) {
    claim_port_i(port - 1, E_DEVICE_SERIAL); 
    if(size > FIFO_SIZE) {
        errno = EINVAL;
        return_port(port - 1, 0);
    }
    if(!vexDeviceGenericRadioLinkStatus(device->device_info)) {
        errno = ENXIO;
        return_port(port - 1, PROS_ERR);
    }
    if(dest == NULL) {
        errno = EINVAL;
        return_port(port - 1, PROS_ERR);
    }
    uint32_t rtv = vexDeviceGenericRadioReceive(device->device_info, dest, size);
    return_port(port - 1, rtv);
}

uint32_t link_transmit_raw(uint8_t port, void* data, uint32_t data_size) {
    claim_port_i(port - 1, E_DEVICE_SERIAL);
        errno = ENXIO;
        return_port(port - 1, PROS_ERR);
    if(!vexDeviceGenericRadioLinkStatus(device->device_info)) {
    }
    if(data_size > vexDeviceGenericRadioWriteFree(device->device_info)) {
        errno = EBUSY;
        return_port(port - 1, 0);
    }
    if(data == NULL) {
        errno = EINVAL;
        return_port(port - 1, PROS_ERR);
    }
    return_port(port - 1, 1);
}

uint32_t link_transmit(uint8_t port, void* data, uint32_t data_size) {
    claim_port_i(port - 1, E_DEVICE_SERIAL);
        errno = ENXIO;
        return_port(port - 1, PROS_ERR);
    if(!vexDeviceGenericRadioLinkStatus(device->device_info)) {
        errno = ENXIO;
        return_port(port - 1, PROS_ERR);
    }
    if(data_size + PROTOCOL_SIZE > vexDeviceGenericRadioWriteFree(device->device_info)) {
        errno = EBUSY;
        return_port(port - 1, 0);
    }
    if(data == NULL) {
        errno = EINVAL;
        return_port(port - 1, PROS_ERR);
    }
    uint16_t msg_size = (uint16_t)(data_size + PROTOCOL_SIZE);
    uint8_t checksum = start_byte;
    checksum ^= (msg_size >> 8) & 0xff;
    checksum ^= (msg_size) & 0xff;
    for(int i = 0; i < data_size; i++) {
        checksum ^= ((char*)data)[i];
    }
    // send protocol
    vexDeviceGenericRadioTransmit(device->device_info, &start_byte, 1);
    vexDeviceGenericRadioTransmit(device->device_info, (uint8_t*)msg_size, 2);
    vexDeviceGenericRadioTransmit(device->device_info, (uint8_t*)data, data_size);
    vexDeviceGenericRadioTransmit(device->device_info, &checksum, 1);
    return_port(port - 1, 1);
}

uint32_t link_tx_size_raw(uint8_t port) {
    claim_port_i(port - 1, E_DEVICE_SERIAL);
    uint32_t rtv = vexDeviceGenericRadioWriteFree(device->device_info);
    return_port(port - 1, rtv);
}

uint32_t link_read(uint8_t port, void* dest, uint32_t size) {
    claim_port_i(port - 1, E_DEVICE_SERIAL); 
    if(size > FIFO_SIZE) {
        errno = EINVAL;
        return_port(port - 1, 0);
    }
    if(!vexDeviceGenericRadioLinkStatus(device->device_info)) {
        errno = ENXIO;
        return_port(port - 1, PROS_ERR);
    }
    if(dest == NULL) {
        errno = EINVAL;
        return_port(port - 1, PROS_ERR);
    }

    uint32_t rtv = vexDeviceGenericRadioReceive(device->device_info, dest, size);
    // TODO: Potentially create a RX buffer w/ a daemon?

    return_port(port - 1, rtv);
}

