#include "system/hot.h"
#include "kapi.h"
#include "v5_api.h"

// stored only in cold
struct hot_table __HOT_TABLE = {0};
struct hot_table* const HOT_TABLE = &__HOT_TABLE;

#define MAGIC0 0x52616368
#define MAGIC1 0x8CEF7310

extern void set_get_timestamp_int_func(const int (*func)(void));
static const int get_timestamp_int(void);

__attribute__((section(".hot_magic"))) uint32_t MAGIC[] = {MAGIC0, MAGIC1};
uint32_t const volatile* const MAGIC_ADDR = MAGIC;

// The linker decides on these symbols in each section just as normal
// When linking in hot, these pointers work just like any other weak symbol
// Note: to get C++ style initialize and friends, we strip out cpp_initialize and friends so that linker
// regenerates that function with the call to the correct (user-written) C++ version
extern char const* _PROS_COMPILE_TIMESTAMP;
extern char const* _PROS_COMPILE_DIRECTORY;
extern const int   _PROS_COMPILE_TIMESTAMP_INT;

extern unsigned __exidx_start;
extern unsigned __exidx_end;

// this expands to a bunch of:
// extern void autonomous();
#define FUNC(F) void F();
#include "system/user_functions/list.h"
#undef FUNC

__attribute__((section(".hot_init"))) void install_hot_table(struct hot_table* const tbl) {
	// printf("Hot initializing\n");
	tbl->compile_timestamp = _PROS_COMPILE_TIMESTAMP;
	tbl->compile_directory = _PROS_COMPILE_DIRECTORY;
	tbl->__exidx_start = &__exidx_start;
	tbl->__exidx_end = &__exidx_end;

// this expands to a bunch of:
// tbl->functions.autonomous = autonomous;
#define FUNC(F) tbl->functions.F = F;
#include "system/user_functions/list.h"
#undef FUNC

	// all of these weak symbols are given to us by the linker
	// These values should come from the hot region, since that's where this
	// function is linked
	extern __attribute__((weak)) uint8_t* __sbss_start[];
	extern __attribute__((weak)) uint8_t* __sbss_end[];
	memset(__sbss_start, 0, (size_t)__sbss_end - (size_t)__sbss_start);

	extern __attribute__((weak)) uint8_t* __bss_start[];
	extern __attribute__((weak)) uint8_t* __bss_end[];
	memset(__bss_start, 0, (size_t)__bss_end - (size_t)__bss_start);

	extern __attribute__((weak)) void (*const __preinit_array_start[])(void);
	extern __attribute__((weak)) void (*const __preinit_array_end[])(void);
	for (void (*const* ctor)() = __preinit_array_start; ctor < __preinit_array_end; ctor++) {
		(*ctor)();
	}

	extern __attribute__((weak)) void (*const __init_array_start[])(void);
	extern __attribute__((weak)) void (*const __init_array_end[])(void);
	for (void (*const* ctor)() = __init_array_start; ctor < __init_array_end; ctor++) {
		(*ctor)();
	}

	// Set the function pointer in newlib_stubs so that it can fetch the
	// timestamp in the hot package. 
	set_get_timestamp_int_func(get_timestamp_int);
}

// this function really exists on the cold section! Called by pros_init
// this does the check if we're running with hot/cold and invokes the hot table
// installer (install_hot_table) located in hot memory
void invoke_install_hot_table() {
	// install_hot_table is at 0x07800008
	// MAGIC_ADDR is at 0x0780000
	// printf("%s %p %p %x %x\n", __FUNCTION__, (void*)install_hot_table, (void*)HOT_TABLE, MAGIC_ADDR[0], MAGIC_ADDR[1]);
	if (vexSystemLinkAddrGet() == (uint32_t)0x03800000 && MAGIC_ADDR[0] == MAGIC0 && MAGIC_ADDR[1] == MAGIC1) {
		install_hot_table(HOT_TABLE);
	} else {
		memset(HOT_TABLE, 0, sizeof(*HOT_TABLE));
	}
}

// This is a callback function used by newlib to get the unix timestamp
// newlib cannot access any symbols in the hot package, so we have the hot
// package pass a function pointer to this function. Newlib then uses that
// function pointer. 
static const int get_timestamp_int(void) {
	return _PROS_COMPILE_TIMESTAMP_INT;
}
