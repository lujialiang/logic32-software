LIBC_ROOT = ../libc

CC = logic32-elf-gcc
AS = logic32-elf-as
LD = logic32-elf-ld
SIZE = logic32-elf-size
OBJCOPY = logic32-elf-objcopy
OBJDUMP = logic32-elf-objdump
SIM = l32-sim

CFLAGS += -I. -I$(LIBC_ROOT)

ifdef OPT
CFLAGS += -O$(OPT)
endif

# app
C_SRC += main.c

BUILD_DIR=build/

C_SRC += $(notdir ./main.c)

C_SRC += $(notdir ./libc/_fifo.c)
C_SRC += $(notdir ./libc/ctype.c)
C_SRC += $(notdir ./libc/errno.c)
C_SRC += $(notdir ./libc/heap.c)
C_SRC += $(notdir ./libc/printf.c)
C_SRC += $(notdir ./libc/soc_init.c)
C_SRC += $(notdir ./libc/stdio.c)
C_SRC += $(notdir ./libc/stdlib.c)
C_SRC += $(notdir ./libc/string.c)
C_SRC += $(notdir ./libc/vsprintf.c)

vpath %.c ./libc

INCDIR += ./libc

CFLAGS += $(addprefix -I,$(INCDIR))

COBJS = $(C_SRC:%.c=$(BUILD_DIR)%.o)

# Assembly source file
ASM_SRC += crt0.s
AOBJS = $(ASM_SRC:%.s=$(BUILD_DIR)%.o)

CFLAGS += -I.
LDFLAGS += -nostartfiles

.PHONY: all
all: a.bin

a.out: $(AOBJS) $(COBJS)
	$(CC) $(LDFLAGS) -o a.out $^ $(LIBS) -Wl,-Map=a.map

a.bin: a.out
	$(OBJCOPY) -O binary $< $@

size: a.out
	$(SIZE) $<

$(BUILD_DIR)%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)%.o: %.s
	$(AS) $< -o $@

%.s: %.c
	$(CC) $(CFLAGS) -S $<

.PHONY: tags
tags:
	ctags -R

.PHONY: sim
sim: a.bin
	l32-sim -bin a.bin -kbd-en

.PHONY: simd
simd: a.bin
	l32-sim -bin a.bin -kbd-en -c 300 -d

.PHONY: dis
dis: a.out
	$(OBJDUMP) -d a.out

.PHONY: clean
clean:
	$(RM) -rf $(BUILD_DIR)*.o a.out *.map *.o tags *.bin

