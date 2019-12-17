
#include <stdio.h>

#ifdef logic_32
#include "fdlibm.h"	/* must include FDLIBM's fdlibm.h */
#else
#include <math.h>
#endif

//double y0();

int main(void)
{
#if 0
	_LIB_VERSION =  _IEEE_;
	printf("IEEE: y0(1e300) = %1.20e\n",y0(1e300));
	_LIB_VERSION = _XOPEN_;
	printf("XOPEN y0(1e300) = %1.20e\n",y0(1e300));
	_LIB_VERSION = _POSIX_;
	printf("POSIX y0(1e300) = %1.20e\n",y0(1e300));
	_LIB_VERSION = _SVID_;
	printf("SVID  y0(1e300) = %1.20e\n",y0(1e300));
#else
	double f = 0.1;
	double f2 = -0.9999;
#ifdef logic_32
	_LIB_VERSION =  _IEEE_;
	//_LIB_VERSION = _POSIX_;
	//_LIB_VERSION = _SVID_;
#endif
	printf("sin(%lf) = %lf\n", f, sin(f));
	printf("cos(%lf) = %lf\n", f, cos(f));
	printf("fabs(%lf) = %lf\n", f2, fabs(f2));
	//exit(0);
#endif
	return 0;
}
