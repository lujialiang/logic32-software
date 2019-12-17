	.text
	.align 32
	.global	main
main:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	addk -20, b15
	mvkl 65536, a0
	mvkh 65536, a0
	stw a0, *-a15(4)
	mvkl -1, a0
	sth a0, *-a15(10)
	mvkl 7, a0
	sth a0, *-a15(12)
	mvkl 3, a0
	stb a0, *-a15(13)
	mvkl 5, a0
	stb a0, *-a15(14)
	mvkl 1234, a0
	stw a0, *-a15(20)
	mvkl 0, a0
	mv a0, a4
	mvkl -20, a14
	add a15, a14, b15
	mv a15, b15
	ldw *b15, a15
	nop 4
	add 4, b15, b15
	ldw *b15, a14
	nop 4
	add 4, b15, b15
	b .S2 a14 
	nop 5
