.globl _delay
.text
.text
.align 4
_delay:
;.frame $sp,0,$31
brn L.3
L.2:
sub $4,$4,1
L.3:
cmp $4,$0
bne L.2
L.1:
jmp $31
.globl _main
.text
.align 4
_main:
;.frame $sp,24,$31
add $sp,$sp,-24
;.mask 0xc0000000,-4
sw $30,16($sp)
sw $31,20($sp)
lea $30,2750
lea $4,L.6
jal _puts
jal _seti
brn L.8
L.7:
lea $4,L.10
mov $25,$30
lea $30,1($25)
mov $5,$25
jal _printf
lea $4,10000
jal _delay
L.8:
brn L.7
mov $2,$0
L.5:
lw $30,16($sp)
lw $31,20($sp)
add $sp,$sp,24
jmp $31
.extern _disi
.extern _seti
.extern _putint
.extern _printf
.extern _puts
.extern _putchar
.rdata
.align 1
L.10:
.ubyte 104
.ubyte 101
.ubyte 108
.ubyte 108
.ubyte 111
.ubyte 32
.ubyte 119
.ubyte 111
.ubyte 114
.ubyte 108
.ubyte 100
.ubyte 33
.ubyte 32
.ubyte 37
.ubyte 100
.ubyte 10
.ubyte 0
.align 1
L.6:
.ubyte 105
.ubyte 110
.ubyte 105
.ubyte 116
.ubyte 32
.ubyte 48
.ubyte 0
.text
.align 4
.data
.align 4
.bss
.align 4
