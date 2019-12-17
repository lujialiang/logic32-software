.globl _node
.data
.align 4
_node:
.word 0x64
.word 0x0
.globl _str1
.data
.align 4
_str1:
.word L.1
.globl _str2
.data
.align 4
_str2:
.word L.1
.data
.align 1
_buf2:
.byte 76
.byte 51
.byte 50
.byte 83
.byte 79
.byte 67
.byte 0
.space 25
.globl _fun1
.text
.text
.align 4
_fun1:
;.frame $sp,24,$31
add $sp,$sp,-24
;.mask 0xc0000000,-4
sw $30,16($sp)
sw $31,20($sp)
lea $30,2849
lea $4,L.3
mov $5,$30
lea $6,4087
jal _printf
lea $25,4087
cmp $30,$25
bgeu L.4
lea $4,L.6
jal _printf
brn L.5
L.4:
lea $4,L.7
jal _printf
L.5:
L.2:
lw $30,16($sp)
lw $31,20($sp)
add $sp,$sp,24
jmp $31
.globl _main
.text
.align 4
_main:
;.frame $sp,48,$31
add $sp,$sp,-48
;.mask 0xc0f00000,-12
sw $20,16($sp)
sw $21,20($sp)
sw $22,24($sp)
sw $23,28($sp)
sw $30,32($sp)
sw $31,36($sp)
jal _fun1
lea $4,L.9
jal _printf
lea $30,-1
mov $23,$30
lea $4,L.10
jal _printf
mov $4,$30
mov $5,$23
jal _test_mul
mov $25,$2
lea $4,L.11
mov $5,$30
mov $6,$23
mov $7,$25
jal _printf
mov $4,$30
mov $5,$23
jal _test_div
mov $25,$2
lea $4,L.12
mov $5,$30
mov $6,$23
mov $7,$25
jal _printf
mov $4,$30
mov $5,$23
jal _test_mod
mov $25,$2
lea $4,L.13
mov $5,$30
mov $6,$23
mov $7,$25
jal _printf
lea $4,L.14
jal _printf
mov $4,$30
mov $5,$23
jal _test_mulu
mov $25,$2
lea $4,L.11
mov $5,$30
mov $6,$23
mov $7,$25
jal _printf
mov $4,$30
mov $5,$23
jal _test_divu
mov $25,$2
lea $4,L.12
mov $5,$30
mov $6,$23
mov $7,$25
jal _printf
mov $4,$30
mov $5,$23
jal _test_modu
mov $25,$2
lea $4,L.13
mov $5,$30
mov $6,$23
mov $7,$25
jal _printf
not $25,$30
lea $22,1($25)
neg $25,$22
sw $25,-4+48($sp)
lea $4,L.15
mov $5,$22
jal _printf
lea $4,L.16
lw $5,-4+48($sp)
jal _printf
brn L.18
L.17:
lea $20,0x2000000
mov $21,$0
L.20:
mov $25,$20
lea $20,4($25)
lea $24,16711680
sw $24,($25)
L.21:
lea $21,1($21)
lea $25,130560
cmp $21,$25
blt L.20
lea $20,0x2000000
mov $21,$0
L.24:
mov $25,$20
lea $20,4($25)
lea $24,65280
sw $24,($25)
L.25:
lea $21,1($21)
lea $25,130560
cmp $21,$25
blt L.24
lea $20,0x2000000
mov $21,$0
L.28:
mov $25,$20
lea $20,4($25)
lea $24,255
sw $24,($25)
L.29:
lea $21,1($21)
lea $25,130560
cmp $21,$25
blt L.28
L.18:
mov $2,$0
L.8:
lw $20,16($sp)
lw $21,20($sp)
lw $22,24($sp)
lw $23,28($sp)
lw $30,32($sp)
lw $31,36($sp)
add $sp,$sp,48
jmp $31
.extern _printk
.extern _sum
.globl _usb1
.align 2
.comm _usb1,2
.globl _usa1
.align 2
.comm _usa1,2
.globl _sb1
.align 2
.comm _sb1,2
.globl _sa1
.align 2
.comm _sa1,2
.extern _test_modu
.extern _test_mod
.extern _test_divu
.extern _test_div
.extern _test_mulu
.extern _test_mul
.extern _memset
.extern _strncmp
.extern _strcmp
.extern _strlen
.extern _putint
.extern _printf
.extern _puts
.extern _putchar
.rdata
.align 1
L.16:
.byte 100
.byte 32
.byte 61
.byte 32
.byte 37
.byte 100
.byte 10
.byte 0
.align 1
L.15:
.byte 99
.byte 32
.byte 61
.byte 32
.byte 37
.byte 100
.byte 10
.byte 0
.align 1
L.14:
.byte 116
.byte 101
.byte 115
.byte 116
.byte 32
.byte 109
.byte 117
.byte 108
.byte 117
.byte 44
.byte 32
.byte 100
.byte 105
.byte 118
.byte 117
.byte 44
.byte 32
.byte 109
.byte 111
.byte 100
.byte 117
.byte 10
.byte 0
.align 1
L.13:
.byte 37
.byte 100
.byte 32
.byte 37
.byte 37
.byte 32
.byte 37
.byte 100
.byte 32
.byte 61
.byte 32
.byte 37
.byte 100
.byte 10
.byte 0
.align 1
L.12:
.byte 37
.byte 100
.byte 32
.byte 47
.byte 32
.byte 37
.byte 100
.byte 32
.byte 61
.byte 32
.byte 37
.byte 100
.byte 10
.byte 0
.align 1
L.11:
.byte 37
.byte 100
.byte 32
.byte 42
.byte 32
.byte 37
.byte 100
.byte 32
.byte 61
.byte 32
.byte 37
.byte 100
.byte 10
.byte 0
.align 1
L.10:
.byte 116
.byte 101
.byte 115
.byte 116
.byte 32
.byte 109
.byte 117
.byte 108
.byte 44
.byte 32
.byte 100
.byte 105
.byte 118
.byte 44
.byte 32
.byte 109
.byte 111
.byte 100
.byte 10
.byte 0
.align 1
L.9:
.byte 104
.byte 101
.byte 108
.byte 108
.byte 111
.byte 32
.byte 119
.byte 111
.byte 114
.byte 108
.byte 100
.byte 33
.byte 10
.byte 0
.align 1
L.7:
.byte 62
.byte 61
.byte 32
.byte 63
.byte 63
.byte 63
.byte 10
.byte 0
.align 1
L.6:
.byte 60
.byte 32
.byte 79
.byte 75
.byte 10
.byte 0
.align 1
L.3:
.byte 37
.byte 100
.byte 32
.byte 37
.byte 100
.byte 10
.byte 0
.align 1
L.1:
.byte 115
.byte 116
.byte 114
.byte 49
.byte 0
