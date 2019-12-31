#include <stdio.h>

//#include "diskio.h"
#include "fat_filelib.h"
#include "disk.h"

int diskio_read(uint32 sector, uint8 *buffer, uint32 sector_count);
int diskio_write(uint32 sector, uint8 *buffer, uint32 sector_count);

int main(void)
{
	FL_FILE* file; 
	FL_DIR dir;
	int ch;
	printf("initialize SD card... ");
	if( disk_initialize() != DISK_OK ) {
		printf("faluie\n");
		return -1;
	}
	printf("done\n");
	printf("initialize FAT filesystme... ");
	fl_init();
	printf("done\n");
	if( fl_attach_media(diskio_read,diskio_write) != FAT_INIT_OK ) {
		printf("faluie\n");
		return -2;
	}
	printf("done\n");
	// List root directory
	fl_listdirectory("/");

	printf("\nopen message.txt...");
	fl_opendir("/", &dir);
	file = fl_fopen("/message.txt","r");
	if( file == NULL ) {
		printf("faluie\n");
		while(1);
	}

	printf("message.txt context:\n");
	while( (ch = fl_fgetc(file)) != EOF )
	{
		putchar(ch);
	}
	putchar(10);

	printf("succeed open message.txt\n");
	fl_fclose(file);
	fl_shutdown();
	return 0;
}

int diskio_read(uint32 sector, uint8 *buffer, uint32 sector_count)
{
	int res;
	while( sector_count )
	{
		res = disk_readp0( buffer, sector);
		if( res != DISK_OK ) {
			return 0;
		}
		sector++;
		buffer += 512;
		sector_count--;
	}
	return 1;
}

int diskio_write(uint32 sector, uint8 *buffer, uint32 sector_count)
{
	int res;
	while( sector_count )
	{
		res = disk_writep0(buffer, sector);
		if( res != DISK_OK ) {
			return 0;
		}
		sector++;
		buffer += 512;
		sector_count--;
	}
	return 1;
}
