
.text

.include "config.inc"

.extern  OSRunning
.extern  OSPrioCur
.extern  OSPrioHighRdy
.extern  OSTCBCur
.extern  OSTCBHighRdy
.extern  OSTaskSwHook
.extern  OSTimeTick
.extern  OSIntExit
.extern  OSIntNesting

.global  OS_CPU_SR_Save
.global  OS_CPU_SR_Restore
.global  OSStartHighRdy
.global  OSCtxSw
.global  OSIntCtxSw
.global  systick_isr
.global  OSIntCtxSw_no_save

.macro  popa
lw $1,0x0004($29)
lw $2,0x0008($29)
lw $3,0x000c($29)
lw $4,0x0010($29)
lw $5,0x0014($29)
lw $6,0x0018($29)
lw $7,0x001c($29)
lw $8,0x0020($29)
lw $9,0x0024($29)
lw $10,0x0028($29)
lw $11,0x002c($29)
lw $12,0x0030($29)
lw $13,0x0034($29)
lw $14,0x0038($29)
lw $15,0x003c($29)
lw $16,0x0040($29)
lw $17,0x0044($29)
lw $18,0x0048($29)
lw $19,0x004c($29)
lw $20,0x0050($29)
lw $21,0x0054($29)
lw $22,0x0058($29)
lw $23,0x005c($29)
lw $24,0x0060($29)
lw $25,0x0064($29)
lw $26,0x0068($29)
lw $27,0x006c($29)
lw $28,0x0070($29)
lw $30,0x0078($29)
lw $31,0x007c($29)
.endm

.macro  pusha
sw 0x0004($29),$1
sw 0x0008($29),$2
sw 0x000c($29),$3
sw 0x0010($29),$4
sw 0x0014($29),$5
sw 0x0018($29),$6
sw 0x001c($29),$7
sw 0x0020($29),$8
sw 0x0024($29),$9
sw 0x0028($29),$10
sw 0x002c($29),$11
sw 0x0030($29),$12
sw 0x0034($29),$13
sw 0x0038($29),$14
sw 0x003c($29),$15
sw 0x0040($29),$16
sw 0x0044($29),$17
sw 0x0048($29),$18
sw 0x004c($29),$19
sw 0x0050($29),$20
sw 0x0054($29),$21
sw 0x0058($29),$22
sw 0x005c($29),$23
sw 0x0060($29),$24
sw 0x0064($29),$25
sw 0x0068($29),$26
sw 0x006c($29),$27
sw 0x0070($29),$28
sw 0x0078($29),$30
sw 0x007c($29),$31
.endm

/* OS_CPU_SR OS_CPU_SR_Save(void); */
OS_CPU_SR_Save:
	mfc $2, $psw
	disi
	jmpr $ra

/* void OS_CPU_SR_Restore(OS_CPU_SR cpu_sr); */
OS_CPU_SR_Restore:
	mtc $psw, $4
	jmpr $ra

/* void OSStartHighRdy(void) */
OSStartHighRdy:
	/* OSRunning := 1 */
	ldi $k1, 1
    ldi $k0, OSRunning
	sb ($k0), $k1

	ldi $k0, OSTCBHighRdy	/* $k1 = OSTCBHighRdy->OSTCBStkPtr */
	lw $k1, ($k0)
	lw $sp, ($k1)			/* $sp = ($k1) */

	ldi $k1, 0x40000
	;ldi $k1, 0x400
	ldi $k0, ADDR_SYSTICK_RELOAD
    sw  ($k0), $k1
	ldi $k1, 3
    ldi $k0, ADDR_SYSTICK_CTRL
	sw ($k0), $k1

	popa

	lw $k1, 128($sp)
	mtc $epc, $k1
	lw $k1, 132($sp)
	mtc $esr, $k1

	addi $sp, $sp, 136
	reti

/* void OSCtxSw(void); */
OSCtxSw:
	trap
	jmpr $ra

/* void OSIntCtxSw(void); */
OSIntCtxSw_no_save:
	addi $sp, $sp, -136

	pusha
    
	mfc $k1, $epc
	sw 128($sp), $k1
	mfc $k1, $esr
	sw 132($sp), $k1

	ldi $k0, OSTCBCur		/* $k1 = OSTCBCur->OSTCBStkPtr */
    lw $k1, ($k0)
	sw ($k1), $sp

OSIntCtxSw:

	ldi $k0, OSTCBHighRdy	/* $k1 = OSTCBHighRdy->OSTCBStkPtr */
    lw $k1, ($k0)
	lw $sp, ($k1)			/* $sp = ($k1) */

	jal OSTaskSwHook

    /* OSTCBCur := OSTCBHighRdy */
	ldi $k0, OSTCBHighRdy
    lw $k1, ($k0)

	ldi $k0, OSTCBCur
    sw ($k0), $k1

    /* OSPrioCur := OSPrioHighRdy */
	ldi $k0, OSPrioHighRdy
    lb $k1, ($k0)

	ldi $k0, OSPrioCur
    sb ($k0), $k1

	popa

	lw $k1, 128($sp)
	mtc $epc, $k1
	lw $k1, 132($sp)
	mtc $esr, $k1

	addi $sp, $sp, 136
	reti

systick_isr:
	addi $sp, $sp, -136

	pusha
    
	mfc $k1, $epc
	sw 128($sp), $k1
	mfc $k1, $esr
	sw 132($sp), $k1

    /* OSIntNesting := OSIntNesting + 1 */
    ldi $k0, OSIntNesting
    lb $k1, ($k0)
	addi $k1, $k1, 1
	sb ($k0), $k1

	addi $zero, $k1, -1
	bne NO_SAVE_SP
	ldi $k0, OSTCBCur		/* $k1 = OSTCBCur->OSTCBStkPtr */
    lw $k1, ($k0)
	sw ($k1), $sp

NO_SAVE_SP:
    jal OSTimeTick
    jal OSIntExit

	popa

	lw $k1, 128($sp)
	mtc $epc, $k1
	lw $k1, 132($sp)
	mtc $esr, $k1

	addi $sp, $sp, 136

	reti

