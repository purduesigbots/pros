/**
 * \file pros/link.h
 * \ingroup c-link
 *
 * Contains prototypes for functions related to the robot to robot communications.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \defgroup c-link VEX Link C API
 */

#ifndef _PROS_LINK_H_
#define _PROS_LINK_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
namespace pros {
#endif

/**
 * \ingroup c-link
 * */

/**
 * \addtogroup c-link
 *  @{
 */

/**
 * \enum link_type_e_t
 * \brief Enum for the type of link (TX or RX)
 */
typedef enum link_type_e {
	E_LINK_RECEIVER = 0, ///< Indicates that the radio is a receiver.
	E_LINK_TRANSMITTER, ///< Indicates that the link is a transmitter.
	E_LINK_RX = E_LINK_RECEIVER, ///< Alias for E_LINK_RECEIVER
	E_LINK_TX = E_LINK_TRANSMITTER ///< Alias for E_LINK_TRANSMITTER
} link_type_e_t;

#ifdef PROS_USE_SIMPLE_NAMES
#ifdef __cplusplus
#define LINK_RECEIVER pros::E_LINK_RECEIVER
#define LINK_TRANSMITTER pros::E_LINK_TRANSMITTER
#define LINK_RX pros::E_LINK_RX
#define LINK_TX pros::E_LINK_TX
#else
#define LINK_RECEIVER E_LINK_RECEIVER
#define LINK_TRANSMITTER E_LINK_TRANSMITTER
#define LINK_RX E_LINK_RX
#define LINK_TX E_LINK_TX
#endif
#endif

/// @brief
/// The maximum size of a link buffer
#define LINK_BUFFER_SIZE 512

#ifdef __cplusplus
namespace c {
#endif

/**
 * Initializes a link on a radio port, with an indicated type. There might be a
 * 1 to 2 second delay from when this function is called to when the link is initializes.
 * PROS currently only supports the use of one radio per brain.
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
 *
 * \return PROS_ERR if initialization fails, 1 if the initialization succeeds.
 *
 * \b Example
 * \code
 * #define LINK_TRANSMITTER_PORT 1
 * #define LINK_ID "ROBOT1"
 *
 * void initialize() {
 *   link_init(LINK_TRANSMITTER_PORT, LINK_ID, E_LINK_TRANSMITTER);
 * }
 * \endcode
 */
uint32_t link_init(uint8_t port, const char* link_id, link_type_e_t type);

/**
 * Initializes a link on a radio port, with an indicated type and the ability for
 * vexlink to override the controller radio. There might be a 1 to 2 second delay
 * from when this function is called to when the link is initializes.
 * PROS currently only supports the use of one radio per brain.
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
 *
 * \return PROS_ERR if initialization fails, 1 if the initialization succeeds.
 *
 * \b Example
 * \code
 * #define LINK_PORT 1
 * #define LINK_ID "ROBOT1"
 *
 * void initialize() {
 *   link_init(LINK_PORT, LINK_ID, E_LINK_TRANSMITTER);
 *   link_init_override(LINK_PORT, LINK_ID, E_LINK_TRANSMITTER);
 * }
 * \endcode
 */
uint32_t link_init_override(uint8_t port, const char* link_id, link_type_e_t type);

/**
 * Checks if a radio link on a port is active or not.
 *
 * \note This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a radio.
 * ENXIO - The sensor is still calibrating, or no link is connected via the radio.
 *
 * \param port
 *      The port of the radio for the intended link.
 *
 * \return If a radio is connected to a port and it's connected to a link.
 *
 * \b Example
 * \code
 * #define LINK_TRANSMITTER_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     if (link_connected(LINK_TRANSMITTER_PORT)) {
 *       screen_print(TEXT_MEDIUM, 1, "Link connected!");
 *     }
 *   delay(20);
 *   }
 * }
 * \endcode
 */
bool link_connected(uint8_t port);

/**
 * Returns the bytes of data available to be read
 *
 * \note This function uses the following values of errno when an error state is
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
 *
 * \b Example
 * \code
 * #define LINK_RECIVER_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     uint32_t receiveable_size = link_raw_receivable_size(LINK_RECIVER_PORT);
 *     screen_print(TEXT_MEDIUM, 1, "link_raw_receiveable_size: %d", receiveable_size);
 *     delay(20);
 *   }
 * }
 * \endcode
 */
uint32_t link_raw_receivable_size(uint8_t port);

/**
 * Returns the bytes of data available in transmission buffer.
 *
 * \note This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a radio.
 * ENXIO - The sensor is still calibrating, or no link is connected via the radio.
 *
 * \param port
 *      The port of the radio for the intended link.
 *
 * \return PROS_ERR if port is not a link/radio,
 *
 * \b Example
 * \code
 * #define LINK_TRANSMITTER_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     uint32_t transmittable_size = link_raw_transmittable_size(LINK_TRANSMITTER_PORT);
 *     screen_print(TEXT_MEDIUM, 1, "link_raw_transmittable_size: %d", transmittable_size);
 *     delay(20);
 *   }
 * }
 * \endcode
 */
uint32_t link_raw_transmittable_size(uint8_t port);

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
 * \param port
 *      The port of the radio for the intended link.
 * \param data
 *      Buffer with data to send
 * \param data_size
 *      Bytes of data to be read to the destination buffer
 *
 * \return PROS_ERR if port is not a link, and the successfully transmitted
 * data size if it succeeded.
 *
 * \b Example
 * \code
 * #define LINK_TRANSMITTER_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     char* data = "Hello!";
 *     link_transmit_raw(LINK_TRANSMITTER_PORT, (void*)data, sizeof(*data) * sizeof(data));
 *     delay(20);
 *   }
 * }
 * \endcode
 */
uint32_t link_transmit_raw(uint8_t port, void* data, uint16_t data_size);

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
 * \param port
 *      The port of the radio for the intended link.
 * \param dest
 *      Destination buffer to read data to
 * \param data_size
 *      Bytes of data to be read to the destination buffer
 *
 * \return PROS_ERR if port is not a link, and the successfully received
 * data size if it succeeded.
 *
 * \b Example
 * \code
 * #define LINK_RECIVER_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     char* result;
 *     char* expected = "Hello!";
 *     link_receive_raw(LINK_RECIVER_PORT, (void*)result, sizeof(*expected) * sizeof(expected));
 *     delay(20);
 *   }
 * }
 * \endcode
 */
uint32_t link_receive_raw(uint8_t port, void* dest, uint16_t data_size);

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
 * \param port
 *      The port of the radio for the intended link.
 * \param data
 *      Buffer with data to send
 * \param data_size
 *      Bytes of data to be read to the destination buffer
 *
 * \return PROS_ERR if port is not a link, and the successfully transmitted
 * data size if it succeeded.
 *
 * \b Example
 * \code
 * #define LINK_TRANSMITTER_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     char* data = "Hello!";
 *     link_transmit(LINK_TRANSMITTER_PORT, (void*)data, sizeof(*data) * sizeof(data));
 *     delay(20);
 *   }
 * }
 * \endcode
 */
uint32_t link_transmit(uint8_t port, void* data, uint16_t data_size);

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
 *
 * \b Example
 * \code
 * #define LINK_RECIVER_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     char* result;
 *     char* expected = "Hello!";
 *     link_receive(LINK_RECIVER_PORT, (void*)result, sizeof(*expected) * sizeof(expected));
 *     delay(20);
 *   }
 * }
 * \endcode
 */
uint32_t link_receive(uint8_t port, void* dest, uint16_t data_size);

/**
 * Clear the receive buffer of the link, and discarding the data.
 *
 * \note This function uses the following values of errno when an error state is
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
 *
 * \b Example
 * \code
 * #define LINK_TRANSMITTER_PORT 1
 *
 * void opcontrol() {
 *   while (true) {
 *     char* data = "Hello!";
 *     link_transmit(LINK_TRANSMITTER_PORT, (void*)data, sizeof(*data) * sizeof(data));
 *     link_clear_receive_buf(LINK_TRANSMITTER_PORT);
 *     delay(20);
 *   }
 * }
 * \endcode
 */
uint32_t link_clear_receive_buf(uint8_t port);

///@}

#ifdef __cplusplus
}
}
}
#endif

#endif
