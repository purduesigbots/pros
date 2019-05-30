/**
 * \file pros/serial.hpp
 *
 * Contains prototypes for the V5 Generic Serial related functions.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/serial.html to learn
 * more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2018, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_SERIAL_HPP_
#define _PROS_SERIAL_HPP_

#include <cstdint>
#include "pros/serial.h"

namespace pros {
class Serial {
	public:
	/**
	 * Creates a Serial object for the given port and specifications.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - The given value is not within the range of V5 ports (1-21).
	 * EACCES - Another resource is currently trying to access the port.
	 *
	 * \param port
	 *        The V5 port number from 1-21
	 * \param baudrate
	 *        The baudrate to run the port at
	 */
	explicit Serial(std::uint8_t port, std::int32_t baudrate);

	explicit Serial(std::uint8_t port);

	/******************************************************************************/
	/**                      Serial communication functions                      **/
	/**                                                                          **/
	/**  These functions allow programmers to communicate using UART over RS485  **/
	/******************************************************************************/

	/**
	 * Sets the baudrate for the serial port to operate at.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - The given value is not within the range of V5 ports (1-21).
	 * EACCES - Another resource is currently trying to access the port.
	 *
	 * \param baudrate
	 *        The baudrate to operate at
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t set_baudrate(std::int32_t baudrate) const;

	/**
	 * Clears the internal input and output FIFO buffers.
	 *
	 * This can be useful to reset state and remove old, potentially unneeded data
	 * from the input FIFO buffer or to cancel sending any data in the output FIFO
	 * buffer.
	 *
	 * \note This function does not cause the data in the output buffer to be
	 * written, it simply clears the internal buffers. Unlike stdout, generic
	 * serial does not use buffered IO (the FIFO buffers are written as soon
	 * as possible).
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - The given value is not within the range of V5 ports (1-21).
	 * EACCES - Another resource is currently trying to access the port.
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t flush() const;

	/**
	 * Returns the number of bytes available to be read in the the port's FIFO
	 * input buffer.
	 *
	 * \note This function does not actually read any bytes, is simply returns the
	 * number of bytes available to be read.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - The given value is not within the range of V5 ports (1-21).
	 * EACCES - Another resource is currently trying to access the port.
	 *
	 * \return The number of bytes avaliable to be read or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	virtual std::int32_t get_read_avail() const;

	/**
	 * Returns the number of bytes free in the port's FIFO output buffer.
	 *
	 * \note This function does not actually write any bytes, is simply returns the
	 * number of bytes free in the port's buffer.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - The given value is not within the range of V5 ports (1-21).
	 * EACCES - Another resource is currently trying to access the port.
	 *
	 * \return The number of bytes free or PROS_ERR if the operation failed,
	 * setting errno.
	 */
	virtual std::int32_t get_write_free() const;

	/**
	 * Reads the next byte avaliable in the port's input buffer without removing it.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - The given value is not within the range of V5 ports (1-21).
	 * EACCES - Another resource is currently trying to access the port.
	 *
	 * \return The next byte avaliable to be read, -1 if none are available, or
	 * PROS_ERR if the operation failed, setting errno.
	 */
	virtual std::int32_t peek_byte() const;

	/**
	 * Reads the next byte avaliable in the port's input buffer.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - The given value is not within the range of V5 ports (1-21).
	 * EACCES - Another resource is currently trying to access the port.
	 *
	 * \return The next byte avaliable to be read, -1 if none are available, or
	 * PROS_ERR if the operation failed, setting errno.
	 */
	virtual std::int32_t read_byte() const;

	/**
	 * Reads up to the next length bytes from the port's input buffer and places
	 * them in the user supplied buffer.
	 *
	 * \note This function will only return bytes that are currently avaliable to be
	 * read and will not block waiting for any to arrive.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - The given value is not within the range of V5 ports (1-21).
	 * EACCES - Another resource is currently trying to access the port.
	 *
	 * \param buffer
	 *        The location to place the data read
	 * \param length
	 *        The maximum number of bytes to read
	 *
	 * \return The number of bytes read or PROS_ERR if the operation failed, setting
	 * errno.
	 */
	virtual std::int32_t read(std::uint8_t* buffer, std::int32_t length) const;

	/**
	 * Write the given byte to the port's output buffer.
	 *
	 * \note Data in the port's output buffer is written to the serial port as soon
	 * as possible on a FIFO basis and can not be done manually by the user.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - The given value is not within the range of V5 ports (1-21).
	 * EACCES - Another resource is currently trying to access the port.
	 * EIO - Serious internal write error.
	 *
	 * \param buffer
	 *        The byte to write
	 *
	 * \return The number of bytes written or PROS_ERR if the operation failed,
	 * setting errno.
	 */
	virtual std::int32_t write_byte(std::uint8_t buffer) const;

	/**
	 * Writes up to length bytes from the user supplied buffer to the port's output
	 * buffer.
	 *
	 * \note Data in the port's output buffer is written to the serial port as soon
	 * as possible on a FIFO basis and can not be done manually by the user.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - The given value is not within the range of V5 ports (1-21).
	 * EACCES - Another resource is currently trying to access the port.
	 * EIO - Serious internal write error.
	 *
	 * \param buffer
	 *        The data to write
	 * \param length
	 *        The maximum number of bytes to write
	 *
	 * \return The number of bytes written or PROS_ERR if the operation failed,
	 * setting errno.
	 */
	virtual std::int32_t write(std::uint8_t* buffer, std::int32_t length) const;

	private:
	const std::uint8_t _port;
};

namespace literals {
const pros::Serial operator"" _ser(const unsigned long long int m);
}  // namespace literals
}  // namespace pros
#endif  // _PROS_SERIAL_HPP_
