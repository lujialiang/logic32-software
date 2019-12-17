	.text
	.align 32
	.global	main
main:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	add -4, b15, b15
	mvkl 7, a0
	stw a0, *-a15(4)
	mvkl 3, a0
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
