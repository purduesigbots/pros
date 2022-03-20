list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(TOOLCHAIN arm-none-eabi)

if(NOT DEFINED ENV{PROS_TOOLCHAIN})
    if(CMAKE_HOST_SYSTEM_NAME STREQUAL Linux)
        set(PREFIX /usr) # TODO: need to verify paths
    elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL Darwin)
        set(PREFIX /opt/homebrew)
    elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL Windows)
        message(FATAL_ERROR "please make sure PROS_TOOLCHAIN is set!")
    endif()
else()
    set(PREFIX $ENV{PROS_TOOLCHAIN})
endif()

set(TOOLCHAIN_BIN_DIR ${PREFIX}/bin)
set(TOOLCHAIN_INC_DIR ${PREFIX}/${TOOLCHAIN}/include)
set(TOOLCHAIN_LIB_DIR ${PREFIX}/${TOOLCHAIN}/lib)

set(EXT "")

if(CMAKE_HOST_SYSTEM_NAME STREQUAL Darwin)
    # the arm-gcc-bin formula for homebrew needs some massaging to get the right
    # paths in a version-agnostic way
    set(TOOLCHAIN_INC_DIR ${PREFIX}/share/gcc-${TOOLCHAIN}/../../${TOOLCHAIN}/include)
    set(TOOLCHAIN_LIB_DIR ${PREFIX}/share/gcc-${TOOLCHAIN}/../../${TOOLCHAIN}/lib)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL Windows)
    set(EXT ".exe")
endif()

# test the compiler with a static library instead of a binary to avoid falsely
# reporting a broken compiler (when it can't link with the normal stdlib)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# default to gnu11 for C and gnu++17 for c++. users can override this
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(MACHINE_FLAGS
        -mcpu=cortex-a9
        -mfpu=neon-fp16
        -mfloat-abi=softfp
        -Os
        -g)
set(GCC_FLAGS
        -ffunction-sections
        -fdata-sections
        -fdiagnostics-color
        -funwind-tables)
set(CPP_FLAGS -D_POSIX_THREADS -D_UNIX98_THREAD_MUTEX_ATTRIBUTES)
set(WARN_FLAGS -Wno-psabi)

set(CMAKE_C_FLAGS_INIT ${MACHINE_FLAGS} ${CPP_FLAGS} ${WARN_FLAGS} ${GCC_FLAGS} CACHE INTERNAL "Initial set of flags for the C Compiler")
set(CMAKE_CXX_FLAGS_INIT ${MACHINE_FLAGS} ${CPP_FLAGS} ${WARN_FLAGS} ${GCC_FLAGS} CACHE_INTERNAL "Initial set of flags for the C++ Compiler")
set(CMAKE_EXE_LINKER_FLAGS_INIT ${MACHINE_FLAGS} ${WARN_FLAGS} -nostdlib ${GCC_FLAGS} LINKER:--gc-sections CACHE INTERNAL "Initial set of flags for the linker")

set(CMAKE_C_COMPILER ${TOOLCHAIN_BIN_DIR}/${TOOLCHAIN}-gcc${EXT} CACHE INTERNAL "C Compiler")
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_BIN_DIR}/${TOOLCHAIN}-g++${EXT} CACHE INTERNAL "C++ Compiler")
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_BIN_DIR}/${TOOLCHAIN}-gcc${EXT} CACHE INTERNAL "ASM Compiler")