
#include <stdio.h>
#include <string.h>
#include "foo.h"

#define SCREEN_WIDTH 320
#define SCREEN_HIGHT 240

short sa1, sb1;
unsigned short usa1, usb1;

int sum(int n, ...);
int printk(const char *fmt, ...);

struct node {
	int d;
	struct node *next;
};

struct node node = { 100, NULL };

const char *str1 = "str1";
const char *str2 = "str1";

static char buf2[32] = { 'L', '3', '2', 'S', 'O', 'C', 0 };

#if 0
int main(void)
{
	int m,n;
	buf2[3] = '-';
	buf2[4] = 's';
	buf2[5] = 'o';
	buf2[6] = 'c';
	buf2[7] = '\0';
	puts(buf2);
	puts("hello world!");
	m = 0x1234;
	n = 10;
	putint(m);
	putint(n);
	printf("%d\n",-n);
	printf("%d\n",-sum(5, 1, 2, 3, 4, 5));
	printf("%d\n",n-11);
	printf("hello %d\n", m);
	printf("hello 0x%x\n", m);
	printf("cmp = %d\n", strcmp(str1, str2));
	printf("d = %d, next = 0x%x\n", node.d, node.next);
	return 0;
}
#else

void fun1(void)
{
	unsigned int mclst = 0xb21;
	printf("%d %d\n", mclst, 0xff7);
	if( mclst < 0xFF7 ) {
		printf("< OK\n");
	}
	else {
		printf(">= ???\n");
	}
}

int main(void)
{
	int a, b, c, d;
	fun1();
	printf("hello world!\n");
	a = -1;
	b = -1;
	printf("test mul, div, mod\n");
	printf("%d * %d = %d\n", a, b, test_mul(a, b));
	printf("%d / %d = %d\n", a, b, test_div(a, b));
	printf("%d %% %d = %d\n", a, b, test_mod(a, b));

	printf("test mulu, divu, modu\n");
	printf("%d * %d = %d\n", a, b, test_mulu(a, b));
	printf("%d / %d = %d\n", a, b, test_divu(a, b));
	printf("%d %% %d = %d\n", a, b, test_modu(a, b));

	c = ~a + 1;
	d = -c;
	printf("c = %d\n", c);
	printf("d = %d\n", d);

#if 0
	while(1)
	{
		int i;
		int *p;
		p = (int *)(1<<25);
		puts("red");
		for(i=0;i<SCREEN_WIDTH*SCREEN_HIGHT;i++) {
			*p++ = 0xff0000;
		}
		p = (int *)(1<<25);
		puts("green");
		for(i=0;i<SCREEN_WIDTH*SCREEN_HIGHT;i++) {
			*p++ = 0x00ff00;
		}
		p = (int *)(1<<25);
		puts("blue");
		for(i=0;i<SCREEN_WIDTH*SCREEN_HIGHT;i++) {
			*p++ = 0x0000ff;
		}
	}
#endif

	return 0;
}
#endif
