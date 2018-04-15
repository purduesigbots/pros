ARCHTUPLE=arm-none-eabi-
DEVICE=VEX EDR V5

MFLAGS=-mcpu=cortex-a9 -mfpu=neon-fp16 -mfloat-abi=softfp
CPPFLAGS=-D_POSIX_THREADS -D_UNIX98_THREAD_MUTEX_ATTRIBUTES -Os
GCCFLAGS=-ffunction-sections -fdata-sections -fdiagnostics-color

WARNFLAGS+=

SPACE :=
SPACE +=
COMMA := ,
LNK_FLAGS = -T?%$(FWDIR)/v5.ld --gc-sections

ASMFLAGS=$(MFLAGS) $(WARNFLAGS)
CFLAGS=$(MFLAGS) $(CPPFLAGS) $(WARNFLAGS) $(GCCFLAGS) --std=gnu11
CXXFLAGS=$(MFLAGS) $(CPPFLAGS) $(WARNFLAGS) -funwind-tables $(GCCFLAGS)  --std=gnu++17
LDFLAGS=$(MFLAGS) $(WARNFLAGS) -nostdlib  $(subst ?%,$(SPACE),$(addprefix -Wl$(COMMA), $(LNK_FLAGS)))
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

# these rules are for build-compile-commands, which just print out sysroot information
cc-sysroot:
	@echo | $(CC) -c -x c $(CFLAGS) $(EXTRA_CFLAGS) --verbose -o /dev/null -
cxx-sysroot:
	@echo | $(CXX) -c -x c++ $(CXXFLAGS) $(EXTRA_CXXFLAGS) --verbose -o /dev/null -
