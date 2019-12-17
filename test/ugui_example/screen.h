#ifndef _SCREEN_H_
#define _SCREEN_H_

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


//#define RGB565_MODE

#ifdef RGB565_MODE
typedef uint16_t pixel_t;
#else
typedef uint32_t pixel_t;
#endif

extern pixel_t *screen_start;
extern pixel_t *screen_point;

pixel_t get_pixel(int x, int y);
void put_pixel(pixel_t color);
void put_pixel_ex(int x, int y,pixel_t color);
void goto_xy(int x, int y);
void screen_init(void);
void screen_reflash(void);
void screen_exit(void);
void screen_wait(void);

#endif // _SCREEN_H_
