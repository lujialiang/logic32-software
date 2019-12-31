#include <stdio.h>
#include <string.h>
#include <setjmp.h> 

jmp_buf env;

double divide(double to, double by)
{
	double result;
	if(by == 0)
		longjmp(env, 1);
	result = to/by;
	return result;
}

double f(double to, double by)
{
	double result = 0;
	if( setjmp(env) == 0)
	{
		result = divide(to, by);
	}
	else
		printf("Cannot / 0\n");
	return result;
}

int main(void)
{
	double a, b;
	a = 1.0;
	b = 3.0;
	printf("%lf / %lf = %lf\n", a, b, f(a, b));
	b = 0.0;
	printf("%lf / %lf = %lf\n", a, b, f(a, b));
	return 0;
}
