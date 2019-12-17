PLAMFOR = L32_SOC
#PLAMFOR = LINUX

include header.mk
include source.mk
include app.mk

ifeq ($(PLAMFOR),LINUX)
CFILES += ./GUI_X/GUI_X_Touch.c
CFILES += ./GUI_X/GUI_X_pc.c
CFILES += ./pc_screen.c
CFILES += ./pc_main.c
else
CFILES += ./GUI_X/GUI_X_Touch.c
CFILES += ./GUI_X/GUI_X_uCOS.c
CFILES += ./l32_screen.c
CFILES += ./syscall.c
CFILES += ./uart.c
endif

INCLUDES = $(addprefix -I,$(dir $(HFILES)))

vpath %.c $(dir $(CFILES))

TARGET = a.out
BIN_TARGET = a.bin
BUILD_DIR = ./obj/

SRCS += $(notdir $(CFILES))

OBJS = $(SRCS:%.c=$(BUILD_DIR)%.o)

ifeq ($(PLAMFOR),LINUX)
CC = gcc
CFLAGS += -Wall -g  $(INCLUDES) -I.
CFLAGS += $(shell sdl-config --cflags --libs)
CFLAGS += -DSIM_PC
LDFLAGS += -lSDL
LDFLAGS += -lm
else
ifeq ($(PLAMFOR),L32_SOC)

CC = logic32-elf-gcc
AS = logic32-elf-as
LD = logic32-elf-ld
SIZE = logic32-elf-size
OBJCOPY = logic32-elf-objcopy
OBJDUMP = logic32-elf-objdump
SIM = l32-sim

LIBC_ROOT = ../l32_libc
CFLAGS += -I. -I$(LIBC_ROOT) $(INCLUDES)

ifdef OPT
CFLAGS += -O$(OPT)
endif

LDFLAGS += -nostartfiles
#LDFLAGS += -Tdata 0x10000

#CRTS = crt0.o
include ucosii.mk
OBJS += $(UCOS_OBJS)

#%.o: %.s
#	$(AS) $<

# Assembly source file
ASM_SRC += crt0.s
ASM_SRC += $(notdir ./uCOS-II/Ports/logic32/os_cpu_a.s)
vpath %.s ./uCOS-II/Ports/logic32
AOBJS = $(ASM_SRC:%.s=$(BUILD_DIR)%.o)

else
$(error "unkonk plamfor")
endif

endif

all: $(TARGET)

#$(TARGET): $(CRTS) $(OBJS)
#@echo linking $(TARGET) ... 
#@l32-link -M x.map -o $@ $^ $(LDFLAGS)

CFLAGS += -I ./uCOS-II/Ports/logic32

a.out: $(AOBJS) $(OBJS)
	@echo linking $(TARGET) ... 
	$(CC) $(LDFLAGS) -o a.out $^ $(LIBS) -Wl,-Map=your_file.map -lc -lm

a.bin: a.out
	$(OBJCOPY) -O binary $< $@

$(BUILD_DIR)%.o: %.c
	@echo compiling $< ...
	@$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)%.o: %.s
	@echo compiling $< ...
	$(AS) $< -o $@

%.s: %.c
	$(CC) $(CFLAGS) -S $<

ifeq ($(PLAMFOR),LINUX)
sim: $(TARGET)
	./$(TARGET)
else
sim_gui: $(BIN_TARGET)
	l32-sim -bin $(BIN_TARGET) -kbd-en -gui -rgb565 -w 320 -h 240 #-trace-en
sim: $(BIN_TARGET)
	l32-sim -bin $(BIN_TARGET) -kbd-en #-trace-en -trace-call
size: a.out
	$(SIZE) a.out
endif

tags:
	ctags -R
	
clean:
	$(RM) $(TARGET) $(BUILD_DIR)/*.o *~ a.out *.o tags x.map

.PHONY: all clean distclean sim tags size
