#include <stdio.h>

int main(void)
{
	char *p = malloc(1);
	if ( p == NULL ) {
		uart_puts("error");
	}
	else {
		uart_puts("hello world!\n");
	}
	return 0;
}

