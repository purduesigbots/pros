# Universal C Makefile for MCU targets

# Path to project root (NO trailing slash!)
ROOT=..
# Binary output directory
BINDIR=$(ROOT)/bin
# Subdirectories to include in the build
SUBDIRS=

# Nothing below here needs to be modified by typical users

# Include common aspects of this project
-include $(ROOT)/common.mk

ASMSRC:=$(wildcard *.$(ASMEXT))
ASMOBJ:=$(patsubst %.o,$(BINDIR)/%.o,$(ASMSRC:.$(ASMEXT)=.o))
HEADERS:=$(wildcard *.$(HEXT))
CSRC=$(wildcard *.$(CEXT))
COBJ:=$(patsubst %.o,$(BINDIR)/%.o,$(CSRC:.$(CEXT)=.o))
CPPSRC:=$(wildcard *.$(CPPEXT))
CPPOBJ:=$(patsubst %.o,$(BINDIR)/%.o,$(CPPSRC:.$(CPPEXT)=.o))
OUT:=$(BINDIR)/$(OUTNAME)

.PHONY: all do_subdirs _force_look

# By default, compile program
all: do_subdirs .

# Phony force-look target
_force_look:
	@true

# Compiles the program if anything is changed
.: $(SUBDIRS) $(ASMOBJ) $(COBJ) $(CPPOBJ)
	@touch .

# Builds the subdirectories
do_subdirs: _force_look
	@for dir in $(SUBDIRS); do $(MAKE) --no-print-directory -C $$dir || exit 1; done

# Assembly source file management
$(ASMOBJ): $(BINDIR)/%.o: %.$(ASMEXT)
	@echo AS $<
	@$(AS) $(AFLAGS) -o $@ $<

# Object management
$(COBJ): $(BINDIR)/%.o: %.$(CEXT) $(HEADERS)
	@echo CC $(INCLUDE) $<
	@$(CC) $(INCLUDE) $(CFLAGS) -o $@ $<

$(CPPOBJ): $(BINDIR)/%.o: %.$(CPPEXT) $(HEADERS)
	@echo CPC $(INCLUDE) $<
	@$(CPPCC) $(INCLUDE) $(CPPFLAGS) -o $@ $<
