#include <stdio.h>

char buffer[1024];

int main(void)
{
#if 0
	char *p;
	int cnt = 0;

	while (1) {
		cnt++;
		p = malloc(1024);
		//free(p);
		if ( p == NULL ) {
			sprintf(buffer, "error, cnt: %d", cnt);
			uart_puts(buffer);
			return 1;
		}
		else {
			sprintf(buffer, "p = 0x%08x, cnt: %d", p, cnt);
			uart_puts(buffer);
		}
	}

#else
	int i;
	for (i=0; i<10; i++) {
		printf("hello world! %d\n", i);
	}
#endif
	return 0;
}

