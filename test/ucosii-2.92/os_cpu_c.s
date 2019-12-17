	.file	"os_cpu_c.c"
	.section	.text
	.align 4
	.globl	OSInitHookBegin
	.type	OSInitHookBegin, @function
OSInitHookBegin:
	sw -4($sp), $fp	; store from sp
	addi $sp, $sp, -4
	mov $fp, $sp
	mov $sp, $fp
	addi $sp, $sp, 4
	lw $fp, -4($sp) ; load form sp
	ret
	.size	OSInitHookBegin, .-OSInitHookBegin
	.align 4
	.globl	OSInitHookEnd
	.type	OSInitHookEnd, @function
OSInitHookEnd:
	sw -4($sp), $fp	; store from sp
	addi $sp, $sp, -4
	mov $fp, $sp
	mov $sp, $fp
	addi $sp, $sp, 4
	lw $fp, -4($sp) ; load form sp
	ret
	.size	OSInitHookEnd, .-OSInitHookEnd
	; extern	App_TaskCreateHook
	.align 4
	.globl	OSTaskCreateHook
	.type	OSTaskCreateHook, @function
OSTaskCreateHook:
	sw -4($sp), $ra	; store from sp
	sw -8($sp), $fp	; store from sp
	addi $sp, $sp, -8
	mov $fp, $sp
	addi $sp, $sp, -4
	sw -4($fp), $a0 ; movsi
	lw $a0, -4($fp) ; load word
	jal App_TaskCreateHook
	addi $sp, $sp, 4
	mov $sp, $fp
	addi $sp, $sp, 8
	lw $ra, -4($sp) ; load form sp
	lw $fp, -8($sp) ; load form sp
	ret
	.size	OSTaskCreateHook, .-OSTaskCreateHook
	; extern	App_TaskDelHook
	.align 4
	.globl	OSTaskDelHook
	.type	OSTaskDelHook, @function
OSTaskDelHook:
	sw -4($sp), $ra	; store from sp
	sw -8($sp), $fp	; store from sp
	addi $sp, $sp, -8
	mov $fp, $sp
	addi $sp, $sp, -4
	sw -4($fp), $a0 ; movsi
	lw $a0, -4($fp) ; load word
	jal App_TaskDelHook
	addi $sp, $sp, 4
	mov $sp, $fp
	addi $sp, $sp, 8
	lw $ra, -4($sp) ; load form sp
	lw $fp, -8($sp) ; load form sp
	ret
	.size	OSTaskDelHook, .-OSTaskDelHook
	; extern	App_TaskIdleHook
	.align 4
	.globl	OSTaskIdleHook
	.type	OSTaskIdleHook, @function
OSTaskIdleHook:
	sw -4($sp), $ra	; store from sp
	sw -8($sp), $fp	; store from sp
	addi $sp, $sp, -8
	mov $fp, $sp
	jal App_TaskIdleHook
	mov $sp, $fp
	addi $sp, $sp, 8
	lw $ra, -4($sp) ; load form sp
	lw $fp, -8($sp) ; load form sp
	ret
	.size	OSTaskIdleHook, .-OSTaskIdleHook
	; extern	printf
	; extern	App_TaskReturnHook
	.section	.rodata
.LC0:
	.string	"OSTaskReturnHook() OSTCBPrio = %d\n"
	.section	.text
	.align 4
	.globl	OSTaskReturnHook
	.type	OSTaskReturnHook, @function
OSTaskReturnHook:
	sw -4($sp), $ra	; store from sp
	sw -8($sp), $fp	; store from sp
	addi $sp, $sp, -8
	mov $fp, $sp
	addi $sp, $sp, -12
	sw -4($fp), $a0 ; movsi
	ldi $a0, .LC0
	sw ($sp), $a0 ; movsi
	lw $a0, -4($fp) ; load word
	lb $a0, 38($a0)
	lsli $a0, $a0, 24	; asli
	lsri $a0, $a0, 24
	sw 4($sp), $a0	; store from sp
	jal printf
	lw $a0, -4($fp) ; load word
	jal App_TaskReturnHook
	addi $sp, $sp, 12
	mov $sp, $fp
	addi $sp, $sp, 8
	lw $ra, -4($sp) ; load form sp
	lw $fp, -8($sp) ; load form sp
	ret
	.size	OSTaskReturnHook, .-OSTaskReturnHook
	; extern	App_TaskStatHook
	.align 4
	.globl	OSTaskStatHook
	.type	OSTaskStatHook, @function
OSTaskStatHook:
	sw -4($sp), $ra	; store from sp
	sw -8($sp), $fp	; store from sp
	addi $sp, $sp, -8
	mov $fp, $sp
	jal App_TaskStatHook
	mov $sp, $fp
	addi $sp, $sp, 8
	lw $ra, -4($sp) ; load form sp
	lw $fp, -8($sp) ; load form sp
	ret
	.size	OSTaskStatHook, .-OSTaskStatHook
	; extern	OS_TaskReturn
	.align 4
	.globl	OSTaskStkInit
	.type	OSTaskStkInit, @function
OSTaskStkInit:
	sw -4($sp), $fp	; store from sp
	addi $sp, $sp, -4
	mov $fp, $sp
	addi $sp, $sp, -24
	sw -4($fp), $a0 ; movsi
	sw -8($fp), $a1 ; movsi
	sw -12($fp), $a2 ; movsi
	sh -14($fp), $a3
	ldi $a1, -1996488704
	lw $a0, -12($fp) ; load word
	sw ($a1), $a0 ; movsi
	lw $a0, -12($fp) ; load word
	addi $a0, $a0, 4
	sw -20($fp), $a0 ; movsi
	lw $a1, -20($fp) ; load word
	ldi $a0, -8
	and $a0, $a1, $a0
	sw -20($fp), $a0 ; movsi
	lw $a0, -20($fp) ; load word
	addi $a0, $a0, -4
	sw -20($fp), $a0 ; movsi
	lw $a0, -20($fp) ; load word
	addi $a0, $a0, -4
	mov $a1, $a0
	sw -20($fp), $a1 ; movsi
	ldi $a0, 16
	sw ($a1), $a0 ; movsi
	lw $a0, -20($fp) ; load word
	addi $a0, $a0, -4
	mov $a1, $a0
	sw -20($fp), $a1 ; movsi
	lw $a0, -4($fp) ; load word
	sw ($a1), $a0 ; movsi
	lw $a0, -20($fp) ; load word
	addi $a0, $a0, -4
	mov $a1, $a0
	sw -20($fp), $a1 ; movsi
	ldi $a0, OS_TaskReturn
	sw ($a1), $a0 ; movsi
	ldi $a0, 30
	sw -24($fp), $a0 ; movsi
.L9:
	lw $a1, -24($fp) ; load word
	ldi $a0, 0
	cmp $a1,$a0
	blt .L10
	lw $a1, -24($fp) ; load word
	ldi $a0, 4
	cmp $a1,$a0
	bne .L12
	lw $a0, -20($fp) ; load word
	addi $a0, $a0, -4
	mov $a1, $a0
	sw -20($fp), $a1 ; movsi
	lw $a0, -8($fp) ; load word
	sw ($a1), $a0 ; movsi
	jmp .L11
.L12:
	lw $a0, -20($fp) ; load word
	addi $a0, $a0, -4
	mov $a1, $a0
	sw -20($fp), $a1 ; movsi
	lw $a0, -24($fp) ; load word
	sw ($a1), $a0 ; movsi
.L11:
	lw $a0, -24($fp) ; load word
	addi $a0, $a0, -1
	sw -24($fp), $a0 ; movsi
	jmp .L9
.L10:
	lw $a0, -20($fp) ; load word
	mov $v0, $a0
	addi $sp, $sp, 24
	mov $sp, $fp
	addi $sp, $sp, 4
	lw $fp, -4($sp) ; load form sp
	ret
	.size	OSTaskStkInit, .-OSTaskStkInit
	; extern	App_TaskSwHook
	.align 4
	.globl	OSTaskSwHook
	.type	OSTaskSwHook, @function
OSTaskSwHook:
	sw -4($sp), $ra	; store from sp
	sw -8($sp), $fp	; store from sp
	addi $sp, $sp, -8
	mov $fp, $sp
	jal App_TaskSwHook
	mov $sp, $fp
	addi $sp, $sp, 8
	lw $ra, -4($sp) ; load form sp
	lw $fp, -8($sp) ; load form sp
	ret
	.size	OSTaskSwHook, .-OSTaskSwHook
	; extern	App_TCBInitHook
	.align 4
	.globl	OSTCBInitHook
	.type	OSTCBInitHook, @function
OSTCBInitHook:
	sw -4($sp), $ra	; store from sp
	sw -8($sp), $fp	; store from sp
	addi $sp, $sp, -8
	mov $fp, $sp
	addi $sp, $sp, -4
	sw -4($fp), $a0 ; movsi
	lw $a0, -4($fp) ; load word
	jal App_TCBInitHook
	addi $sp, $sp, 4
	mov $sp, $fp
	addi $sp, $sp, 8
	lw $ra, -4($sp) ; load form sp
	lw $fp, -8($sp) ; load form sp
	ret
	.size	OSTCBInitHook, .-OSTCBInitHook
	; extern	App_TimeTickHook
	.align 4
	.globl	OSTimeTickHook
	.type	OSTimeTickHook, @function
OSTimeTickHook:
	sw -4($sp), $ra	; store from sp
	sw -8($sp), $fp	; store from sp
	addi $sp, $sp, -8
	mov $fp, $sp
	jal App_TimeTickHook
	mov $sp, $fp
	addi $sp, $sp, 8
	lw $ra, -4($sp) ; load form sp
	lw $fp, -8($sp) ; load form sp
	ret
	.size	OSTimeTickHook, .-OSTimeTickHook
	; extern	OS_CPU_SR_Save
	; extern	OSIntNesting
	; extern	OS_CPU_SR_Restore
	; extern	OSTimeTick
	; extern	OSIntExit
	.align 4
	.globl	OS_CPU_SysTickHandler
	.type	OS_CPU_SysTickHandler, @function
OS_CPU_SysTickHandler:
	sw -4($sp), $ra	; store from sp
	sw -8($sp), $fp	; store from sp
	addi $sp, $sp, -8
	mov $fp, $sp
	addi $sp, $sp, -4
	jal OS_CPU_SR_Save
	mov $a0, $v0
	sw -4($fp), $a0 ; movsi
	ldi $a1, OSIntNesting
	ldi $a0, OSIntNesting
	lb $a0, ($a0)
	addi $a0, $a0, 1
	sb ($a1), $a0
	lw $a0, -4($fp) ; load word
	jal OS_CPU_SR_Restore
	jal OSTimeTick
	jal OSIntExit
	addi $sp, $sp, 4
	mov $sp, $fp
	addi $sp, $sp, 8
	lw $ra, -4($sp) ; load form sp
	lw $fp, -8($sp) ; load form sp
	ret
	.size	OS_CPU_SysTickHandler, .-OS_CPU_SysTickHandler
	.ident	"GCC: (GNU) 3.4.2"
