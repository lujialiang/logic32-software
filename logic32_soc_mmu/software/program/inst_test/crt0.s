
.org 0

div.d $f18,$f18,$f16
halt

.end

L0:
add $1, $2, $3
sub $1, $2, $3
lw $0, ($0)
lb $0, ($0)
lh $0, ($0)
jmp $lr
brn L0

halt
