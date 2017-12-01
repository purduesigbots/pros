-include ./common.mk

WARNFLAGS += -Wpedantic

LIBNAME=libpros

ROOT=.
BINDIR=$(ROOT)/bin
SRCDIR=$(ROOT)/src
INCDIR=$(ROOT)/include

INCLUDE=-iquote$(INCDIR)

ASMSRC:=$(foreach asmext,$(ASMEXTS),$(call rwildcard, $(SRCDIR),*.$(asmext)))
ASMOBJ:=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(ASMSRC)))
CSRC:=$(foreach cext,$(CEXTS),$(call rwildcard, $(SRCDIR),*.$(cext)))
COBJ:=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(CSRC)))
CXXSRC:=$(foreach cxxext,$(CXXEXTS),$(call rwildcard, $(SRCDIR),*.$(cxxext)))
CXXOBJ:=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(CXXSRC)))

LIBRARIES=$(wildcard $(FWDIR)/*.a) -L$(FWDIR) -Wl,--start-group,-lv5rts,-lc,-lm,-lgcc,-lstdc++,-lsupc++,--end-group

ifndef OUTBIN
OUTNAME:=$(notdir $(realpath .))
endif
OUTBIN:=$(BINDIR)/$(OUTNAME).bin
OUTELF:=$(BINDIR)/$(OUTNAME).elf

.PHONY: all clean library template flash upload upload-legacy _force_look release develop

VERSION:=`git describe --abbrev=0`

all: clean $(OUTBIN)

quick: $(OUTBIN)

clean:
	@echo Cleaning project
	-$Drm -rf $(BINDIR)

$(OUTBIN): $(OUTELF)
	$(VV)mkdir -p $(dir $@)
	@echo -n "Creating $@ for $(DEVICE) "
	$(call test_output,$D$(OBJCOPY) $< -O binary $@,$(DONE_STRING))

$(OUTELF): $(sort $(ASMOBJ) $(COBJ) $(CXXOBJ))
	@echo -n "Linking project with $(ARCHIVE_TEXT_LIST) "
	$(call test_output,$D$(LD) $(LDFLAGS) $^ $(LIBRARIES) -o $@,$(OK_STRING))
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
