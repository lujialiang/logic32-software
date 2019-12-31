#include <stdio.h>

#include "filesys.h"

int filesys_init(void);

int main(void)
{
	FILE* file; 
	FL_DIR dir;
	int ch;
	if( filesys_init() != 0 ) {
		printf("file system init failure\n");
		return -1;
	}

	#if 0
	// List root directory
	fl_listdirectory("/");

	printf("\nopen message.txt...");
	fl_opendir("/", &dir);
	#endif

	file = fopen("/message.txt","r");
	if( file == NULL ) {
		printf("faluie\n");
		while(1);
	}

	printf("message.txt context:\n");
	while( (ch = fgetc(file)) != EOF )
	{
		putchar(ch);
	}
	putchar(10);

	printf("succeed open message.txt\n");
	fclose(file);
	fl_shutdown();
	return 0;
}
