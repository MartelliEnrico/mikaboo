BASEDIR ?= /usr/local
INCDIR := $(BASEDIR)/include/uarm
VPATH := $(INCDIR) include lib/phase1 src

CC := arm-none-eabi-gcc
CFLAGS += -O2 -Wall
CPPFLAGS += -I $(INCDIR) -I include
TARGET_ARCH := -mcpu=arm7tdmi
LDFLAGS += -T $(INCDIR)/ldscripts/elf32ltsarm.h.uarmcore.x

.PHONY: all
all: kernel

kernel: crtso.o libuarm.o p1test.o process.o threads.o messages.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

.PHONY: check
check: kernel machine.uarm.cfg
	uarm -c machine.uarm.cfg -e -x

machine.uarm.cfg: machine.uarm.stub
	sed -e "s|\@INCDIR\@|$(INCDIR)|g" -e "s|\@CURDIR\@|$(CURDIR)|g" $< > $@

.PHONY: clean
clean: 
	rm -f *.o kernel machine.uarm.cfg term0.uarm