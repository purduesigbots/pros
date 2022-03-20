# set tools
set(CMAKE_OBJCOPY ${TOOLCHAIN_BIN_DIR}/${TOOLCHAIN}-objcopy${EXT})
set(CMAKE_SIZE ${TOOLCHAIN_BIN_DIR}/${TOOLCHAIN}-size${EXT})
set(CMAKE_READELF ${TOOLCHAIN_BIN_DIR}/${TOOLCHAIN}-readelf${EXT})
set(CMAKE_STRIP ${TOOLCHAIN_BIN_DIR}/${TOOLCHAIN}-strip${EXT})

find_program(NUMFMT NAMES gnumfmt numfmt)

find_program(SED sed)

# provide convenience function for printing the section sizes of a target
function(print_section_sizes TARGET)
    if(SED)
        set(SED_CMD "| sed -e 's/  dec/total/'")
    else()
        set(SED_CMD "")
    endif()
    add_custom_command(TARGET ${TARGET}
            POST_BUILD
            COMMAND printf "%s\n" "Section sizes:"
            COMMAND ${CMAKE_SIZE} -d --common ${TARGET} ${SED_CMD} | ${NUMFMT} --field=4 --header)
endfunction()

function(create_binary ELF_TARGET_NAME EXTRA_OPTIONS)
    add_custom_target(${ELF_TARGET_NAME}.bin
            ALL
            DEPENDS ${ELF_TARGET_NAME}.elf
            COMMAND ${CMAKE_OBJCOPY} ${ELF_TARGET_NAME}.elf -O binary ${EXTRA_OPTIONS} ${CMAKE_BINARY_DIR}/${ELF_TARGET_NAME}.bin
            BYPRODUCTS ${CMAKE_BINARY_DIR}/${ELF_TARGET_NAME}.bin)
endfunction()

