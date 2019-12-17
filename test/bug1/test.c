#include <stdio.h>
#include <stdint.h>

typedef union {
	unsigned int buffer[2];
	double	value;
} f64_buffer_t;

int main(void)
{
	f64_buffer_t u1;
	u1.value = 1.1;
	printf("%x, %x\n", u1.buffer[0], u1.buffer[1]);
	printf("%d, %d\n", u1.buffer[0], u1.buffer[1]);
	return 0;
}

