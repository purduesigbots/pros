ARCHTUPLE=arm-none-eabi-
DEVICE=VEX EDR V5

MFLAGS=-mcpu=cortex-a9 -mfpu=neon-fp16 -mfloat-abi=softfp -Os -g
CPPFLAGS=-D_POSIX_THREADS -D_UNIX98_THREAD_MUTEX_ATTRIBUTES -D_POSIX_TIMERS -D_POSIX_MONOTONIC_CLOCK
GCCFLAGS=-ffunction-sections -fdata-sections -fdiagnostics-color -funwind-tables

WARNFLAGS+=-Wno-psabi

SPACE := $() $()
COMMA := ,

DEPDIR := .d
$(shell mkdir -p $(DEPDIR))
DEPFLAGS = -MT $$@ -MMD -MP -MF $(DEPDIR)/$$*.Td
MAKEDEPFOLDER = -$(VV)mkdir -p $(DEPDIR)/$$(dir $$(patsubst $(BINDIR)/%, %, $(ROOT)/$$@))
RENAMEDEPENDENCYFILE = -$(VV)mv -f $(DEPDIR)/$$*.Td $$(patsubst $(SRCDIR)/%, $(DEPDIR)/%.d, $(ROOT)/$$<) && touch $$@

LIBRARIES+=$(wildcard $(FWDIR)/*.a)
# Cannot include newlib and libc because not all of the req'd stubs are implemented
EXCLUDE_COLD_LIBRARIES+=$(FWDIR)/libc.a $(FWDIR)/libm.a
COLD_LIBRARIES=$(filter-out $(EXCLUDE_COLD_LIBRARIES), $(LIBRARIES))
wlprefix=-Wl,$(subst $(SPACE),$(COMMA),$1)
LNK_FLAGS=--gc-sections --start-group $(strip $(LIBRARIES)) -lgcc -lstdc++ --end-group -T$(FWDIR)/v5-common.ld

ASMFLAGS=$(MFLAGS) $(WARNFLAGS)
CFLAGS=$(MFLAGS) $(CPPFLAGS) $(WARNFLAGS) $(GCCFLAGS) --std=gnu11
CXXFLAGS=$(MFLAGS) $(CPPFLAGS) $(WARNFLAGS) $(GCCFLAGS) --std=gnu++17
LDFLAGS=$(MFLAGS) $(WARNFLAGS) -nostdlib $(GCCFLAGS)
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
NO_COLOR=$(shell printf "%b" "\033[0m")
OK_COLOR=$(shell printf "%b" "\033[32;01m")
ERROR_COLOR=$(shell printf "%b" "\033[31;01m")
WARN_COLOR=$(shell printf "%b" "\033[33;01m")
STEP_COLOR=$(shell printf "%b" "\033[37;01m")
OK_STRING=$(OK_COLOR)[OK]$(NO_COLOR)
DONE_STRING=$(OK_COLOR)[DONE]$(NO_COLOR)
ERROR_STRING=$(ERROR_COLOR)[ERRORS]$(NO_COLOR)
WARN_STRING=$(WARN_COLOR)[WARNINGS]$(NO_COLOR)
ECHO=/bin/printf "%s\n"
echo=@$(ECHO) "$2$1$(NO_COLOR)"
echon=@/bin/printf "%s" "$2$1$(NO_COLOR)"

define test_output_2
@if test $(BUILD_VERBOSE) -eq $(or $4,1); then printf "%s\n" "$2"; fi;
@output="$$($2 2>&1)"; exit=$$?;           \
if test 0 -ne $$exit; then                 \
  printf "%s%s\n" "$1" "$(ERROR_STRING)";  \
  printf "%s\n" "$$output";                \
  exit $$exit;                             \
elif test -n "$$output"; then              \
  printf "%s%s\n" "$1" "$(WARN_STRING)";   \
  printf "%s\n" "$$output";                \
else                                       \
  printf "%s%s\n" "$1" "$3";               \
fi;
endef

define test_output
@output=$$($1 2>&1); exit=$$?;            \
if test 0 -ne $$exit; then                \
  printf "%s\n" "$(ERROR_STRING)" $$?;    \
  printf "%s\n" $$output;                 \
  exit $$exit;                            \
elif test -n "$$output"; then             \
  printf "%s\n" "$(WARN_STRING)";         \
  printf "%s" $$output;                   \
else                                      \
  printf "%s\n" "$2";                     \
fi;
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

-include $(wildcard $(FWDIR)/*.mk)

.PHONY: all clean quick

quick: $(DEFAULT_BIN)

all: clean $(DEFAULT_BIN)

clean:
	@echo Cleaning project
	-$Drm -rf $(BINDIR)
	-$Drm -rf $(DEPDIR)

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
	$(call test_output_2,Creating $@ ,$(AR) rcs $@ $^, $(DONE_STRING))

.PHONY: library
library: $(LIBAR)

.PHONY: template
template: clean-template $(LIBAR)
	$Dprosv5 c create-template . $(LIBNAME) $(VERSION) $(foreach file,$(TEMPLATE_FILES) $(LIBAR),--system "$(file)") --target v5 $(CREATE_TEMPLATE_FLAGS)
endif

# if project is a library source, compile the archive and link output.elf against the archive rather than source objects
ifeq ($(IS_LIBRARY),1)
ELF_DEPS+=$(filter-out $(call GETALLOBJ,$(EXCLUDE_SRC_FROM_LIB)), $(call GETALLOBJ,$(EXCLUDE_SRCDIRS)))
LIBRARIES+=$(LIBAR)
else
ELF_DEPS+=$(call GETALLOBJ,$(EXCLUDE_SRCDIRS))
endif

$(MONOLITH_BIN): $(MONOLITH_ELF) $(BINDIR)
	$(call test_output_2,Creating $@ for $(DEVICE) ,$(OBJCOPY) $< -O binary -R .hot_init $@,$(DONE_STRING))

$(MONOLITH_ELF): $(ELF_DEPS) $(LIBRARIES)
	$(call _pros_ld_timestamp)
	$(call test_output_2,Linking project with $(ARCHIVE_TEXT_LIST) ,$(LD) $(LDFLAGS) $(ELF_DEPS) $(LDTIMEOBJ) $(call wlprefix,-T$(FWDIR)/v5.ld $(LNK_FLAGS)) -o $@,$(OK_STRING))
	@echo Section sizes:
	-$(VV)$(SIZETOOL) $(SIZEFLAGS) $@ $(SIZES_SED) $(SIZES_NUMFMT)

$(COLD_BIN): $(COLD_ELF)
	$(call test_output_2,Creating cold package binary for $(DEVICE) ,$(OBJCOPY) $< -O binary -R .hot_init $@,$(DONE_STRING))

$(COLD_ELF): $(COLD_LIBRARIES)
	$(VV)mkdir -p $(dir $@)
	$(call test_output_2,Creating cold package with $(ARCHIVE_TEXT_LIST) ,$(LD) $(LDFLAGS) $(call wlprefix,--gc-keep-exported --whole-archive $^ -lstdc++ --no-whole-archive) $(call wlprefix,-T$(FWDIR)/v5.ld $(LNK_FLAGS) -o $@),$(OK_STRING))
	$(call test_output_2,Stripping cold package ,$(OBJCOPY) --strip-symbol=install_hot_table --strip-symbol=__libc_init_array --strip-symbol=_PROS_COMPILE_DIRECTORY --strip-symbol=_PROS_COMPILE_TIMESTAMP --strip-symbol=_PROS_COMPILE_TIMESTAMP_INT $@ $@, $(DONE_STRING))
	@echo Section sizes:
	-$(VV)$(SIZETOOL) $(SIZEFLAGS) $@ $(SIZES_SED) $(SIZES_NUMFMT)

$(HOT_BIN): $(HOT_ELF) $(COLD_BIN)
	$(call test_output_2,Creating $@ for $(DEVICE) ,$(OBJCOPY) $< -O binary $@,$(DONE_STRING))

$(HOT_ELF): $(COLD_ELF) $(ELF_DEPS)
	$(call _pros_ld_timestamp)
	$(call test_output_2,Linking hot project with $(COLD_ELF) and $(ARCHIVE_TEXT_LIST) ,$(LD) -nostartfiles $(LDFLAGS) $(call wlprefix,-R $<) $(filter-out $<,$^) $(LDTIMEOBJ) $(LIBRARIES) $(call wlprefix,-T$(FWDIR)/v5-hot.ld $(LNK_FLAGS) -o $@),$(OK_STRING))
	@printf "%s\n" "Section sizes:"
	-$(VV)$(SIZETOOL) $(SIZEFLAGS) $@ $(SIZES_SED) $(SIZES_NUMFMT)

define asm_rule
$(BINDIR)/%.$1.o: $(SRCDIR)/%.$1
	$(VV)mkdir -p $$(dir $$@)
	$$(call test_output_2,Compiled $$< ,$(AS) -c $(ASMFLAGS) -o $$@ $$<,$(OK_STRING))
endef
$(foreach asmext,$(ASMEXTS),$(eval $(call asm_rule,$(asmext))))

define c_rule
$(BINDIR)/%.$1.o: $(SRCDIR)/%.$1
$(BINDIR)/%.$1.o: $(SRCDIR)/%.$1 $(DEPDIR)/$(basename $1).d
	$(VV)mkdir -p $$(dir $$@)
	$(MAKEDEPFOLDER)
	$$(call test_output_2,Compiled $$< ,$(CC) -c $(INCLUDE) -iquote"$(INCDIR)/$$(dir $$*)" $(CFLAGS) $(EXTRA_CFLAGS) $(DEPFLAGS) -o $$@ $$<,$(OK_STRING))
	$(RENAMEDEPENDENCYFILE)
endef
$(foreach cext,$(CEXTS),$(eval $(call c_rule,$(cext))))

define cxx_rule
$(BINDIR)/%.$1.o: $(SRCDIR)/%.$1
$(BINDIR)/%.$1.o: $(SRCDIR)/%.$1 $(DEPDIR)/$(basename %).d
	$(VV)mkdir -p $$(dir $$@)
	$(MAKEDEPFOLDER)
	$$(call test_output_2,Compiled $$< ,$(CXX) -c $(INCLUDE) -iquote"$(INCDIR)/$$(dir $$*)" $(CXXFLAGS) $(EXTRA_CXXFLAGS) $(DEPFLAGS) -o $$@ $$<,$(OK_STRING))
	$(RENAMEDEPENDENCYFILE)
endef
$(foreach cxxext,$(CXXEXTS),$(eval $(call cxx_rule,$(cxxext))))

define _pros_ld_timestamp
$(VV)mkdir -p $(dir $(LDTIMEOBJ))
@# Pipe a line of code defining _PROS_COMPILE_TOOLSTAMP and _PROS_COMPILE_DIRECTORY into GCC,
@# which allows compilation from stdin. We define _PROS_COMPILE_DIRECTORY using a command line-defined macro
@# which is the pwd | tail bit, which will truncate the path to the last 23 characters
@# 
@# const int _PROS_COMPILE_TIMESTAMP_INT = $(( $(date +%s) - $(date +%z) * 3600 ))
@# char const * const _PROS_COMPILE_TIEMSTAMP = __DATE__ " " __TIME__
@# char const * const _PROS_COMPILE_DIRECTORY = "$(shell pwd | tail -c 23)";
@#
@# The shell command $$(($$(date +%s)+($$(date +%-z)/100*3600))) fetches the current
@# unix timestamp, and then adds the UTC timezone offset to account for time zones.

$(call test_output_2,Adding timestamp ,echo 'const int _PROS_COMPILE_TIMESTAMP_INT = $(shell echo $$(($$(date +%s)+($$(date +%-z)/100*3600)))); char const * const _PROS_COMPILE_TIMESTAMP = __DATE__ " " __TIME__; char const * const _PROS_COMPILE_DIRECTORY = "$(shell pwd | tail -c 23)";' | $(CC) -c -x c $(CFLAGS) $(EXTRA_CFLAGS) -o $(LDTIMEOBJ) -,$(OK_STRING))
endef

# these rules are for build-compile-commands, which just print out sysroot information
cc-sysroot:
	@echo | $(CC) -c -x c $(CFLAGS) $(EXTRA_CFLAGS) --verbose -o /dev/null -
cxx-sysroot:
	@echo | $(CXX) -c -x c++ $(CXXFLAGS) $(EXTRA_CXXFLAGS) --verbose -o /dev/null -

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst $(SRCDIR)/%,$(DEPDIR)/%.d,$(CSRC) $(CXXSRC)))
