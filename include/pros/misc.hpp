/**
 * \file pros/misc.hpp
 *
 * \brief Contains miscellaneous declarations such as controller and competition
 * functions in C++
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/controller to learn more.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 * All rights reservered.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _PROS_MISC_HPP_
#define _PROS_MISC_HPP_

#include "pros/misc.h"

namespace pros {
class Controller {
	public:
	/**
	 * Creates a controller object for the given controller id.
	 *
	 * \param id
	 * 			  The ID of the controller (e.g. the master or partner controller).
	 * 			  Must be one of CONTROLLER_MASTER or CONTROLLER_PARTNER
	 */
	Controller(controller_id_e_t id);

	/**
	 * Checks if the controller is connected.
	 *
	 * This function uses the following values of errno when an error state is reached:
	 * EACCES - Another resource is currently trying to access the controller port.
	 *
	 * \return 1 if the controller is connected, 0 otherwise
	 */
	std::int32_t is_connected(void);

	/**
	 * Gets the value of an analog channel (joystick) on a controller.
	 *
	 * This function uses the following values of errno when an error state is reached:
	 * EACCES - Another resource is currently trying to access the controller port.
	 *
	 * \param channel
	 * 			  The analog channel to get.
	 * 			  Must be one of ANALOG_LEFT_X, ANALOG_LEFT_Y, ANALOG_RIGHT_X, ANALOG_RIGHT_Y
	 *
	 * \return The current reading of the analog channel: [-127, 127].
	 * 			   If the controller was not connected, then 0 is returned
	 */
	std::int32_t get_analog(controller_analog_e_t channel);

	/**
	 * Checks if a digital channel (button) on the controller is currently pressed.
	 *
	 * \note
	 * 			The naming scheme for the buttons is not yet finalized as VEX finalizes
	 * 			the controller naming pattern
	 *
	 * This function uses the following values of errno when an error state is reached:
	 * EACCES - Another resource is currently trying to access the controller port.
	 *
	 * \param button
	 * 			  The button to read.
	 * 			  Must be one of DIGITAL_{RIGHT,DOWN,LEFT,UP,A,B,Y,X,R1,R2,L1,L2}
	 *
	 * \return 1 if the button on the controller is pressed.
	 * 			   If the controller was not connected, then 0 is returned
	 */
	std::int32_t get_digital(controller_digital_e_t button);

	/**
	 * Returns a rising-edge case for a controller button press.
	 *
	 * This function is not thread-safe.
	 * Multiple tasks polling a single button may return different results under the
	 * same circumstances, so only one task should call this function for any given
	 * button. E.g., Task A calls this function for buttons 1 and 2. Task B may call
	 * this function for button 3, but should not for buttons 1 or 2. A typical
	 * use-case for this function is to call inside opcontrol to detect new button
	 * presses, and not in any other tasks.
	 *
	 * This function uses the following values of errno when an error state is reached:
	 * EACCES - Another resource is currently trying to access the controller port.
	 *
	 * \param button
	 * 			  The button to read.
	 * 			  Must be one of DIGITAL_{RIGHT,DOWN,LEFT,UP,A,B,Y,X,R1,R2,L1,L2}
	 *
	 * \return 1 if the button on the controller is pressed and had not been pressed
	 *         the last time this function was called, 0 otherwise.
	 */
	std::int32_t get_digital_new_press(controller_digital_e_t button);

	private:
	controller_id_e_t _id;
};

namespace battery {
/**
 * Gets the current voltage of the battery, as reported by VEXos.
 *
 * This function uses the following values of errno when an error state is reached:
 * EACCES - Another resource is currently trying to access the battery port.
 *
 * \return The current voltage of the battery
 */
double get_capacity(void);

/**
 * Gets the current current of the battery in milliamps, as reported by VEXos.
 *
 * This function uses the following values of errno when an error state is reached:
 * EACCES - Another resource is currently trying to access the battery port.
 *
 * \return The current current of the battery
 */
int32_t get_current(void);

/**
 * Gets the current temperature of the battery, as reported by VEXos.
 *
 * This function uses the following values of errno when an error state is reached:
 * EACCES - Another resource is currently trying to access the battery port.
 *
 * \return The current temperature of the battery
 */
double get_temperature(void);

/**
 * Gets the current capacity of the battery in millivolts, as reported by VEXos.
 *
 * This function uses the following values of errno when an error state is reached:
 * EACCES - Another resource is currently trying to access the battery port.
 *
 * \return The current capacity of the battery
 */
int32_t get_voltage(void);
}  // namespace battery

namespace competition {
/**
 * Get the current status of the competition control.
 *
 * \return The competition control status as a mask of bits with
 * 			   COMPETITION_{ENABLED,AUTONOMOUS,CONNECTED}.
 */
std::uint8_t get_status(void);
std::uint8_t is_autonomous(void);
std::uint8_t is_connected(void);
std::uint8_t is_disabled(void);
}  // namespace competition
}  // namespace pros

#endif
