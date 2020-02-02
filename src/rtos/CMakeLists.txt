project(RTOS)

enable_language(ASM)
include_directories(../../include/rtos)

list(APPEND RTOS_SRC_FILES
	list.c
	queue.c
	stream_buffer.c
	task_notify_when_deleting.c
	tasks.c
	timers.c
	semphr.c)


if (V5_COMPILE)
	list(APPEND RTOS_SRC_FILES
		heap_4.c
		port.c
		portASM.S)
else()
	list(APPEND RTOS_SRC_FILES
		${PROSPROJECT_SOURCE_DIR}/tests/freeRTOSLinux/src/port.c)
endif()

add_library(RTOS
	${RTOS_SRC_FILES}
)


