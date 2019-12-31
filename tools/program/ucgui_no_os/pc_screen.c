#include <SDL/SDL.h>
#include "screen.h"

uint32_t *screen_start;
uint32_t *screen_point;

static SDL_Surface* hello = NULL;
static SDL_Surface* screen = NULL;

void put_pixel_ex(uint32_t x,uint32_t y,uint32_t color) {
	x %= SCREEN_X_MAX;
	y %= SCREEN_Y_MAX;
	*(uint32_t*)(screen_start + y*SCREEN_X_MAX + x) = color;
}

void put_pixel(uint32_t color) {
	*screen_point = color;
}

uint32_t get_pixel(uint32_t x,uint32_t y) {
	x %= SCREEN_X_MAX;
	y %= SCREEN_Y_MAX;
	return *(uint32_t*)(screen_start + y*SCREEN_X_MAX + x);
}

void goto_xy(uint32_t x,uint32_t y) {
	x %= SCREEN_X_MAX;
	y %= SCREEN_Y_MAX;
	screen_point = (uint32_t*)(screen_start + y*SCREEN_X_MAX + x);
}

SDL_Thread *thread = 0;
int reflash_thread( void *data )
{
	SDL_Event e;
	while( 1 )
	{
		screen_reflash();
		SDL_Delay( 1 );
#if 0		
		if( SDL_PollEvent(&e) != 0 ) {
			if( e.type == SDL_QUIT ) {
				exit(1);
			}
		}
#endif		
	}
	return 0;	 
}

void screen_init(void)
{
    //The images
    SDL_Init( SDL_INIT_EVERYTHING );
    //Set up screen
    screen = SDL_SetVideoMode( SCREEN_X_MAX, SCREEN_Y_MAX, 32, SDL_SWSURFACE );
	screen_start = (uint32_t*)screen->pixels;
	screen_point = screen_start;
	// Create reflash theread
	thread = SDL_CreateThread( reflash_thread, 0 );
	
}
void screen_reflash(void) {
	SDL_Flip( screen );
}

void screen_wait(void) {
#if 0
	SDL_Delay(2000);
#else
	int quit = 0;
	SDL_Event e;
	while(!quit) {
		while( SDL_PollEvent(&e) != 0 ) {
			if( e.type == SDL_QUIT ) {
				quit = 1;
			}
		}
	}
#endif
}

void screen_exit(void) {
    //Free memory
    SDL_FreeSurface( hello );
    //Quit SDL
    SDL_Quit();
}
