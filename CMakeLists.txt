cmake_minimum_required(VERSION 3.6)

set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(tools /usr/bin)
set(CMAKE_C_COMPILER ${tools}/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${tools}/arm-none-eabi-g++)
set(STRIP ${tools}/arm-none-eabi-strip)
set(ROOT ${PROSPROJECT_SOURCE_DIR})

project(PROSPROJECT)

include_directories(include
					firmware/libv5rts/sdk/vexv5/include
					tests/include)


set(WARNFLAGS -Wall -Wpedantic)
set(MFLAGS -mcpu=cortex-a9 -mfpu=neon-fp16 -mfloat-abi=softfp -Os -g)
set(CPPFLAGS -D_POSIX_THREADS -D_UNIX98_THREAD_MUTEX_ATTRIBUTES)
set(GCCFLAGS -ffunction-sections -fdata-sections -fdiagnostics-color -funwind-tables)

add_compile_options(${MFLAGS} ${CPPFLAGS} ${WARNFLAGS} ${GCCFLAGS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=gnu++17")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=gnu11")

set(LIBRARIES ../firmware/libc.a,../firmware/libm.a,../firmware/libv5rts/sdk/vexv5/libv5rts.patched.a)
set(LINKERSCRIPT ./firmware/v5.ld)

set(SDK_PATH ${ROOT}/firmware/libv5rts/sdk/vexv5)
set(PATCHED_SDK ${SDK_PATH}/libv5rts.patched.a)

enable_language(ASM)

add_executable(PROSPROJECT
src/common/cobs.c
src/common/gid.c
src/common/linkedlist.c
src/common/set.c
src/common/string.c
src/devices/battery.c
src/devices/controller.c
src/devices/registry.c
src/devices/vdml.c
src/devices/vdml_adi.c
src/devices/vdml_motors.c
src/devices/vdml_serial.c
src/devices/vdml_vision.c
src/system/dev/dev_driver.c
src/system/dev/ser_daemon.c
src/system/dev/ser_driver.c
src/system/dev/usd_driver.c
src/system/dev/vfs.c
src/system/envlock.c
src/system/hot.c
src/system/mlock.c
src/system/newlib_stubs.c
src/system/system_daemon.c
src/system/user_functions.c
src/devices/battery.cpp
src/devices/controller.cpp
src/devices/vdml_adi.cpp
src/devices/vdml_motors.cpp
src/devices/vdml_serial.cpp
src/devices/vdml_vision.cpp
src/display/llemu.cpp
src/rtos/rtos.cpp
src/system/cpp_support.cpp
src/system/startup.c
src/system/rtos_hooks.c
src/system/unwind.c
src/system/xilinx_vectors.s

tests/testhost.cpp
)

add_subdirectory(src/display)
add_subdirectory(src/rtos)

add_custom_command(TARGET PROSPROJECT
						  PRE_LINK
						  COMMAND mkdir -p ./firmware
						  COMMAND cp ../firmware/*.ld ./firmware/
						  BYPRODUCTS v5-common.ld v5-hot.ld v5.ld
)

add_custom_command(TARGET PROSPROJECT
						  PRE_LINK
						  COMMAND ${STRIP} libv5rts.a @${PROJECT_SOURCE_DIR}/libv5rts-strip-options.txt -o ${PATCHED_SDK}
						  DEPENDS ${SDK_PATH}/libv5rts.a
						  WORKING_DIRECTORY ${SDK_PATH}
						  )

add_dependencies(PROSPROJECT
LVGL
RTOS
)


set(BUILTLIBRARIES ./src/rtos/libRTOS.a,./src/display/libLVGL.a)

target_link_libraries(PROSPROJECT ${MFLAGS} ${WARNFLAGS} -nostdlib ${GCCFLAGS} -Wl,-T${LINKERSCRIPT},--gc-sections,--start-group,${BUILTLIBRARIES},${LIBRARIES},-lc,-lm,-lgcc,-lstdc++,-lsupc++,--end-group)
