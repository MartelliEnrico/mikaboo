TARGET ?= phase1
phase1_TEST := p1test.o
phase1_OBJS := process.o threads.o messages.o

BASEDIR ?= /usr/local
INCDIR := $(BASEDIR)/include/uarm
OBJDIR := build
VPATH := $(INCDIR) include src/phase1 tests

CC := arm-none-eabi-gcc
WARNING_FLAGS := -Wall -Wextra
CFLAGS += -O2 $(WARNING_FLAGS) -I $(INCDIR) -I include
TARGET_ARCH := -mcpu=arm7tdmi
LD := arm-none-eabi-ld
LDFLAGS += -T $(INCDIR)/ldscripts/elf32ltsarm.h.uarmcore.x
LDLIBS += $(INCDIR)/crtso.o $(INCDIR)/libuarm.o

OBJECTS := $(addprefix $(OBJDIR)/,$($(TARGET)_TEST) $($(TARGET)_OBJS))

.PHONY: all
all: kernel

kernel: $(OBJECTS)
	$(LD) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(OBJDIR)/p1test.o: CFLAGS := $(filter-out $(WARNING_FLAGS),$(CFLAGS))

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(COMPILE.c) $(OUTPUT_OPTION) $<

$(OBJDIR): ; mkdir -p $@

.PHONY: check
check: kernel machine.uarm.cfg
	@./uarm_termination_watcher.sh

machine.uarm.cfg: machine.uarm.stub
	sed -e "s|\@INCDIR\@|$(INCDIR)|g" -e "s|\@CURDIR\@|$(CURDIR)|g" $< > $@

.PHONY: clean
clean:
	-$(RM) $(OBJDIR)/*.o kernel

.PHONY: distclean
distclean: clean
	-$(RM) term0.uarm machine.uarm.cfg
