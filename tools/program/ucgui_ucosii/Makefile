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
#CFILES += ./l32_main.c
endif

INCLUDES = $(addprefix -I,$(dir $(HFILES)))

vpath %.c $(dir $(CFILES))

TARGET = a.out
BUILD_DIR = ./obj

SRCS += $(notdir $(CFILES))

OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

ifeq ($(PLAMFOR),LINUX)
CC = gcc
CFLAGS += -Wall -g  $(INCLUDES) -I.
CFLAGS += $(shell sdl-config --cflags --libs)
CFLAGS += -DSIM_PC
LDFLAGS += -lSDL
LDFLAGS += -lm
else
ifeq ($(PLAMFOR),L32_SOC)

CC = lcc
AS = l32-asm
LIBC_ROOT = ../l32_libc
LIBS += $(LIBC_ROOT)/libc.a
LIBS += $(LIBC_ROOT)/libm.a
CFLAGS += -I. -I$(LIBC_ROOT) $(INCLUDES)
LDFLAGS += $(LIBS)

CRTS = crt0.o
include ucosii.mk
OBJS += $(UCOS_OBJS)

%.o: %.s
	$(AS) $<

else
$(error "unkonk plamfor")
endif

endif

all: $(TARGET)

#$(CC) -o $@ $^ $(LDFLAGS)

$(TARGET): $(CRTS) $(OBJS)
	@echo linking $(TARGET) ... 
	@l32-link -M x.map -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c
	@echo compiling $< ...
	@$(CC) $(CFLAGS) -c $< -o $@

ifeq ($(PLAMFOR),LINUX)
sim: $(TARGET)
	./$(TARGET)
else
sim_gui: $(TARGET)
	l32-sim -p a.out -kbd-en -gui -rgb565 -w 320 -h 240 #-trace-en
sim: $(TARGET)
	l32-sim -p a.out -kbd-en #-trace-en -trace-call
size:
	l32-size a.out
endif

tags:
	ctags -R
	
clean:
	$(RM) $(TARGET) $(BUILD_DIR)/*.o *~ a.out *.o x.map

distclean:
	$(RM) $(TARGET) $(BUILD_DIR)/*.o *~ a.out *.o tags x.map

.PHONY: all clean distclean sim tags size
