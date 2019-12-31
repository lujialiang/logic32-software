/*-
 * Copyright (c) 2017 Lu JiaLiang
 * Email: 1056568029@qq.com
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id$
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "a_out.h"

static int _is_a_out_file(char *file_name)
{
	int len = strlen(file_name);
	int suffix_len = strlen(".out");
	char *p;
	if( len <= suffix_len ) {
		return 0;
	}
	else {
		p = file_name + (len - suffix_len);
		if( strcmp(".out", p) != 0) {
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
	if( _is_a_out_file( argv[1] ) == 0 ) {
		fprintf( stderr, "%s: not a correct object filename\n", argv[1] ); 
		return -1;
	}
	if( (fp = fopen(argv[1],"rb") ) == NULL ) {
		perror(argv[1]);
		return -1;
	}
	
	fread( &header, sizeof(struct exec), 1, fp);

	if( header.a_text > 0 ) {
		FILE *fp_text = fopen("obj_code_pkg.vhd","wt");
		if( !fp_text ) {
			perror("create obj_code_pkg.vhd");
		}
		else {
			uint8_t *text_buffer = (uint8_t*)malloc(header.a_text);
			if( !text_buffer ) {
				perror("alloc text buffer:");
				fclose(fp_text);
				fclose(fp);
				return -1;
			}
			fread( text_buffer, header.a_text, 1, fp);
			printf("dump text to obj_code_pkg.vhd\n");
			printf("text size : %d*4 = %d\n", header.a_text/4, header.a_text);
			fprintf( fp_text, "library ieee;\n");
			fprintf( fp_text, "use ieee.std_logic_1164.all;\n");
			fprintf( fp_text, "use ieee.numeric_std.all;\n");
			fprintf( fp_text, "\n");
			fprintf( fp_text, "use work.logic_32_pkg.all;\n");
			fprintf( fp_text, "\n");
			fprintf( fp_text, "package obj_code_pkg is\n\n");

			if( header.a_text % 4 == 0 ) {
				int i;
				uint32_t data;
				uint32_t n;
				n = header.a_text/4;
				fprintf( fp_text, "constant OBJ_CODE_SIZE  : integer := %d;\n\n", n);
				fprintf( fp_text, "constant OBJ_CODE : obj_code_t(integer range 0 to OBJ_CODE_SIZE - 1) := (\n\t");
				for(i=0; i<n; i++) {
					data = text_buffer[i*4] | text_buffer[i*4+1]<<8 | text_buffer[i*4+2]<<16 | text_buffer[i*4+3]<<24;
					if( i == n - 1 ) {
						fprintf( fp_text, "X\"%08x\"", data);
						fprintf( fp_text, "\n);");
						break;
					}
					else {
						fprintf( fp_text, "X\"%08x\",", data);
					}
					if( (i+1)%8 ) {
						fputc(32,fp_text);
					}
					else {
						fprintf( fp_text, "\n\t");
					}
				}
			}
			else {
				int i;
				uint32_t data;
				uint32_t n;
				n = header.a_text/4;
				fprintf( fp_text, "constant OBJ_CODE_SIZE  : integer := %d;\n\n", n+1);
				fprintf( fp_text, "constant OBJ_CODE : obj_code_t(integer range 0 to OBJ_CODE_SIZE - 1) := (\n\t");
				for(i=0; i<n; i++) {
					data = text_buffer[i*4] | text_buffer[i*4+1]<<8 | text_buffer[i*4+2]<<16 | text_buffer[i*4+3]<<24;
					fprintf( fp_text, "X\"%08x\",", data);
					if( (i+1)%8 ) {
						fputc(32,fp_text);
					}
					else {
						fprintf( fp_text, "\n\t");
					}
				}
				switch( header.a_text%4 ) {
					case 1:
						data = text_buffer[i*4];
						fprintf(fp_text, "X\"%08x\"", data);
						break;
					case 2:
						data = text_buffer[i*4] | text_buffer[i*4+1]<<8;
						fprintf(fp_text, "X\"%08x\"", data);
						break;
					case 3:
						data = text_buffer[i*4] | text_buffer[i*4+1]<<8 | text_buffer[i*4+2]<<16;
						fprintf(fp_text, "X\"%08x\"", data);
						break;
				}
				fprintf( fp_text, "\n);");

			}
			fprintf( fp_text, "\nend package;");
			free(text_buffer);
		}
		fclose(fp_text);
	}

	putchar(10);

	if( header.a_data > 0 ) {
		FILE *fp_data = fopen("obj_data_pkg.vhd","wt");
		if( !fp_data ) {
			perror("open obj_data_pkg.vhd:");
		}
		else {
			uint8_t *data_buffer = (uint8_t*)malloc(header.a_data);
			if( !data_buffer ) {
				perror("alloc data buffer:");
				fclose(fp_data);
				fclose(fp);
				return -1;
			}
			fread( data_buffer, header.a_data, 1, fp);
			printf("dump data to obj_data_pkg.vhd\n");
			printf("data size : %d*4 = %d\n", header.a_data/4, header.a_data);
			fprintf( fp_data, "library ieee;\n");
			fprintf( fp_data, "use ieee.std_logic_1164.all;\n");
			fprintf( fp_data, "use ieee.numeric_std.all;\n");
			fprintf( fp_data, "\n");
			fprintf( fp_data, "use work.logic_32_pkg.all;\n");
			fprintf( fp_data, "\n");
			fprintf( fp_data, "package obj_data_pkg is\n\n");

			if( header.a_data % 4 == 0 ) {
				int i;
				uint32_t data;
				uint32_t n;
				n = header.a_data/4;
				fprintf( fp_data, "constant OBJ_DATA_SIZE  : integer := %d;\n", n);
				fprintf( fp_data, "constant OBJ_DATA : obj_code_t(integer range 0 to OBJ_DATA_SIZE - 1) := (\n\t");
				for(i=0; i<n; i++) {
					data = data_buffer[i*4] | data_buffer[i*4+1]<<8 | data_buffer[i*4+2]<<16 | data_buffer[i*4+3]<<24;
					if( i == n - 1 ) {
						fprintf( fp_data, "X\"%08x\"", data);
						fprintf( fp_data, "\n);");
						break;
					}
					else {
						fprintf( fp_data, "X\"%08x\",", data);
					}
					if( (i+1)%8 ) {
						fputc(32,fp_data);
					}
					else {
						fprintf( fp_data, "\n\t");
					}
				}
			}
			else {
				int i;
				uint32_t data;
				uint32_t n;
				n = header.a_data/4;
				fprintf( fp_data, "constant OBJ_DATA_SIZE  : integer := %d;\n\n", n+1);
				fprintf( fp_data, "constant OBJ_DATA : obj_code_t(integer range 0 to OBJ_DATA_SIZE - 1) := (\n\t");
				for(i=0; i<n; i++) {
					data = data_buffer[i*4] | data_buffer[i*4+1]<<8 | data_buffer[i*4+2]<<16 | data_buffer[i*4+3]<<24;
					fprintf( fp_data, "X\"%08x\",", data);
					if( (i+1)%8 ) {
						fputc(32,fp_data);
					}
					else {
						fprintf( fp_data, "\n\t");
					}
				}
				switch( header.a_data%4 ) {
					case 1:
						data = data_buffer[i*4];
						fprintf(fp_data, "X\"%08x\"", data);
						break;
					case 2:
						data = data_buffer[i*4] | data_buffer[i*4+1]<<8;
						fprintf(fp_data, "X\"%08x\"", data);
						break;
					case 3:
						data = data_buffer[i*4] | data_buffer[i*4+1]<<8 | data_buffer[i*4+2]<<16;
						fprintf(fp_data, "X\"%08x\"", data);
						break;
				}
				fprintf( fp_data, "\n);");

			}
			fprintf( fp_data, "\nend package;");
			free(data_buffer);
		}
		fclose(fp_data);
	}

	fclose(fp);
	return 0;
}
