#include <stdio.h>
#include <string.h>
#include <setjmp.h> 

int main(void)
{
	jmp_buf env;
	int i;
	i = setjmp(env);
	printf("i = %d\n", i);
	if( i != 0 ) {
		printf("form longjmp, i = %d\n", i);
		return 0;
	}
	longjmp(env, 22);
	printf("This line dones not get printed\n");
	return -1;
}
