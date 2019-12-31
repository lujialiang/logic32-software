
#include "led.h"
#include <stdio.h>

void LED_GPIO_Config(void)
{
}

void LED(int led, int onoff)
{
	if( onoff ) {
		printf("led%d ON\n", led);
	}
	else {
		printf("led%d OFF\n", led);
	}
}

