#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(void)
{
	int fd;
	int save_errno;
	fd = open("LockFile.test", O_RDONLY | O_CREAT, 0444);
	if( flock(fd, LOCK_EX | LOCK_NB) < 0 ) {
		fprintf(stderr, "%d: %s\n", errno, strerror(errno));
	}
	while(1);
}
