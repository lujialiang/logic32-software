CROSS_TOOL = logic32-elf-
CC = $(CROSS_TOOL)gcc
AS = $(CROSS_TOOL)as
OBJDUJP = $(CROSS_TOOL)objdump
OBJCOPY = $(CROSS_TOOL)objcopy
SIZE = $(CROSS_TOOL)size

ifdef OPT
CFLAGS += -O$(OPT)
endif

all: a.out

a.out: main.o syscall.o uart.o
	$(CC) -Wl,-Map=a.map $^ -lc

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

sim2: a.bin
	l32-sim -bin a.bin -d

clean:
	$(RM) -rf *.o a.out *.bin *.map *.s

PHONY: all dis clean size
