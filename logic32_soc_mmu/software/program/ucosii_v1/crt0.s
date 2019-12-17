
.text

.include "config.inc"

.extern _main
.extern _panic
.extern _panic_undef_inst
.extern _panic_div_by_zero
.extern _OS_CPU_SysTickHandler

.extern  _OSRunning
.extern  _OSPrioCur
.extern  _OSPrioHighRdy
.extern  _OSTCBCur
.extern  _OSTCBHighRdy
.extern  _OSIntExit
.extern  _OSTaskSwHook
.extern  _OS_CPU_ExceptStkBase

.global  _OS_CPU_SR_Save
.global  _OS_CPU_SR_Restore
.global  _OSStartHighRdy
.global  _OSCtxSw
.global  _OSIntCtxSw

.extern _OSIntNesting

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
	jmp $31

/* void cli(void); */
.global _cli
_cli:
	disi
	jmp $31

/* OS_CPU_SR OS_CPU_SR_Save(void); */
.global _OS_CPU_SR_Save
_OS_CPU_SR_Save:
	mfc $2, $psw
	disi
	jmp $31

/* void OS_CPU_SR_Restore(OS_CPU_SR cpu_sr); */
.global _OS_CPU_SR_Restore
_OS_CPU_SR_Restore:
	mtc $psw, $4
	jmp $31

/* void OSStartHighRdy(void) */
_OSStartHighRdy:
	/* OSRunning := 1 */
	lea $4, 1
	sb $4, _OSRunning

	lw $29, _OSTCBHighRdy	/* $29 = OSTCBHighRdy->OSTCBStkPtr */
	lw $29, ($29)			/* $sp = ($29) */

	lea $2, 0x10000
	sw $2, ADDR_SYSTICK_RELOAD
	lea $2, 3
	sw $2, ADDR_SYSTICK_CTRL

	popa

	add $sp, $sp, 124
	reti

// void OSCtxSw(void)
_OSCtxSw:
	trap
	jmp $31

// void OSIntCtxSw(void);
_OSIntCtxSw_no_save:
	add $sp, $sp, -124

	pusha

	lw $2, _OSTCBCur		/* $1 = OSTCBCur->OSTCBStkPtr */
	sw $sp, ($2)

_OSIntCtxSw:

	lw $29, _OSTCBHighRdy	/* $29 = OSTCBHighRdy->OSTCBStkPtr */
	lw $29, ($29)			/* $sp = ($29) */

	/* void OSTaskSwHook(void) */
	jal _OSTaskSwHook

	lw $2, _OSTCBHighRdy
	sw $2, _OSTCBCur
	lb $2, _OSPrioHighRdy
	sb $2, _OSPrioCur

	popa

	add $sp, $sp, 124
	reti

systick_isr:
	add $sp, $sp, -124

	pusha

    lb $2, _OSIntNesting
	add $2, $2, 1
	sb $2, _OSIntNesting

	sub $0, $2, 1
	bnz NO_SAVE_SP
	lw $2, _OSTCBCur		// $1 = OSTCBCur->OSTCBStkPtr
	sw $sp, ($2)

/*
	.extern _save_sp
	mov $4, $sp
	jal _save_sp
*/

NO_SAVE_SP:
	// OSTimeTick */
	.extern _OSTimeTick
	// OSIntExit */
	.extern _OSIntExit
    jal _OSTimeTick
    jal _OSIntExit

	popa

	add $sp, $sp, 124
	reti

