#ifndef _SCREEN_H__
#define _SCREEN_H__

#include <stdint.h>

#ifndef FALSE
	#define FALSE 0
#endif //FALSE

#ifndef TRUE
	#define TRUE 1
#endif //TRUE

//Screen attributes
#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 320
#define SCREEN_BPP    32

extern uint32_t *screen_start;

void put_pixel(uint32_t x,uint32_t y,uint32_t color);

uint32_t get_pixel(uint32_t x,uint32_t y);

int screen_init(void);

void screen_exit(void);

#endif //_SCREEN_H__
