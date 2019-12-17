HOST=l32
LDFLAGS=-s
CFLAGS=

lcc: $(HOST).o lcc.o
	$(CC) -o lcc $(LDFLAGS) $(HOST).o lcc.o; rm -f $(HOST).o lcc.o
	cp $@ ../../bin/l32-cc

bprint:	bprint.c ../src/profio.c
	$(CC) -o bprint $(LDFLAGS) bprint.c

install: lcc
	cp lcc /usr/local/bin

clean:
	$(RM) *.o a.out core bprint lcc

clobber:
	$(RM) *.o a.out core bprint lcc
