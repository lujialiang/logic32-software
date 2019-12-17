#include <string.h>
#include "malloc.h"
#include "heap.h"

void* _malloc(size_t size)
{
	return OS_HeapMalloc(HEAP_USER, size);
}

void* _calloc(size_t num, size_t size)
{
	size_t total_size = num*size;
	void *p = OS_HeapMalloc(HEAP_USER, total_size);
	if( p == NULL ) {
		return NULL;
	}
	return memset(p,0,total_size);
}

void _free(void* ptr)
{
	OS_HeapFree(ptr);
}
