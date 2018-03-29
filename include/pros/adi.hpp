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
	ADIPort(std::uint8_t port);
	ADIPort(std::uint8_t port, adi_port_config_e_t type);
	virtual ~ADIPort();

	std::int32_t config_set(adi_port_config_e_t type) const;
	std::int32_t config_get() const;
	std::int32_t value_set(std::int32_t value) const;
	std::int32_t value_get() const;

	protected:
	ADIPort();
	int _port;
};

class ADIAnalogIn : private ADIPort {
	public:
	ADIAnalogIn(std::uint8_t port);

	std::int32_t calibrate() const;
	std::int32_t value_get_calibrated() const;
	std::int32_t value_get_calibrated_HR() const;

	using ADIPort::value_get;
};

using ADIPotentiometer = ADIAnalogIn;
using ADILineSensor = ADIAnalogIn;
using ADILightSensor = ADIAnalogIn;
using ADIAccelerometer = ADIAnalogIn;

class ADIAnalogOut : private ADIPort {
	public:
	ADIAnalogOut(std::uint8_t port);

	using ADIPort::value_set;
};

class ADIDigitalOut : private ADIPort {
	public:
	ADIDigitalOut(std::uint8_t port);
	ADIDigitalOut(std::uint8_t port, bool init_state);

	using ADIPort::value_set;
};

class ADIDigitalIn : private ADIPort {
	public:
	ADIDigitalIn(std::uint8_t port);

	std::int32_t get_new_press() const;

	using ADIPort::value_get;
};

using ADIButton = ADIDigitalIn;

class ADIMotor : private ADIPort {
	public:
	ADIMotor(std::uint8_t port);

	std::int32_t stop() const;

	using ADIPort::value_set;
	using ADIPort::value_get;
};

class ADIEncoder : private ADIPort {
	public:
	ADIEncoder(std::uint8_t port_bottom, std::uint8_t port_top);
	ADIEncoder(std::uint8_t port_bottom, std::uint8_t port_top, bool reversed);

	std::int32_t reset() const;

	using ADIPort::value_get;
};

class ADIUltrasonic : private ADIPort {
	public:
	ADIUltrasonic(std::uint8_t port_bottom, std::uint8_t port_top);

	using ADIPort::value_get;
};
}

#endif  // _PROS_ADI_HPP_
