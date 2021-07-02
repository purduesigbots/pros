cmake_minimum_required(VERSION 3.16.3)

# Add custom command to print firmware size in Berkley format
function(firmware_size target)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_SIZETOOL} -B
        "${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}"
    )
endfunction()

# Add a command to generate firmare in a provided format
function(generate_object target suffix type)
    if(ARGC GREATER 4)
        string(REPLACE ";" "-R " _ARGS "${ARGN}")
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -O ${type} -R ${_ARGS}
            "${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_BINARY_DIR}/${target}${suffix}"
            BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/${target}${suffix}"
        )
    else()
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -O ${type}
            "${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_BINARY_DIR}/${target}${suffix}"
            BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/${target}${suffix}"
        )
    endif()
endfunction()

# Update a target LINK_DEPENDS property with a custom linker script.
# That allows to rebuild that target if the linker script gets changed
function(linker_script_target_dependency target path_to_script)
    get_target_property(_cur_link_deps ${target} LINK_DEPENDS)
    string(APPEND _cur_link_deps " ${path_to_script}")
    set_target_properties(${target} PROPERTIES LINK_DEPENDS ${_cur_link_deps})
endfunction()

# generate an object containing compile time information (used e.g. in the pros terminal banner)
function(generate_pros_ld_timestamp target)
    add_custom_command(
        TARGET ${target} PRE_LINK
        COMMAND rm -f "${CMAKE_BINARY_DIR}/${target}/_pros_ld_timestamp.obj"
        COMMAND mkdir -p "${CMAKE_BINARY_DIR}/${target}"
        COMMAND
        "${CMAKE_SOURCE_DIR}/pros-ld-timestamp.sh" "${CMAKE_C_COMPILER}" "${CMAKE_C_FLAGS}" "${CMAKE_BINARY_DIR}/${target}/_pros_ld_timestamp.obj"
        BYPRODUCTS "${CMAKE_BINARY_DIR}/${target}/_pros_ld_timestamp.obj"
    )
endfunction()

project(pros C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)

set(INCDIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(FWDIR ${CMAKE_CURRENT_SOURCE_DIR}/firmware)

# build LVGL separately until we can remove it from the kernel entirely

set(DISPLAY_SRCS
    "src/display/display.c"
    "src/display/error.c"
    "src/display/llemu.c"
    "src/display/llemu.cpp"
    "src/display/lv_core/lv_group.c"
    "src/display/lv_core/lv_indev.c"
    "src/display/lv_core/lv_lang.c"
    "src/display/lv_core/lv_obj.c"
    "src/display/lv_core/lv_refr.c"
    "src/display/lv_core/lv_style.c"
    "src/display/lv_core/lv_vdb.c"
    "src/display/lv_draw/lv_draw.c"
    "src/display/lv_draw/lv_draw_arc.c"
    "src/display/lv_draw/lv_draw_img.c"
    "src/display/lv_draw/lv_draw_label.c"
    "src/display/lv_draw/lv_draw_line.c"
    "src/display/lv_draw/lv_draw_rbasic.c"
    "src/display/lv_draw/lv_draw_rect.c"
    "src/display/lv_draw/lv_draw_triangle.c"
    "src/display/lv_draw/lv_draw_vbasic.c"
    "src/display/lv_fonts/lv_font_builtin.c"
    "src/display/lv_fonts/lv_font_dejavu_10.c"
    "src/display/lv_fonts/lv_font_dejavu_10_cyrillic.c"
    "src/display/lv_fonts/lv_font_dejavu_10_latin_sup.c"
    "src/display/lv_fonts/lv_font_dejavu_20.c"
    "src/display/lv_fonts/lv_font_dejavu_20_cyrillic.c"
    "src/display/lv_fonts/lv_font_dejavu_20_latin_sup.c"
    "src/display/lv_fonts/lv_font_dejavu_30.c"
    "src/display/lv_fonts/lv_font_dejavu_30_cyrillic.c"
    "src/display/lv_fonts/lv_font_dejavu_30_latin_sup.c"
    "src/display/lv_fonts/lv_font_dejavu_40.c"
    "src/display/lv_fonts/lv_font_dejavu_40_cyrillic.c"
    "src/display/lv_fonts/lv_font_dejavu_40_latin_sup.c"
    "src/display/lv_fonts/lv_font_monospace_8.c"
    "src/display/lv_fonts/lv_font_symbol_10.c"
    "src/display/lv_fonts/lv_font_symbol_20.c"
    "src/display/lv_fonts/lv_font_symbol_30.c"
    "src/display/lv_fonts/lv_font_symbol_40.c"
    "src/display/lv_fonts/pros_font_dejavu_mono_10.c"
    "src/display/lv_fonts/pros_font_dejavu_mono_20.c"
    "src/display/lv_fonts/pros_font_dejavu_mono_30.c"
    "src/display/lv_fonts/pros_font_dejavu_mono_40.c"
    "src/display/lv_fonts/pros_font_dejavu_mono_latin_sup_10.c"
    "src/display/lv_fonts/pros_font_dejavu_mono_latin_sup_20.c"
    "src/display/lv_fonts/pros_font_dejavu_mono_latin_sup_30.c"
    "src/display/lv_fonts/pros_font_dejavu_mono_latin_sup_40.c"
    "src/display/lv_hal/lv_hal_disp.c"
    "src/display/lv_hal/lv_hal_indev.c"
    "src/display/lv_hal/lv_hal_tick.c"
    "src/display/lv_misc/lv_anim.c"
    "src/display/lv_misc/lv_area.c"
    "src/display/lv_misc/lv_circ.c"
    "src/display/lv_misc/lv_color.c"
    "src/display/lv_misc/lv_font.c"
    "src/display/lv_misc/lv_fs.c"
    "src/display/lv_misc/lv_gc.c"
    "src/display/lv_misc/lv_ll.c"
    "src/display/lv_misc/lv_log.c"
    "src/display/lv_misc/lv_math.c"
    "src/display/lv_misc/lv_mem.c"
    "src/display/lv_misc/lv_task.c"
    "src/display/lv_misc/lv_templ.c"
    "src/display/lv_misc/lv_txt.c"
    "src/display/lv_misc/lv_ufs.c"
    "src/display/lv_objx/lv_arc.c"
    "src/display/lv_objx/lv_bar.c"
    "src/display/lv_objx/lv_btn.c"
    "src/display/lv_objx/lv_btnm.c"
    "src/display/lv_objx/lv_calendar.c"
    "src/display/lv_objx/lv_canvas.c"
    "src/display/lv_objx/lv_cb.c"
    "src/display/lv_objx/lv_chart.c"
    "src/display/lv_objx/lv_cont.c"
    "src/display/lv_objx/lv_ddlist.c"
    "src/display/lv_objx/lv_gauge.c"
    "src/display/lv_objx/lv_img.c"
    "src/display/lv_objx/lv_imgbtn.c"
    "src/display/lv_objx/lv_kb.c"
    "src/display/lv_objx/lv_label.c"
    "src/display/lv_objx/lv_led.c"
    "src/display/lv_objx/lv_line.c"
    "src/display/lv_objx/lv_list.c"
    "src/display/lv_objx/lv_lmeter.c"
    "src/display/lv_objx/lv_mbox.c"
    "src/display/lv_objx/lv_objx_templ.c"
    "src/display/lv_objx/lv_page.c"
    "src/display/lv_objx/lv_preload.c"
    "src/display/lv_objx/lv_roller.c"
    "src/display/lv_objx/lv_slider.c"
    "src/display/lv_objx/lv_spinbox.c"
    "src/display/lv_objx/lv_sw.c"
    "src/display/lv_objx/lv_ta.c"
    "src/display/lv_objx/lv_table.c"
    "src/display/lv_objx/lv_tabview.c"
    "src/display/lv_objx/lv_tileview.c"
    "src/display/lv_objx/lv_win.c"
    "src/display/lv_themes/lv_theme.c"
    "src/display/lv_themes/lv_theme_alien.c"
    "src/display/lv_themes/lv_theme_default.c"
    "src/display/lv_themes/lv_theme_material.c"
    "src/display/lv_themes/lv_theme_mono.c"
    "src/display/lv_themes/lv_theme_nemo.c"
    "src/display/lv_themes/lv_theme_night.c"
    "src/display/lv_themes/lv_theme_templ.c"
    "src/display/lv_themes/lv_theme_zen.c"
)

set(DISPLAY_HDRS
    "include/display/lvgl.h"
    "include/display/lv_conf.h"
    "include/display/lv_conf_checker.h"
    "include/display/lv_core/lv_group.h"
    "include/display/lv_core/lv_indev.h"
    "include/display/lv_core/lv_lang.h"
    "include/display/lv_core/lv_obj.h"
    "include/display/lv_core/lv_refr.h"
    "include/display/lv_core/lv_style.h"
    "include/display/lv_core/lv_vdb.h"
    "include/display/lv_draw/lv_draw.h"
    "include/display/lv_draw/lv_draw_arc.h"
    "include/display/lv_draw/lv_draw_img.h"
    "include/display/lv_draw/lv_draw_label.h"
    "include/display/lv_draw/lv_draw_line.h"
    "include/display/lv_draw/lv_draw_rbasic.h"
    "include/display/lv_draw/lv_draw_rect.h"
    "include/display/lv_draw/lv_draw_triangle.h"
    "include/display/lv_draw/lv_draw_vbasic.h"
    "include/display/lv_fonts/lv_font_builtin.h"
    "include/display/lv_hal/lv_hal.h"
    "include/display/lv_hal/lv_hal_disp.h"
    "include/display/lv_hal/lv_hal_indev.h"
    "include/display/lv_hal/lv_hal_tick.h"
    "include/display/lv_misc/lv_anim.h"
    "include/display/lv_misc/lv_area.h"
    "include/display/lv_misc/lv_circ.h"
    "include/display/lv_misc/lv_color.h"
    "include/display/lv_misc/lv_font.h"
    "include/display/lv_misc/lv_fs.h"
    "include/display/lv_misc/lv_gc.h"
    "include/display/lv_misc/lv_ll.h"
    "include/display/lv_misc/lv_log.h"
    "include/display/lv_misc/lv_math.h"
    "include/display/lv_misc/lv_mem.h"
    "include/display/lv_misc/lv_symbol_def.h"
    "include/display/lv_misc/lv_task.h"
    "include/display/lv_misc/lv_templ.h"
    "include/display/lv_misc/lv_txt.h"
    "include/display/lv_misc/lv_ufs.h"
    "include/display/lv_objx/lv_arc.h"
    "include/display/lv_objx/lv_bar.h"
    "include/display/lv_objx/lv_btn.h"
    "include/display/lv_objx/lv_btnm.h"
    "include/display/lv_objx/lv_calendar.h"
    "include/display/lv_objx/lv_canvas.h"
    "include/display/lv_objx/lv_cb.h"
    "include/display/lv_objx/lv_chart.h"
    "include/display/lv_objx/lv_cont.h"
    "include/display/lv_objx/lv_ddlist.h"
    "include/display/lv_objx/lv_gauge.h"
    "include/display/lv_objx/lv_img.h"
    "include/display/lv_objx/lv_imgbtn.h"
    "include/display/lv_objx/lv_kb.h"
    "include/display/lv_objx/lv_label.h"
    "include/display/lv_objx/lv_led.h"
    "include/display/lv_objx/lv_line.h"
    "include/display/lv_objx/lv_list.h"
    "include/display/lv_objx/lv_lmeter.h"
    "include/display/lv_objx/lv_mbox.h"
    "include/display/lv_objx/lv_objx_templ.h"
    "include/display/lv_objx/lv_page.h"
    "include/display/lv_objx/lv_preload.h"
    "include/display/lv_objx/lv_roller.h"
    "include/display/lv_objx/lv_slider.h"
    "include/display/lv_objx/lv_spinbox.h"
    "include/display/lv_objx/lv_sw.h"
    "include/display/lv_objx/lv_ta.h"
    "include/display/lv_objx/lv_table.h"
    "include/display/lv_objx/lv_tabview.h"
    "include/display/lv_objx/lv_tileview.h"
    "include/display/lv_objx/lv_win.h"
    "include/display/lv_themes/lv_theme.h"
    "include/display/lv_themes/lv_theme_alien.h"
    "include/display/lv_themes/lv_theme_default.h"
    "include/display/lv_themes/lv_theme_material.h"
    "include/display/lv_themes/lv_theme_mono.h"
    "include/display/lv_themes/lv_theme_nemo.h"
    "include/display/lv_themes/lv_theme_night.h"
    "include/display/lv_themes/lv_theme_templ.h"
    "include/display/lv_themes/lv_theme_zen.h"
    "include/display/lv_version.h"
)

add_library(LVGL ${DISPLAY_SRCS} ${DIPLAY_HDRS})
target_include_directories(LVGL
    PUBLIC "${INCDIR}"
           "${INCDIR}/display/lv_core"
           "${INCDIR}/display/lv_draw"
           "${INCDIR}/display/lv_fonts"
           "${INCDIR}/display/lv_hal"
           "${INCDIR}/display/lv_misc"
           "${INCDIR}/display/lv_objx"
           "${INCDIR}/display/lv_themes"
    PRIVATE "${FWDIR}/libv5rts/sdk/vexv5/include"
)

# build libpros.a

set(FREERTOS_SRCS
    "src/rtos/heap_4.c"
    "src/rtos/list.c"
    "src/rtos/port.c"
    "src/rtos/portASM.S"
    "src/rtos/queue.c"
    "src/rtos/rtos.cpp"
    "src/rtos/semphr.c"
    "src/rtos/stream_buffer.c"
    "src/rtos/task_notify_when_deleting.c"
    "src/rtos/tasks.c"
    "src/rtos/timers.c"
)

set(FREERTOS_HDRS
    "include/rtos/FreeRTOS.h"
    "include/rtos/FreeRTOSConfig.h"
    "include/rtos/list.h"
    "include/rtos/message_buffer.h"
    "include/rtos/portable.h"
    "include/rtos/portmacro.h"
    "include/rtos/projdefs.h"
    "include/rtos/queue.h"
    "include/rtos/semphr.h"
    "include/rtos/stack_macros.h"
    "include/rtos/StackMacros.h"
    "include/rtos/stream_buffer.h"
    "include/rtos/task.h"
    "include/rtos/tcb.h"
    "include/rtos/timers.h"
)

set(PROS_SRCS
    "src/common/cobs.c"
    "src/common/gid.c"
    "src/common/linkedlist.c"
    "src/common/set.c"
    "src/common/string.c"
    "src/devices/battery.c"
    "src/devices/battery.cpp"
    "src/devices/controller.c"
    "src/devices/controller.cpp"
    "src/devices/registry.c"
    "src/devices/vdml_adi.c"
    "src/devices/vdml_adi.cpp"
    "src/devices/vdml_distance.c"
    "src/devices/vdml_distance.cpp"
    "src/devices/vdml_ext_adi.c"
    "src/devices/vdml_imu.c"
    "src/devices/vdml_imu.cpp"
    "src/devices/vdml_motors.c"
    "src/devices/vdml_motors.cpp"
    "src/devices/vdml_optical.c"
    "src/devices/vdml_optical.cpp"
    "src/devices/vdml_rotation.c"
    "src/devices/vdml_rotation.cpp"
    "src/devices/vdml_serial.c"
    "src/devices/vdml_serial.cpp"
    "src/devices/vdml_usd.c"
    "src/devices/vdml_usd.cpp"
    "src/devices/vdml_vision.c"
    "src/devices/vdml_vision.cpp"
    "src/devices/vdml.c"
    "${DISPLAY_SRCS}"
    "${FREERTOS_SRCS}"
    "src/system/dev/dev_driver.c"
    "src/system/dev/file_system_stubs.c"
    "src/system/dev/ser_daemon.c"
    "src/system/dev/ser_driver.c"
    "src/system/dev/usd_driver.c"
    "src/system/dev/vfs.c"
    "src/system/cpp_support.cpp"
    "src/system/envlock.c"
    "src/system/hot.c"
    "src/system/mlock.c"
    "src/system/newlib_stubs.c"
    "src/system/rtos_hooks.c"
    "src/system/startup.c"
    "src/system/system_daemon.c"
    "src/system/unwind.c"
    "src/system/user_functions.c"
    "src/system/xilinx_vectors.s"
)

set(PROS_HDRS
    "include/common/cobs.h"
    "include/common/gid.h"
    "include/common/linkedlist.h"
    "include/common/set.h"
    "include/common/string.h"
    "${DISPLAY_HDRS}"
    "include/pros/adi.h"
    "include/pros/adi.hpp"
    "include/pros/api_legacy.h"
    "include/pros/apix.h"
    "include/pros/colors.h"
    "include/pros/distance.h"
    "include/pros/distance.hpp"
    "include/pros/ext_adi.h"
    "include/pros/imu.h"
    "include/pros/imu.hpp"
    "include/pros/llemu.h"
    "include/pros/llemu.hpp"
    "include/pros/misc.h"
    "include/pros/misc.hpp"
    "include/pros/motors.h"
    "include/pros/motors.hpp"
    "include/pros/optical.h"
    "include/pros/optical.hpp"
    "include/pros/rotation.h"
    "include/pros/rotation.hpp"
    "include/pros/rtos.h"
    "include/pros/rtos.hpp"
    "include/pros/serial.h"
    "include/pros/serial.hpp"
    "include/pros/vision.h"
    "include/pros/vision.hpp"
    "${FREERTOS_HDRS}"
    "include/system/dev/banners.h"
    "include/system/dev/dev.h"
    "include/system/dev/ser.h"
    "include/system/dev/usd.h"
    "include/system/dev/vfs.h"
    "include/system/user_functions/c_list.h"
    "include/system/user_functions/cpp_list.h"
    "include/system/user_functions/list.h"
    "include/system/hot.h"
    "include/system/optimizers.h"
    "include/system/user_functions.h"
    "include/vdml/port.h"
    "include/vdml/registry.h"
    "include/vdml/vdml.h"
    "include/api.h"
    "include/kapi.h"
    "include/main.h"
)

add_library(pros STATIC ${PROS_SRCS} ${PROS_HDRS})
add_dependencies(pros LVGL)
target_include_directories(pros
    PUBLIC ${INCDIR}
    # HACK: mimic the "same source structure includes" we had in the old makefile, but only for the annoying-to-edit
    #       display and RTOS sources
    PUBLIC ${INCIDR}/display
    PUBLIC ${INCDIR}/display/lv_core
    PUBLIC ${INCDIR}/display/lv_draw
    PUBLIC ${INCDIR}/display/lv_fonts
    PUBLIC ${INCDIR}/display/lv_hal
    PUBLIC ${INCDIR}/display/lv_misc
    PUBLIC ${INCDIR}/display/lv_objx
    PUBLIC ${INCDIR}/display/lv_themes
    PUBLIC ${INCDIR}/rtos
    PRIVATE ${FWDIR}/libv5rts/sdk/vexv5/include
)

# strip conflicting symbols from libv5rts.a
add_custom_command(
    TARGET pros PRE_LINK
    COMMAND "${CMAKE_STRIP}" "${FWDIR}/libv5rts/sdk/vexv5/libv5rts.a"
        @${CMAKE_SOURCE_DIR}/libv5rts-strip-options.txt
        -o "${CMAKE_BINARY_DIR}/libv5rts.patched.a"
    BYPRODUCTS "${CMAKE_BINARY_DIR}/libv5rts.patched.a"
)

# extract objects from libv5rts.a
add_custom_command(
    TARGET pros PRE_LINK
    DEPENDS "${CMAKE_BINARY_DIR}/libv5rts.patched.a"
    COMMAND mkdir -p "${CMAKE_BINARY_DIR}/libv5rts"
    COMMAND "${CMAKE_AR_TOOL}" x "${CMAKE_BINARY_DIR}/libv5rts.patched.a" --output "${CMAKE_BINARY_DIR}/libv5rts"
    BYPRODUCTS "${CMAKE_BINARY_DIR}/libv5rts/*.obj"
)

# insert extracted libv5rts.a objects into libpros.a
add_custom_command(
    TARGET pros POST_BUILD
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/libv5rts
    COMMAND "${CMAKE_AR_TOOL}" rcs "${CMAKE_BINARY_DIR}/libpros.a" ./*.obj
    COMMAND cp "${CMAKE_BINARY_DIR}/libpros.a" "${CMAKE_SOURCE_DIR}/firmware/libpros.a"
    BYPRODUCTS "${CMAKE_SOURCE_DIR}/firmware/libpros.a"
)

set(LIBS "")
string(REPLACE ";" "," _LIBS "${LIBS}")

# monolith build
add_executable(monolith "src/main.cpp" ${PROS_HDRS})
target_include_directories(monolith PUBLIC ${INCDIR})
linker_script_target_dependency(monolith "${FWDIR}/v5.ld")
linker_script_target_dependency(monolith "${FWDIR}/v5-common.ld")

generate_pros_ld_timestamp(monolith)

set(LNK_FLAGS "--gc-sections,--start-group,${FWDIR}/libc.a,${FWDIR}/libm.a,${_LIBS},-lgcc,-lstdc++,--end-group,-T${FWDIR}/v5-common.ld")

target_link_libraries(monolith
    LVGL
    pros
    "${CMAKE_BINARY_DIR}/monolith/_pros_ld_timestamp.obj"
    -nostdlib
    -Wl,-T${FWDIR}/v5.ld,${LNK_FLAGS}
)

firmware_size(monolith)
generate_object(monolith .bin binary .hot_init)

# hot/cold build
add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/cold.elf
    ${CMAKE_CXX_COMPILER}
        -nostdlib
        -Wl,--gc-keep-exported,--whole-archive,${FWDIR}/libc.a,${FWDIR}/libm.a,${_LIBS},-lstdc++,--no-whole-archive
        -Wl,-T${FWDIR}/v5.ld,${LNK_FLAGS},-o,${CMAKE_BINARY_DIR}/cold.elf
    COMMAND ${CMAKE_OBJCOPY}
        -N install_hot_table
        -N __libc_init_array
        -N _PROS_COMPILE_DIRECTORY
        -N _PROS_COMPILE_TIMESTAMP
        ${CMAKE_BINARY_DIR}/cold.elf ${CMAKE_BINARY_DIR}/cold.elf
    DEPENDS LVGL pros "${FWDIR}/libc.a" "${FWDIR}/libm.a"
    VERBATIM
)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/cold.bin
    ${CMAKE_OBJCOPY} ${CMAKE_BINARY_DIR}/cold.elf -O binary -R .hot_init ${CMAKE_BINARY_DIR}/cold.bin
    MAIN_DEPENDENCY ${CMAKE_BINARY_DIR}/cold.elf
)

add_custom_target(cold.bin DEPENDS "${CMAKE_BINARY_DIR}/cold.bin")

add_executable(hot "src/main.cpp" ${PROS_HDRS})
add_dependencies(hot cold.bin)
generate_pros_ld_timestamp(hot)
target_link_libraries(hot
    -Wl,-nostartfiles,-R,${CMAKE_BINARY_DIR}/cold.elf
    LVGL
    pros
    "${CMAKE_BINARY_DIR}/hot/_pros_ld_timestamp.obj"
    "${FWDIR}/libc.a" "${FWDIR}/libm.a"
    -Wl,-T${FWDIR}/v5-hot.ld,${LNK_FLAGS}
)
linker_script_target_dependency(hot "${FWDIR}/v5.ld")
linker_script_target_dependency(hot "${FWDIR}/v5-common.ld")
linker_script_target_dependency(hot "${FWDIR}/v5-hot.ld")

firmware_size(hot)
generate_object(hot .bin binary)