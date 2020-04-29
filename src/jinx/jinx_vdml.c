/**
 * \file jinx_vdml.c
 *
 * \brief Contains VDML-specific applications of jinx
 *
 * This file contains functionality which automatically tracks
 * variables of different smart devices.
 *
 * \copyright (c) 2017-2018, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * LIcense, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "jinx/jinx_vdml.h"
#include "vdml/registry.h"

static jinx_track_s_t *device_vars[NUM_V5_PORTS];

// Data to track for motors
struct motor_track_var {
  double velocity;
  int32_t current;
  double position;
  double torque;
};

// Data about each vision sensor object
struct vision_sensor_obj {
  int16_t x;
  int16_t y;
  uint16_t obj_id;
};

// Data to track for each vision sensor
struct vision_track_var {
  struct vision_sensor_obj *objects;
  uint16_t num_objects;
  // uint8_t vision_zero;
};

static jinx_track_s_t *adi_track_var;

void initialize_tracker() {
  for (int i = 0; i < NUM_V5_PORTS; i++) {
    device_vars[i] = NULL;
  }
}

bool fill_motor_data(void *buff, void *args) {
  if (args == NULL) {
    return false;
  }
  uint8_t port = *(uint8_t *)(args);
  if (registry_get_plugged_type(port) != registry_get_bound_type(port)) {
    // The device was unplugged, so stop tracking it
    if (!(device_vars[port]->contains_func)) {
      kfree(device_vars[port]->data_func.data_ptr);
    }
    jinx_untrack_variable(device_vars[port]);
    return false;
  }
  ((struct motor_track_var *)(buff))->velocity =
      motor_get_actual_velocity(port + 1);
  ((struct motor_track_var *)(buff))->current =
      motor_get_current_draw(port + 1);
  ((struct motor_track_var *)(buff))->position = motor_get_position(port + 1);
  ((struct motor_track_var *)(buff))->torque = motor_get_torque(port + 1);
  return true;
}

bool fill_vision_data(void *buff, void *args) {
  if (args == NULL) {
    return false;
  }
  uint8_t port = *(uint8_t *)(args);
  if (registry_get_plugged_type(port) != registry_get_bound_type(port)) {
    // The device was unplugged, so stop tracking it
    if (!(device_vars[port]->contains_func)) {
      kfree(device_vars[port]->data_func.data_ptr);
    }
    jinx_untrack_variable(device_vars[port]);
    return false;
  }
  vision_object_s_t obj_arr[5];
  int32_t num_written = vision_read_by_size(port + 1, 0, 5, obj_arr);
  for (int j = 0; j < num_written; j++) {
    ((struct vision_track_var *)(buff))->objects[j].x =
        obj_arr[j].x_middle_coord;
    ((struct vision_track_var *)(buff))->objects[j].y =
        obj_arr[j].y_middle_coord;
    ((struct vision_track_var *)(buff))->objects[j].obj_id =
        obj_arr[j].signature;
  }
  ((struct vision_track_var *)(buff))->num_objects =
      vision_get_object_count(port + 1);
  // ((struct vision_track_var *)(buff))->vision_zero =
      // vision_get_zero_point(port + 1);
  return true;
}

bool fill_adi_data(void *buff, void *args) {
  for (int i = 0; i < 8; i++) {
    ((int32_t *)(buff))[i] = adi_port_get_value(i + 1);
  }
  return true;
}

void track_motor_vars(uint8_t port) {
  char *subel[4] = {"velocity", "current", "position", "tourque"};
  jinx_var_extra_args_t exargs = {
      .subelements = subel, .n_subelements = 4, .modifiable = 0};
  jinx_var_t jinx_motor_var =
      jinx_create_variable(28, "Motor Variables", "2did", &exargs);
  uint8_t *port_no = kmalloc(sizeof(*port_no));
  *port_no = port;
  device_vars[port] =
      jinx_track_variable_fptr(jinx_motor_var, fill_motor_data, &port_no);
}

void track_vision_vars(uint8_t port) {
  char *subel[17] = {"x1", "y1", "obj_id1", "x2",         "y2",      "obj_id2",
                     "x3", "y3", "obj_id3", "x4",         "y4",      "obj_id4",
                     "x5", "y5", "obj_id5", "total_objs", "zero_pos"};
  jinx_var_extra_args_t exargs = {
      .subelements = subel, .n_subelements = 17, .modifiable = 0};
  // Size: Vision Objects are 6 bytes each and we have 5 of them plus 3 for
  // number of elements and center
  jinx_var_t jinx_vision_var = jinx_create_variable(
      33, "Vision Sensor Variables", "2hH2hH2hH2hH2h2H?", &exargs);
  uint8_t *port_no = kmalloc(sizeof *port_no);
  *port_no = port;
  device_vars[port] =
      jinx_track_variable_fptr(jinx_vision_var, fill_vision_data, &port_no);
}

void track_adi_vars() {
  char *subel[8] = {"port1", "port2", "port3", "port4",
                    "port5", "port6", "port7", "port8"};
  jinx_var_extra_args_t exargs = {
      .subelements = subel, .n_subelements = 8, .modifiable = 0};
  jinx_var_t jinx_adi_var =
      jinx_create_variable(32, "ADI Values", "8i", &exargs);
  adi_track_var = jinx_track_variable_fptr(jinx_adi_var, fill_adi_data, NULL);
}
