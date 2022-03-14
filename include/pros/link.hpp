/**
 * \file pros/link.hpp
 *
 * Contains prototypes for functions related to robot to robot communications.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/link.html to learn
 * more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * Copyright (c) 2017-2021, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _PROS_LINK_HPP_
#define _PROS_LINK_HPP_

#include <cstdint>
#include <string>

#include "pros/link.h"

namespace pros {
class Link {
	private:
	std::uint8_t _port;

	public:
	/**
	 * Initializes a link on a radio port, with an indicated type. There might be a
	 * 1 to 2 second delay from when this function is called to when the link is initializes.
	 *
	 * This function uses the following values of errno when an error state is
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
	 *      Indicates whether the radio link on the brain is a transmitter or reciever,
	 *      with the transmitter having double the transmitting bandwidth as the recieving
	 *      end (1040 bytes/s vs 520 bytes/s).
	 * \param ov
	 * 		Indicates if the radio on the given port needs vexlink to override the controller radio
	 *
	 * \return PROS_ERR if initialization fails, 1 if the initialization succeeds.
	 */
	Link(const std::uint8_t port, const std::string link_id, link_type_e_t type, bool ov = false);

	/**
	 * Checks if a radio link on a port is active or not.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a radio.
	 * ENXIO - The sensor is still calibrating, or no link is connected via the radio.
	 *
	 * \return If a radio is connected to a port and it's connected to a link.
	 */
	bool connected();

	/**
	 * Returns the bytes of data number of without protocol available to be read
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a radio.
	 * ENXIO - The sensor is still calibrating, or no link is connected via the radio.
	 *
	 * \return PROS_ERR if port is not a link/radio, else the bytes available to be
	 * read by the user.
	 */
	std::uint32_t raw_receivable_size();

	/**
	 * Returns the bytes of data available in transmission buffer.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a radio.
	 * ENXIO - The sensor is still calibrating, or no link is connected via the radio.
	 *
	 * \return PROS_ERR if port is not a link/radio,
	 */
	std::uint32_t raw_transmittable_size();

	/**
	 * Send raw serial data through vexlink.
	 *
	 * This function uses the following values of errno when an error state is
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
	 */
	std::uint32_t transmit_raw(void* data, std::uint16_t data_size);

	/**
	 * Receive raw serial data through vexlink.
	 *
	 * This function uses the following values of errno when an error state is
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
	 */
	std::uint32_t receive_raw(void* dest, std::uint16_t data_size);

	/**
	 * Send packeted message through vexlink, with a checksum and start byte.
	 *
	 * This function uses the following values of errno when an error state is
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
	 */
	std::uint32_t transmit(void* data, std::uint16_t data_size);

	/**
	 * Receive packeted message through vexlink, with a checksum and start byte.
	 *
	 * This function uses the following values of errno when an error state is
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
	 */
	std::uint32_t receive(void* dest, std::uint16_t data_size);

	/**
	 * Clear the receive buffer of the link, and discarding the data.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as a radio.
	 * ENXIO - The sensor is still calibrating, or no link is connected via the radio.

	 * \return PROS_ERR if port is not a link, 1 if the operation succeeded.
	 */
	std::uint32_t clear_receive_buf();
};
}  // namespace pros

#endif
