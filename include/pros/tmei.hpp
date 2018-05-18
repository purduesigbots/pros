/*
 * \file pros/tmei.hpp
 *
 * \brief C++ bindings for Touch Management Engine/Interface layer 0
 *
 * This file represents a low-level interface for interacting with the built-in
 * LCD touch screen.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _PROS_TMEI_HPP_
#define _PROS_TMEI_HPP_

#include <cstdint>
#include <string>

#include "pros/tmei.h"

namespace pros {
namespace display {
/**
 * Sets the foreground color for subsequent graphics operations.
 *
 * \param color
 *        The foreground color to set (it is recommended to use values
 *        from the enum defined in colors.h)
 */
void set_color_fg(std::uint32_t color);

/**
 * Sets the background color for subsequent graphics operations.
 *
 * \param color
 *        The background color to set (it is recommended to use values
 *        from the enum defined in colors.h)
 */
void set_color_bg(std::uint32_t color);

/**
 * Resets the display to the default black screen.
 */
void erase(void);

/**
 * Scrolls lines on the display.
 *
 * \param start_line
 *        The line from which scrolling will start
 * \param lines
 *        The number of lines to scroll
 */
void scroll(std::int16_t start_line, std::int16_t lines);

/**
 * Scrolls lines within a region on the display.
 *
 * This function behaves in the same way as display_scroll, except that you
 * specify a rectangular region within which to scroll lines instead of a start
 * line.
 *
 * \param x0, y0
 *        The (x,y) coordinates of the first corner of the rectangular region
 * \param x1, y1
 *        The (x,y) coordinates of the second corner of the rectangular region
 * \param lines
 *        The number of lines to scroll
 */
void scroll_rect(std::int16_t x0, std::int16_t y0, std::int16_t x1,
                 std::int16_t y1, std::int16_t lines);

/**
 * Copies a screen region from an off-screen buffer to the screen.
 *
 * \param x0, y0
 *        The (x,y) coordinates of the first corner of the rectangular region of
 *        the screen
 * \param x1, y1
 *        The (x,y) coordinates of the second corner of the rectangular region
 *        of the screen
 * \param buf
 *        Off-screen buffer containing screen data
 * \param stride
 *        Off-screen buffer width in pixels, such that image size is
 * stride-padding
 */
void copy_rect(std::int16_t x0, std::int16_t y0, std::int16_t x1,
               std::int16_t y1, std::uint32_t *buf, std::int32_t stride);
/**
 * Draws a pixel on the screen using the current foreground color.
 *
 * \param x, y
 *        The (x,y) coordinates of the pixel
 */
void draw_pixel(std::int16_t x, std::int16_t y);

/**
 * Draws a pixel on the screen using the current background color.
 *
 * \param x, y
 *        The (x,y) coordinates of the pixel
 */
void clear_pixel(std::int16_t x, std::int16_t y);

/**
 * Draws a line on the screen using the current foreground color.
 *
 * \param x0, y0
 *        The (x, y) coordinates of the first point of the line
 * \param x1, y1
 *        The (x, y) coordinates of the second point of the line
 */
void draw_line(std::int16_t x0, std::int16_t y0, std::int16_t x1,
               std::int16_t y1);

/**
 * Draws a line on the screen using the current background color.
 *
 * \param x0, y0
 *        The (x, y) coordinates of the first point of the line
 * \param x1, y1
 *        The (x, y) coordinates of the second point of the line
 */
void clear_line(std::int16_t x0, std::int16_t y0, std::int16_t x1,
                std::int16_t y1);

/**
 * Draws a rectangle on the screen using the current foreground color.
 *
 * \param x0, y0
 *        The (x,y) coordinates of the first point of the rectangle
 * \param x1, y1
 *        The (x,y) coordinates of the second point of the rectangle
 */
void draw_rect(std::int16_t x0, std::int16_t y0, std::int16_t x1,
               std::int16_t y1);

/**
 * Draws a rectangle on the screen using the current background color.
 *
 * \param x0, y0
 *        The (x,y) coordinates of the first point of the rectangle
 * \param x1, y1
 *        The (x,y) coordinates of the second point of the rectangle
 */
void clear_rect(std::int16_t x0, std::int16_t y0, std::int16_t x1,
                std::int16_t y1);

/**
 * Fills a rectangular region of the screen using the current foreground color.
 *
 * \param x0, y0
 *        The (x,y) coordinates of the first point of the rectangle
 * \param x1, y1
 *        The (x,y) coordinates of the second point of the rectangle
 */
void fill_rect(std::int16_t x0, std::int16_t y0, std::int16_t x1,
               std::int16_t y1);

/**
 * Draws a circle on the screen using the current foreground color.
 *
 * \param x, y
 *        The (x,y) coordinates of the center of the circle
 * \param r
 *        The radius of the circle
 */
void draw_circle(std::int16_t x, std::int16_t y, std::int16_t r);

/**
 * Draws a circle on the screen using the current background color.
 *
 * \param x, y
 *        The (x,y) coordinates of the center of the circle
 * \param r
 *        The radius of the circle
 */
void clear_circle(std::int16_t x, std::int16_t y, std::int16_t r);

/**
 * Fills a circular region of the screen using the current foreground color.
 *
 * \param x, y
 *        The (x,y) coordinates of the center of the circle
 * \param r
 *        The radius of the circle
 */
void fill_circle(std::int16_t x, std::int16_t y, std::int16_t r);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
namespace {
template <typename T> T convert_args(T arg) { return arg; }
const char *convert_args(const std::string &arg) { return arg.c_str(); }
} // namespace
#pragma GCC diagnostic pop

/**
 * Prints a formatted string to the screen on the specified line.
 *
 * \param line
 *        The line number on which to print
 * \param fmt
 *        Format string
 * \param ...
 *        Optional list of arguments for the format string
 */
template <typename... Params>
void printf(const std::int16_t line, const char *fmt, Params... args) {
  display_printf(line, fmt, convert_args(args)...);
}

/**
 * Prints a formatted string to the screen at the specified coordinates.
 *
 * \param x, y
 *        The (x,y) coordinates of the top left corner of the string
 * \param fmt
 *        Format string
 * \param ...
 *        Optional list of arguments for the format string
 */
template <typename... Params>
void printf_at(std::int16_t x, std::int16_t y, const char *fmt,
               Params... args) {
  display_printf_at(x, y, fmt, convert_args(args)...);
}

/**
 * Prints a large, formatted string to the screen on the specified line.
 *
 * \param line
 *        The line number on which to print
 * \param fmt
 *        Format string
 * \param ...
 *        Optional list of arguments for the format string
 */
template <typename... Params>
void big_printf(const std::int16_t line, const char *fmt, Params... args) {
  display_big_printf(line, fmt, convert_args(args)...);
}

/**
 * Prints a large, formatted string to the screen at the specified coordinates.
 *
 * \param x, y
 *        The (x, y) coordinates of the top left corner of the string
 * \param fmt
 *        Format string
 * \param ...
 *        Optional list of arguments for the format string
 */
template <typename... Params>
void big_printf_at(std::int16_t x, std::int16_t y, const char *fmt,
                   Params... args) {
  display_printf_at(x, y, fmt, convert_args(args)...);
}

/**
 * Prints a small, formatted string to the screen at the specified coordinates.
 *
 * \param x, y
 *        The (x, y) coordinates of the top left corner of the string
 * \param fmt
 *        Format string
 * \param ...
 *        Optional list of arguments for the format string
 */
template <typename... Params>
void small_printf_at(std::int16_t x, std::int16_t y, const char *fmt,
                     Params... args) {
  display_small_printf_at(x, y, fmt, convert_args(args)...);
}

/**
 * Prints a centered, formatted string to the screen on the specified line.
 *
 * \param line
 *        The line number on which to print
 * \param fmt
 *        Format string
 * \param ...
 *        Optional list of arguments for the format string
 */
template <typename... Params>
void center_printf(const std::int16_t line, const char *fmt, Params... args) {
  display_center_printf(line, fmt, convert_args(args)...);
}

/**
 * Prints a large, centered, formatted string to the screen on the specified
 * line.
 *
 * \param line
 *        The line number on which to print
 * \param fmt
 *        Format string
 * \param ...
 *        Optional list of arguments for the format string
 */
template <typename... Params>
void center_big_printf(const std::int16_t line, const char *fmt,
                       Params... args) {
  display_center_big_printf(line, fmt, convert_args(args)...);
}

/**
 * Prints a string to the screen on the specified line.
 *
 * \param line
 *        The line number on which to print
 * \param text
 *        The text to display
 */
void puts(const std::int16_t line, const std::string text);

/**
 * Prints a string to the screen at the specified coordinates.
 *
 * \param x, y
 *        The (x,y) coordinates of the top left corner of the string
 * \param text
 *        The text to display
 */
void puts_at(std::int16_t x, std::int16_t y, const std::string text);

/**
 * Prints a large string to the screen on the specified line.
 *
 * \param line
 *        The line number on which to print
 * \param text
 *        The text to display
 */
void big_puts(const std::int16_t line, const std::string text);

/**
 * Prints a large string to the screen at the specified coordinates.
 *
 * \param x, y
 *        The (x,y) coordinates of the top left corner of the string
 * \param text
 *        The text to display
 */
void big_puts_at(std::int16_t x, std::int16_t y, const std::string text);

/**
 * Prints a small string to the screen at the specified coordinates.
 *
 * \param x, y
 *        The (x,y) coordinates of the top left corner of the string
 * \param text
 *        The text to display
 */
void small_puts_at(std::int16_t x, std::int16_t y, const std::string text);

/**
 * Prints a centered string on the specified line.
 *
 * \param line
 *        The line number on which to print
 * \param text
 *        The text to display
 */
void center_puts(const std::int16_t line, const std::string text);

/**
 * Prints a large, centered string on the specified line.
 *
 * \param line
 *        The line number on which to print
 * \param text
 *        The text to display
 */
void center_big_puts(const std::int16_t line, const std::string text);
} // namespace display
} // namespace pros

namespace pros {
namespace touch {
using event_e_t = touch_event_e_t;
using event_cb_fn_t = void (*)(std::int32_t, std::int32_t);

/**
 * Registers a user-defined callback function for an event type.
 *
 * When an event of the specified type is fired, the user-provided callback will
 * be invoked.
 *
 * \param cb
 *        A callback function of type touch_event_cb_fn_t (void (*cb)(int16_t,
 * int16_t))
 * \param event_type
 *        The desired type of event to listen for
 */
void register_callback(event_cb_fn_t cb, event_e_t event_type);

/**
 * Unregisters a user-defined callback for an event type.
 *
 * Subsequent events of the specified type will no longer invoke the
 * user-provided
 * callback when fired.
 *
 * \param cb
 * 		  A callback function of type touch_event_cb_fn_t (void
 * (*cb)(int16_t, int16_t))
 * \param event_type
 * 		  The event type associated with the callback
 */
void unregister_callback(event_cb_fn_t cb, event_e_t event_type);
} // namespace touch
} // namespace pros

#endif // _PROS_TMEI_HPP_
