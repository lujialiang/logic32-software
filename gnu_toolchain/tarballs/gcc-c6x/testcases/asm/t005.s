	.text
	.align 32
	.global	add
add:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	ldw *+a15(8), a1
	nop 4
	ldw *+a15(12), a2
	nop 4
	add a1, a2, a0
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
	.global	main
main:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	add -8, b15, b15
	add -8, b15, b15
	mvkl 100, a0
	stw a0, *-a15(4)
	mvkl 100000, a0
	mvkh 100000, a0
	stw a0, *-a15(8)
	ldw *-a15(4), a0
	nop 4
	stw a0, *b15
	ldw *-a15(8), a0
	nop 4
	stw a0, *+b15(4)
	mvkl L3, a0
	mvkh L3, a0
	add -4, b15, b15
	stw a0, *b15
	mvkl add, a0
	mvkh add, a0
	b a0
	nop 5
L3:
	mv a4, a0
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
