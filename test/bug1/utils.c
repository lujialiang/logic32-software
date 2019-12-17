
static void _putint0(unsigned int n)
{
	if(n>=10) {
		_putint0(n/10);
	}
	_putchar(n%10+'0');
}

void putint(int n)
{
	if( n < 0 ) {
		_putchar('-');
		n *= -1;
	}
	_putint0(n);
	_putchar(10);
}

