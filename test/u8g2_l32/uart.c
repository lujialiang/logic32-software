#include "soc.h"

int kbhit(void)
{
    volatile char *p = (volatile char *)ADDR_UART0_SR;
    return *p & UART0_RX_DONE;
}

void uart_putc(int c)
{
	*(volatile char *)0x80000000 = c;
}

int uart_getc(void)
{
    while( !kbhit() );
    return *(char *)ADDR_UART0_DR;
}

void uart_puts(const char *s)
{
	while (*s) {
		uart_putc(*s);
		s++;
	}
	uart_putc('\n');
}
 
