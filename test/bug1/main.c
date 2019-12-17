#include <stdio.h>
#include <math.h>

float fadd(float a, float b)
{
	return a + b;
}

#if 0
typedef union {
	unsigned int u32_value;
	float	f32_value;
} u32_f32_t;

int main(void)
{
	float f1, f2, f3;
	u32_f32_t u1;

	f1 = 11.1;
	f2 = 22.2;

	u1.f32_value = fadd(f1, f2);

	printf("%d\n", (int)u1.f32_value);

	return 0;
}
#else

#define	PI	3.141592653

int main(void)
{
	float f1, f2;
	float f3;
	f1 = 11.1;
	f2 = 22.2;
	printf("%f\n", f1);
	printf("%f\n", f2);
	printf("%f\n", fadd(f1, f2));
	printf("%f\n", sin(PI/6));
	printf("%f\n", sin(PI/4));
	return 0;
}
#endif
