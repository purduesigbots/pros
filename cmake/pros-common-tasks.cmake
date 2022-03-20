include(arm-none-eabi-binutils.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(FW_DIR ${CMAKE_SOURCE_DIR}/firmware)

# convenient linker commands
set(WHOLE_ARCHIVE "-Wl,--whole-archive")
set(NO_WHOLE_ARCHIVE "-Wl,--no-whole-archive")
set(START_GROUP "-Wl,--start-group")
set(END_GROUP "-Wl,--end-group")

set(TIMESTAMP_FILE ${FW_DIR}/timestamp.c)

# this file gets generated at configure time
configure_file(${FW_DIR}/timestamp.c.in ${TIMESTAMP_FILE})

# add target others can depend on to get a fresh timestamp every build
add_custom_target(rebuild_timestamp
        COMMAND ${CMAKE_COMMAND} -E touch_nocreate ${TIMESTAMP_FILE})

# convenience function for creating an executable with a dependency on
# rebuild_timestamp
function(add_timestamped_executable TARGET SOURCES)
    add_executable(${TARGET} ${TIMESTAMP_FILE} ${SOURCES})
    add_dependencies(${TARGET} rebuild_timestamp)
endfunction()

# create TARGET_NAME.elf as a target
function(add_pros_executable TARGET_NAME SOURCES)
    add_timestamped_executable(${TARGET_NAME}.elf "${SOURCES}")
    target_link_options(${TARGET_NAME}.elf -nostdlib)
    print_section_sizes(${TARGET_NAME}.elf)
endfunction()

# create monolith.elf and monolith.bin
function(add_pros_monolith SOURCES LIBRARIES)
    add_pros_executable(monolith "${SOURCES}")

    target_link_libraries(monolith.elf
            "-Wl,-T ${FW_DIR}/v5.ld"
            ${START_GROUP} ${LIBRARIES} gcc stdc++ ${END_GROUP}
            "-Wl,-T ${FW_DIR}/v5-common.ld")

    # strip out the .hot_init section when creating monolith.bin
    create_binary(monolith "-R .hot_init")
endfunction()

# create cold.package.elf and cold.package.bin
function(add_pros_cold_package COLD_LIBRARIES LIBRARIES)
    # CMake requires that every executable have at least one source, even if that
    # source is completely empty. so here we create a dummy cold.cpp file to use
    file(WRITE ${CMAKE_BINARY_DIR}/cold.cpp "")

    add_pros_executable(cold.package ${CMAKE_BINARY_DIR}/cold.cpp)

    target_link_libraries(cold.package.elf
            -Wl,--gc-keep-exported
            ${WHOLE_ARCHIVE} ${COLD_LIBRARIES} stdc++ ${NO_WHOLE_ARCHIVE}
            "-Wl,-T ${FW_DIR}/v5.ld"
            ${START_GROUP} ${LIBRARIES} gcc stdc++ ${END_GROUP}
            "-Wl,-T ${FW_DIR}/v5-common.ld")

    # postprocess the cold package to strip out some symbols we don't want to
    # keep in it before creating cold.package.bin
    add_custom_command(TARGET cold.package.elf
            POST_BUILD
            COMMAND "${CMAKE_OBJCOPY}"
            "--strip-symbol=install_hot_table"
            "--strip-symbol=__libc_init_array"
            "--strip-symbol=_PROS_COMPILE_DIRECTORY"
            "--strip-symbol=_PROS_COMPILE_TIMESTAMP"
            "cold.package.elf" "cold.package.elf")

    create_binary(cold.package)
endfunction()

# create hot.package.elf and hot.package.bin
function(add_pros_hot_package SOURCES LIBRARIES)
    add_pros_executable(hot.package "${SOURCES}")

    target_link_options(hot.package.elf -nostartfiles)

    target_link_libraries(hot.package.elf
            "-Wl,-R cold.package.elf"
            ${LIBRARIES}
            "-Wl,-T ${FW_DIR}/v5-hot.ld"
            ${START_GROUP} ${LIBRARIES} gcc stdc++ ${END_GROUP}
            "-Wl,-T ${FW_DIR}/v5-common.ld")

    create_binary(hot.package)
endfunction()