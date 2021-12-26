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

 #include "link.h"
 #include "kapi.h"
 #include "pros/apix.h"
 #include "vdml.h"

static uint8_t link_count = 0;

uint32_t link_init(uint8_t port, link_type_e_t type) {
    claim_port(port - 1, E_DEVICE_RADIO, false);
    vexGenericRadioConnection(port - 1, (char)('A' + link_count), type, true);
    link_count++;
    return_port(port - 1, 1);
}

bool link_connected(uint8_t port) {
    claim_port(port - 1, E_DEVICE_RADIO, false);
    bool rtv = vexGenericRadioLinkStatus(device);
    return_port(port - 1, rtv);
}
