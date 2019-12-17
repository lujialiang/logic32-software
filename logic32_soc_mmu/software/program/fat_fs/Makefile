
CC = lcc
AS = l32-asm

LIBC_ROOT = ../l32_libc

CFLAGS += -I. -I$(LIBC_ROOT)

# app
SRCS += main.c
SRCS += filesys.c

# FAT file system sources
SRCS += $(notdir fs/fat_access.c)
SRCS += $(notdir fs/fat_cache.c)
SRCS += $(notdir fs/fat_filelib.c)
SRCS += $(notdir fs/fat_format.c)
SRCS += $(notdir fs/fat_misc.c)
SRCS += $(notdir fs/fat_string.c)
SRCS += $(notdir fs/fat_table.c)
SRCS += $(notdir fs/fat_write.c)
vpath %.c ./fs
INCDIR += ./fs

# mmc/sd card driver source
SRCS += $(notdir disk.c)
vpath %.c ./dev
INCDIR += ./dev

OBJS = $(SRCS:%.c=$(BUILDDIR)%.o)

CFLAGS += $(addprefix -I,$(INCDIR))

all: a.out

a.out: crt0.o $(OBJS)
	$(CC) -o a.out $^ $(LIBC_ROOT)/libc.a

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
	l32-sim -p a.out -disk ../disk0.img

size: a.out
	l32-size a.out

tags:
	ctags -R

clean:
	$(RM) *.o a.out *.map

distclean:
	$(RM) *.o a.out tags *.map

.PHONY: all clean distclean dis sim tags size
