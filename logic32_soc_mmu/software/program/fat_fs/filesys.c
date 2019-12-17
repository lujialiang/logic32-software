
#include "fat_filelib.h"
#include "disk.h"
#include "filesys.h"

static int diskio_read(uint32 sector, uint8 *buffer, uint32 sector_count)
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

static int diskio_write(uint32 sector, uint8 *buffer, uint32 sector_count)
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

int filesys_init(void)
{
	if( disk_initialize() != DISK_OK ) {
		return -1;
	}
	fl_init();
	if( fl_attach_media(diskio_read,diskio_write) != FAT_INIT_OK ) {
		return -2;
	}
	return 0;
}

void* fopen(const char *path, const char *mode)
{
	return fl_fopen(path, mode);
}

void fclose(void *f)
{
	fl_fclose(f);
}

int fflush(void *f)
{
	return fl_fflush(f);
}

int fgetc(void *f)
{
	return fl_fgetc(f);
}

char *fgets(char *s, int n, void *f)
{
	return fl_fgets(s, n, f);
}

int fputc(int c, void *f)
{
	return fl_fputc(c, f);
}

int fputs(const char * str, void *f)
{
	return fl_fputs(str, f);
}

int fwrite(const void * data, int size, int count, void *f)
{
	return fl_fwrite(data, size, count, f);
}

int fread(void * buffer, int size, int length, void *f)
{
	return fl_fread(buffer, size, length, f);
}

int fseek( void *f, long offset, int origin)
{
	return fl_fseek(f, offset, origin);
}

int fgetpos(void *f , unsigned * position)
{
	return fl_fgetpos(f, (uint32*)position);
}

long ftell(void *f)
{
	return fl_ftell(f);
}

int feof(void *f)
{
	return fl_feof(f);
}

int remove(const char * filename)
{
	return fl_remove(filename);
}

int mkdir(const char *path)
{
	return fl_createdirectory(path);
}

//#define rmdir(a)		0

