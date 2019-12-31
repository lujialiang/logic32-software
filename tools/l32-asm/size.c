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
		fprintf( stderr, "usage: %s < [xxx.o] | [xxx.out] >\n", argv[0] );
		return -1;
	}
	if( ( match_suffix( argv[1] , ".o" ) == 0 ) && ( match_suffix( argv[1] , ".out" ) == 0 ) ) {
		fprintf( stderr, "%s: not a correct object filename\n", argv[1] ); 
		return -1;
	}
	if( (fp = fopen(argv[1],"rb") ) == NULL ) {
		perror(argv[1]);
		return -1;
	}
	fread( &header, sizeof(struct exec), 1, fp);
	printf("\n");
	printf("filename: %s\n", argv[1]);
	printf("magic: 0x%08X\n", header.a_magic);
	printf("text size: %d(%.2fK)\n", header.a_text, (float)header.a_text/1024.0);
	printf("data size: %d(%.2fK)\n", header.a_data, (float)header.a_data/1024.0);
	printf("bss size: %d(%.2fK)\n", header.a_bss, (float)header.a_bss/1024.0);
	printf("symbol table size: %d(%.2fK)\n", header.a_syms, (float)header.a_syms/1024.0);
	printf("entry point: 0x%08x\n", header.a_entry);
	if(	(int)header.a_trsize == 0 ) {
		printf("text relocation info size: 0\n");
	}
	else {
		printf("text relocation info size: (%d x %d) = %d\n", \
			(int)sizeof(struct relocation_info), \
			(int)(header.a_trsize/sizeof(struct relocation_info)), \
			(int)header.a_trsize);
	}
	if(	(int)header.a_drsize == 0 ) {
		printf("data relocation info size: 0\n");
	}
	else {
		printf("data relocation info size: (%d x %d) =%d\n", \
			(int)sizeof(struct relocation_info), \
			(int)(header.a_drsize/sizeof(struct relocation_info)), \
			(int)header.a_drsize);
	}
	printf("string table size: %d\n", header.a_strs);
	printf("\n");
	fclose(fp);
	return 0;
}
