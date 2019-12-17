
#include "screen.h"

pixel_t *screen_start = (pixel_t *)(1<<25);
pixel_t *screen_point;

void put_pixel_ex(int x, int y, pixel_t color)
{
	*(pixel_t*)(screen_start + y*SCREEN_WIDTH + x) = color;
}

void put_pixel(pixel_t color)
{
	*screen_point = color;
}

pixel_t get_pixel(int x, int y)
{
	return *(pixel_t *)(screen_start + y*SCREEN_WIDTH + x);
}

void goto_xy(int x, int y)
{
	screen_point = (pixel_t*)(screen_start + y*SCREEN_WIDTH + x);
}

void screen_init(void)
{
	screen_start = (pixel_t *)(1<<25);
	screen_point = screen_start;
}

