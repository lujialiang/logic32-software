
CC = lcc
AS = l32-asm

CFLAGS = -I.

SRCS += main.c
SRCS += stdio.c
SRCS += vsprintf.c
SRCS += string.c
SRCS += isr.c

OBJS = $(SRCS:%.c=%.o)

all: a.out

a.out: crt0.o $(OBJS)
	$(CC) -o a.out $^

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
	l32-sim -p a.out -kbd-en

tags:
	ctags *.c *.h

clean:
	$(RM) *.o a.out tags

.PHONY: all clean dis sim tags
