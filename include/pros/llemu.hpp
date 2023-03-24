#ifndef _PROS_LLEMU_HPP_
#define _PROS_LLEMU_HPP_

#include <cstdint>
#include <string>

/******************************************************************************/
/**                                 LLEMU Weak Stubs                         **/
/**                                                                          **/
/**   These functions allow main.cpp to be compiled without LVGL present     **/
/******************************************************************************/
namespace pros {
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

    /**
     * Displays a string on the emulated three-button LCD screen.
     *
     * This function uses the following values of errno when an error state is
     * reached:
     * ENXIO  - The LCD has not been initialized. Call lcd_initialize() first.
     * EINVAL - The line number specified is not in the range [0-7]
     *
     * \param line
     *        The line on which to display the text [0-7]
     * \param text
     *        The text to display
     *
     * \return True if the operation was successful, or false otherwise, setting
     * errno values as specified above.
     */
    extern __attribute__((weak)) bool set_text(std::int16_t line, std::string text);

    /**
     * Clears the contents of a line of the emulated three-button LCD screen.
     *
     * This function uses the following values of errno when an error state is
     * reached:
     * ENXIO  - The LCD has not been initialized. Call lcd_initialize() first.
     * EINVAL - The line number specified is not in the range [0-7]
     *
     * \param line
     *        The line to clear
     *
     * \return True if the operation was successful, or false otherwise, setting
     * errno values as specified above.
     */
    extern __attribute__((weak)) bool clear_line(std::int16_t line);

    /**
     * Creates an emulation of the three-button, UART-based VEX LCD on the display.
     *
     * \return True if the LCD was successfully initialized, or false if it has
     * already been initialized.
     */
    extern __attribute__((weak)) bool initialize(void);

    /**
     * Gets the button status from the emulated three-button LCD.
     *
     * The value returned is a 3-bit integer where 1 0 0 indicates the left button
     * is pressed, 0 1 0 indicates the center button is pressed, and 0 0 1
     * indicates the right button is pressed. 0 is returned if no buttons are
     * currently being pressed.
     *
     * Note that this function is provided for legacy API compatibility purposes,
     * with the caveat that the V5 touch screen does not actually support pressing
     * multiple points on the screen at the same time.
     *
     * \return The buttons pressed as a bit mask
     */
    extern __attribute__((weak)) std::uint8_t read_buttons(void);

    /**
     * Registers a callback function for the rightmost button.
     *
     * When the rightmost button on the emulated three-button LCD is pressed, the
     * user-provided callback function will be invoked.
     *
     * \param cb
     * A callback function of type lcd_btn_cb_fn_t(void (*cb)(void))
     */
    extern __attribute__((weak)) void register_btn1_cb(lcd_btn_cb_fn_t cb);

    /**
     * Checks whether the emulated three-button LCD has already been initialized.
     *
     * \return True if the LCD has been initialized or false if not.
     */
    extern __attribute__((weak)) bool is_initialized(void);

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
     * \param ...
     *        Optional list of arguments for the format string
     *
     * \return True if the operation was successful, or false otherwise, setting
     * errno values as specified above.
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
} // namespace lcd
} // namespace pros

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

#endif // _PROS_LLEMU_HPP_
