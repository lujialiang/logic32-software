#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>

long long _readMicroseconds()
{
	return 0;
}


time_t
time (time_t *tloc)
{
	time_t t;
	t=(time_t)(_readMicroseconds()/(long long )1000000);
	if (tloc!=NULL)
	{
		*tloc=t;
	}
	return t;
}
