#include <stdio.h>
#include "_fifo.h"

int main(void)
{
	int32_t i;
	uint8_t dat;
	fifo_t fifo;
	fifo_t *pfifo;

	pfifo = &fifo;
	fifo_init(pfifo);
	for(i='0';i<='9';i++) {
		fifo_put(pfifo,i);
	}
	while(!fifo_get(pfifo,&dat)) {
		printf("%c ",dat);
	}
	putchar(10);
	return 0;
}
