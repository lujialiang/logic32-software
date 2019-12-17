#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <stdio.h>
#include <stdbool.h>

//#include "gui.h"
#include "screen.h"

volatile bool quit = false;
SDL_Event event;

int event_thread(void *par);
void handle_sdl_events(void);

SDL_Thread *thread0 = NULL;
SDL_Thread *thread1 = NULL;

SDL_sem *MouseSem = NULL;

int mouse_x = 0, mouse_y = 0;

void MainTask(void);

int gui_thread(void *par)
{
    while(!quit) {
		MainTask();
    }
    return 0;
}

int mouse_thread(void *par)
{
    while(!quit) {
        SDL_SemWait(MouseSem);
		GUI_TOUCH_Exec();
    }
    return 0;
}

int main(int argc, char** argv)
{
	quit = false;
    screen_init();
	
    MouseSem = SDL_CreateSemaphore(0);
    
	thread0 = SDL_CreateThread( gui_thread, NULL );
	thread1 = SDL_CreateThread( mouse_thread, NULL );
	
    //While the user hasn't quit
    while( quit == false )
    {
        //While there's an event to handle
        while( SDL_PollEvent( &event ) )
        {
            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
            handle_sdl_events();
        }
    }

	SDL_KillThread( thread0 );
	SDL_KillThread( thread1 );
    SDL_DestroySemaphore( MouseSem );
	SDL_Quit();
    return 0;
}

void SendEvent(int x, int y)
{			
	mouse_x = x;
	mouse_y = y;
	SDL_SemPost(MouseSem);
	SDL_Delay(10);
	SDL_SemPost(MouseSem);
	SDL_Delay(10);
}
void handle_sdl_events(void)
{
	static int key_pressed = 0;
    //If the mouse moved
    if( event.type == SDL_MOUSEMOTION )
    {
		if( key_pressed ) {
			SendEvent(event.button.x, event.button.y);
		}
		else {			
			SendEvent(800,800);
		}
    }
    //If a mouse button was pressed
    if( event.type == SDL_MOUSEBUTTONDOWN )
    {
        //If the left mouse button was pressed
        if( event.button.button == SDL_BUTTON_LEFT )
        {
			key_pressed = 1;
			SendEvent(event.button.x, event.button.y);
			SendEvent(event.button.x, event.button.y);
        }
    }
    //If a mouse button was released
    if( event.type == SDL_MOUSEBUTTONUP )
    {
        //If the left mouse button was released
        if( event.button.button == SDL_BUTTON_LEFT )
        {
			key_pressed = 0;
			SendEvent(800,800);
        }
    }
}

