
#include <stdio.h>

void seti(void);
void disi(void);

void delay(unsigned int n)
{
	while(n) {
		n--;
	}
}

int main(void)
{
	int i = 0;
	puts("init0");
	seti();
	while(1) {
		printf("hello world! %d\n",i++);
		delay(100000);
	}
	return 0;
}
