CROSS_TOOL = logic32-elf-
CC = $(CROSS_TOOL)gcc
AS = $(CROSS_TOOL)as
OBJDUJP = $(CROSS_TOOL)objdump
OBJCOPY = $(CROSS_TOOL)objcopy
SIZE = $(CROSS_TOOL)size

ifdef OPT
CFLAGS += -O$(OPT)
endif

SRCS += $(notdir main.c)
SRCS += $(notdir uart.c)
SRCS += $(notdir l32_screen.c)
SRCS += $(notdir syscall.c)
SRCS += $(notdir sbrk.c)

SRCS += $(notdir lvgl/src/lv_objx/lv_table.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_led.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_canvas.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_kb.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_tabview.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_cont.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_cb.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_objx_templ.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_chart.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_mbox.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_tileview.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_bar.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_imgbtn.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_btnm.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_sw.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_lmeter.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_line.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_arc.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_calendar.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_win.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_btn.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_img.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_spinbox.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_label.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_ddlist.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_slider.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_gauge.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_page.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_roller.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_preload.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_list.c)
SRCS += $(notdir lvgl/src/lv_objx/lv_ta.c)
SRCS += $(notdir lvgl/src/lv_misc/lv_txt.c)
SRCS += $(notdir lvgl/src/lv_misc/lv_log.c)
SRCS += $(notdir lvgl/src/lv_misc/lv_templ.c)
SRCS += $(notdir lvgl/src/lv_misc/lv_utils.c)
SRCS += $(notdir lvgl/src/lv_misc/lv_color.c)
SRCS += $(notdir lvgl/src/lv_misc/lv_ll.c)
SRCS += $(notdir lvgl/src/lv_misc/lv_area.c)
SRCS += $(notdir lvgl/src/lv_misc/lv_math.c)
SRCS += $(notdir lvgl/src/lv_misc/lv_circ.c)
SRCS += $(notdir lvgl/src/lv_misc/lv_fs.c)
SRCS += $(notdir lvgl/src/lv_misc/lv_anim.c)
SRCS += $(notdir lvgl/src/lv_misc/lv_task.c)
SRCS += $(notdir lvgl/src/lv_misc/lv_async.c)
SRCS += $(notdir lvgl/src/lv_misc/lv_gc.c)
SRCS += $(notdir lvgl/src/lv_misc/lv_mem.c)
SRCS += $(notdir lvgl/src/lv_themes/lv_theme_nemo.c)
SRCS += $(notdir lvgl/src/lv_themes/lv_theme_default.c)
SRCS += $(notdir lvgl/src/lv_themes/lv_theme_material.c)
SRCS += $(notdir lvgl/src/lv_themes/lv_theme_templ.c)
SRCS += $(notdir lvgl/src/lv_themes/lv_theme_alien.c)
SRCS += $(notdir lvgl/src/lv_themes/lv_theme.c)
SRCS += $(notdir lvgl/src/lv_themes/lv_theme_mono.c)
SRCS += $(notdir lvgl/src/lv_themes/lv_theme_night.c)
SRCS += $(notdir lvgl/src/lv_themes/lv_theme_zen.c)
SRCS += $(notdir lvgl/src/lv_draw/lv_draw.c)
SRCS += $(notdir lvgl/src/lv_draw/lv_img_decoder.c)
SRCS += $(notdir lvgl/src/lv_draw/lv_draw_arc.c)
SRCS += $(notdir lvgl/src/lv_draw/lv_draw_rect.c)
SRCS += $(notdir lvgl/src/lv_draw/lv_draw_basic.c)
SRCS += $(notdir lvgl/src/lv_draw/lv_draw_line.c)
SRCS += $(notdir lvgl/src/lv_draw/lv_draw_label.c)
SRCS += $(notdir lvgl/src/lv_draw/lv_draw_img.c)
SRCS += $(notdir lvgl/src/lv_draw/lv_img_cache.c)
SRCS += $(notdir lvgl/src/lv_draw/lv_draw_triangle.c)
SRCS += $(notdir lvgl/src/lv_core/lv_group.c)
SRCS += $(notdir lvgl/src/lv_core/lv_obj.c)
SRCS += $(notdir lvgl/src/lv_core/lv_style.c)
SRCS += $(notdir lvgl/src/lv_core/lv_indev.c)
SRCS += $(notdir lvgl/src/lv_core/lv_refr.c)
SRCS += $(notdir lvgl/src/lv_core/lv_disp.c)
SRCS += $(notdir lvgl/src/lv_hal/lv_hal_indev.c)
SRCS += $(notdir lvgl/src/lv_hal/lv_hal_tick.c)
SRCS += $(notdir lvgl/src/lv_hal/lv_hal_disp.c)
SRCS += $(notdir lvgl/src/lv_font/lv_font_roboto_28.c)
SRCS += $(notdir lvgl/src/lv_font/lv_font_roboto_12.c)
SRCS += $(notdir lvgl/src/lv_font/lv_font_fmt_txt.c)
SRCS += $(notdir lvgl/src/lv_font/lv_font.c)
SRCS += $(notdir lvgl/src/lv_font/lv_font_roboto_16.c)
SRCS += $(notdir lvgl/src/lv_font/lv_font_unscii_8.c)
SRCS += $(notdir lvgl/src/lv_font/lv_font_roboto_22.c)

vpath %.c ./lvgl/src/lv_objx/
vpath %.c ./lvgl/src/lv_misc/
vpath %.c ./lvgl/src/lv_themes/
vpath %.c ./lvgl/src/lv_draw/
vpath %.c ./lvgl/src/lv_core/
vpath %.c ./lvgl/src/lv_hal/
vpath %.c ./lvgl/src/lv_font/

INCLUDE += ./lvgl/src/lv_objx/
INCLUDE += ./lvgl/src/lv_misc/
INCLUDE += ./lvgl/src/lv_themes/
INCLUDE += ./lvgl/src/lv_draw/
INCLUDE += ./lvgl/src/lv_core/
INCLUDE += ./lvgl/src/lv_hal/
INCLUDE += ./lvgl/src/lv_font/

CFLAGS += $(addprefix -I,$(INCDIR))
CFLAGS += -I.

BUILD_DIR = ./build/

OBJS = $(SRCS:%.c=$(BUILD_DIR)%.o)

all: a.out

a.out: $(OBJS)
	$(CC) $(LDFLAGS) -o a.out $(OBJS) -lc -Wl,-Map=a.map

$(BUILD_DIR)%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.s: %.c
	$(CC) $(CFLAGS) -S -o $@ $<

tags:
	ctags -R

dis: a.out
	$(OBJDUJP) -d $<

size: a.out
	$(SIZE) $<

a.bin: a.out
	$(OBJCOPY) -O binary a.out a.bin

sim: a.bin
	l32-sim -kbd-en -gui -w 128 -h 160 -bin a.bin

simd: a.bin
	l32-sim -kbd-en -bin a.bin -d -c 800

clean:
	$(RM) -rf *.o a.out *.bin *.map
	$(RM) -rf sim $(OBJS) tags
	$(RM) -rf $(BUILD_DIR)*.o

.PHONY: all dis clean size tags

