#######################################################################################
###########################  User configurable parameters  ############################
# filename extensions
CEXTS:=c
ASMEXTS:=s S
CXXEXTS:=cpp c++ cc

EXTRA_CFLAGS=
EXTRA_CXXFLAGS=$(EXTRA_CFLAGS)

# If this project builds a library, you'll want to edit these options
LIBNAME=
LIBVERSION=
# space separated list of extra files that get copied to every project
# to include include/a.h, write LIBFILES=include/a.h
LIBFILES=
# space separated list of files to include in the library's archive
# to include src/a.c and src/dir/b.c, write LIBSRC=a.c dir/b.c
LIBSRC=

# Uncomment the next line to make the default rule to build the library
#.DEFAULT_GOAL=library

#######################################################################################
#######################################################################################
# Nothing below this line should be edited by typical users
# Path to project root (for top-level, so the project is in ./; first-level, ../; etc.)
ROOT=.

# Binary output directory
BINDIR=$(ROOT)/bin
SRCDIR=$(ROOT)/src
INCDIR=$(ROOT)/include
FWDIR=$(ROOT)/firmware

# Include common aspects of this project
-include $(ROOT)/common.mk

INCLUDE=-I$(INCDIR) -I$(SRCDIR)

ASMSRC:=$(foreach asmext,$(ASMEXTS),$(call rwildcard, $(SRCDIR),*.$(asmext)))
ASMOBJ:=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(ASMSRC)))
CSRC:=$(foreach cext,$(CEXTS),$(call rwildcard, $(SRCDIR),*.$(cext)))
COBJ:=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(CSRC)))
CXXSRC:=$(foreach cxxext,$(CXXEXTS),$(call rwildcard, $(SRCDIR),*.$(cxxext)))
CXXOBJ:=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(CXXSRC)))
ARCHIVES:=$(wildcard $(FWDIR)/*.a)

ARCHIVE_TEXT_LIST:=$(subst $(SPACE),$(COMMA)$(SPACE),$(sort $(foreach archive,$(ARCHIVES),$(basename $(notdir $(archive))))))

ifdef $(shell comand -v gnumfmt 2> /dev/null)  # support for MacOS
SIZES_NUMFMT:=| gnumfmt --field=-4 --header $(NUMFMTFLAGS)
else
ifdef $(shell command -v numfmt 2> /dev/null)
SIZES_NUMFMT:=| numfmt --field=-4 --header $(NUMFMTFLAGS)
else
SIZES_NUMFMT:=
endif
endif

ifdef $(shell command -v sed 2> /dev/null)
SIZES_SED:=| sed -e 's/  dec/total/'
else
SIZES_SED:=
endif

LIBRARIES=$(ARCHIVES) -L$(FWDIR) -Wl,--start-group,-lc,-lm,-lgcc,--end-group

OUTNAME:=output
OUTBIN:=$(BINDIR)/$(OUTNAME).bin
OUTELF:=$(BINDIR)/$(OUTNAME).elf

.PHONY: all clean flash upload upload-legacy library print_compiling

# By default, compile program
all: $(OUTBIN)

# Remove all intermediate object files (remove the binary directory)
clean:
	@echo Cleaning project
	-$Drm -rf $(BINDIR)

# Uploads program to device
upload: $(OUTBIN)
	$(call echo,Uploading $^ to $(DEVICE) using PROS CLI,$(STEP_COLOR))
	pros flash -f $^

# Alias to upload, more consistent with our terminology
flash: upload

# Uploads program to device using legacy uniflasher JAR file
upload-legacy: $(OUTBIN)
	$(call echo,Uploading $^ to $(DEVICE) using uniflasher,$(STEP_COLOR))
	@java -jar firmware/uniflash.jar vex $^

$(OUTBIN): $(OUTELF)
	$(VV)mkdir -p $(dir $@)
	@echo Section sizes:
	-$(VV)$(SIZETOOL) $(SIZEFLAGS) $< $(SIZES_SED) $(SIZES_NUMFMT)
	@echo -n "Creating $@ for $(DEVICE) "
	$(call test_output,$D$(OBJCOPY) $< -O binary $@,$(DONE_STRING))

$(OUTELF): $(sort $(ASMOBJ) $(COBJ) $(CXXOBJ))
	@echo -n "Linking project with $(ARCHIVE_TEXT_LIST) "
	$(call test_output,$D$(CC) $(LDFLAGS) $^ $(LIBRARIES) -o $@,$(OK_STRING))

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
	$$(call test_output,$D$(CC) -c $(INCLUDE) $(CFLAGS) $(EXTRA_CFLAGS) -o $$@ $$<,$(OK_STRING))
endef
$(foreach cext,$(CEXTS),$(eval $(call c_rule,$(cext))))

define cxx_rule
$(BINDIR)/%.$1.o: $(SRCDIR)/%.$1
	@echo -n "Compiling $$< "
	$(VV)mkdir -p $$(dir $$@)
	$$(call test_output,$D$(CXX) -c $(INCLUDE) $(CXXFLAGS) $(EXTRA_CXXFLAGS) -o $$@ $$<,$(OK_STRING))
endef
$(foreach cxxext,$(CXXEXTS),$(eval $(call cxx_rule,$(cxxext))))

# Template targets
ifdef LIBNAME
LIBOBJ=$(addsuffix .o,$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%,$(LIBSRC))))
TEMPLATE=$(ROOT)/$(LIBNAME)-template

library: clean print_compiling $(LIBOBJ)
	@echo Creating archive of $(LIBOBJ)
	$D$(MCUPREFIX)ar rvs $(BINDIR)/$(LIBNAME).a $(LIBOBJ)
	@echo Copying assets to $(TEMPLATE)
	$(VV)mkdir -p $(TEMPLATE) $(TEMPLATE)/firmware $(addprefix $(TEMPLATE)/, $(dir $(LIBSRC)))
	$(D)cp $(BINDIR)/$(LIBNAME).a $(TEMPLATE)/firmware/$(LIBNAME).a
	$(foreach f,$(LIBFILES),$(D)cp $(f) $(TEMPLATE)/$(f);)
	@echo Creating template descriptor for PROS CLI
	$(D)pros conduct create-template $(LIBNAME) $(LIBVERSION) $(TEMPLATE) --ignore template.pros --upgrade-files firmware/$(LIBNAME).a $(foreach f,$(LIBFILES),--upgrade-files $(f))
ifdef $(shell command -v zip 2> /dev/null)
		@echo Zipping template
		$(D)cd $(TEMPLATE) && zip -r ../$(basename $(TEMPLATE)) *
		@echo Created template at $(basename $(TEMPLATE)).zip
else
		@echo Need to zip $(TEMPLATE) so that $(notdir $(basename $(TEMPLATE))).zip/template.pros exists
endif
endif
