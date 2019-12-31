
.text

.include "config.inc"

.extern  _OSRunning
.extern  _OSPrioCur
.extern  _OSPrioHighRdy
.extern  _OSTCBCur
.extern  _OSTCBHighRdy
.extern  _OSTaskSwHook
.extern _OSTimeTick
.extern _OSIntExit
.extern _OSIntNesting

.global  _OS_CPU_SR_Save
.global  _OS_CPU_SR_Restore
.global  _OSStartHighRdy
.global  _OSCtxSw
.global  _OSIntCtxSw
.global systick_isr
.global _OSIntCtxSw_no_save:

/* OS_CPU_SR OS_CPU_SR_Save(void); */
_OS_CPU_SR_Save:
	mfc $2, $psw
	disi
	jmp $lr

/* void OS_CPU_SR_Restore(OS_CPU_SR cpu_sr); */
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

	lea $k1, 0x40000
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

