
CC = lcc
AS = l32-asm
DIS = l32-dis
SIM = l32-sim
SIZE = l32-size

LIBC_ROOT = ../l32_libc
CFLAGS += -I. -I$(LIBC_ROOT)

# app
SRCS += main.c

OBJS = $(SRCS:%.c=%.o)

all: a.out

a.out: crt0.o $(OBJS)
	$(CC) -o a.out $^ $(LIBC_ROOT)/libc.a

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.s
	$(AS) $<

%.s: %.c
	$(CC) $(CFLAGS) -S $<

dis: a.out
	$(DIS) $<

sim: a.out
	$(SIM) -p a.out -nic-en -kbd-en

size: a.out
	$(SIZE) $<

tags:
	ctags -R

clean:
	$(RM) *.o a.out *.map

distclean:
	$(RM) *.o a.out tags *.map

.PHONY: all clean distclean dis sim tags size
