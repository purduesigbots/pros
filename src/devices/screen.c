/**
 * \file screen.c
 *
 * \brief Brain screen display and touch functions.
 *
 * Contains user calls to the v5 screen for touching and displaying graphics.
 *
 * \copyright (c) 2017-2021, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "pros/screen.h"

#include <stdint.h>
#include <stdlib.h>
#include <sys/_stdint.h>

#include "common/linkedlist.h"
#include "kapi.h"
#include "pros/apix.h"
#include "v5_api.h"  // vexDisplay* 

/******************************************************************************/
/**                  Screen Graphical Display Functions                      **/
/**                                                                          **/
/**   These functions allow programmers to display shapes on the v5 screen   **/
/******************************************************************************/

static mutex_t _screen_mutex = NULL;

typedef struct touch_event_position_data_s {
	int16_t x;
	int16_t y;
} touch_event_position_data_s_t;

uint32_t screen_set_pen(uint32_t color){
	if (!mutex_take(_screen_mutex, TIMEOUT_MAX)) {
		errno = EACCES;
		return PROS_ERR;
	}
	vexDisplayForegroundColor(color);
	if (!mutex_give(_screen_mutex)) {
		return PROS_ERR;
	} else {
		return 1;
	}
}

uint32_t screen_set_eraser(uint32_t color){
	if (!mutex_take(_screen_mutex, TIMEOUT_MAX)) {
		errno = EACCES;
		return PROS_ERR;
	}
	vexDisplayBackgroundColor(color);
	if (!mutex_give(_screen_mutex)) {
		return PROS_ERR;
	} else {
		return 1;
	}
}

uint32_t screen_get_pen(void){
	if (!mutex_take(_screen_mutex, TIMEOUT_MAX)) {
		errno = EACCES;
		return PROS_ERR;
	}
	uint32_t color = vexDisplayForegroundColorGet();
	if (!mutex_give(_screen_mutex)) {
		return PROS_ERR;
	}
	return color;
}

uint32_t screen_get_eraser(void){
	if (!mutex_take(_screen_mutex, TIMEOUT_MAX)) {
		errno = EACCES;
		return PROS_ERR;
	}
	uint32_t color = vexDisplayBackgroundColorGet();
	if (!mutex_give(_screen_mutex)) {
		return PROS_ERR;
	}
	return color;
}

uint32_t screen_erase(void){
	if (!mutex_take(_screen_mutex, TIMEOUT_MAX)) {
		errno = EACCES;
		return PROS_ERR;
	}
	vexDisplayErase();
	if (!mutex_give(_screen_mutex)) {
		return PROS_ERR;
	} else {
		return 1;
	}
}

uint32_t screen_scroll(int16_t start_line, int16_t lines){
	if (!mutex_take(_screen_mutex, TIMEOUT_MAX)) {
		errno = EACCES;
		return PROS_ERR;
	}
	vexDisplayScroll(start_line, lines);
	if (!mutex_give(_screen_mutex)) {
		return PROS_ERR;
	} else {
		return 1;
	}
}

uint32_t screen_scroll_area(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t lines){
	if (!mutex_take(_screen_mutex, TIMEOUT_MAX)) {
		errno = EACCES;
		return PROS_ERR;
	}
	vexDisplayScrollRect(x0, y0, x1, y1, lines);
	if (!mutex_give(_screen_mutex)) {
		return PROS_ERR;
	} else {
		return 1;
	}
}

uint32_t screen_copy_area(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t* buf, int32_t stride){
	if (!mutex_take(_screen_mutex, TIMEOUT_MAX)) {
		errno = EACCES;
		return PROS_ERR;
	}
	vexDisplayCopyRect(x0, y0, x1, y1, buf, stride);
	if (!mutex_give(_screen_mutex)) {
		return PROS_ERR;
	} else {
		return 1;
	}
}

uint32_t screen_draw_pixel(int16_t x, int16_t y){
	if (!mutex_take(_screen_mutex, TIMEOUT_MAX)) {
		errno = EACCES;
		return PROS_ERR;
	}
	vexDisplayPixelSet(x, y);
	if (!mutex_give(_screen_mutex)) {
		return PROS_ERR;
	} else {
		return 1;
	}
}

uint32_t screen_erase_pixel(int16_t x, int16_t y){
	if (!mutex_take(_screen_mutex, TIMEOUT_MAX)) {
		errno = EACCES;
		return PROS_ERR;
	}
	vexDisplayPixelClear(x, y);
	if (!mutex_give(_screen_mutex)) {
		return PROS_ERR;
	} else {
		return 1;
	}
}

uint32_t screen_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1){
	if (!mutex_take(_screen_mutex, TIMEOUT_MAX)) {
		errno = EACCES;
		return PROS_ERR;
	}
	vexDisplayLineDraw(x0, y0, x1, y1);
	if (!mutex_give(_screen_mutex)) {
		return PROS_ERR;
	} else {
		return 1;
	}
}

uint32_t screen_erase_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1){
	if (!mutex_take(_screen_mutex, TIMEOUT_MAX)) {
		errno = EACCES;
		return PROS_ERR;
	}
	vexDisplayLineClear(x0, y0, x1, y1);
	if (!mutex_give(_screen_mutex)) {
		return PROS_ERR;
	} else {
		return 1;
	}
}

uint32_t screen_draw_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1){
	if (!mutex_take(_screen_mutex, TIMEOUT_MAX)) {
		errno = EACCES;
		return PROS_ERR;
	}
	vexDisplayRectDraw(x0, y0, x1, y1);
	if (!mutex_give(_screen_mutex)) {
		return PROS_ERR;
	} else {
		return 1;
	}
}

uint32_t screen_erase_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1){
	if (!mutex_take(_screen_mutex, TIMEOUT_MAX)) {
		errno = EACCES;
		return PROS_ERR;
	}
	vexDisplayRectClear(x0, y0, x1, y1);
	if (!mutex_give(_screen_mutex)) {
		return PROS_ERR;
	} else {
		return 1;
	}
}

uint32_t screen_fill_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1){
	if (!mutex_take(_screen_mutex, TIMEOUT_MAX)) {
		errno = EACCES;
		return PROS_ERR;
	}
	vexDisplayRectFill(x0, y0, x1, y1);
	if (!mutex_give(_screen_mutex)) {
		return PROS_ERR;
	} else {
		return 1;
	}
}

uint32_t screen_draw_circle(int16_t x, int16_t y, int16_t radius){
	if (!mutex_take(_screen_mutex, TIMEOUT_MAX)) {
		errno = EACCES;
		return PROS_ERR;
	}
	vexDisplayCircleDraw(x, y, radius);
	if (!mutex_give(_screen_mutex)) {
		return PROS_ERR;
	} else {
		return 1;
	}
}

uint32_t screen_erase_circle(int16_t x, int16_t y, int16_t radius){
	if (!mutex_take(_screen_mutex, TIMEOUT_MAX)) {
		errno = EACCES;
		return PROS_ERR;
	}
	vexDisplayCircleClear(x, y, radius);
	if (!mutex_give(_screen_mutex)) {
		return PROS_ERR;
	} else {
		return 1;
	}
}

uint32_t screen_fill_circle(int16_t x, int16_t y, int16_t radius){
	if (!mutex_take(_screen_mutex, TIMEOUT_MAX)) {
		errno = EACCES;
		return PROS_ERR;
	}
	vexDisplayCircleFill(x, y, radius);
	if (!mutex_give(_screen_mutex)) {
		return PROS_ERR;
	} else {
		return 1;
	}
}

/******************************************************************************/
/**                       Screen Text Display Functions                      **/
/**                                                                          **/
/**     These functions allow programmers to display text on the v5 screen   **/
/******************************************************************************/

uint32_t screen_print(text_format_e_t txt_fmt, const int16_t line, const char* text, ...){
    va_list args;
    va_start(args, text);
    if (screen_vprintf((uint8_t)txt_fmt, line, text, args) == PROS_ERR) {
		return PROS_ERR;
	}
    va_end(args);
	return 1;
}

uint32_t screen_print_at(text_format_e_t txt_fmt, int16_t x, int16_t y, const char* text, ...){
    va_list args;
    va_start(args, text);
    if (screen_vprintf_at((uint8_t)txt_fmt, x, y, text, args) == PROS_ERR) {
		return PROS_ERR;
	}
    va_end(args);
	return 1;
}

uint32_t screen_vprintf(text_format_e_t txt_fmt, const int16_t line, const char* text, va_list args){
	if (!mutex_take(_screen_mutex, TIMEOUT_MAX)) {
		errno = EACCES;
		return PROS_ERR;
	}
	char* out;
	vasprintf(&out, text, args);
	va_list empty;
	switch(txt_fmt){
	case E_TEXT_SMALL:
	case E_TEXT_MEDIUM:
		{
            vexDisplayVString(line, out, empty);
            break;
        }
    case E_TEXT_LARGE:
		{
            vexDisplayVBigString(line, out, empty);
            break;
        }
    case E_TEXT_MEDIUM_CENTER:
		{
            vexDisplayVCenteredString(line, out, empty);
            break;
        }
    case E_TEXT_LARGE_CENTER:
        {
            vexDisplayVBigCenteredString(line, out, empty);
            break;
        }
    default:
        {
            vexDisplayVString(line, out, empty);
            break;
        }
    }
	if (!mutex_give(_screen_mutex)) {
		return PROS_ERR;
	} else {
		return 1;
	}
}

uint32_t screen_vprintf_at(text_format_e_t txt_fmt, const int16_t x, const int16_t y, const char* text, va_list args){
	if (!mutex_take(_screen_mutex, TIMEOUT_MAX)) {
		errno = EACCES;
		return PROS_ERR;
	}
	char* out;
	vasprintf(&out, text, args);
	va_list empty;
	switch(txt_fmt){
    case E_TEXT_SMALL:
		{
            vexDisplayVSmallStringAt(x, y, out, empty);
            break;
        }
    case E_TEXT_MEDIUM:
    case E_TEXT_MEDIUM_CENTER:
		{
            vexDisplayVStringAt(x, y, out, empty);
            break;
        }
    case E_TEXT_LARGE:
    case E_TEXT_LARGE_CENTER:
		{
            vexDisplayVBigStringAt(x, y, out, empty);
            break;
        }
    default:
        {
            vexDisplayVStringAt(x, y, out, empty);
            break;
        }
    }
	if (!mutex_give(_screen_mutex)) {
		return PROS_ERR;
	} else {
		return 1;
	}
}
/******************************************************************************/
/**                         Screen Touch Functions                           **/
/**                                                                          **/
/**               These functions allow programmers to access                **/
/**                    information about screen touches                      **/
/******************************************************************************/

static const screen_touch_status_s_t PROS_SCREEN_ERR = {.touch_status = E_TOUCH_ERROR, .x = -1, .y = -1, .press_count = -1, .release_count = -1};

screen_touch_status_s_t screen_touch_status(void){
	if (!mutex_take(_screen_mutex, TIMEOUT_MAX)) {
		errno = EACCES;
		return PROS_SCREEN_ERR;
	}
	V5_TouchStatus v5_touch_status;
	screen_touch_status_s_t rtv;
	vexTouchDataGet(&v5_touch_status);
	rtv.touch_status = (last_touch_e_t)v5_touch_status.lastEvent;
	rtv.x = v5_touch_status.lastXpos;
	rtv.y = v5_touch_status.lastYpos;
	rtv.press_count = v5_touch_status.pressCount;
	rtv.release_count = v5_touch_status.releaseCount;
	if (!mutex_give(_screen_mutex)) {
		errno = EACCES;
		return PROS_SCREEN_ERR;
	} 
	return rtv;
}

static linked_list_s_t* _touch_event_release_handler_list = NULL;
static linked_list_s_t* _touch_event_press_handler_list = NULL;
static linked_list_s_t* _touch_event_press_auto_handler_list = NULL;

static void _set_up_touch_callback_storage() {
	_touch_event_release_handler_list = linked_list_init();
	_touch_event_press_handler_list = linked_list_init();
	_touch_event_press_auto_handler_list = linked_list_init();
}

uint32_t screen_touch_callback(touch_event_cb_fn_t cb, last_touch_e_t event_type) {
	if (!mutex_take(_screen_mutex, TIMEOUT_MAX)) {
		errno = EACCES;
		return PROS_ERR;
	}
	switch (event_type) {
	case E_TOUCH_RELEASED:
		linked_list_prepend_func(_touch_event_release_handler_list, (generic_fn_t)cb);
		break;
	case E_TOUCH_PRESSED:
		linked_list_prepend_func(_touch_event_press_handler_list, (generic_fn_t)cb);
		break;
	case E_TOUCH_HELD:
		linked_list_prepend_func(_touch_event_press_auto_handler_list, (generic_fn_t)cb);
		break;
	case E_TOUCH_ERROR:
		return PROS_ERR;
		break;
	}
	if (!mutex_give(_screen_mutex)) {
		return PROS_ERR;
	} else {
		return 1;
	}
}

static task_stack_t touch_handle_task_stack[TASK_STACK_DEPTH_DEFAULT];
static static_task_s_t touch_handle_task_buffer;
static task_t touch_handle_task;

// volatile because some linters think this is going to be optimized out
static volatile void _handle_cb(ll_node_s_t* current, void* extra_data) {
	(current->payload.func)();
}

static inline bool _touch_status_equivalent(V5_TouchStatus x, V5_TouchStatus y) {
	return (x.lastEvent == y.lastEvent) && (x.lastXpos == y.lastXpos) && (x.lastYpos == y.lastYpos);
}

void _touch_handle_task(void* ignore) {
	V5_TouchStatus last, current;
	while (true) {
		mutex_take(_screen_mutex, TIMEOUT_MAX);
        vexTouchDataGet(&current);
		mutex_give(_screen_mutex);
		if (!_touch_status_equivalent(current, last)) {
			switch (current.lastEvent) {
			case E_TOUCH_RELEASED:
				linked_list_foreach(_touch_event_release_handler_list, _handle_cb, NULL);
				break;
			case E_TOUCH_PRESSED:
				linked_list_foreach(_touch_event_press_handler_list, _handle_cb, NULL);
				break;
			case E_TOUCH_HELD:
				linked_list_foreach(_touch_event_press_auto_handler_list, _handle_cb, NULL);
				break;
			}
			last = current;
		}
		delay(10);
	}
}

void graphical_context_daemon_initialize(void) {
	_screen_mutex = mutex_create();
	_set_up_touch_callback_storage();
	touch_handle_task =
	    task_create_static(_touch_handle_task, NULL, TASK_PRIORITY_MIN + 2, TASK_STACK_DEPTH_DEFAULT,
	                       "PROS Graphics Touch Handler", touch_handle_task_stack, &touch_handle_task_buffer);
}
