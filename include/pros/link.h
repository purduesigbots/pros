/**
 * \file pros/link.h
 *
 * Contains prototypes for functions related to the robot to robot communications.
 *
 * Visit https://pros.cs.purdue.edu/v5/api/c/link.html to learn
 * more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * Copyright (c) 2017-2022, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_LINK_H_
#define _PROS_LINK_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
namespace pros {
#endif

typedef enum link_type_e {
    E_LINK_RECIEVER = 0,
    E_LINK_TRANSMITTER
} link_type_e_t;

#define LINK_BUFFER_SIZE 512

#ifdef __cplusplus
namespace c {
#endif

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
 *
 * \return PROS_ERR if initialization fails, 1 if the initialization succeeds.
 */
uint32_t link_init(uint8_t port, const char* link_id, link_type_e_t type);

/**
 * Initializes a link on a radio port, with an indicated type and the ability for
 * vexlink to override the controller radio. There might be a 1 to 2 second delay
 * from when this function is called to when the link is initializes.
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
 *
 * \return PROS_ERR if initialization fails, 1 if the initialization succeeds.
 */
uint32_t link_init_override(uint8_t port, const char* link_id, link_type_e_t type);

/**
 * Checks if a radio link on a port is active or not.
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a radio.
 * ENXIO - The sensor is still calibrating, or no link is connected via the radio.
 * 
 * \param port 
 *      The port of the radio for the intended link.
 *
 * \return If a radio is connected to a port and it's connected to a link.
 */
bool link_connected(uint8_t port);

/**
 * Returns the bytes of data available to be read  
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a radio.
 * ENXIO - The sensor is still calibrating, or no link is connected via the radio.
 * 
 * \param port 
 *      The port of the radio for the intended link.
 *
 * \return PROS_ERR if port is not a link/radio, else the bytes available to be
 * read by the user.
 */
uint32_t link_raw_receivable_size(uint8_t port);

/**
 * Returns the bytes of data available in transmission buffer.
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a radio.
 * ENXIO - The sensor is still calibrating, or no link is connected via the radio.
 * 
 * \param port 
 *      The port of the radio for the intended link.
 *
 * \return PROS_ERR if port is not a link/radio, 
 */
uint32_t link_raw_transmittable_size(uint8_t port);

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
 * \param port 
 *      The port of the radio for the intended link.
 * \param data
 *      Buffer with data to send
 * \param data_size
 *      Bytes of data to be read to the destination buffer
 * 
 * \return PROS_ERR if port is not a link, and the successfully transmitted 
 * data size if it succeeded.
 */
uint32_t link_transmit_raw(uint8_t port, void* data, uint16_t data_size);

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
 * \param port 
 *      The port of the radio for the intended link.
 * \param dest
 *      Destination buffer to read data to
 * \param data_size
 *      Bytes of data to be read to the destination buffer
 * 
 * \return PROS_ERR if port is not a link, and the successfully received 
 * data size if it succeeded.
 */
uint32_t link_receive_raw(uint8_t port, void* dest, uint16_t data_size);

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
 * \param port 
 *      The port of the radio for the intended link.
 * \param data
 *      Buffer with data to send
 * \param data_size
 *      Bytes of data to be read to the destination buffer
 * 
 * \return PROS_ERR if port is not a link, and the successfully transmitted 
 * data size if it succeeded.
 */
uint32_t link_transmit(uint8_t port, void* data, uint16_t data_size);

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
 * 
 * \param port 
 *      The port of the radio for the intended link.
 * \param dest
 *      Destination buffer to read data to
 * \param data_size
 *      Bytes of data to be read to the destination buffer
 * 
 * \return PROS_ERR if port is not a link or protocol error, and the successfully 
 * transmitted data size if it succeeded.
 */
uint32_t link_receive(uint8_t port, void* dest, uint16_t data_size);

/**
 * Clear the receive buffer of the link, and discarding the data.
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a radio.
 * ENXIO - The sensor is still calibrating, or no link is connected via the radio.
 * 
 * \param port 
 *      The port of the radio for the intended link.
 * 
 * \return PROS_ERR if port is not a link, and the successfully received 
 * data size if it succeeded.
 */
uint32_t link_clear_receive_buf(uint8_t port);

#ifdef __cplusplus
}
}
}
#endif

#endif
