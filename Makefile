TARGET ?= phase1
phase1_TEST := p1test.o
phase1_OBJS := process.o threads.o messages.o

BASEDIR ?= /usr/local
INCDIR := $(BASEDIR)/include/uarm
OBJDIR := build
VPATH := include $(INCDIR) include/sys src/phase1 src/tests

CROSS_COMPILE ?= arm-none-eabi-
CC := $(CROSS_COMPILE)gcc
OPTIMIZATION ?= -Ofast
WARNINGS ?= -Wall -Wextra
CFLAGS = $(OPTIMIZATION) $(WARNINGS)
CPPFLAGS := -I include -I $(INCDIR) -idirafter include/sys
TARGET_ARCH := -mcpu=arm7tdmi
LD := $(CROSS_COMPILE)ld
LDFLAGS := -T $(INCDIR)/ldscripts/elf32ltsarm.h.uarmcore.x -nostdlib
LDLIBS := $(INCDIR)/crtso.o $(INCDIR)/libuarm.o
ASFLAGS := -fPIC

OBJECTS := $(addprefix $(OBJDIR)/,$($(TARGET)_TEST) $($(TARGET)_OBJS))

.PHONY: all debug check clean distclean help

all: kernel

debug: OPTIMIZATION := -Og
debug: CFLAGS += -g3
debug: kernel

kernel: $(OBJECTS)
	$(LD) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(OBJDIR)/$($(TARGET)_TEST): WARNINGS :=

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(COMPILE.c) $(OUTPUT_OPTION) $<

$(OBJDIR):
	mkdir -p $@

check: kernel bios machine.uarm.cfg
	tools/uarm_termination_watcher.sh

bios: tools/bios.S
	$(COMPILE.S) -o $@ $<

machine.uarm.cfg: tools/machine.uarm.stub
	sed "s|\@CURDIR\@|$(CURDIR)|g" $< > $@

clean:
	-$(RM) $(OBJDIR)/*.o kernel

distclean: clean
	-$(RM) bios term0.uarm machine.uarm.cfg

help:
	@echo "mikaboo - group lso17az05"
	@echo "Usage: make [TARGETS] [OPTIONS]"
	@echo
	@echo "Available Targets:"
	@echo "  all            Builds the kernel for release"
	@echo "                 This is the default target"
	@echo "  debug          Builds the kernel with debug configuration"
	@echo "  check          Run the automatic test using the uarm emulator"
	@echo "  clean          Removes build files generated from 'all' or 'debug'"
	@echo "  distclean      Removes utility files generated from 'check'"
	@echo "  help           This help message"
	@echo
	@echo "Available Options:"
	@echo "  TARGET         Choose the phase to build"
	@echo "                 Default is '$(TARGET)'"
	@echo "  BASEDIR        The base directory where uarm was installed"
	@echo "                 Default is '$(BASEDIR)'"
	@echo "  CROSS_COMPILE  The prefix of the installed build tools"
	@echo "                 Default is '$(CROSS_COMPILE)'"
	@echo "  OPTIMIZATION   The optimization flags used for release build"
	@echo "                 This is overridden for the debug build"
	@echo "                 Default is '$(OPTIMIZATION)'"
	@echo "  WARNINGS       The warnings flags used to build the kernel"
	@echo "                 Default is '$(WARNINGS)'"
	@echo
