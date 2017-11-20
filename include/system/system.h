#pragma once

#include <stdint.h>

typedef int32_t word_t;
typedef uint32_t uword_t;

#define TASK_PRIORITY_MAX 16
#define TASK_PRIORITY_MIN 1
#define TASK_PRIORITY_DEFAULT 8

// Default stack size is 32 KB (a lot)
#define TASK_STACK_DEPTH_DEFAULT 8192
#define TASK_STACK_DEPTH_MIN 256
