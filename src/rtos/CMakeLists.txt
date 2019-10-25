project(RTOS)

include_directories(../../include/rtos)

add_library(RTOS
heap_4.c
list.c
semphr.c
stream_buffer.c
task_notify_when_deleting.c
port.c
queue.c
tasks.c
timers.c
)
