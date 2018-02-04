/*
 * \file tmei.h
 *
 * \brief Touch Management Engine/Interface layer 0
 *
 * This file represents a low-level interface for interacting with the built-in
 * LCD touch screen.
 *
 * \copyright (c) 2017, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_TMEI_H_
#define _PROS_TMEI_H_

#include <stdarg.h>   // va_list, va_start, va_end
#include <stdbool.h>  // bool
#define _GNU_SOURCE
// NOTE: this would normally be in the C file, but it won't compile that way
#include <stdio.h>  // vasprintf (GNU extension)
#undef _GNU_SOURCE

#include "pros/colors.h"  // color macros

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Set the foreground color for subsequent graphics operations
 *
 * \param[in] color	The foreground color to set (it is recommended to use values
 * 					from the enum defined in colors.h)
 */
void display_set_color_fg(uint32_t color);
/**
 * \brief Set the background color for subsequent graphics operations
 *
 * \param[in] color	The background color to set (it is recommended to use values
 * 					from the enum defined in colors.h)
 */
void display_set_color_bg(uint32_t color);

/**
 * \brief Reset the display to the default black screen
 */
void display_erase(void);

/**
 * \brief Scroll lines on the display
 *
 * \param[in] start_line	The line from which scrolling will start
 * \param[in] lines			The number of lines to scroll
 */
void display_scroll(int16_t start_line, int16_t lines);
/**
 * \brief Scroll lines within a region on the display
 *
 * This function behaves in the same way as `display_scroll`, except that you
 * specify a rectangular region within which to scroll lines instead of a start
 * line.
 *
 * \param[in] x0, y0	The (x,y) coordinates of the first corner of the
 * 						rectangular region
 * \param[in] x1, y1	The (x,y) coordinates of the second corner of the
 * 						rectangular region
 * \param[in] lines 	The number of lines to scroll
 */
void display_scroll_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t lines);

/**
 * \brief Copy a screen region from an off-screen buffer to the screen
 *
 * \param[in] x0, y0 	The (x,y) coordinates of the first corner of the
 * 						rectangular region of the screen
 * \param[in] x1, y1	The (x,y) coordinates of the second corner of the
 * 						rectangular region of the screen
 * \param[in] buf		Off-screen buffer containing screen data
 * \param[in] stride	Off-screen buffer width in pixels, such that image size
 * 						is stride-padding
 */
void display_copy_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t* buf, int32_t stride);

/**
 * \brief Draw a pixel on the screen using the current foreground color
 *
 * \param[in] x, y 	The (x,y) coordinates of the pixel
 */
void display_draw_pixel(int16_t x, int16_t y);
/**
 * \brief Draw a pixel on the screen using the current background color
 *
 * \param[in] x, y 	The (x,y) coordinates of the pixel
 */
void display_clear_pixel(int16_t x, int16_t y);

/**
 * \brief Draw a line on the screen using the current foreground color
 *
 * \param[in] x0, y0	The (x, y) coordinates of the first point of the line
 * \param[in] x1, y1 	The (x, y) coordinates of the second point of the line
 */
void display_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
/**
 * \brief Draw a line on the screen using the current background color
 *
 * \param[in] x0, y0	The (x, y) coordinates of the first point of the line
 * \param[in] x1, y1 	The (x, y) coordinates of the second point of the line
 */
void display_clear_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

/**
 * \brief Draw a rectangle on the screen using the current foreground color
 *
 * \param[in] x0, y0 	The (x,y) coordinates of the first point of the rectangle
 * \param[in] x1, y1 	The (x,y) coordinates of the second point of the rectangle
 */
void display_draw_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
/**
 * \brief Draw a rectangle on the screen using the current background color
 *
 * \param[in] x0, y0 	The (x,y) coordinates of the first point of the rectangle
 * \param[in] x1, y1 	The (x,y) coordinates of the second point of the rectangle
 */
void display_clear_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
/**
 * \brief Fill a rectangular region of the screen using the current foreground
 * 		  color
 *
 * \param[in] x0, y0 	The (x,y) coordinates of the first point of the rectangle
 * \param[in] x1, y1 	The (x,y) coordinates of the second point of the rectangle
 */
void display_fill_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

/**
 * \brief Draw a circle on the screen using the current foreground color
 *
 * \param[in] x, y 	The (x,y) coordinates of the center of the circle
 * \param[in] r 	The radius of the circle
 */
void display_draw_circle(int16_t x, int16_t y, int16_t r);
/**
 * \brief Draw a circle on the screen using the current background color
 *
 * \param[in] x, y 	The (x,y) coordinates of the center of the circle
 * \param[in] r 	The radius of the circle
 */
void display_clear_circle(int16_t x, int16_t y, int16_t r);
/**
 * \brief Fill a circular region of the screen using the current foreground
 * 		  color
 *
 * \param[in] x, y 	The (x,y) coordinates of the center of the circle
 * \param[in] r 	The radius of the circle
 */
void display_fill_circle(int16_t x, int16_t y, int16_t r);

/**
 * \brief Print a formatted string to the screen on the specified line
 *
 * \param[in] line The line number on which to print
 * \param[in] fmt  Format string
 * \param[in] ...  Optional list of arguments for the format string
 */
void display_printf(const int16_t line, const char* fmt, ...);
/**
 * \brief Print a formatted string to the screen at the specified coordinates
 *
 * \param[in] x, y The (x,y) coordinates of the top left corner of the string
 * \param[in] fmt  Format string
 * \param[in] ...  Optional list of arguments for the format string
 */
void display_printf_at(int16_t x, int16_t y, const char* fmt, ...);
/**
 * \brief Print a large, formatted string to the screen on the specified line
 *
 * \param[in] line The line number on which to print
 * \param[in] fmt  Format string
 * \param[in] ...  Optional list of arguments for the format string
 */
void display_big_printf(const int16_t line, const char* fmt, ...);
/**
 * \brief Print a large, formatted string to the screen at the specified
 * 		  coordinates
 *
 * \param[in] x, y The (x, y) coordinates of the top left corner of the string
 * \param[in] fmt  Format string
 * \param[in] ...  Optional list of arguments for the format string
 */
void display_big_printf_at(int16_t x, int16_t y, const char* fmt, ...);
/**
 * \brief Print a small, formatted string to the screen at the specified
 * 		  coordinates
 *
 * \param[in] x, y The (x, y) coordinates of the top left corner of the string
 * \param[in] fmt  Format string
 * \param[in] ...  Optional list of arguments for the format string
 */
void display_small_printf_at(int16_t x, int16_t y, const char* fmt, ...);
/**
 * \brief Print a centered, formatted string to the screen on the specified line
 *
 * \param[in] line The line number on which to print
 * \param[in] fmt  Format string
 * \param[in] ...  Optional list of arguments for the format string
 */
void display_center_printf(const int16_t line, const char* fmt, ...);
/**
 * \brief Print a large, centered, formatted string to the screen on the specified
 * 		  line
 *
 * \param[in] line The line number on which to print
 * \param[in] fmt  Format string
 * \param[in] ...  Optional list of arguments for the format string
 */
void display_center_big_printf(const int16_t line, const char* fmt, ...);
/**
 * \brief Print a string to the screen on the specified line
 *
 * \param[in] line The line number on which to print
 * \param[in] text The text to display
 */
void display_puts(const int16_t line, const char* text);
/**
 * \brief Print a string to the screen at the specified coordinates
 *
 * \param[in] x, y The (x,y) coordinates of the top left corner of the string
 * \param[in] text The text to display
 */
void display_puts_at(int16_t x, int16_t y, const char* text);
/**
 * \brief Print a large string to the screen on the specified line
 *
 * \param[in] line The line number on which to print
 * \param[in] text The text to display
 */
void display_big_puts(const int16_t line, const char* text);
/**
 * \brief Print a large string to the screen at the specified coordinates
 *
 * \param[in] x, y The (x,y) coordinates of the top left corner of the string
 * \param[in] text The text to display
 */
void display_big_puts_at(int16_t x, int16_t y, const char* text);
/**
 * \brief Print a small string to the screen at the specified coordinates
 *
 * \param[in] x, y The (x,y) coordinates of the top left corner of the string
 * \param[in] text The text to display
 */
void display_small_puts_at(int16_t x, int16_t y, const char* text);
/**
 * \brief Print a centered string on the specified line
 *
 * \param[in] line The line number on which to print
 * \param[in] text The text to display
 */
void display_center_puts(const int16_t line, const char* text);
/**
 * \brief Print a large, centered string on the specified line
 *
 * \param[in] line The line number on which to print
 * \param[in] text The text to display
 */
void display_center_big_puts(const int16_t line, const char* text);
/**
 * \brief Print a formatted string to the screen on the specified line
 *
 * Same as `display_printf` except that this uses a `va_list` instead of the
 * ellipsis operator so this can be used by other functions.
 *
 * \param[in] line The line number on which to print
 * \param[in] fmt  Format string
 * \param[in] args List of arguments for the format string
 */
void display_vprintf(const int16_t line, const char* fmt, va_list args);
/**
 * \brief Print a formatted string to the screen at the specified coordinates
 *
 * Same as `display_printf_at` except that this uses a `va_list` instead of the
 * ellipsis operator so this can be used by other functions.
 *
 * \param[in] x, y The (x,y) coordinates of the top left corner of the string
 * \param[in] fmt  Format string
 * \param[in] args List of arguments for the format string
 */
void display_vprintf_at(int16_t x, int16_t y, const char* fmt, va_list args);
/**
 * \brief Print a large, formatted string to the screen on the specified line
 *
 * Same as `display_big_printf` except that this uses a `va_list` instead of the
 * ellipsis operator so this can be used by other functions.
 *
 * \param[in] line The line number on which to print
 * \param[in] fmt  Format string
 * \param[in] args List of arguments for the format string
 */
void display_big_vprintf(const int16_t line, const char* fmt, va_list args);
/**
 * \brief Print a large, formatted string to the screen at the specified coordinates
 *
 * Same as `display_big_printf_at` except that this uses a `va_list` instead of the
 * ellipsis operator so this can be used by other functions.
 *
 * \param[in] x, y The (x,y) coordinates of the top left corner of the string
 * \param[in] fmt  Format string
 * \param[in] args List of arguments for the format string
 */
void display_big_vprintf_at(int16_t x, int16_t y, const char* fmt, va_list args);
/**
 * \brief Print a small, formatted string to the screen at the specified coordinates
 *
 * Same as `display_small_printf_at` except that this uses a `va_list` instead of the
 * ellipsis operator so this can be used by other functions.
 *
 * \param[in] x, y The (x,y) coordinates of the top left corner of the string
 * \param[in] fmt  Format string
 * \param[in] args List of arguments for the format string
 */
void display_small_vprintf_at(int16_t x, int16_t y, const char* fmt, va_list args);
/**
 * \brief Print a centered, formatted string to the screen on the specified line
 *
 * Same as `display_printf` except that this uses a `va_list` instead of the
 * ellipsis operator so this can be used by other functions.
 *
 * \param[in] line The line number on which to print
 * \param[in] fmt  Format string
 * \param[in] args List of arguments for the format string
 */
void display_center_vprintf(const int16_t line, const char* fmt, va_list args);
/**
 * \brief Print a large, centered, formatted string to the screen on the specified line
 *
 * Same as `display_printf` except that this uses a `va_list` instead of the
 * ellipsis operator so this can be used by other functions.
 *
 * \param[in] line The line number on which to print
 * \param[in] fmt  Format string
 * \param[in] args List of arguments for the format string
 */
void display_center_big_vprintf(const int16_t line, const char* fmt, va_list args);

typedef enum touch_event_e { E_TOUCH_EVENT_RELEASE, E_TOUCH_EVENT_PRESS, E_TOUCH_EVENT_PRESS_AND_HOLD } touch_event_e_t;

typedef void (*touch_event_cb_fn_t)(int16_t, int16_t);
typedef struct touch_event_position_data_s {
	int16_t x;
	int16_t y;
} touch_event_position_data_s_t;

/**
 * \brief Register a user-defined callback function for an event type
 *
 * When an event of the specified type is fired, the user-provided callback will
 * be invoked.
 *
 * \param[in] cb 			A callback function of type `touch_event_cb_fn_t`
 * 							(`void (*cb)(int16_t, int16_t)`)
 * \param[in] event_type 	The desired type of event to listen for
 */
void register_touch_callback(touch_event_cb_fn_t cb, touch_event_e_t event_type);

#ifdef __cplusplus
}
#endif  // extern "C"

#endif