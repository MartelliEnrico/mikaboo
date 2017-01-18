BASEDIR ?= /usr/local
INCDIR := $(BASEDIR)/include/uarm
VPATH := $(INCDIR) include lib/phase1 src

CC := arm-none-eabi-gcc
CFLAGS += -O2 -Wall -Wextra
CPPFLAGS += -I $(INCDIR) -I include
TARGET_ARCH := -mcpu=arm7tdmi
LDFLAGS += -T $(INCDIR)/ldscripts/elf32ltsarm.h.uarmcore.x

TARGET ?= phase1

common_OBJS := crtso.o libuarm.o
phase1_TEST := p1test.o
phase1_OBJS := process.o threads.o messages.o

.PHONY: all
all: kernel

kernel: $(common_OBJS) $($(TARGET)_TEST) $($(TARGET)_OBJS)
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

p1test.o: CFLAGS := $(filter-out -Wall -Wextra,$(CFLAGS))

.PHONY: check
check: kernel machine.uarm.cfg
	uarm -c machine.uarm.cfg -e -x

machine.uarm.cfg: machine.uarm.stub
	sed -e "s|\@INCDIR\@|$(INCDIR)|g" -e "s|\@CURDIR\@|$(CURDIR)|g" $< > $@

.PHONY: clean
clean: 
	rm -f *.o kernel machine.uarm.cfg term0.uarm