ROOT:=.
FWDIR:=$(ROOT)/firmware
SRC:=$(ROOT)/src
INCLUDE:=$(ROOT)/include

LIBRARIES=-lgcc -lm
ARCHTUPLE=arm-none-eabi-

MFLAGS=-march=armv7-a -mfpu=neon-fp16 -mfloat-abi=softfp

WARNFLAGS=-Wall

SPACE :=
SPACE +=
COMMA := ,
LNK_FLAGS = -T?%$(FWDIR)/v5.ld --gc-sections

ASMFLAGS=$(MFLAGS) $(WARNFLAGS)
CFLAGS=$(MFLAGS) $(WARNFLAGS) -ffunction-sections -fdata-sections --std=gnu11
CXXFLAGS=$(MFLAGS) $(WARNFLAGS) -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -fno-exceptions --std=gnu++14
LDFLAGS=$(MFLAGS) $(WARNFLAGS) -nostdlib  $(subst ?%,$(SPACE),$(addprefix -Wl$(COMMA), $(LNK_FLAGS)))
SIZEFLAGS=-d --common
NUMFMTFLAGS=--to=iec --format %.2f --suffix=B

AR:=$(ARCHTUPLE)ar
# using arm-none-eabi-as generates a listing by default. This produces a super verbose output.
# Using gcc accomplishes the same thing without the extra output
AS:=$(ARCHTUPLE)gcc
CC:=$(ARCHTUPLE)gcc
CXX:=$(ARCHTUPLE)g++
OBJCOPY:=$(ARCHTUPLE)objcopy
SIZETOOL:=$(ARCHTUPLE)size
READELF:=$(ARCHTUPLE)readelf

# filename extensions
CEXTS:=c
ASMEXTS:=s S
CPPEXTS:=cpp c++ cc


rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)$(filter $(subst *,%,$2),$d))

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