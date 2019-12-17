CROSS_TOOL = logic32-elf-
CC = $(CROSS_TOOL)gcc
AS = $(CROSS_TOOL)as
LD = $(CROSS_TOOL)ld
OBJDUJP = $(CROSS_TOOL)objdump
OBJCOPY = $(CROSS_TOOL)objcopy
SIZE = $(CROSS_TOOL)size

ifdef OPT
CFLAGS += -O$(OPT)
endif

CFLAGS += -I.
#LDFLAGS += -nostartfiles
LIBS += -lc

all: a.out

a.out: main.o uart.o syscall.o
	$(CC) $(LDFLAGS) -o a.out $^ $(LIBS) -Wl,-Map=your_file.map

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.s: %.c
	$(CC) $(CFLAGS) -S -o $@ $<

dis: a.out
	$(OBJDUJP) -d $<

size: a.out
	$(SIZE) $<

a.bin: a.out
	$(OBJCOPY) -O binary a.out a.bin

sim: a.bin
	l32-sim -bin a.bin -kbd-en

simd: a.bin
	l32-sim -bin a.bin -kbd-en -c 50 -d

tags:
	ctags -R

clean:
	$(RM) -rf *.o a.out *.bin *.map tags *.s

PHONY: all dis clean size tags
