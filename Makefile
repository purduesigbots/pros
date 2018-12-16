CEXTS:=c
ASMEXTS:=s S
CXXEXTS:=cpp c++ cc

WARNFLAGS=-Wall -Wpedantic
DEPFLAGS=-M -MT $$@ -MMD -MP -MF $(DEPDIR)/$$*.tmp.mk

LIBNAME=libpros
LIBAR=$(BINDIR)/$(LIBNAME).a

ROOT=.
FWDIR:=$(ROOT)/firmware
BINDIR=$(ROOT)/bin
SRCDIR=$(ROOT)/src
INCDIR=$(ROOT)/include
DEPDIR=$(ROOT)/dep

.DEFAULT_GOAL:=quick

include ./common.mk

EXCLUDE_SRCDIRS=$(SRCDIR)/tests
EXCLUDE_FROM_LIB=$(SRCDIR)/opcontrol.c $(SRCDIR)/init.c $(SRCDIR)/auto.c
LIBV5RTS_EXTRACTION_DIR=$(BINDIR)/libv5rts

TEMPLATE_DIR=$(ROOT)/template
TEMPLATE_FILES=$(ROOT)/common.mk $(FWDIR)/cortex.ld $(FWDIR)/STM32F10x.ld $(FWDIR)/uniflash.jar $(INCDIR)/API.h $(INCDIR)/main.h $(SRCDIR)/opcontrol.c $(SRCDIR)/init.c $(SRCDIR)/auto.c

INCLUDE=-isystem$(INCDIR)

ASMSRC=$(foreach asmext,$(ASMEXTS),$(call rwildcard, $(SRCDIR),*.$(asmext), $1))
ASMOBJ=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(call ASMSRC,$1)))
CSRC=$(foreach cext,$(CEXTS),$(call rwildcard, $(SRCDIR),*.$(cext), $1))
COBJ=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(call CSRC,$1)))
CMKDEPS=$(addprefix $(DEPDIR)/,$(patsubst $(SRCDIR)/%.c,%.mk,$(call CSRC,$1)))
CXXSRC=$(foreach cxxext,$(CXXEXTS),$(call rwildcard, $(SRCDIR),*.$(cxxext), $1))
CXXOBJ=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(call CXXSRC,$1)))
CXXMKDEPS=$(addprefix $(DEPDIR)/,$(patsubst $(SRCDIR)/%.cpp,%.mk,$(call CXXSRC,$1)))

GETALLOBJ=$(sort $(call ASMOBJ,$1) $(call COBJ,$1) $(call CXXOBJ,$1))
GETALLSRC=$(sort $(call ASMOBJ,$1) $(call COBJ,$1) $(call CXXOBJ,$1))

LIBRARIES=-Wl,--start-group $(wildcard $(FWDIR)/*.a) -lm -lgcc -Wl,--end-group
ARCHIVE_TEXT_LIST:=$(subst $(SPACE),$(COMMA),$(notdir $(basename $(wildcard $(FWDIR)/*.a))))

ifndef OUTBIN
OUTNAME:=output
endif
OUTBIN:=$(BINDIR)/$(OUTNAME).bin
OUTELF:=$(BINDIR)/$(OUTNAME).elf

.PHONY: all clean quick library clean-library template clean-template version

quick: $(OUTBIN)

all: version clean $(OUTBIN)

clean: clean-library
	@echo Cleaning project
	-$Drm -rf $(BINDIR)
	-$Drm -rf $(DEPDIR)

clean-library:
	@echo Cleaning libpros
	-$Drm -f $(LIBAR)

library: version clean-library $(LIBAR)

version: version.py
	$(VV)python version.py

move-template-files: clean-template library
	$(VV)mkdir -p $(TEMPLATE_DIR)
	@echo "Moving template files to $(TEMPLATE_DIR)"
	$Dcp --parents -r $(TEMPLATE_FILES) $(TEMPLATE_DIR)
	$(VV)mkdir -p $(TEMPLATE_DIR)/firmware
	$Dcp $(LIBAR) $(TEMPLATE_DIR)/firmware
	$Dcp $(ROOT)/template-Makefile $(TEMPLATE_DIR)/Makefile

template: move-template-files
	pros conduct create-template kernel $(shell cat $(ROOT)/version) $(ROOT)/template -u "firmware/$(LIBNAME).a" -u "include/API.h" -u "common.mk" -i "template.pros"
	cd $(ROOT)/template && zip -r ../kernel-template *

template3: version move-template-files
	@echo "Creating template"
	$Dprosv5 c create-template $(TEMPLATE_DIR) kernel $(shell cat $(ROOT)/version) --system "./**/*" --user "src/opcontrol.{c,cpp,cc}" --user "src/initialize.{cpp,c,cc}" --user "src/autonomous.{cpp,c,cc}" --user "include/main.{hpp,h,hh}" --user "Makefile" --target cortex --output bin/output.bin

clean-template:
	-$Drm -rf $(TEMPLATE_DIR)

# Builds the documentation
documentation:
	doxygen Doxyfile

$(LIBAR): $(call GETALLOBJ,$(EXCLUDE_SRCDIRS) $(EXCLUDE_FROM_LIB))
	@echo -n "Creating $@ "
	$(call test_output,$D$(AR) rcs $@.public $(addprefix $(LIBV5RTS_EXTRACTION_DIR)/, $(LIBV5RTS_OBJECTS)) $^,$(DONE_STRING))
	# @echo -n "Stripping non-public symbols "
	$(ARCHTUPLE)objcopy -S -g --strip-unneeded --keep-symbols public.txt $@.public $@

$(OUTBIN): $(OUTELF)
	$(VV)mkdir -p $(dir $@)
	@echo -n "Creating $@ for $(DEVICE) "
	$(call test_output,$D$(OBJCOPY) $< -O binary $@,$(DONE_STRING))

$(OUTELF): $(call GETALLOBJ,$(EXCLUDE_SRCDIRS))
	@echo -n "Linking project with $(ARCHIVE_TEXT_LIST) "
	$(call test_output,$D$(LD) $(LDFLAGS) $^ $(LIBRARIES) -o $@,$(OK_STRING))
	@echo Section sizes:
	-$(VV)$(SIZETOOL) $(SIZEFLAGS) $@ $(SIZES_SED) $(SIZES_NUMFMT)

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
	$(VV)mkdir -p $$(dir $$@) $$(dir $(DEPDIR)/$$*)
	$D$(CC) $(DEPFLAGS) $(INCLUDE) -iquote$(INCDITdR)/$$(dir $$*) $(CFLAGS) $(EXTRA_CFLAGS) $$<
	$$(call test_output,$D$(CC) -c $(INCLUDE) -iquote$(INCDITdR)/$$(dir $$*) $(CFLAGS) $(EXTRA_CFLAGS) -o $$@ $$<,$(OK_STRING))
	@mv -f $(DEPDIR)/$$*.tmp.mk $(DEPDIR)/$$*.mk
endef
$(foreach cext,$(CEXTS),$(eval $(call c_rule,$(cext))))

define cxx_rule
$(BINDIR)/%.$1.o: $(SRCDIR)/%.$1
	@echo -n "Compiling $$< "
	$(VV)mkdir -p $$(dir $$@) $$(dir $(DEPDIR)/$$*)
	$D$(CXX) $(DEPFLAGS) $(INCLUDE) -iquote$(INCDIR)/$$(dir $$*) $(CXXFLAGS) $(EXTRA_CXXFLAGS) $$<
	$$(call test_output,$D$(CXX) -c $(INCLUDE) -iquote$(INCDIR)/$$(dir $$*) $(CXXFLAGS) $(EXTRA_CXXFLAGS) -o $$@ $$<,$(OK_STRING))
	@mv -f $(DEPDIR)/$$*.tmp.mk $(DEPDIR)/$$*.mk
endef
$(foreach cxxext,$(CXXEXTS),$(eval $(call cxx_rule,$(cxxext))))

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

-include $(CMKDEPS)
-include $(CXXMKDEPS)
