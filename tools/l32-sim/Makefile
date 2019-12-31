
CC = gcc

#OPT_LEVEL = -O3

CFLAGS += -Wall $(OPT_LEVEL)
LDFLAGS += -Wall $(OPT_LEVEL)

CFLAGS += -I ../l32-asm/include -I./

#EXE = .exe

BUILDDIR = ./build/

TARGET = l32-sim$(EXE)

SRCS = sim.c
SRCS += alu.c
SRCS += cctest.c
SRCS += isa.c
SRCS += disk0.c
SRCS += irq_ctrl.c
SRCS += display_ram.c
SRCS += tmr0.c
SRCS += rtc0.c
SRCS += mmu.c
SRCS += kbd.c
SRCS += systick.c
SRCS += random.c
SRCS += tlb.c
SRCS += fpu.c
SRCS += disassembly.c
SRCS += tapdev.c
SRCS += nic.c

OBJS += $(SRCS:%.c=$(BUILDDIR)%.o)

all: $(TARGET)

l32-sim$(EXE): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	cp $@ ../bin/
	
$(BUILDDIR)%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

install: $(TARGET)
	cp l32-sim$(EXE) /usr/local/bin

uninstall:
	$(RM) /usr/local/bin/l32-sim$(EXE)

tags:
	ctags -R

clean:
	$(RM) build/* *.o l32-sim$(EXE)
	make -C ./sdl_shm clean

distclean:
	$(RM) build/* *.o l32-sim$(EXE) tags
	make -C ./sdl_shm clean

.PHONY: tags run clean distclean

