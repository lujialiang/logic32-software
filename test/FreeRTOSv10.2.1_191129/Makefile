
CC = logic32-elf-gcc
AS = logic32-elf-as
LD = logic32-elf-ld
SIZE = logic32-elf-size
OBJCOPY = logic32-elf-objcopy
OBJDUMP = logic32-elf-objdump
SIM = l32-sim

BUILD_DIR=build/

# app
C_SRC += $(notdir ./main.c)
C_SRC += $(notdir ./uart.c)

#C_SRC += $(notdir ./FreeRTOS/croutine.c)
#C_SRC += $(notdir ./FreeRTOS/event_groups.c)
C_SRC += $(notdir ./FreeRTOS/list.c)
C_SRC += $(notdir ./FreeRTOS/queue.c)
#C_SRC += $(notdir ./FreeRTOS/stream_buffer.c)
C_SRC += $(notdir ./FreeRTOS/tasks.c)
#C_SRC += $(notdir ./FreeRTOS/timers.c)

C_SRC += $(notdir ./FreeRTOS/portable/GCC/logic32/port.c)
C_SRC += $(notdir ./FreeRTOS/portable/MemMang/heap_4.c)

vpath %.c ./FreeRTOS
vpath %.c ./FreeRTOS/portable/GCC/logic32
vpath %.c ./FreeRTOS/portable/MemMang

INCDIR += ./.
INCDIR += ./FreeRTOS/include
INCDIR += ./FreeRTOS/portable/GCC/logic32

CFLAGS += $(addprefix -I,$(INCDIR))

COBJS = $(C_SRC:%.c=$(BUILD_DIR)%.o)

# Assembly source file
ASM_SRC += crt0.s
ASM_SRC += $(notdir ./FreeRTOS/portable/GCC/logic32/port_asm.s)
vpath %.s ./FreeRTOS/portable/GCC/logic32
AOBJS = $(ASM_SRC:%.s=$(BUILD_DIR)%.o)

ifdef OPT
CFLAGS += -O$(OPT)
endif

LDFLAGS += -nostartfiles
LDFLAGS += -Tdata 0x10000

.PHONY: all
all: a.bin

a.out: $(AOBJS) $(COBJS)
	$(CC) $(LDFLAGS) -o a.out $^ $(LIBS) -Wl,-Map=your_file.map -lc

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

