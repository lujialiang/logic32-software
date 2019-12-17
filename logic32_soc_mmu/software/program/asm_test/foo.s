.globl _foo
.text
.text
.align 4
_foo:
;.frame $sp,16,$31
add $sp,$sp,-16
;.mask 0x40000000,-16
sw $30,0($sp)
lea $25,10
sw $25,-4+16($sp)
lea $25,20
sw $25,-8+16($sp)
lea $30,30
lw $25,-4+16($sp)
lw $24,-8+16($sp)
add $25,$25,$24
lw $24,-12+16($sp)
add $30,$25,$24
mov $2,$30
L.1:
lw $30,0($sp)
add $sp,$sp,16
jmp $31
.text
.align 4
.data
.align 4
.bss
.align 4
