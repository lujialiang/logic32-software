
.text

.include "config.inc"

.extern _main
.extern systick_isr
.extern _panic_undef_inst
.extern _panic_div_by_zero

.extern _OSIntNesting
.extern _OSIntCtxSw_no_save:

.org 0
	lea $sp, 0x100000
	jal _main
	halt

.org VECTOR_SYSTICK_ISR
	jmp systick_isr
	halt

.org VECTOR_UART0_ISR
	halt

.org VECTOR_TRAP_UNDF
	disi
	jal _panic_undef_inst
	halt

.org VECTOR_TRAP_SWI
	jmp _OSIntCtxSw_no_save
	halt

.org VECTOR_DMA_CH0
	halt

.org VECTOR_IRQ_DISK0
	halt

.org VECTOR_IRQ_TMR0
	halt

.org VECTOR_DIV_BY_ZERO
	disi
	jal _panic_div_by_zero
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

