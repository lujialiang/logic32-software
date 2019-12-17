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

#include "asm.h"
#include "a_out.h"

int out_obj( FILE *fp_obj )
{
	struct exec header;
	int i;
	if( fp_obj == NULL ) {
		return -1;
	}
	memset(&header, 0, sizeof(struct exec));
	header.a_magic = A_MAGIC1;
	header.a_text = text_size;
	header.a_data = data_size;
	header.a_bss = bss_size;
	header.a_syms = sizeof(struct nlist)*global_symbol_cnt;
	header.a_entry = 0;
	header.a_trsize = text_reloc_cnt * sizeof(struct relocation_info);
	header.a_drsize = data_reloc_cnt * sizeof(struct relocation_info);
	header.a_strs = strtab_idx;
	// header
	fwrite(&header,sizeof(struct exec),1,fp_obj);
	// text
	fwrite(text_area,text_size,1,fp_obj);
	// data
	fwrite(data_area,data_size,1,fp_obj);
	// text relocation_info
	for( i=0; i< text_reloc_cnt; i++ ) {
		fwrite(&text_reloc[i], sizeof(struct relocation_info), 1, fp_obj);
	}
	// data relocation_info
	for( i=0; i< data_reloc_cnt; i++) {
		fwrite(&data_reloc[i], sizeof(struct relocation_info), 1, fp_obj);
	}
	// global symbol
	fwrite(global_symbuf,header.a_syms,1,fp_obj);
	// string table
	fwrite(strtab,strtab_idx,1,fp_obj);
	fclose(fp_obj);
	return 0;
}

int read_obj(char *obj_file_name)
{
	FILE *fp_obj;
	struct exec header;
	if( (fp_obj = fopen(obj_file_name,"rb")) == NULL ) {
		perror(obj_file_name);
		return -1;
	}
	fread(&header,(sizeof(struct exec)),1,fp_obj);
	printf("magic:%04o\n", header.a_magic);
	printf("a_text:%d\n",  header.a_text);
	printf("a_data:%d\n",  header.a_data);
	printf("a_bss:%d\n",  header.a_bss);
	printf("a_syms:%d\n",  header.a_syms);
	fclose(fp_obj);
	return 0;
}

void out_byte(unsigned char byte)
{
	unsigned char *p = NULL;
	uint32_t *psize = NULL;
	if( cur_seg == N_TEXT ) {
		p = &text_area[text_size];
		psize = &text_size;
	}
	else if( cur_seg == N_DATA ) {
		p = &data_area[data_size];
		psize = &data_size;
	}
	else if( cur_seg == N_BSS ) {
		error("out byte to bss segment");
		bss_size += BYTE_SIZE;
		return;
	}
	else {
		error("%s:%d internal error", __FILE__, __LINE__);
		return;
	}
	*p = byte;
	*psize += BYTE_SIZE;
}

void out_word(uint32_t word)
{
	unsigned char *p = NULL;
	uint32_t *psize = NULL;
	if( cur_seg == N_TEXT ) {
		p = &text_area[text_size];
		psize = &text_size;
	}
	else if( cur_seg == N_DATA ) {
		p = &data_area[data_size];
		psize = &data_size;
	}
	else {
		error("%s:%d internal error", __FILE__, __LINE__);
		return;
	}
	p[0] = word & 0xff;
	p[1] = word>>8 & 0xff;
	p[2] = word>>16 & 0xff;
	p[3] = word>>24 & 0xff;
	*psize += WORD_SIZE;
}

void out_code(uint32_t n)
{
	if( cur_seg != N_TEXT ) {
		error("only allow in code segment");
	}
	out_word(n);
}

uint32_t bit_field(int h, int l, uint32_t v)
{
	uint32_t t;
	int i;
	t = 0;
	for(i=l ;i<=h;i++) {
		t |= (1<<i);
	}
	return (t & (v<<l));
}

