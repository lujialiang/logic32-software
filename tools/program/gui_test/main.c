
#include <stdio.h>
#include <string.h>

#define SCREEN_WIDTH 320
#define SCREEN_HIGHT 240

int main(void)
{
	int cnt = 0;
	while(cnt < 24*10)
	{
		int i;
		int *p;
		//printf("cnt = %d\n", ++cnt);
		p = (int *)(1<<25);
		//puts("red");
		for(i=0;i<SCREEN_WIDTH*SCREEN_HIGHT;i++) {
			*p++ = 0xff0000;
		}
		p = (int *)(1<<25);
		//puts("green");
		for(i=0;i<SCREEN_WIDTH*SCREEN_HIGHT;i++) {
			*p++ = 0x00ff00;
		}
		p = (int *)(1<<25);
		//puts("blue");
		for(i=0;i<SCREEN_WIDTH*SCREEN_HIGHT;i++) {
			*p++ = 0x0000ff;
		}
	}
	return 0;
}
