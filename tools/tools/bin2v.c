#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

size_t get_file_size(const char *file_name)
{
	struct stat buf;
	if( stat(file_name,&buf)<0 ) { 
		return 0;
	}   
	return buf.st_size;
}

uint8_t read_buffer[1024*512];

int main(int argc, char *argv[])
{
	if(argc != 2) {
		fprintf(stderr, "usage: %s binfile\n", argv[0]);
		exit(1);
	}
	size_t filesize = get_file_size(argv[1]);
	if( filesize <= 0 ) {
		fprintf(stderr, "bad file size\n");
		exit(1);
	}
	if( filesize > sizeof(read_buffer) ) {
		fprintf(stderr, "file too large\n");
		exit(1);
	}
	FILE *fp = fopen(argv[1], "rb");
	if( fp == NULL ) {
		perror(argv[1]);
		exit(1);
	}
	fread(read_buffer, filesize, 1, fp);
	fclose(fp);
	int i;
	for(i=0; i<filesize; i+=4) {
		//printf("ram[%d] <= 32'h%02x%02x%02x%02x;\n", i/4, read_buffer[i+3], read_buffer[i+2], read_buffer[i+1], read_buffer[i]);
		printf("%02x%02x%02x%02x	// 0x%08x\n", read_buffer[i+3], read_buffer[i+2], read_buffer[i+1], read_buffer[i], i);
	}
	return 0;
}

