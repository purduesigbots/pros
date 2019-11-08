cmake_minimum_required(VERSION 3.6)

#TODO: template, hot/cold?, timestamp?

set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(tools /usr/bin)
set(CMAKE_C_COMPILER ${tools}/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${tools}/arm-none-eabi-g++)
set(STRIP ${tools}/arm-none-eabi-strip)
set(ROOT ${PROSPROJECT_SOURCE_DIR})

project(PROSPROJECT)

include_directories(include)

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

add_executable(PROSPROJECT
src/main.cpp
)
add_subdirectory(src)

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

add_custom_target(update_version
						  COMMAND python version.py
						  WORKING_DIRECTORY ${ROOT}
						  )

add_dependencies(PROS
update_version
)

add_dependencies(PROSPROJECT
PROS
)

set(BUILTLIBRARIES ./src/libPROS.a,./src/rtos/libRTOS.a,./src/display/libLVGL.a)

target_link_libraries(PROSPROJECT ${MFLAGS} ${WARNFLAGS} -nostdlib ${GCCFLAGS} -Wl,-T${LINKERSCRIPT},--gc-sections,--start-group,${BUILTLIBRARIES},${LIBRARIES},-lc,-lm,-lgcc,-lstdc++,-lsupc++,--end-group)
