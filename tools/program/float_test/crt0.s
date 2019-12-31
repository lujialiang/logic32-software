
.text

.include "config.inc"

.extern _main
.extern _deal_undef

.org 0
	lea $sp, 0x100000
	jal _main
	halt

.org VECTOR_SYSTICK_ISR
	halt

.org VECTOR_UART0_ISR
	halt

.org VECTOR_TRAP_UNDF
	jmp undef_inst_isr
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

/* void halt(void); */
.global _halt
_halt:
	halt

/* void sei(void); */
.global _sei
_sei:
	enai
	jmp $lr

/* void cli(void); */
.global _cli
_cli:
	disi
	jmp $lr

undef_inst_isr:
	add $sp, $sp, -136
	.pusha
	mfc $4, $epc
	jal _deal_undef
	.popa
	add $sp, $sp, 136
	halt
