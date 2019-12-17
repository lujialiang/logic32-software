CROSS_TOOL = logic32-elf-
CC = $(CROSS_TOOL)gcc
AS = $(CROSS_TOOL)as
OBJDUMP = $(CROSS_TOOL)objdump
OBJCOPY = $(CROSS_TOOL)objcopy
SIZE = $(CROSS_TOOL)size

ifdef OPT
CFLAGS += -O$(OPT)
endif

CFLAGS += -I. 

# app
SRCS += main.c
SRCS += uart.c
SRCS += syscall.c

OBJS = $(SRCS:%.c=%.o)

all: a.out

a.out: $(OBJS)
	$(CC) -o a.out $^ -lc

a.out.map: $(OBJS)
	l32-link -o a.out -M a.out.map $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.s
	$(AS) $<

%.s: %.c
	$(CC) $(CFLAGS) -S $<

dis: a.out
	$(OBJDUMP) -d a.out

a.bin: a.out
	$(OBJCOPY) -O binary a.out a.bin

sim: a.bin
	l32-sim -kbd-en -gui -w 320 -h 240 -bin a.bin

simd: a.bin
	l32-sim -kbd-en -bin a.bin -mp -c 50 -d

size: a.out
	$(SIZE) a.out

tags:
	ctags -R

clean:
	$(RM) *.o a.out *.map *.bin

distclean:
	$(RM) *.o a.out tags *.map

.PHONY: all clean distclean dis sim tags size
