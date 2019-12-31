.globl _putchar
.text
.text
.align 4
_putchar:
;.frame $sp,0,$31
sb $4,0x80000000
mov $2,$0
L.1:
jmp $31
.globl _puts
.text
.align 4
_puts:
;.frame $sp,24,$31
add $sp,$sp,-24
;.mask 0xc0000000,-4
sw $30,16($sp)
sw $31,20($sp)
mov $30,$4
brn L.4
L.3:
mov $25,$30
lea $30,1($25)
lb $4,($25)
jal _putchar
L.4:
lb $25,($30)
cmp $25,$0
bne L.3
lea $4,10
jal _putchar
mov $2,$0
L.2:
lw $30,16($sp)
lw $31,20($sp)
add $sp,$sp,24
jmp $31
.globl _printf
.text
.align 4
_printf:
;.frame $sp,32,$31
add $sp,$sp,-32
;.mask 0xc0000000,-12
sw $30,16($sp)
sw $31,20($sp)
sw $4,32($sp)
sw $5,36($sp)
sw $6,40($sp)
sw $7,44($sp)
lea $25,4+32($sp)
sw $25,-4+32($sp)
lea $4,_buf
lw $5,0+32($sp)
lw $6,-4+32($sp)
jal _vsprintf
sw $2,-8+32($sp)
lea $30,_buf
brn L.11
L.10:
mov $25,$30
lea $30,1($25)
lb $4,($25)
jal _putchar
L.11:
lb $25,($30)
cmp $25,$0
bne L.10
lw $2,-8+32($sp)
L.7:
lw $30,16($sp)
lw $31,20($sp)
add $sp,$sp,32
jmp $31
.text
.align 4
__putint0:
;.frame $sp,24,$31
add $sp,$sp,-24
;.mask 0x80000000,-8
sw $31,16($sp)
sw $4,24($sp)
lw $25,0+24($sp)
lea $24,10
cmp $25,$24
blt L.16
lw $25,0+24($sp)
lea $24,10
div $4,$25,$24
jal __putint0
L.16:
lw $25,0+24($sp)
lea $24,10
mod $25,$25,$24
lea $4,48($25)
jal _putchar
L.15:
lw $31,16($sp)
add $sp,$sp,24
jmp $31
.globl _putint
.text
.align 4
_putint:
;.frame $sp,24,$31
add $sp,$sp,-24
;.mask 0x80000000,-8
sw $31,16($sp)
sw $4,24($sp)
lw $25,0+24($sp)
cmp $25,$0
bge L.19
lea $4,45
jal _putchar
lea $25,-1
lw $24,0+24($sp)
mul $25,$25,$24
sw $25,0+24($sp)
L.19:
lw $4,0+24($sp)
jal __putint0
lea $4,10
jal _putchar
L.18:
lw $31,16($sp)
add $sp,$sp,24
jmp $31
.align 4
.lcomm _buf,1024
.extern _vsprintf
