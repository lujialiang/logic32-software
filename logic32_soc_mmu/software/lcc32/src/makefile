SHELL=/bin/sh
SRC=.

OBJS=alloc.o bind.o dag.o decl.o enode.o error.o expr.o event.o \
	init.o input.o lex.o list.o main.o output.o prof.o profio.o simp.o \
	stmt.o string.o sym.o trace.o tree.o types.o \
	null.o symbolic.o gen.o mips.o l32.o #sparc.o x86.o

rcc: $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $(OBJS)
	cp $@ ../../bin

$(OBJS):	$(SRC)/c.h $(SRC)/token.h $(SRC)/config.h

alloc.o:	$(SRC)/alloc.c;		$(CC) -c $(CFLAGS) $(SRC)/alloc.c
bind.o:		$(SRC)/bind.c;		$(CC) -c $(CFLAGS) $(SRC)/bind.c
dag.o:		$(SRC)/dag.c;		$(CC) -c $(CFLAGS) $(SRC)/dag.c
decl.o:		$(SRC)/decl.c;		$(CC) -c $(CFLAGS) $(SRC)/decl.c
enode.o:	$(SRC)/enode.c;		$(CC) -c $(CFLAGS) $(SRC)/enode.c
error.o:	$(SRC)/error.c;		$(CC) -c $(CFLAGS) $(SRC)/error.c
event.o:	$(SRC)/event.c;		$(CC) -c $(CFLAGS) $(SRC)/event.c
expr.o:		$(SRC)/expr.c;		$(CC) -c $(CFLAGS) $(SRC)/expr.c
gen.o:		$(SRC)/gen.c;		$(CC) -c $(CFLAGS) $(SRC)/gen.c
init.o:		$(SRC)/init.c;		$(CC) -c $(CFLAGS) $(SRC)/init.c
input.o:	$(SRC)/input.c;		$(CC) -c $(CFLAGS) $(SRC)/input.c
lex.o:		$(SRC)/lex.c;		$(CC) -c $(CFLAGS) $(SRC)/lex.c
list.o:		$(SRC)/list.c;		$(CC) -c $(CFLAGS) $(SRC)/list.c
main.o:		$(SRC)/main.c;		$(CC) -c $(CFLAGS) $(SRC)/main.c
mips.o:		$(SRC)/mips.c;		$(CC) -c $(CFLAGS) $(SRC)/mips.c
null.o:		$(SRC)/null.c;		$(CC) -c $(CFLAGS) $(SRC)/null.c
output.o:	$(SRC)/output.c;	$(CC) -c $(CFLAGS) $(SRC)/output.c
prof.o:		$(SRC)/prof.c;		$(CC) -c $(CFLAGS) $(SRC)/prof.c
profio.o:	$(SRC)/profio.c;	$(CC) -c $(CFLAGS) $(SRC)/profio.c
simp.o:		$(SRC)/simp.c;		$(CC) -c $(CFLAGS) $(SRC)/simp.c
sparc.o:	$(SRC)/sparc.c;		$(CC) -c $(CFLAGS) $(SRC)/sparc.c
stmt.o:		$(SRC)/stmt.c;		$(CC) -c $(CFLAGS) $(SRC)/stmt.c
string.o:	$(SRC)/string.c;	$(CC) -c $(CFLAGS) $(SRC)/string.c
sym.o:		$(SRC)/sym.c;		$(CC) -c $(CFLAGS) $(SRC)/sym.c
symbolic.o:	$(SRC)/symbolic.c;	$(CC) -c $(CFLAGS) $(SRC)/symbolic.c
trace.o:	$(SRC)/trace.c;		$(CC) -c $(CFLAGS) $(SRC)/trace.c
tree.o:		$(SRC)/tree.c;		$(CC) -c $(CFLAGS) $(SRC)/tree.c
types.o:	$(SRC)/types.c;		$(CC) -c $(CFLAGS) $(SRC)/types.c
x86.o:		$(SRC)/x86.c;		$(CC) -c $(CFLAGS) $(SRC)/x86.c

l32.o:		$(SRC)/l32.c;		$(CC) -c $(CFLAGS) $(SRC)/l32.c

$(SRC)/mips.c:		$(SRC)/mips.md;	../lburg/lburg $(SRC)/mips.md  >$(SRC)/mips.c
$(SRC)/sparc.c:	$(SRC)/sparc.md;../lburg/lburg $(SRC)/sparc.md >$(SRC)/sparc.c
$(SRC)/x86.c:		$(SRC)/x86.md;	../lburg/lburg $(SRC)/x86.md   >$(SRC)/x86.c

$(SRC)/l32.c:		$(SRC)/l32.md;	../lburg/lburg $(SRC)/l32.md   >$(SRC)/l32.c

test:	tst/8q.s tst/array.s tst/cf.s tst/cq.s tst/cvt.s tst/fields.s \
	tst/front.s tst/incr.s tst/init.s tst/limits.s tst/paranoia.s \
	tst/sort.s tst/spill.s tst/stdarg.s tst/struct.s tst/switch.s \
	tst/wf1.s tst/yacc.s

T=$(SRC)/../tst
RUN=$(SRC)/run
RCC=rcc
LCC=lcc
CMD=@$(RUN) $(TARGET)

tst/8q.s:	$(RUN) $(RCC) $(T)/8q.c		$(T)/8q.0;	$(CMD) 8q
tst/array.s:	$(RUN) $(RCC) $(T)/array.c	$(T)/array.0;	$(CMD) array
tst/cf.s:	$(RUN) $(RCC) $(T)/cf.c		$(T)/cf.0;	$(CMD) cf
tst/cq.s:	$(RUN) $(RCC) $(T)/cq.c		$(T)/cq.0;	$(CMD) cq
tst/cvt.s:	$(RUN) $(RCC) $(T)/cvt.c	$(T)/cvt.0;	$(CMD) cvt
tst/fields.s:	$(RUN) $(RCC) $(T)/fields.c	$(T)/fields.0;	$(CMD) fields
tst/front.s:	$(RUN) $(RCC) $(T)/front.c	$(T)/front.0;	$(CMD) front
tst/incr.s:	$(RUN) $(RCC) $(T)/incr.c	$(T)/incr.0;	$(CMD) incr
tst/init.s:	$(RUN) $(RCC) $(T)/init.c	$(T)/init.0;	$(CMD) init
tst/limits.s:	$(RUN) $(RCC) $(T)/limits.c	$(T)/limits.0;	$(CMD) limits
tst/paranoia.s:	$(RUN) $(RCC) $(T)/paranoia.c	$(T)/paranoia.0;$(CMD) paranoia
tst/sort.s:	$(RUN) $(RCC) $(T)/sort.c	$(T)/sort.0;	$(CMD) sort
tst/spill.s:	$(RUN) $(RCC) $(T)/spill.c	$(T)/spill.0;	$(CMD) spill
tst/stdarg.s:	$(RUN) $(RCC) $(T)/stdarg.c	$(T)/stdarg.0;	$(CMD) stdarg
tst/struct.s:	$(RUN) $(RCC) $(T)/struct.c	$(T)/struct.0;	$(CMD) struct
tst/switch.s:	$(RUN) $(RCC) $(T)/switch.c	$(T)/switch.0;	$(CMD) switch
tst/wf1.s:	$(RUN) $(RCC) $(T)/wf1.c      	$(T)/wf1.0;	$(CMD) wf1
tst/yacc.s:	$(RUN) $(RCC) $(T)/yacc.c     	$(T)/yacc.0;	$(CMD) yacc

OPTS=CC='$(LCC) -B./ -d0.1 -A' \
	CFLAGS='-Wf-target=$(TARGET) -I$(SRC)/../include/`echo $(TARGET)|tr - /` \
	-I$(SRC)' LDFLAGS='$(LDFLAGS)'

triple:	rcc
	rm -f *.o
	make -f $(SRC)/makefile $(OPTS)
	-strip rcc
	-od rcc +8 >od2
	rm -f *.o
	make -f $(SRC)/makefile $(OPTS)
	-strip rcc
	-od rcc +8 >od3
	cmp od[23] && rm od[23]

run: rcc
	l32-asm crt0.s
	./rcc -target=l32 ./test0.c > test.s && l32-asm test.s && l32-link crt0.o test.o #&& l32-dis a.out

install: rcc
	cp rcc /usr/local/bin/l32-rcc

sim: rcc
	l32-sim -p a.out

.PHONY: tags
tags:
	ctags *.c *.h

testclean:
	-rm -f tst/*.[12so]
	-cd tst; rm -f 8q array cf cq cvt fields front incr init \
		limits paranoia sort spill stdarg struct switch wf1 yacc
	
clean:		testclean
		-rm -f *.o

clobber:	clean
		rm -f rcc lburg
		rm -f $(SRC)/mips.c $(SRC)/sparc.c $(SRC)/x86.c
