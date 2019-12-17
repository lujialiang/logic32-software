#include <errno.h>
#include <sys/stat.h>

#include "uart.h"

#define STDIN_FILENO    0       /* standard input file descriptor */
#define STDOUT_FILENO   1       /* standard output file descriptor */
#define STDERR_FILENO   2       /* standard error file descriptor */

#undef errno
extern int  errno;

#define STACK_BUFFER  10240     /* Reserved stack space in bytes. */

int isatty (int file)
{
	if ((file == STDOUT_FILENO) || (file == STDERR_FILENO)) {
		return  1;
	}
	else {
		errno = EBADF;
		return  -1;
	}
}       /* _isatty () */

int fstat(int file, struct stat *st)
{
	if ((STDOUT_FILENO == file) || (STDERR_FILENO == file)) {
		st->st_mode = S_IFCHR;
		return  0;
	}
	else {
		errno = EBADF;
		return  -1;
	}
}       /* _fstat () */

int read(int file, char *buf, int len)
{
	if (STDIN_FILENO == file) {
		int  i;
		for (i = 0; i < len; i++) {
			buf[i] = uart_getc();
			#if	1	//def UART_AUTO_ECHO
			uart_putc(buf[i]);
			#endif
			/* Return partial buffer if we get EOL */
			if ('\n' == buf[i]) {
				return (i + 1);
			}
		}
		return  i;			/* Filled the buffer */
    }
	else {
		errno = EBADF;
		return  -1;
	}
}	/* _read () */

int lseek (int file, int offset, int whence)
{
	if ((STDOUT_FILENO == file) || (STDERR_FILENO == file)) {
		return  0;
	}
	else {
		errno = EBADF;
		return  (long) -1;
	}
}       /* _lseek () */

int write(int file, char *buf, int nbytes)
{
	int i;
	/* We only handle stdout and stderr */
	if ((file != STDOUT_FILENO) && (file != STDERR_FILENO)) {
		errno = EBADF;
		return -1;
	}
	/* Output character at at time */
	for (i = 0; i < nbytes; i++)
	{
		uart_putc(buf[i]);
	}
	return nbytes;
}       /* _write () */

int close(int file)
{
	errno = EBADF;
	return -1;                    /* Always fails */
}       /* _close () */

static char heap[STACK_BUFFER];

void * sbrk(int nbytes)
{
	/* Symbol defined by linker map */
	extern int  data_end;              /* start of free memory (as symbol) */

	/* Value set by crt0.S */
	//extern void *stack;           /* end of free memory */

	/* The statically held previous end of the heap, with its initialization. */
	//static void *heap_ptr = (void *)&data_end;         /* Previous end */
	static void *heap_ptr = (void *)heap;         /* Previous end */

	if (((heap_ptr + nbytes)) < (void *)(heap+STACK_BUFFER) ) {
		void *base  = heap_ptr;
		heap_ptr   += nbytes;
		return  base;
	}
	else {
		errno = ENOMEM;
		return  (void *) -1;
	}
}       /* _sbrk () */

void _exit(int status)  
{
    /* end of the universe, cause memory fault */
    __asm("halt");
    for (;;);
}

#if 0
clock_t times(struct tms *buf)
{
  errno = EACCES;
  return  -1;
}       /* _times () */

int gettimeofday (struct timeval *tp, void *tzp)
{
	return 0;
}
#endif
