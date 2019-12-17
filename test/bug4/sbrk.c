#include <errno.h>

#undef errno
extern int  errno;

#define STACK_BUFFER  10240     /* Reserved stack space in bytes. */

char heap[STACK_BUFFER];

void *
sbrk (int nbytes)
{
  /* Symbol defined by linker map */
  extern int  data_end;              /* start of free memory (as symbol) */

  /* Value set by crt0.S */
  //extern void *stack;           /* end of free memory */

  /* The statically held previous end of the heap, with its initialization. */
  //static void *heap_ptr = (void *)&data_end;         /* Previous end */
  static void *heap_ptr = (void *)heap;         /* Previous end */

  if (((heap_ptr + nbytes)) < (void *)(heap+STACK_BUFFER) )
    {
      void *base  = heap_ptr;
      heap_ptr   += nbytes;
                
      return  base;
    }
  else
    {
      errno = ENOMEM;
      return  (void *) -1;
    }
}       /* _sbrk () */
