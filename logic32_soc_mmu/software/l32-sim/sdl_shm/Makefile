
CC = gcc
CFLAGS = $(shell sdl-config --cflags --libs)

TARGET = sdl_shm

SRCS += main.c
SRCS += screen.c

OBJS = $(SRCS:%.c=%.o)

all: $(TARGET)

a: a.c
	gcc -o a a.c

$(TARGET): $(OBJS)

	$(CC) $^ $(CFLAGS) -o $@ -lrt

run: $(TARGET)
	./$<

.PHONY: install
install: sdl_shm
	cp sdl_shm /usr/local/bin

%.o: %.c
	$(CC) -c $(CFLAGS) $<

.PHONY: tags
tags:
	ctags *.c *.h

.PHONY: clean
clean:
	$(RM) -rf *.o $(TARGET) tags

.PHONY: all run clean
