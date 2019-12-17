CC = gcc
CFLAGS += -Wall -I ./include -I./
CFLAGS += -Werror
LDFLAGS += -Wall
LDFLAGS += -Werror

SRCS += hash.c
SRCS += link.c
SRCS += misc.c
SRCS += archive.c

OBJS = $(SRCS:%.c=$(BUILDDIR)%.o)

BUILDDIR = ./build/

TARGETS = l32-link

all: $(TARGETS)

l32-link: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^
	cp $@ ../bin

$(BUILDDIR)%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

install: $(TARGETS)
	cp $(TARGETS) /usr/local/bin/l32-link

uninstall:
	$(RM) /usr/local/bin/l32-link

tags:
	ctags -R

clean:
	$(RM) $(BUILDDIR)*.o *.o $(TARGETS)

distclean:
	$(RM) $(BUILDDIR)*.o *.o $(TARGETS) tags

.PHONY: all install uninstall tags run clean distclean
