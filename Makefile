TARGET ?= phase1
phase1_TEST := p1test.o
phase1_OBJS := process.o threads.o messages.o

BASEDIR ?= /usr/local
INCDIR := $(BASEDIR)/include/uarm
OBJDIR := build
VPATH := $(INCDIR) include lib/phase1 src

CC := arm-none-eabi-gcc
CFLAGS += -O2 -Wall -Wextra
CPPFLAGS += -I $(INCDIR) -I include
TARGET_ARCH := -mcpu=arm7tdmi
LD := arm-none-eabi-ld
LDFLAGS += -T $(INCDIR)/ldscripts/elf32ltsarm.h.uarmcore.x
LDLIBS += $(INCDIR)/crtso.o $(INCDIR)/libuarm.o
DEPFLAGS = -MM -MF $(subst .o,.d,$@) -MP -MT $@

OBJECTS := $(addprefix $(OBJDIR)/,$($(TARGET)_TEST) $($(TARGET)_OBJS))

.PHONY: all
all: kernel

kernel: $(OBJECTS)
	$(LD) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(OBJDIR)/p1test.o: CFLAGS := $(filter-out -Wall -Wextra,$(CFLAGS))

ifneq "$(MAKECMDGOALS)" "clean"
	-include $(OBJECTS:.o=.d)
endif

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	@$(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) $<
	$(COMPILE.c) $(OUTPUT_OPTION) $<

$(OBJDIR): ; mkdir -p $@

.PHONY: check
check: kernel machine.uarm.cfg ; @./uarm_termination_watcher.sh

machine.uarm.cfg: machine.uarm.stub
	sed -e "s|\@INCDIR\@|$(INCDIR)|g" -e "s|\@CURDIR\@|$(CURDIR)|g" $< > $@

.PHONY: clean
clean: ; -$(RM) $(OBJDIR)/*.o $(OBJDIR)/*.d kernel
