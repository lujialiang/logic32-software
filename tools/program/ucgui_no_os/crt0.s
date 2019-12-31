
.text

.include "config.inc"

.extern _soc_init
.extern _main
.extern _OS_TimeMS

.org 0
	jmp reset
	halt

.org VECTOR_SYSTICK_ISR
	jmp systick_isr
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

.org VECTOR_BUS_ERROR 
	halt

.org VECTOR_ITBL_MISS
	halt

.org VECTOR_DTBL_MISS
	halt

.org VECTOR_HARD_FAULT
	halt

reset:
	lea $sp, 0x800000
    jal _soc_init
	jal _main
	halt

	.extern _GUI_TOUCH_Exec
systick_isr:
	add $sp, $sp, -136
	.pusha
	jal _GUI_TOUCH_Exec
	.popa
	add $sp, $sp, 136

	mov $k1, $1
	lw $k2, _OS_TimeMS
	add $k2, $k2, 1
	sw $k2, _OS_TimeMS
	mov $1, $k1
	reti

.global _halt
_halt:
	halt

.global _sei
_sei:
	enai
	jmp $31

.end
