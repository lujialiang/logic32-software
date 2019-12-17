.file	"crt1.asm"
.text
.global	__start
.func	__start

; irq vector
.equ VECTOR_SYSTICK_ISR,0x10
.equ VECTOR_UART0_ISR,  0x20
.equ VECTOR_TRAP_UNDF,  0x30
.equ VECTOR_TRAP_SWI,   0x40
.equ VECTOR_DMA_CH0,    0x50
.equ VECTOR_IRQ_DISK0,  0x60
.equ VECTOR_IRQ_TMR0,   0x70
.equ VECTOR_DIV_BY_ZERO,0x80
.equ VECTOR_BUS_ERRORR,	0x90

.org 0
__start:
	ldi $sp, 0x1000000
	jal main
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

.org VECTOR_BUS_ERRORR
    halt
