
#include <ucos_ii.h>
#include <stdio.h>

void sei();
void cli();
void halt();

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

void save_sp(OS_STK *p)
{
	OSIntNesting++;
	if( OSIntNesting == 1 ) {
		OSTCBCur->OSTCBStkPtr = p;
	}
}
