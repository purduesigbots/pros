/**
 * \file pros/llemu.hpp
 * \ingroup cpp-llemu
 * 
 * Legacy LCD Emulator
 *
 * \details This file defines a high-level API for emulating the three-button, UART-based
 * VEX LCD, containing a set of functions that facilitate the use of a software-
 * emulated version of the classic VEX LCD module.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_LLEMU_HPP_
#define _PROS_LLEMU_HPP_

#include <cstdint>
#include <string>

/******************************************************************************/
/**                        LLEMU Conditional Include                         **/
/**                                                                          **/
/**   When the libvgl versions of llemu.hpp is present, common.mk will       **/
/**   define a macro which lets this file know that liblvgl's llemu.hpp is   **/
/**   present. If it is, we conditionally include it so that it gets         **/
/**   included into api.h.                                                   **/
/******************************************************************************/
#ifdef _PROS_INCLUDE_LIBLVGL_LLEMU_HPP
#include "liblvgl/llemu.hpp"
#endif

/******************************************************************************/
/**                                 LLEMU Weak Stubs                         **/
/**                                                                          **/
/**   These functions allow main.cpp to be compiled without LVGL present     **/
/******************************************************************************/

namespace pros {
    
/**
 * \ingroup cpp-llemu 
 */
namespace lcd {
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-function"
    namespace {
    template <typename T>
    T convert_args(T arg) {
        return arg;
    }
    const char* convert_args(const std::string& arg) {
        return arg.c_str();
    }
    }  // namespace
    #pragma GCC diagnostic pop

    using lcd_btn_cb_fn_t = void (*)(void);

    /* 
     * These weak symbols allow the example main.cpp in to compile even when 
     * the liblvgl template is missing from the project. 
     * 
     * For documentation on these functions, please see the doxygen comments for
     * these functions in the libvgl llemu headers.
     */

    extern __attribute__((weak)) bool set_text(std::int16_t line, std::string text);
    extern __attribute__((weak)) bool clear_line(std::int16_t line);
    extern __attribute__((weak)) bool initialize(void);
    extern __attribute__((weak)) std::uint8_t read_buttons(void);
    extern __attribute__((weak)) void register_btn1_cb(lcd_btn_cb_fn_t cb);
    extern __attribute__((weak)) bool is_initialized(void);

    /**
     * \addtogroup cpp-llemu
     * @{ 
     */
    
    /*
     * Note: This template resides in this file since the 
     */

    /**
     * Displays a formatted string on the emulated three-button LCD screen.
     *
     * This function uses the following values of errno when an error state is
     * reached:
     * ENXIO  - The LCD has not been initialized. Call lcd_initialize() first.
     * EINVAL - The line number specified is not in the range [0-7]
     *
     * \param line
     *        The line on which to display the text [0-7]
     * \param fmt
     *        Format string
     * \param ...args
     *        Optional list of arguments for the format string
     *
     * \return True if the operation was successful, or false otherwise, setting
     * errno values as specified above.
     * 
     * \b Example
     * \code
     * #include "pros/llemu.hpp"
     * 
     * void initialize() {
     *   pros::lcd::initialize();
     *   pros::lcd::print(0, "My formatted text: %d!", 2);
     * }
     * \endcode
     */
    template <typename... Params>
    bool print(std::int16_t line, const char* fmt, Params... args) {
	    return pros::c::lcd_print(line, fmt, convert_args(args)...);
    }

    #ifndef LCD_BTN_LEFT
        #define LCD_BTN_LEFT 4
    #endif

    #ifndef LCD_BTN_CENTER
        #define LCD_BTN_CENTER 2
    #endif

    #ifndef LCD_BTN_RIGHT
        #define LCD_BTN_RIGHT 1
    #endif
    /// @}
} // namespace lcd
} // namespace pros

#endif // _PROS_LLEMU_HPP_
