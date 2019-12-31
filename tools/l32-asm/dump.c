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

#define MAX_SYMBOLS 1024

static int match_suffix(char *file_name, char *suffix)
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

int main(int argc,char *argv[])
{
	FILE *fp = NULL;
	struct exec header;
	if( argc != 2 ) {
		fprintf( stderr, "usage: %s [ xxx.out | xxx.o]\n", argv[0] );
		return -1;
	}
	if( ( match_suffix(argv[1] , ".o") == 0 ) && (match_suffix( argv[1] , ".out") == 0 ) ) {
		fprintf( stderr, "%s: not a correct object filename\n", argv[1] ); 
		return -1;
	}
	if( (fp = fopen(argv[1],"rb") ) == NULL ) {
		perror(argv[1]);
		return -1;
	}
	fread( &header, sizeof(struct exec), 1, fp);

	printf("\ntext size : %d\n", header.a_text);
	printf("data size : %d\n", header.a_data);
	printf("bss size : %d\n", header.a_bss);
	printf("sybol table size : %d\n" , header.a_syms);
	printf("string table size : %d\n", header.a_strs);

	uint32_t offset = sizeof(struct exec) + header.a_text + header.a_data;
	offset += header.a_trsize;
	offset += header.a_drsize;
	rewind(fp);
	fseek( fp, offset, SEEK_SET );

	char string_table[MAX_SYMBOLS*32];
	struct nlist symbols[MAX_SYMBOLS];
	int n = header.a_syms/sizeof(struct nlist);
	printf("n = %d\n", n);
	printf("\ndump symbols\n");
	fread( symbols, sizeof(struct nlist)*n, 1, fp);
	fread( string_table, header.a_syms, 1, fp);

	int i;
	for(i=0; i < n; i++ ) {
		printf("\nindex = %d\n", i);
		printf("n_name: %s\n", &string_table[symbols[i].n_strx]);
		printf("n_type: %08x\n", symbols[i].n_type);
		printf("n_value: %08x\n", symbols[i].n_value);
	}

	printf("\n");
	fclose(fp);
	return 0;
}

