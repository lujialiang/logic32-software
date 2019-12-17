#include <stdio.h>
#include <string.h>

float fabs(float x)
{
	return x>0?x:-x;
}

int main(int argc, char *argv[])
{
	char *s = "GoldenGlobalView";
	char *l = "lob";
	char *p;
	p = strstr(s, l);
	if( p ) {
		printf("'%s'\n", p);
	}
	else {
		puts("NotFound!");
	}
	return 0;
}
