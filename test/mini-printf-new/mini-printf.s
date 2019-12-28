	.file	"mini-printf.c"
	.section	.text
	.align 4
	.type	mini_strlen, @function
mini_strlen:
	addi $sp, $sp, -4
	sw 0($sp), $fp	; store from sp
	mov $fp, $sp
	addi $sp, $sp, -8
	sw -4($fp), $a0 ; movsi
	mov $a0, $zero
	sw -8($fp), $a0 ; movsi
.L2:
	lw $a1, -4($fp) ; load word
	lw $a0, -8($fp) ; load word
	add $a0, $a1, $a0
	lb $a0, ($a0)
	lsli $a0, $a0, 24	; asli
	lsri $a0, $a0, 24
	cmp $a0, $zero
	beq .L3
	lw $a0, -8($fp) ; load word
	addi $a0, $a0, 1
	sw -8($fp), $a0 ; movsi
	jmp .L2
.L3:
	lw $a0, -8($fp) ; load word
	mov $v0, $a0
	addi $sp, $sp, 8
	mov $sp, $fp
	lw $fp, 0($sp) ; load form sp
	addi $sp, $sp, 4
	ret
	.size	mini_strlen, .-mini_strlen
	.align 4
	.type	mini_itoa, @function
mini_itoa:
	addi $sp, $sp, -4
	sw 0($sp), $fp	; store from sp
	mov $fp, $sp
	addi $sp, $sp, -52
	sw -4($fp), $a0 ; movsi
	sw -8($fp), $a1 ; movsi
	sw -12($fp), $a2 ; movsi
	sw -16($fp), $a3 ; movsi
	lw $a0, 4($fp) ; load word
	sw -20($fp), $a0 ; movsi
	mov $a0, $zero
	sw -24($fp), $a0 ; movsi
	lw $a0, -8($fp) ; load word
	subi $zero, $a0, 16
	bleu .L5
	sw -44($fp), $zeroa
	jmp .L4
.L5:
	lw $a0, -4($fp) ; load word
	cmp $a0, $zero
	bge .L7
	lw $a0, -16($fp) ; load word
	cmp $a0, $zero
	bne .L7
	ldi $a0, 1
	sw -24($fp), $a0 ; movsi
	lw $a0, -4($fp) ; load word
	sub $a0, $0, $a0	; neg
	sw -4($fp), $a0 ; movsi
.L7:
	lw $a1, -4($fp) ; load word
	lw $a0, -8($fp) ; load word
	modu $a0, $a1, $a0
	sw -36($fp), $a0 ; movsi
	addi $a1, $fp, -20
	lw $a0, ($a1) ; load word
	sw -52($fp), $a0 ; movsi
	addi $a0, $a0, 1
	sw ($a1), $a0 ; movsi
	lw $a0, -36($fp) ; load word
	subi $zero, $a0, 9
	bgt .L10
	lw $a0, -36($fp) ; load word
	sb -48($fp), $a0
	lb $a1, -48($fp)
	addi $a0, $a1, 48
	sb -48($fp), $a0
	jmp .L11
.L10:
	lw $a0, -12($fp) ; load word
	cmp $a0, $zero
	beq .L12
	lw $a0, -36($fp) ; load word
	sb -48($fp), $a0
	lb $a3, -48($fp)
	addi $a0, $a3, 55
	sb -48($fp), $a0
	jmp .L11
.L12:
	lw $a0, -36($fp) ; load word
	sb -48($fp), $a0
	lb $a1, -48($fp)
	addi $a0, $a1, 87
	sb -48($fp), $a0
.L11:
	lb $a0, -48($fp)
	lw $a3, -52($fp) ; load word
	sb ($a3), $a0
	lw $a1, -4($fp) ; load word
	lw $a0, -8($fp) ; load word
	divu $a0, $a1, $a0
	sw -4($fp), $a0 ; movsi
	lw $a0, -4($fp) ; load word
	cmp $a0, $zero
	ble .L8
	jmp .L7
.L8:
	lw $a0, -20($fp) ; load word
	lw $a1, 4($fp) ; load word
	sub $a0, $a0, $a1
	sw -28($fp), $a0 ; movsi
.L14:
	lw $a1, -28($fp) ; load word
	lw $a0, 8($fp) ; load word
	cmp $a1, $a0
	bgeu .L15
	addi $a2, $fp, -20
	lw $a0, ($a2) ; load word
	mov $a1, $a0
	ldi $a3, 48
	sb ($a1), $a3
	addi $a0, $a0, 1
	sw ($a2), $a0 ; movsi
	lw $a0, -28($fp) ; load word
	addi $a0, $a0, 1
	sw -28($fp), $a0 ; movsi
	jmp .L14
.L15:
	lw $a0, -24($fp) ; load word
	cmp $a0, $zero
	beq .L17
	addi $a0, $fp, -20
	lw $a1, ($a0) ; load word
	mov $a2, $a1
	ldi $a3, 45
	sb ($a2), $a3
	addi $a1, $a1, 1
	sw ($a0), $a1 ; movsi
.L17:
	lw $a0, -20($fp) ; load word
	mov $a1, $zero
	sb ($a0), $a1
	lw $a1, -20($fp) ; load word
	lw $a0, 4($fp) ; load word
	sub $a0, $a1, $a0
	sw -32($fp), $a0 ; movsi
	mov $a0, $zero
	sw -28($fp), $a0 ; movsi
.L18:
	lw $a0, -32($fp) ; load word
	lsri $a1, $a0, 1
	lw $a0, -28($fp) ; load word
	cmp $a1, $a0
	bleu .L19
	lw $a1, 4($fp) ; load word
	lw $a0, -28($fp) ; load word
	add $a0, $a1, $a0
	lb $a0, ($a0)
	sb -37($fp), $a0
	lw $a1, 4($fp) ; load word
	lw $a0, -28($fp) ; load word
	add $a2, $a1, $a0
	lw $a1, -32($fp) ; load word
	lw $a0, -28($fp) ; load word
	sub $a1, $a1, $a0
	lw $a0, 4($fp) ; load word
	add $a0, $a1, $a0
	addi $a0, $a0, -1
	lb $a0, ($a0)
	sb ($a2), $a0
	lw $a1, -32($fp) ; load word
	lw $a0, -28($fp) ; load word
	sub $a1, $a1, $a0
	lw $a0, 4($fp) ; load word
	add $a0, $a1, $a0
	addi $a0, $a0, -1
	lb $a3, -37($fp)
	sb ($a0), $a3
	lw $a0, -28($fp) ; load word
	addi $a0, $a0, 1
	sw -28($fp), $a0 ; movsi
	jmp .L18
.L19:
	lw $a0, -32($fp) ; load word
	sw -44($fp), $a0 ; movsi
.L4:
	lw $v0, -44($fp) ; load word
	addi $sp, $sp, 52
	mov $sp, $fp
	lw $fp, 0($sp) ; load form sp
	addi $sp, $sp, 4
	ret
	.size	mini_itoa, .-mini_itoa
	.align 4
	.type	_putc, @function
_putc:
	addi $sp, $sp, -4
	sw 0($sp), $fp	; store from sp
	mov $fp, $sp
	addi $sp, $sp, -12
	sw -4($fp), $a0 ; movsi
	sw -8($fp), $a1 ; movsi
	lw $a0, -8($fp) ; load word
	lw $a1, -8($fp) ; load word
	lw $a2, 4($a0) ; load word
	lw $a0, ($a1) ; load word
	sub $a0, $a2, $a0
	addi $a1, $a0, 1
	lw $a0, -8($fp) ; load word
	lw $a0, 8($a0) ; load word
	cmp $a1, $a0
	bltu .L22
	sw -12($fp), $zeroa
	jmp .L21
.L22:
	lw $a0, -8($fp) ; load word
	addi $a3, $a0, 4
	lw $a1, ($a3) ; load word
	lw $a0, -4($fp) ; load word
	mov $a2, $a0	; movb
	mov $a0, $a1
	sb ($a0), $a2
	addi $a1, $a1, 1
	sw ($a3), $a1 ; movsi
	lw $a0, -8($fp) ; load word
	lw $a0, 4($a0) ; load word
	mov $a1, $zero
	sb ($a0), $a1
	ldi $a0, 1
	sw -12($fp), $a0 ; movsi
.L21:
	lw $v0, -12($fp) ; load word
	addi $sp, $sp, 12
	mov $sp, $fp
	lw $fp, 0($sp) ; load form sp
	addi $sp, $sp, 4
	ret
	.size	_putc, .-_putc
	.align 4
	.type	_puts, @function
_puts:
	addi $sp, $sp, -4
	sw 0($sp), $fp	; store from sp
	mov $fp, $sp
	addi $sp, $sp, -16
	sw -4($fp), $a0 ; movsi
	sw -8($fp), $a1 ; movsi
	sw -12($fp), $a2 ; movsi
	lw $a3, -12($fp) ; load word
	lw $a0, -12($fp) ; load word
	lw $a1, -12($fp) ; load word
	lw $a2, 4($a0) ; load word
	lw $a0, ($a1) ; load word
	sub $a1, $a2, $a0
	lw $a0, 8($a3) ; load word
	sub $a0, $a0, $a1
	addi $a1, $a0, -1
	lw $a0, -8($fp) ; load word
	cmp $a1, $a0
	bgeu .L24
	lw $a3, -12($fp) ; load word
	lw $a0, -12($fp) ; load word
	lw $a1, -12($fp) ; load word
	lw $a2, 4($a0) ; load word
	lw $a0, ($a1) ; load word
	sub $a1, $a2, $a0
	lw $a0, 8($a3) ; load word
	sub $a0, $a0, $a1
	addi $a0, $a0, -1
	sw -8($fp), $a0 ; movsi
.L24:
	mov $a0, $zero
	sw -16($fp), $a0 ; movsi
.L25:
	lw $a1, -16($fp) ; load word
	lw $a0, -8($fp) ; load word
	cmp $a1, $a0
	bgeu .L26
	lw $a0, -12($fp) ; load word
	addi $a3, $a0, 4
	lw $a1, ($a3) ; load word
	lw $a2, -4($fp) ; load word
	lw $a0, -16($fp) ; load word
	add $a2, $a2, $a0
	mov $a0, $a1
	lb $a2, ($a2)
	sb ($a0), $a2
	addi $a1, $a1, 1
	sw ($a3), $a1 ; movsi
	lw $a0, -16($fp) ; load word
	addi $a0, $a0, 1
	sw -16($fp), $a0 ; movsi
	jmp .L25
.L26:
	lw $a0, -12($fp) ; load word
	lw $a0, 4($a0) ; load word
	mov $a1, $zero
	sb ($a0), $a1
	lw $a0, -8($fp) ; load word
	mov $v0, $a0
	addi $sp, $sp, 16
	mov $sp, $fp
	lw $fp, 0($sp) ; load form sp
	addi $sp, $sp, 4
	ret
	.size	_puts, .-_puts
	.align 4
	.globl	mini_vsnprintf
	.type	mini_vsnprintf, @function
mini_vsnprintf:
	addi $sp, $sp, -8
	sw 0($sp), $fp	; store from sp
	sw 4($sp), $ra	; store from sp
	mov $fp, $sp
	addi $sp, $sp, -92
	sw -4($fp), $a0 ; movsi
	sw -8($fp), $a1 ; movsi
	sw -12($fp), $a2 ; movsi
	sw -16($fp), $a3 ; movsi
	lw $a0, -4($fp) ; load word
	sw -28($fp), $a0 ; movsi
	lw $a0, -4($fp) ; load word
	sw -24($fp), $a0 ; movsi
	lw $a0, -8($fp) ; load word
	sw -20($fp), $a0 ; movsi
.L29:
	addi $a2, $fp, -12
	lw $a0, ($a2) ; load word
	mov $a1, $a0
	lb $a1, ($a1)
	addi $a0, $a0, 1
	sw ($a2), $a0 ; movsi
	sb -53($fp), $a1
	lsli $a0, $a1, 24	; asli
	lsri $a0, $a0, 24
	cmp $a0, $zero
	beq .L36
	lw $a1, -24($fp) ; load word
	lw $a0, -28($fp) ; load word
	sub $a0, $a1, $a0
	addi $a1, $a0, 1
	lw $a0, -20($fp) ; load word
	cmp $a1, $a0
	bltu .L31
	jmp .L36
.L31:
	lb $a0, -53($fp)
	lsli $a0, $a0, 24	; asli
	lsri $a0, $a0, 24
	subi $zero, $a0, 37
	beq .L32
	lb $a0, -53($fp)
	lsli $a0, $a0, 24	; asli
	lsri $a0, $a0, 24
	addi $a1, $fp, -28
	jal _putc
	jmp .L29
.L32:
	mov $a0, $zero
	sb -54($fp), $a0
	addi $a1, $fp, -12
	lw $a0, ($a1) ; load word
	lb $a2, ($a0)
	addi $a0, $a0, 1
	sw ($a1), $a0 ; movsi
	sb -53($fp), $a2
	lb $a0, -53($fp)
	lsli $a0, $a0, 24	; asli
	lsri $a0, $a0, 24
	subi $zero, $a0, 48
	bne .L34
	addi $a1, $fp, -12
	lw $a0, ($a1) ; load word
	lb $a2, ($a0)
	addi $a0, $a0, 1
	sw ($a1), $a0 ; movsi
	sb -53($fp), $a2
	lb $a0, -53($fp)
	lsli $a0, $a0, 24	; asli
	lsri $a0, $a0, 24
	cmp $a0, $zero
	bne .L35
	jmp .L36
.L35:
	lb $a0, -53($fp)
	lsli $a0, $a0, 24	; asli
	lsri $a0, $a0, 24
	subi $zero, $a0, 47
	bleu .L37
	lb $a0, -53($fp)
	lsli $a0, $a0, 24	; asli
	lsri $a0, $a0, 24
	subi $zero, $a0, 57
	bgtu .L37
	lb $a0, -53($fp)
	addi $a0, $a0, -48
	sb -54($fp), $a0
.L37:
	addi $a0, $fp, -12
	lw $a1, ($a0) ; load word
	lb $a2, ($a1)
	addi $a1, $a1, 1
	sw ($a0), $a1 ; movsi
	sb -53($fp), $a2
.L34:
	lb $a0, -53($fp)
	lsli $a0, $a0, 24	; asli
	sw -84($fp), $a0 ; movsi
	lw $a1, -84($fp) ; load word
	lsri $a1, $a1, 24
	sw -84($fp), $a1 ; movsi
	lw $a0, -84($fp) ; load word
	subi $zero, $a0, 100
	beq .L41
	lw $a1, -84($fp) ; load word
	subi $zero, $a1, 100
	bgt .L49
	lw $a0, -84($fp) ; load word
	subi $zero, $a0, 88
	beq .L44
	lw $a1, -84($fp) ; load word
	subi $zero, $a1, 88
	bgt .L50
	lw $a0, -84($fp) ; load word
	cmp $a0, $zero
	beq .L36
	jmp .L48
.L50:
	lw $a1, -84($fp) ; load word
	subi $zero, $a1, 99
	beq .L46
	jmp .L48
.L49:
	lw $a0, -84($fp) ; load word
	subi $zero, $a0, 117
	beq .L41
	lw $a1, -84($fp) ; load word
	subi $zero, $a1, 117
	bgt .L51
	lw $a0, -84($fp) ; load word
	subi $zero, $a0, 115
	beq .L47
	jmp .L48
.L51:
	lw $a1, -84($fp) ; load word
	subi $zero, $a1, 120
	beq .L44
	jmp .L48
.L41:
	lw $a0, -16($fp) ; load word
	sw -68($fp), $a0 ; movsi
	lw $a0, -16($fp) ; load word
	addi $a0, $a0, 4
	sw -16($fp), $a0 ; movsi
	sw -72($fp), $zeroa
	lb $a0, -53($fp)
	lsli $a0, $a0, 24	; asli
	lsri $a0, $a0, 24
	subi $zero, $a0, 117
	bne .L42
	ldi $a1, 1
	sw -72($fp), $a1 ; movsi
.L42:
	addi $a0, $fp, -52
	sw ($sp), $a0 ; movsi
	lb $a0, -54($fp)
	lsli $a0, $a0, 24	; asli
	lsri $a0, $a0, 24
	sw 4($sp), $a0	; store from sp
	lw $a1, -68($fp) ; load word
	lw $a0, ($a1) ; load word
	ldi $a1, 10
	mov $a2, $zero
	lw $a3, -72($fp) ; load word
	jal mini_itoa
	mov $a0, $v0
	sw -64($fp), $a0 ; movsi
	addi $a0, $fp, -52
	addi $a2, $fp, -28
	lw $a1, -64($fp) ; load word
	jal _puts
	jmp .L29
.L44:
	lw $a0, -16($fp) ; load word
	sw -76($fp), $a0 ; movsi
	lw $a0, -16($fp) ; load word
	addi $a0, $a0, 4
	sw -16($fp), $a0 ; movsi
	sw -80($fp), $zeroa
	lb $a0, -53($fp)
	lsli $a0, $a0, 24	; asli
	lsri $a0, $a0, 24
	subi $zero, $a0, 88
	bne .L45
	ldi $a1, 1
	sw -80($fp), $a1 ; movsi
.L45:
	addi $a0, $fp, -52
	sw ($sp), $a0 ; movsi
	lb $a0, -54($fp)
	lsli $a0, $a0, 24	; asli
	lsri $a0, $a0, 24
	sw 4($sp), $a0	; store from sp
	lw $a1, -76($fp) ; load word
	lw $a0, ($a1) ; load word
	ldi $a1, 16
	lw $a2, -80($fp) ; load word
	ldi $a3, 1
	jal mini_itoa
	mov $a0, $v0
	sw -64($fp), $a0 ; movsi
	addi $a0, $fp, -52
	addi $a2, $fp, -28
	lw $a1, -64($fp) ; load word
	jal _puts
	jmp .L29
.L46:
	lw $a1, -16($fp) ; load word
	lw $a0, -16($fp) ; load word
	addi $a0, $a0, 4
	sw -16($fp), $a0 ; movsi
	lw $a0, ($a1) ; load word
	lsli $a0, $a0, 24	; asli
	lsri $a0, $a0, 24
	addi $a1, $fp, -28
	jal _putc
	jmp .L29
.L47:
	lw $a1, -16($fp) ; load word
	lw $a0, -16($fp) ; load word
	addi $a0, $a0, 4
	sw -16($fp), $a0 ; movsi
	lw $a0, ($a1) ; load word
	sw -60($fp), $a0 ; movsi
	lw $a0, -60($fp) ; load word
	jal mini_strlen
	mov $a1, $v0
	addi $a2, $fp, -28
	lw $a0, -60($fp) ; load word
	jal _puts
	jmp .L29
.L48:
	lb $a0, -53($fp)
	lsli $a0, $a0, 24	; asli
	lsri $a0, $a0, 24
	addi $a1, $fp, -28
	jal _putc
	jmp .L29
.L36:
	lw $a1, -24($fp) ; load word
	lw $a0, -28($fp) ; load word
	sub $a0, $a1, $a0
	mov $v0, $a0
	addi $sp, $sp, 92
	mov $sp, $fp
	lw $fp, 0($sp) ; load form sp
	lw $ra, 4($sp) ; load form sp
	addi $sp, $sp, 8
	ret
	.size	mini_vsnprintf, .-mini_vsnprintf
	.align 4
	.globl	mini_snprintf
	.type	mini_snprintf, @function
mini_snprintf:
	addi $sp, $sp, -8
	sw 0($sp), $fp	; store from sp
	sw 4($sp), $ra	; store from sp
	mov $fp, $sp
	addi $sp, $sp, -16
	sw -4($fp), $a0 ; movsi
	sw -8($fp), $a1 ; movsi
	addi $a0, $fp, 12
	sw -16($fp), $a0 ; movsi
	lw $a0, -4($fp) ; load word
	lw $a1, -8($fp) ; load word
	lw $a2, 8($fp) ; load word
	lw $a3, -16($fp) ; load word
	jal mini_vsnprintf
	mov $a0, $v0
	sw -12($fp), $a0 ; movsi
	lw $a0, -12($fp) ; load word
	mov $v0, $a0
	addi $sp, $sp, 16
	mov $sp, $fp
	lw $fp, 0($sp) ; load form sp
	lw $ra, 4($sp) ; load form sp
	addi $sp, $sp, 8
	ret
	.size	mini_snprintf, .-mini_snprintf
	.ident	"GCC: (GNU) 3.4.2"
