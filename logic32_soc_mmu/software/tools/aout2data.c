#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "a_out.h"

int match_suffix(char *file_name, char *suffix)
{
	int len = strlen(file_name);
	int suffix_len = strlen(suffix);
	char *p; 
	if( len <= suffix_len ) { 
		return 0;
	}   
	else {
		p = file_name + (len - suffix_len);
		if( strcmp (suffix, p) != 0) {
			return 0;
		}
		return 1;
	}   
}

int main(int argc,char *argv[])
{
	FILE *fp = NULL;
	struct exec header;
	if( argc != 2 ) {
		fprintf( stderr, "usage: %s xxx.out\n", argv[0] );
		return -1;
	}
	if( match_suffix( argv[1] , ".out" ) == 0 ) {
		fprintf( stderr, "%s: not a correct object filename\n", argv[1] ); 
		return -1;
	}
	if( (fp = fopen(argv[1],"rb") ) == NULL ) {
		perror(argv[1]);
		return -1;
	}
	
	fread( &header, sizeof(struct exec), 1, fp);

	if( header.a_text%4 != 0 ) {
		fprintf(stderr, "%s: bad text size", argv[1]);
		fclose(fp);
		return -1;
	}

	if( header.a_data%4 != 0 ) {
		fprintf(stderr, "%s: bad data size", argv[1]);
		fclose(fp);
	}

	if( header.a_text > 0 ) {
		FILE *fp_text = fopen("rom.data","wt");
		if( !fp_text ) {
			perror("rom.data");
		}
		else {
			int i;
			uint8_t *text_buffer = (uint8_t*)malloc(header.a_text);
			if( !text_buffer ) {
				perror("alloc text buffer:");
				fclose(fp_text);
				fclose(fp);
				return -1;
			}
			fread( text_buffer, header.a_text, 1, fp);

			for(i=0; i<header.a_text; i+=4) {
				fprintf(fp_text, "%02x%02x%02x%02x    // 0x%08x\n", text_buffer[i+3], text_buffer[i+2], text_buffer[i+1], text_buffer[i], i); 
			}

			free(text_buffer);
		}
		fclose(fp_text);
	}

	putchar(10);

	if( header.a_data > 0 ) {
		FILE *fp_data = fopen("ram.data","wt");
		if( !fp_data ) {
			perror("ram.data:");
		}
		else {
			int i;
			uint8_t *data_buffer = (uint8_t*)malloc(header.a_data);
			if( !data_buffer ) {
				perror("alloc data buffer:");
				fclose(fp_data);
				fclose(fp);
				return -1;
			}
			fread( data_buffer, header.a_data, 1, fp);

			for(i=0; i<header.a_data; i+=4) {
				fprintf(fp_data, "%02x%02x%02x%02x    // 0x%08x\n", data_buffer[i+3], data_buffer[i+2], data_buffer[i+1], data_buffer[i], i); 
			}

			free(data_buffer);
		}
		fclose(fp_data);
	}

	fclose(fp);
	return 0;
}
