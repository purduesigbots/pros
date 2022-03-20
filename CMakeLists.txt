cmake_minimum_required(VERSION 3.22)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(arm-none-eabi-binutils.cmake)
include(pros-common-tasks.cmake)
include(version.cmake)

project(pros)

set(C_SOURCES
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
        src/devices/vdml_distance.c
        src/devices/vdml_ext_adi.c
        src/devices/vdml_gps.c
        src/devices/vdml_imu.c
        src/devices/vdml_motors.c
        src/devices/vdml_optical.c
        src/devices/vdml_rotation.c
        src/devices/vdml_serial.c
        src/devices/vdml_usd.c
        src/devices/vdml_vision.c
        src/display/llemu.c
        src/rtos/heap_4.c
        src/rtos/list.c
        src/rtos/port.c
        src/rtos/queue.c
        src/rtos/semphr.c
        src/rtos/semphr.c
        src/rtos/task_notify_when_deleting.c
        src/rtos/tasks.c
        src/rtos/timers.c
        src/system/dev/dev_driver.c
        src/system/dev/file_system_stubs.c
        src/system/dev/ser_daemon.c
        src/system/dev/ser_driver.c
        src/system/dev/usd_driver.c
        src/system/dev/vfs.c
        src/system/envlock.c
        src/system/hot.c
        src/system/mlock.c
        src/system/newlib_stubs.c
        src/system/rtos_hooks.c
        src/system/startup.c
        src/system/system_daemon.c
        src/system/unwind.c
        src/system/user_functions.c)
set(CXX_SOURCES
        src/devices/battery.cpp
        src/devices/controller.cpp
        src/devices/screen.cpp
        src/devices/vdml_adi.cpp
        src/devices/vdml_distance.cpp
        src/devices/vdml_gps.cpp
        src/devices/vdml_imu.cpp
        src/devices/vdml_motors.cpp
        src/devices/vdml_optical.cpp
        src/devices/vdml_rotation.cpp
        src/devices/vdml_serial.cpp
        src/devices/vdml_usd.cpp
        src/devices/vdml_vision.cpp
        src/display/llemu.cpp
        src/rtos/rtos.cpp
        src/system/cpp_support.cpp)
set(ASM_SOURCES
        src/rtos/portASM.S
        src/system/xilinx_vectors.s)
set(INCLUDE_DIRECTORIES
        include
        ${FW_DIR}/libv5rts/sdk/vexv5/include)

find_package(V5rts)
add_subdirectory(src/display)

set(LIBRARIES
        lvgl
        ${FW_DIR}/libc.a
        ${FW_DIR}/libm.a)

if(DEFINED LIBRARY_BUILD) # pros configure --library
    add_library(libpros.a STATIC ${C_SOURCES} ${CXX_SOURCES} ${ASM_SOURCES})
    target_include_directories(libpros.a PUBLIC ${INCLUDE_DIRECTORIES})
    add_dependencies(libpros.a always_check_version)
    add_custom_command(TARGET libpros.a
            PRE_LINK
            COMMAND ${CMAKE_STRIP} $<TARGET_FILE:V5rts::V5rts> @${CMAKE_SOURCE_DIR}/libv5rts-strip-options.txt ${CMAKE_BINARY_DIR}/libv5rts.patched.a
            BYPRODUCTS ${CMAKE_BINARY_DIR}/libv5rts.patched.a)
    target_link_libraries(libpros.a PUBLIC lvgl PRIVATE ${CMAKE_BINARY_DIR}/libv5rts.patched.a)

    list(PREPEND libpros.a)
endif()

if(DEFINED MONOLITH_BUILD) # pros configure [--library] --monolith
    set(MONOLITH_SOURCES src/main.cpp)
    if(NOT DEFINED LIBRARY_BUILD)
        list(APPEND MONOLITH_SOURCES ${C_SOURCES} ${CXX_SOURCES} ${ASM_SOURCES})
    endif()

    add_pros_monolith("${MONOLITH_SOURCES}" "${LIBRARIES}")

    if(NOT DEFINED LIBRARY_BUILD)
        add_dependencies(monolith.elf always_check_version)
        target_include_directories(monolith.elf ${INCLUDE_DIRECTORIES})
    endif()
else() # pros configure [--library]
    ### cold package ###
    add_pros_cold_package("${LIBRARIES}" "${LIBRARIES}")

    ### hot package ###
    set(HOT_SOURCES src/main.cpp)
    if(NOT DEFINED LIBRARY_BUILD)
        list(APPEND HOT_SOURCES ${C_SOURCES} ${CXX_SOURCES} ${ASM_SOURCES})
    endif()

    add_pros_hot_package("${HOT_SOURCES}" "${LIBRARIES}")

    if(NOT DEFINED LIBRARY_BUILD)
        add_dependencies(hot.package.elf always_check_version)
        target_include_directories(hot.package.elf ${INCLUDE_DIRECTORIES})
    endif()
endif()

set(INC_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEMPLATE_DIR ${CMAKE_SOURCE_DIR}/template)
set(TEMPLATE_FILES
        ${FW_DIR}/v5.ld
        ${FW_DIR}/v5-common.ld
        ${FW_DIR}/v5-hot.ld
        ${FW_DIR}/libc.a
        ${FW_DIR}/libm.a
        ${FW_DIR}/timestamp.c.in
        ${INC_DIR}/api.h
        ${INC_DIR}/main.h
        ${INC_DIR}/pros
        ${INC_DIR}/display
        ${SRC_DIR}/main.cpp
        ${CMAKE_SOURCE_DIR}/cmake/arm-none-eabi-binutils.cmake
        ${CMAKE_SOURCE_DIR}/cmake/arm-none-eabi-toolchain.cmake
        ${CMAKE_SOURCE_DIR}/pros-common-tasks.cmake)

read_version(TEMPLATE_VERSION)
add_custom_target(template
        COMMENT "Creating template version ${TEMPLATE_VERSION}"
        COMMAND ${CMAKE_COMMAND} -E make_directory ${TEMPLATE_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${TEMPLATE_FILES} ${TEMPLATE_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${TEMPLATE_DIR}/firmware
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:libpros.a> ${TEMPLATE_DIR}/firmware
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/CMakeLists.template.txt ${TEMPLATE_DIR}/CMakeLists.txt
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/template-gitignore ${CMAKE_SOURCE_DIR}/.gitignore
        COMMAND pros conduct create-template
            ${TEMPLATE_DIR}
            kernel ${TEMPLATE_VERSION}
            --system ./**/*
            --user src/main.{cpp,c,cc}
            --user include/main.{hpp,h,hh}
            --user CMakeLists.txt
            --user .gitignore
            --target v5
            --output build/monolith.bin
            --cold_output build/cold.package.bin
            --hot_output build/hot.package.bin
            --cold_addr 58720256
            --hot_addr 125829120)