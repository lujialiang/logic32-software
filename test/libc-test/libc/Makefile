
CC = lcc
AS = l32-asm
AR = l32-ar

CFLAGS += -I.

LIBC_SRCS += errno.c
LIBC_SRCS += heap.c
LIBC_SRCS += printf.c
LIBC_SRCS += stdio.c
LIBC_SRCS += stdlib.c
LIBC_SRCS += string.c
LIBC_SRCS += ctype.c
LIBC_SRCS += soc_init.c

LIBC_OBJS = $(LIBC_SRCS:%.c=%.o)
LIBC_OBJS += _setjmp.o	# _setjmp.s

all: libc.a

libc.a: $(LIBC_OBJS)
	$(AR) cru libc.a $^
	$(RM) *.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.s
	$(AS) $<

%.s: %.c
	$(CC) $(CFLAGS) -S $<

tags:
	ctags *.c *.h

clean:
	$(RM) *.o tags *.map libc.a

.PHONY: all clean tags
