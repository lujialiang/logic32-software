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
CFLAGS += -I./u8g2_csrc
CFLAGS += -DU8G2_16BIT

include source.mk

# app
C_SRC += main.c
C_SRC += uart.c
C_SRC += syscall.c
C_SRC += l32_screen.c
C_SRC += u8x8_d_sdl_128x64.c

OBJS = $(C_SRC:%.c=%.o)

all: a.out

a.out: $(OBJS)
	$(CC) -OS -o a.out $^ -lc

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
	l32-sim -kbd-en -gui -rgb565 -w 128 -h 64 -bin a.bin

simd: a.bin
	l32-sim -kbd-en -bin a.bin -mp -c 50 -d

size: a.out
	$(SIZE) a.out

tags:
	ctags -R

clean:
	$(RM) *.o a.out *.map *.bin tags

distclean:
	$(RM) *.o a.out tags *.map tags

.PHONY: all clean distclean dis sim tags size
