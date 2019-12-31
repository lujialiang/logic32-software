CROSS_TOOL = logic32-elf-
CC = $(CROSS_TOOL)gcc
AS = $(CROSS_TOOL)as
OBJDUMP = $(CROSS_TOOL)objdump
OBJCOPY = $(CROSS_TOOL)objcopy
SIZE = $(CROSS_TOOL)size

ifdef OPT
CFLAGS += -O$(OPT)
endif

LCD_W = 320
LCD_H = 240

CFLAGS += -I. 
CFLAGS += -I./u8g2_csrc

CFLAGS += -D U8G2_USE_LARGE_FONTS
CFLAGS += -D LCD_WIDTH=$(LCD_W) -D LCD_HEIGHT=$(LCD_H)

include source.mk

# app
C_SRC += main.c
C_SRC += uart.c
C_SRC += syscall.c
C_SRC += screen.c
C_SRC += u8x8_d_sdl_128x64.c

OBJS = $(C_SRC:%.c=%.o)

all: a.out

a.out: $(OBJS)
	$(CC) -OS -o a.out $^ -lc

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
	l32-sim -kbd-en -gui -rgb565 -w $(LCD_W) -h $(LCD_H) -bin a.bin

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
