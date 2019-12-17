#ifndef _HEAP_H_INCLUDE_
#define _HEAP_H_INCLUDE_

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#ifdef USE_RTOS
#include "rtos.h"
#endif

#if 1
#define PRINTF_DEBUG(x,...)
#else
#define PRINTF_DEBUG	printf
#endif

/***************** Heap ******************/
//typedef struct OS_Heap_s OS_Heap_t;
#define HEAP_USER    (OS_Heap_t*)0
#define HEAP_SYSTEM  (OS_Heap_t*)1
#define HEAP_SMALL   (OS_Heap_t*)2
#define HEAP_UI      (OS_Heap_t*)3

#define HEAP_MAGIC 0x1234abcd
#define HEAP_COUNT 8

typedef struct HeapNode_s {
   struct HeapNode_s *next;
   int size;
} HeapNode_t;

struct OS_Heap_s {
   uint32_t magic;
   const char *name;
#ifdef USE_RTOS
   OS_Semaphore_t *semaphore;
#endif
   HeapNode_t *available;
   HeapNode_t base;
   int count;
   struct OS_Heap_s *alternate;
};
typedef struct OS_Heap_s OS_Heap_t;

OS_Heap_t *OS_HeapCreate(const char *name, void *memory, size_t size);
void OS_HeapDestroy(OS_Heap_t *heap);
void *OS_HeapMalloc(OS_Heap_t *heap, int bytes);
void OS_HeapFree(void *block);
void OS_HeapAlternate(OS_Heap_t *heap, OS_Heap_t *alternate);
void OS_HeapRegister(void *index, OS_Heap_t *heap);

#endif //_HEAP_H_
