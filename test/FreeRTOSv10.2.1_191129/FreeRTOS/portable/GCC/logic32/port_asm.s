
.text

.include "config.inc"

.global OSStartHighRdy
.global systick_isr
.global software_interrupt_isr
.global pxCurrentTCB

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

.macro  portSaveContext
	/* save context begin */
	addi $sp, $sp, -136

	pusha
    
	mfc $k1, $epc
	sw 128($sp), $k1
	mfc $k1, $esr
	sw 132($sp), $k1

    ldi $k0, pxCurrentTCB   /* $k1 := pxCurrentTCB->pxTopOfStack */
    lw $k1, ($k0)
    sw ($k1), $sp           /* ($k1) := $sp */
	/* save context end */
.endm

.macro  portRestoreContext
	/* restore context begin */
    ldi $k0, pxCurrentTCB   /* $k1 := pxCurrentTCB->pxTopOfStack */
    lw $k1, ($k0)
    lw $sp, ($k1)           /* $sp := ($k1) */

	popa

	lw $k1, 128($sp)
	mtc $epc, $k1
	lw $k1, 132($sp)
	mtc $esr, $k1

	addi $sp, $sp, 136
	/* restore context end */
.endm

/* void OSStartHighRdy(void) */
OSStartHighRdy:

	portRestoreContext

	reti

    ;ldi $k0, pxCurrentTCB   /* $k1 := pxCurrentTCB->pxTopOfStack */
    ;lw $k1, ($k0)
    ;lw $sp, ($k1)           /* $sp := ($k1) */

	;popa

	;lw $k1, 128($sp)
	;mtc $epc, $k1
	;lw $k1, 132($sp)
	;mtc $esr, $k1

	;addi $sp, $sp, 136
	;reti

software_interrupt_isr:

	portSaveContext

    jal vTaskSwitchContext

	portRestoreContext

	reti

systick_isr:

	portSaveContext

    jal xTaskIncrementTick
    cmp $v0, $zero
    bne 1f
    jal vTaskSwitchContext

1:
	portRestoreContext

	reti

