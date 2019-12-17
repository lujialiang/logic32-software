
#include <stdio.h>

#include "includes.h"
#include "GUI.h"

void led_task(void *p_arg)
{
	INT8U err_code;
	INT16U os_version;
    (void)p_arg;
	os_version = OSVersion();
	printf("Start UCOS-II %d.%d.%d\n",\
		os_version/10000, (os_version%10000)/100, os_version%100);
    while (1)
    {
        LED(1, ON);
        if( (err_code = OSTimeDlyHMSM(0, 0,0,100)) != OS_ERR_NONE ) {
			printf("#1 OSTimeDlyHMSM() error %d\n", err_code);
			while(1);
		}

        LED(1, OFF);
        if( (err_code = OSTimeDlyHMSM(0, 0,0,100)) != OS_ERR_NONE ) {
			printf("#2 OSTimeDlyHMSM() error %d\n", err_code);
			while(1);
		}
    }
}

void key_task(void *p_arg)
{
	while(1) {
		GUI_TOUCH_Exec();
		OSTimeDly(10);
	}
}

void MainTask(void);

void gui_task(void *p_arg)
{
	MainTask();
    while(1);
}
