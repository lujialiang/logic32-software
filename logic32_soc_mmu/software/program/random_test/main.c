
#include <stdio.h>
#include <stdarg.h>

char * const str1 = "msg1";
char * const str2 = "msg2";
char * const str3 = "msg3";

int rand(void)
{
	return *(int *)0x86000000;
}

int main(void)
{
	int i;
	for(i=0; i<10; i++) {
		printf("random_number[%d] = 0x%08x\n", i, rand());
	}
	return 0;
}
