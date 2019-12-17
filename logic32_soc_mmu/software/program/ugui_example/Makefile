
CC = l32-cc
AS = l32-asm

LIBC_ROOT = ../l32_libc

CFLAGS += -I.
CFLAGS += -I$(LIBC_ROOT)

# libc
vpath %.c $(LIBC_ROOT)
SRCS += stdio.c
SRCS += vsprintf.c
SRCS += string.c

# app
SRCS += ugui.c
SRCS += main.c
SRCS += screen.c
SRCS += font.c

OBJS = $(SRCS:%.c=%.o)

all: a.out

a.out: crt0.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) -c $(CFLAGS) $<

%.s: %.c
	$(CC) $(CFLAGS) -S $<

%.o: %.s
	$(AS) $<

sim: a.out
	l32-sim -p a.out #-c 200

.PHONY: tags
tags:
	ctags -R

.PHONY: clean
clean:
	$(RM) -rf *.o a.out tags

.PHONY: all run clean
