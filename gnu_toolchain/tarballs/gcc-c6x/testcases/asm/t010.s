	.text
	.align 32
	.global	main
main:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	add -8, b15, b15
	mvkl 5, a0
	stw a0, *-a15(4)
L2:
	ldw *-a15(4), b1
	nop 4
	mvkl 9, b2
	cmpgt .L2 b1, b2, b0
	[!b0] mvkl .S2 L5, b1 
	[!b0] mvkh .S2 L5, b1 
	[!b0] b .S2 b1 
	nop 5
	b L3 
	nop 5
L5:
	ldw *-a15(4), b2
	nop 4
	stw b2, *-a15(8)
	ldw *-a15(4), a0
	nop 4
	add 2, a0, a0
	stw a0, *-a15(4)
	b L2 
	nop 5
L3:
	mvkl 5, b1
	stw b1, *-a15(4)
L6:
	ldw *-a15(4), b2
	nop 4
	mvkl 9, b1
	cmpgt .L2 b2, b1, b0
	[!b0] mvkl .S2 L8, b2 
	[!b0] mvkh .S2 L8, b2 
	[!b0] b .S2 b2 
	nop 5
	b L7 
	nop 5
L8:
	ldw *-a15(4), a0
	nop 4
	stw a0, *-a15(8)
	ldw *-a15(4), b1
	nop 4
	add 2, b1, b1
	stw b1, *-a15(4)
	b L6 
	nop 5
L7:
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
