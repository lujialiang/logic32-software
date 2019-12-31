
#include <stdarg.h>

int sum(int n, ...)
{
	int i, d, sum = 0;
	va_list vap;
	va_start(vap, n);
	for(i=0;i<n;i++) {
		d = va_arg(vap, int);
		sum += d;
	}
	va_end(vap);
	return sum;
}
