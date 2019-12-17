
#include <errno.h>
#include <sys/stat.h>

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

int isatty (int file)
{
	//PANIC();
	return  1;
}       /* _isatty () */

int fstat(int file, struct stat *st)
{
	//PANIC();
	st->st_mode = S_IFCHR;
	return  0;
}       /* _fstat () */

int read (int file, char *ptr, int   len)
{
	PANIC();
	return  0; /* EOF */
}       /* _read () */

int lseek (int   file, int   offset, int   whence)
{
	PANIC();
	return  0;
}       /* _lseek () */

int write (int   file, char *buf, int   nbytes)
{
	int i;

	//PANIC();

	/* Output character at at time */
	for (i = 0; i < nbytes; i++)
	{
		//outbyte (buf[i]);
		uart_putchar(buf[i]);
	}

	return nbytes;
}       /* _write () */

int close (int   file)
{
	PANIC();
	errno = EBADF;
	return -1;                    /* Always fails */
}       /* _close () */

char heap[STACK_BUFFER];

void * sbrk (int nbytes)
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
