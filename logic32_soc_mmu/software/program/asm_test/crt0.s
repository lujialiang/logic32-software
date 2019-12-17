
.text

.include "config.inc"

.extern _main
.extern _systick_isr

.org 0
	lea $sp, 0x10000
	jal _main
	halt

.org VECTOR_SYSTICK_ISR
	jmp _task_switch0
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

.global _seti
_seti:
	enai
	jmp $31

.global _disi
_disi:
	disi
	jmp $31

_task_switch0:
	add $sp,$sp,-132
	;sw $0, 0($sp)
	sw $1, 4($sp)
	sw $2, 8($sp)
	sw $3, 12($sp)
	sw $4, 16($sp)
	sw $5, 20($sp)
	sw $6, 24($sp)
	sw $7, 28($sp)
	sw $8, 32($sp)
	sw $9, 36($sp)
	sw $10,40($sp)
	sw $11,44($sp)
	sw $12,48($sp)
	sw $13,52($sp)
	sw $14,56($sp)
	sw $15,60($sp)
	sw $16,64($sp)
	sw $17,68($sp)
	sw $18,72($sp)
	sw $19,76($sp)
	sw $20,80($sp)
	sw $21,84($sp)
	sw $22,88($sp)
	sw $23,92($sp)
	sw $24,96($sp)
	sw $25,100($sp)
	sw $26,104($sp)
	sw $27,108($sp)
	sw $28,112($sp)
	;sw $29,116($sp)
	sw $30,120($sp)
	sw $31,124($sp)

	/* void systick_isr(void); */
	jal _systick_isr

	;lw $0, 0($sp)
	lw $1, 4($sp)
	lw $2, 8($sp)
	lw $3, 12($sp)
	lw $4, 16($sp)
	lw $5, 20($sp)
	lw $6, 24($sp)
	lw $7, 28($sp)
	lw $8, 32($sp)
	lw $9, 36($sp)
	lw $10,40($sp)
	lw $11,44($sp)
	lw $12,48($sp)
	lw $13,52($sp)
	lw $14,56($sp)
	lw $15,60($sp)
	lw $16,64($sp)
	lw $17,68($sp)
	lw $18,72($sp)
	lw $19,76($sp)
	lw $20,80($sp)
	lw $21,84($sp)
	lw $22,88($sp)
	lw $23,92($sp)
	lw $24,96($sp)
	lw $25,100($sp)
	lw $26,104($sp)
	lw $27,108($sp)
	lw $28,112($sp)
	;lw $29,116($sp)
	lw $30,120($sp)
	lw $31,124($sp)

	add $sp,$sp,132
	reti

