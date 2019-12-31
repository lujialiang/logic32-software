	.text
	.align 32
	.global	testSI
testSI:
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
	.global	testHI
testHI:
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
	add -8, b15, b15
	add -4, b15, b15
	mvkl 7, a0
	sth a0, *-a15(6)
	ldh *-a15(6), a0
	nop 4
	shl a0, 16, a0
	stw a0, *b15
	ldw *b15, a0
	nop 4
	shr a0, 16, a0
	stw a0, *b15
	mvkl L4, a0
	mvkh L4, a0
	add -4, b15, b15
	stw a0, *b15
	mvkl testSI, a0
	mvkh testSI, a0
	b a0
	nop 5
L4:
	mvkl 5, a0
	stb a0, *-a15(7)
	ldb *-a15(7), a0
	nop 4
	shl a0, 24, a0
	stw a0, *b15
	ldw *b15, a0
	nop 4
	shr a0, 24, a0
	stw a0, *b15
	mvkl L5, a0
	mvkh L5, a0
	add -4, b15, b15
	stw a0, *b15
	mvkl testSI, a0
	mvkh testSI, a0
	b a0
	nop 5
L5:
	mvkl 9, a0
	stw a0, *-a15(4)
	ldh *-a15(4), a0
	nop 4
	sth a0, *b15
	mvkl L6, a0
	mvkh L6, a0
	add -4, b15, b15
	stw a0, *b15
	mvkl testHI, a0
	mvkh testHI, a0
	b a0
	nop 5
L6:
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
