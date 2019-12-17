#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#define ADDR_RANDOM_BASE	0x86000000
#define ADDR_TLB_BASE		0x87000000

void mmu_en(void);

char * const str1 = "msg1";
char * const str2 = "msg2";
char * const str3 = "msg3";

int rand(void)
{
	return *(int *)ADDR_RANDOM_BASE;
}

void write_mr_tlb(int n, unsigned d)
{
	uint32_t *p = (uint32_t *)ADDR_TLB_BASE;
	if( n < 0 || n > 63 ) {
		return;
	}
	p[n] = d;
}

uint32_t read_mr_tlb(int n)
{
	uint32_t *p = (uint32_t *)ADDR_TLB_BASE;
	if( n < 0 || n > 63 ) {
		return -1;
	}
	return p[n];
}

void write_tr_tlb(int n, unsigned d)
{
	uint32_t *p = (uint32_t *)ADDR_TLB_BASE + 64;
	if( n < 0 || n > 63 ) {
		return;
	}
	p[n] = d;
}

uint32_t read_tr_tlb(int n)
{
	uint32_t *p = (uint32_t *)ADDR_TLB_BASE + 64;
	if( n < 0 || n > 63 ) {
		return -1;
	}
	return p[n];
}

void mmu_init(void)
{
	int i;
	for(i=0;i<64;i++) {
		write_mr_tlb(i, 0);
		write_tr_tlb(i, 0);
	}
	write_mr_tlb(0, 1);				// page 0
	write_tr_tlb(0, 0);
	write_mr_tlb(1, (0x1<<1)+1);	// page 1
	write_tr_tlb(1,  0x1<<4);
	write_mr_tlb(2, (0x2<<1)+1);	// page 2
	write_tr_tlb(2,  0x2<<4);
	write_mr_tlb(3, (0x3<<1)+1);	// page 3
	write_tr_tlb(3,  0x3<<4);
	write_mr_tlb(4, (0x4<<1)+1);	// page 4
	write_tr_tlb(4,  0x4<<4);
	write_mr_tlb(5, (0x5<<1)+1);	// page 5
	write_tr_tlb(5,  0x5<<4);
	write_mr_tlb(6, (0x6<<1)+1);	// page 6
	write_tr_tlb(6,  0x6<<4);
	write_mr_tlb(7, (0x7<<1)+1);	// page 7
	write_tr_tlb(7,  0x7<<4);
	write_mr_tlb(8, (0x8<<1)+1);	// page 8
	write_tr_tlb(8,  0x8<<4);
	write_mr_tlb(9, (0x9<<1)+1);	// page 9
	write_tr_tlb(9,  0x9<<4);
	write_mr_tlb(10, (0xff<<1)+1);	// page 10	stack
	write_tr_tlb(10,  0xff<<4);
	write_mr_tlb(11, (0xfe<<1)+1);	// page 11	stack
	write_tr_tlb(11,  0xfe<<4);
	write_mr_tlb(12, (0x80000<<1)+1);	// page 12	uart0
	write_tr_tlb(12,  0x80000<<4);
	mmu_en();
}

int main(void)
{
	int i;
	mmu_init();
	for(i=0;i<32;i++) {
		printf("i = %d\n", i);
	}
	return 0;
}

