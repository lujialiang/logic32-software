#include <stdio.h>

int main(void)
{
	int i = -1;
	int j = -65538;
	printf("%d, %d\n", i, j);
	printf("%08x\n", 0x80000001);
	printf("%p\n", 0x80000001);
	return 0;
}
