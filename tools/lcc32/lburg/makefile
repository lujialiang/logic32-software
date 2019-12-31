CC=gcc
CFLAGS=-g
LDFLAGS=-g
YFLAGS=
OBJS=lburg.o gram.o

lburg: lburg.o gram.o
		$(CC) -o lburg $(LDFLAGS) $(OBJS)

$(OBJS): lburg.h

clean:
		rm -f *.o core

clobber: clean
		rm -f y.tab.c gram.c lburg

.PHONY: clean clobber
