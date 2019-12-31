#include <stdio.h>
#include <stdint.h>

#include "GUI.h"
#include "screen.h"

int mouse_x = 0;
int mouse_y = 0;

void delay(size_t n)
{
	while(n--);
}

void MainTask(void);

int main(void)
{
#if 0
	GUI_Init();
	GUI_DispString("Hello world!");
	printf("hello uc/gui 2.93\n");
	delay(5000000);
#else
    while(1) {
		MainTask();
    }
#endif
	return 0;
}
