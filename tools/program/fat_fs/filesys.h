#ifndef _FILE_H_INCLUDE_
#define _FILE_H_INCLUDE_

#include "fat_filelib.h"

typedef FL_FILE FILE;

#if 0
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
int fgetpos(void *f , uint32 * position);
long ftell(void *f);
int feof(void *f);
int remove(const char * filename);
int mkdir(const char *path);
#endif

#endif
