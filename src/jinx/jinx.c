#include <sys/fcntl.h>
#include "common/gid.h"
#include "jinx/cwpack.h"
#include "kapi.h"

// Number of bytes reserved for messagepack overhead (3 bytes) and for the JINX message type (1)
#define SCHEMA_RESERVED_BYTES 4
// Max global ID value
#define JINX_GID_MAX 0x1000
// Timeout values (ms)
#define DATA_TIMEOUT 20
#define SCHEMA_TIMEOUT 2000
#define VARIABLE_TIMEOUT 50

// Schema metadata
struct jinx_variable {
	uint16_t id;
	uint16_t value_size;

	uint8_t* schema;
	size_t schema_size;
};

// Jinx Data metadata
struct metadata_info {
	uint8_t size;
	uint8_t time_offset;
	uint16_t id;
};

// Buffers and related variables for data and schemas
static uint8_t data_buffer[1024];
static uint16_t data_ptr = 0;
static uint8_t schema_buffer[1024];
static uint16_t schema_ptr = 0;
static uint16_t schema_ctr = 0;
static uint32_t schema_bitmap[gid_size_to_words(JINX_GID_MAX)];

// Pointers and variables for variables jinx is tracking
static jinx_track_s_t* track_buff = NULL;
static jinx_track_s_t* track_buff_tail = NULL;

static uint32_t gid_bitmap[gid_size_to_words(JINX_GID_MAX)];
static struct gid_metadata jinx_gid_metadata = {.bitmap = gid_bitmap,
                                                .max = JINX_GID_MAX,
                                                .reserved = 1,  // 0 is "invalid"
                                                .bitmap_size = gid_size_to_words(JINX_GID_MAX)};

// Jinx file number for writing to serial port
static int jinx_file_no = -1;

// Mutex variables
static static_sem_s_t mtx_buffer;
static mutex_t mutex;

// Task initialization variables
static task_stack_t jinx_task_stack[TASK_STACK_DEPTH_DEFAULT];
static static_task_s_t jinx_task_buf;
static task_t jinx_daemon_task;

// Send the data from the data buffer over serial communication
static void data_flush() {
	lcd_print(0, "%x %u", *(uint32_t*)(data_buffer + 1), *(uint32_t*)(data_buffer + 1));
	write(jinx_file_no, data_buffer, data_ptr);
	// Set first 4 bytes to current timestamp
	uint32_t* time = (uint32_t*)(data_buffer + 1);
	*time = millis();
	data_ptr = 5;
	task_notify_ext(jinx_daemon_task, 2, E_NOTIFY_ACTION_BITS, NULL);
}

// Send the schemas in the buffer over serial communication
static void schema_flush() {
	// 0xDE is MessagePack code for a map whose length is 16 bits (Map 16)
	// See https://github.com/msgpack/msgpack/blob/master/spec.md#map-format-family
	// for more details
	lcd_print(1, "%d %d", schema_ptr, millis());
	if (schema_ctr < 16) {
		schema_buffer[SCHEMA_RESERVED_BYTES - 1] = 0x80 | schema_ctr;
		schema_buffer[SCHEMA_RESERVED_BYTES - 2] = 'S';
		write(jinx_file_no, schema_buffer + 2, schema_ptr - 2);
	} else {
		schema_buffer[0] = 'S';
		schema_buffer[1] = 0xDE;
		uint16_t* len = (uint16_t*)(schema_buffer + 2);
		*len = schema_ctr;
		write(jinx_file_no, schema_buffer, schema_ptr);
	}
	memset(schema_bitmap, 0, sizeof(schema_bitmap));
	schema_ptr = SCHEMA_RESERVED_BYTES;
	schema_ctr = 0;
	task_notify_ext(jinx_daemon_task, 1, E_NOTIFY_ACTION_BITS, NULL);
}

// Task to flush buffers. Current timeouts:
// Data buffer: 20 ms
// Schema buffer: 2 s
// Variable pushing: 100 ms
void jinx_task() {
	uint32_t last_schema_flush = millis();
	uint32_t last_data_flush = millis();
	uint32_t last_variable_push = millis();
	uint32_t next_wake = DATA_TIMEOUT;
	while (true) {
		register uint32_t n = task_notify_take(true, next_wake);
		lcd_print(2, "%x %u", n, millis());
		if (n & 1) {
			last_schema_flush = millis();
		}
		if (n & 2) {
			last_data_flush = millis();
		}
		uint32_t since_last_schema_flush = millis() - last_schema_flush;
		uint32_t since_last_data_flush = millis() - last_data_flush;
		uint32_t since_last_variable_push = millis() - last_variable_push;
		next_wake = DATA_TIMEOUT;
		if (since_last_schema_flush >= SCHEMA_TIMEOUT) {
			schema_flush();
		} else {
			register uint32_t next_schema = SCHEMA_TIMEOUT - since_last_schema_flush;
			next_wake = next_schema < next_wake ? next_schema : next_wake;
		}
		if (since_last_data_flush >= DATA_TIMEOUT) {
			data_flush();
		} else if (DATA_TIMEOUT - since_last_data_flush <= next_wake) {
			register uint32_t next_data = DATA_TIMEOUT - since_last_data_flush;
			next_wake = next_data < next_wake ? next_data : next_wake;
		}
		if (since_last_variable_push >= VARIABLE_TIMEOUT) {
			for (jinx_track_s_t* curr = track_buff; curr != NULL; curr = curr->next) {
				if(curr->contains_func) {
					void* buff = kmalloc(((struct jinx_variable*)(curr->jinx_var))->value_size);
					if(curr->data_func.fill_func_data.fill_func(buff,curr->data_func.fill_func_data.args)) {
						jinx_push_variable(curr->jinx_var,buff);
					}
					kfree(buff);
				}
				else {
					jinx_push_variable(curr->jinx_var, curr->data_func.data_ptr);
				}
			}
			last_variable_push = millis();
		} else {
			register uint32_t next_variable = VARIABLE_TIMEOUT - since_last_variable_push;
			next_wake = next_variable < next_wake ? next_variable : next_wake;
		}
	}
}
// adapted from CWpack goodies for a dynamic memory context.
// See CWPACK license at include/jinx/CWPACK.license
static int handle_cwpack_buffer_overflow(struct cw_pack_context* ctx, unsigned long add) {
	unsigned long contains = (unsigned long)(ctx->current - ctx->start);
	unsigned long tot_len = contains + add;
	unsigned long buffer_length = (unsigned long)(ctx->end - ctx->start);
	while (buffer_length < tot_len) buffer_length = 2 * buffer_length;

	void* buf = kmalloc(buffer_length);
	if (!buf) {
		return CWP_RC_BUFFER_OVERFLOW;
	}
	memcpy(buf, ctx->start, contains);
	ctx->start = buf;
	ctx->current = ctx->start + contains;
	ctx->end = ctx->start + buffer_length;
	return CWP_RC_OK;
}

void jinx_init(void) {
	gid_init(&jinx_gid_metadata);
	mutex = mutex_create_static(&mtx_buffer);
	jinx_file_no = open("/ser/jinx", O_CREAT);
	jinx_daemon_task = task_create_static(jinx_task, NULL, TASK_PRIORITY_MIN + 2, TASK_STACK_DEPTH_DEFAULT,
	                                      "JINX Daemon (PROS)", jinx_task_stack, &jinx_task_buf);
	fdctl(jinx_file_no, 10, NULL);

	schema_buffer[0] = 'S';
	schema_buffer[1] = 0xDE;  // msgpack map
	schema_ptr = SCHEMA_RESERVED_BYTES;

	data_buffer[0] = 'D';
	uint32_t* time = (uint32_t*)(data_buffer + 1);
	*time = millis();
	data_ptr = 5;
}

jinx_var_t jinx_create_variable(uint16_t value_size, char* value_name, char* value_fmt,
                                jinx_var_extra_args_t* extra_args) {
	static struct cw_pack_context cwp_ctx;
	cw_pack_context_init(&cwp_ctx, kmalloc(0x20), 0x20, &handle_cwpack_buffer_overflow);

	uint16_t gid = (uint16_t)gid_alloc(&jinx_gid_metadata);
	if (!gid) {
		return 0;
	}
	struct jinx_variable* var = kmalloc(sizeof(*var));
	var->id = gid;
	var->value_size = value_size;

	cw_pack_unsigned(&cwp_ctx, gid);
	cw_pack_map_size(&cwp_ctx, extra_args == NULL ? 2 : 4);
	cw_pack_str(&cwp_ctx, "n", 1);
	cw_pack_str(&cwp_ctx, value_name, strlen(value_name));
	cw_pack_str(&cwp_ctx, "s", 1);
	cw_pack_str(&cwp_ctx, value_fmt, strlen(value_fmt));
	if (extra_args != NULL) {
		cw_pack_str(&cwp_ctx, "e", 1);
		cw_pack_array_size(&cwp_ctx, extra_args->n_subelements);
		for (int i = 0; i < extra_args->n_subelements; i++) {
			cw_pack_str(&cwp_ctx, (extra_args->subelements)[i], strlen((extra_args->subelements)[i]));
		}
		cw_pack_str(&cwp_ctx, "m", 1);
		cw_pack_unsigned(&cwp_ctx, (uint64_t)(extra_args->modifiable));
	}

	var->schema = cwp_ctx.start;
	var->schema_size = cwp_ctx.current - cwp_ctx.start;

	// kprint_hex(var->schema, var->schema_size);

	return (jinx_var_t)var;
}

void jinx_push_variable(jinx_var_t jinx_value, void* ptr) {
	if (!mutex_take(mutex, TIMEOUT_MAX)) {
		return;
	}
	const struct jinx_variable* var = (struct jinx_variable*)jinx_value;
	struct metadata_info metadata = {.size = var->value_size, .id = var->id};
	metadata.time_offset = (uint8_t)(millis() - *(uint32_t*)(data_buffer + 1));

	// Check if the data to copy is too large. If it is, flush the buffer first
	if (sizeof(metadata) + metadata.size > 1024 - data_ptr) {
		data_flush();
	}
	// Copy the metadata to the buffer
	memcpy(data_buffer + data_ptr, &metadata, sizeof(metadata));
	data_ptr += sizeof(metadata);

	memcpy(data_buffer + data_ptr, ptr, metadata.size);
	data_ptr += metadata.size;
	if (!bmp_check(schema_bitmap, metadata.id)) {
		// Check if the schema is too large. If it is, flush the buffer first
		if (var->schema_size > 1024 - schema_ptr) {
			schema_flush();
		}
		memcpy(schema_buffer + schema_ptr, var->schema, var->schema_size);
		bmp_set(schema_bitmap, metadata.id);
		schema_ptr += var->schema_size;
		schema_ctr++;
	}
	mutex_give(mutex);
}

jinx_track_s_t* jinx_track_variable(jinx_var_t jinx_value, void* ptr) {
	jinx_track_s_t* new_var = kmalloc(sizeof(*new_var));
	new_var->jinx_var = jinx_value;
	new_var->data_func.data_ptr = ptr;
	new_var->contains_func = false;
	new_var->next = NULL;
	new_var->prev = track_buff_tail;
	if(track_buff_tail != NULL) {
		track_buff_tail->next = new_var;
	}
	track_buff_tail = new_var;
	if (track_buff == NULL) {
		track_buff = new_var;
	}
	return new_var;
}

jinx_track_s_t* jinx_track_variable_fptr(jinx_var_t jinx_value, bool (*fill_func)(void* buff, void* args),void* args_to_pass) {
	jinx_track_s_t* new_var = kmalloc(sizeof(*new_var));
	new_var->jinx_var = jinx_value;
	new_var->contains_func = true;
	new_var->data_func.fill_func_data.fill_func = fill_func;
	new_var->data_func.fill_func_data.args = args_to_pass;
	new_var->next = NULL;
	new_var->prev = track_buff_tail;
	if(track_buff_tail != NULL) {
		track_buff_tail->next = new_var;
	}
	track_buff_tail = new_var;
	if (track_buff == NULL) {
		track_buff = new_var;
	}
	return new_var;
}

void jinx_untrack_variable(jinx_track_s_t* track_var) {
	for (jinx_track_s_t* curr = track_buff; curr != NULL; curr = curr->next) {
		if (curr == track_var) {
			if (curr->prev == NULL) {
				track_buff = curr->next;
			} else {
				(curr->prev)->next = curr->next;
			}
			if (curr->next == NULL) {
				track_buff_tail = curr->prev;
			} else {
				(curr->next)->prev = curr->prev;
			}
			if(curr->contains_func)
				kfree(curr->data_func.fill_func_data.args);
			kfree(curr->jinx_var);
			kfree(curr);
		}
	}
}
