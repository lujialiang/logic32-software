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
LDFLAGS += -lg

# app
SRCS += ugui.c
SRCS += main.c
SRCS += l32_screen.c
SRCS += font.c
SRCS += syscall.c
SRCS += uart.c

OBJS = $(SRCS:%.c=%.o)

all: a.out

a.out: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

a.bin: a.out
	$(OBJCOPY) -O binary a.out a.bin

%.o: %.c
	$(CC) -c $(CFLAGS) $<

%.s: %.c
	$(CC) $(CFLAGS) -S $<

%.o: %.s
	$(AS) $<

#sim: a.bin
#	l32-sim -bin a.bin #-c 200

sim: a.bin
	l32-sim -kbd-en -gui -bin a.bin

#	l32-sim -kbd-en -gui -w 128 -h 160 -bin a.bin


.PHONY: tags
tags:
	ctags -R

.PHONY: clean
clean:
	$(RM) -rf *.o a.out tags a.bin

.PHONY: all run clean
