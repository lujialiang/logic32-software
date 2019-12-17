#include <heap.h>
#include <soc.h>

void heap_init(void)
{
	OS_Heap_t *heap;
	heap = OS_HeapCreate("HEAP_USER", (void *)ADDR_HEAP_BASE, HEAP_LIMIT);
	OS_HeapRegister(HEAP_USER, heap);
}

void soc_init(void)
{
	heap_init();
}
