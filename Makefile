# Universal C Makefile for MCU targets

LIBNAME=libpros

# Path to project root (for top-level, so the project is in ./; first-level, ../; etc.)
ROOT=.
# Binary output directory
BINDIR=$(ROOT)/bin
# Subdirectories to include in the build
SUBDIRS=src

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

.PHONY: all clean documentation library template upload _force_look release develop

# default version just uses the latest tag
VERSION := `git describe --abbrev=0`

# By default, compile program
all: $(BINDIR) $(OUT)

release:
	$(eval CCFLAGS += -D FW_VERSION="$(VERSION)")

develop:
	$(eval VERSION := `git describe --abbrev=1`)
	$(eval CCFLAGS += -D FW_VERSION="$(VERSION)")

# Remove all intermediate object files (remove the binary directory)
clean:
	-rm -f $(OUT)
	-rm -rf $(BINDIR)

# Uploads program to device
upload: all
	$(UPLOAD)

# Phony force-look target
_force_look:
	@true

# Compiles library for general HS use
library: clean $(BINDIR) $(SUBDIRS) $(ASMOBJ) $(COBJ) $(CPPOBJ)
	# Get rid of junk that the user should be doing
	-rm -f $(BINDIR)/opcontrol.o
	-rm -f $(BINDIR)/init.o
	-rm -f $(BINDIR)/auto.o
	$(MCUPREFIX)ar rvs $(BINDIR)/$(LIBNAME)_sym.a $(BINDIR)/*.o
	$(MCUPREFIX)objcopy -S -g --strip-unneeded --keep-symbols public.txt $(BINDIR)/$(LIBNAME)_sym.a $(BINDIR)/$(LIBNAME).a

TEMPLATEFILES:=src/auto.c src/init.c src/opcontrol.c include/API.h include/main.h firmware
template: library
	-rm -rf $(addprefix $(ROOT)/template/,$(TEMPLATEFILES))
	mkdir -p $(ROOT)/template/src $(ROOT)/template/inlcude $(ROOT)/template/firmware
	$(foreach f,$(TEMPLATEFILES),cp -r $(ROOT)/$(f) $(ROOT)/template/$(f); )
	cp $(BINDIR)/$(LIBNAME).a $(ROOT)/template/firmware/$(LIBNAME).a
	pros conduct create-template kernel $(VERSION) pros-mainline --location $(ROOT)/template -u "firmware/$(LIBNAME).a" -u "include/API.h" -u "common.mk" -i "template.pros"

# Builds the documentation
documentation:
	doxygen Doxyfile

# Looks in subdirectories for things to make
$(SUBDIRS): %: _force_look
	@$(MAKE) --no-print-directory -C $@

# Ensure binary directory exists
$(BINDIR):
	-@mkdir -p $(BINDIR)

# Compile program
$(OUT): $(SUBDIRS) $(ASMOBJ) $(COBJ) $(CPPOBJ)
	@echo LN $(BINDIR)/*.o $(LIBRARIES) to $@
	@$(CC) $(LDFLAGS) $(BINDIR)/*.o $(LIBRARIES) -o $@
	@$(MCUPREFIX)size $(SIZEFLAGS) $(OUT)
	$(MCUPREPARE)

# Assembly source file management
$(ASMOBJ): $(BINDIR)/%.o: %.$(ASMEXT) $(HEADERS)
	@echo AS $<
	@$(AS) $(AFLAGS) -o $@ $<

# Object management
$(COBJ): $(BINDIR)/%.o: %.$(CEXT) $(HEADERS)
	@echo CC $(INCLUDE) $<
	@$(CC) $(INCLUDE) $(CFLAGS) -o $@ $<

$(CPPOBJ): $(BINDIR)/%.o: %.$(CPPEXT) $(HEADERS)
	@echo CPC $(INCLUDE) $<
	@$(CPPCC) $(INCLUDE) $(CPPFLAGS) -o $@ $<
