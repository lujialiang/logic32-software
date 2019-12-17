
.text

.include "config.inc"

.extern main
.extern systick_isr
.extern panic_undef_inst
.extern panic_div_by_zero

.extern OSIntNesting
.extern OSIntCtxSw_no_save

.org 0
	ldi $sp, 0x100000
	jal main
	halt

.org VECTOR_SYSTICK_ISR
	jmp systick_isr
	halt

.org VECTOR_UART0_ISR
	halt

.org VECTOR_TRAP_UNDF
	disi
	jal panic_undef_inst
	halt

.org VECTOR_TRAP_SWI
	jmp OSIntCtxSw_no_save
	halt

.org VECTOR_DMA_CH0
	halt

.org VECTOR_IRQ_DISK0
	halt

.org VECTOR_IRQ_TMR0
	halt

.org VECTOR_DIV_BY_ZERO
	disi
	jal panic_div_by_zero
	halt

.org VECTOR_BUS_ERROR
	halt

/* void halt(void); */
.global halt
halt:
	halt

/* void sei(void); */
.global sei
sei:
	enai
	jmpr $ra

/* void cli(void); */
.global cli
cli:
	disi
	jmpr $ra

