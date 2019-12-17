LIBC_ROOT = ../l32_libc

CC = logic32-elf-gcc
AS = logic32-elf-as
LD = logic32-elf-ld
SIZE = logic32-elf-size
OBJCOPY = logic32-elf-objcopy
OBJDUMP = logic32-elf-objdump
SIM = l32-sim

CFLAGS += -I. -I$(LIBC_ROOT)

# app
C_SRC += misc.c

BUILD_DIR=build/

C_SRC += $(notdir ./USER/main.c)
C_SRC += $(notdir ./USER/hooks.c)

C_SRC += $(notdir ./BSP/BSP.c)
C_SRC += $(notdir ./BSP/led.c)
C_SRC += $(notdir ./APP/app.c)
C_SRC += $(notdir ./USER/main.c)

C_SRC += $(notdir ./l32_libc/_fifo.c)
C_SRC += $(notdir ./l32_libc/ctype.c)
C_SRC += $(notdir ./l32_libc/errno.c)
C_SRC += $(notdir ./l32_libc/heap.c)
C_SRC += $(notdir ./l32_libc/printf.c)
C_SRC += $(notdir ./l32_libc/soc_init.c)
C_SRC += $(notdir ./l32_libc/stdio.c)
C_SRC += $(notdir ./l32_libc/stdlib.c)
C_SRC += $(notdir ./l32_libc/string.c)
C_SRC += $(notdir ./l32_libc/vsprintf.c)

C_SRC += $(notdir ./uCOS-II/Source/os_core.c)
C_SRC += $(notdir ./uCOS-II/Source/os_q.c)
C_SRC += $(notdir ./uCOS-II/Source/os_flag.c)
C_SRC += $(notdir ./uCOS-II/Source/os_sem.c)
C_SRC += $(notdir ./uCOS-II/Source/os_tmr.c)
C_SRC += $(notdir ./uCOS-II/Source/os_task.c)
C_SRC += $(notdir ./uCOS-II/Source/os_time.c)
C_SRC += $(notdir ./uCOS-II/Source/os_mbox.c)
C_SRC += $(notdir ./uCOS-II/Source/os_mem.c)
C_SRC += $(notdir ./uCOS-II/Source/os_mutex.c)

C_SRC += $(notdir ./uCOS-II/Ports/logic32/os_cpu_c.c)
C_SRC += $(notdir ./uCOS-II/Ports/logic32/os_dbg.c)

vpath %.c ./APP ./USER ./BSP
vpath %.c ./uCOS-II/Source
vpath %.c ./uCOS-II/Ports/logic32
vpath %.c ./l32_libc

INCDIR += ./APP
INCDIR += ./USER
INCDIR += ./BSP
INCDIR += ./uCOS-II/Source
INCDIR += ./uCOS-II/Ports/logic32
INCDIR += ./l32_libc

CFLAGS += $(addprefix -I,$(INCDIR))

COBJS = $(C_SRC:%.c=$(BUILD_DIR)%.o)

# Assembly source file
ASM_SRC += crt0.s
ASM_SRC += $(notdir ./uCOS-II/Ports/logic32/os_cpu_a.s)
vpath %.s ./uCOS-II/Ports/logic32
AOBJS = $(ASM_SRC:%.s=$(BUILD_DIR)%.o)

STM_COMMON=/opt/STM32F4-Discovery_FW_V1.1.0

CFLAGS += -I.

ifdef OPT
CFLAGS += -O$(OPT)
endif

LDFLAGS += -nostartfiles
LDFLAGS += -Tdata 0x10000

.PHONY: all
all: a.bin

a.out: $(AOBJS) $(COBJS)
	$(CC) $(LDFLAGS) -o a.out $^ $(LIBS) -Wl,-Map=your_file.map

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
	l32-sim -bin a.bin -kbd-en -mp

.PHONY: simd
simd: a.bin
	l32-sim -bin a.bin -kbd-en -mp -d

.PHONY: dis
dis: a.out
	$(OBJDUMP) -d a.out

.PHONY: clean
clean:
	$(RM) -rf $(BUILD_DIR)*.o a.out *.map *.o tags *.bin

