/**
 * \file screen.h
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
#endif

/**
 * ! Different font sizes that can be used in printing text.
 */
typedef enum {
    E_TEXT_SMALL = 0, ///< Small text font size
    E_TEXT_MEDIUM, ///< Normal/Medium text font size
    E_TEXT_LARGE, ///< Large text font size
    E_TEXT_MEDIUM_CENTER, ///< Medium centered text
    E_TEXT_LARGE_CENTER ///< Large centered text
} text_format_e_t;

/**
 * ! Enum indicating what the current touch status is for the touchscreen.
 */
typedef enum {
    E_TOUCH_RELEASED = 0, ///< Last interaction with screen was a quick press
    E_TOUCH_PRESSED, ///< Last interaction with screen was a release
    E_TOUCH_HELD, ///< User is holding screen down
    E_TOUCH_ERROR ///< An error occured while taking/returning the mutex
} last_touch_e_t;

/**
 * ! Struct representing screen touch status, screen last x, screen last y, press count, release count.
 */
typedef struct screen_touch_status_s {
    last_touch_e_t touch_status; ///< Represents if the screen is being held, released, or pressed.
    int16_t x; ///< Represents the x value of the location of the touch.
    int16_t y; ///< Represents the y value of the location of the touch.
    int32_t press_count; ///< Represents how many times the screen has be pressed. 
    int32_t release_count; ///< Represents how many times the user released after a touch on the screen.
} screen_touch_status_s_t;

#ifdef PROS_USE_SIMPLE_NAMES
#ifdef __cplusplus
#define TEXT_SMALL pros::E_TEXT_SMALL
#define TEXT_MEDIUM pros::E_TEXT_MEDIUM
#define TEXT_LARGE pros::E_TEXT_LARGE
#define TEXT_MEDIUM_CENTER pros::E_TEXT_MEDIUM_CENTER
#define TEXT_LARGE_CENTER pros::E_TEXT_LARGE_CENTER
#define TOUCH_RELEASED pros::E_TOUCH_RELEASED
#define TOUCH_PRESSED pros::E_TOUCH_PRESSED
#define TOUCH_HELD pros::E_TOUCH_HELD
#else
#define TEXT_SMALL E_TEXT_SMALL
#define TEXT_MEDIUM E_TEXT_MEDIUM
#define TEXT_LARGE E_TEXT_LARGE
#define TEXT_MEDIUM_CENTER E_TEXT_MEDIUM_CENTER
#define TEXT_LARGE_CENTER E_TEXT_LARGE_CENTER
#define TOUCH_RELEASED E_TOUCH_RELEASED
#define TOUCH_PRESSED E_TOUCH_PRESSED
#define TOUCH_HELD E_TOUCH_HELD
#endif
#endif

typedef void (*touch_event_cb_fn_t)();

#ifdef __cplusplus
namespace c {
#endif

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
 *         there was an error either taking or returning the screen mutex.
 */
uint32_t screen_set_pen(uint32_t color);

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
 * \return Returns 1 if the mutex was successfully returned, or 
 * PROS_ERR if there was an error either taking or returning the screen mutex.
 */
uint32_t screen_set_eraser(uint32_t color);

/**
 *  Get the current pen color.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EACCESS - Another resource is currently trying to access the screen mutex.
 * 
 * \return The current pen color in the form of a value from the enum defined 
 *         in colors.h, or PROS_ERR if there was an error taking or returning 
 *         the screen mutex.
 */
uint32_t screen_get_pen(void);

/**
 * Get the current eraser color.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EACCESS - Another resource is currently trying to access the screen mutex.
 *
 * \return The current eraser color in the form of a value from the enum 
 *         defined in colors.h, or PROS_ERR if there was an error taking or 
 *         returning the screen mutex.
 */
uint32_t screen_get_eraser(void);

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
uint32_t screen_erase(void);

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
 *         taking or returning the screen mutex.
 */
uint32_t screen_scroll(int16_t start_line, int16_t lines);

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
 *           taking or returning the screen mutex.
 */
uint32_t screen_scroll_area(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t lines);

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
 * \return 1 if there were no errors, or PROS_ERR if an error occured 
 *         taking or returning the screen mutex.
 */
uint32_t screen_copy_area(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t* buf, int32_t stride);

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
 *         taking or returning the screen mutex.
 */
uint32_t screen_draw_pixel(int16_t x, int16_t y);

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
 *         taking or returning the screen mutex.
 */
uint32_t screen_erase_pixel(int16_t x, int16_t y);

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
 *         taking or returning the screen mutex.
 */
uint32_t screen_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

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
 *         taking or returning the screen mutex.
 */
uint32_t screen_erase_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

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
 *         taking or returning the screen mutex.
 */
uint32_t screen_draw_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

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
 *         taking or returning the screen mutex.
 */
uint32_t screen_erase_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

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
 *         taking or returning the screen mutex.
 */
uint32_t screen_fill_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

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
 *         taking or returning the screen mutex.
 */
uint32_t screen_draw_circle(int16_t x, int16_t y, int16_t radius);

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
 *         taking or returning the screen mutex.
 */
uint32_t screen_erase_circle(int16_t x, int16_t y, int16_t radius);

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
 *         taking or returning the screen mutex.
 */
uint32_t screen_fill_circle(int16_t x, int16_t y, int16_t radius);

/******************************************************************************/
/**                       Screen Text Display Functions                      **/
/**                                                                          **/
/**     These functions allow programmers to display text on the v5 screen   **/
/******************************************************************************/

/**
 * Print a formatted string to the screen on the specified line
 * 
 * Will default to a medium sized font by default if invalid txt_fmt is given.
 * 
 * \param txt_fmt Text format enum that determines if the text is medium, large, medium_center, or large_center. (DOES NOT SUPPORT SMALL)
 * \param line The line number on which to print
 * \param text  Format string
 * \param ...  Optional list of arguments for the format string
 * 
 *  \return 1 if there were no errors, or PROS_ERR if an error occured 
 *          taking or returning the screen mutex.
 */
uint32_t screen_print(text_format_e_t txt_fmt, const int16_t line, const char* text, ...);

/**
 * Print a formatted string to the screen at the specified point
 * 
 * Will default to a medium sized font by default if invalid txt_fmt is given.
 * 
 * Text formats medium_center and large_center will default to medium and large respectively.
 * 
 * \param txt_fmt Text format enum that determines if the text is small, medium, or large.
 * \param x The y coordinate of the top left corner of the string
 * \param y The x coordinate of the top left corner of the string
 * \param text  Format string
 * \param ...  Optional list of arguments for the format string
 * 
 *  \return 1 if there were no errors, or PROS_ERR if an error occured 
 *          taking or returning the screen mutex.
 */
uint32_t screen_print_at(text_format_e_t txt_fmt, const int16_t x, const int16_t y, const char* text, ...);

/**
 * Print a formatted string to the screen on the specified line
 *
 * Same as `display_printf` except that this uses a `va_list` instead of the
 * ellipsis operator so this can be used by other functions.
 * 
 * Will default to a medium sized font by default if invalid txt_fmt is given.
 * Exposed mostly for writing libraries and custom functions.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EACCESS - Another resource is currently trying to access the screen mutex.
 * 
 * \param txt_fmt Text format enum that determines if the text is medium, large, medium_center, or large_center. (DOES NOT SUPPORT SMALL)
 * \param line The line number on which to print
 * \param text  Format string
 * \param args List of arguments for the format string
 * 
 * \return 1 if there were no errors, or PROS_ERR if an error occured 
 *          while taking or returning the screen mutex.
 */
uint32_t screen_vprintf(text_format_e_t txt_fmt, const int16_t line, const char* text, va_list args);

/**
 * Print a formatted string to the screen at the specified coordinates
 *
 * Same as `display_printf_at` except that this uses a `va_list` instead of the
 * ellipsis operator so this can be used by other functions.
 * 
 * Will default to a medium sized font by default if invalid txt_fmt is given.
 * 
 * Text formats medium_center and large_center will default to medium and large respectively.
 * Exposed mostly for writing libraries and custom functions.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EACCESS - Another resource is currently trying to access the screen mutex.
 * 
 * \param txt_fmt Text format enum that determines if the text is small, medium, or large.
 * \param x, y The (x,y) coordinates of the top left corner of the string
 * \param text  Format string
 * \param args List of arguments for the format string
 *  
 * \return 1 if there were no errors, or PROS_ERR if an error occured 
 *          while taking or returning the screen mutex.
 */
uint32_t screen_vprintf_at(text_format_e_t txt_fmt, const int16_t x, const int16_t y, const char* text, va_list args);

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
 * If an error occured, the screen_touch_status_s_t will have its last_touch_e_t
 *  enum specifier set to E_TOUCH_ERR, and other values set to -1.
 */
screen_touch_status_s_t screen_touch_status(void);

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
 *          while taking or returning the screen mutex.
 */
uint32_t screen_touch_callback(touch_event_cb_fn_t cb, last_touch_e_t event_type);

#ifdef __cplusplus
} //namespace c
} //namespace pros
}
#endif  

#endif 
