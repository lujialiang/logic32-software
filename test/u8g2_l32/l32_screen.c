
#include "screen.h"

pixel_t *screen_start = (pixel_t *)(1<<25);
pixel_t *screen_point;

void put_pixel_ex(int x, int y, pixel_t color)
{
	//x %= SCREEN_X_MAX;
	//y %= SCREEN_Y_MAX;
	*(pixel_t*)(screen_start + y*SCREEN_X_MAX + x) = color;
}

void put_pixel(pixel_t color)
{
	*screen_point = color;
}

pixel_t get_pixel(int x, int y)
{
	//x %= SCREEN_X_MAX;
	//y %= SCREEN_Y_MAX;
	return *(pixel_t *)(screen_start + y*SCREEN_X_MAX + x);
}

void goto_xy(int x, int y)
{
	//x %= SCREEN_X_MAX;
	//y %= SCREEN_Y_MAX;
	screen_point = (pixel_t*)(screen_start + y*SCREEN_X_MAX + x);
}

void screen_init(void)
{
	screen_start = (pixel_t *)(1<<25);
	screen_point = screen_start;
}

