/*-
 * Copyright (c) 2017,2018,2019 Lu JiaLiang
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
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "a_out.h"

static uint32_t get_file_size(const char *file_name) {
	struct stat buf;
	if( stat(file_name,&buf)<0 ) {
		return 0;
	}
	return (uint32_t)buf.st_size;
}

/*
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
		if( strcmp(suffix, p) != 0) {
			return 0;
		}
		return 1;
	}
}
*/

int main(int argc,char *argv[])
{
	FILE *fp_out = NULL;
	struct exec header;
	bool have_text = false;
	bool have_data = false;
	bool have_out = false;
	char text_file_name[32];
	char data_file_name[32];
	char out_file_name[32];
	int ch;
	while ( (ch = getopt(argc, argv, "t:d:o:")) != -1 ) {
		switch(ch) {
			case 't':
				have_text = true;
				strncpy(text_file_name, optarg, 32);
				text_file_name[31] = '\0';
				break;
			case 'd':
				have_data = true;
				strncpy(data_file_name, optarg, 32);
				data_file_name[31] = '\0';
				break;
			case 'o':
				have_out = true;
				strncpy(out_file_name, optarg, 32);
				out_file_name[31] = '\0';
				break;
			default:
				break;
		}
	}
	uint32_t text_size = 0;
	uint32_t data_size = 0;
	uint32_t bss_size = 0;

	FILE *fp_text = NULL;
	FILE *fp_data = NULL;

	if ( have_text ) {
		fp_text = fopen(text_file_name, "rb");
		if (fp_text == NULL) {
			perror(text_file_name);
			exit(1);
		}
		text_size = get_file_size(text_file_name);
		printf("the text: %s, size: %d\n", text_file_name, text_size);
	}
	if ( have_data ) {
		fp_data = fopen(data_file_name, "rb");
		if (fp_data == NULL) {
			perror(data_file_name);
			exit(1);
		}
		data_size = get_file_size(data_file_name);
		printf("the data: %s, size: %d\n", data_file_name, data_size);
	}
	if ( have_out ) {
		printf("the output: %s\n", out_file_name);
	}
	else {
		strcpy(out_file_name, "a.out");
	}

	fp_out = fopen(out_file_name, "wb");
	if (fp_out == NULL) {
		perror(out_file_name);
		exit(1);
	}

	memset(&header, 0, sizeof(struct exec));
	header.a_magic = A_MAGIC1;
	header.a_text = text_size;
	header.a_data = data_size;
	header.a_bss = bss_size;
	header.a_syms = 0;
	header.a_entry = 0;
	header.a_trsize = 0;
	header.a_drsize = 0;
	header.a_strs = 0;

	uint8_t *text_area = NULL;
	uint8_t *data_area = NULL;
	if ( text_size ) {
		text_area = malloc(text_size);
		if ( text_area == NULL ) {
			perror("malloc(text_size)");
			exit(1);
		}
		fread(text_area, text_size, 1, fp_text);
		fclose(fp_text);
	}

	if ( data_size ) {
		data_area = malloc(data_size);
		if ( data_area == NULL ) {
			perror("malloc(data_size)");
			exit(1);
		}
		fread(data_area, data_size, 1, fp_data);
		fclose(fp_data);
	}

	// header
	fwrite(&header,sizeof(struct exec),1,fp_out);

	// text
	if ( text_area ) {
		fwrite(text_area,text_size,1,fp_out);
	}
	// data
	if ( data_area ) {
		fwrite(data_area,data_size,1,fp_out);
	}

	fclose(fp_out);

	return 0;

}
