.globl _foo
.text
.text
.align 4
_foo:
;.frame $sp,8,$31
add $sp,$sp,-8
lea $25,0x88000004
sw $25,-4+8($sp)
lw $25,-4+8($sp)
mov $2,$25
L.1:
add $sp,$sp,8
jmp $31
