
#include <ucos_ii.h>
#include <lib_def.h>

#include <stdio.h>

void sei();
void cli();
void halt();

/*
 *	PSW
 *	PC
 *	$31
 *	...
 *	$1
 *	sp
 */

OS_STK *OSTaskStkInit (void (*task)(void *p_arg), void *p_arg, OS_STK *ptos, INT16U opt)
{
    OS_STK *p_stk;
	
	//printf("OSTaskStkInit(), task=0x%x, stack=0x%x\n", task, ptos);

    (void)opt;
	/* Load stack pointer */
    p_stk      = ptos + 1u;
	/* Align the stack to 8-bytes. */
    p_stk      = (OS_STK *)((OS_STK)(p_stk) & 0xFFFFFFF8u);
	/* Registers stacked as if auto-saved on exception */
	/* xPSR */
    *(--p_stk) = (OS_STK)0x00000010uL;
	/* Entry Point */
    *(--p_stk) = (OS_STK)task;
	/* R31 (LR) */
    *(--p_stk) = (OS_STK)OS_TaskReturn;
	/* R30 ~ R0 */
	{
		int i;
		for(i=30;i>=0;i--) {
			*(--p_stk) = (OS_STK)i;
		}
	}
	//printf("#2 p_stk = 0x%x\n", (unsigned)p_stk);
    return (p_stk);
}

void panic(void)
{
	cli();
	puts("panic: unknow error");
	halt();
}

void panic_undef_inst(void)
{
	cli();
	puts("\npanic: undefined instruction!!!\n");
	halt();
}

void panic_div_by_zero(void)
{
	cli();
	puts("\npanic: div by zero!!!\n");
	halt();
}

void dump_stk(unsigned *p)
{
	int i;
	unsigned d;
	unsigned *t = p;
	putchar(10);
	for(i=0;i<32;i++) {
		d = *p;
		printf("p[%d] [%p] = %x\n", i, p, d);
		p++;
	}
	putchar(10);
	for(;i<35;i++) {
		d = *p;
		printf("p[%d] [%p] = %x\n", i, p, d);
		p++;
	}
	putchar(10);
}

void OS_CPU_SysTickHandler(void)
{
    OS_CPU_SR cpu_sr;
    OS_ENTER_CRITICAL();/* Tell uC/OS-II that we are starting an ISR          */
    OSIntNesting++;
    OS_EXIT_CRITICAL();
    OSTimeTick();		/* Call uC/OS-II's OSTimeTick()                       */
    OSIntExit();       /* Tell uC/OS-II that we are leaving the ISR          */
}

void save_sp(OS_STK *p)
{
	OSIntNesting++;
	if( OSIntNesting == 1 ) {
		OSTCBCur->OSTCBStkPtr = p;
	}
}
