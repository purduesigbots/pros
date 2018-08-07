/**
 * \file controller.c
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
#include "ifi/v5_api.h"
#include "kapi.h"
#include "vdml/vdml.h"

// From enum in misc.h
#define NUM_BUTTONS 12

// Represents the array of "was_pressed" for all available buttons.
// There are two controllers, thus two sets of button states are stored.
bool button_pressed[NUM_BUTTONS * 2];

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

int32_t controller_get_analog(controller_id_e_t id,
                              controller_analog_e_t channel) {
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

int32_t controller_get_digital(controller_id_e_t id,
                               controller_digital_e_t button) {
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

int32_t controller_get_digital_new_press(controller_id_e_t id,
                                         controller_digital_e_t button) {
  int32_t pressed = controller_get_digital(id, button);
  uint8_t button_num = button - E_CONTROLLER_DIGITAL_L1;
  if (id == E_CONTROLLER_PARTNER)
    button_num += NUM_BUTTONS;

  if (!pressed)
    button_pressed[button_num] = false;

  if (pressed && !button_pressed[button_num]) {
    // button is currently pressed and was not detected as being pressed during
    // last check
    button_pressed[button_num] = true;
    return true;
  } else
    return false; // button is not pressed or was already detected
}

uint8_t competition_get_status(void) {
  if (!internal_port_mutex_take(V5_PORT_CONTROLLER_1)) {
    errno = EACCES;
    return 255;
  }
  uint8_t rtn = vexCompetitionStatus();
  internal_port_mutex_give(V5_PORT_CONTROLLER_1);
  return rtn;
}
