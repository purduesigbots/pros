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
 * Copyright (c) 2017-2021, Purdue University ACM SIGBots.
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

#define COBS_HEADER_SIZE    0   // Placeholder, Fill in later once protocol implemented
#define FIFO_SIZE           512 // VEX Defined TX/RX Queue Size

static uint8_t link_count = 0;

uint32_t link_init(uint8_t port, char* link_id, link_type_e_t type) {
    claim_port_i(port - 1, E_DEVICE_RADIO);
    vexDeviceGenericRadioConnection(device->device_info, link_id, type, true);
    link_count++;
    return_port(port - 1, 1);
}

bool link_connected(uint8_t port) {
    claim_port(port - 1, E_DEVICE_RADIO, false);
    bool rtv = vexDeviceGenericRadioLinkStatus(device->device_info);
    return_port(port - 1, rtv);
}

uint32_t link_readable_size(uint8_t port) {
    claim_port(port - 1, E_DEVICE_RADIO, false);
    bool rtv = vexDeviceGenericRadioReceiveAvail(device->device_info);
    return_port(port - 1, rtv);
}

uint8_t link_get_count(void) {
    return link_count;
}

uint32_t link_transmit_raw(uint8_t port, void* data) {
    claim_port_i(port - 1, E_DEVICE_RADIO);
    uint32_t data_size = sizeof((uint8_t*)data);
    if(!vexDeviceGenericRadioLinkStatus(device->device_info)) {
        errno = ENXIO;
        return_port(port - 1, PROS_ERR);
    }
    if(data_size > vexDeviceGenericRadioWriteFree(device->device_info)) {
        errno = EBUSY;
        return_port(port - 1, 0);
    }
    if(data == NULL) {
        errno = EINVAL;
        return_port(port - 1, PROS_ERR);
    }
    vexDeviceGenericRadioTransmit(device->device_info, data, data_size);
    return_port(port - 1, 1);
}

uint32_t link_read_raw(uint8_t port, void* dest, uint32_t size) {
    claim_port_i(port - 1, E_DEVICE_RADIO);
    if(size > FIFO_SIZE || size > sizeof((uint8_t*)dest)) {
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


