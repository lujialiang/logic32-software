#include <stdio.h>
#include <string.h>

#define ADDR_NIC_START 0x88000000
#define ADDR_NIC_STOP  0x88000014

#define ADDR_NIC_CTR    0x88000000
#define ADDR_NIC_RSCT   0x88000004
#define ADDR_NIC_WSCT   0x88000008
#define ADDR_NIC_RIDX   0x8800000c
#define ADDR_NIC_WIDX   0x88000010
#define ADDR_NIC_DAT    0x88000014
#define NIC_EN  0
#define NIC_IE  1
#define NIC_IF  2
#define NIC_RDP 3
#define NIC_WRP 4
#define NIC_RDY 5

unsigned foo(unsigned *p)
{
	//unsigned *p = (unsigned *)0x88000004;
	return (unsigned)p;
}

int main(int argc, char *argv[])
{
	int i;
	unsigned *p = (unsigned *)ADDR_NIC_START;
	for(i=0;i<6;i++) {
	#ifdef linux
		printf("%08x: %08x\n", foo(p), 0);
	#else
		printf("%p: %08x\n", p, *p);
	#endif
		p++;
	}
	while(1);
	return 0;
}

