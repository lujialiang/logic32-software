LIBC_ROOT = ../l32_libc
LIBS += $(LIBC_ROOT)/libc.a

CC = lcc
AS = l32-asm
LD = l32-link
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

#C_SRC += $(notdir ./uCOS-II/Source/ucos_ii.c)
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

INCDIR += ./APP
INCDIR += ./USER
INCDIR += ./BSP
INCDIR += ./uCOS-II/Source
INCDIR += ./uCOS-II/Ports/logic32

CFLAGS += $(addprefix -I,$(INCDIR))

COBJS = $(C_SRC:%.c=$(BUILD_DIR)%.o)

# Assembly source file
ASM_SRC += crt0.s
ASM_SRC += $(notdir ./uCOS-II/Ports/logic32/os_cpu_a.s)
vpath %.s ./uCOS-II/Ports/logic32
AOBJS = $(ASM_SRC:%.s=$(BUILD_DIR)%.o)

STM_COMMON=/opt/STM32F4-Discovery_FW_V1.1.0

CFLAGS += -I.

.PHONY: all
all: a.out

a.out: $(AOBJS) $(COBJS)
	$(CC) -M a.map -o a.out $^ $(LIBS)

$(BUILD_DIR)%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)%.o: %.s
	l32-asm $< -o $@

%.s: %.c
	$(CC) $(CFLAGS) -S $<

.PHONY: tags
tags:
	ctags -R

.PHONY: sim
sim: a.out
	l32-sim -p a.out -kbd-en

.PHONY: dis
dis: a.out
	l32-dis a.out

.PHONY: clean
clean:
	rm -f $(BUILD_DIR)*.o a.out a.map *.o tags

