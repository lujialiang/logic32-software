/* 
 * Copyright (C) 1999, 2000, 2001, 2002 Stephane Carrez (stcarrez@nerim.fr)	
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions. No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 */


	.text
	
	.global	setjmp
	.globl	longjmp

setjmp:
	sw 0($4),$1
	;sw 4($4),$2
	sw 8($4),$3
	;sw 12($4),$4
	;sw 16($4),$5
	sw 20($4),$6
	sw 24($4),$7
	sw 28($4),$8
	sw 32($4),$9
	sw 36($4),$10
	sw 40($4),$11
	sw 44($4),$12
	sw 48($4),$13
	sw 52($4),$14
	sw 56($4),$15
	sw 60($4),$16
	sw 64($4),$17
	sw 68($4),$18
	sw 72($4),$19
	sw 76($4),$20
	sw 80($4),$21
	sw 84($4),$22
	sw 88($4),$23
	sw 92($4),$24
	sw 96($4),$25
	sw 100($4),$26
	sw 104($4),$27
	sw 108($4),$28
	sw 112($4),$29
	sw 116($4),$30
	sw 120($4),$31
	ldi $2, 0		; return 0
	ret

; arguments are buffer + return value
longjmp:
	lw $1, 0($4)
	;lw $2, 4($4)
	lw $3, 8($4)
	;lw $4, 12($4)
	;lw $5, 16($4)
	lw $6, 20($4)
	lw $7, 24($4)
	lw $8, 28($4)
	lw $9, 32($4)
	lw $10, 36($4)
	lw $11, 40($4)
	lw $12, 44($4)
	lw $13, 48($4)
	lw $14, 52($4)
	lw $15, 56($4)
	lw $16, 60($4)
	lw $17, 64($4)
	lw $18, 68($4)
	lw $19, 72($4)
	lw $20, 76($4)
	lw $21, 80($4)
	lw $22, 84($4)
	lw $23, 88($4)
	lw $24, 92($4)
	lw $25, 96($4)
	lw $26, 100($4)
	lw $27, 104($4)
	lw $28, 108($4)
	lw $29, 112($4)
	lw $30, 116($4)
	lw $31, 120($4)
	mov $2, $5
	cmp $5, $0
	bne 1f
	ldi	$2, 1
1:
	ret

