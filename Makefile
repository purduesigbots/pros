# Universal C Makefile for MCU targets

LIBNAME=libpros
LIBAR=$(BINDIR)/$(LIBNAME).a

# Path to project root (for top-level, so the project is in ./; first-level, ../; etc.)
ROOT=.
# Binary output directory
BINDIR=$(ROOT)/bin
# Subdirectories to include in the build
SUBDIRS=src


# Nothing below here needs to be modified by typical users

# Include common aspects of this project
-include $(ROOT)/common.mk


TEMPLATE_DIR=$(ROOT)/template
TEMPLATE_FILES=$(ROOT)/common.mk $(ROOT)/firmware/*.ld $(ROOT)/include/API.h $(ROOT)/include/main.h $(ROOT)/src/opcontrol.c $(ROOT)/src/init.c $(ROOT)/src/auto.c $(ROOT)/src/Makefile

ASMSRC:=$(wildcard *.$(ASMEXT))
ASMOBJ:=$(patsubst %.o,$(BINDIR)/%.o,$(ASMSRC:.$(ASMEXT)=.o))
HEADERS:=$(wildcard *.$(HEXT))
CSRC=$(wildcard *.$(CEXT))
COBJ:=$(patsubst %.o,$(BINDIR)/%.o,$(CSRC:.$(CEXT)=.o))
CPPSRC:=$(wildcard *.$(CPPEXT))
CPPOBJ:=$(patsubst %.o,$(BINDIR)/%.o,$(CPPSRC:.$(CPPEXT)=.o))
OUT:=$(BINDIR)/$(OUTNAME)

.PHONY: all clean documentation library template flash upload upload-legacy _force_look release develop

# default version just uses the latest tag
VERSION := `git describe --dirty --abbrev`

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
	$(FLASH)

# Alias to upload, more consistent with our terminology
flash: upload

# Uploads program to device using legacy uniflasher JAR file
upload-legacy: all
	$(UPLOAD)

# Phony force-look target
_force_look:
	@true

# Compiles library for general HS use
library: $(LIBAR)

$(LIBAR): clean $(BINDIR) $(SUBDIRS) $(ASMOBJ) $(COBJ) $(CPPOBJ)
	# Get rid of junk that the user should be doing
	-rm -f $(BINDIR)/opcontrol.o
	-rm -f $(BINDIR)/init.o
	-rm -f $(BINDIR)/auto.o
	$(MCUPREFIX)ar rvs $(BINDIR)/$(LIBNAME)_sym.a $(BINDIR)/*.o
	$(MCUPREFIX)objcopy -S -g --strip-unneeded --keep-symbols public.txt $(BINDIR)/$(LIBNAME)_sym.a $(BINDIR)/$(LIBNAME).a

template: clean-template library
	-mkdir -p $(TEMPLATE_DIR)
	@echo "Moving template files to $(TEMPLATE_DIR)"
	cp --parents $(TEMPLATE_FILES) $(TEMPLATE_DIR)
	mkdir -p $(TEMPLATE_DIR)/firmware
	cp $(LIBAR) $(TEMPLATE_DIR)/firmware
	cp $(ROOT)/template-Makefile $(TEMPLATE_DIR)/Makefile
	@echo "Creating template"
	prosv5 c create-template $(TEMPLATE_DIR) kernel $(VERSION) --system "./**/*" --user "src/opcontrol.{c,cpp,cc}" --user "src/init.{cpp,c,cc}" --user "src/auto.{cpp,c,cc}" --user "include/main.{hpp,h,hh}" --user "Makefile" --user "src/Makefile" --target cortex --output bin/output.bin

clean-template:
	-$Drm -rf $(TEMPLATE_DIR)

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
