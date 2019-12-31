
CC = lcc
AS = l32-asm

CFLAGS = -I.

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

a.out: crt0.o $(OBJS)
	$(CC) -o a.out $^

a.out.map: $(OBJS)
	l32-link -o a.out -M a.out.map $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.s
	$(AS) $<

dis: a.out
	l32-dis a.out

sim: a.out
	l32-sim -p a.out -disk ../disk0.img

#disk0.img:
#dd if=/dev/zero of=disk0.img bs=512 count=2880

tags:
	ctags *.c *.h

clean:
	$(RM) *.o a.out tags

.PHONY: all clean dis sim tags
