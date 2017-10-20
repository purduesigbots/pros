-include ./common.mk

WARNFLAGS += -Wpedantic

LIBNAME=libpros

ROOT=.
BINDIR=$(ROOT)/bin
SRCDIR=$(ROOT)/src
INCDIR=$(ROOT)/include

INCLUDE=-I$(INCDIR)

ASMSRC:=$(foreach asmext,$(ASMEXTS),$(call rwildcard, $(SRCDIR),*.$(asmext)))
ASMOBJ:=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(ASMSRC)))
CSRC:=$(foreach cext,$(CEXTS),$(call rwildcard, $(SRCDIR),*.$(cext)))
COBJ:=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(CSRC)))
CXXSRC:=$(foreach cxxext,$(CXXEXTS),$(call rwildcard, $(SRCDIR),*.$(cxxext)))
CXXOBJ:=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(CXXSRC)))

LIBRARIES=$(wildcard $(FWDIR)/*.a) -L$(FWDIR) -Wl,--start-group,-lv5rts,-lc,-lm,-lgcc,--end-group

OUTNAME:=$(notdir $(realpath .))
OUTBIN:=$(BINDIR)/$(OUTNAME).bin
OUTELF:=$(BINDIR)/$(OUTNAME).elf

.PHONY: all clean library template flash upload upload-legacy _force_look release develop

VERSION:=`git describe --abbrev=0`

all: clean $(OUTBIN)

clean:
	@echo Cleaning project
	-$Drm -rf $(BINDIR)

$(OUTBIN): $(OUTELF)
	@echo Creating binary
	$(VV)mkdir -p $(dir $@)
	$D$(OBJCOPY) $(OUTELF) -O binary $(OUTBIN)
	@echo Output size:
	-$D$(SIZETOOL) $(SIZEFLAGS) $< | sed --expression='s/  dec/total/' | numfmt --field=-4 --header $(NUMFMTFLAGS)

$(OUTELF): $(ASMOBJ) $(COBJ)
	@echo Linking project
	$D$(CC) $(LDFLAGS) $(ASMOBJ) $(COBJ) $(LIBRARIES) -o $@

$(ASMOBJ): $(BINDIR)/%.o: $(SRCDIR)/%
	@echo Assembling $<
	$(VV)mkdir -p $(dir $@)
	$D$(AS) -c $(ASMFLAGS) -o $@ $<

$(COBJ): $(BINDIR)/%.o: $(SRCDIR)/%
	@echo Compiling $<
	$(VV)mkdir -p $(dir $@)
	$D$(CC) -c $(INCLUDE) -I$(INCDIR)/$(dir $*) $(CFLAGS) -o $@ $<

$(CXXOBJ): $(BINDIR)/%.o: $(SRCDIR)/%
	@echo Compiling $<
	$(VV)mkdir -p $(dir $@)
	$D$(CXX) -c $(INCLUDE) -I$(INCDIR)/$(dir $*) $(CXXFLAGS) -o $@ $<
