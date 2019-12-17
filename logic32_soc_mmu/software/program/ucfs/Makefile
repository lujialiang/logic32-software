CC = lcc
AS = l32-asm

mmc_en = 1

BUILDIR  = ./obj
BUILDIR2 = ./obj/

vpath %.c ./FSL/fat
CFILES += $(notdir ./FSL/fat/fat_dir.c)
CFILES += $(notdir ./FSL/fat/fat_open.c)
CFILES += $(notdir ./FSL/fat/fat_data.c)
CFILES += $(notdir ./FSL/fat/fat_in.c)
CFILES += $(notdir ./FSL/fat/fat_out.c)
CFILES += $(notdir ./FSL/fat/fat_misc.c)
CFILES += $(notdir ./FSL/fat/fat_ioct.c)

vpath %.c ./API
CFILES += $(notdir ./API/fs_info.c)
CFILES += $(notdir ./API/api_misc.c)
CFILES += $(notdir ./API/api_dir.c)
CFILES += $(notdir ./API/api_out.c)
CFILES += $(notdir ./API/api_in.c)

vpath %.c ./CLIB
CFILES += $(notdir ./CLIB/clibmisc.c)

vpath %.c ./LBL
CFILES += $(notdir ./LBL/lb_misc.c)

vpath %.c ./sample
CFILES += $(notdir ./sample/main.c)

ifeq ($ramdist_en),1)
vpath %.c ./DEVICE/ram
CFILES += $(notdir ./DEVICE/ram/r_misc.c)
endif

#CFILES += ./DEVICE/windrive/wd_misc.c

ifeq ($(mmc_en),1)
vpath %.c ./DEVICE/smc
CFILES += $(notdir ./DEVICE/smc/smc_log.c)
CFILES += $(notdir ./DEVICE/smc/smc_ecc.c)
CFILES += $(notdir ./DEVICE/smc/smc_phy.c)

vpath  %.c ./DEVICE/smc/hardware/EP7312
INCLUDE += -I./DEVICE/smc/hardware/EP7312
CFILES += $(notdir ./DEVICE/smc/hardware/EP7312/smc_X_hw.c)
endif

ifeq ($(ide_en),1)
vpath %.c ./DEVICE/IDE
CFILES += $(notdir ./DEVICE/IDE/ide_drv.c)
CFILES += $(notdir ./DEVICE/IDE/hardware/sed137xe_v1r0/ide_X_hw.c)
CFILES += $(notdir ./DEVICE/IDE/hardware/EP7312/ide_X_hw.c)
endif

vpath %.c ./OS
CFILES += $(notdir ./OS/fs_x_no_os.c)
#CFILES += $(notdir ./OS/FS_X_win32.c)
#CFILES += $(notdir ./OS/fs_x_embos.c)
#CFILES += $(notdir ./OS/fs_x_ucos_ii.c)

COBJS = $(CFILES:%.c=$(BUILDIR2)%.o)

CFILES += mmc_drv.c
CFILES += disk.c

INCLUDE += -I./FSL/fat
INCLUDE += -I./API

#./CONFIG/EP7312/fs_conf.h
#./CONFIG/EP7312/fs_port.h
#./CONFIG/M16C_137X_IP/fs_conf.h
#./CONFIG/M16C_137X_IP/fs_port.h

#./CONFIG/Win32/fs_conf.h
#./CONFIG/Win32/fs_port.h
INCLUDE += -I./CONFIG/Win32

INCLUDE += -I./CLIB
INCLUDE += -I./LBL

ifeq ($(mmc_en),1)
INCLUDE += -I./DEVICE/smc
endif

#./DEVICE/smc/hardware/EP7312/smc_X_hw.h
#./DEVICE/IDE/hardware/sed137xe_v1r0/ide_X_hw.h
#./DEVICE/IDE/hardware/EP7312/ide_X_hw.h

ifeq ($(ide_en),1)
INCLUDE += -I./DEVICE/IDE
endif
INCLUDE += -I./OS

CFLAGS += $(INCLUDE)

LIBC_ROOT = ../l32_libc
CFLAGS += -I. -I$(LIBC_ROOT)

AOBJS += crt0.o

all: a.out

a.out: crt0.o $(COBJS)
	$(CC) -o a.out $^ $(LIBC_ROOT)/libc.a

$(BUILDIR2)%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.o: %.s
	$(AS) $<

sim: a.out
	l32-sim -p a.out -disk disk0.img

tags:
	ctags -R

clean:
	$(RM) *.o obj/*.o a.out

.PHONY: all clean tags
