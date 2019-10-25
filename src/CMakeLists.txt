project(PROS)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)

include_directories(../firmware/libv5rts/sdk/vexv5/include)

add_library(PROS
common/cobs.c
common/gid.c
common/linkedlist.c
common/set.c
common/string.c
devices/battery.c
devices/controller.c
devices/registry.c
devices/vdml.c
devices/vdml_adi.c
devices/vdml_motors.c
devices/vdml_serial.c
devices/vdml_vision.c
system/dev/dev_driver.c
system/dev/ser_daemon.c
system/dev/ser_driver.c
system/dev/usd_driver.c
system/dev/vfs.c
system/envlock.c
system/hot.c
system/mlock.c
system/newlib_stubs.c
system/system_daemon.c
system/user_functions.c
devices/battery.cpp
devices/controller.cpp
devices/vdml_adi.cpp
devices/vdml_motors.cpp
devices/vdml_serial.cpp
devices/vdml_vision.cpp
display/llemu.cpp
rtos/rtos.cpp
system/cpp_support.cpp
system/startup.c
system/rtos_hooks.c
system/unwind.c
system/xilinx_vectors.s
)

add_subdirectory(display)
add_subdirectory(rtos)

target_link_libraries(PROS
INTERFACE LVGL
INTERFACE RTOS)
