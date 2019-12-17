#include <sys/stat.h>

int read (file, ptr, len)
     int    file;
     char * ptr;
     int    len;
{
  return 0;
}

int lseek (file, ptr, dir)
     int file;
     int ptr;
     int dir;
{
  return 0;
}

int write (file, ptr, len)
     int    file;
     char * ptr;
     int    len;
{
  return 0;
}

int open (path, flags)
     const char * path;
     int flags;
{
  return 0;
}

int close (file)
     int file;
{
  return 0;
}

unsigned sbrk (incr)
     int incr;
{
  return 0;
}

int fstat (file, st)
     int file;
     struct stat * st;
{
  st->st_mode = S_IFCHR;
  return 0;
}

int isatty (fd)
     int fd;
{
  return 0;
}

