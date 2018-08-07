#pragma once

#include <stdint.h>
#include <string.h>

void jinx_init(void);

typedef struct {
	char** subelements;
	size_t n_subelements;
	uint8_t modifiable;
} jinx_var_extra_args_t;

typedef void* jinx_var_t;

// Package for variable tracking
typedef struct track_variable {
	jinx_var_t jinx_var;
	bool contains_func;
	union data {
		void* data_ptr;
		struct func_data {
			bool (*fill_func)(void* buff, void* args);
			void* args;
		} fill_func_data;
	} data_func;
	struct track_variable* next;
	struct track_variable* prev;
} jinx_track_s_t;

// we'll start off with JINX just polling all "jinx_begin_track" every 20ms.
// Will add the ability to change this later

jinx_var_t jinx_create_variable(uint16_t value_size, char* value_name, char* value_fmt,
                                jinx_var_extra_args_t* extra_args);
void jinx_push_variable(jinx_var_t jinx_value, void* ptr);

jinx_track_s_t* jinx_track_variable(jinx_var_t jinx_value, void* ptr);

jinx_track_s_t* jinx_track_variable_fptr(jinx_var_t jinx_value, bool (*fill_func)(void* buff,void* args),void* args_to_pass);

void jinx_untrack_variable(jinx_track_s_t* track_var);

#define JINX_FMT(x) \
	_Generic((x),                                    \
					char: "c", unsigned char: "B",           \
					_Bool: "?",                              \
					short: "h", unsigned short: "H",         \
					int: "i", unsigned int: "I",             \
					long: "l", unsigned long: "L",           \
					long long: "q", unsigned long long: "Q", \
					float: "f", double: "d",                 \
					char*: "s",                              \
					void*: "P"                               \
					 )
#define JINX_STR_HELPER(x) #x
#define JINX_ARR_FMT(x) \
	_Generic((*x), \
					char: JINX_STR_HELPER(sizeof(x)) "c",  unsigned char: JINX_STR_HELPER(sizeof(x)) "B", \
					_Bool: JINX_STR_HELPER(sizeof(x)) "?", \
					short: JINX_STR_HELPER(sizeof(x)) "h", unsigned short: JINX_STR_HELPER(sizeof(x)) "H", \
					int: JINX_STR_HELPER(sizeof(x)) "i", unsigned int: JINX_STR_HELPER(sizeof(x)) "I", \
					long: JINX_STR_HELPER(sizeof(x)) "l", unsigned long: JINX_STR_HELPER(sizeof(x)) "L", \
					long long: JINX_STR_HELPER(sizeof(x)) "q", unsigned long long: JINX_STR_HELPER(sizeof(x)) "Q", \
					float: JINX_STR_HELPER(sizeof(x)) "f", double: JINX_STR_HELPER(sizeof(x)) "d", \
					char*: JINX_STR_HELPER(sizeof(x)) "s", \
					void*: JINX_STR_HELPER(sizeof(x)) "P" \
				  )
