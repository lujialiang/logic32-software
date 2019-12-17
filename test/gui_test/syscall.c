
#include <stdio.h>
#include <errno.h>

#include <string.h>
#include "uart.h"

#include <errno.h>

#undef errno
extern int  errno;

#define STACK_BUFFER  10240     /* Reserved stack space in bytes. */

char buff[256];

void halt(void)
{
	asm("halt");
}

#define	PANIC()	do {				\
	sprintf(buff, "%s:%d, %s(), fatal error", __FILE__, __LINE__, __func__);	\
	uart_puts(buff);				\
	halt();							\
} while(0)	

int isatty(int fd)
{
	PANIC();
	errno = ENOSYS;
	return -1;
}

int fstat(void)
{
	PANIC();
	errno = ENOSYS;
	return -1;
}

int read(int __fd, void *__buf, size_t __nbyte )
{
	PANIC();
	errno = ENOSYS;
	return -1;
}

off_t lseek (int __fildes, off_t __offset, int __whence )
{
	PANIC();
	errno = ENOSYS;
	return -1;
}

int write(int __fd, const void *__buf, size_t __nbyte )
{
	PANIC();
	errno = ENOSYS;
	return -1;
}

int close(int __fildes )
{
	PANIC();
	errno = ENOSYS;
	return -1;
}

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
