#pragma once

struct hot_table {
	char const* compile_timestamp;
	char const* compile_directory;

	struct {
#define FUNC(F) void (*F)();
#include "system/user_functions/list.h"
#undef FUNC
	} functions;
};

extern struct hot_table* const HOT_TABLE;
