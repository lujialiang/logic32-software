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

void dump_symbols(void)
{
	int i;
	for(i=0;i<HSHSIZ;i++) {
		if( symbuf[i].n_name ) {
			char *symbol_name = symbuf[i].n_name;
			printf("%-16s %08x",symbol_name, symbuf[i].n_value);
			if( symbuf[i].n_type == N_TEXT ) {
				printf("\t TEXT");
			}
			else if( symbuf[i].n_type == N_DATA ) {
				printf("\t DATA");
			}
			else if( symbuf[i].n_type == N_BSS ) {
				printf("\t BSS");
			}
			else if( symbuf[i].n_type == N_ABS ) {
				printf("\t ABS");
			}
			else {
				printf("\t ????");
			}
			printf("\n");
		}
	}
}

void dump_areas(void)
{
	unsigned i;
	printf("\n\ntext[%d]\n",text_size);
	for( i=0; i<text_size; i++ ) {
		printf("%02X,",text_area[i]);
		putchar((i+1)%16==0?10:32);
	}
	printf("\n\ndata[%d]\n",data_size);
	for( i=0; i<data_size; i++ ) {
		printf("%02X,",data_area[i]);
		putchar((i+1)%16==0?10:32);
	}
	printf("\n\nbss[%d]\n",bss_size);
}

void dump_strtab(void)
{
	unsigned i;
	int cnt = 0;
	printf("\n\ndump string table\n\n");
	for(i=0,cnt=0; i< strtab_idx; i++,cnt++) {
		char *p = &strtab[i];
		printf("%d:\t\"%s\"\n",cnt+1,p);
		p = &strtab[i];
		while(*p) {
			p++;
			i++;
		}
	}
}

struct nlist global_symbuf[HSHSIZ];
uint32_t global_symbol_cnt = 0;

void dump_globals(void)
{
	int i;
	global_symbol_cnt = 0;
	for(i=0;i<HSHSIZ;i++) {
		if( symbuf[i].n_name ) {
			if( symbuf[i].n_type & N_EXT ) {
				struct nlist *dst = &global_symbuf[global_symbol_cnt];
				struct nlist *src = &symbuf[i];				
				memcpy(dst,src,sizeof(struct nlist));
				dst->n_strx = dst->n_name - strtab;
				global_symbol_cnt++;
			}
		}
	}
}

