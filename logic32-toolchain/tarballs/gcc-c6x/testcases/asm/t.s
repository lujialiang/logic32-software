	.text
	.align 32
	.global	main
main:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	addk -32, b15
	add -4, b15, b15
	stw a10, *b15
	add -4, b15, b15
	stw a11, *b15
	add -8, b15, b15
	mvkl 1, a0
	stw a0, *-a15(4)
	mvkl 100000, a1
	mvkh 100000, a1
	stw a1, *-a15(8)
	mvkl 3, a0
	stw a0, *-a15(12)
	mvkl 4, a1
	stw a1, *-a15(16)
	mvkl 5, a0
	stw a0, *-a15(20)
	mvkl 6, a1
	stw a1, *-a15(24)
	mvkl 7, a0
	stw a0, *-a15(28)
	mvkl -32, a14
	add a15, a14, a11
	ldw *-a15(4), a1
	nop 4
	ldw *-a15(8), a0
	nop 4
	add a1, a0, a10
	mvkl L2, a0
	mvkh L2, a0
	mvkl 2, a1
	stw a1, *b15
	ldw *-a15(12), a1
	nop 4
	stw a1, *+b15(4)
	add -4, b15, b15
	stw a0, *b15
	mvkl __subsi3, a0
	mvkh __subsi3, a0
	b a0
	nop 5
L2:
	mv a4, a0
	shl a10, a0, a0
	mvkl L3, a1
	mvkh L3, a1
	stw a0, *b15
	ldw *-a15(16), a0
	nop 4
	stw a0, *+b15(4)
	add -4, b15, b15
	stw a1, *b15
	mvkl __mulsi3, a1
	mvkh __mulsi3, a1
	b a1
	nop 5
L3:
	mv a4, a0
	stw a0, *a11
	mvkl 0, a0
	mv a0, a4
	mvkl -40, a14
	add a15, a14, b15
	ldw *b15, a11
	nop 4
	add 4, b15, b15
	ldw *b15, a10
	nop 4
	add 4, b15, b15
	mv a15, b15
	ldw *b15, a15
	nop 4
	add 4, b15, b15
	ldw *b15, a14
	nop 4
	add 4, b15, b15
	b .S2 a14 
	nop 5
