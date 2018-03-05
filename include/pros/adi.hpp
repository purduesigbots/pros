/**
 * \file pros/adi.hpp
 *
 * \brief Prototypes for interfacing with the ADI.
 *
 * This file contains the header info for the functions used to modify the
 * status of the ADI ports.
 *
 * \copyright (c) 2017-2018, Purdue University ACM SIGBots.
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
	ADIPort(uint8_t port);
	ADIPort(uint8_t port, adi_port_config_e_t type);
	virtual ~ADIPort();

	int32_t config_set(adi_port_config_e_t type);
	int32_t config_get();
	int32_t value_set(int32_t value);
	int32_t value_get();

      protected:
	ADIPort();
	int _port;
};

class ADIAnalogIn : private ADIPort {
      public:
	ADIAnalogIn(uint8_t port);

	int32_t calibrate();
	using ADIPort::value_get;
	int32_t value_get_calibrated();
	int32_t value_get_calibrated_HR();
};

using ADIPotentiometer = ADIAnalogIn;
using ADILineSensor = ADIAnalogIn;
using ADILightSensor = ADIAnalogIn;
using ADIAccelerometer = ADIAnalogIn;

class ADIAnalogOut : private ADIPort {
      public:
	ADIAnalogOut(uint8_t port);

	using ADIPort::value_set;
};

class ADIDigitalOut : private ADIPort {
      public:
	ADIDigitalOut(uint8_t port);
	ADIDigitalOut(uint8_t port, bool init_state);

	using ADIPort::value_set;
};

class ADIDigitalIn : private ADIPort {
      public:
	ADIDigitalIn(uint8_t port);

	using ADIPort::value_get;
	int32_t get_new_press();
};

using ADIButton = ADIDigitalIn;

class ADIMotor : private ADIPort {
      public:
	ADIMotor(uint8_t port);

	using ADIPort::value_set;
	using ADIPort::value_get;
	int32_t stop();
};

class ADIEncoder : private ADIPort {
      public:
	ADIEncoder(uint8_t port_bottom, uint8_t port_top);
	ADIEncoder(uint8_t port_bottom, uint8_t port_top, bool reversed);

	using ADIPort::value_get;
	int32_t reset();
};

class ADIUltrasonic : private ADIPort {
      public:
	ADIUltrasonic(uint8_t port_bottom, uint8_t port_top);

	using ADIPort::value_get;
};
}

#endif
