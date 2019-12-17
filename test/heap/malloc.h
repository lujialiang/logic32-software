#ifndef _MALLOC_H_INCLUDE_
#define _MALLOC_H_INCLUDE_

#include <stddef.h>

void* _malloc(size_t size);
void* _calloc(size_t num, size_t size);
void _free(void* ptr);

#endif
