/**
 * \file pros/imu.h
 *
 * Contains prototypes for functions related to the VEX Inertial sensor.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/imu.html to learn
 * more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * Copyright (c) 2017-2019, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_IMU_H_
#define _PROS_IMU_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
namespace pros {
namespace c {
#endif

typedef enum imu_status_e {
  E_IMU_STATUS_CALIBRATING = 0x01,
} imu_status_e_t;

typedef struct __attribute__((__packed__)) quaternion_s {
  double w;
  double x;
  double y;
  double z;
} quaternion_s_t;

typedef struct __attribute__((__packed__)) attitude_s {
  double pitch;
  double roll;
  double yaw;
} attitude_s_t;

typedef struct __attribute__((__packed__)) imu_s {
  double x;
  double y;
  double x;
  double w;
} imu_s_t;

/**
 * Calibrate IMU
 *
 * Takes approx. 2 seconds
 *
 * \param port
 *        The V5 IMU port number from 1-21
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t imu_reset(uint8_t port);
double imu_get_heading(uint8_t port);
double imu_get_degrees(uint8_t port);
// TODO: figure out whether these need to have an i/o parameter instead
quaternion_s_t imu_get_quaternion(uint8_t port);
angles_s_t imu_get_angles(uint8_t port);
attitude_s_t imu_get_attitude(uint8_t port);
imu_s_t imu_get_raw_gyro(uint8_t port);
imu_s_t imu_get_raw_accel(uint8_t port);
imu_status_e_t imu_get_status(uint8_t port);
// NB: not used
// void imu_set_mode(uint8_t port, uint32_t mode);
// uint32_t imu_get_mode(uint8_t port);

#ifdef __cplusplus
}
}
}
#endif

#endif
