
CC = l32-cc
AS = l32-asm

LIBC_ROOT = ../l32_libc

CFLAGS += -I. -I$(LIBC_ROOT)

# app
SRCS += main.c

OBJS = $(SRCS:%.c=%.o)

all: a.out

a.out: crt0.o $(OBJS)
	$(CC) -o a.out $^ #$(LIBC_ROOT)/libc.a

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
	l32-sim -kbd-en -gui -w 320 -h 240 -p a.out

size: a.out
	l32-size a.out

tags:
	ctags -R

clean:
	$(RM) *.o a.out *.map

distclean:
	$(RM) *.o a.out tags *.map

.PHONY: all clean distclean dis sim tags size
