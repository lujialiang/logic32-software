
< I > < load && store instructions >

1. lw $2, g_a
	ldih $1, 0x0000		--> reloc_high(g_a)
	lw $2, 0x0000($1)	--> reloc_low(g_a)

2. lw $2, 0x12345678
	ldih $1, 0x1234
	lw $2, 0x5678($1)

3. lw $2, 0x1234
	lw $2, 0x1234($0)

4. lw $2, g_a + 0x12345678
	ldih $1, 0x1234		--> reloc_high(g_a)
	lw $2, 0x5678($1)	--> reloc_low(g_a)

5. lw $2, g_a($3)
	ldih $1, 0x0000		--> reloc_high(g_a)
	add $1, $1, $3
	lw $2, 0x0000($1)	--> reloc_low(g_a)

6. lw $2, g_a + 0x12345678($3)
	ldih $1, 0x1234		--> reloc_high(g_a)
	add $1, $1, $3
	lw $2, 0x5678($1)	--> reloc_low(g_a)

7. l2 $2, 0x12345678($3)
	ldih $1, 0x1234
	add $1, $1, $3
	lw $2, 0x5678($1)


< II > < lea instructions >

1. lea 2, g_a
	ldih $1, 0x0000		--> reloc_high(g_a)
	ori $2, $1, 0x0000	--> reloc_low(g_a)

2. lea $2, 0x12345678
	ldih $1, 0x1234
	ori $2, $1, 0x5678

3. lea $2, 0x1234
	ori $2, $0, 0x1234

3.1 lea $2, 0x1234($3)
	add $2, $3, 0x1234

4. lea $2, g_a + 0x12345678
	ldih $1, 0x1234		--> reloc_high(g_a)
	ori $2, $1, 0x5678	--> reloc_low(g_a)

5. lea $2, g_a($3)
	ldih $1, 0x0000		--> reloc_high(g_a)
	ori $1, $1, 0x0000	--> reloc_low(g_a)
	add $2, $1, $3

6. lea $2, g_a + 0x12345678($3)
	ldih $1, 0x1234		--> reloc_high(g_a)
	ori $1, $1, 0x5678	--> reloc_low(g_a)
	add $2, $1, $3

7. lea $2, 0x12345678($3)
	ldih $1, 0x1234
	ori $1, $1, 0x5678
	add $2, $1, $3

