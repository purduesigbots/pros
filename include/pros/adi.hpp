/**
 * \file pros/adi.hpp
 *
 * \brief Prototypes and functions for interfacing with the ADI in C++.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/adi to learn more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _PROS_ADI_HPP_
#define _PROS_ADI_HPP_

#include "pros/adi.h"

namespace pros {
class ADIPort {
	public:
	ADIPort(std::uint8_t port, adi_port_config_e_t type = E_ADI_TYPE_UNDEFINED);
	virtual ~ADIPort();

	std::int32_t get_config() const;
	std::int32_t get_value() const;
	std::int32_t set_config(adi_port_config_e_t type) const;
	std::int32_t set_value(std::int32_t value) const;

	protected:
	ADIPort();
	std::uint8_t _port;
};

class ADIAnalogIn : private ADIPort {
	public:
	ADIAnalogIn(std::uint8_t port);

	std::int32_t calibrate() const;
	std::int32_t get_value_calibrated() const;
	std::int32_t get_value_calibrated_HR() const;

	using ADIPort::get_value;
};

using ADIPotentiometer = ADIAnalogIn;
using ADILineSensor = ADIAnalogIn;
using ADILightSensor = ADIAnalogIn;
using ADIAccelerometer = ADIAnalogIn;

class ADIAnalogOut : private ADIPort {
	public:
	ADIAnalogOut(std::uint8_t port);

	using ADIPort::set_value;
};

class ADIDigitalOut : private ADIPort {
	public:
	ADIDigitalOut(std::uint8_t port, bool init_state = LOW);

	using ADIPort::set_value;
};

class ADIDigitalIn : private ADIPort {
	public:
	ADIDigitalIn(std::uint8_t port);

	std::int32_t get_new_press() const;

	using ADIPort::get_value;
};

using ADIButton = ADIDigitalIn;

class ADIMotor : private ADIPort {
	public:
	ADIMotor(std::uint8_t port);

	std::int32_t stop() const;

	using ADIPort::set_value;
	using ADIPort::get_value;
};

class ADIEncoder : private ADIPort {
	public:
	ADIEncoder(std::uint8_t port_top, std::uint8_t port_bottom, bool reversed = false);

	std::int32_t reset() const;

	using ADIPort::get_value;
};

class ADIUltrasonic : private ADIPort {
	public:
	ADIUltrasonic(std::uint8_t port_echo, std::uint8_t port_ping);

	using ADIPort::get_value;
};
}

#endif
