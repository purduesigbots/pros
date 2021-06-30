/*
 * \file graphics.h
 *
 * Graphical interface with the v5 brain
 *
 * This file represents a low-level interface for interacting with the built-in
 * LCD touch screen.
 *
 * \copyright (c) 2017-2021, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <stdarg.h>   // va_list, va_start, va_end
#include <stdbool.h>  // bool
#define _GNU_SOURCE
#include <stdio.h>  // vasprintf (GNU extension)
#undef _GNU_SOURCE

#include "kapi.h"

#include "v5_apitypes.h"  // V5_TouchEvent

#include "common/linkedlist.h"  // linked list impl
#include "pros/colors.h"     

/**
 * Set the foreground color for subsequent graphics operations
 *
 * \param color	The foreground color to set (it is recommended to use values
 * 					from the enum defined in colors.h)
 */
void display_set_color_fg(uint32_t color);

/**
 * Set the background color for subsequent graphics operations
 *
 * \param color	The background color to set (it is recommended to use values
 * 					from the enum defined in colors.h)
 */
void display_set_color_bg(uint32_t color);

/**
 * Get the foreground color for subsequent graphics operations
 *
 * \return color	The foreground color to set (it is recommended to us
 */
uint32_t display_get_color_fg(void);

/**
 * Get the background color for subsequent graphics operations
 *
 * \return color	The background color that the background color is set to.
 */
uint32_t display_get_color_bg(void);

/**
 * Reset the display to the default black screen
 */
void display_erase(void);

/**
 * Scroll lines on the display
 *
 * \param start_line	The line from which scrolling will start
 * \param lines			The number of lines to scroll
 */
void display_scroll(int16_t start_line, int16_t lines);

/**
 * Scroll lines within a region on the display
 *
 * This function behaves in the same way as `display_scroll`, except that you
 * specify a rectangular region within which to scroll lines instead of a start
 * line.
 *
 * \param x0, y0	The (x,y) coordinates of the first corner of the
 * 						rectangular region
 * \param x1, y1	The (x,y) coordinates of the second corner of the
 * 						rectangular region
 * \param lines 	The number of lines to scroll
 */
void display_scroll_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t lines);

/**
 * Copy a screen region from an off-screen buffer to the screen
 *
 * \param x0, y0 	The (x,y) coordinates of the first corner of the
 * 						rectangular region of the screen
 * \param x1, y1	The (x,y) coordinates of the second corner of the
 * 						rectangular region of the screen
 * \param buf		Off-screen buffer containing screen data
 * \param stride	Off-screen buffer width in pixels, such that image size
 * 						is stride-padding
 */
void display_copy_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t* buf, int32_t stride);

/**
 * Draw a pixel on the screen using the current foreground color
 *
 * \param x, y 	The (x,y) coordinates of the pixel
 */
void display_set_pixel_fg(int16_t x, int16_t y);

/**
 * Draw a pixel on the screen using the current background color
 *
 * \param x, y 	The (x,y) coordinates of the pixel
 */
void display_set_pixel_bg(int16_t x, int16_t y);

/**
 * Draw a line on the screen using the current foreground color
 *
 * \param x0, y0	The (x, y) coordinates of the first point of the line
 * \param x1, y1 	The (x, y) coordinates of the second point of the line
 */
void display_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

/**
 * Draw a line on the screen using the current background color
 *
 * \param x0, y0	The (x, y) coordinates of the first point of the line
 * \param x1, y1 	The (x, y) coordinates of the second point of the line
 */
void display_clear_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

/**
 * Draw a rectangle on the screen using the current foreground color
 *
 * \param x0, y0 	The (x,y) coordinates of the first point of the rectangle
 * \param x1, y1 	The (x,y) coordinates of the second point of the rectangle
 */
void display_draw_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

/**
 * Draw a rectangle on the screen using the current background color
 *
 * \param x0, y0 	The (x,y) coordinates of the first point of the rectangle
 * \param x1, y1 	The (x,y) coordinates of the second point of the rectangle
 */
void display_clear_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

/**
 * Fill a rectangular region of the screen using the current foreground
 * 		  color
 *
 * \param x0, y0 	The (x,y) coordinates of the first point of the rectangle
 * \param x1, y1 	The (x,y) coordinates of the second point of the rectangle
 */
void display_fill_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

/**
 * Draw a circle on the screen using the current foreground color
 *
 * \param x, y 	The (x,y) coordinates of the center of the circle
 * \param r 	The radius of the circle
 */
void display_draw_circle(int16_t x, int16_t y, int16_t r);

/**
 * Draw a circle on the screen using the current background color
 *
 * \param x, y 	The (x,y) coordinates of the center of the circle
 * \param r 	The radius of the circle
 */
void display_clear_circle(int16_t x, int16_t y, int16_t r);

/**
 * Fill a circular region of the screen using the current foreground
 * 		  color
 *
 * \param x, y 	The (x,y) coordinates of the center of the circle
 * \param r 	The radius of the circle
 */
void display_fill_circle(int16_t x, int16_t y, int16_t r);

/**
 * Print a formatted string to the screen on the specified line
 *
 * Same as `display_printf` except that this uses a `va_list` instead of the
 * ellipsis operator so this can be used by other functions.
 * 
 * Will default to a medium sized font by default if invalid text_format is given.
 * Exposed mostly for writing libraries and custom functions.
 * 
 * \param txt_fmt Text format num that correspondes with a higher level enum
 * \param x,The y coordinates of the top left corner of the string
 * \param y The y coordinates of the top left corner of the string
 * \param fmt  Format string
 * \param args List of arguments for the format string
 */
void display_vprintf_at(uint8_t txt_fmt, const int16_t x, const int16_t y, const char* fmt, va_list args);

/**
 * Print a formatted string to the screen on the specified line
 *
 * Same as `display_printf` except that this uses a `va_list` instead of the
 * ellipsis operator so this can be used by other functions.
 * 
 * Will default to a medium sized font by default if invalid text_format is given.
 * Exposed mostly for writing libraries and custom functions.
 * 
 * \param txt_fmt Text format num that correspondes with a higher level enum
 * \param line The line number on which to print
 * \param fmt  Format string
 * \param args List of arguments for the format string
 */
void display_vprintf(uint8_t txt_fmt, const int16_t line, const char* fmt, va_list args);

/**
 * Print a normal unformatted string to the screen at the specified coordinates
 * 
 * Will default to a medium sized font by default if invalid text_format is given.
 * 
 * Text formats medium_center and large_center will default to medium and large respectively.
 * 
 * \param txt_fmt Text format enum that determines if the text is small, medium, and large.
 * \param x,The y coordinates of the top left corner of the string
 * \param y The y coordinates of the top left corner of the string
 * \param opacity The opacity of the text to display
 * \param format The text to display, with formatting parameters afterwards
 * \param args List of arguments for the format string
 */
void display_opacity_vprintf( int32_t xpos, int32_t ypos, uint32_t opacity, const char *format, va_list args);

typedef enum touch_event_e { 
	E_TOUCH_EVENT_RELEASE, 
	E_TOUCH_EVENT_PRESS, 
	E_TOUCH_EVENT_PRESS_AND_HOLD 
} touch_event_e_t;

typedef void (*touch_event_cb_fn_t)(int16_t, int16_t);

typedef struct touch_event_position_data_s {
	int16_t x;
	int16_t y;
} touch_event_position_data_s_t;

/**
 * Gets the touch event, x position, y position, pressCount, and release count for a touch.
 * 
 * \return A V5_TouchStatus struct containing the above values.
 */
V5_TouchStatus display_touch_status(void);

/**
 * Register a user-defined callback function for an event type
 *
 * When an event of the specified type is fired, the user-provided callback will
 * be invoked.
 *
 * \param cb 			A callback function of type `touch_event_cb_fn_t`
 * 							(`void (*cb)(int16_t, int16_t)`)
 * \param event_type 	The desired type of event to listen for
 */
void register_touch_callback(touch_event_cb_fn_t cb, touch_event_e_t event_type);

/**
 * Touch event handler, loop for handling the screen's
 *  touch detection linked list.
 */
void _handle_touch_events(V5_TouchEvent event, int16_t x, int16_t y);
