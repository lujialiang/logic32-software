
.text

.include "config.inc"

.extern _soc_init
.extern _main
.extern _panic
.extern _panic_undef_inst
.extern _panic_div_by_zero
.extern _panic_bus_error
.extern _OS_CPU_SysTickHandler

.extern  _OSRunning
.extern  _OSPrioCur
.extern  _OSPrioHighRdy
.extern  _OSTCBCur
.extern  _OSTCBHighRdy
.extern  _OSIntExit
.extern  _OSTaskSwHook
.extern  _OS_CPU_ExceptStkBase
.extern _OSTimeTick
.extern _OSIntExit

.global  _OS_CPU_SR_Save
.global  _OS_CPU_SR_Restore
.global  _OSStartHighRdy
.global  _OSCtxSw
.global  _OSIntCtxSw

.extern _OSIntNesting

.org 0
	jmp _reset

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

.org VECTOR_BUS_ERROR
	halt
	;disi
	mfc $4, $epc
	jal _panic_bus_error
	halt

.org VECTOR_ITBL_MISS
	halt

.org VECTOR_DTBL_MISS
	halt

.org VECTOR_HARD_FAULT
	halt

_reset:
	lea $sp, 0x800000
	jal _soc_init
	jal _main
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

/* OS_CPU_SR OS_CPU_SR_Save(void); */
.global _OS_CPU_SR_Save
_OS_CPU_SR_Save:
	mfc $2, $psw
	disi
	jmp $lr

/* void OS_CPU_SR_Restore(OS_CPU_SR cpu_sr); */
.global _OS_CPU_SR_Restore
_OS_CPU_SR_Restore:
	mtc $psw, $4
	jmp $lr

/* void OSStartHighRdy(void) */
_OSStartHighRdy:
	/* OSRunning := 1 */
	lea $k1, 1
	sb $k1, _OSRunning

	lw $k1, _OSTCBHighRdy	/* $k1 = OSTCBHighRdy->OSTCBStkPtr */
	lw $sp, ($k1)			/* $sp = ($k1) */

	lea $k1, 0x04000
	sw $k1, ADDR_SYSTICK_RELOAD
	lea $k1, 3
	sw $k1, ADDR_SYSTICK_CTRL

	.popa
	lw $k1, 128($sp)
	mtc $epc, $k1
	lw $k1, 132($sp)
	mtc $esr, $k1

	add $sp, $sp, 136
	reti

// void OSCtxSw(void)
_OSCtxSw:
	trap
	jmp $lr

// void OSIntCtxSw(void);
_OSIntCtxSw_no_save:
	add $sp, $sp, -136

	.pusha
	mfc $k1, $epc
	sw $k1, 128($sp)
	mfc $k1, $esr
	sw $k1, 132($sp)

	lw $k1, _OSTCBCur		/* $1 = OSTCBCur->OSTCBStkPtr */
	sw $sp, ($k1)

_OSIntCtxSw:

	lw $k1, _OSTCBHighRdy	/* $k1 = OSTCBHighRdy->OSTCBStkPtr */
	lw $sp, ($k1)			/* $sp = ($k1) */

	/* void OSTaskSwHook(void) */
	jal _OSTaskSwHook

	lw $k1, _OSTCBHighRdy
	sw $k1, _OSTCBCur
	lb $k1, _OSPrioHighRdy
	sb $k1, _OSPrioCur

	.popa
	lw $k1, 128($sp)
	mtc $epc, $k1
	lw $k1, 132($sp)
	mtc $esr, $k1

	add $sp, $sp, 136
	reti

systick_isr:
	add $sp, $sp, -136

	.pusha
	mfc $k1, $epc
	sw $k1, 128($sp)
	mfc $k1, $esr
	sw $k1, 132($sp)

    lb $k1, _OSIntNesting
	add $k1, $k1, 1
	sb $k1, _OSIntNesting

	sub $zero, $k1, 1
	bne NO_SAVE_SP
	lw $k1, _OSTCBCur		// $1 = OSTCBCur->OSTCBStkPtr
	sw $sp, ($k1)

NO_SAVE_SP:
    jal _OSTimeTick
    jal _OSIntExit

	.popa
	lw $k1, 128($sp)
	mtc $epc, $k1
	lw $k1, 132($sp)
	mtc $esr, $k1

	add $sp, $sp, 136

	reti

