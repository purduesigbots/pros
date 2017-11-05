# Universal C Makefile for MCU targets
# Top-level template file to configure build

# Nothing in this file should be modified by typical users

MAKE_COMMAND=make

# Makefile for IFI VeX Cortex Microcontroller (STM32F103VD series)
DEVICE=VexCortex
# Prefix for ARM tools (must be on the path)
MCUPREFIX=arm-none-eabi-
# Flags for the assembler
MCUAFLAGS=-mthumb -mcpu=cortex-m3 -mlittle-endian
# Flags for the compiler
MCUCFLAGS=-mthumb -mcpu=cortex-m3 -mlittle-endian -mfloat-abi=soft
# Flags for the linker
MCULFLAGS=-nostartfiles -Wl,-static -Bfirmware -Wl,-u,VectorTable -Wl,-T -Xlinker firmware/cortex.ld

# Flags for programs
AFLAGS:=$(MCUAFLAGS)
ARFLAGS:=$(MCUCFLAGS)
CCFLAGS:=-Wall $(MCUCFLAGS) -Os -ffunction-sections -fdata-sections -fsigned-char -fomit-frame-pointer -fsingle-precision-constant
CFLAGS:=$(CCFLAGS) -std=gnu99 -Werror=implicit-function-declaration
CXXFLAGS:=$(CCFLAGS) -fno-exceptions -fno-rtti -felide-constructors
LDFLAGS:=-Wall $(MCUCFLAGS) $(MCULFLAGS) -Wl,--gc-sections
SIZEFLAGS=-d --common
NUMFMTFLAGS=--to=iec --format %.2f --suffix=B

AR:=$(MCUPREFIX)ar
# using arm-none-eabi-as generates a listing by default. This produces a super verbose output.
# Using gcc accomplishes the same thing without the extra output
AS:=$(MCUPREFIX)gcc
CC:=$(MCUPREFIX)gcc
CXX:=$(MCUPREFIX)g++
OBJCOPY:=$(MCUPREFIX)objcopy
SIZETOOL:=$(MCUPREFIX)size
READELF:=$(MCUPREFIX)readelf

# Miscelaneous Utilities
SPACE :=
SPACE +=
COMMA := ,
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
