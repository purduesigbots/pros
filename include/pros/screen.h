/**
 * \file screen.h
 *
 * \brief Brain screen display and touch functions.
 *
 * Contains user calls to the v5 screen for touching and displaying graphics.
 *
 * \copyright (c) 2017, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_SCREEN_H_
#define _PROS_SCREEN_H_

#include <stdarg.h>   
#include <stdbool.h>  
#define _GNU_SOURCE
#include <stdio.h>  
#undef _GNU_SOURCE
#include <stdint.h>

#include "pros/colors.h"     // c color macros

#ifdef __cplusplus
extern "C" {
namespace pros {
namespace c {
#endif

/**
 * \brief Different font sizes that can be used in printing text.
 * Also can determine if the medium/large text is centered or not.
 *  
 *  small - Small text font size
 *  normal - Normal/Medium text font size
 *  large - Large text font size
 *  medium_center - Medium centered text
 *  large_center - Large centered text
 * 
 */

typedef enum {
    small,
    medium,
    large,
    medium_center,
    large_center
} text_format_e_t;

/**
 * \brief Can be used to determine what the current touch status is for the touchscreen.
 * (Seems redundant, but exists for the sake of user ease)
 *  
 *  pressed - Last interaction with screen was a quick press (not released yet)
 *  released - Last interaction with screen was a release
 *  held - User is holding screen down (not released or quick press)
 * 
 */

typedef enum {
    released,
    pressed,
    held
} last_touch_e_t;

/******************************************************************************/
/**                  Screen Graphical Display Functions                      **/
/**                                                                          **/
/**   These functions allow programmers to display shapes on the v5 screen   **/
/******************************************************************************/

/**
 * \brief Set the pen color for subsequent graphics operations
 *
 * \param[in] color	The pen color to set (it is recommended to use values
 * 		 from the enum defined in colors.h)
 */
void screen_set_pen(uint32_t color);

/**
 * \brief Set the eraser color for clearing and the current background.
 *
 * \param[in] color	The background color to set (it is recommended to use values
 * 					from the enum defined in colors.h)
 */
void screen_set_eraser(uint32_t color);

/**
 * \brief  Get the current pen color.
 * 
 * \return The current pen color in the form of a value from the enum defined in colors.h.
 */
uint32_t screen_get_pen(void);

/**
 * \brief Get the current eraser color.
 *
 * \return The current eraser color in the form of a value from the enum defined in colors.h.
 */
uint32_t screen_get_eraser(void);

/**
 * \brief Reset the display to the default black screen
 */
void screen_clear(void);

/**
 * \brief Scroll lines on the display upwards.
 *
 * \param[in] start_line	The line from which scrolling will start
 * \param[in] lines			The number of lines to scroll up
 */
void screen_scroll(int16_t start_line, int16_t lines);

/**
 * \brief Scroll lines within a region on the display
 *
 * This function behaves in the same way as `screen_scroll`, except that you
 * specify a rectangular region within which to scroll lines instead of a start
 * line.
 *
 * \param[in] x0, y0	The (x,y) coordinates of the first corner of the
 * 						rectangular region
 * \param[in] x1, y1	The (x,y) coordinates of the second corner of the
 * 						rectangular region
 * \param[in] lines 	The number of lines to scroll upwards
 */
void screen_scroll_area(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t lines);

/**
 * \brief Copy a screen region (designated by a rectangle) from an off-screen buffer 
 * to the screen
 *
 * \param[in] x0, y0 	The (x,y) coordinates of the first corner of the
 * 						rectangular region of the screen
 * \param[in] x1, y1	The (x,y) coordinates of the second corner of the
 * 						rectangular region of the screen
 * \param[in] buf		Off-screen buffer containing screen data
 * \param[in] stride	Off-screen buffer width in pixels, such that image size
 * 						is stride-padding
 */
void screen_copy_area(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t* buf, int32_t stride);

/**
 * \brief Draw a single pixel on the screen using the current pen color
 *
 * \param[in] x, y 	The (x,y) coordinates of the pixel
 */
void screen_draw_pixel(int16_t x, int16_t y);

/**
 * \brief Clear a pixel from the screen (Sets the location)
 *
 * \param[in] x, y 	The (x,y) coordinates of the cleared
 */
void screen_clear_pixel(int16_t x, int16_t y);

/**
 * \brief Draw a line on the screen using the current pen color
 *
 * \param[in] x0, y0	The (x, y) coordinates of the first point of the line
 * \param[in] x1, y1 	The (x, y) coordinates of the second point of the line
 */
void screen_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

/**
 * \brief Draw a line on the screen using the current background color
 *
 * \param[in] x0, y0	The (x, y) coordinates of the first point of the line
 * \param[in] x1, y1 	The (x, y) coordinates of the second point of the line
 */
void screen_clear_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

/**
 * \brief Draw a rectangle on the screen using the current pen color
 *
 * \param[in] x0, y0 	The (x,y) coordinates of the first point of the rectangle
 * \param[in] x1, y1 	The (x,y) coordinates of the second point of the rectangle
 */
void screen_draw_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

/**
 * \brief Draw a rectangle on the screen using the current eraser color
 *
 * \param[in] x0, y0 	The (x,y) coordinates of the first point of the rectangle
 * \param[in] x1, y1 	The (x,y) coordinates of the second point of the rectangle
 */
void screen_clear_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

/**
 * \brief Fill a rectangular region of the screen using the current pen
 * 		  color
 *
 * \param[in] x0, y0 	The (x,y) coordinates of the first point of the rectangle
 * \param[in] x1, y1 	The (x,y) coordinates of the second point of the rectangle
 */
void screen_fill_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

/**
 * \brief Draw a circle on the screen using the current pen color
 *
 * \param[in] x, y 	The (x,y) coordinates of the center of the circle
 * \param[in] r 	The radius of the circle
 */
void screen_draw_circle(int16_t x, int16_t y, int16_t radius);

/**
 * \brief Draw a circle on the screen using the current eraser color
 *
 * \param[in] x, y 	The (x,y) coordinates of the center of the circle
 * \param[in] r 	The radius of the circle
 */
void screen_clear_circle(int16_t x, int16_t y, int16_t radius);

/**
 * \brief Fill a circular region of the screen using the current pen
 * 		  color
 *
 * \param[in] x, y 	The (x,y) coordinates of the center of the circle
 * \param[in] r 	The radius of the circle
 */
void screen_fill_circle(int16_t x, int16_t y, int16_t radius);

/******************************************************************************/
/**                       Screen Text Display Functions                      **/
/**                                                                          **/
/**     These functions allow programmers to display text on the v5 screen   **/
/******************************************************************************/

/**
 * \brief Print a normal unformatted string to the screen on the specified line
 * 
 * Will default to a medium sized font by default if invalid text_format is given.
 * 
 * \param[in] txt_fmt Text format enum that determines if the text is medium, large, medium_center, large_center. (DOES NOT SUPPORT SMALL)
 * \param[in] line The line number on which to print
 * \param[in] text The text to display
 */
void screen_print(text_format_e_t txt_fmt, const int16_t line, const char* text);

/**
 * \brief Print a normal unformatted string to the screen at the specified coordinates
 * 
 * Will default to a medium sized font by default if invalid text_format is given.
 * 
 * Text formats medium_center and large_center will default to medium and large respectively.
 * 
 * \param[in] txt_fmt Text format enum that determines if the text is small, medium, and large.
 * \param[in] x, y The (x,y) coordinates of the top left corner of the string
 * \param[in] text The text to display
 */
void screen_print_at(text_format_e_t txt_fmt, int16_t x, int16_t y, const char* text);

/**
 * \brief Print a formatted string to the screen on the specified line
 * 
 * Will default to a medium sized font by default if invalid text_format is given.
 * 
 * \param[in] txt_fmt Text format enum that determines if the text is medium, large, medium_center, or large_center. (DOES NOT SUPPORT SMALL)
 * \param[in] line The line number on which to print
 * \param[in] fmt  Format string
 * \param[in] ...  Optional list of arguments for the format string
 */
void screen_printf(text_format_e_t txt_fmt, const int16_t line, const char* text, ...);

/**
 * \brief Print a formatted string to the screen at the specified point
 * 
 * Will default to a medium sized font by default if invalid text_format is given.
 * 
 * Text formats medium_center and large_center will default to medium and large respectively.
 * 
 * \param[in] txt_fmt Text format enum that determines if the text is small, medium, or large.
 * \param[in] x, y The (x,y) coordinates of the top left corner of the string
 * \param[in] fmt  Format string
 * \param[in] ...  Optional list of arguments for the format string
 */
void screen_printf_at(text_format_e_t txt_fmt, const int16_t x, const int16_t y, const char* text, ...);

/**
 * \brief Print a formatted string to the screen on the specified line
 *
 * Same as `display_printf` except that this uses a `va_list` instead of the
 * ellipsis operator so this can be used by other functions.
 * 
 * Will default to a medium sized font by default if invalid text_format is given.
 * Exposed mostly for writing libraries and custom functions.
 * 
 * \param[in] txt_fmt Text format enum that determines if the text is medium, large, medium_center, or large_center. (DOES NOT SUPPORT SMALL)
 * \param[in] line The line number on which to print
 * \param[in] fmt  Format string
 * \param[in] args List of arguments for the format string
 */
void screen_vprintf(text_format_e_t txt_fmt, const int16_t line, const char* fmt, va_list args);

/**
 * \brief Print a formatted string to the screen at the specified coordinates
 *
 * Same as `display_printf_at` except that this uses a `va_list` instead of the
 * ellipsis operator so this can be used by other functions.
 * 
 * Will default to a medium sized font by default if invalid text_format is given.
 * 
 * Text formats medium_center and large_center will default to medium and large respectively.
 * Exposed mostly for writing libraries and custom functions.
 * 
 * \param[in] txt_fmt Text format enum that determines if the text is small, medium, or large.
 * \param[in] x, y The (x,y) coordinates of the top left corner of the string
 * \param[in] fmt  Format string
 * \param[in] args List of arguments for the format string
 */
void screen_vprintf_at(text_format_e_t txt_fmt, const int16_t x, const int16_t y, const char* fmt, va_list args);

/******************************************************************************/
/**                         Screen Touch Functions                           **/
/**                                                                          **/
/**               These functions allow programmers to access                **/
/**                    information about screen touches                      **/
/******************************************************************************/

/**
 * \brief Gets the x coordinate of the last touch. 0 by default.
 * 
 * \return The x coordinate of the last touch.
 */
int16_t screen_last_x(void);

/**
 * \brief Gets the y coordinate of the last touch. 0 by default.
 * 
 * \return The y coordinate of the last touch.
 */
int16_t screen_last_y(void);

/**
 * \brief Gets the touch status of the last touch of the screen.
 * 
 * \return The touch_event_e_t enum specifier that indicates the last touch status of the screen (E_TOUCH_EVENT_RELEASE, E_TOUCH_EVENT_PRESS, or E_TOUCH_EVENT_PRESS_AND_HOLD).
 * This will be released by default if no action was taken. 
 */
last_touch_e_t screen_last_touch_status(void);

#ifdef __cplusplus
} //namespace c
} //namespace pros
}
#endif  // extern "C"

#endif // header guard