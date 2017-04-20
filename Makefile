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

.PHONY: all debug check clean distclean

all: kernel

debug: OPTIMIZATION := -Og
debug: CFLAGS += -g3
debug: kernel

kernel: $(OBJECTS)
	$(LD) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(OBJDIR)/p1test.o: WARNINGS :=

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(COMPILE.c) $(OUTPUT_OPTION) $<

$(OBJDIR):
	mkdir -p $@

check: kernel BIOS machine.uarm.cfg
	tools/uarm_termination_watcher.sh

BIOS: tools/BIOS.s
	$(COMPILE.S) -o $@ $<

machine.uarm.cfg: tools/machine.uarm.stub
	sed "s|\@CURDIR\@|$(CURDIR)|g" $< > $@

clean:
	-$(RM) $(OBJDIR)/*.o kernel

distclean: clean
	-$(RM) term0.uarm machine.uarm.cfg
