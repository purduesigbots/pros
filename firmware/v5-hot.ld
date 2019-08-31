/*******************************************************************/
/*                                                                 */
/* This file is nearly the same as v5.ld, except that everything   */
/* goes in HOT_MEMORY and HOT user code doesn't need heap or stack */
/* (only the cold memory code - PROS does)                         */
/*                                                                 */
/*******************************************************************/

/* Define Memories in the system */
INCLUDE firmware/v5-common.ld

/* Make install_hot_table entry point so that linker can prune from there */
ENTRY(install_hot_table)

/* Define the sections, and where they are mapped in memory */
SECTIONS
{
/* Since install_hot_table is the entry point, it should be the first address
 * in memory, but this makes sure it happens explicitly and consistently with
 * cold linkage */
.hot_init : {
  KEEP (*(.hot_magic))
  KEEP (*(.hot_init))
} > HOT_MEMORY
.text : {
   KEEP (*(.vectors))
   /* boot data should be exactly 32 bytes long */
   *(.boot_data)
   . = 0x20;
   *(.boot)
   . = ALIGN(64);
   *(.freertos_vectors)
   *(.text)
   *(.text.*)
   *(.gnu.linkonce.t.*)
   *(.plt)
   *(.gnu_warning)
   *(.gcc_execpt_table)
   *(.glue_7)
   *(.glue_7t)
   *(.vfp11_veneer)
   *(.ARM.extab)
   *(.gnu.linkonce.armextab.*)
} > HOT_MEMORY

.init : {
   KEEP (*(.init))
} > HOT_MEMORY

.fini : {
   KEEP (*(.fini))
} > HOT_MEMORY

.rodata : {
   __rodata_start = .;
   *(.rodata)
   *(.rodata.*)
   *(.gnu.linkonce.r.*)
   __rodata_end = .;
} > HOT_MEMORY

.rodata1 : {
   __rodata1_start = .;
   *(.rodata1)
   *(.rodata1.*)
   __rodata1_end = .;
} > HOT_MEMORY

.sdata2 : {
   __sdata2_start = .;
   *(.sdata2)
   *(.sdata2.*)
   *(.gnu.linkonce.s2.*)
   __sdata2_end = .;
} > HOT_MEMORY

.sbss2 : {
   __sbss2_start = .;
   *(.sbss2)
   *(.sbss2.*)
   *(.gnu.linkonce.sb2.*)
   __sbss2_end = .;
} > HOT_MEMORY

.data : {
   __data_start = .;
   *(.data)
   *(.data.*)
   *(.gnu.linkonce.d.*)
   *(.jcr)
   *(.got)
   *(.got.plt)
   __data_end = .;
} > HOT_MEMORY

.data1 : {
   __data1_start = .;
   *(.data1)
   *(.data1.*)
   __data1_end = .;
} > HOT_MEMORY

.got : {
   *(.got)
} > HOT_MEMORY

.ctors : {
   __CTOR_LIST__ = .;
   ___CTORS_LIST___ = .;
   KEEP (*crtbegin.o(.ctors))
   KEEP (*(EXCLUDE_FILE(*crtend.o) .ctors))
   KEEP (*(SORT(.ctors.*)))
   KEEP (*(.ctors))
   __CTOR_END__ = .;
   ___CTORS_END___ = .;
} > HOT_MEMORY

.dtors : {
   __DTOR_LIST__ = .;
   ___DTORS_LIST___ = .;
   KEEP (*crtbegin.o(.dtors))
   KEEP (*(EXCLUDE_FILE(*crtend.o) .dtors))
   KEEP (*(SORT(.dtors.*)))
   KEEP (*(.dtors))
   __DTOR_END__ = .;
   ___DTORS_END___ = .;
} > HOT_MEMORY

.fixup : {
   __fixup_start = .;
   *(.fixup)
   __fixup_end = .;
} > HOT_MEMORY

.eh_frame : {
   *(.eh_frame)
} > HOT_MEMORY

.eh_framehdr : {
   __eh_framehdr_start = .;
   *(.eh_framehdr)
   __eh_framehdr_end = .;
} > HOT_MEMORY

.gcc_except_table : {
   *(.gcc_except_table)
} > HOT_MEMORY

.mmu_tbl (ALIGN(16384)) : {
   __mmu_tbl_start = .;
   *(.mmu_tbl)
   __mmu_tbl_end = .;
} > HOT_MEMORY

.ARM.exidx : {
   __exidx_start = .;
   *(.ARM.exidx*)
   *(.gnu.linkonce.armexidix.*.*)
   __exidx_end = .;
} > HOT_MEMORY

.preinit_array : {
   __preinit_array_start = .;
   KEEP (*(SORT(.preinit_array.*)))
   KEEP (*(.preinit_array))
   __preinit_array_end = .;
} > HOT_MEMORY

.init_array : {
   __init_array_start = .;
   KEEP (*(SORT(.init_array.*)))
   KEEP (*(.init_array))
   __init_array_end = .;
} > HOT_MEMORY

.fini_array : {
   __fini_array_start = .;
   KEEP (*(SORT(.fini_array.*)))
   KEEP (*(.fini_array))
   __fini_array_end = .;
} > HOT_MEMORY

.ARM.attributes : {
   __ARM.attributes_start = .;
   *(.ARM.attributes)
   __ARM.attributes_end = .;
} > HOT_MEMORY

.sdata : {
   __sdata_start = .;
   *(.sdata)
   *(.sdata.*)
   *(.gnu.linkonce.s.*)
   __sdata_end = .;
} > HOT_MEMORY

.sbss (NOLOAD) : {
   __sbss_start = .;
   *(.sbss)
   *(.sbss.*)
   *(.gnu.linkonce.sb.*)
   __sbss_end = .;
} > HOT_MEMORY

.tdata : {
   __tdata_start = .;
   *(.tdata)
   *(.tdata.*)
   *(.gnu.linkonce.td.*)
   __tdata_end = .;
} > HOT_MEMORY

.tbss : {
   __tbss_start = .;
   *(.tbss)
   *(.tbss.*)
   *(.gnu.linkonce.tb.*)
   __tbss_end = .;
} > HOT_MEMORY

.bss (NOLOAD) : {
   __bss_start = .;
   *(.bss)
   *(.bss.*)
   *(.gnu.linkonce.b.*)
   *(COMMON)
   __bss_end = .;
} > HOT_MEMORY

_SDA_BASE_ = __sdata_start + ((__sbss_end - __sdata_start) / 2 );

_SDA2_BASE_ = __sdata2_start + ((__sbss2_end - __sdata2_start) / 2 );

/* HOT memory doesn't need a stack or heap */
_end = .;
}
