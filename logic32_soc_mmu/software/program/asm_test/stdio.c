
#include <stdio.h>
#include <stdarg.h>

int vsprintf(char *buf, const char *fmt, va_list args);

int putchar(int c)
{
	*(volatile char *)0x80000000 = c;
	return 0;
}

int puts(char *s)
{
	while(*s) {
		putchar(*s++);
	}
	putchar(10);
	return 0;
}

static char buf[1024];

int printf(const char *fmt, ...)
{
	va_list args;
	int i;
	va_start (args, fmt);
	i = vsprintf(buf, fmt, args);
	{
		char *s = buf;
		while(*s) {
			putchar(*s++);
		}
	}
	va_end (args);
	return i;
}

static void _putint0(int n)
{
	if(n>=10) {
		_putint0(n/10);
	}
	putchar(n%10+'0');
}

void putint(int n)
{
	if( n < 0 ) {
		putchar('-');
		n *= -1;
	}
	_putint0(n);
	putchar(10);
}
