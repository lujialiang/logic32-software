
static void _putint0(unsigned int n)
{
    if(n>=10) {
        _putint0(n/10);
    }   
    uart_putchar(n%10+'0');
}

void putint(int n)
{
    if( n < 0 ) {
        putchar('-');
        n *= -1;
    }
    _putint0(n);
    uart_putchar(10);
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
    uart_puts(buf);
}


