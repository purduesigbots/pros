/**
 * \file pros/adi.h
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
#ifndef _PROS_ADI_H_
#define _PROS_ADI_H_

/**
 * Represents the port type for an ADI port. Mirrors _V5_AdiPortConfiguration
 */
typedef enum adi_port_config_e {
	E_ADI_ANALOG_IN = 0,
	E_ADI_ANALOG_OUT,
	E_ADI_DIGITAL_IN,
	E_ADI_DIGITAL_OUT,

	E_ADI_SMART_BUTTON,
	E_ADI_SMART_POT,

	E_ADI_LEGACY_BUTTON,
	E_ADI_LEGACY_POT,
	E_ADI_LEGACY_LINE_SENSOR,
	E_ADI_LEGACY_LIGHT_SENSOR,
	E_ADI_LEGACY_GYRO,
	E_ADI_LEGACY_ACCELEROMETER,

	E_ADI_LEGACY_SERVO,
	E_ADI_LEGACY_PWM,

	E_ADI_LEGACY_ENCODER,
	E_ADI_LEGACY_ULTRASONIC,

	E_ADI_TYPE_UNDEFINED = 255,
	E_ADI_ERR = PROS_ERR
} adi_port_config_e_t;

/*
 * For the following functions:
 * If they don't need a return value (e.g. a setter):
 * 		They will return PROS_ERR upon failure and 1 upons success
 * If they should return a bool (e.g. motor_get_over_temp_flag):
 *		They will return 1 for true, 0 for false, and PROS_ERR upon failure
 * If they should return a 32-bit integer (e.g. a getter):
 *		They will return their value. If an error occurs, they will return
 *		PROS_ERR. If their actual return value should be PROS_ERR (INT32_MAX)
 *		then they will set errno = 0 to indicate no error has occured.
 * If they should return a double (e.g. a getter):
 *		They will return their value, or PROS_ERROR_F upon failure
 *
 * Upon returning PROS_ERR or PROS_ERR_F, errno will be set to indicate the
 * type of error. Again, some functions may return PROS_ERR as a valid value;
 * in this case, errno will be set to 0
 */

/**
 * Configures an ADI port to act as a given sensor type.
 *
 * \param port
 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
 * \param type
 *        The configuration type for the port
 */
int32_t adi_port_config_set(int port, adi_port_config_e_t type);

/**
 * Returns the configuration for the given ADI port
 *
 * \param port
 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') for which to return
 *        the configuration
 */
adi_port_config_e_t adi_port_config_get(int port);

/**
 * Sets the value for the given ADI port
 * This only works on ports configured as outputs, and the behavior will change
 * depending on the configuration of the port
 *
 * \param port
 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') for which the value
 *        will be set
 * \param value
 *        The value to set the ADI port to
 */
int32_t adi_value_set(int port, int32_t value);

/**
 * Returns the value for the given ADI port
 *
 * \param port
 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') for which the value
 *        will be returned
 */
int32_t adi_value_get(int port);

#endif
