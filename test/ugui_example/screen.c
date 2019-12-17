#include "screen.h"

uint32_t *screen_start;

void put_pixel(uint32_t x,uint32_t y,uint32_t color) {
	x %= SCREEN_WIDTH;
	y %= SCREEN_HEIGHT;
	//*(uint32_t*)(screen_start + y*SCREEN_WIDTH + x) = color;
} 

uint32_t get_pixel(uint32_t x,uint32_t y) {
	x %= SCREEN_WIDTH;
	y %= SCREEN_HEIGHT;
	//return *(uint32_t*)(screen_start + y*SCREEN_WIDTH + x);
	return 0;
}

int screen_init(void)
{
    return TRUE;
}

void screen_exit(void)
{
}
