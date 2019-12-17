#ifndef _STDIO_H_INCLUDE_ 
#define _STDIO_H_INCLUDE_

#include <stddef.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

int putchar(int c);
int puts(char *s);

int kbhit(void);
int _getchar(void);

int printf(const char* format, ...);
int sprintf(char* buffer, const char* format, ...);
int snprintf(char* buffer, size_t count, const char* format, ...);

void putint(int n);
void puthex(unsigned n);

/* file system */
int filesys_init(void);
void* fopen(const char *path, const char *mode);
void fclose(void *f);
int fflush(void *f);
int fgetc(void *f);
char *fgets(char *s, int n, void *f);
int fputc(int c, void *f);
int fputs(const char * str, void *f);
int fwrite(const void * data, int size, int count, void *f);
int fread(void * buffer, int size, int length, void *f);
int fseek( void *f, long offset, int origin);
int fgetpos(void *f , unsigned * position);
long ftell(void *f);
int feof(void *f);
int remove(const char * filename);
int mkdir(const char *path);

#endif
