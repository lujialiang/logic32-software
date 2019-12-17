#include <stdio.h>
#include <stdint.h>

int disassembly(uint32_t IR);

void deal_undef(uint32_t *p)
{
	printf("epc = %p, [epc] = 0x%08x\n", p, *p);
	disassembly(*p);
}

double fabs(double x)
{
	return x>0?x:-x;
}

int main(int argc, char *argv[])
{
	double f1 = -0.1234;
	double f2;
	f2 = fabs(f1);
	printf("f1 = %lf, f2 = %lf\n", f1, f2);
	return 0;
}
