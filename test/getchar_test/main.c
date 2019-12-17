#include <stdio.h>
#include <string.h>

#define	TEST_FLOAT

#if 0
int main(void)
{
	int i = 0;
	char key;
	while( (key = getchar()) != 0x1b ) {
		i++;
		printf("#%d %d, 0x%x\n", i, key, key);
	}
	return 0;
}
#else

#ifndef TEST_FLOAT
int main(void)
{
	int a, b, c;

	a = 0;
	printf("please input a:");
	fflush(stdout);
	scanf("%d", &a);

	b = 0;
	printf("please input b:");
	fflush(stdout);
	scanf("%d", &b);

	c = a + b;

	printf("a + b = %d\n", c);
	fflush(stdout);

	return 0;
}
#else
int main(void)
{
	float a, b, c;

	a = 0;
	printf("please input a:");
	fflush(stdout);
	scanf("%f", &a);

	b = 0;
	printf("please input b:");
	fflush(stdout);
	scanf("%f", &b);

	c = a + b;

	printf("a + b = %f\n", c);
	fflush(stdout);

	return 0;
}
#endif

#endif
