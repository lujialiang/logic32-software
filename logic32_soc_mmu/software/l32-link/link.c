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
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "a_out.h"
#include "asm.h"
#include "hash.h"
#include "link.h"
#include "misc.h"
#include "_ar.h"

#define _here() //printf("@ %s, %d\n", __FILE__, __LINE__ )

struct module modules_head;

static uint32_t total_text_size = 0;
static uint32_t total_data_size = 0;
static uint32_t total_bss_size = 0;

static uint32_t text_seg_load_address = 0;
static uint32_t data_seg_load_address = 0;
static uint32_t bss_seg_load_address = 0;

struct nlist sym_text_start;
struct nlist sym_text_size;
struct nlist sym_data_start;
struct nlist sym_data_size;
struct nlist sym_bss_start;
struct nlist sym_bss_size;

void insert_global_sym(struct nlist *psym, const char *n_name, uint8_t n_type, uint32_t n_value)
{
    struct glosym *pglosym;
	if( hash_lookup( (char *) n_name ) != NULL ) {
		error("linker: multiply defined symbol '%s'", n_name);
		return;
	}
	psym->n_name = (char *)n_name;
	psym->n_type = n_type;
	psym->n_value = n_value;
	pglosym = hash_in(psym, 1);
	pglosym->filename = "NONE";
}

struct module *load_module(char *file, int fd, int check_size)
{
	struct stat buf;
	struct exec exec_hdr;
	int nr;
	size_t size;
	// read exec header
	if( (nr = read(fd, &exec_hdr, sizeof(struct exec))) != sizeof(struct exec) ) {
		_here();
		fprintf(stderr, "%s: %s\n", file, strerror(errno));
		error_cnt++;
		//close(fd);
		return NULL;
	}
	// check magic
	if( exec_hdr.a_magic != A_MAGIC1 ) {
		_here();
		fprintf(stderr, "%s: %s\n", file, "bad format");
		error_cnt++;
		//close(fd);
		return NULL;
	}
	if( check_size ) {
		// get file size
		if( fstat(fd, &buf) < 0 ) {
			_here();
			fprintf(stderr, "%s: %s\n", file, strerror(errno));
			error_cnt++;
			//close(fd);
			return NULL;
		}
		// check size
		size = sizeof(struct exec);
		size += exec_hdr.a_text;
		size += exec_hdr.a_data;
		size += exec_hdr.a_syms;
		size += exec_hdr.a_trsize;
		size += exec_hdr.a_drsize;
		size += exec_hdr.a_strs;
		if( size != buf.st_size ) {
			_here();
			fprintf(stderr, "%s: %s\n", file, "bad format");
			error_cnt++;
			//close(fd);
			return NULL;
		}
	}

	struct module *pm = (struct module*)malloc(sizeof(struct module));
	if( pm == NULL ) {
		_here();
		fprintf(stderr, "%s: %s\n", "malloc()", strerror(errno));
		error_cnt++;
		//close(fd);
		return NULL;
	}
	memset(pm,0,sizeof(struct module));
	pm->next = NULL;

	/* copy file name */
	pm->filename = (char*)malloc(strlen(file)+1);
	strcpy(pm->filename,file);
	/* copy exec header */
	memcpy(&(pm->header), &exec_hdr, sizeof(struct exec));

	/* read text segment */
	if( pm->header.a_text ) {
		pm->text = (uint8_t *)malloc(pm->header.a_text);
		if( (nr = read(fd, pm->text, pm->header.a_text)) != pm->header.a_text ) {
			_here();
			fprintf(stderr, "%s: %s\n", file, strerror(errno));
			error_cnt++;
			//close(fd);
			free(pm);
			return NULL;
		}
	}

	/* read data segment */
	if( pm->header.a_data ) {
		pm->data = (uint8_t *)malloc(pm->header.a_data);
		nr = read(fd, pm->data, pm->header.a_data);
		if( nr != pm->header.a_data ) {
			_here();
			fprintf(stderr, "%s: %s\n", file, strerror(errno));
			error_cnt++;
			//close(fd);
			free(pm);
			return NULL;
		}
	}
	/* read text relocationg info */
	if( pm->header.a_trsize ) {
		pm->textrel = (struct relocation_info*)malloc(pm->header.a_trsize);
		nr = read(fd, pm->textrel,pm->header.a_trsize);
		if( nr != pm->header.a_trsize ) {
			_here();
			fprintf(stderr, "%s: %s\n", file, strerror(errno));
			error_cnt++;
			//close(fd);
			free(pm);
			return NULL;
		}
	}
	/* read data relocationg info */
	if( pm->header.a_drsize ) {
		pm->datarel = (struct relocation_info*)malloc(pm->header.a_drsize);
		nr = read(fd, pm->datarel, pm->header.a_drsize);
		if( nr != pm->header.a_drsize ) {
			_here();
			fprintf(stderr, "%s: %s\n", file, strerror(errno));
			error_cnt++;
			//close(fd);
			free(pm);
			return NULL;
		}
	}
	/* read symbol table */
	if( pm->header.a_syms ) {
		pm->symbols = (struct nlist*)malloc(pm->header.a_syms);
		nr = read(fd, pm->symbols, pm->header.a_syms);
		if( nr != pm->header.a_syms ) {
			_here();
			fprintf(stderr, "%s: %s\n", file, strerror(errno));
			error_cnt++;
			//close(fd);
			free(pm);
			return NULL;
		}
	}
	/* read string table */
	pm->string_size =  pm->header.a_strs;
	if( pm->string_size ) {
		pm->strings = (char*)malloc(pm->string_size);
		nr = read(fd, pm->strings,pm->string_size);
		if( nr != pm->string_size ) {
			_here();
			fprintf(stderr, "%s: %s\n", file, strerror(errno));
			error_cnt++;
			//close(fd);
			free(pm);
			return NULL;
		}
	}
	/* set segment adderss */
	pm->text_start_address = total_text_size;
	pm->data_start_address = total_data_size;
	pm->bss_start_address = total_bss_size;
	/* end, increse section address */
	total_text_size += pm->header.a_text;
	total_data_size += pm->header.a_data;
	total_bss_size += pm->header.a_bss;
	/* round up to 4 byte */
	total_text_size = (total_text_size + 3) & 0xfffffffc;
	total_data_size = (total_data_size + 3) & 0xfffffffc;
	total_bss_size = (total_bss_size + 3) & 0xfffffffc;
	/* close object file */
	//close(fd);
	return pm;
}

int free_module(struct module *pm)
{
	if( pm == NULL ) {
		return -1;
	}
	free(pm->filename);
	free(pm->text);
	free(pm->data);
	free(pm->textrel);
	free(pm->datarel);
	free(pm->symbols);
	free(pm->strings);
	return 0;
}

int dump_module_symbols(struct module *pm)
{
	int i;
    struct nlist *psyms = pm->symbols;
    char *string_table = pm->strings;
    int n = (pm->header.a_syms)/sizeof(struct nlist);
    struct glosym *pglosym;
	for(i=0; i<n; i++) {
        psyms->n_name = &string_table[psyms->n_strx];
		if( (psyms->n_type & ~N_EXT) == N_UNDF ) {
			// symbol not defined, ignore it
			psyms++;
			continue;
		}
        pglosym = hash_in(psyms, 1);
		pglosym->filename = pm->filename;
        if( pglosym->defined ) {
            switch( psyms->n_type & ~N_EXT ) {
                case N_UNDF:
                    break;
                case N_ABS:
                    break;
                case N_TEXT:
                    pglosym->value += pm->text_start_address;
                    break;
                case N_DATA:
                    pglosym->value += pm->data_start_address;
                    break;
                case N_BSS:
                    pglosym->value += pm->bss_start_address;
                    break;
                default:
                    break;
            }
        }
		if( pglosym->multiply_defined ) {
			error("%s: multiply defined symbol '%s'", pglosym->filename, pglosym->name);
			struct glosym *tmp;
			int i = 1;
			tmp = hash_lookup( pglosym->name );
			while(tmp) {
				if( tmp->defined ) {
					fprintf(stderr, "#%d %s: symbol '%s'\n", i++, tmp->filename, pglosym->name);
				}
				tmp = tmp->link;
			}
			putchar(10);
		}
		psyms++;
	}
	return 0;
}

static char *out_file_name = NULL;
static char *map_file_name = NULL;
static struct glosym **symhash_tab = NULL;

static void usage(char *prog_name)
{
	fprintf(stdout, "usage: %s [-o outfile] [-M mapfile] objfiles,...\n", prog_name);
}

static int n_type_2_char(int n_type)
{
	int tmp = (n_type & N_EXT)?32:0;
	switch(n_type & ~N_EXT) {
		case N_UNDF:	return 'u'-tmp;
		case N_ABS:		return 'a'-tmp;
		case N_TEXT:	return 't'-tmp;
		case N_DATA:	return 'd'-tmp;
		case N_BSS:		return 'b'-tmp;
		default:		return '?';
	}
}

int link_main( struct module * pm )
{
    int i, j,n;
    if( pm == NULL ) {
        return -1;
    }
	while(pm) {
		/* if j == 0 , solve text relocate information */
		/* if j == 1 , solve data relocate information */
		#define TEXT_RELOC	0
		#define DATA_RELOC	1
		for(j=0; j<2; j++ ) {
			if( j == TEXT_RELOC ) {
				n = (pm->header.a_trsize)/sizeof(struct relocation_info);
			}
			else {
				n = (pm->header.a_drsize)/sizeof(struct relocation_info);
			}
			for( i=0; i<n; i++ ) {
				struct relocation_info *pr;
				uint32_t *p32;
				uint16_t *p16;
				if( j == 0 ) { 
					pr = &(pm->textrel[i]);
					p32 = (uint32_t*)(pm->text + pr->address);
					p16 = (uint16_t*)(pm->text + pr->address);
				}
				else {
					pr = &(pm->datarel[i]);
					p32 = (uint32_t*)(pm->data + pr->address);
					p16 = (uint16_t*)(pm->data + pr->address);
				}
				if( pr->extern_flag ) {
					// global symbols
					struct glosym *pglosym = NULL;
					pglosym = hash_lookup( pm->symbols[pr->index].n_name );
					if( pglosym == NULL ) {
						error("undefined symbol '%s'\n", (pm->symbols[pr->index]).n_name);
					}
					else {
						while( pglosym ) {
							if( pglosym->defined ) {
								switch( pr->length ) {
									case BIT16:
									{
										uint32_t tmp = *p16+(pglosym->value & 0xffff);
										if( tmp & 0xffff0000 ) {
											fprintf(stderr, "!!! BIT16 overfllow\n");
										}
										*p16 += pglosym->value & 0xffff;
										break;
									}
									case BIT16H:
									{
										uint32_t tmp = *p16+((pglosym->value>>16) & 0xffff);
										if( tmp & 0xffff0000 ) {
											fprintf(stderr, "!!! BIT16H overfllow\n");
										}
										*p16 += (pglosym->value>>16) & 0xffff;
										break;
									}
									case BIT30:
										*p32 = (*p32 & 0xc0000000) | (((uint32_t)pglosym->value)>>2);
										break;
									case BIT32:
										*p32 = pglosym->value;
										break;
									case BIT_16H_16L:
									{
										uint32_t immed_value;
										immed_value = (p16[0]<<16) + p16[2];
										immed_value += (uint32_t)pglosym->value;
										p16[0] = immed_value>>16;
										p16[2] = immed_value;
										break;
									}
									case BIT_16H_NONE_16L:
									{
										uint32_t immed_value;
										immed_value = (p16[0]<<16) + p16[4];
										immed_value += (uint32_t)pglosym->value;
										p16[0] = immed_value>>16;
										p16[4] = immed_value;
										break;
									}
									default:
										fatal_error("(%s, %d): unknow internal error", __FILE__, __LINE__);
										break;
								}
								break;
							}
							pglosym = pglosym->link;
						}
						if( pglosym == NULL ) {
							error("undefined symbol '%s'\n", (pm->symbols[pr->index]).n_name); 
						}
					}
				}
				else {
					// local symbols
					switch( pr->length ) {
						case BIT16:
							switch( pr->index & ~N_EXT) {
								case N_TEXT:
									*p16 = (pm->text_start_address + *p16)&0xffff;
									break;
								case N_DATA:
									*p16 = (pm->data_start_address + *p16)&0xffff;
									break;
								case N_BSS:
									*p16 = (pm->bss_start_address + *p16)&0xffff;
									break;
								default:
									fatal_error("(%s, %d): unknow internal error", __FILE__, __LINE__);
									break;
							}
							break;
						case BIT16H:
							switch( pr->index & ~N_EXT) {
								case N_TEXT:
									*p16 = ((pm->text_start_address + *p16)>>16)&0xffff;
									break;
								case N_DATA:
									*p16 = ((pm->data_start_address + *p16)>>16)&0xffff;
									break;
								case N_BSS:
									*p16 = ((pm->bss_start_address + *p16)>>16)&0xffff;
									break;
								default:
									fatal_error("(%s, %d): internal error", __FILE__, __LINE__);
									break;
							}
							break;
						case BIT_16H_16L:
						{
							uint32_t immed_value;
							immed_value = (p16[0]<<16) + p16[2];
							switch( pr->index & ~N_EXT) {
								case N_TEXT:
									immed_value += pm->text_start_address;
									break;
								case N_DATA:
									immed_value += pm->data_start_address;
									break;
								case N_BSS:
									immed_value += pm->bss_start_address;
									break;
								default:
									fatal_error("(%s, %d): internal error", __FILE__, __LINE__);
									break;
							}
							p16[0] = immed_value>>16;
							p16[2] = immed_value;
							break;
						}
						case BIT_16H_NONE_16L:
						{
							uint32_t immed_value;
							immed_value = (p16[0]<<16) + p16[4];
							switch( pr->index & ~N_EXT) {
								case N_TEXT:
									immed_value += pm->text_start_address;
									break;
								case N_DATA:
									immed_value += pm->data_start_address;
									break;
								case N_BSS:
									immed_value += pm->bss_start_address;
									break;
								default:
									fatal_error("(%s, %d): internal error", __FILE__, __LINE__);
									break;
							}
							p16[0] = immed_value>>16;
							p16[4] = immed_value;
							break;
						}
						case BIT30:
						{
							uint32_t data30b = (*p32 & 0x3fffffff)<<2;
							switch( pr->index & ~N_EXT) {
								case N_TEXT:
									data30b = (pm->text_start_address + data30b)>>2&0x3fffffff;
									*p32 &= 0xc0000000;
									*p32 |= data30b;
									break;
								case N_DATA:
									data30b = (pm->data_start_address + data30b)>>2&0x3fffffff;
									*p32 &= 0xc0000000;
									*p32 |= data30b;
									break;
								case N_BSS:
									data30b = (pm->bss_start_address + data30b)&0x3fffffff;
									*p32 &= 0xc0000000;
									*p32 |= data30b;
									break;
								default:
									fatal_error("(%s, %d): unknow internal error", __FILE__, __LINE__);
									break;
							}
							break;
						}
						case BIT32:
						{
							uint32_t data32b = *p32;
							switch( pr->index & ~N_EXT) {
								case N_TEXT:
									*p32 = pm->text_start_address + data32b;
									break;
								case N_DATA:
									*p32 = pm->data_start_address + data32b;
									break;
								case N_BSS:
									*p32 = pm->bss_start_address + data32b;
									break;
								default:
									fatal_error("(%s, %d): unknow internal error", __FILE__, __LINE__);
									break;
							}
							break;
						}
						default:
							fatal_error("(%s, %d): unknow internal error", __FILE__, __LINE__);
					}
				}
			}
		}
        pm = pm->next;
	}
    return 0;
}

int main(int argc,char *argv[])
{
	int i;
	int obj_file_start_index = 1;
	if( argc < 2 ) {
		usage(argv[0]);
		return -1;
	}
	// parser arguments
	for( i=1; i<argc; i++) {
		if(strcmp(argv[i], "-o") == 0) {
			i++;
			if( i == argc ) {
				error("no output file");
				usage(argv[0]);
				return -1;
			}
			out_file_name = argv[i];
			obj_file_start_index = i+1;
		}
        else if(strcmp(argv[i], "-data") ==0) {
			unsigned long load_address;
            i++;
            if( i == argc ) { 
                error("no address");
                return -1; 
            }
            if( str2num(&load_address, argv[i]) != 0 ) { 
                printf("set data segment start address to 0x%08x\n", (uint32_t)load_address);
            }
            else {
                error("bad .data address number");
                return -1; 
            }
			data_seg_load_address = load_address;
        }
        else if(strcmp(argv[i], "-text") ==0) {
			unsigned long load_address;
            i++;
            if( i == argc ) { 
                error("no address");
                return -1; 
            }
            if( str2num(&load_address, argv[i]) != 0 ) { 
                printf("set text segment start address to 0x%08x\n", (uint32_t)load_address);
            }
            else {
                error("bad .text address number");
                return -1; 
            }
			text_seg_load_address = load_address;
        }
		else if(strcmp(argv[i], "-M") == 0) {
			i++;
			if( i == argc ) {
				error("no map file");
				usage(argv[0]);
				return -1;
			}
			map_file_name = argv[i];
			obj_file_start_index = i+1;
		}
	}
	if( out_file_name == NULL ) {
		out_file_name = "a.out";
	}

	if( obj_file_start_index == argc ) {
		error("no input file");
		usage(argv[0]);
		return -1;
	}

    total_text_size = 0;
    total_data_size = 0;
    total_bss_size = 0;

	memset(&modules_head,0,sizeof(struct module));
    symhash_tab = hash_init();

	/* load modules */
	for( i = obj_file_start_index; i<argc; i++ ) {
		int nr;
		int fd = open(argv[i], O_RDONLY);
		uint8_t buf[SARMAG];
		const uint32_t magic = A_MAGIC1;
		struct module *tp = NULL;
		if( fd < 0 ) {
			fprintf(stderr, "%s: %s\n", argv[i], strerror(errno));
			error_cnt++;
			continue;
		}
		if( (nr = read(fd, buf, SARMAG)) != SARMAG ) {
			fprintf(stderr, "%s: %s\n", argv[i], strerror(errno));
			close(fd);
			error_cnt++;
			continue;
		}
		if( !bcmp(buf, &magic, 4) ) {
			// check a.out object file
			if( lseek(fd, 0, SEEK_SET) != 0 ) {
				fprintf(stderr, "%s: %s\n", argv[i], strerror(errno));
				close(fd);
				error_cnt++;
				continue;
			}
			if( (tp = load_module(argv[i], fd, 1)) != NULL ) {
				struct module *tp2 = &modules_head;
				while(tp2->next) {
					tp2 = tp2->next;
				}
				tp2->next = tp;
			}
			close(fd);
		}
		else if( !strncmp((const char*)buf, ARMAG, SARMAG) ) {
			// check ar file
			int archive_fd;
			struct obj_file_info list_head;
			struct obj_file_info *p, *t;
			archive_fd = get_file_list_from_archive(argv[i], &list_head);
			if( archive_fd >= 0 ) {
				int cnt = 0;
				p = list_head.link;
				while(p) {
					t = p->link;
					if( lseek(p->fd, p->offset, SEEK_SET) != p->offset) {
						fprintf(stderr, "%s: %s\n", argv[i], strerror(errno));
						break;
					}
					if( (tp = load_module(p->filename, p->fd, 0)) != NULL ) {
						struct module *tp2 = &modules_head;
						while(tp2->next) {
							tp2 = tp2->next;
						}
						tp2->next = tp;
					}
					free(p);
					p = t;
					cnt++;
				}
				close(fd);
				close(archive_fd);
			}
			close(fd);
			continue;
		}
		else {
			fprintf(stderr, "%s: %s\n", argv[i], "bad magic");
			error_cnt++;
			continue;
		}
	}

	{
		struct module *mp;
		// update load address

		//text_seg_load_address = 0;
		if( data_seg_load_address == 0 ) {
			data_seg_load_address = text_seg_load_address + total_text_size;
		}

		bss_seg_load_address  = data_seg_load_address + total_data_size;
		// text segment
		mp = modules_head.next;
		while(mp) {
			mp->text_start_address += text_seg_load_address;
			mp = mp->next;
		}
		// data segment after text segment, adjust data offsets for every module
		mp = modules_head.next;
		while(mp) {
			mp->data_start_address += data_seg_load_address;
			mp = mp->next;
		}
		// bss segment after data segment, adjust bss offsets for every module
		mp = modules_head.next;
		while(mp) {
			mp->bss_start_address += bss_seg_load_address;
			mp = mp->next;
		}
		/* dump & adjust global symbols */
		mp = modules_head.next;
		while(mp) {
			dump_module_symbols(mp);
			mp = mp->next;
		}
	}

	insert_global_sym(&sym_text_start, "__text_start", N_ABS, text_seg_load_address);
	insert_global_sym(&sym_text_size,  "__text_size",  N_ABS, total_text_size);
	insert_global_sym(&sym_data_start, "__data_start", N_ABS, data_seg_load_address);
	insert_global_sym(&sym_data_size,  "__data_size",  N_ABS, total_data_size);
	insert_global_sym(&sym_bss_start,  "__bss_start", N_ABS,  bss_seg_load_address);
	insert_global_sym(&sym_bss_size,   "__bss_size",  N_ABS,  total_bss_size);

    /* link main */
	if( error_cnt == 0 ) {
		link_main( modules_head.next );
	}

	/* write a.out file */
	if( error_cnt == 0 )
	{
		FILE *fp_out = NULL;
		struct exec header;
		memset( &header, 0, sizeof(struct exec) );
		header.a_magic = A_MAGIC1;

		/* get text size */
		header.a_text = total_text_size;
		/* get data size */
		header.a_data = total_data_size;
		/* get bss size */
		header.a_bss = total_bss_size;
		
		{
			/* open file */
			if( (fp_out = fopen(out_file_name, "wb")) == NULL ) {
				perror(out_file_name);
			}
			else {
				/* wirte header */
				fseek( fp_out, 0, SEEK_SET );
				fwrite( &header, sizeof(struct exec), 1, fp_out);
				/* write text */
				{
					struct module *pm = modules_head.next;
					uint32_t write_cnt = text_seg_load_address;
					while(pm) {
						if( write_cnt > pm->text_start_address ) {
							fatal_error("address overlap\n");
						}
						else {
							for(; write_cnt < pm->text_start_address; write_cnt++) {
								putc(0,fp_out);
							}
							fwrite(pm->text, pm->header.a_text,1,fp_out);
							write_cnt = pm->text_start_address + pm->header.a_text;
						}
						/* fill the last object file */
						if( pm->next == NULL ) {
							int n = pm->header.a_text%4;
							if(n) {
								n = 4 - n;
								while(n) {
									putc(0,fp_out);
									n--;
								}
							}
						}
						pm = pm->next;
					}
				}
				/* write data */
				{
					struct module *pm = modules_head.next;
					uint32_t write_cnt = data_seg_load_address;
					while(pm) {
						if( write_cnt > pm->data_start_address ) {
							fatal_error("address overlap\n");
						}
						else {
							for(; write_cnt < pm->data_start_address; write_cnt++) {
								putc(0,fp_out);
							}
							fwrite(pm->data, pm->header.a_data,1,fp_out);
							write_cnt = pm->data_start_address + pm->header.a_data;
						}
						/* fill the last object file */
						if( pm->next == NULL ) {
							int n = pm->header.a_data%4;
							if( n ) {
								n = 4 - n;
								while(n) {
									putc(0,fp_out);
									n--;
								}
							}
						}
						pm = pm->next;
					}
				}
				/* close file */
				fclose(fp_out);
			}
		}
	}

	/* write map file */
	if( error_cnt == 0 && map_file_name ) {
		FILE *fp_map = NULL;
		time_t now;
		struct tm *timenow = NULL;
		/* open map file */
		if( (fp_map = fopen(map_file_name, "wt")) == NULL ) {
			perror(map_file_name);
		}
		time(&now);
		timenow = localtime(&now);
		fprintf(fp_map, "\nlink time: %s\n", asctime(timenow));
		fprintf(fp_map, "text size: %d\ndata size: %d\nbss size: %d\n\n",\
			total_text_size, total_data_size, total_bss_size);
		{
			/* dump symbols */
			struct glosym *pglosym = NULL;
			int i;
			for( i=0; i<NHASH; i++ )
			{
				pglosym = symhash_tab[i];
				while( pglosym ) {
					if( pglosym->defined ) {
						fprintf(fp_map, "%08x\t", (unsigned)pglosym->value);
						fprintf(fp_map, "%c\t", n_type_2_char(pglosym->refs->n_type));
						fprintf(fp_map, "%-33s", pglosym->name );
						fprintf(fp_map, " %s\n", pglosym->filename);
					}
					pglosym = pglosym->link;
				}
			}
		}
		/* close map file */
		fclose(fp_map);
	}

	/* free modules */
	{
		struct module *tp,*tp2;
		tp = modules_head.next;
		while(tp) {
			tp2 = tp->next;
			free_module(tp);
			free(tp);
			tp = tp2;
		}
	}

    /* free global symbols in hash table */
    hash_deinit();

	if(!error_cnt) {
		fprintf(stdout, "text size: %d, data size: %d, bss size:%d\n",\
			total_text_size, total_data_size, total_bss_size);
	}
    
	return (error_cnt != 0);
}
