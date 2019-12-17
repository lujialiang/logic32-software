
void uart_putchar(int c)
{
	*(volatile char *)0x80000000 = c;
}

void uart_puts(const char *s)
{
	while (*s) {
		uart_putchar(*s);
		s++;
	}
}
 
