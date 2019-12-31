	.text
	.align 32
	.global	gt
gt:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	add -4, b15, b15
	mvkl 0, a0
	stw a0, *-a15(4)
	ldw *+a15(8), b1
	nop 4
	ldw *+a15(12), b2
	nop 4
	cmpgt .L2 b1, b2, b0
	[!b0] mvkl .S2 L2, b1 
	[!b0] mvkh .S2 L2, b1 
	[!b0] b .S2 b1 
	nop 5
	mvkl 1, b2
	stw b2, *-a15(4)
L2:
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
	.align 32
	.global	lt
lt:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	add -4, b15, b15
	mvkl 0, a0
	stw a0, *-a15(4)
	ldw *+a15(8), b1
	nop 4
	ldw *+a15(12), b2
	nop 4
	cmplt .L2 b1, b2, b0
	[!b0] mvkl .S2 L4, b1 
	[!b0] mvkh .S2 L4, b1 
	[!b0] b .S2 b1 
	nop 5
	mvkl 1, b2
	stw b2, *-a15(4)
L4:
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
	.align 32
	.global	ge
ge:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	add -4, b15, b15
	mvkl 0, a0
	stw a0, *-a15(4)
	ldw *+a15(8), b1
	nop 4
	ldw *+a15(12), b2
	nop 4
	cmplt .L2 b1, b2, b0
	[b0] mvkl .S2 L6, b1 
	[b0] mvkh .S2 L6, b1 
	[b0] b .S2 b1 
	nop 5
	mvkl 1, b2
	stw b2, *-a15(4)
L6:
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
	.align 32
	.global	le
le:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	add -4, b15, b15
	mvkl 0, a0
	stw a0, *-a15(4)
	ldw *+a15(8), b1
	nop 4
	ldw *+a15(12), b2
	nop 4
	cmpgt .L2 b1, b2, b0
	[b0] mvkl .S2 L8, b1 
	[b0] mvkh .S2 L8, b1 
	[b0] b .S2 b1 
	nop 5
	mvkl 1, b2
	stw b2, *-a15(4)
L8:
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
	.align 32
	.global	eq
eq:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	add -4, b15, b15
	mvkl 0, a0
	stw a0, *-a15(4)
	ldw *+a15(8), b1
	nop 4
	ldw *+a15(12), b2
	nop 4
	cmpeq .L2 b1, b2, b0
	[!b0] mvkl .S2 L10, b1 
	[!b0] mvkh .S2 L10, b1 
	[!b0] b .S2 b1 
	nop 5
	mvkl 1, b2
	stw b2, *-a15(4)
L10:
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
	.align 32
	.global	ne
ne:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	add -4, b15, b15
	mvkl 0, a0
	stw a0, *-a15(4)
	ldw *+a15(8), b1
	nop 4
	ldw *+a15(12), b2
	nop 4
	cmpeq .L2 b1, b2, b0
	[b0] mvkl .S2 L12, b1 
	[b0] mvkh .S2 L12, b1 
	[b0] b .S2 b1 
	nop 5
	mvkl 1, b2
	stw b2, *-a15(4)
L12:
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
	.align 32
	.global	gt_u
gt_u:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	add -4, b15, b15
	mvkl 0, a0
	stw a0, *-a15(4)
	ldw *+a15(8), b1
	nop 4
	ldw *+a15(12), b2
	nop 4
	cmpgtu .L2 b1, b2, b0
	[!b0] mvkl .S2 L14, b1 
	[!b0] mvkh .S2 L14, b1 
	[!b0] b .S2 b1 
	nop 5
	mvkl 1, b2
	stw b2, *-a15(4)
L14:
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
	.align 32
	.global	lt_u
lt_u:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	add -4, b15, b15
	mvkl 0, a0
	stw a0, *-a15(4)
	ldw *+a15(8), b1
	nop 4
	ldw *+a15(12), b2
	nop 4
	cmpltu .L2 b1, b2, b0
	[!b0] mvkl .S2 L16, b1 
	[!b0] mvkh .S2 L16, b1 
	[!b0] b .S2 b1 
	nop 5
	mvkl 1, b2
	stw b2, *-a15(4)
L16:
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
	.align 32
	.global	ge_u
ge_u:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	add -4, b15, b15
	mvkl 0, a0
	stw a0, *-a15(4)
	ldw *+a15(8), b1
	nop 4
	ldw *+a15(12), b2
	nop 4
	cmpltu .L2 b1, b2, b0
	[b0] mvkl .S2 L18, b1 
	[b0] mvkh .S2 L18, b1 
	[b0] b .S2 b1 
	nop 5
	mvkl 1, b2
	stw b2, *-a15(4)
L18:
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
	.align 32
	.global	le_u
le_u:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	add -4, b15, b15
	mvkl 0, a0
	stw a0, *-a15(4)
	ldw *+a15(8), b1
	nop 4
	ldw *+a15(12), b2
	nop 4
	cmpgtu .L2 b1, b2, b0
	[b0] mvkl .S2 L20, b1 
	[b0] mvkh .S2 L20, b1 
	[b0] b .S2 b1 
	nop 5
	mvkl 1, b2
	stw b2, *-a15(4)
L20:
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
	.align 32
	.global	eq_u
eq_u:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	add -4, b15, b15
	mvkl 0, a0
	stw a0, *-a15(4)
	ldw *+a15(8), b1
	nop 4
	ldw *+a15(12), b2
	nop 4
	cmpeq .L2 b1, b2, b0
	[!b0] mvkl .S2 L22, b1 
	[!b0] mvkh .S2 L22, b1 
	[!b0] b .S2 b1 
	nop 5
	mvkl 1, b2
	stw b2, *-a15(4)
L22:
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
	.align 32
	.global	ne_u
ne_u:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	add -4, b15, b15
	mvkl 0, a0
	stw a0, *-a15(4)
	ldw *+a15(8), b1
	nop 4
	ldw *+a15(12), b2
	nop 4
	cmpeq .L2 b1, b2, b0
	[b0] mvkl .S2 L24, b1 
	[b0] mvkh .S2 L24, b1 
	[b0] b .S2 b1 
	nop 5
	mvkl 1, b2
	stw b2, *-a15(4)
L24:
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
	.align 32
	.global	main
main:
	add -4, b15, b15
	stw a15, *b15
	mv b15, a15
	mvkl 0, a0
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
