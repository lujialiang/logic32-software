CC = gcc

all: aout2bin aout2data bin2mif bin2v

aout2bin: aout2bin.c
	$(CC) -o $@ $^

aout2data: aout2data.c
	$(CC) -o $@ $^

bin2mif: bin2mif.c
	$(CC) -o $@ $^

bin2v: bin2v.c
	$(CC) -o $@ $^

install: aout2bin aout2data bin2mif bin2v
	cp $^ /usr/local/bin/

clean:
	$(RM) -rf aout2bin aout2data bin2mif bin2v
