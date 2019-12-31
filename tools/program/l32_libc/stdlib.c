#include <string.h>
#include <heap.h>
#include <soc.h>

void* malloc(size_t size)
{
	puts("call malloc()");
	return OS_HeapMalloc(HEAP_USER, size);
}

void* calloc(size_t num, size_t size)
{
	puts("call calloc()");
	{
	size_t total_size = num*size;
	void *p = OS_HeapMalloc(HEAP_USER, total_size);
	if( p == NULL ) {
		return NULL;
	}
	return memset(p,0,total_size);
	}
}

void free(void* ptr)
{
	puts("call free()");
	OS_HeapFree(ptr);
}

extern void halt(void);

void exit(int status)
{
	(void *)status;
	halt();
}

long labs(long n)
{
	return n>0?n:-n;
}

int abs(int n)
{
	return n>0?n:-n;
}

int rand(void)
{
	return *((int *)ADDR_RANDOM_NUMBER);
}
