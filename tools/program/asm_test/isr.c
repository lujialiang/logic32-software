
#include <stdio.h>

static tick_cnt = 0;

void systick_isr(void)
{
	printf("systick isr!, cnt = %d\n", tick_cnt++);
}

