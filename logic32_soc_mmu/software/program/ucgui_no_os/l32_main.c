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

void sei(void);
void MainTask(void);

void systick_init(void)
{
	unsigned int *p = (unsigned int *)0x83000000;
	p[1] = 0x4000;	// reload
	p[0] = 3;		// ctrl
}

int main(void)
{
	systick_init();
	sei();
    while(1) {
		MainTask();
    }
	return 0;
}
