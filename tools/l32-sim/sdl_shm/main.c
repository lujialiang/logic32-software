#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL/SDL.h>  
#include <errno.h>

#include "screen.h"

bool rgb565_mode = false;

static uint32_t rgb565_to_rgb24(uint32_t rgb)
{
	uint8_t rgb24[3];
	rgb24[0] = (( rgb & 0xf800) >> 8) & 0xff;
	rgb24[1] = (( rgb & 0x07e0) >> 3) & 0xff;
	rgb24[2] = (( rgb & 0x001f) << 3) & 0xff;
	return (rgb24[2]<<16 | rgb24[1] << 8 | rgb24[0]);
}

#define BUF_SIZE (1<<25)
#define BUF_SIZE2 (SCREEN_X_MAX*SCREEN_Y_MAX*4)

#define MYKEY 29

// msg
#define MSG_KEY	1234
#define MSG_BUFFER_SIZE	(sizeof(int)*2)
struct msg_st
{
	long int msg_type;
	int buffer[2];
};
int msg_init_done = false;
int msg_id = -1;
//

SDL_Event event;

int mouse_x = 0;
int mouse_y = 0;

void SendEvent(int x, int y)
{
	struct msg_st data;
	mouse_x = x;
	mouse_y = y;
	//printf("x = %d, y = %d\n", mouse_x, mouse_y);
	data.msg_type = 1;    //注意2
	//向队列发送数据
	data.buffer[0] = x;
	data.buffer[1] = y;
	if(msgsnd(msg_id, (void*)&data, MSG_BUFFER_SIZE, 0) == -1)
	{
		//fprintf(stderr, "msgsnd failed\n");
		//exit(EXIT_FAILURE);
	}
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

uint32_t *display_buffer = NULL;

int main(int argc, char* argv[])
{
	int i;
	int shmid;
	uint8_t *shmptr;
	struct msg_st data;
	msg_id = -1;
	msg_init_done = false;
	rgb565_mode = false;
	display_buffer = NULL;
#ifdef USE_FILE_LOCK
	int lock_fd = open("/tmp/sdl_gui_lock_file.lock", O_RDONLY | O_CREAT, 0444);
	if( flock(lock_fd, LOCK_EX | LOCK_NB) < 0 ) {
		if( errno == 11 ) {
			fprintf(stderr, "error: program already running\n");
		}
		else {
			fprintf(stderr, "%s\n", strerror(errno));
		}
		return -1;
	}
#endif
	for(i=1; i<argc; i++) {
		if( strcmp("-w", argv[i]) == 0 ) {
			if( i == argc - 1 ) {
				fprintf(stderr, "argument error\n");	
				return -1;
			}
			i++;
			screen_width = atoi(argv[i]);
		}
		else if( strcmp("-h", argv[i]) == 0 ) {
			if( i == argc - 1 ) {
				fprintf(stderr, "argument error\n");	
				return -1;
			}
			i++;
			screen_height = atoi(argv[i]);
		}
		else if( strcmp("-rgb565", argv[i]) == 0 ) {
			rgb565_mode = true;
		}
	}
	if( rgb565_mode ) {
		display_buffer = (uint32_t *)malloc(screen_width*screen_height*4);
		if( display_buffer == NULL ) {
			fprintf(stderr, "malloc(): %s\n", strerror(errno));
			exit(1);
		}
	}
	if( (shmid = shmget(MYKEY, BUF_SIZE, IPC_CREAT|0666)) == -1 ) {
		printf("shmget error\n");
		exit(1);
	}
	if( (shmptr = shmat(shmid, 0, 0)) == (void *)-1 ) {
		printf("shmat error\n");
		exit(1);
	}
	// init msg
	msg_id = msgget((key_t)MSG_KEY, 0666 | IPC_CREAT);
	msg_init_done = true;
	if(msg_id == -1)
	{
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		msg_init_done = false;
	}
	// init screen
	screen_init();
	// main loop
	bool quit = false;
	while(!quit) {
		if( rgb565_mode == true ) {
			int w, h;
			int index;
			uint16_t *piexl_ptr = (uint16_t *)shmptr; 
			uint32_t *p = display_buffer;
			for( h=0; h<screen_height; h++) {
				for( w=0; w<screen_width; w++) {
					*p++  = rgb565_to_rgb24( *piexl_ptr++ );
				}
			}
			memcpy(screen_start, (void*)display_buffer, screen_width*screen_height*4);
		}
		else {
			memcpy(screen_start, (void*)shmptr, screen_width*screen_height*4);
		}
		screen_reflash();
		usleep(20000);
		while( SDL_PollEvent(&event) != 0 ) {
			if( event.type == SDL_QUIT ) {
				SendEvent(-1, -1);
				quit = true;
			}
			handle_sdl_events();
		}
	}
	// deinit screen
	screen_exit();
	// deinit msg
	if( msg_init_done ) {
		if(msgctl(msg_id, IPC_RMID, 0) == -1)
		{
			//fprintf(stderr, "msgctl(IPC_RMID) failed\n");
			return -1;
		}
	}
	if( rgb565_mode ) {
		free(display_buffer);
	}
#ifdef USE_FILE_LOCK
	close(lock_fd);
#endif
	return 0;
}
