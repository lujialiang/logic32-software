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

#include <string.h>
#include <unistd.h>
#include "asm.h"
#include "include.h"
#include "keyword_hash.h"

char curr_file_name[256];
FILE *fp_in = NULL;
FILE *fp_out = NULL;
uint32_t line_num = 0;

uint8_t text_area[TEXT_AREA_SIZE];
uint32_t text_size = 0;
uint8_t data_area[DATA_AREA_SIZE];
uint32_t data_size = 0;
uint32_t bss_size = 0;

struct relocation_info text_reloc[TEXT_RELOC_SIZE];
uint32_t text_reloc_cnt = 0;
struct relocation_info data_reloc[DATA_RELOC_SIZE];
uint32_t data_reloc_cnt = 0;

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

static char *change_suffix(char *src_file_name, char *src_suffix, char *dest_file_name, char *dest_suffix)
{
	int len = strlen(src_file_name);
	int src_suffix_len = strlen(src_suffix);
	char *p;
	if( len <= src_suffix_len ) {
		return NULL;
	}
	else {
		p = src_file_name + (len - src_suffix_len);
		if( strcmp(src_suffix, p) != 0 ) {
			return NULL;
		}
		strcpy(dest_file_name,src_file_name);
		p = dest_file_name + (len - src_suffix_len);
		*p = '\0';
		strcat(p, dest_suffix);
		return dest_file_name;
	}
}

local_symbol_t local_symbol_table[LOCAL_SYMOBOL_SIZE];
int local_symbol_cnt = 0;

void local_symbol_table_init(void)
{
	int i;
	for(i=0;i<LOCAL_SYMOBOL_SIZE;i++) {
		local_symbol_table[i].lable = 0;
		local_symbol_table[i].value = 0;
	}
	local_symbol_cnt = 0;
}

void dump_local_symbol_table(void)
{
	int i;
	putchar(10);
	printf("dump local symbol table\n");
	for(i=0;i<local_symbol_cnt;i++) {
		printf("name:%d, value=0x%08x\n", local_symbol_table[i].lable, local_symbol_table[i].value);
	}
	putchar(10);
}

struct relocation_info *next_text_reloc(void)
{
	if( text_reloc_cnt < TEXT_RELOC_SIZE ) {
		return &text_reloc[text_reloc_cnt++];
	}
	return NULL;
}

struct relocation_info *next_data_reloc(void)
{
	if( data_reloc_cnt < DATA_RELOC_SIZE ) {
		return &data_reloc[data_reloc_cnt++];
	}
	return NULL;
}

int add_reloc(uint32_t address, int length, struct nlist *psym)
{
	struct relocation_info *pr;
	if( cur_seg == N_TEXT ) {
		pr = next_text_reloc();
		pr->pcrel_flag = 1;
	}
	else if( cur_seg == N_DATA ) {
		pr = next_data_reloc();
		pr->pcrel_flag = 0;
	}
	else {
		error("internal error, unknow segment");
		return -1;
	}
	pr->address = address;
	pr->length = length;
	pr->space = 0;
	if( (psym->n_type & ~N_EXT) == N_UNDF ) {
		int i = 0;
		for( i=0; i< global_symbol_cnt; i++ ) {
			char *psn = &strtab[global_symbuf[i].n_strx];
			if( psym->n_name == NULL ) {
				error("internal error, psym->n_name NULL");
			}
			if( psn == NULL ) {
				error("internal error, global_symbuf[%d]->name NULL",i);
			}
			if( strcmp(psym->n_name, psn) == 0 )
			{
				pr->index = i;
				break;
			}
		}
		if( i >= global_symbol_cnt ) {
			pr->index = 0;
			error("undefind globol symbol '%s'",psym->n_name);
		}
		pr->extern_flag = 1;
	}
	else {
		pr->index = psym->n_type;
		pr->extern_flag = 0;
	}
	return 0;
}

void usage(void)
{
	fprintf(stderr, "usage: l32-asm <xxx.s> [-o xxx.o]\n");
}

static char obj_file_name[256];

int main(int argc,char* argv[])
{
	fp_in = NULL;
	fp_out = NULL;
	line_num = 0;

	if( argc != 2 && argc != 4 ) {
		fprintf(stderr, "error: bad argument\n");
		usage();
		return -1;
	}
	else {
		// check argv[2]
		if( argc == 4 && strcmp("-o", argv[2]) ) {
			fprintf(stderr, "Error: Argument\n");
			usage();
			return -1;
		}
		// open in
		if( match_suffix(argv[1], ".s") ) {
			fp_in = fopen(argv[1],"rt");
			if(fp_in == NULL) {
				perror(argv[1]);
				return -1;
			}
			strcpy(curr_file_name, argv[1]);
		}
		else {
			fprintf(stderr,"%s not a .s file\n",argv[1]);
			return -1;
		}
	}
	// open out
	if(argc == 2) {
		if( change_suffix(argv[1], ".s", obj_file_name, ".o") ) {
			fp_out = fopen( obj_file_name, "wb");
			if(fp_out == NULL) {
				perror(obj_file_name);
				fclose(fp_in);
				return -1;
			}
		}
		else {
			fclose(fp_in);
			fprintf(stderr,"convet .s file name to .o file name error\n");
			return -1;
		}
	}
	else if(argc == 4) {
		fp_out = fopen( argv[3], "wb");
		strcpy(obj_file_name, argv[3]);
		if(fp_out == NULL) {
			perror(argv[2]);
			fclose(fp_in);
			return -1;
		}
		strcpy(curr_file_name, argv[3]);
	}
	else {
		fclose(fp_in);
		return -1;
	}

	memset(text_area,0,sizeof(text_area));
	memset(data_area,0,sizeof(data_area));
	text_size = 0;
	data_size = 0;
	bss_size = 0;

	memset(text_reloc,0,sizeof(text_reloc));
	memset(data_reloc,0,sizeof(data_reloc));
	text_reloc_cnt = 0;
	data_reloc_cnt = 0;

	strtab_init();
	hsh_init();

	/* 初始化关键字散列表 */
	int i;
	int n;
	const keyword_hash_t *p;
	keyword_hash_init();
	p = keyword_buffer;
	n = get_keyhash_buffer_size(); 
	for(i=0; i<n;i++) {
		keyword_hash_in(p->name, p->type, p->value, p->pfunc);
		p++;
	}
	/* 初始化操作码散列表 */
	p = function_buffer;
	n =  get_function_buffer_size();
	for(i=0; i<n;i++) {
		keyword_hash_in(p->name, p->type, p->value, p->pfunc);
		p++;
	}

	pass = 1;
	include_init();
	local_symbol_table_init();
	parser();
	if( !error_cnt ) {
		dump_globals();
		pass = 2;
		parser();
		if( !error_cnt ) {
			out_obj(fp_out);
		}
	}

	fclose(fp_in);

	if( error_cnt ) {
		if( unlink(obj_file_name) ) {
			perror(obj_file_name);
		}
	}

	return (error_cnt != 0);
}
