/**
 * @file lv_conf.h
 *
 */

#ifndef LV_CONF_H
#define LV_CONF_H

/*----------------
 * Dynamic memory
 *----------------*/

/* Memory size which will be used by the library
 * to store the graphical objects and other data */
#define LV_MEM_CUSTOM                                                          \
  1 /*1: use custom malloc/free, 0: use the built-in                           \
       lv_mem_alloc/lv_mem_free*/
#if LV_MEM_CUSTOM == 0
#define LV_MEM_SIZE                                                            \
  (32U * 1024U)     /*Size memory used by `lv_mem_alloc` in bytes (>= 2kB)*/
#define LV_MEM_ATTR /*Complier prefix for big array declaration*/
#define LV_MEM_AUTO_DEFRAG 1 /*Automatically defrag on free*/
#else                        /*LV_MEM_CUSTOM*/
#define LV_MEM_CUSTOM_INCLUDE                                                  \
  "kapi.h"                          /*Header for the dynamic memory function*/
#define LV_MEM_CUSTOM_ALLOC kmalloc /*Wrapper to malloc*/
#define LV_MEM_CUSTOM_FREE kfree    /*Wrapper to free*/
#endif                              /*LV_MEM_CUSTOM*/
#define LV_ENABLE_GC 0

/*===================
   Graphical settings
 *===================*/

/* Horizontal and vertical resolution of the library.*/
#define LV_HOR_RES (480)
#define LV_VER_RES (240)
#define LV_DPI 126

/* Size of VDB (Virtual Display Buffer: the internal graphics buffer).
 * Required for buffered drawing, opacity and anti-aliasing
 * VDB makes the double buffering, you don't need to deal with it!
 * Typical size: ~1/10 screen */
#define LV_VDB_SIZE                                                            \
  (LV_VER_RES *                                                                \
   LV_HOR_RES) /*Size of VDB in pixel count (1/10 screen size is good for      \
                  first)*/
#define LV_VDB_ADR                                                             \
  0 /*Place VDB to a specific address (e.g. in external RAM) (0: allocate      \
       automatically into RAM)*/

/* Use two Virtual Display buffers (VDB) parallelize rendering and flushing
 * (optional)
 * The flushing should use DMA to write the frame buffer in the background*/
#define LV_VDB_DOUBLE 0 /*1: Enable the use of 2 VDBs*/
#define LV_VDB2_ADR                                                            \
  0 /*Place VDB2 to a specific address (e.g. in external RAM) (0: allocate     \
       automatically into RAM)*/

/* Enable anti-aliasing (lines, and radiuses will be smoothed) */
#define LV_ANTIALIAS 1 /*1: Enable anti-aliasing*/

/*Screen refresh settings*/
#define LV_REFR_PERIOD 40   /*Screen refresh period in milliseconds*/
#define LV_INV_FIFO_SIZE 32 /*The average count of objects on a screen */

/*=================
   Misc. setting
 *=================*/

/*Input device settings*/
#define LV_INDEV_READ_PERIOD 50 /*Input device read period in milliseconds*/
#define LV_INDEV_POINT_MARKER                                                  \
  0 /*Mark the pressed points  (required: USE_LV_REAL_DRAW = 1)*/
#define LV_INDEV_DRAG_LIMIT 10 /*Drag threshold in pixels */
#define LV_INDEV_DRAG_THROW                                                    \
  20 /*Drag throw slow-down in [%]. Greater value means faster slow-down */
#define LV_INDEV_LONG_PRESS_TIME 400 /*Long press time in milliseconds*/
#define LV_INDEV_LONG_PRESS_REP_TIME                                           \
  100 /*Repeated trigger period in long press [ms] */

/*Color settings*/
#define LV_COLOR_DEPTH 32 /*Color depth: 1/8/16/24*/
#define LV_COLOR_TRANSP                                                        \
  LV_COLOR_LIME /*Images pixels with this color will not be drawn (with chroma \
                   keying)*/

/*Text settings*/
#define LV_TXT_UTF8 1 /*Enable UTF-8 coded Unicode character usage */
#define LV_TXT_BREAK_CHARS " ,.;:-_" /*Can break texts on these chars*/
#define LV_TXT_LINE_BREAK_LONG_LEN 12
#define LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN 3
#define LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN 1

/*Graphics feature usage*/
#define USE_LV_ANIMATION 1 /*1: Enable all animations*/
#define USE_LV_SHADOW 1    /*1: Enable shadows*/
#define USE_LV_GROUP 1     /*1: Enable object groups (for keyboards)*/
#define USE_LV_GPU 0       /*1: Enable GPU interface*/
#define USE_LV_REAL_DRAW                                                       \
  1 /*1: Enable function which draw directly to the frame buffer instead of    \
       VDB (required if LV_VDB_SIZE = 0)*/
#define USE_LV_FILESYSTEM 1 /*1: Enable file system (required by images*/
#define USE_LV_MULTI_LANG 1

/*Compiler attributes*/
#define LV_ATTRIBUTE_TICK_INC /* Define a custom attribute to tick increment   \
                                 function */
#define LV_ATTRIBUTE_TASK_HANDLER
#define LV_ATTRIBUTE_MEM_ALIGN
#define LV_COMPILER_VLA_SUPPORTED 1
#define LV_COMPILER_NON_CONST_INIT_SUPPORTED 1

#define USE_LV_LOG 0
/*================
 *  THEME USAGE
 *================*/
#define LV_THEME_LIVE_UPDATE 1
#define USE_LV_THEME_TEMPL 0 /*Just for test*/
#define USE_LV_THEME_DEFAULT 0 /*Built mainly from the built-in styles. Consumes very few RAM*/
#define USE_LV_THEME_ALIEN 1 /*Dark futuristic theme*/
#define USE_LV_THEME_NIGHT 1 /*Dark elegant theme*/
#define USE_LV_THEME_MONO 1 /*Mono color theme for monochrome displays*/
#define USE_LV_THEME_MATERIAL 1 /*Flat theme with bold colors and light shadows*/
#define USE_LV_THEME_ZEN 1 /*Peaceful, mainly light theme */

/*==================
 *    FONT USAGE
 *===================*/

/* More info about fonts: https://littlevgl.com/basics#fonts
 * To enable a built-in font use 1,2,4 or 8 values
 * which will determine the bit-per-pixel */
#define LV_FONT_DEFAULT                                                        \
  &lv_font_dejavu_20 /*Always set a default font from the built-in fonts*/

#define USE_LV_FONT_DEJAVU_10 4
#define USE_LV_FONT_DEJAVU_10_LATIN_SUP 4
#define USE_LV_FONT_DEJAVU_10_CYRILLIC 4
#define USE_LV_FONT_SYMBOL_10 4

#define USE_LV_FONT_DEJAVU_20 4
#define USE_LV_FONT_DEJAVU_20_LATIN_SUP 4
#define USE_LV_FONT_DEJAVU_20_CYRILLIC 4
#define USE_LV_FONT_SYMBOL_20 4

#define USE_LV_FONT_DEJAVU_30 0
#define USE_LV_FONT_DEJAVU_30_LATIN_SUP 0
#define USE_LV_FONT_DEJAVU_30_CYRILLIC 0
#define USE_LV_FONT_SYMBOL_30 0

#define USE_LV_FONT_DEJAVU_40 0
#define USE_LV_FONT_DEJAVU_40_LATIN_SUP 0
#define USE_LV_FONT_DEJAVU_40_CYRILLIC 0
#define USE_LV_FONT_SYMBOL_40 0

/* PROS adds the mono variant of DejaVu sans */
#define USE_PROS_FONT_DEJAVU_MONO_10 4
#define USE_PROS_FONT_DEJAVU_MONO_10_LATIN_SUP 4

#define USE_PROS_FONT_DEJAVU_MONO_20 4
#define USE_PROS_FONT_DEJAVU_MONO_LATIN_SUP_20 4

#define USE_PROS_FONT_DEJAVU_MONO_30 0
#define USE_PROS_FONT_DEJAVU_MONO_30_LATIN_SUP 0

#define USE_PROS_FONT_DEJAVU_MONO_40 0
#define USE_PROS_FONT_DEJAVU_MONO_40_LATIN_SUP 0

/*===================
 *  LV_OBJ SETTINGS
 *==================*/
#define LV_OBJ_FREE_NUM_TYPE                                                   \
  uint32_t /*Type of free number attribute (comment out disable free number)*/
#define LV_OBJ_FREE_PTR 1 /*Enable the free pointer attribute*/

/*==================
 *  LV OBJ X USAGE
 *================*/
/*
 * Documentation of the object types: https://littlevgl.com/object-types
 */

/*****************
 * Simple object
 *****************/

/*Label (dependencies: -*/
#define USE_LV_LABEL 1
#if USE_LV_LABEL != 0
#define LV_LABEL_SCROLL_SPEED                                                  \
  25 /*Hor, or ver. scroll speed [px/sec] in 'LV_LABEL_LONG_SCROLL/ROLL'       \
        mode*/
#endif

/*Image (dependencies: lv_label*/
#define USE_LV_IMG 1
#if USE_LV_IMG != 0
#  define LV_IMG_CF_INDEXED 1
#  define LV_IMG_CF_ALPHA 1
#endif

/*Line (dependencies: -*/
#define USE_LV_LINE 1
#define USE_LV_ARC 1

/*******************
 * Container objects
 *******************/

/*Container (dependencies: -*/
#define USE_LV_CONT 1

/*Page (dependencies: lv_cont)*/
#define USE_LV_PAGE 1

/*Window (dependencies: lv_cont, lv_btn, lv_label, lv_img, lv_page)*/
#define USE_LV_WIN 1

/*Tab (dependencies: lv_page, lv_btnm)*/
#define USE_LV_TABVIEW 1
#if USE_LV_TABVIEW != 0
#define LV_TABVIEW_ANIM_TIME                                                   \
  300 /*Time of slide animation [ms] (0: no animation)*/
#endif
#define USE_LV_TILEVIEW 1
#if USE_LV_TILEVIEW
#  define LV_TILEVIEW_ANIM_TIME 300
#endif


/*************************
 * Data visualizer objects
 *************************/

/*Bar (dependencies: -)*/
#define USE_LV_BAR 1

/*Line meter (dependencies: *;)*/
#define USE_LV_LMETER 1

/*Gauge (dependencies:bar, lmeter)*/
#define USE_LV_GAUGE 1

/*Chart (dependencies: -)*/
#define USE_LV_CHART 1

#define USE_LV_TABLE 1
#if USE_LV_TABLE
#  define LV_TABLE_COL_MAX 12
#endif

/*LED (dependencies: -)*/
#define USE_LV_LED 1

/*Message box (dependencies: lv_rect, lv_btnm, lv_label)*/
#define USE_LV_MBOX 1

/*Text area (dependencies: lv_label, lv_page)*/
#define USE_LV_TA 1
#if USE_LV_TA != 0
#define LV_TA_CURSOR_BLINK_TIME 400 /*ms*/
#define LV_TA_PWD_SHOW_TIME 1500    /*ms*/
#endif

#define USE_LV_SPINBOX 1
#define USE_LV_CALENDAR 1

#define USE_PRELOAD 1
#if USE_LV_PRELOAD != 0
#  define LV_PRELOAD_DEF_ARC_LENGTH 60
#  define LV_PRELOAD_DEF_SPIN_TIME 1000
#  define LV_PRELOAD_DEF_ANIM LV_PRELOAD_TYPE_SPINNING_ARC
#endif

#define USE_LV_CANVAS 1
/*************************
 * User input objects
 *************************/

/*Button (dependencies: lv_cont*/
#define USE_LV_BTN 1
#if USE_LV_BTN != 0
#  define LV_BTN_INK_EFFECT 1
#endif

#define USE_LV_IMGBTN 1
#if USE_LV_IMGBTN
#  define LV_IMGBTN_TILED 0
#endif

/*Button matrix (dependencies: -)*/
#define USE_LV_BTNM 1

/*Keyboard (dependencies: lv_btnm)*/
#define USE_LV_KB 0

/*Check box (dependencies: lv_btn, lv_label)*/
#define USE_LV_CB 1

/*List (dependencies: lv_page, lv_btn, lv_label, (lv_img optionally for icons
 * ))*/
#define USE_LV_LIST 1
#if USE_LV_LIST != 0
#define LV_LIST_FOCUS_TIME                                                     \
  100 /*Default animation time of focusing to a list element [ms] (0: no       \
         animation)  */
#endif

/*Drop down list (dependencies: lv_page, lv_label)*/
#define USE_LV_DDLIST 1
#if USE_LV_DDLIST != 0
#define LV_DDLIST_ANIM_TIME                                                    \
  200 /*Open and close default animation time [ms] (0: no animation)*/
#endif

/*Roller (dependencies: lv_ddlist)*/
#define USE_LV_ROLLER 1
#if USE_LV_ROLLER != 0
#define LV_ROLLER_ANIM_TIME                                                    \
  200 /*Focus animation time [ms] (0: no                                       \
         animation)*/
#endif

/*Slider (dependencies: lv_bar)*/
#define USE_LV_SLIDER 1

/*Switch (dependencies: lv_slider)*/
#define USE_LV_SW 1

#if LV_INDEV_DRAG_THROW <= 0
#warning "LV_INDEV_DRAG_THROW must be greater than 0"
#undef LV_INDEV_DRAG_THROW
#define LV_INDEV_DRAG_THROW 1
#endif

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#  define _CRT_SECURE_NO_WARNINGS
#endif
#include "display/lv_conf_checker.h"
#endif /*LV_CONF_H*/
