
.text

.global _main

.org 0
	;lea $sp, 0x100000
	lea $sp, 0x1ff0
	jal _main
	halt

.org 0x90
	halt

.global _halt
_halt:
	halt

.end
