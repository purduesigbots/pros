#include "kapi.h"
#include "system/hot.h"


// stored only in cold
struct hot_table __HOT_TABLE = { 0 };
struct hot_table* const HOT_TABLE = &__HOT_TABLE;

__attribute__((section (".hot_magic"))) uint32_t MAGIC[] = {0x52616368, 0x8CEF7310};
uint32_t const volatile * const MAGIC_ADDR = MAGIC;

// The linker decides on these symbols in each section just as normal
// When linking in hot, these pointers work just like any other weak symbol
// Note: to get C++ style initialize and friends, we strip out cpp_initialize and friends so that linker
// regenerates that function with the call to the correct (user-written) C++ version
extern char const* _PROS_COMPILE_TIMESTAMP;
extern char const* _PROS_COMPILE_DIRECTORY;

void __libc_init_array();

// this expands to a bunch of:
// extern void autonomous();
#define FUNC(F) void F();
#include "system/user_functions/list.h"
#undef FUNC

__attribute__((section (".hot_init")))
void install_hot_table(struct hot_table* const tbl) {
  printf("Hot initializing\n");
  tbl->compile_timestamp = _PROS_COMPILE_TIMESTAMP;
  tbl->compile_directory = _PROS_COMPILE_DIRECTORY;

  // this expands to a bunch of:
  // tbl->functions.autonomous = autonomous;
  #define FUNC(F) tbl->functions.F = F;
  #include "system/user_functions/list.h"
  #undef FUNC

  // extern uint8_t __sbss_start[];
  // printf("%s %p %p\n", __FUNCTION__, (void*)__libc_init_array, (void*)__sbss_start);

  // Zero fill the sbss segment.
  asm volatile("    ldr     r0, =__sbss_start\n"
          "    ldr     r1, =__sbss_end\n"
          "    mov     r2, #0\n"
          "zero_loop1:\n"
          "    cmp     r0, r1\n"
          "    it      lt\n"
          "    strlt   r2, [r0], #4\n"
          "    blt     zero_loop1"
          :
          :
          : "r0", "r1", "r2");

    // Zero fill the bss segment.
  asm volatile("    ldr     r0, =__bss_start\n"
          "    ldr     r1, =__bss_end\n"
          "    mov     r2, #0\n"
          "zero_loop2:\n"
          "    cmp     r0, r1\n"
          "    it      lt\n"
          "    strlt   r2, [r0], #4\n"
          "    blt     zero_loop2");

  __libc_init_array();
}

void invoke_install_hot_table() {
  printf("%s %p %p %x %x\n", __FUNCTION__, (void*)install_hot_table, (void*)HOT_TABLE, MAGIC_ADDR[0], MAGIC_ADDR[1]);
  if(MAGIC_ADDR[0] == 0x52616368 && MAGIC_ADDR[1] == 0x8CEF7310) {
    install_hot_table(HOT_TABLE);
  } else {
    memset(HOT_TABLE, 0, sizeof(*HOT_TABLE));
  }
}
