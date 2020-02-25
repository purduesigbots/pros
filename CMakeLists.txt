cmake_minimum_required(VERSION 3.6)

option(V5_COMPILE "Compile for the V5 BRAIN" ON)

if(V5_COMPILE)
	set(CMAKE_SYSTEM_PROCESSOR arm)
   	set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

   	set(CMAKE_C_COMPILER arm-none-eabi-gcc)
   	set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
   	set(STRIP arm-none-eabi-strip)
endif()

project(PROSPROJECT)

include_directories(include
					firmware/libv5rts/sdk/vexv5/include)


add_compile_options(-Wall --pedantic -Wno-psabi)
add_compile_options(-g) # -Os
add_compile_options(-D_POSIX_THREADS -D_UNIX98_THREAD_MUTEX_ATTRIBUTES)
add_compile_options(-ffunction-sections -fdata-sections -fdiagnostics-color -funwind-tables)

list(APPEND SRC_FILES
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
	tests/testhost.cpp
	tests/src/basicTest.cpp
)

if(V5_COMPILE)
	add_compile_options(-mcpu=cortex-a9 -mfpu=neon-fp16 -mfloat-abi=softfp)

	set(LIBRARIES ../firmware/libc.a,../firmware/libm.a,../firmware/libv5rts/sdk/vexv5/libv5rts.patched.a)
   	set(LINKERSCRIPT ./firmware/v5.ld)
   
	set(SDK_PATH ${PROSPROJECT_SOURCE_DIR}/firmware/libv5rts/sdk/vexv5)
   	set(PATCHED_SDK ${SDK_PATH}/libv5rts.patched.a)

	enable_language(ASM)

   	list(APPEND SRC_FILES
   		src/system/xilinx_vectors.s
		src/system/unwind.c
		src/system/rtos_hooks.c
   	)
else()
	add_compile_options(-DLINUX_COMPILE)

	include_directories(tests/include)
	list(APPEND SRC_FILES
		tests/LinuxStubs/vexOSStubs.c)
endif()

add_compile_options(${CPPFLAGS} ${WARNFLAGS} ${GCCFLAGS})

add_executable(PROSPROJECT
	${SRC_FILES}
)

set_property(TARGET PROSPROJECT PROPERTY CXX_STANDARD 17)
set_property(TARGET PROSPROJECT PROPERTY C_STANDARD 11)

add_subdirectory(src/display)
set(BUILT_LIBRARIES ./src/display/libLVGL.a)
set(SDK_PATH ${PROSPROJECT_SOURCE_DIR}/firmware/libv5rts/sdk/vexv5)
set(PATCHED_SDK ${SDK_PATH}/libv5rts.patched.a)
add_dependencies(PROSPROJECT LVGL)

add_dependencies(PROSPROJECT RTOS)
set(BUILT_LIBRARIES "${BUILT_LIBRARIES},./src/rtos/libRTOS.a")
add_subdirectory(src/rtos)

if(V5_COMPILE)
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

   add_custom_command(TARGET PROSPROJECT
			                 POST_BUILD
							 COMMAND arm-none-eabi-objcopy PROSPROJECT -O binary -R .hot_init TESTMONOLITH.bin
							 DEPENDS PROSPROJECT
							 BYPRODUCTS TESTMONOLITH.bin)

   target_link_libraries(PROSPROJECT ${MFLAGS} ${WARNFLAGS} -nostdlib ${GCCFLAGS} -Wl,-T${LINKERSCRIPT},--gc-sections,--start-group,${BUILT_LIBRARIES},${LIBRARIES},-lgcc,-lstdc++,--end-group)
else()
	target_link_libraries(PROSPROJECT RTOS LVGL pthread)
endif()
