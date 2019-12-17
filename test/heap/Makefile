CROSS_TOOL = logic32-elf-
CC = $(CROSS_TOOL)gcc
AS = $(CROSS_TOOL)as
LD = $(CROSS_TOOL)ld
OBJDUJP = $(CROSS_TOOL)objdump
OBJCOPY = $(CROSS_TOOL)objcopy
SIZE = $(CROSS_TOOL)size

CFLAGS += -O2
CFLAGS += -I.
#LDFLAGS += -nostartfiles
LIBS += -lc

all: a.out

a.out: crt0.o heap.o main.o malloc.o uart.o syscall.o
	$(CC) $(LDFLAGS) -o a.out $^ $(LIBS) -Wl,-Map=your_file.map

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.s: %.c
	$(CC) $(CFLAGS) -S -o $@ $<
	cat $@

dis: a.out
	$(OBJDUJP) -d $<

size: a.out
	$(SIZE) $<

a.bin: a.out
	$(OBJCOPY) -O binary a.out a.bin

sim: a.bin
	l32-sim -bin a.bin

tags:
	ctags -R

clean:
	$(RM) -rf *.o a.out *.bin *.map tags

PHONY: all dis clean size tags
