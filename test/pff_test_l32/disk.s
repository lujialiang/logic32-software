	.file	"disk.c"
	; extern	puts
	.section	.text
	.align 4
	.globl	disk_readp0
	.type	disk_readp0, @function
disk_readp0:
	addi $sp, $sp, -4
	sw 0($sp), $t0	; store from sp
	mov $s0, $a0
	ldi $a2, -2113929212
	sw ($a2), $a1 ; movsi
	ldi $a2, -2113929216
	lw $a0, ($a2) ; load word
	ori $a0, $a0, 8
	sw ($a2), $a0 ; movsi
	ldi $a2, -2113929204
	ldi $a0, 0
	sw ($a2), $a0 ; movsi
	ldi $a2, -2113929196
	mov $a1, $a0
	ldi $a3, 511
.L13:
	add $a0, $s0, $a1
	lb $t0, ($a2)
	sb ($a0), $t0
	addi $a1, $a1, 1
	cmp $a1,$a3
	ble .L13
	ldi $v0, 0
	lw $t0, 0($sp) ; load form sp
	addi $sp, $sp, 4
	ret
	.size	disk_readp0, .-disk_readp0
	.align 4
	.globl	disk_writep0
	.type	disk_writep0, @function
disk_writep0:
	mov $a3, $a0
	ldi $a2, -2113929208
	sw ($a2), $a1 ; movsi
	ldi $a2, -2113929200
	ldi $a0, 0
	sw ($a2), $a0 ; movsi
	ldi $a2, -2113929196
	mov $a1, $a0
	ldi $s0, 511
.L25:
	add $a0, $a3, $a1
	lb $a0, ($a0)
	lsli $a0, $a0, 24	; asli
	lsri $a0, $a0, 24
	sw ($a2), $a0 ; movsi
	addi $a1, $a1, 1
	cmp $a1,$s0
	ble .L25
	ldi $a2, -2113929216
	lw $a0, ($a2) ; load word
	ori $a0, $a0, 16
	sw ($a2), $a0 ; movsi
	ldi $v0, 0
	ret
	.size	disk_writep0, .-disk_writep0
	.ident	"GCC: (GNU) 3.4.2"
