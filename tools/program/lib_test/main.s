.globl _main
.text
.text
.align 4
_main:
;.frame $sp,56,$31
add $sp,$sp,-56
;.mask 0xc0c00000,-28
sw $22,16($sp)
sw $23,20($sp)
sw $30,24($sp)
sw $31,28($sp)
l.d $f18,L.2
s.d $f18,-8+56($sp)
l.d $f18,L.3
s.d $f18,-16+56($sp)
l.d $f18,-8+56($sp)
l.d $f16,-16+56($sp)
add.d $f10,$f18,$f16
s.d $f10,-24+56($sp)
lea $30,-8+56($sp)
lea $23,-16+56($sp)
lea $22,-24+56($sp)
lea $4,L.4
cvt.w.d $f2,$f18
mfc1 $5,$f2
cvt.w.d $f2,$f16
mfc1 $6,$f2
l.d $f18,-24+56($sp)
cvt.w.d $f2,$f18
mfc1 $7,$f2
jal _printf
lea $4,L.5
lw $5,($30)
lw $6,($23)
lw $7,($22)
jal _printf
lea $4,L.5
lw $5,4($30)
lw $6,4($23)
lw $7,4($22)
jal _printf
mov $2,$0
L.1:
lw $22,16($sp)
lw $23,20($sp)
lw $30,24($sp)
lw $31,28($sp)
add $sp,$sp,56
jmp $31
.extern _puthex
.extern _putint
.extern _printf
.extern _puts
.extern _putchar
.rdata
.align 1
L.5:
.ubyte 37
.ubyte 120
.ubyte 44
.ubyte 32
.ubyte 37
.ubyte 120
.ubyte 44
.ubyte 32
.ubyte 37
.ubyte 120
.ubyte 10
.ubyte 0
.align 1
L.4:
.ubyte 37
.ubyte 100
.ubyte 44
.ubyte 32
.ubyte 37
.ubyte 100
.ubyte 44
.ubyte 32
.ubyte 37
.ubyte 100
.ubyte 10
.ubyte 0
.align 8
L.3:
.word 0xae147ae1
.word 0x4010e147
.align 8
L.2:
.word 0x1eb851ec
.word 0x4010eb85
