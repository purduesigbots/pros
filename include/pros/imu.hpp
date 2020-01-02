/**
 * \file pros/imu.hpp
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
#ifndef _PROS_IMU_HPP_
#define _PROS_IMU_HPP_

#include <cstdint>
#include "pros/imu.h"

namespace pros {
class Imu {
	const std::uint8_t _port;

	public:
	Imu(const std::uint8_t port) : _port(port){};

	virtual std::int32_t reset() const;
    virtual double get_rotation() const;
	virtual double get_vex_heading() const;
    virtual double get_heading() const;
	virtual double get_vex_degrees() const;
	virtual pros::c::quaternion_s_t get_quaternion() const;
	virtual pros::c::euler_s_t get_euler() const;
	virtual double get_pitch() const;
	virtual double get_roll() const;
	virtual double get_yaw() const;
	virtual pros::c::imu_gyro_s_t get_gyro_rate() const;
	virtual pros::c::imu_accel_s_t get_accel() const;
	virtual pros::c::imu_status_e_t get_status() const;
	virtual bool is_calibrating() const;
};
}  // namespace pros

#endif
