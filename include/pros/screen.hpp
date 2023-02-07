/**
 * \file screen.hpp
 *
 * Brain screen display and touch functions.
 *
 * Contains user calls to the v5 screen for touching and displaying graphics.
 *
 * \copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_SCREEN_HPP_
#define _PROS_SCREEN_HPP_

#include "pros/screen.h"
#include <cstdint>
#include <string>

namespace pros {
namespace screen {

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

    /******************************************************************************/
    /**                  Screen Graphical Display Functions                      **/
    /**                                                                          **/
    /**   These functions allow programmers to display shapes on the v5 screen   **/
    /******************************************************************************/

    /**
     * Set the pen color for subsequent graphics operations
     * 
     * This function uses the following values of errno when an error state is
     * reached:
     * EACCESS - Another resource is currently trying to access the screen mutex.
     *
     * \param color	The pen color to set (it is recommended to use values
     * 		 from the enum defined in colors.h)
     * 
     * \return Returns 1 if the mutex was successfully returned, or PROS_ERR if 
     * there was an error either taking or returning the screen mutex.
     */
    std::uint32_t set_pen(const std::uint32_t color);

    /**
     * Set the eraser color for erasing and the current background.
     *
     * This function uses the following values of errno when an error state is
     * reached:
     * EACCESS - Another resource is currently trying to access the screen mutex.
     * 
     * \param color	The background color to set (it is recommended to use values
     * 					from the enum defined in colors.h)
     * 
     * \return Returns 1 if the mutex was successfully returned, or PROS_ERR
     *  if there was an error either taking or returning the screen mutex.
     */
    std::uint32_t set_eraser(const std::uint32_t color);

    /**
     *  Get the current pen color.
     *
     * This function uses the following values of errno when an error state is
     * reached:
     * EACCESS - Another resource is currently trying to access the screen mutex.
     * 
     * \return The current pen color in the form of a value from the enum 
     * defined in colors.h, or PROS_ERR if there was an error taking or 
     * returning the screen mutex.
     */
    std::uint32_t get_pen();

    /**
     * Get the current eraser color.
     *
     * This function uses the following values of errno when an error state is
     * reached:
     * EACCESS - Another resource is currently trying to access the screen mutex.
     *
     * \return The current eraser color in the form of a value from the enum
     *  defined in colors.h, or PROS_ERR if there was an error taking or 
     *  returning the screen mutex.
     */
    std::uint32_t get_eraser();

    /**
     * Clear display with eraser color
     *
     * This function uses the following values of errno when an error state is
     * reached:
     * EACCESS - Another resource is currently trying to access the screen mutex.
     * 
     * \return 1 if there were no errors, or PROS_ERR if an error occured 
     *         taking or returning the screen mutex.
     */
    std::uint32_t erase();

    /**
     * Scroll lines on the display upwards.
     *
     * This function uses the following values of errno when an error state is
     * reached:
     * EACCESS - Another resource is currently trying to access the screen mutex.
     *
     * \param start_line    The line from which scrolling will start
     * \param lines			The number of lines to scroll up
     * 
     * \return 1 if there were no errors, or PROS_ERR if an error occured
     *  taking or returning the screen mutex.
     */
    std::uint32_t scroll(const std::int16_t start_line, const std::int16_t lines);

    /**
     * Scroll lines within a region on the display
     *
     * This function behaves in the same way as `screen_scroll`, except that you
     * specify a rectangular region within which to scroll lines instead of a start
     * line.
     *
     * This function uses the following values of errno when an error state is
     * reached:
     * EACCESS - Another resource is currently trying to access the screen mutex.
     *
     * \param x0, y0	The (x,y) coordinates of the first corner of the
     * 						rectangular region
     * \param x1, y1	The (x,y) coordinates of the second corner of the
     * 						rectangular region
     * \param lines 	The number of lines to scroll upwards
     * 
     * \return 1 if there were no errors, or PROS_ERR if an error occured 
     * taking or returning the screen mutex.
     */
    std::uint32_t scroll_area(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1, std::int16_t lines);

    /**
     * Copy a screen region (designated by a rectangle) from an off-screen buffer 
     * to the screen
     *
     * This function uses the following values of errno when an error state is
     * reached:
     * EACCESS - Another resource is currently trying to access the screen mutex.
     *
     * \param x0, y0 	The (x,y) coordinates of the first corner of the
     * 						rectangular region of the screen
     * \param x1, y1	The (x,y) coordinates of the second corner of the
     * 						rectangular region of the screen
     * \param buf		Off-screen buffer containing screen data
     * \param stride	Off-screen buffer width in pixels, such that image size
     * 						is stride-padding
     * 
     * \return 1 if there were no errors, or PROS_ERR if an error occured taking
     *  or returning the screen mutex.
     */
    std::uint32_t copy_area(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1, uint32_t* buf, const std::int32_t stride);

    /**
     * Draw a single pixel on the screen using the current pen color
     *
     * This function uses the following values of errno when an error state is
     * reached:
     * EACCESS - Another resource is currently trying to access the screen mutex.
     *
     * \param x, y 	The (x,y) coordinates of the pixel
     * 
     * \return 1 if there were no errors, or PROS_ERR if an error occured 
     * taking or returning the screen mutex.
     */
    std::uint32_t draw_pixel(const std::int16_t x, const std::int16_t y);

    /**
     * Erase a pixel from the screen (Sets the location)
     *
     * This function uses the following values of errno when an error state is
     * reached:
     * EACCESS - Another resource is currently trying to access the screen mutex.
     *
     * \param x, y 	The (x,y) coordinates of the erased
     * 
     * \return 1 if there were no errors, or PROS_ERR if an error occured 
     * taking or returning the screen mutex.
     */
    std::uint32_t erase_pixel(const std::int16_t x, const std::int16_t y);

    /**
     * Draw a line on the screen using the current pen color
     *
     * This function uses the following values of errno when an error state is
     * reached:
     * EACCESS - Another resource is currently trying to access the screen mutex.
     *
     * \param x0, y0	The (x, y) coordinates of the first point of the line
     * \param x1, y1 	The (x, y) coordinates of the second point of the line
     * 
     * \return 1 if there were no errors, or PROS_ERR if an error occured 
     * taking or returning the screen mutex.
     */
    std::uint32_t draw_line(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1);

    /**
     * Erase a line on the screen using the current eraser color
     *
     * This function uses the following values of errno when an error state is
     * reached:
     * EACCESS - Another resource is currently trying to access the screen mutex.
     *
     * \param x0, y0	The (x, y) coordinates of the first point of the line
     * \param x1, y1 	The (x, y) coordinates of the second point of the line
     * 
     * \return 1 if there were no errors, or PROS_ERR if an error occured 
     * taking or returning the screen mutex.
     */
    std::uint32_t erase_line(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1);

    /**
     * Draw a rectangle on the screen using the current pen color
     *
     * This function uses the following values of errno when an error state is
     * reached:
     * EACCESS - Another resource is currently trying to access the screen mutex.
     *
     * \param x0, y0 	The (x,y) coordinates of the first point of the rectangle
     * \param x1, y1 	The (x,y) coordinates of the second point of the rectangle
     * 
     * \return 1 if there were no errors, or PROS_ERR if an error occured 
     * taking or returning the screen mutex.
     */
    std::uint32_t draw_rect(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1);

    /**
     * Erase a rectangle on the screen using the current eraser color
     *
     * This function uses the following values of errno when an error state is
     * reached:
     * EACCESS - Another resource is currently trying to access the screen mutex.
     *
     * \param x0, y0 	The (x,y) coordinates of the first point of the rectangle
     * \param x1, y1 	The (x,y) coordinates of the second point of the rectangle
     * 
     * \return 1 if there were no errors, or PROS_ERR if an error occured 
     * taking or returning the screen mutex.
     */
    std::uint32_t erase_rect(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1);

    /**
     * Fill a rectangular region of the screen using the current pen
     * 		  color
     *
     * This function uses the following values of errno when an error state is
     * reached:
     * EACCESS - Another resource is currently trying to access the screen mutex.
     *
     * \param x0, y0 	The (x,y) coordinates of the first point of the rectangle
     * \param x1, y1 	The (x,y) coordinates of the second point of the rectangle
     * 
     * \return 1 if there were no errors, or PROS_ERR if an error occured 
     * taking or returning the screen mutex.
     */
    std::uint32_t fill_rect(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1);

    /**
     * Draw a circle on the screen using the current pen color
     *
     * This function uses the following values of errno when an error state is
     * reached:
     * EACCESS - Another resource is currently trying to access the screen mutex.
     *
     * \param x, y 	The (x,y) coordinates of the center of the circle
     * \param r 	The radius of the circle
     * 
     * \return 1 if there were no errors, or PROS_ERR if an error occured 
     * taking or returning the screen mutex.
     */
    std::uint32_t draw_circle(const std::int16_t x, const std::int16_t y, const std::int16_t radius);

    /**
     * Erase a circle on the screen using the current eraser color
     *
     * This function uses the following values of errno when an error state is
     * reached:
     * EACCESS - Another resource is currently trying to access the screen mutex.
     *
     * \param x, y 	The (x,y) coordinates of the center of the circle
     * \param r 	The radius of the circle
     * 
     * \return 1 if there were no errors, or PROS_ERR if an error occured 
     * taking or returning the screen mutex.
     */
    std::uint32_t erase_circle(const std::int16_t x, const std::int16_t y, const std::int16_t radius);

    /**
     * Fill a circular region of the screen using the current pen
     * 		  color
     *
     * This function uses the following values of errno when an error state is
     * reached:
     * EACCESS - Another resource is currently trying to access the screen mutex.
     *
     * \param x, y 	The (x,y) coordinates of the center of the circle
     * \param r 	The radius of the circle
     * 
     * \return 1 if there were no errors, or PROS_ERR if an error occured 
     * taking or returning the screen mutex.
     */
    std::uint32_t fill_circle(const std::int16_t x, const std::int16_t y, const std::int16_t radius);

    /******************************************************************************/
    /**                       Screen Text Display Functions                      **/
    /**                                                                          **/
    /**     These functions allow programmers to display text on the v5 screen   **/
    /******************************************************************************/

    /**
     * Print a formatted string to the screen, overwrite available for printing at location too.
     * 
     * Will default to a medium sized font by default if invalid txt_fmt is given.
     * 
     * \param txt_fmt Text format enum that determines if the text is medium, large, medium_center, or large_center. (DOES NOT SUPPORT SMALL)
     * \param line The line number on which to print
     * \param x The (x,y) coordinates of the top left corner of the string
     * \param y The (x,y) coordinates of the top left corner of the string
     * \param fmt  Format string
     * \param ...  Optional list of arguments for the format string
     */
    template <typename... Params>
    void print(pros::text_format_e_t txt_fmt, const std::int16_t line, const char* text, Params... args){
	    pros::c::screen_print(txt_fmt, line, text, convert_args(args)...);
    }

    template <typename... Params>
    void print(pros::text_format_e_t txt_fmt, const std::int16_t x, const std::int16_t y, const char* text, Params... args){
	    pros::c::screen_print_at(txt_fmt, x, y, text, convert_args(args)...);
    }
    
    /******************************************************************************/
    /**                         Screen Touch Functions                           **/
    /**                                                                          **/
    /**               These functions allow programmers to access                **/
    /**                    information about screen touches                      **/
    /******************************************************************************/
    
   /**
     * Gets the touch status of the last touch of the screen.
     * 
     * \return The last_touch_e_t enum specifier that indicates the last touch status of the screen (E_TOUCH_EVENT_RELEASE, E_TOUCH_EVENT_PRESS, or E_TOUCH_EVENT_PRESS_AND_HOLD).
     * This will be released by default if no action was taken. 
     * If an error occured, the screen_touch_status_s_t will have its 
     * last_touch_e_t enum specifier set to E_TOUCH_ERR, and other values set to -1.
     */
    screen_touch_status_s_t touch_status();
    
    /**
     * Assigns a callback function to be called when a certain touch event happens.
     *
     * This function uses the following values of errno when an error state is
     * reached:
     * EACCESS - Another resource is currently trying to access the screen mutex.
     * 
     * \param cb Function pointer to callback when event type happens
     * \param event_type Touch event that will trigger the callback.
     * 
     * \return 1 if there were no errors, or PROS_ERR if an error occured 
     * while taking or returning the screen mutex.
     */
    std::uint32_t touch_callback(touch_event_cb_fn_t cb, last_touch_e_t event_type);

} //namespace screen
} //namespace pros

#endif //header guard
