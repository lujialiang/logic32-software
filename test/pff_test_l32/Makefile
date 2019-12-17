
CROSS_TOOL = logic32-elf-
CC = $(CROSS_TOOL)gcc
AS = $(CROSS_TOOL)as
OBJDUJP = $(CROSS_TOOL)objdump
OBJCOPY = $(CROSS_TOOL)objcopy
SIZE = $(CROSS_TOOL)size

ifdef OPT
CFLAGS += -O$(OPT)
endif

CFLAGS += -I.

LDFLAGS += -Tdata 0x10000

#SRCS += main.c
SRCS += stdio.c
SRCS += stdlib.c
SRCS += vsprintf.c
SRCS += string.c

SRCS += main.c
SRCS += mmcbbp.c
SRCS += pff.c
SRCS += disk.c

OBJS = $(SRCS:%.c=%.o)

all: a.out

a.out: $(OBJS)
	$(CC) $(LDFLAGS) -o a.out $^

a.out.map: $(OBJS)
	l32-link -o a.out -M a.out.map $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.s: %.c
	$(CC) $(CFLAGS) -S -o $@ $<

%.o: %.s
	$(AS) $<

dis: a.out
	$(OBJDUJP) -d $<

size: a.out
	$(SIZE) $<

a.bin: a.out
	$(OBJCOPY) -O binary a.out a.bin

sim: a.bin
	l32-sim -bin a.bin -mp -disk ./disk0.img

simd: a.bin
	l32-sim -bin a.bin -mp -disk ./disk0.img -d

#disk0.img:
#dd if=/dev/zero of=disk0.img bs=512 count=2880

tags:
	ctags *.c *.h

clean:
	$(RM) *.o a.out tags a.bin

.PHONY: all clean dis sim tags
