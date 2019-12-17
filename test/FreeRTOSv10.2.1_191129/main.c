/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "uart.h"

#define mainTINY_STACK	256
void vTest(void *pvParameters);
void vTest2(void *pvParameters);

/*-----------------------------------------------------------*/

/*
 * Create all the demo tasks - then start the scheduler.
 */
int main (void) 
{
	uart_puts("hello 123");
	/* When re-starting a debug session (rather than cold booting) we want
	to ensure the installed interrupt handlers do not execute until after the
	scheduler has been started. */
	portDISABLE_INTERRUPTS();

	xTaskCreate( vTest, "TST1", mainTINY_STACK, ( void * ) 10, tskIDLE_PRIORITY, NULL );
	xTaskCreate( vTest2, "TST2", mainTINY_STACK, ( void * ) 10, tskIDLE_PRIORITY, NULL );

	/* Finally start the scheduler. */
	vTaskStartScheduler();

	/* Should not get here as the processor is now under control of the 
	scheduler! */

   	return 0;
}

void vTest(void *pvParameters)
{
	const portTickType xDelay = 10 / portTICK_RATE_MS;
	
	for(;;)
	{
		uart_putc('A');
		vTaskDelay( xDelay );
	}
}

void vTest2(void *pvParameters)
{
	const portTickType xDelay = 10 / portTICK_RATE_MS;
	
	for(;;)
	{
		uart_putc('B');
		vTaskDelay( xDelay );
	}
}
