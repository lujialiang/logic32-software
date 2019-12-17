LIBC_ROOT = ../l32_libc

CC = lcc
AS = l32-asm
LD = l32-link
SIM = l32-sim

CFLAGS += -I. -I$(LIBC_ROOT)

# libc
vpath %.c $(LIBC_ROOT)
SRCS += stdio.c
SRCS += vsprintf.c
SRCS += string.c

# app
SRCS += port.c

BUILD_DIR=build/
PROJ_NAME=$(BUILD_DIR)ucos_test

SRCS += $(notdir ./USER/main.c)
SRCS += $(notdir ./USER/hooks.c)

SRCS += $(notdir ./BSP/BSP.c)
SRCS += $(notdir ./BSP/led.c)
SRCS += $(notdir ./APP/app.c)
SRCS += $(notdir ./USER/main.c)

#SRCS += $(notdir ./uCOS-II/Source/ucos_ii.c)
SRCS += $(notdir ./uCOS-II/Source/os_core.c)
SRCS += $(notdir ./uCOS-II/Source/os_q.c)
SRCS += $(notdir ./uCOS-II/Source/os_flag.c)
SRCS += $(notdir ./uCOS-II/Source/os_sem.c)
SRCS += $(notdir ./uCOS-II/Source/os_tmr.c)
SRCS += $(notdir ./uCOS-II/Source/os_task.c)
SRCS += $(notdir ./uCOS-II/Source/os_time.c)
SRCS += $(notdir ./uCOS-II/Source/os_mbox.c)
SRCS += $(notdir ./uCOS-II/Source/os_mem.c)
SRCS += $(notdir ./uCOS-II/Source/os_mutex.c)

SRCS += $(notdir ./uCOS-II/Ports/ARM-Cortex-M4/Generic/GNU/os_cpu_c.c)
#SRCS += $(notdir ./uCOS-II/Ports/ARM-Cortex-M4/Generic/GNU/os_dbg.c)

vpath %.c ./APP ./USER ./BSP
vpath %.c ./uCOS-II/Source
vpath %.c ./uCOS-II/Ports/ARM-Cortex-M4/Generic/GNU

INCDIR += ./APP
INCDIR += ./USER
INCDIR += ./BSP
INCDIR += ./uCOS-II/Source
INCDIR += ./uCOS-II/Ports/ARM-Cortex-M4/Generic/GNU

CFLAGS += $(addprefix -I,$(INCDIR))

OBJS = $(SRCS:%.c=$(BUILD_DIR)%.o)


STM_COMMON=/opt/STM32F4-Discovery_FW_V1.1.0

CFLAGS += -I.

.PHONY: all
all: a.out

a.out: crt0.o $(OBJS)
	$(CC) -M a.map -o a.out $^

$(BUILD_DIR)%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.o: %.s
	l32-asm $<

%.s: %.c
	$(CC) $(CFLAGS) -S $<

.PHONY: tags
tags:
	ctags -R

.PHONY: sim
sim: a.out
	#l32-sim -p a.out 2>/dev/null
	l32-sim -p a.out 

.PHONY: dis
dis: a.out
	l32-dis a.out

.PHONY: clean
clean:
	rm -f $(BUILD_DIR)*.o
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
	rm -f tags
	rm -f a.out a.map *.o

