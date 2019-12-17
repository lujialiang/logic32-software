
#include <errno.h>
#include <sys/stat.h>

#undef errno
extern int  errno;


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
	PANIC();
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

	PANIC();

	/* Output character at at time */
	for (i = 0; i < nbytes; i++)
	{
	//outbyte (buf[i]);
	}

	return nbytes;
}       /* _write () */

int close (int   file)
{
	PANIC();
	errno = EBADF;
	return -1;                    /* Always fails */
}       /* _close () */

void * sbrk(ptrdiff_t __incr)
{
	PANIC();
	errno = ENOMEM;
	return  (void *) -1;
}

