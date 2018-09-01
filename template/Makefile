# Universal C Makefile for MCU targets

# Path to project root (for top-level, so the project is in ./; first-level, ../; etc.)
ROOT=.
# Binary output directory
BINDIR=$(ROOT)/bin
SRCDIR=$(ROOT)/src

# Nothing below here needs to be modified by typical users

# Include common aspects of this project
-include $(ROOT)/common.mk

ASMSRC:=$(call rwildcard,$(SRCDIR),*.$(ASMEXT))
ASMOBJ:=$(patsubst %.o,$(BINDIR)/%.o,$(ASMSRC:.$(ASMEXT)=.o))
HEADERS:=$(wildcard *.$(HEXT))
CSRC=$(call rwildcard,$(SRCDIR),*.$(CEXT))
COBJ:=$(addprefix $(BINDIR)/, $(patsubst $(SRCDIR)/%.$(CEXT),%.o,$(CSRC)))
CPPSRC:=$(call rwildcard,$(SRCDIR),*.$(CPPEXT))
CPPOBJ:=$(patsubst %.o,$(BINDIR)/%.o,$(CPPSRC:.$(CPPEXT)=.o))
OUT:=$(BINDIR)/$(OUTNAME)

.PHONY: all clean upload _force_look

# By default, compile program
all: $(BINDIR) $(OUT)

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

# Ensure binary directory exists
$(BINDIR):
	-@mkdir -p $(BINDIR)

# Compile program
$(OUT): $(ASMOBJ) $(COBJ) $(CPPOBJ)
	@echo LN $(BINDIR)/*.o $(LIBRARIES) to $@
	@$(CC) $(LDFLAGS) $(BINDIR)/*.o $(LIBRARIES) -o $@
	@$(MCUPREFIX)size $(SIZEFLAGS) $(OUT)
	$(MCUPREPARE)

# Assembly source file management
$(ASMOBJ): $(BINDIR)/%.o: $(SRCDIR)/%.$(ASMEXT) $(HEADERS)
	@mkdir -p $(dir $@)
	@echo AS $<
	@$(AS) $(AFLAGS) -o $@ $<

# Object management
$(COBJ): $(BINDIR)/%.o: $(SRCDIR)/%.$(CEXT) $(HEADERS)
	@mkdir -p $(dir $@)
	@echo CC $(INCLUDE) $< -o $@
	@$(CC) $(INCLUDE) $(CFLAGS) -o $@ $<

$(CPPOBJ): $(BINDIR)/%.o: $(SRCDIR)/%.$(CPPEXT) $(HEADERS)
	@mkdir -p $(dir $@)
	@echo CPC $(INCLUDE) $<
	@$(CPPCC) $(INCLUDE) $(CPPFLAGS) -o $@ $<

