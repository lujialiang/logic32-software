CC=gcc
CFLAGS=-g
LDFLAGS=-g
OBJS=cpp.o lex.o nlist.o tokens.o macro.o eval.o \
	include.o hideset.o unix.o

lcc-cpp: $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $(OBJS)
	cp $@ ../../bin

$(OBJS): cpp.h

install: lcc-cpp
	cp lcc-cpp /usr/local/bin

clean:
		rm -f *.o core

clobber: clean
		rm -f lcc-cpp *.out

.PHONY: install clean clobber
