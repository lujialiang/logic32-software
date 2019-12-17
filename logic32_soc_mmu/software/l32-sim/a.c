#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define SCREEN_X_MAX 320
#define SCREEN_Y_MAX 240

#define BUF_SIZE (SCREEN_X_MAX*SCREEN_Y_MAX*4)
#define MYKEY 25

uint32_t disp_buffer[BUF_SIZE/4];

int main(int argc, char* argv[])
{
	int shmid;
	uint32_t *shmptr;
	if( (shmid = shmget(MYKEY, BUF_SIZE, IPC_CREAT|0666)) == -1 ) {
		printf("shmget error\n");
		exit(1);
	}
	if( (shmptr = shmat(shmid, 0, 0)) == (void *)-1 ) {
		printf("shmat error\n");
		exit(1);
	}
	int i, j, k;
	uint32_t rgb[3] = { 0xff0000, 0x00ff00, 0x0000ff };
	char *strs[3] = { "red", "green", "blue" };
	while(1) {
		for(k=0; k<3; k++) {
			puts(strs[k]);
			for(i=0;i<SCREEN_Y_MAX;i++) {
				for(j=0;j<SCREEN_X_MAX;j++) {
					//shmptr[SCREEN_X_MAX*i+j] = rgb[k];
					disp_buffer[SCREEN_X_MAX*i+j] = rgb[k];
				}
			}
			memcpy(shmptr, disp_buffer, BUF_SIZE);
			//usleep(1000*500);
			usleep(1000*(1000/24));
		}
	}
	return 0;
}
