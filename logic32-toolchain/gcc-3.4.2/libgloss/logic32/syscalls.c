#include <syscall.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

#include "fileio.h"

#define SYS_ftruncate 3000
#define SYS_isatty 3001

/* Set to 1 if we are running on hardware. The config instruction is 
 * always emulated on the simulator. The emulation code pokes this 
 * variable to 1.
 */
extern int _hardware;

/* _cpu_config==0 => Abel
 * _cpu_config==1 => Zeta
 * _cpu_config==2 => Phi
 */
//extern int _cpu_config;

int _use_syscall;


extern int _syscall(int *foo, int ID, ...);

extern int main(int argc, char **argv);

static char *args[]={"dummy.exe"};

void _initIO(void);

void __attribute__ ((weak)) _premain()  
{
	int t;
	/* we only use syscalls w/Zeta */
	_use_syscall=0;	//(_cpu_config==1);
	_initIO();
	
	t=main(1, args);
	exit(t);
	for (;;);
}


/* NOTE!!!! compiled with -fomit-frame-pointer to make sure that 'status' has the
 * correct value when breakpointing at _exit
 */
void __attribute__ ((weak)) _exit (int status)  
{
	/* end of the universe, cause memory fault */
	__asm("halt");
	for (;;);
}

int __attribute__ ((weak))
_DEFUN (write, (fd, buf, nbytes),
       int fd _AND
       char *buf _AND
       int nbytes)  
{
#if 0
	if (_use_syscall)
	{
		int t;
		int result;
		result=_syscall(&t, SYS_write, fd, buf, nbytes);
		errno=t;
		return result;
	} else
#endif
	{
	  int i;
		
	  for (i = 0; i < nbytes; i++) {
	    if (*(buf + i) == '\n') {
	      outbyte ('\r');
	    }
	    outbyte (*(buf + i));
	  }
	  return (nbytes);
	}
}


/*
 * read  -- read bytes from the serial port. Ignore fd, since
 *          we only have stdin.
 */
int __attribute__ ((weak))
_DEFUN (read, (fd, buf, nbytes),
       int fd _AND
       char *buf _AND
       int nbytes)  
{
#if 0
	if (_use_syscall)
	{
		int t;
		int result;
		result=_syscall(&t, SYS_read, fd, buf, nbytes);
		errno=t;
		return result;
	} else
#endif
	{
	  int i = 0;
	
	  for (i = 0; i < nbytes; i++) {
		char t=inbyte();

		if ((t!='\r')&&(t!='\n'))
		{
		    *(buf + i) = t;
		    outbyte(t); // echo
		} else
		{
	    	// terminate the line in the way expected by the libraries
		    *(buf + i) = '\n';
	    	i++;
		    outbyte('\r');
		    outbyte('\n');
	      break;
	    }
	  }
	  return (i);
	}
}



/*
 * open -- open a file descriptor. We don't have a filesystem, so
 *         we return an error.
 */
int __attribute__ ((weak)) open(const char *buf,
       int flags,
       int mode,
       ...)  
{
#if 0
	if (_use_syscall)
	{
		int t;
		int result;
		result=_syscall(&t, SYS_open, buf, strlen(buf)+1, flags, mode);
		errno=t;
		return result;
	} else
#endif
	{
	  errno = EIO;
	  return (-1);
	}
}



/*
 * close -- We don't need to do anything, but pretend we did.
 */
int __attribute__ ((weak))
_DEFUN (close ,(fd),
       int fd)  
{
#if 0
	if (_use_syscall)
	{
		int t;
		int result;
		result=_syscall(&t, SYS_close, fd);
		errno=t;
		return result;
	} else
#endif
	{
	  return (0);
	}
}




int __attribute__ ((weak))
ftruncate (int file, off_t length)  
{
	/*
	if (_use_syscall)
	{
		int t;
		int result;
		result=_syscall(&t, SYS_ftruncate, file, length);
		errno=t;
		return result;
	} else
	*/
	{
	  return -1;
	}
}


/*
 * unlink -- since we have no file system, 
 *           we just return an error.
 */
int __attribute__ ((weak))
_DEFUN (unlink, (path),
        char * path)  
{
#if 0
	if (_use_syscall)
	{
		int t;
		int result;
		result=_syscall(&t, SYS_unlink, path, strlen(path)+1);
		errno=t;
		return result;
	} else
#endif
	{
	  errno = EIO;
	  return (-1);
	}
}


/*
 * lseek --  Since a serial port is non-seekable, we return an error.
 */
off_t __attribute__ ((weak))
_DEFUN (lseek, (fd,  offset, whence),
       int fd _AND
       off_t offset _AND
       int whence)  
{
#if 0
	if (_use_syscall)
	{
		int t;
		int result;
		result=_syscall(&t, SYS_lseek, fd, offset, whence);
		errno=t;
		return result;
	} else
#endif
	{
	  errno = ESPIPE;
	  return ((off_t)-1);
	}
}

/* we convert from bigendian to smallendian*/
static long conv(char *a, int len) 
{
	long t=0;
	int i;
	for (i=0; i<len; i++)
	{
		t|=(((int)a[i])&0xff)<<((len-1-i)*8);
	}
	return t;
}

static void convert(struct fio_stat *gdb_stat, struct stat *buf)
{
	memset(buf, 0, sizeof(*buf));
	buf->st_dev=conv(gdb_stat->fst_dev, sizeof(gdb_stat->fst_dev));
	buf->st_ino=conv(gdb_stat->fst_ino, sizeof(gdb_stat->fst_ino));
	buf->st_mode=conv(gdb_stat->fst_mode, sizeof(gdb_stat->fst_mode));
	buf->st_nlink=conv(gdb_stat->fst_nlink, sizeof(gdb_stat->fst_nlink));
	buf->st_uid=conv(gdb_stat->fst_uid, sizeof(gdb_stat->fst_uid));
	buf->st_gid=conv(gdb_stat->fst_gid, sizeof(gdb_stat->fst_gid));
	buf->st_rdev=conv(gdb_stat->fst_rdev, sizeof(gdb_stat->fst_rdev));
	buf->st_size=conv(gdb_stat->fst_size, sizeof(gdb_stat->fst_size));
#if 0
	conv_64(fio_ulong_t,  &gdb_stat->fst_blksize);
	conv_64(fio_ulong_t,  &gdb_stat->fst_blocks);
#endif
#if 0
    conv_time(fst_atime;
	conv_time(fst_mtime;
	conv_time(fst_ctime;
#endif
}

int __attribute__ ((weak))
_DEFUN (fstat, (fd, buf),
       int fd _AND
       struct stat *buf)  
{
#if 0
	if (_use_syscall)
	{
		int t;
		int result;
		struct fio_stat gdb_stat;
		result=_syscall(&t, SYS_fstat, fd, &gdb_stat);
		convert(&gdb_stat, buf);
		errno=t;
		return result;
	} else
#endif
	{
/*
 * fstat -- Since we have no file system, we just return an error.
 */
	  buf->st_mode = S_IFCHR;	/* Always pretend to be a tty */
	  buf->st_blksize = 0;

	  return (0);
	}
}


int __attribute__ ((weak))
_DEFUN (stat, (path, buf),
       const char *path _AND
       struct stat *buf)  
{
#if 0
	if (_use_syscall)
	{
		int t;
		int result;
		struct fio_stat gdb_stat;
		result=_syscall(&t, SYS_stat, path, strlen(path)+1, &gdb_stat);
		convert(&gdb_stat, buf);
		errno=t;
		return result;
	} else
#endif
	{
	  errno = EIO;
	  return (-1);
	}
}



int __attribute__ ((weak))
_DEFUN (isatty, (fd),
       int fd)  
{
#if 0
	if (_use_syscall)
	{
		int t;
		int result;
		result=_syscall(&t, SYS_isatty, fd);
		errno=t;
		return result;
	} else
#endif
	{
		/*
		 * isatty -- returns 1 if connected to a terminal device,
		 *           returns 0 if not. Since we're hooked up to a
		 *           serial port, we'll say yes _AND return a 1.
		 */
	  return (1);
	}
}
