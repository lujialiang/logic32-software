	.text
	.align 32
	.global	main
main:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	add -4, b15, b15
	mvkl 1, a0
	stw a0, *-a15(4)
L2:
	ldw *-a15(4), a0
	nop 4
	add 1, a0, a0
	stw a0, *-a15(4)
	b L2 
	nop 5
