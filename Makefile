-include ./common.mk

WARNFLAGS += -Wpedantic

LIBNAME=libpros
LIBAR=$(BINDIR)/$(LIBNAME).a

ROOT=.
BINDIR=$(ROOT)/bin
SRCDIR=$(ROOT)/src
INCDIR=$(ROOT)/include

EXCLUDE_SRCDIRS=$(SRCDIR)/tests
TEMPLATE_DIR=$(SRCDIR)/template
LIBV5RTS_EXTRACTION_DIR=$(BINDIR)/libv5rts

INCLUDE=-iquote$(INCDIR)

ASMSRC=$(foreach asmext,$(ASMEXTS),$(call rwildcard, $(SRCDIR),*.$(asmext), $1))
ASMOBJ=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(call ASMSRC,$1)))
CSRC=$(foreach cext,$(CEXTS),$(call rwildcard, $(SRCDIR),*.$(cext), $1))
COBJ=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(CSRC)))
CXXSRC=$(foreach cxxext,$(CXXEXTS),$(call rwildcard, $(SRCDIR),*.$(cxxext), $1))
CXXOBJ=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(CXXSRC)))

GETALLOBJ=$(sort $(call ASMOBJ,$1) $(call COBJ,$1) $(call CXXOBJ,$1))

LIBRARIES=$(wildcard $(FWDIR)/*.a) -L$(FWDIR) -Wl,--start-group,-lv5rts,-lc,-lm,-lgcc,-lstdc++,-lsupc++,--end-group
ARCHIVE_TEXT_LIST:=$(subst $(SPACE),$(COMMA),$(notdir $(basename $(wildcard $(FWDIR)/*.a))))

ifndef OUTBIN
OUTNAME:=output
endif
OUTBIN:=$(BINDIR)/$(OUTNAME).bin
OUTELF:=$(BINDIR)/$(OUTNAME).elf
LDTIMEOBJ:=$(BINDIR)/_pros_ld_timestamp.o

.PHONY: all clean quick library

VERSION:=`git describe --abbrev=0`

quick: $(OUTBIN)

all: clean $(OUTBIN)

clean: clean-library
	@echo Cleaning project
	-$Drm -rf $(BINDIR)

clean-library:
	@echo Cleaning libpros
	-$Drm -f $(LIBAR)

library: clean-library $(LIBAR)


$(LIBAR): $(call GETALLOBJ,$(EXCLUDE_SRCDIRS) $(TEMPLATE_DIR))
	$(VV)mkdir -p $(LIBV5RTS_EXTRACTION_DIR)
	@echo -n "Extracting libv5rts "
	$Dcd $(LIBV5RTS_EXTRACTION_DIR) && $(AR) x ../../$(FWDIR)/libv5rts.a
	@echo -n "Creating $@ "
	$(call test_output,$D$(AR) rcs $@ $^ $(LIBV5RTS_EXTRACTION_DIR)/*,$(DONE_STRING))
	@echo -n "Stripping non-public symbols "
	$(call test_output,$D$(OBJCOPY) -S -D -g --strip-unneeded --keep-symbols public_symbols.txt $@,$(DONE_STRING))

$(OUTBIN): $(OUTELF)
	$(VV)mkdir -p $(dir $@)
	@echo -n "Creating $@ for $(DEVICE) "
	$(call test_output,$D$(OBJCOPY) $< -O binary $@,$(DONE_STRING))

$(OUTELF): $(call GETALLOBJ,$(EXCLUDE_SRCDIRS))
	$(call _pros_ld_timestamp)
	@echo -n "Linking project with $(ARCHIVE_TEXT_LIST) "
	$(call test_output,$D$(LD) $(LDFLAGS) $^ $(LDTIMEOBJ) $(LIBRARIES) -o $@,$(OK_STRING))
	@echo Section sizes:
	-$(VV)$(SIZETOOL) $(SIZEFLAGS) $@ | sed --expression='s/  dec/total/' | numfmt --field=-4 --header $(NUMFMTFLAGS)

define asm_rule
$(BINDIR)/%.$1.o: $(SRCDIR)/%.$1
	@echo -n "Compiling $$< "
	$(VV)mkdir -p $$(dir $$@)
	$$(call test_output,$D$(AS) -c $(ASMFLAGS) -o $$@ $$<,$(OK_STRING))
endef
$(foreach asmext,$(ASMEXTS),$(eval $(call asm_rule,$(asmext))))

define c_rule
$(BINDIR)/%.$1.o: $(SRCDIR)/%.$1
	@echo -n "Compiling $$< "
	$(VV)mkdir -p $$(dir $$@)
	$$(call test_output,$D$(CC) -c $(INCLUDE) -iquote$(INCDIR)/$$(dir $$*) $(CFLAGS) $(EXTRA_CFLAGS) -o $$@ $$<,$(OK_STRING))
endef
$(foreach cext,$(CEXTS),$(eval $(call c_rule,$(cext))))

define cxx_rule
$(BINDIR)/%.$1.o: $(SRCDIR)/%.$1
	@echo -n "Compiling $$< "
	$(VV)mkdir -p $$(dir $$@)
	$$(call test_output,$D$(CXX) -c $(INCLUDE) -iquote$(INCDIR)/$$(dir $$*) $(CXXFLAGS) $(EXTRA_CXXFLAGS) -o $$@ $$<,$(OK_STRING))
endef
$(foreach cxxext,$(CXXEXTS),$(eval $(call cxx_rule,$(cxxext))))

define _pros_ld_timestamp
@echo -n "Adding timestamp "
@# Pipe a line of code defining _PROS_COMPILE_TOOLSTAMP and _PROS_COMPILE_DIRECTORY into GCC,
@# which allows compilation from stdin. We cefine _PROS_COMPILE_DIRECTORY using a command line-defined macro
@# which is the pwd | sed ... | tail bit, which will grab the last 3 segments of the path and truncate it 23 characters
$(call test_output, @echo 'char const * const _PROS_COMPILE_TIMESTAMP = __DATE__ " " __TIME__; char const * const _PROS_COMPILE_DIRECTORY = PCD;' | $(CC) -c -x c $(CFLAGS) $(EXTRA_CFLAGS) -DPCD="\"`pwd | sed -n 's/.*\/\(.*\/.*\/.*\)/\1/p' | tail -c 23`\"" -o $(LDTIMEOBJ) -,$(OK_STRING))
endef