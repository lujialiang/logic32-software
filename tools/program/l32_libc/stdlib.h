#ifndef _STDLIB_H_INCLUDE_
#define _STDLIB_H_INCLUDE_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

void* malloc(size_t size);
void* calloc(size_t num, size_t size);
void free(void* ptr);

void exit(int status);

long labs(long);
int abs(int);

int rand(void);

#ifdef __cplusplus
}
#endif

#endif
