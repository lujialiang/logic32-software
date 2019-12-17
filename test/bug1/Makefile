CROSS_TOOL = logic32-elf-
CC = $(CROSS_TOOL)gcc
AS = $(CROSS_TOOL)as
OBJDUJP = $(CROSS_TOOL)objdump
OBJCOPY = $(CROSS_TOOL)objcopy
SIZE = $(CROSS_TOOL)size

ifdef OPT
CFLAGS += -O$(OPT)
endif

CSRS += main.c
CSRS += syscall.c
CSRS += uart.c

OBJS = $(CSRS:%.c=%.o)

all: a.out

a.out: $(OBJS)
	$(CC) $(LDFLAGS) -o a.out $(OBJS) -lc -lm -Wl,-Map=a.map

a.bin: a.out
	$(OBJCOPY) -O binary a.out a.bin

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.s: %.c
	$(CC) $(CFLAGS) -S -o $@ $<

dis: a.out
	$(OBJDUJP) -d $<

size: a.out
	$(SIZE) $<

sim: a.bin
	l32-sim -bin a.bin

simd: a.bin
	l32-sim -bin a.bin -d

clean:
	$(RM) -rf *.o a.out *.bin *.map *.s

PHONY: all dis clean size
