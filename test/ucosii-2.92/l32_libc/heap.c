
#include "heap.h"

static OS_Heap_t *HeapArray[HEAP_COUNT];

/***************** Heap *******************/
/******************************************/
OS_Heap_t *OS_HeapCreate(const char *name, void *memory, size_t size)
{
   OS_Heap_t *heap;
   assert(((size_t)memory & 3) == 0);
   heap = (OS_Heap_t*)memory;
   heap->magic = HEAP_MAGIC;
   heap->name = name;
#ifdef USE_RTOS
   heap->semaphore = OS_SemaphoreCreate(name, 1);
#endif
   heap->available = (HeapNode_t*)(heap + 1);
   heap->available->next = &heap->base;
   heap->available->size = (size - sizeof(OS_Heap_t)) / sizeof(HeapNode_t);
   heap->base.next = heap->available;
   heap->base.size = 0;
   heap->count = 0;
   heap->alternate = NULL;
   return heap;
}


/******************************************/
void OS_HeapDestroy(OS_Heap_t *heap)
{
#ifdef USE_RTOS
   OS_SemaphoreDelete(heap->semaphore);
#endif
}


/******************************************/
//Modified from Kernighan & Ritchie "The C Programming Language"
void *OS_HeapMalloc(OS_Heap_t *heap, int bytes)
{
   HeapNode_t *node, *prevp;
   int nunits;

#ifdef USE_RTOS
   if(heap == NULL && OS_ThreadSelf())
      heap = OS_ThreadSelf()->heap;
#endif

   if((size_t)heap < HEAP_COUNT)
      heap = HeapArray[(size_t)heap];

   nunits = (bytes + sizeof(HeapNode_t) - 1) / sizeof(HeapNode_t) + 1;
#ifdef USE_RTOS
   OS_SemaphorePend(heap->semaphore, OS_WAIT_FOREVER);
#endif
   prevp = heap->available;
   for(node = prevp->next; ; prevp = node, node = node->next)
   {
      if(node->size >= nunits)       //Big enough?
      {
         if(node->size == nunits)    //Exactly
            prevp->next = node->next;
         else
         {                           //Allocate tail end
            node->size -= nunits;
            node += node->size;
            node->size = nunits;
         }
         heap->available = prevp;
         node->next = (HeapNode_t*)heap;
#ifdef DEBUG
         PRINTF_DEBUG("malloc(%ld, %d)\n", node->size * sizeof(HeapNode_t), heap->count); 
#endif
         ++heap->count;
#ifdef USE_RTOS
         OS_SemaphorePost(heap->semaphore);
#endif
         return (void*)(node + 1);
      }
      if(node == heap->available)   //Wrapped around free list
      {
#ifdef USE_RTOS
         OS_SemaphorePost(heap->semaphore);
#endif
         if(heap->alternate)
            return OS_HeapMalloc(heap->alternate, bytes);
#ifdef DEBUG
         PRINTF_DEBUG("M%d ", heap->count);
#endif
         return NULL;
      }
   }
}


/******************************************/
//Modified from K&R
void OS_HeapFree(void *block)
{
   OS_Heap_t *heap;
   HeapNode_t *bp, *node;

   if(block == NULL)
      return;
   bp = (HeapNode_t*)block - 1;   //point to block header
   heap = (OS_Heap_t*)bp->next;
   assert(heap->magic == HEAP_MAGIC);
   if(heap->magic != HEAP_MAGIC)
      return;
#ifdef USE_RTOS
   OS_SemaphorePend(heap->semaphore, OS_WAIT_FOREVER);
#endif
   --heap->count;
#ifdef DEBUG
   PRINTF_DEBUG("free(%ld, %d)\n", bp->size * sizeof(HeapNode_t), heap->count); 
#endif
   for(node = heap->available; !(node < bp && bp < node->next); node = node->next)
   {
      if(node >= node->next && (bp > node || bp < node->next))
         break;               //freed block at start or end of area
   }

   if(bp + bp->size == node->next)   //join to upper
   {
      bp->size += node->next->size;
      bp->next = node->next->next;
   }
   else
   {
      bp->next = node->next;
   }

   if(node + node->size == bp)       //join to lower
   {
      node->size += bp->size;
      node->next = bp->next;
   }
   else
      node->next = bp;
   heap->available = node;
#ifdef USE_RTOS
   OS_SemaphorePost(heap->semaphore);
#endif
}

/******************************************/
void OS_HeapAlternate(OS_Heap_t *heap, OS_Heap_t *alternate)
{
   heap->alternate = alternate;
}

/******************************************/
void OS_HeapRegister(void *index, OS_Heap_t *heap)
{
   if((size_t)index < HEAP_COUNT)
      HeapArray[(size_t)index] = heap;
}
