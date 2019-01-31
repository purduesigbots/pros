################################################################################
######################### User configurable parameters #########################
# filename extensions
CEXTS:=c
ASMEXTS:=s S
CXXEXTS:=cpp c++ cc

# probably shouldn't modify these, but you may need them below
ROOT=.
FWDIR:=$(ROOT)/firmware
BINDIR=$(ROOT)/bin
SRCDIR=$(ROOT)/src
INCDIR=$(ROOT)/include
EXTRA_INCDIR=$(FWDIR)/libv5rts/sdk/vexv5/include

# Directories to be excluded from all builds
EXCLUDE_SRCDIRS+=$(SRCDIR)/tests

WARNFLAGS+=-Wall -Wpedantic
EXTRA_CFLAGS=
EXTRA_CXXFLAGS=

.DEFAULT_GOAL=quick
USE_PACKAGE:=1

# Set this to 1 to add additional rules to compile your project as a PROS library template
IS_LIBRARY:=1
LIBNAME:=libpros
VERSION=$(shell cat $(ROOT)/version)
# EXCLUDE_SRC_FROM_LIB= $(SRCDIR)/unpublishedfile.c
EXCLUDE_SRC_FROM_LIB+=$(EXCLUDE_SRCDIRS)
# this line excludes opcontrol.c and similar files
EXCLUDE_SRC_FROM_LIB+=$(foreach file, $(SRCDIR)/opcontrol $(SRCDIR)/initialize $(SRCDIR)/autonomous,$(foreach cext,$(CEXTS),$(file).$(cext)) $(foreach cxxext,$(CXXEXTS),$(file).$(cxxext)))

# files that get distributed to every user (beyond your source archive) - add
# whatever files you want here. This line is configured to add all header files
# that are in the the include directory get exported
TEMPLATE_FILES=$(ROOT)/common.mk $(FWDIR)/v5.ld $(FWDIR)/v5-common.ld $(FWDIR)/v5-hot.ld
TEMPLATE_FILES+= $(INCDIR)/api.h $(INCDIR)/main.h $(INCDIR)/pros/*.* $(INCDIR)/display
TEMPLAtE_FILES+= $(SRCDIR)/opcontrol.cpp $(SRCDIR)/initialize.cpp $(SRCDIR)/autonomous.cpp

PATCHED_SDK=$(FWDIR)/libv5rts/sdk/vexv5/libv5rts.patched.a

EXTRA_LIB_DEPS=$(INCDIR)/api.h $(PATCHED_SDK)

################################################################################
################################################################################
########## Nothing below this line should be edited by typical users ###########
-include ./common.mk

.PHONY: $(INCDIR)/api.h
$(INCDIR)/api.h: version.py
	$(VV)python version.py

$(PATCHED_SDK): $(FWDIR)/libv5rts/sdk/vexv5/libv5rts.a
	@echo -n "Stripping unwanted symbols from libv5rts.a "
	$(call test_output,$D$(STRIP) $^ @libv5rts-strip-options.txt -o $@, $(DONE_STRING))

template: clean-template library
	$(VV)mkdir -p $(TEMPLATE_DIR)
	@echo "Moving template files to $(TEMPLATE_DIR)"
	$Dcp --parents -r $(TEMPLATE_FILES) $(TEMPLATE_DIR)
	$(VV)mkdir -p $(TEMPLATE_DIR)/firmware
	$Dcp $(LIBAR) $(TEMPLATE_DIR)/firmware
	$Dcp $(ROOT)/template-Makefile $(TEMPLATE_DIR)/Makefile
	@echo "Creating template"
	$Dprosv5 c create-template $(TEMPLATE_DIR) kernel $(shell cat $(ROOT)/version) --system "./**/*" --user "src/opcontrol.{c,cpp,cc}" --user "src/initialize.{cpp,c,cc}" --user "src/autonomous.{cpp,c,cc}" --user "include/main.{hpp,h,hh}" --user "Makefile" --target v5 --output bin/output.bin

LIBV5RTS_EXTRACTION_DIR=$(BINDIR)/libv5rts
$(LIBAR): $(call GETALLOBJ,$(EXCLUDE_SRC_FROM_LIB)) $(EXTRA_LIB_DEPS)
	$(VV)mkdir -p $(LIBV5RTS_EXTRACTION_DIR)
	@echo -n "Extracting libv5rts "
	$(call test_output,$Dcd $(LIBV5RTS_EXTRACTION_DIR) && $(AR) x ../../$(PATCHED_SDK),$(DONE_STRING))
	$(eval LIBV5RTS_OBJECTS := $(shell $(AR) t $(PATCHED_SDK)))
	@echo -n "Creating $@ "
	$(call test_output,$D$(AR) rcs $@ $(addprefix $(LIBV5RTS_EXTRACTION_DIR)/, $(LIBV5RTS_OBJECTS)) $(call GETALLOBJ,$(EXCLUDE_SRC_FROM_LIB)),$(DONE_STRING))
	@echo -n "Stripping non-public symbols "
	$(call test_output,$D$(OBJCOPY) -S -D -g --strip-unneeded --keep-symbols public_symbols.txt $@,$(DONE_STRING))
