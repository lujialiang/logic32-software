
; irq vector
.equ VECTOR_SYSTICK_ISR,0x10
.equ VECTOR_UART0_ISR,	0x20
.equ VECTOR_TRAP_UNDF,	0x30
.equ VECTOR_TRAP_SWI,	0x40
.equ VECTOR_DMA_CH0,	0x50
.equ VECTOR_IRQ_DISK0,	0x60
.equ VECTOR_IRQ_TMR0,	0x70
.equ VECTOR_DIV_BY_ZERO,0x80

; uart0
.equ ADDR_UART0_BASE, 0x80000000
.equ UART0_DR, 0x00
.equ UART0_SR, 0x04
.equ UART0_CR, 0x08

.equ ADDR_GPIO0_BASE, 0x80010000

; dma0
; 0x81000000

; disk0
; 0x82000000

; systick
.equ ADDR_SYSTICK_CTRL,   0x83000000
.equ ADDR_SYSTICK_RELOAD, 0x83000004
.equ ADDR_SYSTICK_COUNT,  0x83000008
.equ SYSTICK_EN,	0
.equ SYSTICK_IE,	1

; rtc0
.equ ADDR_RTC0_DAT,	0x84000000
.equ ADDR_RTC0_CTR,	0x84000004
.equ ADDR_RTC0_YEAR,	0x84000008
.equ ADDR_RTC0_MON,	0x8400000c
.equ ADDR_RTC0_DAY,	0x84000010
.equ ADDR_RTC0_HOUR,	0x84000014
.equ ADDR_RTC0_MIN,	0x84000018
.equ ADDR_RTC0_SEC,	0x8400001c
.equ ADDR_RTC0_WEEK,	0x84000020
.equ RTC0_EN,	0
.equ RTC0_IE,	1
.equ RTC0_IF,	2
.equ RTC0_LD,	3

; timer0
.equ ADDR_TMR0_CNT,	0X85000000	; timer0 counter regster, 32 bit
.equ ADDR_TMR0_RLD,	0X85000004	; timer0 reload regster, 32 bit
.equ ADDR_TMR0_CTR,	0X85000008	; timer0 control regster, 32 bit
.equ TMR0_EN,	0	; timer0 enable
.equ TMR0_IE,	1	; timer0 interrupt enable
.equ TMR0_DI,	2	; timer0 counting direction, 0: down, 1: up
.equ TMR0_IF,	3	; timer0 interrup flag, when set, read timer0 control regster will clear it

; random number generater
.equ ADDR_RANDOM,	0X86000000

; tlb
.equ ADDR_TLB,		0X87000000
