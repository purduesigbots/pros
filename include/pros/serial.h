/**
 * \file pros/serial.h
 *
 * Contains prototypes for the V5 Generic Serial related functions.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/serial.html to learn
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

#ifndef _PROS_SERIAL_H_
#define _PROS_SERIAL_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
namespace pros {
namespace c {
#endif

/******************************************************************************/
/**                      Serial communication functions                      **/
/**                                                                          **/
/**  These functions allow programmers to communicate using UART over RS485  **/
/******************************************************************************/

/**
 * Enables generic serial on the given port.
 *
 * \note This function must be called before any of the generic serial
 * functions will work.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t serial_enable(uint8_t port);

/**
 * Sets the baudrate for the serial port to operate at.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param baudrate
 *        The baudrate to operate at
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t serial_set_baudrate(uint8_t port, int32_t baudrate);

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
 * \param port
 *        The V5 port number from 1-21
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t serial_flush(uint8_t port);

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
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The number of bytes avaliable to be read or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t serial_get_read_avail(uint8_t port);

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
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The number of bytes free or PROS_ERR if the operation failed,
 * setting errno.
 */
int32_t serial_get_write_free(uint8_t port);

/**
 * Reads the next byte avaliable in the port's input buffer without removing it.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The next byte avaliable to be read, -1 if none are available, or
 * PROS_ERR if the operation failed, setting errno.
 */
int32_t serial_peek_byte(uint8_t port);

/**
 * Reads the next byte avaliable in the port's input buffer.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The given value is not within the range of V5 ports (1-21).
 * EACCES - Another resource is currently trying to access the port.
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The next byte avaliable to be read, -1 if none are available, or
 * PROS_ERR if the operation failed, setting errno.
 */
int32_t serial_read_byte(uint8_t port);

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
 * \param port
 *        The V5 port number from 1-21
 * \param buffer
 *        The location to place the data read
 * \param length
 *        The maximum number of bytes to read
 *
 * \return The number of bytes read or PROS_ERR if the operation failed, setting
 * errno.
 */
int32_t serial_read(uint8_t port, uint8_t* buffer, int32_t length);

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
 * \param port
 *        The V5 port number from 1-21
 * \param buffer
 *        The byte to write
 *
 * \return The number of bytes written or PROS_ERR if the operation failed,
 * setting errno.
 */
int32_t serial_write_byte(uint8_t port, uint8_t buffer);

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
 * \param port
 *        The V5 port number from 1-21
 * \param buffer
 *        The data to write
 * \param length
 *        The maximum number of bytes to write
 *
 * \return The number of bytes written or PROS_ERR if the operation failed,
 * setting errno.
 */
int32_t serial_write(uint8_t port, uint8_t* buffer, int32_t length);

#ifdef __cplusplus
}  // namespace c
}  // namespace pros
}
#endif

#endif  // _PROS_SERIAL_H_