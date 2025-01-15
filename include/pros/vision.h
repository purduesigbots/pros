/**
 * \file pros/vision.h
 * \ingroup c-vision
 *
 * Contains prototypes for the VEX Vision Sensor-related functions.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2024, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * \defgroup c-vision Vision Sensor C API
 * \note Additional example code for this module can be found in its [Tutorial.](@ref vision)
 */

#ifndef _PROS_VISION_H_
#define _PROS_VISION_H_

/**
 * \ingroup c-vision
 */

/**
 * \addtogroup c-vision
 *  @{
 */

/// \name Macros
///Parameters given by VEX
///@{

#define VISION_OBJECT_ERR_SIG 255

/**
 * The width of the Vision Sensor’s field of view.
 */
#define VISION_FOV_WIDTH 316

/**
 * The height of the Vision Sensor’s field of view.
 */
#define VISION_FOV_HEIGHT 212

///@}

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
namespace pros {
#endif

/**
 * \enum vision_object_type_e_t
 * This enumeration defines the different types of objects that can be detected by the Vision Sensor
 */
typedef enum vision_object_type {
	E_VISION_OBJECT_NORMAL = 0,
	E_VISION_OBJECT_COLOR_CODE = 1,
	E_VISION_OBJECT_LINE = 2
} vision_object_type_e_t;

/**
 * \struct vision_signature_s_t
 * This structure contains the parameters used by the Vision Sensor to detect objects.
 */
typedef struct __attribute__((__packed__)) vision_signature {
	uint8_t id;
	uint8_t _pad[3];
	float range;
	int32_t u_min;
	int32_t u_max;
	int32_t u_mean;
	int32_t v_min;
	int32_t v_max;
	int32_t v_mean;
	uint32_t rgb;
	uint32_t type;
} vision_signature_s_t;

/**
 * \typedef vision_color_code_t
 * Color codes are just signatures with multiple IDs and a different type.
 */
typedef uint16_t vision_color_code_t;

/**
 * \struct vision_object_s_t
 * This structure contains a descriptor of an object detected by the Vision Sensor
 */
typedef struct __attribute__((__packed__)) vision_object {
	/// Object signature
	uint16_t signature;
	/// Object type, e.g. normal, color code, or line detection
	vision_object_type_e_t type;
	/// Left boundary coordinate of the object
	int16_t left_coord;
	/// Top boundary coordinate of the object
	int16_t top_coord;
	/// Width of the object
	int16_t width;
	/// Height of the object
	int16_t height;
	/// Angle of a color code object in 0.1 degree units (e.g. 10 -> 1 degree, 155 -> 15.5 degrees)
	uint16_t angle;
	/// Coordinates of the middle of the object (computed from the values above)
	int16_t x_middle_coord;
	/// Coordinates of the middle of the object (computed from the values above)
	int16_t y_middle_coord;
} vision_object_s_t;

/**
 * \enum vision_zero
 * This enumeration defines different zero points for returned vision objects.
 */
typedef enum vision_zero {
	/// (0,0) coordinate is the top left of the FOV
	E_VISION_ZERO_TOPLEFT = 0,
	/// (0,0) coordinate is the center of the FOV
	E_VISION_ZERO_CENTER = 1
} vision_zero_e_t;

#ifdef PROS_USE_SIMPLE_NAMES
#ifdef __cplusplus
#define VISION_OBJECT_NORMAL pros::E_VISION_OBJECT_NORMAL
#define VISION_OBJECT_COLOR_CODE pros::E_VISION_OBJECT_COLOR_CODE
#define VISION_OBJECT_LINE pros::E_VISION_OBJECT_LINE
#define VISION_ZERO_TOPLEFT pros::E_VISION_ZERO_TOPLEFT
#define VISION_ZERO_CENTER pros::E_VISION_ZERO_CENTER
#else
#define VISION_OBJECT_NORMAL E_VISION_OBJECT_NORMAL
#define VISION_OBJECT_COLOR_CODE E_VISION_OBJECT_COLOR_CODE
#define VISION_OBJECT_LINE E_VISION_OBJECT_LINE
#define VISION_ZERO_TOPLEFT E_VISION_ZERO_TOPLEFT
#define VISION_ZERO_CENTER E_VISION_ZERO_CENTER
#endif
#endif

#ifdef __cplusplus
namespace c {
#endif

/// \name Functions
///@{

/**
 * Clears the vision sensor LED color, reseting it back to its default behavior,
 * displaying the most prominent object signature color.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * #define VISION_PORT 1
 * void initialize() {
 *   vision_clear_led(VISION_PORT);
 * }
 * \endcode
 */
int32_t vision_clear_led(uint8_t port);

/**
 * Creates a signature from the vision sensor utility
 *
 * \param id
 *        The signature ID
 * \param u_min
 *        Minimum value on U axis
 * \param u_max
 *        Maximum value on U axis
 * \param u_mean
 *        Mean value on U axis
 * \param v_min
 *        Minimum value on V axis
 * \param v_max
 *        Maximum value on V axis
 * \param v_mean
 *        Mean value on V axis
 * \param range
 *        Scale factor
 * \param type
 *        Signature type
 *
 * \return A vision_signature_s_t that can be set using vision_set_signature
 *
 * \b Example
 * \code
 * #define VISION_PORT 1
 * #define EXAMPLE_SIG 1
 *
 * void opcontrol() {
 * // values acquired from the vision utility
 * vision_signature_s_t RED_SIG =
 *   vision_signature_from_utility(EXAMPLE_SIG, 8973, 11143, 10058, -2119, -1053, -1586, 5.4, 0);
 * vision_set_signature(VISION_PORT, EXAMPLE_SIG, &RED_SIG);
 * while (true) {
 *   vision_signature_s_t rtn = vision_get_by_sig(VISION_PORT, 0, EXAMPLE_SIG);
 *   // Gets the largest object of the EXAMPLE_SIG signature
 *   printf("sig: %d", rtn.signature);
 *   // Prints "sig: 1"
 *   delay(2);
 *   }
 * }
 * \endcode
 */
vision_signature_s_t vision_signature_from_utility(const int32_t id, const int32_t u_min, const int32_t u_max,
                                                   const int32_t u_mean, const int32_t v_min, const int32_t v_max,
                                                   const int32_t v_mean, const float range, const int32_t type);

/**
 * Creates a color code that represents a combination of the given signature
 * IDs. If fewer than 5 signatures are to be a part of the color code, pass 0
 * for the additional function parameters.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - Fewer than two signatures have been provided or one of the
 *          signatures is out of its [1-7] range (or 0 when omitted).
 *
 * \param port
 *        The V5 port number from 1-21
 * \param sig_id1
 *        The first signature id [1-7] to add to the color code
 * \param sig_id2
 *        The second signature id [1-7] to add to the color code
 * \param sig_id3
 *        The third signature id [1-7] to add to the color code
 * \param sig_id4
 *        The fourth signature id [1-7] to add to the color code
 * \param sig_id5
 *        The fifth signature id [1-7] to add to the color code
 *
 * \return A vision_color_code_t object containing the color code information.
 * 
 * \b Example
 * \code
 * #define VISION_PORT 1
 * #define EXAMPLE_SIG 1
 * #define OTHER_SIG 2
 * 
 * void opcontrol() {
 *   vision_color_code_t code1 = vision_create_color_code(VISION_PORT, EXAMPLE_SIG, OTHER_SIG);
 * }
 * \endcode
 */
vision_color_code_t vision_create_color_code(uint8_t port, const uint32_t sig_id1, const uint32_t sig_id2,
                                             const uint32_t sig_id3, const uint32_t sig_id4, const uint32_t sig_id5);

/**
 * Gets the nth largest object according to size_id.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 * EDOM - size_id is greater than the number of available objects.
 * EHOSTDOWN - Reading the vision sensor failed for an unknown reason.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param size_id
 *        The object to read from a list roughly ordered by object size
 *        (0 is the largest item, 1 is the second largest, etc.)
 *
 * \return The vision_object_s_t object corresponding to the given size id, or
 * PROS_ERR if an error occurred.
 * 
 * \b Example
 * \code
 * #define VISION_PORT 1
 * 
 * void opcontrol() {
 * while (true) {
 *   vision_object_s_t rtn = vision_get_by_size(VISION_PORT, 0);
 *   // Gets the largest object
 *   printf("sig: %d", rtn.signature);
 *   delay(2);
 *   }
 * }
 * \endcode
 */
vision_object_s_t vision_get_by_size(uint8_t port, const uint32_t size_id);

/**
 * Gets the nth largest object of the given signature according to size_id.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 * EINVAL - sig_id is outside the range [1-8]
 * EDOM - size_id is greater than the number of available objects.
 * EAGAIN - Reading the vision sensor failed for an unknown reason.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param size_id
 *        The object to read from a list roughly ordered by object size
 *        (0 is the largest item, 1 is the second largest, etc.)
 * \param signature
 *        The signature ID [1-7] for which an object will be returned.
 *
 * \return The vision_object_s_t object corresponding to the given signature and
 * size_id, or PROS_ERR if an error occurred.
 * 
 * \b Example
 * \code
 * #define VISION_PORT 1
 * #define EXAMPLE_SIG 1
 * 
 * void opcontrol() {
 * while (true) {
 *   vision_object_s_t rtn = vision_get_by_sig(VISION_PORT, 0, EXAMPLE_SIG);
 *   // Gets the largest object of the EXAMPLE_SIG signature
 *   printf("sig: %d", rtn.signature);
 *   // Prints "sig: 1"
 *   delay(2);
 *   }
 * }
 * \endcode
 */
vision_object_s_t vision_get_by_sig(uint8_t port, const uint32_t size_id, const uint32_t sig_id);

/**
 * Gets the nth largest object of the given color code according to size_id.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 * EAGAIN - Reading the vision sensor failed for an unknown reason.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param size_id
 *        The object to read from a list roughly ordered by object size
 *        (0 is the largest item, 1 is the second largest, etc.)
 * \param color_code
 *        The vision_color_code_t for which an object will be returned
 *
 * \return The vision_object_s_t object corresponding to the given color code
 * and size_id, or PROS_ERR if an error occurred.
 * 
 * \b Example
 * \code
 * #define VISION_PORT 1
 * #define EXAMPLE_SIG 1
 * #define OTHER_SIG 2
 * 
 * void opcontrol() {
 *   vision_color_code_t code1 = vision_create_color_code(VISION_PORT, EXAMPLE_SIG, OTHER_SIG);
 *   while (true) {
 *     vision_object_s_t rtn = vision_get_by_code(VISION_PORT, 0, code1);
 *     // Gets the largest object
 *     printf("sig: %d", rtn.signature);
 *     delay(2);
 *   }
 * }
 * \endcode
 */
vision_object_s_t vision_get_by_code(uint8_t port, const uint32_t size_id, const vision_color_code_t color_code);

/**
 * Gets the exposure parameter of the Vision Sensor. 
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The current exposure setting from [0,150], PROS_ERR if an error
 * occurred
 * 
 * \b Example
 * \code
 * #define VISION_PORT 1
 * 
 * void initialize() {
 *   if (vision_get_exposure(VISION_PORT) < 50)
 *   vision_set_exposure(VISION_PORT, 50);
 * }
 * \endcode
 */
int32_t vision_get_exposure(uint8_t port);

/**
 * Gets the number of objects currently detected by the Vision Sensor.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return The number of objects detected on the specified vision sensor.
 * Returns PROS_ERR if the port was invalid or an error occurred.
 * 
 * \b Example
 * \code
 * #define VISION_PORT 1
 * 
 * void opcontrol() {
 *   while (true) {
 *     printf("Number of Objects Detected: %d\n", vision_get_object_count(VISION_PORT));
 *     delay(2);
 *   }
 * }
 * \endcode
 */
int32_t vision_get_object_count(uint8_t port);

/**
 * Get the white balance parameter of the Vision Sensor.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 *
 * \param port
 * 		    The V5 port number from 1-21
 *
 * \return The current RGB white balance setting of the sensor
 * 
 * \b Example
 * \code
 * #define VISION_PORT 1
 * #define VISION_WHITE 0xff
 * 
 * void initialize() {
 *   if (vision_get_white_balance(VISION_PORT) != VISION_WHITE)
 *   vision_set_white_balance(VISION_PORT, VISION_WHITE);
 * }
 * \endcode
 */
int32_t vision_get_white_balance(uint8_t port);

/**
 * Prints the contents of the signature as an initializer list to the terminal.
 *
 * \param sig
 *        The signature for which the contents will be printed
 *
 * \return 1 if no errors occured, PROS_ERR otherwise
 * 
 * \b Example
 * \code
 * #define VISION_PORT 1
 * #define EXAMPLE_SIG 1
 * 
 * void opcontrol() {
 *   vision_signature_s_t sig = vision_get_signature(VISION_PORT, EXAMPLE_SIG);
 *   vision_print_signature(sig);
 * }
 * \endcode
 */
int32_t vision_print_signature(const vision_signature_s_t sig);

/**
 * Reads up to object_count object descriptors into object_arr.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21), or
 *          fewer than object_count number of objects were found.
 * ENODEV - The port cannot be configured as a vision sensor
 * EDOM - size_id is greater than the number of available objects.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param size_id
 *        The object to read from a list roughly ordered by object size
 *        (0 is the largest item, 1 is the second largest, etc.)
 * \param object_count
 *        The number of objects to read
 * \param[out] object_arr
 *             A pointer to copy the objects into
 *
 * \return The number of object signatures copied. This number will be less than
 * object_count if there are fewer objects detected by the vision sensor.
 * Returns PROS_ERR if the port was invalid, an error occurred, or fewer objects
 * than size_id were found. All objects in object_arr that were not found are
 * given VISION_OBJECT_ERR_SIG as their signature.
 * 
 * \b Example
 * \code
 * #define VISION_PORT 1
 * #define NUM_VISION_OBJECTS 4
 * 
 * void opcontrol() {
 *   vision_object_s_t object_arr[NUM_VISION_OBJECTS];
 *   while (true) {
 *     vision_read_by_size(VISION_PORT, 0, NUM_VISION_OBJECTS, object_arr);
 *     printf("sig: %d", object_arr[0].signature);
 *     // Prints the signature of the largest object found
 *     delay(2);
 *   }
 * }
 * \endcode
 */
int32_t vision_read_by_size(uint8_t port, const uint32_t size_id, const uint32_t object_count,
                            vision_object_s_t* const object_arr);

/**
 * Reads up to object_count object descriptors into object_arr.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21), or
 *          fewer than object_count number of objects were found.
 * ENODEV - The port cannot be configured as a vision sensor
 * EDOM - size_id is greater than the number of available objects.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param object_count
 *        The number of objects to read
 * \param size_id
 *        The object to read from a list roughly ordered by object size
 *        (0 is the largest item, 1 is the second largest, etc.)
 * \param signature
 *        The signature ID [1-7] for which objects will be returned.
 * \param[out] object_arr
 *             A pointer to copy the objects into
 *
 * \return The number of object signatures copied. This number will be less than
 * object_count if there are fewer objects detected by the vision sensor.
 * Returns PROS_ERR if the port was invalid, an error occurred, or fewer objects
 * than size_id were found. All objects in object_arr that were not found are
 * given VISION_OBJECT_ERR_SIG as their signature.
 * 
 * \b Example
 * \code
 * #define VISION_PORT 1
 * #define EXAMPLE_SIG 1
 * #define NUM_VISION_OBJECTS 4
 * 
 * void opcontrol() {
 *   vision_object_s_t object_arr[NUM_VISION_OBJECTS];
 *   while (true) {
 *     vision_read_by_sig(VISION_PORT, 0, EXAMPLE_SIG, NUM_VISION_OBJECTS, object_arr);
 *     printf("sig: %d", object_arr[0].signature);
 *     // Prints "sig: 1"
 *     delay(2);
 *   }
 * }
 * \endcode
 */
int32_t vision_read_by_sig(uint8_t port, const uint32_t size_id, const uint32_t sig_id, const uint32_t object_count,
                           vision_object_s_t* const object_arr);

/**
 * Reads up to object_count object descriptors into object_arr.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21), or
 *          fewer than object_count number of objects were found.
 * ENODEV - The port cannot be configured as a vision sensor
 *
 * \param port
 *        The V5 port number from 1-21
 * \param object_count
 *        The number of objects to read
 * \param size_id
 *        The object to read from a list roughly ordered by object size
 *        (0 is the largest item, 1 is the second largest, etc.)
 * \param color_code
 *        The vision_color_code_t for which objects will be returned
 * \param[out] object_arr
 *             A pointer to copy the objects into
 *
 * \return The number of object signatures copied. This number will be less than
 * object_count if there are fewer objects detected by the vision sensor.
 * Returns PROS_ERR if the port was invalid, an error occurred, or fewer objects
 * than size_id were found. All objects in object_arr that were not found are
 * given VISION_OBJECT_ERR_SIG as their signature.
 * 
 * \b Example
 * \code
 * #define VISION_PORT 1
 * #define EXAMPLE_SIG 1
 * #define OTHER_SIG 2
 * #define NUM_VISION_OBJECTS 4
 * 
 * void opcontrol() {
 *   vision_object_s_t object_arr[NUM_VISION_OBJECTS];
 *   vision_color_code_t code1 = vision_create_color_code(VISION_PORT, EXAMPLE_SIG, OTHER_SIG, 0, 0, 0);
 *   while (true) {
 *     vision_read_by_code(VISION_PORT, 0, code1, NUM_VISION_OBJECTS, object_arr);
 *     printf("sig: %d", object_arr[0].signature);
 *     // Prints the signature of the largest object found
 *     delay(2);
 *   }
 * }
 * \endcode
 */
int32_t vision_read_by_code(uint8_t port, const uint32_t size_id, const vision_color_code_t color_code,
                            const uint32_t object_count, vision_object_s_t* const object_arr);

/**
 * Gets the object detection signature with the given id number.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param signature_id
 *        The signature id to read
 *
 * \return A vision_signature_s_t containing information about the signature.
 * 
 * \b Example
 * \code
 * #define VISION_PORT 1
 * #define EXAMPLE_SIG 1
 * 
 * void opcontrol() {
 *   vision_signature_s_t sig = vision_get_signature(VISION_PORT, EXAMPLE_SIG);
 *   vision_print_signature(sig);
 * }
 * \endcode
 */
vision_signature_s_t vision_get_signature(uint8_t port, const uint8_t signature_id);

/**
 * Stores the supplied object detection signature onto the vision sensor.
 *
 * \note This saves the signature in volatile memory, and the signature will be
 * lost as soon as the sensor is powered down.
 *
 * \param port
 *        The V5 port number from 1-21
 * \param signature_id
 *        The signature id to store into
 * \param[in] signature_ptr
 *            A pointer to the signature to save
 *
 * \return 1 if no errors occured, PROS_ERR otherwise
 * 
 * \b Example
 * \code
 * #define VISION_PORT 1
 * #define EXAMPLE_SIG 1
 * 
 * void opcontrol() {
 *   vision_signature_s_t sig = vision_get_signature(VISION_PORT, EXAMPLE_SIG);
 *   sig.range = 10.0;
 *   vision_set_signature(VISION_PORT, EXAMPLE_SIG, &sig);
 * }
 * \endcode
 */
int32_t vision_set_signature(uint8_t port, const uint8_t signature_id, vision_signature_s_t* const signature_ptr);

/**
 * Enables/disables auto white-balancing on the Vision Sensor.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 * EINVAL - enable was not 0 or 1
 *
 * \param port
 * 		    The V5 port number from 1-21
 * \param enabled
 * 		    Pass 0 to disable, 1 to enable
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * #define VISION_PORT 1
 * 
 * void initialize() {
 *   vision_set_auto_white_balance(VISION_PORT, true);
 * }
 * \endcode
 */
int32_t vision_set_auto_white_balance(uint8_t port, const uint8_t enable);

/**
 * Sets the exposure parameter of the Vision Sensor. 
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 *
 * \param port
 *        The V5 port number from 1-21
 * \param percent
 *        The new exposure setting from [0,150]
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * #define VISION_PORT 1
 * 
 * void initialize() {
 *   if (vision_get_exposure(VISION_PORT) < 50)
 *   vision_set_exposure(VISION_PORT, 50);
 * }
 * \endcode
 */
int32_t vision_set_exposure(uint8_t port, const uint8_t exposure);

/**
 * Sets the vision sensor LED color, overriding the automatic behavior.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 *
 * \param port
 *        The V5 port number from 1-21
 * \param rgb
 *        An RGB code to set the LED to
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * #define VISION_PORT 1
 * 
 * void initialize() {
 *   vision_set_led(VISION_PORT, COLOR_BLANCHED_ALMOND);
 * }
 * \endcode
 */
int32_t vision_set_led(uint8_t port, const int32_t rgb);

/**
 * Sets the white balance parameter of the Vision Sensor.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 *
 * \param port
 * 		    The V5 port number from 1-21
 * \param rgb
 *        The new RGB white balance setting of the sensor
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * #define VISION_PORT 1
 * #define VISION_WHITE 0xff
 * 
 * void initialize() {
 *   vision_set_white_balance(VISION_PORT, VISION_WHITE);
 * }
 * \endcode
 */
int32_t vision_set_white_balance(uint8_t port, const int32_t rgb);

/**
 * Sets the (0,0) coordinate for the Field of View.
 *
 * This will affect the coordinates returned for each request for a
 * vision_object_s_t from the sensor, so it is recommended that this function
 * only be used to configure the sensor at the beginning of its use.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a vision sensor
 *
 * \param port
 * 		    The V5 port number from 1-21
 * \param zero_point
 *        One of vision_zero_e_t to set the (0,0) coordinate for the FOV
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * #define VISION_PORT 1
 * 
 * void initialize() {
 *   vision_set_zero_point(VISION_PORT, E_VISION_ZERO_CENTER);
 * }
 * \endcode
 */
int32_t vision_set_zero_point(uint8_t port, vision_zero_e_t zero_point);

/**
 * Sets the Wi-Fi mode of the Vision sensor
 *
 * This functions uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given port is not within the range of V5 ports (1-21)
 * EACCESS - Anothe resources is currently trying to access the port
 *
 * \param port
 *        The V5 port number from 1-21
 * \param enable
 *        Disable Wi-Fi on the Vision sensor if 0, enable otherwise (e.g. 1)
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * #define VISION_PORT 1
 * 
 * void initialize() {
 *   vision_set_wifi_mode(VISION_PORT, 0);
 * }
 * \endcode
 */
int32_t vision_set_wifi_mode(uint8_t port, const uint8_t enable);

///@}

///@}

#ifdef __cplusplus
}  // namespace c
}  // namespace pros
}
#endif

#endif  // _PROS_VISION_H_
