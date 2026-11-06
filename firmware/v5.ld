/* This stack is used during initialization, but FreeRTOS tasks have their own
   stack allocated in BSS or Heap (kernel tasks in FreeRTOS .bss heap; user tasks
   in standard heap) */
_STACK_SIZE = DEFINED(_STACK_SIZE) ? _STACK_SIZE : 0x2000;

_HEAP_SIZE = DEFINED(_HEAP_SIZE) ? _HEAP_SIZE : 0x02E00000;  /* ~48 MB */

/* Define Memories in the system */
start_of_cold_mem = 0x03800000;
_COLD_MEM_SIZE = 0x04800000;
end_of_cold_mem = start_of_cold_mem + _COLD_MEM_SIZE;

start_of_hot_mem = 0x07800000;
_HOT_MEM_SIZE = 0x00800000;
end_of_hot_mem = start_of_hot_mem + _HOT_MEM_SIZE;

MEMORY
{
   /* user code  72M */
   COLD_MEMORY : ORIGIN = start_of_cold_mem, LENGTH = _COLD_MEM_SIZE /* Just under 19 MB */
   HEAP : ORIGIN = 0x04A00000, LENGTH = _HEAP_SIZE
   HOT_MEMORY : ORIGIN = start_of_hot_mem, LENGTH = _HOT_MEM_SIZE  /* Just over 8 MB */
}

REGION_ALIAS("RAM", COLD_MEMORY);

ENTRY(vexStartup)
