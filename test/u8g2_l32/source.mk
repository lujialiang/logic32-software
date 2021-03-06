C_SRC += $(notdir u8g2_csrc/u8g2_bitmap.c)
C_SRC += $(notdir u8g2_csrc/u8g2_box.c)
C_SRC += $(notdir u8g2_csrc/u8g2_buffer.c)
C_SRC += $(notdir u8g2_csrc/u8g2_circle.c)
C_SRC += $(notdir u8g2_csrc/u8g2_cleardisplay.c)
C_SRC += $(notdir u8g2_csrc/u8g2_font.c)
C_SRC += $(notdir u8g2_csrc/u8g2_fonts.c)
C_SRC += $(notdir u8g2_csrc/u8g2_hvline.c)
C_SRC += $(notdir u8g2_csrc/u8g2_input_value.c)
C_SRC += $(notdir u8g2_csrc/u8g2_intersection.c)
C_SRC += $(notdir u8g2_csrc/u8g2_kerning.c)
C_SRC += $(notdir u8g2_csrc/u8g2_line.c)
C_SRC += $(notdir u8g2_csrc/u8g2_ll_hvline.c)
C_SRC += $(notdir u8g2_csrc/u8g2_message.c)
C_SRC += $(notdir u8g2_csrc/u8g2_polygon.c)
C_SRC += $(notdir u8g2_csrc/u8g2_selection_list.c)
C_SRC += $(notdir u8g2_csrc/u8g2_setup.c)
C_SRC += $(notdir u8g2_csrc/u8log.c)
C_SRC += $(notdir u8g2_csrc/u8log_u8g2.c)
C_SRC += $(notdir u8g2_csrc/u8log_u8x8.c)
C_SRC += $(notdir u8g2_csrc/u8x8_8x8.c)
C_SRC += $(notdir u8g2_csrc/u8x8_byte.c)
C_SRC += $(notdir u8g2_csrc/u8x8_cad.c)
C_SRC += $(notdir u8g2_csrc/u8x8_capture.c)
#C_SRC += $(notdir u8g2_csrc/u8x8_debounce.c)
C_SRC += $(notdir u8g2_csrc/u8x8_display.c)
C_SRC += $(notdir u8g2_csrc/u8x8_d_ssd1322.c)
C_SRC += $(notdir u8g2_csrc/u8x8_fonts.c)
C_SRC += $(notdir u8g2_csrc/u8x8_gpio.c)
C_SRC += $(notdir u8g2_csrc/u8x8_input_value.c)
C_SRC += $(notdir u8g2_csrc/u8x8_message.c)
C_SRC += $(notdir u8g2_csrc/u8x8_selection_list.c)
C_SRC += $(notdir u8g2_csrc/u8x8_setup.c)
C_SRC += $(notdir u8g2_csrc/u8x8_string.c)
C_SRC += $(notdir u8g2_csrc/u8x8_u16toa.c)
C_SRC += $(notdir u8g2_csrc/u8x8_u8toa.c)

vpath %.c ./u8g2_csrc

#CFLAGS += -I./u8g2_csrc
