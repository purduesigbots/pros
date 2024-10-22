#pragma once

#include <unwind-arm-common.h>
struct hot_table {
	char const* compile_timestamp;
	char const* compile_directory;

	void* __exidx_start;
	void* __exidx_end;

	struct {
#define FUNC(F) void (*F)();
#include "system/user_functions/list.h"
#undef FUNC
	} functions;
};

extern struct hot_table* const HOT_TABLE;

// exidx is the table that tells the unwinder how to unwind a stack frame
// for a PC. Under hot/cold, there's two tables and the unwinder was kind
// enough to let us implement a function to give it a table for a PC so
// support for hot/cold is as easy as it gets
struct __EIT_entry {
	_uw fnoffset;
	_uw content;
};