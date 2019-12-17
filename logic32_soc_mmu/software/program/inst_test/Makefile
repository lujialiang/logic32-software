
AS = l32-asm
LD = l32-link
DIS  = l32-dis
SIZE = l32-size
DUMP = l32-dump
SIM  = l32-sim

.PHONY: all
all: a.out

a.out: crt0.o
	#$(LD) -M a.map $^
	$(LD) $^

.PHONY: dis
dis: a.out
	$(DIS) $<

.PHONY: sim
sim: a.out
	$(SIM) -p a.out -c 20000000

.PHONY: size
size: a.out
	$(SIZE) $<

.PHONY: dump
dump: crt0.o
	$(DUMP) $<

%.o: %.s
	$(AS) $<

.PHONY: tags
tags:
	ctags -R

.PHONY: clean
clean:
	$(RM) *.o a.out *.exe stdout.txt stderr.txt a.map
