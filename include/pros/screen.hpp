/**
 * \file screen.hpp
 *
 * Brain screen display and touch functions.
 *
 * Contains user calls to the v5 screen for touching and displaying graphics.
 *
 * \copyright (c) 2017, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_SCREEN_HPP_
#define _PROS_SCREEN_HPP_

#include "pros/screen.h"
#include <cstdint>

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

namespace pros {
namespace screen {

    /******************************************************************************/
    /**                  Screen Graphical Display Functions                      **/
    /**                                                                          **/
    /**   These functions allow programmers to display shapes on the v5 screen   **/
    /******************************************************************************/

    /**
     * Set the pen color for subsequent graphics operations
     *
     * \param color	The pen color to set (it is recommended to use values
     * 		 from the enum defined in colors.h)
     */
    void set_pen(const std::uint32_t color);

    /**
     * Set the eraser color for clearing and the current background.
     *
     * \param color	The background color to set (it is recommended to use values
     * 					from the enum defined in colors.h)
     */
    void set_eraser(const std::uint32_t color);

    /**
     *  Get the current pen color.
     * 
     * \return The current pen color of the screen object in the form of a value from the enum defined in colors.h.
     */
    std::uint32_t get_pen(void);

    /**
     * Get the current eraser color.
     *
     * \return The current eraser color of the screen object in the form of a value from the enum defined in colors.h.
     */
    std::uint32_t get_eraser(void);

    /**
     * Reset the display to the default black screen
     */
    void screen_clear(void);

    /**
     * Scroll lines on the display upwards.
     *
     * \param start_line	The line from which scrolling will start
     * \param lines			The number of lines to scroll up
     */
    void scroll(const std::int16_t start_line, const std::int16_t lines);

    /**
     * Scroll lines within a region on the display
     *
     * This function behaves in the same way as `screen_scroll`, except that you
     * specify a rectangular region within which to scroll lines instead of a start
     * line.
     *
     * \param x0, y0	The (x,y) coordinates of the first corner of the
     * 						rectangular region
     * \param x1, y1	The (x,y) coordinates of the second corner of the
     * 						rectangular region
     * \param lines 	The number of lines to scroll upwards
     */
    void scroll_area(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1, std::int16_t lines);

    /**
     * Copy a screen region (designated by a rectangle) from an off-screen buffer 
     * to the screen
     *
     * \param x0, y0 	The (x,y) coordinates of the first corner of the
     * 						rectangular region of the screen
     * \param x1, y1	The (x,y) coordinates of the second corner of the
     * 						rectangular region of the screen
     * \param buf		Off-screen buffer containing screen data
     * \param stride	Off-screen buffer width in pixels, such that image size
     * 						is stride-padding
     */
    void copy_area(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1, uint32_t* buf, const std::int32_t stride);

    /**
     * Draw a single pixel on the screen using the current pen color
     *
     * \param x, y 	The (x,y) coordinates of the pixel
     */
    void draw_pixel(const std::int16_t x, const std::int16_t y);

    /**
     * Clear a pixel from the screen (Sets the location)
     *
     * \param x, y 	The (x,y) coordinates of the cleared
     */
    void clear_pixel(const std::int16_t x, const std::int16_t y);

    /**
     * Draw a line on the screen using the current pen color
     *
     * \param x0, y0	The (x, y) coordinates of the first point of the line
     * \param x1, y1 	The (x, y) coordinates of the second point of the line
     */
    void draw_line(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1);

    /**
     * Draw a line on the screen using the current background color
     *
     * \param x0, y0	The (x, y) coordinates of the first point of the line
     * \param x1, y1 	The (x, y) coordinates of the second point of the line
     */
    void clear_line(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1);

    /**
     * Draw a rectangle on the screen using the current pen color
     *
     * \param x0, y0 	The (x,y) coordinates of the first point of the rectangle
     * \param x1, y1 	The (x,y) coordinates of the second point of the rectangle
     */
    void draw_rect(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1);

    /**
     * Draw a rectangle on the screen using the current eraser color
     *
     * \param x0, y0 	The (x,y) coordinates of the first point of the rectangle
     * \param x1, y1 	The (x,y) coordinates of the second point of the rectangle
     */
    void clear_rect(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1);

    /**
     * Fill a rectangular region of the screen using the current pen
     * 		  color
     *
     * \param x0, y0 	The (x,y) coordinates of the first point of the rectangle
     * \param x1, y1 	The (x,y) coordinates of the second point of the rectangle
     */
    void fill_rect(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1);

    /**
     * Draw a circle on the screen using the current pen color
     *
     * \param x, y 	The (x,y) coordinates of the center of the circle
     * \param r 	The radius of the circle
     */
    void draw_circle(const std::int16_t x, const std::int16_t y, const std::int16_t radius);

    /**
     * Draw a circle on the screen using the current eraser color
     *
     * \param x, y 	The (x,y) coordinates of the center of the circle
     * \param r 	The radius of the circle
     */
    void clear_circle(const std::int16_t x, const std::int16_t y, const std::int16_t radius);

    /**
     * Fill a circular region of the screen using the current pen
     * 		  color
     *
     * \param x, y 	The (x,y) coordinates of the center of the circle
     * \param r 	The radius of the circle
     */
    void fill_circle(const std::int16_t x, const std::int16_t y, const std::int16_t radius);

    /******************************************************************************/
    /**                       Screen Text Display Functions                      **/
    /**                                                                          **/
    /**     These functions allow programmers to display text on the v5 screen   **/
    /******************************************************************************/

    /**
     * Print a formatted string to the screen, overwrite available for printing at location too.
     * 
     * Will default to a medium sized font by default if invalid text_format is given.
     * 
     * \param txt_fmt Text format enum that determines if the text is medium, large, medium_center, or large_center. (DOES NOT SUPPORT SMALL)
     * \param line The line number on which to print
     * \param x, y The (x,y) coordinates of the top left corner of the string
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
     * Gets the x coordinate of the last touch.
     * 
     * \return The x coordinate of the last touch.
     */
    std::int16_t last_x(void);

    /**
     * Gets the y coordinate of the last touch. 0 by default.
     * 
     * \return The y coordinate of the last touch.
     */
    std::int16_t last_y(void);

    /**
     * Gets the touch status of the last touch of the screen. 0 by default.
     * 
     * \return The touch_event_e_t enum specifier that indicates the last touch status of the screen (E_TOUCH_EVENT_RELEASE, E_TOUCH_EVENT_PRESS, or E_TOUCH_EVENT_PRESS_AND_HOLD). 
     */
    pros::last_touch_e_t touch_status(void);
    
} //namespace screen
} //namespace pros

#endif //header guard