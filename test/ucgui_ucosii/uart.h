#ifndef __UART_H__
#define __UART_H__

void uart_putc(int c);

int uart_getc(void);

void uart_puts(const char *s);

#endif
