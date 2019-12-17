#ifndef _STDIO_H_INCLUDE_ 
#define _STDIO_H_INCLUDE_

#ifndef NULL
#define NULL ((void *)0)
#endif

int putchar(int c);
int puts(char *s);
int printf(const char *fmt, ...);

void putint(int n);

#endif
