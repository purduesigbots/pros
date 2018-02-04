/**
 * pros/misc.h - PROS API header provides high-level user functionality
 *
 * Contains miscellaneous declarations for use by typical VEX programmers using PROS.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 * All rights reservered.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_MISC_H_
#define _PROS_MISC_H_

#ifdef __cplusplus
extern "C" {
#endif

#define NUM_V5_PORTS (22)

/******************************************************************************/
/**                             V5 Competition                               **/
/******************************************************************************/
#define COMPETITION_AUTONOMOUS (1 << 0)
#define COMPETITION_DISABLED (1 << 1)
#define COMPETITION_CONNECTED (1 << 2)
/**
 * Get the current status of the competition control
 *
 * \return
 * 			Returns the competition control status as a mask of bits with
 * 			COMPETITION_{ENABLED,AUTONOMOUS,CONNECTED}.
 */
uint8_t competition_get_status();
#define competition_is_disabled() ((competition_get_status() & COMPETITION_DISABLED) != 0)
#define competition_is_connected() ((competition_get_status() & COMPETITION_CONNECTED) != 0)
#define competition_is_autonomous() ((competition_get_status() & COMPETITION_AUTONOMOUS) != 0)

/******************************************************************************/
/**                              V5 Controller                               **/
/******************************************************************************/
typedef enum { E_CONTROLLER_MASTER = 0, E_CONTROLLER_PARTNER } controller_id_e_t;

typedef enum {
	E_CONTROLLER_ANALOG_LEFT_X = 0,
	E_CONTROLLER_ANALOG_LEFT_Y,
	E_CONTROLLER_ANALOG_RIGHT_X,
	E_CONTROLLER_ANALOG_RIGHT_Y
} controller_analog_e_t;

typedef enum {
	E_CONTROLLER_DIGITAL_L1 = 6,
	E_CONTROLLER_DIGITAL_L2,
	E_CONTROLLER_DIGITAL_R1,
	E_CONTROLLER_DIGITAL_R2,
	E_CONTROLLER_DIGITAL_UP,
	E_CONTROLLER_DIGITAL_DOWN,
	E_CONTROLLER_DIGITAL_LEFT,
	E_CONTROLLER_DIGITAL_RIGHT,
	E_CONTROLLER_DIGITAL_X,
	E_CONTROLLER_DIGITAL_B,
	E_CONTROLLER_DIGITAL_Y,
	E_CONTROLLER_DIGITAL_A
} controller_digital_e_t;

#ifdef PROS_USE_SIMPLE_NAMES
#define CONTROLLER_MASTER E_CONTROLLER_MASTER
#define CONTORLLER_PARTNER E_CONTROLLER_PARTNER
#define ANALOG_LEFT_X E_CONTROLLER_ANALOG_LEFT_X
#define ANALOG_LEFT_Y E_CONTROLLER_ANALOG_LEFT_Y
#define ANALOG_RIGHT_X E_CONTROLLER_ANALOG_RIGHT_X
#define ANALOG_RIGHT_Y E_CONTROLLER_ANALOG_RIGHT_Y
#define DIGITAL_L1 E_CONTROLLER_DIGITAL_L1
#define DIGITAL_L2 E_CONTROLLER_DIGITAL_L2
#define DIGITAL_R1 E_CONTROLLER_DIGITAL_R1
#define DIGITAL_R2 E_CONTROLLER_DIGITAL_R2
#define DIGITAL_UP E_CONTROLLER_DIGITAL_UP
#define DIGITAL_DOWN E_CONTROLLER_DIGITAL_DOWN
#define DIGITAL_LEFT E_CONTROLLER_DIGITAL_LEFT
#define DIGITAL_RIGHT E_CONTROLLER_DIGITAL_RIGHT
#define DIGITAL_X E_CONTROLLER_DIGITAL_X
#define DIGITAL_B E_CONTROLLER_DIGITAL_B
#define DIGITAL_Y E_CONTROLLER_DIGITAL_Y
#define DIGITAL_A E_CONTROLLER_DIGITAL_A
#endif

/**
 * Return 0 or 1 if the controller is connected.
 *
 * \param id
 * 			The ID of the controller (e.g. the master or partner controller).
 * 			Must be one of CONTROLLER_MASTER or CONTROLLER_PARTNER
 * \return
 * 			1 if the contrller is connected, 0 otherwise
 */
int32_t controller_is_connected(controller_id_e_t id);

/**
 * Gets the value of an analog channel (joystick) on a controller.
 *
 * \param id
 * 			The ID of the controller (e.g. the master or partner controller).
 * 			Must be one of CONTROLLER_MASTER or CONTROLLER_PARTNER
 * \param channel
 * 			The analog channel to get.
 * 			Must be one of ANALOG_LEFT_X, ANALOG_LEFT_Y, ANALOG_RIGHT_X, ANALOG_RIGHT_Y
 * \return
 * 			Returns the current reading of the analog channel: [-127, 127].
 * 			If the controller was not connected, then 0 is returned
 */
int32_t controller_get_analog(controller_id_e_t id, controller_analog_e_t channel);

/**
 * Gets the value of an digital channel (button) on a controller.
 *
 * \param id
 * 			The ID of the controller (e.g. the master or partner controller).
 * 			Must be one of CONTROLLER_MASTER or CONTROLLER_PARTNER
 * \param button
 * 			The button to read.
 * 			Must be one of DIGITAL_{RIGHT,DOWN,LEFT,UP,A,B,Y,X,R1,R2,L1,L2}
 * \return
 * 			Returns 1 if the button on the controller is pressed.
 * 			If the controller was not connected, then 0 is returned
 * \note
 * 			The naming scheme for the buttons is not yet finalized as VEX finalizes
 * 			the controller naming pattern
 */
int32_t controller_get_digital(controller_id_e_t id, controller_digital_e_t button);

/******************************************************************************/
/**                           Device Registration                            **/
/******************************************************************************/

/*
 * \brief List of possible v5 devices
 *
 * This list contains all current V5 Devices, and mirrors V5_DeviceType from the
 * api.
 */
typedef enum v5_device_e {
	E_DEVICE_NONE = 0,
	E_DEVICE_MOTOR = 2,
	E_DEVICE_LED = 3,
	E_DEVICE_RGB = 4,
	E_DEVICE_BUMPER = 5,
	E_DEVICE_IMU = 6,
	E_DEVICE_RANGE = 7,
	E_DEVICE_RADIO = 8,
	E_DEVICE_TETHER = 9,
	E_DEVICE_BRAIN = 10,
	E_DEVICE_VISION = 11,
	E_DEVICE_ADI = 12,
	E_DEVICE_GYRO = 0x46,
	E_DEVICE_SONAR = 0x47,
	E_DEVICE_GENERIC = 128,
	E_DEVICE_UNDEFINED = 255
} v5_device_e;

/*
 * \brief Registers a device in the given port
 *
 * Registers a device of the given type in the given port into the registry, if
 * that type of device is detected to be plugged in to that port.
 *
 * \param[in]  port             the port number to register the device
 * \param[in]  device			the type of device to register
 *
 * \return 1 upon success, PROS_ERR upon failure
 *
 * \exception EINVAL Port number is out of range.
 * \exception EINVAL A different device than specified is plugged in.
 * \exception EADDRINUSE the port is already registered to another device
 */
int registry_bind_port(unsigned int port, v5_device_e device_type);

/*
 * \brief Deregisters a devices from the given port
 *
 * Removes the device registed in the given port, if there is one.
 *
 * \param[in]  port      		the port number to deregister
 *
 * \return 1 upon success, PROS_ERR upon failure
 *
 * \exception EINVAL the port number is out of range
 */
int registry_unbind_port(unsigned int port);

/*
 * \brief Checks whether there is a device mismatch among all registered ports.
 *
 * Check what device type is registered in each port and compares it to the
 * device actually plugged into each port. An error is displayed upon detection
 */
void registry_validate_all_bindings();
#ifdef __cplusplus
}
#endif

#endif