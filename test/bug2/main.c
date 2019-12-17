#include <stdio.h>

float fadd(float a, float b)
{
	return a + b;
}

typedef union {
	unsigned int u32_value;
	float	f32_value;
} u32_f32_t;

int main(void)
{
	float a = 1.1;
	float b = 2.2;
	float c;
	u32_f32_t ua;
	u32_f32_t ub;
	u32_f32_t uc;

	c = fadd(a, b);

	ua.f32_value = a;
	ub.f32_value = b;
	uc.f32_value = c;

	printf("a = %f\n", ua.f32_value);
	printf("b = %f\n", ub.f32_value);
	printf("c = %f\n", uc.f32_value);

	printf("a = 0x%x\n", ua.u32_value);
	printf("b = 0x%x\n", ub.u32_value);
	printf("c = 0x%x\n", uc.u32_value);

	return 0;
}
