	.file	"_fifo.c"
	.section	.text
	.align 4
	.globl	fifo_init
	.type	fifo_init, @function
fifo_init:
	addi $sp, $sp, -4
	sw 0($sp), $fp	; store from sp
	mov $fp, $sp
	addi $sp, $sp, -4
	sw -4($fp), $a0 ; movsi
	lw $a1, -4($fp) ; load word
	mov $a0, $zero
	sw ($a1), $a0 ; movsi
	lw $a1, -4($fp) ; load word
	ldi $a0, 1
	sw 4($a1), $a0 ; movsi
	lw $a1, -4($fp) ; load word
	lw $a0, -4($fp) ; load word
	addi $a0, $a0, 16
	sw 8($a1), $a0 ; movsi
	lw $a1, -4($fp) ; load word
	lw $a0, -4($fp) ; load word
	addi $a0, $a0, 16
	sw 12($a1), $a0 ; movsi
	addi $sp, $sp, 4
	mov $sp, $fp
	lw $fp, 0($sp) ; load form sp
	addi $sp, $sp, 4
	ret
	.size	fifo_init, .-fifo_init
	.align 4
	.globl	fifo_put
	.type	fifo_put, @function
fifo_put:
	addi $sp, $sp, -4
	sw 0($sp), $fp	; store from sp
	mov $fp, $sp
	addi $sp, $sp, -12
	sw -4($fp), $a0 ; movsi
	sb -5($fp), $a1
	lw $a0, -4($fp) ; load word
	lw $a0, ($a0) ; load word
	cmp $a0, $zero
	bne .L3
	lw $a0, -4($fp) ; load word
	lw $a0, 8($a0) ; load word
	lb $a1, -5($fp)
	sb ($a0), $a1
	lw $a2, -4($fp) ; load word
	lw $a0, -4($fp) ; load word
	addi $a1, $a0, 47
	lw $a0, 8($a2) ; load word
	cmp $a0, $a1
	bne .L4
	lw $a0, -4($fp) ; load word
	lw $a1, -4($fp) ; load word
	addi $a1, $a1, 16
	sw 8($a0), $a1 ; movsi
	jmp .L5
.L4:
	lw $a1, -4($fp) ; load word
	lw $a0, -4($fp) ; load word
	lw $a0, 8($a0) ; load word
	addi $a0, $a0, 1
	sw 8($a1), $a0 ; movsi
.L5:
	lw $a2, -4($fp) ; load word
	lw $a0, -4($fp) ; load word
	addi $a1, $a0, 47
	lw $a0, 8($a2) ; load word
	cmp $a0, $a1
	bne .L6
	lw $a2, -4($fp) ; load word
	lw $a0, -4($fp) ; load word
	addi $a1, $a0, 16
	lw $a0, 12($a2) ; load word
	cmp $a0, $a1
	bne .L6
	lw $a1, -4($fp) ; load word
	ldi $a0, 1
	sw ($a1), $a0 ; movsi
	jmp .L7
.L6:
	lw $a0, -4($fp) ; load word
	lw $a0, 8($a0) ; load word
	addi $a1, $a0, 1
	lw $a0, -4($fp) ; load word
	lw $a0, 12($a0) ; load word
	cmp $a1, $a0
	bne .L7
	lw $a1, -4($fp) ; load word
	ldi $a0, 1
	sw ($a1), $a0 ; movsi
.L7:
	lw $a1, -4($fp) ; load word
	mov $a0, $zero
	sw 4($a1), $a0 ; movsi
	lw -12($fp), $zero
	jmp .L2
.L3:
	ldi $a0, 255
	sw -12($fp), $a0 ; movsi
.L2:
	lw $v0, -12($fp) ; load word
	addi $sp, $sp, 12
	mov $sp, $fp
	lw $fp, 0($sp) ; load form sp
	addi $sp, $sp, 4
	ret
	.size	fifo_put, .-fifo_put
	.align 4
	.globl	fifo_get
	.type	fifo_get, @function
fifo_get:
	addi $sp, $sp, -4
	sw 0($sp), $fp	; store from sp
	mov $fp, $sp
	addi $sp, $sp, -20
	sw -4($fp), $a0 ; movsi
	sw -8($fp), $a1 ; movsi
	lw $a0, -4($fp) ; load word
	lw $a0, 4($a0) ; load word
	subi $zero, $a0, 1
	bne .L10
	ldi $a0, 255
	sw -12($fp), $a0 ; movsi
	jmp .L9
.L10:
	lw $a1, -8($fp) ; load word
	lw $a0, -4($fp) ; load word
	lw $a0, 12($a0) ; load word
	lb $a0, ($a0)
	sb ($a1), $a0
	lw $a2, -4($fp) ; load word
	lw $a0, -4($fp) ; load word
	addi $a1, $a0, 47
	lw $a0, 12($a2) ; load word
	cmp $a0, $a1
	bne .L12
	lw $a0, -4($fp) ; load word
	lw $a1, -4($fp) ; load word
	addi $a1, $a1, 16
	sw 12($a0), $a1 ; movsi
	jmp .L13
.L12:
	lw $a1, -4($fp) ; load word
	lw $a0, -4($fp) ; load word
	lw $a0, 12($a0) ; load word
	addi $a0, $a0, 1
	sw 12($a1), $a0 ; movsi
.L13:
	lw $a1, -4($fp) ; load word
	sw -16($fp), $a1 ; movsi
	lw -20($fp), $zero
	lw $a0, -4($fp) ; load word
	lw $a1, -4($fp) ; load word
	lw $a2, 8($a0) ; load word
	lw $a0, 12($a1) ; load word
	cmp $a2, $a0
	bne .L14
	ldi $a0, 1
	sw -20($fp), $a0 ; movsi
.L14:
	lw $a0, -20($fp) ; load word
	lw $a1, -16($fp) ; load word
	sw 4($a1), $a0 ; movsi
	lw -12($fp), $zero
.L9:
	lw $v0, -12($fp) ; load word
	addi $sp, $sp, 20
	mov $sp, $fp
	lw $fp, 0($sp) ; load form sp
	addi $sp, $sp, 4
	ret
	.size	fifo_get, .-fifo_get
	.ident	"GCC: (GNU) 3.4.2"
