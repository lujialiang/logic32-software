	.text
	.align 32
	.global	times2
times2:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	ldw *+a15(8), a1
	nop 4
	ldw *+a15(8), a1
	nop 4
	add a1, a1, a0
	mv a0, a4
	mv a15, b15
	mv a15, b15
	ldw *b15, a15
	nop 4
	add 4, b15, b15
	ldw *b15, a14
	nop 4
	add 4, b15, b15
	b .S2 a14 
	nop 5
	.align 32
	.global	nop
nop:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	mv a15, b15
	mv a15, b15
	ldw *b15, a15
	nop 4
	add 4, b15, b15
	ldw *b15, a14
	nop 4
	add 4, b15, b15
	b .S2 a14 
	nop 5
	.align 32
	.global	main
main:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	add -4, b15, b15
	add -4, b15, b15
	mvkl -3, a0
	stw a0, *-a15(4)
	ldw *-a15(4), a0
	nop 4
	stw a0, *b15
	mvkl L4, a0
	mvkh L4, a0
	add -4, b15, b15
	stw a0, *b15
	mvkl times2, a0
	mvkh times2, a0
	b a0
	nop 5
L4:
	mv a4, a0
	stw a0, *-a15(4)
	mvkl L5, a0
	mvkh L5, a0
	add -4, b15, b15
	stw a0, *b15
	mvkl nop, a0
	mvkh nop, a0
	b a0
	nop 5
L5:
	ldw *-a15(4), a0
	nop 4
	mv a0, a4
	add -4, a15, b15
	mv a15, b15
	ldw *b15, a15
	nop 4
	add 4, b15, b15
	ldw *b15, a14
	nop 4
	add 4, b15, b15
	b .S2 a14 
	nop 5
