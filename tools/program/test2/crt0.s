
.text

.global _main

.org 0
	lea $sp, 0x100000
	jal _main
	halt

.global _halt
_halt:
	halt

.end
