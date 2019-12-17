.globl _disk_initialize
.text
.text
.align 4
_disk_initialize:
;.frame $sp,0,$31
mov $2,$0
L.2:
jmp $31
.globl _disk_readp
.text
.align 4
_disk_readp:
;.frame $sp,40,$31
add $sp,$sp,-40
;.mask 0xc0f00000,-4
sw $20,16($sp)
sw $21,20($sp)
sw $22,24($sp)
sw $23,28($sp)
sw $30,32($sp)
sw $31,36($sp)
mov $30,$4
sw $5,44($sp)
mov $23,$6
mov $22,$7
brn L.4
lea $4,L.6
lw $5,4+40($sp)
and $25,$23,0xffff
mov $6,$25
and $25,$22,0xffff
mov $7,$25
jal _printf
L.4:
lea $4,_disk_buf
lw $25,4+40($sp)
mov $5,$25
jal _disk_readp0
cmp $2,$0
bne L.7
mov $21,$0
brn L.12
L.9:
add $25,$21,$30
and $24,$23,0xffff
add $24,$24,$21
lb $24,_disk_buf($24)
sb $24,($25)
L.10:
lea $21,1($21)
L.12:
and $25,$22,0xffff
cmp $21,$25
blt L.9
brn L.13
mov $20,$0
brn L.18
L.15:
lea $4,L.19
add $25,$20,$30
lbu $25,($25)
mov $5,$25
jal _printf
lea $25,1($20)
lea $24,16
mod $25,$25,$24
cmp $25,$0
bne L.20
lea $4,10
jal _putchar
L.20:
L.16:
lea $20,1($20)
L.18:
and $25,$22,0xffff
cmp $20,$25
blt L.15
lea $4,10
jal _putchar
L.13:
mov $2,$0
brn L.3
L.7:
lea $2,1
L.3:
lw $20,16($sp)
lw $21,20($sp)
lw $22,24($sp)
lw $23,28($sp)
lw $30,32($sp)
lw $31,36($sp)
add $sp,$sp,40
jmp $31
.align 4
.lcomm L.24,4
.globl _disk_writep
.text
.text
.align 4
_disk_writep:
;.frame $sp,32,$31
add $sp,$sp,-32
;.mask 0xc0800000,-8
sw $23,16($sp)
sw $30,20($sp)
sw $31,24($sp)
sw $4,32($sp)
sw $5,36($sp)
lea $30,1
lw $25,0+32($sp)
cmp $25,$0
beq L.25
lea $4,L.27
lw $5,L.24
lw $6,4+32($sp)
jal _printf
lw $4,0+32($sp)
lw $25,L.24
mov $5,$25
jal _disk_writep0
sw $2,-4+32($sp)
lw $25,-4+32($sp)
cmp $25,$0
bne L.29
mov $23,$0
brn L.30
L.29:
lea $23,1
L.30:
mov $30,$23
brn L.26
L.25:
lw $25,4+32($sp)
cmp $25,$0
beq L.31
lw $25,4+32($sp)
sw $25,L.24
mov $30,$0
brn L.32
L.31:
mov $30,$0
L.32:
L.26:
mov $2,$30
L.23:
lw $23,16($sp)
lw $30,20($sp)
lw $31,24($sp)
add $sp,$sp,32
jmp $31
.align 4
.lcomm _disk_buf,1024
.extern _disk_writep0
.extern _disk_readp0
.extern _putint
.extern _printf
.extern _puts
.extern _putchar
.rdata
.align 1
L.27:
.byte 35
.byte 35
.byte 35
.byte 32
.byte 115
.byte 101
.byte 99
.byte 116
.byte 111
.byte 114
.byte 58
.byte 37
.byte 100
.byte 44
.byte 32
.byte 99
.byte 111
.byte 117
.byte 110
.byte 116
.byte 32
.byte 61
.byte 32
.byte 37
.byte 100
.byte 10
.byte 0
.align 1
L.19:
.byte 48
.byte 120
.byte 37
.byte 48
.byte 50
.byte 120
.byte 32
.byte 0
.align 1
L.6:
.byte 115
.byte 101
.byte 99
.byte 58
.byte 37
.byte 100
.byte 32
.byte 111
.byte 102
.byte 102
.byte 115
.byte 101
.byte 116
.byte 58
.byte 37
.byte 100
.byte 32
.byte 99
.byte 111
.byte 117
.byte 110
.byte 116
.byte 58
.byte 37
.byte 100
.byte 10
.byte 0
