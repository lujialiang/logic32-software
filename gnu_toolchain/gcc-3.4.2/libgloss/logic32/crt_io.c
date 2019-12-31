#include <syscall.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

// uart0
#define ADDR_UART0_DR  0x80000000
#define ADDR_UART0_SR  0x80000004
#define ADDR_UART0_CR  0x80000008
#define UART0_RX_DONE	1
#define UART0_TX_DONE	2
#define UART0_RX_IE		4
#define UART0_TX_IE		8

/*
 * Wait indefinitely for input byte
 */
//int __attribute__ ((weak)) inbyte()  
int inbyte(void)  
{
	char *p = (char *)ADDR_UART0_SR;
	while( !(*p & UART0_RX_DONE) );
	return *(char *)ADDR_UART0_DR;
}

/* 
 * Output one character to the serial port 
 * 
 * 
 */
//void __attribute__ ((weak)) outbyte(int c)  
void outbyte(int c)  
{
	/* Wait for space in FIFO */
	*(volatile char *)0x80000000 = c;
}

//void __attribute__ ((weak)) _initIO(void)  
void _initIO(void)  
{
}

//long long __attribute__ ((weak)) _readCycles()  
long long _readCycles()  
{
	return 0;
}
