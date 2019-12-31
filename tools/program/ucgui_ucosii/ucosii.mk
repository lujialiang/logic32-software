
# app
SRCS += port.c

#BUILD_DIR=build/

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

UCOS_OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)


