/**
 * \file pros/link.hpp
 * \ingroup cpp-link
 *
 * Contains prototypes for functions related to robot to robot communications.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * Copyright (c) 2017-2021, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * \defgroup cpp-link VEX Link C++ API
 */
#ifndef _PROS_LINK_HPP_
#define _PROS_LINK_HPP_

#include <cstdint>
#include <string>

#include "pros/link.h"
#include "pros/device.hpp"

namespace pros {
/**
 * \ingroup cpp-link
 */
class Link : public Device {
	/**
	 * \addtogroup cpp-link
	 * ///@{
	 */
	private:

	public:
	/**
	 * Initializes a link on a radio port, with an indicated type. There might be a
	 * 1 to 2 second delay from when this function is called to when the link is initializes.
	 *
	 * \note This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a radio.
	 * ENXIO - The sensor is still calibrating, or no link is connected via the radio.
	 *
	 * \param port
	 *      The port of the radio for the intended link.
	 * \param link_id
	 *      Unique link ID in the form of a string, needs to be different from other links in
	 *      the area.
	 * \param type
	 *      Indicates whether the radio link on the brain is a transmitter or receiver,
	 *      with the transmitter having double the transmitting bandwidth as the receiving
	 *      end (1040 bytes/s vs 520 bytes/s).
	 * \param ov
	 * 		Indicates if the radio on the given port needs vexlink to override the controller radio
	 *
	 * \return PROS_ERR if initialization fails, 1 if the initialization succeeds.
	 * 
	 * \b Example:
	 * \code
	 * pros::Link link(1, "my_link", pros::E_LINK_TX);
	 * \endcode
	 */
	explicit Link(const std::uint8_t port, const std::string link_id, link_type_e_t type, bool ov = false);

	/**
	 * Checks if a radio link on a port is active or not.
	 *
	 * \note This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a radio.
	 * ENXIO - The sensor is still calibrating, or no link is connected via the radio.
	 *
	 * \return If a radio is connected to a port and it's connected to a link.
	 * 
	 * \b Example:
	 * \code
	 * pros::Link link(1, "my_link", pros::E_LINK_TX);
	 * if (link.connected()) {
	 * 	// do something
	 * }
	 * \endcode
	 */
	bool connected();

	/**
	 * Returns the bytes of data number of without protocol available to be read
	 *
	 * \note This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a radio.
	 * ENXIO - The sensor is still calibrating, or no link is connected via the radio.
	 *
	 * \return PROS_ERR if port is not a link/radio, else the bytes available to be
	 * read by the user.
	 *
	 * \b Example:
	 * \code
	 * void opcontrol() {
	 * 	pros::Link link(1, "my_link", pros::E_LINK_TX);
	 * 	printf("Bytes available to read: %d", link.receivable_size());
	 * }
	 * \endcode
	 */
	std::uint32_t raw_receivable_size();

	/**
	 * Returns the bytes of data available in transmission buffer.
	 *
	 * \note This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a radio.
	 * ENXIO - The sensor is still calibrating, or no link is connected via the radio.
	 *
	 * \return PROS_ERR if port is not a link/radio,
	 * else the bytes available to be transmitted by the user.
	 *
	 * \b Example:
	 * \code
	 * void opcontrol() {
	 * 	pros::Link link(1, "my_link", pros::E_LINK_TX);
	 * 	printf("Bytes available to transmit: %d", link.transmittable_size());
	 * }
	 */
	std::uint32_t raw_transmittable_size();

	/**
	 * Send raw serial data through vexlink.
	 *
	 * \note This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a radio.
	 * ENXIO - The sensor is still calibrating, or no link is connected via the radio.
	 * EBUSY - The transmitter buffer is still busy with a previous transmission, and there is no
	 * room in the FIFO buffer (queue) to transmit the data.
	 * EINVAL - The data given is NULL
	 *
	 * \param data
	 *      Buffer with data to send
	 * \param data_size
	 *      Buffer with data to send
	 *
	 * \return PROS_ERR if port is not a link, and the successfully transmitted 
	 * data size if it succeeded.
	 * 
	 * \b Example:
	 * \code
	 * void opcontrol() {
	 * 	pros::Link link(1, "my_link", pros::E_LINK_TX);
	 * 	std::uint8_t data[4] = {0x01, 0x02, 0x03, 0x04};
	 *  link.transmit_raw(data, 4);
	 * }
	 * 
	 * \endcode
	 */
	std::uint32_t transmit_raw(void* data, std::uint16_t data_size);

	/**
	 * Receive raw serial data through vexlink.
	 *
	 * \note This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a radio.
	 * ENXIO - The sensor is still calibrating, or no link is connected via the radio.
	 * EINVAL - The destination given is NULL, or the size given is larger than the FIFO buffer
	 * or destination buffer.
	 *
	 * \param dest
	 *      Destination buffer to read data to
	 * \param data_size
	 *      Bytes of data to be read to the destination buffer
	 *
	 * \return PROS_ERR if port is not a link, and the successfully received 
	 * data size if it succeeded.
	 *
	 * \b Example:
	 * \code
	 * void opcontrol() {
	 * 	pros::Link link(1, "my_link", pros::E_LINK_TX);
	 * 	std::uint8_t data[4];
	 *  link.receive_raw(data, 4);
	 * }
	 * \endcode
	 */
	std::uint32_t receive_raw(void* dest, std::uint16_t data_size);

	/**
	 * Send packeted message through vexlink, with a checksum and start byte.
	 *
	 * \note This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a radio.
	 * ENXIO - The sensor is still calibrating, or no link is connected via the radio.
	 * EBUSY - The transmitter buffer is still busy with a previous transmission, and there is no
	 * room in the FIFO buffer (queue) to transmit the data.
	 * EINVAL - The data given is NULL
	 *
	 * \param data
	 *      Buffer with data to send
	 * \param data_size
	 *      Bytes of data to be read to the destination buffer
	 *
	 * \return PROS_ERR if port is not a link, and the successfully transmitted 
	 * data size if it succeeded.
	 *
	 * \b Example:
	 * \code
	 * void opcontrol() {
	 * 	pros::Link link(1, "my_link", pros::E_LINK_TX);
	 * 	std::uint8_t data[4] = {0x01, 0x02, 0x03, 0x04};
	 *  link.transmit(data, 4);
	 * }
	 * \endcode
	 */
	std::uint32_t transmit(void* data, std::uint16_t data_size);

	/**
	 * Receive packeted message through vexlink, with a checksum and start byte.
	 *
	 * \note This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a radio.
	 * ENXIO - The sensor is still calibrating, or no link is connected via the radio.
	 * EINVAL - The destination given is NULL, or the size given is larger than the FIFO buffer
	 * or destination buffer.
	 * EBADMSG - Protocol error related to start byte, data size, or checksum.

	 * \param dest
	 *      Destination buffer to read data to
	 * \param data_size
	 *      Bytes of data to be read to the destination buffer
	 *
	 * \return PROS_ERR if port is not a link, and the successfully received 
	 * data size if it succeeded.
	 *
	 * \b Example:
	 * \code
	 * void opcontrol() {
	 * 	pros::Link link(1, "my_link", pros::E_LINK_TX);
	 * 	std::uint8_t data[4];
	 *  link.receive(data, 4);
	 * }
	 * \endcode
	 */
	std::uint32_t receive(void* dest, std::uint16_t data_size);

	/**
	 * Clear the receive buffer of the link, and discarding the data.
	 *
	 * \note This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a radio.
	 * ENXIO - The sensor is still calibrating, or no link is connected via the radio.
	 *
	 * \return PROS_ERR if port is not a link, 1 if the operation succeeded.
	 * 
	 * \b Example:
	 * \code
	 * void opcontrol() {
	 * 	pros::Link link(1, "my_link", pros::E_LINK_TX);
	 *  link.clear_receive_buf();
	 * }
	 * \endcode
	 */
	std::uint32_t clear_receive_buf();
  
	///@}
};
}  // namespace pros

#endif
