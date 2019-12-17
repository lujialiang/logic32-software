
#include <stdio.h>
#include <string.h>

#define SCREEN_WIDTH 320
#define SCREEN_HIGHT 240

int main(void)
{
	char buffer[128];
	int cnt = 0;
	while(1)	//cnt < 24*10)
	{
		int i;
		int *p;
		sprintf(buffer, "cnt = %d\n", ++cnt);
		uart_puts(buffer);
		p = (int *)(1<<25);
		uart_puts("red");
		for(i=0;i<SCREEN_WIDTH*SCREEN_HIGHT;i++) {
			*p++ = 0xff0000;
		}
		p = (int *)(1<<25);
		uart_puts("green");
		for(i=0;i<SCREEN_WIDTH*SCREEN_HIGHT;i++) {
			*p++ = 0x00ff00;
		}
		p = (int *)(1<<25);
		uart_puts("blue");
		for(i=0;i<SCREEN_WIDTH*SCREEN_HIGHT;i++) {
			*p++ = 0x0000ff;
		}
	}
	return 0;
}
