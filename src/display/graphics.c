/*
 * \file graphics.c
 *
 * \brief Graphical display on the v5 brain screen.
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

#include "display/graphics.h"

#include <stdlib.h>

#include "pros/apix.h"
#include "v5_api.h"  

static mutex_t _graphics_mutex = NULL;

void display_set_color_fg(uint32_t color) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayForegroundColor(color);
	mutex_give(_graphics_mutex);
}
void display_set_color_bg(uint32_t color) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayBackgroundColor(color);
	mutex_give(_graphics_mutex);
}

void display_erase(void) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayErase();
	mutex_give(_graphics_mutex);
}

void display_scroll(int16_t start_line, int16_t lines) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayScroll(start_line, lines);
	mutex_give(_graphics_mutex);
}
void display_scroll_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t lines) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayScrollRect(x0, y0, x1, y1, lines);
	mutex_give(_graphics_mutex);
}

void display_copy_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t* buf, int32_t stride) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayCopyRect(x0, y0, x1, y1, buf, stride);
	mutex_give(_graphics_mutex);
}

void display_set_pixel_fg(int16_t x, int16_t y) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayPixelSet(x, y);
	mutex_give(_graphics_mutex);
}
void display_set_pixel_bg(int16_t x, int16_t y) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayPixelClear(x, y);
	mutex_give(_graphics_mutex);
}
void display_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayLineDraw(x0, y0, x1, y1);
	mutex_give(_graphics_mutex);
}
void display_clear_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayLineClear(x0, y0, x1, y1);
	mutex_give(_graphics_mutex);
}

void display_draw_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayRectDraw(x0, y0, x1, y1);
	mutex_give(_graphics_mutex);
}
void display_clear_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayRectClear(x0, y0, x1, y1);
	mutex_give(_graphics_mutex);
}
void display_fill_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayRectFill(x0, y0, x1, y1);
	mutex_give(_graphics_mutex);
}

void display_draw_circle(int16_t x, int16_t y, int16_t r) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayCircleDraw(x, y, r);
	mutex_give(_graphics_mutex);
}
void display_clear_circle(int16_t x, int16_t y, int16_t r) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayCircleClear(x, y, r);
	mutex_give(_graphics_mutex);
}
void display_fill_circle(int16_t x, int16_t y, int16_t r) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayCircleFill(x, y, r);
	mutex_give(_graphics_mutex);
}

void display_printf(const int16_t line, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	display_vprintf(line, fmt, args);
	va_end(args);
}
void display_printf_at(int16_t x, int16_t y, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	display_vprintf_at(x, y, fmt, args);
	va_end(args);
}
void display_big_printf(const int16_t line, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	display_big_vprintf(line, fmt, args);
	va_end(args);
}
void display_big_printf_at(int16_t x, int16_t y, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	display_vprintf_at(x, y, fmt, args);
	va_end(args);
}
void display_small_printf_at(int16_t x, int16_t y, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	display_small_vprintf_at(x, y, fmt, args);
	va_end(args);
}
void display_center_printf(const int16_t line, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	display_center_vprintf(line, fmt, args);
	va_end(args);
}
void display_center_big_printf(const int16_t line, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	display_center_big_vprintf(line, fmt, args);
	va_end(args);
}
void display_puts(const int16_t line, const char* text) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayString(line, text);
	mutex_give(_graphics_mutex);
}
void display_puts_at(int16_t x, int16_t y, const char* text) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayStringAt(x, y, text);
	mutex_give(_graphics_mutex);
}
void display_big_puts(const int16_t line, const char* text) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayBigString(line, text);
	mutex_give(_graphics_mutex);
}
void display_big_puts_at(int16_t x, int16_t y, const char* text) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayBigStringAt(x, y, text);
	mutex_give(_graphics_mutex);
}
// void display_small_puts(const int16_t line, const char* fmt, ...) {
//     va_list args;
//     va_start(args, fmt);
//     display_small_vputs(line, fmt, args);
//     va_end();
// }
void display_small_puts_at(int16_t x, int16_t y, const char* text) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplaySmallStringAt(x, y, text);
	mutex_give(_graphics_mutex);
}
void display_center_puts(const int16_t line, const char* text) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayCenteredString(line, text);
	mutex_give(_graphics_mutex);
}
void display_center_big_puts(const int16_t line, const char* text) {
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayBigCenteredString(line, text);
	mutex_give(_graphics_mutex);
}

void display_vprintf(const int16_t line, const char* fmt, va_list args) {
	char* out;
	vasprintf(&out, fmt, args);
	va_list empty;
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayVString(line, out, empty);
	mutex_give(_graphics_mutex);
	free(out);
}
void display_vprintf_at(int16_t x, int16_t y, const char* fmt, va_list args) {
	char* out;
	vasprintf(&out, fmt, args);
	va_list empty;
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayVPrintf(x, y, true, out, empty);
	mutex_give(_graphics_mutex);
	free(out);
}
void display_big_vprintf(const int16_t line, const char* fmt, va_list args) {
	char* out;
	vasprintf(&out, fmt, args);
	va_list empty;
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayVBigString(line, out, empty);
	mutex_give(_graphics_mutex);
	free(out);
}
void display_big_vprintf_at(int16_t x, int16_t y, const char* fmt, va_list args) {
	char* out;
	vasprintf(&out, fmt, args);
	va_list empty;
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayVBigStringAt(x, y, out, empty);
	mutex_give(_graphics_mutex);
	free(out);
}
void display_small_vprintf_at(int16_t x, int16_t y, const char* fmt, va_list args) {
	char* out;
	vasprintf(&out, fmt, args);
	va_list empty;
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayVSmallStringAt(x, y, out, empty);
	mutex_give(_graphics_mutex);
	free(out);
}
void display_center_vprintf(const int16_t line, const char* fmt, va_list args) {
	char* out;
	vasprintf(&out, fmt, args);
	va_list empty;
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayCenteredString(line, out, empty);
	mutex_give(_graphics_mutex);
	free(out);
}
void display_center_big_vprintf(const int16_t line, const char* fmt, va_list args) {
	char* out;
	vasprintf(&out, fmt, args);
	va_list empty;
	mutex_take(_graphics_mutex, TIMEOUT_MAX);
	vexDisplayVBigCenteredString(line, out, empty);
	mutex_give(_graphics_mutex);
	free(out);
}

static linked_list_s_t* _touch_event_release_handler_list = NULL;
static linked_list_s_t* _touch_event_press_handler_list = NULL;
static linked_list_s_t* _touch_event_press_auto_handler_list = NULL;

void _set_up_touch_callback_storage() {
	_touch_event_release_handler_list = linked_list_init();
	_touch_event_press_handler_list = linked_list_init();
	_touch_event_press_auto_handler_list = linked_list_init();
}

void register_touch_callback(touch_event_cb_fn_t cb, touch_event_e_t event_type) {
	switch (event_type) {
	case E_TOUCH_EVENT_RELEASE:
		linked_list_prepend_func(_touch_event_release_handler_list, (generic_fn_t)cb);
		break;
	case E_TOUCH_EVENT_PRESS:
		linked_list_prepend_func(_touch_event_press_handler_list, (generic_fn_t)cb);
		break;
	case E_TOUCH_EVENT_PRESS_AND_HOLD:
		linked_list_prepend_func(_touch_event_press_auto_handler_list, (generic_fn_t)cb);
		break;
	}
}

static task_stack_t touch_handle_task_stack[TASK_STACK_DEPTH_DEFAULT];
static static_task_s_t touch_handle_task_buffer;
static task_t touch_handle_task;

void _handle_cb(ll_node_s_t* current, void* data) {
	((touch_event_cb_fn_t)(current->payload.func))(((touch_event_position_data_s_t*)(data))->x,
	                                               ((touch_event_position_data_s_t*)(data))->y);
}
static inline bool _touch_status_equivalent(V5_TouchStatus x, V5_TouchStatus y) {
	return (x.lastEvent == y.lastEvent) && (x.lastXpos == y.lastXpos) && (x.lastYpos == y.lastYpos);
}
void _touch_handle_task(void* ignore) {
	V5_TouchStatus last, current;
	while (true) {
		vexTouchDataGet(&current);
		if (!_touch_status_equivalent(current, last)) {
			touch_event_position_data_s_t event_data = {.x = current.lastXpos, .y = current.lastYpos };
			switch (current.lastEvent) {
			case E_TOUCH_EVENT_RELEASE:
				linked_list_foreach(_touch_event_release_handler_list, _handle_cb, (void*)&event_data);
				break;
			case E_TOUCH_EVENT_PRESS:
				linked_list_foreach(_touch_event_press_handler_list, _handle_cb, (void*)&event_data);
				break;
			case E_TOUCH_EVENT_PRESS_AND_HOLD:
				linked_list_foreach(_touch_event_press_auto_handler_list, _handle_cb,
				                    (void*)&event_data);
				break;
			}
			last = current;
		}
		delay(5);
	}
}

void graphical_context_daemon_initialize(void) {
	_graphics_mutex = mutex_create();
	_set_up_touch_callback_storage();
	touch_handle_task =
	    task_create_static(_touch_handle_task, NULL, TASK_PRIORITY_MAX - 3, TASK_STACK_DEPTH_DEFAULT,
	                       "PROS GRAPHICS Touch Handler", touch_handle_task_stack, &touch_handle_task_buffer);
}