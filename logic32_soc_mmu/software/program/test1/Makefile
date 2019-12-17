
CC = lcc
AS = l32-asm

LIBC_ROOT = ../l32_libc

CFLAGS += -I. -I$(LIBC_ROOT)

# libc
vpath %.c $(LIBC_ROOT)
SRCS += stdio.c
SRCS += vsprintf.c
SRCS += string.c

# app
SRCS += main.c
SRCS += foo1.c
SRCS += foo.c

OBJS = $(SRCS:%.c=%.o)

all: a.out

a.out: crt0.o $(OBJS)
	#$(CC) -o a.out $^
	l32-link -data 0x1000 -o a.out $^

a.out.map: $(OBJS)
	l32-link -o a.out -M a.out.map $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.s
	$(AS) $<

%.s: %.c
	$(CC) $(CFLAGS) -S $<

dis: a.out
	l32-dis a.out

sim: a.out
	#l32-sim -gui -w 640 -h 480 -p a.out
	l32-sim -gui -w 320 -h 240 -p a.out

tags:
	ctags *.c *.h

clean:
	$(RM) *.o a.out tags *.map

.PHONY: all clean dis sim tags
