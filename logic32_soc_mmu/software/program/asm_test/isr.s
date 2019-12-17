.globl _systick_isr
.text
.text
.align 4
_systick_isr:
;.frame $sp,24,$31
add $sp,$sp,-24
;.mask 0x80000000,-8
sw $31,16($sp)
lea $4,L.2
jal _puts
L.1:
lw $31,16($sp)
add $sp,$sp,24
jmp $31
.extern _putint
.extern _printf
.extern _puts
.extern _putchar
.rdata
.align 1
L.2:
.ubyte 9
.ubyte 9
.ubyte 115
.ubyte 121
.ubyte 115
.ubyte 116
.ubyte 105
.ubyte 99
.ubyte 107
.ubyte 32
.ubyte 105
.ubyte 115
.ubyte 114
.ubyte 33
.ubyte 0
.text
.align 4
.data
.align 4
.bss
.align 4
