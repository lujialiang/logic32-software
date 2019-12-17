
#include <stdio.h>

#include "includes.h"

static OS_STK gui_task_stk[GUI_TASK_STK_SIZE];
void gui_task(void *p_arg);

static OS_STK led_task_stk[LED_TASK_STK_SIZE];
void led_task(void *p_arg);

static OS_STK keyboard_task_stk[KEYBOARD_TASK_STK_SIZE];
void key_task(void *p_arg);

int main(void)
{
  	BSP_Init();
	OSInit();
	#if 0
	OSTaskCreate(led_task,(void *)0,
	   &led_task_stk[LED_TASK_STK_SIZE-1], LED_TASK_PRIO);
	#endif

	#if 1
	OSTaskCreate(key_task,(void *)0,
	   &keyboard_task_stk[KEYBOARD_TASK_STK_SIZE-1], KEYBOARD_TASK_PRIO);
	#endif

	OSTaskCreate(gui_task,(void *)0,
	   &gui_task_stk[GUI_TASK_STK_SIZE-1], GUI_TASK_PRIO);

	OSStart();
    return 0;
 }

