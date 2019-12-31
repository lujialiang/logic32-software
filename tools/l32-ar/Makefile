
CC = gcc

CFLAGS += -DCROSS
CFLAGS += -Werror -Wall

SRCS = append.c ar.c archive.c contents.c delete.c extract.c misc.c \
       move.c print.c replace.c strmode.c

OBJS = append.o ar.o archive.o contents.o delete.o extract.o \
       misc.o move.o print.o replace.o strmode.o

all: l32-ar

l32-ar: ${OBJS}
	${CC} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}
	cp $@ ../bin

tags:
	ctags -R

clean:
	rm -f *.o l32-ar

distclean:
	rm -f *.o l32-ar tags

install: all
	cp l32-ar /usr/local/bin/

.PHONY: all clean tags install
