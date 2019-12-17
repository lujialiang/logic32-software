	.text
	.align 32
	.global	main
main:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	add -8, b15, b15
	mvkl 2, a0
	stw a0, *-a15(4)
	ldw *-a15(4), b1
	nop 4
	mvkl 6, b2
	cmpgtu .L2 b1, b2, b0
	[b0] mvkl .S2 L9, b1 
	[b0] mvkh .S2 L9, b1 
	[b0] b .S2 b1 
	nop 5
	ldw *-a15(4), a0
	nop 4
	shl a0, 2, a0
	mvkl L10, b2
	mvkh L10, b2
	add a0, b2, a0
	ldw *a0, b0
	nop 4
	b .S2 b0 
	nop 5
	.align 4
L10:
	.long	L9
	.long	L3
	.long	L4
	.long	L5
	.long	L6
	.long	L7
	.long	L8
L3:
	mvkl 1, a0
	stw a0, *-a15(8)
	b L2 
	nop 5
L4:
	mvkl 2, a1
	stw a1, *-a15(8)
	b L2 
	nop 5
L5:
	mvkl 3, b1
	stw b1, *-a15(8)
	b L2 
	nop 5
L6:
	mvkl 4, b2
	stw b2, *-a15(8)
	b L2 
	nop 5
L7:
	mvkl 5, a0
	stw a0, *-a15(8)
	b L2 
	nop 5
L8:
	mvkl 6, a1
	stw a1, *-a15(8)
	b L2 
	nop 5
L9:
	mvkl -1, b1
	stw b1, *-a15(8)
L2:
	mvkl 0, a0
	mv a0, a4
	add -8, a15, b15
	mv a15, b15
	ldw *b15, a15
	nop 4
	add 4, b15, b15
	ldw *b15, a14
	nop 4
	add 4, b15, b15
	b .S2 a14 
	nop 5
