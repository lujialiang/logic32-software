
.text

.include "config.inc"

.global _main

.org 0
	lea $sp, 0x100000
	jal _main
	halt

.org VECTOR_SYSTICK_ISR
	halt

.org VECTOR_UART0_ISR
	halt

.org VECTOR_TRAP_UNDF
	halt

.org VECTOR_TRAP_SWI
	halt

.org VECTOR_DMA_CH0
	halt

.org VECTOR_IRQ_DISK0
	halt

.org VECTOR_IRQ_TMR0
	halt

.org VECTOR_DIV_BY_ZERO
	halt

.org VECTOR_BUSS_ERROR
	halt

.org VECTOR_ITLB_MISS
	halt

.org VECTOR_DTLB_MISS
	halt

/* void halt(void) */
.global _halt
_halt:
	halt

/* void mmu_en(void) */
.global _mmu_en
_mmu_en:
	mfc $4, $c0
	or $4, $4, 0x80000000
	mtc $c0, $4
	jmp $31

.end
