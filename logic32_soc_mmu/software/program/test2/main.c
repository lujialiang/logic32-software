
#include <stdio.h>
#include <stdarg.h>

char * const str1 = "msg1";
char * const str2 = "msg2";
char * const str3 = "msg3";

int rand(void)
{
	return *(int *)0x83000000;
}

int main(void)
{
#if 0
	char *msg[3];
	msg[0] = str1;
	msg[1] = str2;
	msg[2] = str3;
	puts(msg[0]);
	puts(msg[1]);
	puts(msg[2]);
#else
	int i;
	for(i=0; i<10; i++) {
		printf("random_number[%d] = 0x%08x\n", i, rand());
	}
#endif
	return 0;
}
