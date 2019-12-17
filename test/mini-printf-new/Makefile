CROSS_TOOL = logic32-elf-
CC = $(CROSS_TOOL)gcc
AS = $(CROSS_TOOL)as
OBJDUJP = $(CROSS_TOOL)objdump
OBJCOPY = $(CROSS_TOOL)objcopy
SIZE = $(CROSS_TOOL)size

ifdef OPT
CFLAGS += -O$(OPT)
endif

SRCS += mini-printf.c
SRCS += test1.c
SRCS += uart.c

all: a.out

a.out: test1.o mini-printf.o uart.o
	$(CC) $^ -Wl,-Map=test.map -Tdata 0x20000

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
	l32-sim -bin a.bin

simd: a.bin
	l32-sim -bin a.bin -d

tags:
	ctags -R

clean:
	$(RM) -rf *.o a.out *.bin *.map *.s tags

PHONY: all dis clean size tags
