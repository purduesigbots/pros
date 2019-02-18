ARCHTUPLE=arm-none-eabi-
DEVICE=VEX EDR V5

MFLAGS=-mcpu=cortex-a9 -mfpu=neon-fp16 -mfloat-abi=softfp
CPPFLAGS=-D_POSIX_THREADS -D_UNIX98_THREAD_MUTEX_ATTRIBUTES -Os
GCCFLAGS=-ffunction-sections -fdata-sections -fdiagnostics-color

WARNFLAGS+=

SPACE :=
SPACE +=
COMMA := ,

LIBRARIES+=$(wildcard $(FWDIR)/*.a)
wlprefix=-Wl,$(subst $(SPACE),$(COMMA),$1)
LNK_FLAGS=--gc-sections --start-group $(strip $(LIBRARIES)) -lc -lm -lgcc -lstdc++ -lsupc++ --end-group

ASMFLAGS=$(MFLAGS) $(WARNFLAGS)
CFLAGS=$(MFLAGS) $(CPPFLAGS) $(WARNFLAGS) $(GCCFLAGS) --std=gnu11
CXXFLAGS=$(MFLAGS) $(CPPFLAGS) $(WARNFLAGS) -funwind-tables $(GCCFLAGS) --std=gnu++17
LDFLAGS=$(MFLAGS) $(WARNFLAGS) -nostdlib
SIZEFLAGS=-d --common
NUMFMTFLAGS=--to=iec --format %.2f --suffix=B

AR:=$(ARCHTUPLE)ar
# using arm-none-eabi-as generates a listing by default. This produces a super verbose output.
# Using gcc accomplishes the same thing without the extra output
AS:=$(ARCHTUPLE)gcc
CC:=$(ARCHTUPLE)gcc
CXX:=$(ARCHTUPLE)g++
LD:=$(ARCHTUPLE)g++
OBJCOPY:=$(ARCHTUPLE)objcopy
SIZETOOL:=$(ARCHTUPLE)size
READELF:=$(ARCHTUPLE)readelf
STRIP:=$(ARCHTUPLE)strip

ifneq (, $(shell command -v gnumfmt 2> /dev/null))
	SIZES_NUMFMT:=| gnumfmt --field=-4 --header $(NUMFMTFLAGS)
else
ifneq (, $(shell command -v numfmt 2> /dev/null))
	SIZES_NUMFMT:=| numfmt --field=-4 --header $(NUMFMTFLAGS)
else
	SIZES_NUMFMT:=
endif
endif

ifneq (, $(shell command -v sed 2> /dev/null))
SIZES_SED:=| sed -e 's/  dec/total/'
else
SIZES_SED:=
endif

rwildcard=$(foreach d,$(filter-out $3,$(wildcard $1*)),$(call rwildcard,$d/,$2,$3)$(filter $(subst *,%,$2),$d))

# Colors
NO_COLOR=\x1b[0m
OK_COLOR=\x1b[32;01m
ERROR_COLOR=\x1b[31;01m
WARN_COLOR=\x1b[33;01m
STEP_COLOR=\x1b[37;01m
OK_STRING=$(OK_COLOR)[OK]$(NO_COLOR)
DONE_STRING=$(OK_COLOR)[DONE]$(NO_COLOR)
ERROR_STRING=$(ERROR_COLOR)[ERRORS]$(NO_COLOR)
WARN_STRING=$(WARN_COLOR)[WARNINGS]$(NO_COLOR)
ECHO=/bin/echo -e
echo=@$(ECHO) "$2$1$(NO_COLOR)"
echon=@$(ECHO) -n "$2$1$(NO_COLOR)"

define test_output
@rm -f temp.log temp.errors
$1 2> temp.log || touch temp.errors
@if test -e temp.errors; then $(ECHO) "$(ERROR_STRING)" && cat temp.log; elif test -s temp.log; then $(ECHO) "$(WARN_STRING)" && cat temp.log; else $(ECHO) "$2"; fi;
@if test -e temp.errors; then rm -f temp.log temp.errors && false; fi;
@rm -f temp.log temp.errors
endef

# Makefile Verbosity
ifeq ("$(origin VERBOSE)", "command line")
BUILD_VERBOSE = $(VERBOSE)
endif
ifeq ("$(origin V)", "command line")
BUILD_VERBOSE = $(V)
endif

ifndef BUILD_VERBOSE
BUILD_VERBOSE = 0
endif

# R is reduced (default messages) - build verbose = 0
# V is verbose messages - verbosity = 1
# VV is super verbose - verbosity = 2
ifeq ($(BUILD_VERBOSE), 0)
R = @echo
D = @
VV = @
endif
ifeq ($(BUILD_VERBOSE), 1)
R = @echo
D =
VV = @
endif
ifeq ($(BUILD_VERBOSE), 2)
R =
D =
VV =
endif

INCLUDE=$(foreach dir,$(INCDIR) $(EXTRA_INCDIR),-iquote"$(dir)")

ASMSRC=$(foreach asmext,$(ASMEXTS),$(call rwildcard, $(SRCDIR),*.$(asmext), $1))
ASMOBJ=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(call ASMSRC,$1)))
CSRC=$(foreach cext,$(CEXTS),$(call rwildcard, $(SRCDIR),*.$(cext), $1))
COBJ=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(call CSRC, $1)))
CXXSRC=$(foreach cxxext,$(CXXEXTS),$(call rwildcard, $(SRCDIR),*.$(cxxext), $1))
CXXOBJ=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(call CXXSRC,$1)))

GETALLOBJ=$(sort $(call ASMOBJ,$1) $(call COBJ,$1) $(call CXXOBJ,$1))

ARCHIVE_TEXT_LIST=$(subst $(SPACE),$(COMMA),$(notdir $(basename $(LIBRARIES))))

LDTIMEOBJ:=$(BINDIR)/_pros_ld_timestamp.o

MONOLITH_BIN:=$(BINDIR)/monolith.bin
MONOLITH_ELF:=$(basename $(MONOLITH_BIN)).elf

HOT_BIN:=$(BINDIR)/hot.package.bin
HOT_ELF:=$(basename $(HOT_BIN)).elf
COLD_BIN:=$(BINDIR)/cold.package.bin
COLD_ELF:=$(basename $(COLD_BIN)).elf

# Check if USE_PACKAGE is defined to check for migration steps from purduesigbots/pros#87
ifndef USE_PACKAGE
$(error Your Makefile must be migrated! Visit https://pros.cs.purdue.edu/v5/releases/kernel3.1.6.html to learn how)
endif

DEFAULT_BIN=$(MONOLITH_BIN)
ifeq ($(USE_PACKAGE),1)
DEFAULT_BIN=$(HOT_BIN)
endif

.PHONY: all clean quick

quick: $(DEFAULT_BIN)

all: clean $(DEFAULT_BIN)

clean:
	@echo Cleaning project
	-$Drm -rf $(BINDIR)

ifeq ($(IS_LIBRARY),1)
ifeq ($(LIBNAME),libbest)
$(errror "You should rename your library! libbest is the default library name and should be changed")
endif

LIBAR=$(BINDIR)/$(LIBNAME).a
TEMPLATE_DIR=$(ROOT)/template

clean-template:
	@echo Cleaning $(TEMPLATE_DIR)
	-$Drm -rf $(TEMPLATE_DIR)

$(LIBAR): $(call GETALLOBJ,$(EXCLUDE_SRC_FROM_LIB)) $(EXTRA_LIB_DEPS)
	-$Drm -f $@
	@echo -n "Creating $@ "
	$(call test_output,$D$(AR) rcs $@ $^, $(DONE_STRING))

.PHONY: library
library: $(LIBAR)

.PHONY: template
template: clean-template $(LIBAR)
	$Dprosv5 c create-template . $(LIBNAME) $(VERSION) $(foreach file,$(TEMPLATE_FILES) $(LIBAR),--system "$(file)") --target v5 $(CREATE_TEMPLATE_FLAGS)
endif

# if project is a library source, compile the archive and link output.elf against the archive rather than source objects
ifeq ($(IS_LIBRARY),1)
ELF_DEPS=$(filter-out $(call GETALLOBJ,$(EXCLUDE_SRC_FROM_LIB)), $(call GETALLOBJ,$(EXCLUDE_SRCDIRS)))
LIBRARIES+=$(LIBAR)
else
ELF_DEPS=$(call GETALLOBJ,$(EXCLUDE_SRCDIRS))
endif

$(MONOLITH_BIN): $(MONOLITH_ELF) $(BINDIR)
	@echo -n "Creating $@ for $(DEVICE) "
	$(call test_output,$D$(OBJCOPY) $< -O binary -R .hot_init $@,$(DONE_STRING))

$(MONOLITH_ELF): $(ELF_DEPS) $(LIBRARIES)
	$(call _pros_ld_timestamp)
	@echo -n "Linking project with $(ARCHIVE_TEXT_LIST) "
	$(call test_output,$D$(LD) $(LDFLAGS) $(ELF_DEPS) $(LDTIMEOBJ) $(call wlprefix,-T$(FWDIR)/v5.ld $(LNK_FLAGS)) -o $@,$(OK_STRING))
	@echo Section sizes:
	-$(VV)$(SIZETOOL) $(SIZEFLAGS) $@ $(SIZES_SED) $(SIZES_NUMFMT)

$(COLD_BIN): $(COLD_ELF)
	@echo -n "Creating cold package binary for $(DEVICE) "
	$(call test_output,$D$(OBJCOPY) $< -O binary -R .hot_init $@,$(DONE_STRING))

$(COLD_ELF): $(LIBRARIES)
	$(call _pros_ld_timestamp)
	@echo -n "Creating cold package with $(ARCHIVE_TEXT_LIST) "
	$(call test_output,$D$(LD) $(LDFLAGS) $(LDTIMEOBJ) $(call wlprefix,--gc-keep-exported --whole-archive $^ -lstdc++ --no-whole-archive) $(call wlprefix,-T$(FWDIR)/v5.ld $(LNK_FLAGS) -o $@),$(OK_STRING))
	@echo -n "Stripping cold package "
	$(call test_output,$D$(OBJCOPY) --strip-symbol=install_hot_table --strip-symbol=__libc_init_array --strip-symbol=_PROS_COMPILE_DIRECTORY --strip-symbol=_PROS_COMPILE_TIMESTAMP $@ $@, $(DONE_STRING))
	@echo Section sizes:
	-$(VV)$(SIZETOOL) $(SIZEFLAGS) $@ $(SIZES_SED) $(SIZES_NUMFMT)

$(HOT_BIN): $(HOT_ELF) $(COLD_BIN)
	@echo -n "Creating $@ for $(DEVICE) "
	$(call test_output,$D$(OBJCOPY) $< -O binary $@,$(DONE_STRING))

$(HOT_ELF): $(COLD_ELF) $(ELF_DEPS)
	$(call _pros_ld_timestamp)
	@echo -n "Linking hot project with $(COLD_ELF) and $(ARCHIVE_TEXT_LIST) "
	$(call test_output,$D$(LD) $(LDFLAGS) $(call wlprefix,-nostartfiles -R $<) $(filter-out $<,$^) $(LDTIMEOBJ) $(LIBRARIES) $(call wlprefix,-T$(FWDIR)/v5-hot.ld $(LNK_FLAGS) -o $@),$(OK_STRING))
	@echo Section sizes:
	-$(VV)$(SIZETOOL) $(SIZEFLAGS) $@ $(SIZES_SED) $(SIZES_NUMFMT)

define asm_rule
$(BINDIR)/%.$1.o: $(SRCDIR)/%.$1
	$(VV)mkdir -p $$(dir $$@)
	@echo -n "Compiling $$< "
	$$(call test_output,$D$(AS) -c $(ASMFLAGS) -o $$@ $$<,$(OK_STRING))
endef
$(foreach asmext,$(ASMEXTS),$(eval $(call asm_rule,$(asmext))))

define c_rule
$(BINDIR)/%.$1.o: $(SRCDIR)/%.$1
	$(VV)mkdir -p $$(dir $$@)
	@echo -n "Compiling $$< "
	$$(call test_output,$D$(CC) -c $(INCLUDE) -iquote"$(INCDIR)/$$(dir $$*)" $(CFLAGS) $(EXTRA_CFLAGS) -o $$@ $$<,$(OK_STRING))
endef
$(foreach cext,$(CEXTS),$(eval $(call c_rule,$(cext))))

define cxx_rule
$(BINDIR)/%.$1.o: $(SRCDIR)/%.$1
	$(VV)mkdir -p $$(dir $$@)
	@echo -n "Compiling $$< "
	$$(call test_output,$D$(CXX) -c $(INCLUDE) -iquote"$(INCDIR)/$$(dir $$*)" $(CXXFLAGS) $(EXTRA_CXXFLAGS) -o $$@ $$<,$(OK_STRING))
endef
$(foreach cxxext,$(CXXEXTS),$(eval $(call cxx_rule,$(cxxext))))

define _pros_ld_timestamp
$(VV)mkdir -p $(dir $(LDTIMEOBJ))
@echo -n "Adding timestamp "
@# Pipe a line of code defining _PROS_COMPILE_TOOLSTAMP and _PROS_COMPILE_DIRECTORY into GCC,
@# which allows compilation from stdin. We define _PROS_COMPILE_DIRECTORY using a command line-defined macro
@# which is the pwd | tail bit, which will truncate the path to the last 23 characters
$(call test_output, $(VV)echo 'char const * const _PROS_COMPILE_TIMESTAMP = __DATE__ " " __TIME__; char const * const _PROS_COMPILE_DIRECTORY = "$(shell pwd | tail -c 23)";' | $(CC) -c -x c $(CFLAGS) $(EXTRA_CFLAGS) -o $(LDTIMEOBJ) -,$(OK_STRING))
endef

# these rules are for build-compile-commands, which just print out sysroot information
cc-sysroot:
	@echo | $(CC) -c -x c $(CFLAGS) $(EXTRA_CFLAGS) --verbose -o /dev/null -
cxx-sysroot:
	@echo | $(CXX) -c -x c++ $(CXXFLAGS) $(EXTRA_CXXFLAGS) --verbose -o /dev/null -
