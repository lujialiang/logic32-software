
#include <stdio.h>
#include <stdarg.h>
#include <soc.h>

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

int kbhit(void)
{
	char *p = (char *)ADDR_UART0_SR;
	return *p & UART0_RX_DONE;
}

int _getchar(void)
{
	while( !kbhit() );
	return *(char *)ADDR_UART0_DR;
}

static void _putint0(unsigned int n)
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

void puthex(unsigned n)
{
	int i;
	const char *hex_str = "0123456789ABCDEF";
	char buf[9];
	for(i=0;i<8;i++) {
		buf[7-i] = hex_str[n&0x0f];
		n >>= 4;
	}
	buf[9] = 0;
	puts(buf);
}
