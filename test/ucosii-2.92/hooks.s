	.file	"hooks.c"
	.section	.text
	.align 4
	.globl	App_TaskCreateHook
	.type	App_TaskCreateHook, @function
App_TaskCreateHook:
	sw -4($sp), $fp	; store from sp
	addi $sp, $sp, -4
	mov $fp, $sp
	addi $sp, $sp, -4
	sw -4($fp), $a0 ; movsi
	addi $sp, $sp, 4
	mov $sp, $fp
	addi $sp, $sp, 4
	lw $fp, -4($sp) ; load form sp
	ret
	.size	App_TaskCreateHook, .-App_TaskCreateHook
	.align 4
	.globl	App_TaskDelHook
	.type	App_TaskDelHook, @function
App_TaskDelHook:
	sw -4($sp), $fp	; store from sp
	addi $sp, $sp, -4
	mov $fp, $sp
	addi $sp, $sp, -4
	sw -4($fp), $a0 ; movsi
	addi $sp, $sp, 4
	mov $sp, $fp
	addi $sp, $sp, 4
	lw $fp, -4($sp) ; load form sp
	ret
	.size	App_TaskDelHook, .-App_TaskDelHook
	.align 4
	.globl	App_TaskIdleHook
	.type	App_TaskIdleHook, @function
App_TaskIdleHook:
	sw -4($sp), $fp	; store from sp
	addi $sp, $sp, -4
	mov $fp, $sp
	mov $sp, $fp
	addi $sp, $sp, 4
	lw $fp, -4($sp) ; load form sp
	ret
	.size	App_TaskIdleHook, .-App_TaskIdleHook
	.align 4
	.globl	App_TaskReturnHook
	.type	App_TaskReturnHook, @function
App_TaskReturnHook:
	sw -4($sp), $fp	; store from sp
	addi $sp, $sp, -4
	mov $fp, $sp
	addi $sp, $sp, -4
	sw -4($fp), $a0 ; movsi
	addi $sp, $sp, 4
	mov $sp, $fp
	addi $sp, $sp, 4
	lw $fp, -4($sp) ; load form sp
	ret
	.size	App_TaskReturnHook, .-App_TaskReturnHook
	.align 4
	.globl	App_TaskStatHook
	.type	App_TaskStatHook, @function
App_TaskStatHook:
	sw -4($sp), $fp	; store from sp
	addi $sp, $sp, -4
	mov $fp, $sp
	mov $sp, $fp
	addi $sp, $sp, 4
	lw $fp, -4($sp) ; load form sp
	ret
	.size	App_TaskStatHook, .-App_TaskStatHook
	.align 4
	.globl	App_TaskSwHook
	.type	App_TaskSwHook, @function
App_TaskSwHook:
	sw -4($sp), $fp	; store from sp
	addi $sp, $sp, -4
	mov $fp, $sp
	mov $sp, $fp
	addi $sp, $sp, 4
	lw $fp, -4($sp) ; load form sp
	ret
	.size	App_TaskSwHook, .-App_TaskSwHook
	.align 4
	.globl	App_TCBInitHook
	.type	App_TCBInitHook, @function
App_TCBInitHook:
	sw -4($sp), $fp	; store from sp
	addi $sp, $sp, -4
	mov $fp, $sp
	addi $sp, $sp, -4
	sw -4($fp), $a0 ; movsi
	addi $sp, $sp, 4
	mov $sp, $fp
	addi $sp, $sp, 4
	lw $fp, -4($sp) ; load form sp
	ret
	.size	App_TCBInitHook, .-App_TCBInitHook
	; extern	OSRunning
	; extern	puts
	.section	.rodata
.LC0:
	.string	"App_TimeTickHook() : os not running\n"
	.section	.text
	.align 4
	.globl	App_TimeTickHook
	.type	App_TimeTickHook, @function
App_TimeTickHook:
	sw -4($sp), $ra	; store from sp
	sw -8($sp), $fp	; store from sp
	addi $sp, $sp, -8
	mov $fp, $sp
	ldi $a0, OSRunning
	lb $a0, ($a0)
	lsli $a0, $a0, 24	; asli
	lsri $a0, $a0, 24
	ldi $a1, 0
	cmp $a0,$a1
	bne .L8
	ldi $a0, .LC0
	jal puts
.L8:
	mov $sp, $fp
	addi $sp, $sp, 8
	lw $ra, -4($sp) ; load form sp
	lw $fp, -8($sp) ; load form sp
	ret
	.size	App_TimeTickHook, .-App_TimeTickHook
	.ident	"GCC: (GNU) 3.4.2"
