
#include <stdio.h>

#include "includes.h"

static OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];

void sei(void);
void cli(void);
void swi(void);

void Task_Key(void *p_arg);
static OS_STK key_task_stk[KEYBOARD_TASK_STK_SIZE];

int main(void)
{
  	BSP_Init();
	OSInit();
	OSTaskCreate(Task_LED,(void *)0,
	   &startup_task_stk[STARTUP_TASK_STK_SIZE-1], STARTUP_TASK_PRIO);

	OSTaskCreate(Task_Key,(void *)0,
	   &key_task_stk[KEYBOARD_TASK_STK_SIZE-1], KEYBOARD_TASK_PRIO);

	OSStart();
    return 0;
 }

