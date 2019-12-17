/*
 * FreeRTOS Kernel V10.2.1
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the NIOS2 port.
 *----------------------------------------------------------*/

/* Standard Includes. */
#include <string.h>
#include <errno.h>

/* logic32 includes. */
#include "soc.h"
#include "uart.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Interrupts are enabled. */
#define portINITIAL_ESTATUS     ( StackType_t ) 0x00000010uL 

/*-----------------------------------------------------------*/

/* 
 * Setup the timer to generate the tick interrupts.
 */
static void prvSetupTimerInterrupt( void );

/*-----------------------------------------------------------*/
/* 
 * See header file for description. 
 */
void TaskExitError(void)
{
	uart_puts("exit error");
	asm volatile ("halt");
}

StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters )
{    
    StackType_t *p_stk;
	int i;

	/* Load stack pointer */
    p_stk      = pxTopOfStack - 7u;
	/* Align the stack to 8-bytes. */
    p_stk      = (StackType_t *)((StackType_t)(p_stk) & 0xFFFFFFF8u);
	/* Registers stacked as if auto-saved on exception */
	--p_stk;
	/* PSW */
    *(--p_stk) = (StackType_t)portINITIAL_ESTATUS;
	/* Entry Point */
    *(--p_stk) = (StackType_t)pxCode;
	/* R31 (LR) */
    *(--p_stk) = (StackType_t)TaskExitError;
	/* R30 ~ R0 */
	for (i=30;i>=0;i--) {
		if( i== 4 ) {
			*(--p_stk) = (StackType_t)pvParameters;
		}
		else {
			*(--p_stk) = (StackType_t)i;
		}
	}
    return (p_stk);
}

/*-----------------------------------------------------------*/

/* 
 * See header file for description. 
 */
BaseType_t xPortStartScheduler( void )
{
	/* Start the timer that generates the tick ISR.  Interrupts are disabled
	here already. */
	prvSetupTimerInterrupt();
	
	/* Start the first task. */
    extern void OSStartHighRdy(void);
    OSStartHighRdy();

	/* Should not get here! */
	return 0;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
	/* It is unlikely that the NIOS2 port will require this function as there
	is nothing to return to.  */
}
/*-----------------------------------------------------------*/

/*
 * Setup the systick timer to generate the tick interrupts at the required
 * frequency.
 */
void prvSetupTimerInterrupt( void )
{
    *(volatile unsigned int *)ADDR_SYSTICK_RELOAD = 0x40000;
    *(volatile unsigned int *)ADDR_SYSTICK_CTRL = 3;
}

/*-----------------------------------------------------------*/
