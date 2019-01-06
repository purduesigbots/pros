CEXTS:=c
ASMEXTS:=s S
CXXEXTS:=cpp c++ cc

WARNFLAGS=-Wall -Wpedantic

LIBNAME=libpros
LIBAR=$(BINDIR)/$(LIBNAME).a

ROOT=.
FWDIR:=$(ROOT)/firmware
BINDIR=$(ROOT)/bin
SRCDIR=$(ROOT)/src
INCDIR=$(ROOT)/include
IFI_INC=$(FWDIR)/libv5rts/sdk/vexv5/include
SDK=$(FWDIR)/libv5rts/sdk/vexv5/libv5rts.a

.DEFAULT_GOAL:=quick

-include ./common.mk

EXCLUDE_SRCDIRS=$(SRCDIR)/tests
EXCLUDE_FROM_LIB=$(SRCDIR)/opcontrol.cpp $(SRCDIR)/initialize.cpp $(SRCDIR)/autonomous.cpp
LIBV5RTS_EXTRACTION_DIR=$(BINDIR)/libv5rts

TEMPLATE_DIR=$(ROOT)/template
TEMPLATE_FILES=$(ROOT)/common.mk $(FWDIR)/v5.ld $(INCDIR)/api.h $(INCDIR)/main.h $(INCDIR)/pros/*.* $(SRCDIR)/opcontrol.cpp $(SRCDIR)/initialize.cpp $(SRCDIR)/autonomous.cpp $(INCDIR)/display

INCLUDE=-iquote$(INCDIR) -iquote$(IFI_INC)

ASMSRC=$(foreach asmext,$(ASMEXTS),$(call rwildcard, $(SRCDIR),*.$(asmext), $1))
ASMOBJ=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(call ASMSRC,$1)))
CSRC=$(foreach cext,$(CEXTS),$(call rwildcard, $(SRCDIR),*.$(cext), $1))
COBJ=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(call CSRC,$1)))
CXXSRC=$(foreach cxxext,$(CXXEXTS),$(call rwildcard, $(SRCDIR),*.$(cxxext), $1))
CXXOBJ=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(call CXXSRC,$1)))

GETALLOBJ=$(sort $(call ASMOBJ,$1) $(call COBJ,$1) $(call CXXOBJ,$1))

LIBRARIES=-L$(FWDIR) -Wl,--start-group $(wildcard $(FWDIR)/*.a) $(SDK) -lc -lm -lgcc -lstdc++ -lsupc++ -Wl,--end-group
ARCHIVE_TEXT_LIST:=$(subst $(SPACE),$(COMMA),$(notdir $(basename $(wildcard $(FWDIR)/*.a) $(SDK))))

ifndef OUTBIN
OUTNAME:=output
endif
OUTBIN:=$(BINDIR)/$(OUTNAME).bin
OUTELF:=$(BINDIR)/$(OUTNAME).elf
LDTIMEOBJ:=$(BINDIR)/_pros_ld_timestamp.o

.PHONY: all clean quick library clean-library template clean-template version fix-libv5rts

quick: $(OUTBIN)

all: clean $(OUTBIN)

clean: clean-library
	@echo Cleaning project
	-$Drm -rf $(BINDIR)

clean-library:
	@echo Cleaning libpros
	-$Drm -f $(LIBAR)

library: version clean-library $(LIBAR)

version: version.py
	$(VV)python version.py

template: clean-template library
	$(VV)mkdir -p $(TEMPLATE_DIR)
	@echo "Moving template files to $(TEMPLATE_DIR)"
	$Dcp --parents -r $(TEMPLATE_FILES) $(TEMPLATE_DIR)
	$(VV)mkdir -p $(TEMPLATE_DIR)/firmware
	$Dcp $(LIBAR) $(TEMPLATE_DIR)/firmware
	$Dcp $(ROOT)/template-Makefile $(TEMPLATE_DIR)/Makefile
	@echo "Creating template"
	$Dprosv5 c create-template $(TEMPLATE_DIR) kernel $(shell cat $(ROOT)/version) --system "./**/*" --user "src/opcontrol.{c,cpp,cc}" --user "src/initialize.{cpp,c,cc}" --user "src/autonomous.{cpp,c,cc}" --user "include/main.{hpp,h,hh}" --user "Makefile" --target v5 --output bin/output.bin

clean-template:
	-$Drm -rf $(TEMPLATE_DIR)

fix-libv5rts:
	@echo -n "Stripping unwanted symbols from libv5rts.a "
	$(call test_output,$D$(STRIP) $(SDK) @libv5rts-strip-options.txt,$(DONE_STRING))

$(LIBAR): fix-libv5rts $(call GETALLOBJ,$(EXCLUDE_SRCDIRS) $(EXCLUDE_FROM_LIB))
	$(VV)mkdir -p $(LIBV5RTS_EXTRACTION_DIR)
	@echo -n "Extracting libv5rts "
	$(call test_output,$Dcd $(LIBV5RTS_EXTRACTION_DIR) && $(AR) x ../../$(SDK),$(DONE_STRING))
	$(eval LIBV5RTS_OBJECTS := $(shell $(AR) t $(SDK)))
	@echo -n "Creating $@ "
	$(call test_output,$D$(AR) rcs $@ $(addprefix $(LIBV5RTS_EXTRACTION_DIR)/, $(LIBV5RTS_OBJECTS)) $(filter-out fix-libv5rts,$^),$(DONE_STRING))
# @echo -n "Stripping non-public symbols "
# $(call test_output,$D$(OBJCOPY) -S -D -g --strip-unneeded --keep-symbols public_symbols.txt $@,$(DONE_STRING))

$(OUTBIN): $(OUTELF)
	$(VV)mkdir -p $(dir $@)
	@echo -n "Creating $@ for $(DEVICE) "
	$(call test_output,$D$(OBJCOPY) $< -O binary $@,$(DONE_STRING))

$(OUTELF): fix-libv5rts $(call GETALLOBJ,$(EXCLUDE_SRCDIRS))
	$(call _pros_ld_timestamp)
	@echo -n "Linking project with $(ARCHIVE_TEXT_LIST) "
	$(call test_output,$D$(LD) $(LDFLAGS) $(filter-out fix-libv5rts,$^) $(LDTIMEOBJ) $(LIBRARIES) -o $@,$(OK_STRING))
	@echo Section sizes:
	-$(VV)$(SIZETOOL) $(SIZEFLAGS) $@ $(SIZES_SED) $(SIZES_NUMFMT)

define asm_rule
$(BINDIR)/%.$1.o: $(SRCDIR)/%.$1
	$(VV)mkdir -p $$(dir $$@)
	@echo -n "Compiling $$< "
	$(VV)mkdir -p $$(dir $$@)
	$$(call test_output,$D$(AS) -c $(ASMFLAGS) -o $$@ $$<,$(OK_STRING))
endef
$(foreach asmext,$(ASMEXTS),$(eval $(call asm_rule,$(asmext))))

define c_rule
$(BINDIR)/%.$1.o: $(SRCDIR)/%.$1
	$(VV)mkdir -p $$(dir $$@)
	@echo -n "Compiling $$< "
	$(VV)mkdir -p $$(dir $$@)
	$$(call test_output,$D$(CC) -c $(INCLUDE) -iquote$(INCDIR)/$$(dir $$*) $(CFLAGS) $(EXTRA_CFLAGS) -o $$@ $$<,$(OK_STRING))
endef
$(foreach cext,$(CEXTS),$(eval $(call c_rule,$(cext))))

define cxx_rule
$(BINDIR)/%.$1.o: $(SRCDIR)/%.$1
	$(VV)mkdir -p $$(dir $$@)
	@echo -n "Compiling $$< "
	$(VV)mkdir -p $$(dir $$@)
	$$(call test_output,$D$(CXX) -c $(INCLUDE) -iquote$(INCDIR)/$$(dir $$*) $(CXXFLAGS) $(EXTRA_CXXFLAGS) -o $$@ $$<,$(OK_STRING))
endef
$(foreach cxxext,$(CXXEXTS),$(eval $(call cxx_rule,$(cxxext))))

define _pros_ld_timestamp
@echo -n "Adding timestamp "
@# Pipe a line of code defining _PROS_COMPILE_TOOLSTAMP and _PROS_COMPILE_DIRECTORY into GCC,
@# which allows compilation from stdin. We define _PROS_COMPILE_DIRECTORY using a command line-defined macro
@# which is the pwd | sed ... | tail bit, which will grab the last 3 segments of the path and truncate it 23 characters
$(call test_output, @echo 'char const * const _PROS_COMPILE_TIMESTAMP = __DATE__ " " __TIME__; char const * const _PROS_COMPILE_DIRECTORY = PCD;' | $(CC) -c -x c $(CFLAGS) $(EXTRA_CFLAGS) -DPCD="\"`pwd | tail -c 23`\"" -o $(LDTIMEOBJ) -,$(OK_STRING))
endef
