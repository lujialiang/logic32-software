
.text

.extern _main
.extern _soc_init

.org 0
	lea $sp, 0x100000
	jal _soc_init
	jal _main
	halt

.global _halt
_halt:
	halt

.end
