#ifndef _SCREEN_H_INCLUDE_
#define _SCREEN_H_INCLUDE_

#include <stdint.h>
#include <stdbool.h>

#define DEFAULT_SCREEN_WIDTH	320
#define DEFAULT_SCREEN_HEIGHT	240

extern int screen_width;
extern int screen_height;

extern uint32_t *screen_start;
extern uint32_t *screen_point;

uint32_t get_pixel(uint32_t x,uint32_t y);
void put_pixel(uint32_t color);
void put_pixel_ex(uint32_t x,uint32_t y,uint32_t color);
void goto_xy(uint32_t x,uint32_t y);
void screen_init(void);
void screen_reflash(void);
void screen_exit(void);
void screen_wait(void);

#endif
