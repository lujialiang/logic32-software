	.file	"crt0.S"
	.text
	.global	_start
	.func	_start
_start:
	ldi $sp, 0x10000
	jal _premain
	halt

