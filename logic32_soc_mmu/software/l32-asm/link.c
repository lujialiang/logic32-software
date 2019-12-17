#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "link.h"
#include "a_out.h"
#include "my_dbg.h"
#include "hash_ex.h"

extern int match_suffix(char *file_name, char *suffix);
int dump_module_symbols(struct module *pm);

struct module modules_head;

static uint32_t total_text_size = 0;
static uint32_t total_data_size = 0;
static uint32_t total_bss_size = 0;

static uint32_t text_seg_load_address = 0;
static uint32_t data_seg_load_address = 0;
static uint32_t bss_seg_load_address = 0;

extern int error_cnt;
void error(char*s,...);

struct module *load_module( char *filename )
{
	if( match_suffix(filename, ".o") ) {
		FILE *fp = fopen(filename, "rb");
		if( fp == NULL ) {
			perror(filename);
			return NULL;
		}
		struct module *pm = (struct module*)malloc(sizeof(struct module));
		if( pm == NULL ) {
			perror("malloc");
			fclose(fp);
			return NULL;
		}
		else {
			memset(pm,0,sizeof(struct module));
			pm->next = NULL;
			/* copy file name */
			pm->filename = (char*)malloc(strlen(filename)+1);
			strcpy(pm->filename,filename);
			/* read exec header */
			fread(&(pm->header),sizeof(struct exec),1,fp);
			/* read text segment */
			if( pm->header.a_text ) {
				pm->text = (uint8_t *)malloc(pm->header.a_text);
				fread(pm->text, pm->header.a_text, 1, fp);
			}
			/* read data segment */
			if( pm->header.a_data ) {
				pm->data = (uint8_t *)malloc(pm->header.a_data);
				fread(pm->data, pm->header.a_data, 1, fp);
			}
			/* read text relocationg info */
			if( pm->header.a_trsize ) {
				pm->textrel = (struct relocation_info*)malloc(pm->header.a_trsize);
				fread(pm->textrel,pm->header.a_trsize,1,fp);
			}
			/* read data relocationg info */
			if( pm->header.a_drsize ) {
				pm->datarel = (struct relocation_info*)malloc(pm->header.a_drsize);
				fread(pm->datarel,pm->header.a_drsize,1,fp);
			}
			/* read symbol table */
			if( pm->header.a_syms ) {
				pm->symbols = (struct nlist*)malloc(pm->header.a_syms);
				fread(pm->symbols, pm->header.a_syms, 1, fp);
			}
			/* read string table */
			pm->string_size =  pm->header.a_strs;
			if( pm->string_size ) {
				pm->strings = (char*)malloc(pm->string_size);
				fread(pm->strings,pm->string_size,1,fp);
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
			fclose(fp);
			return pm;
		}
	}
	else {
		printf("%s is't object file\n", filename);
		return NULL;
	}
}

int free_module( struct module *pm )
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

int dump_reloc(char *module_name, struct relocation_info *pr, struct nlist *psyms, int n)
{
	int i;
    if( pr == NULL ) {
        return -1;
    }
    if( psyms == NULL ) {
        return -1;
    }
	for(i=0; i<n; i++) {
		printf("\n<%s %d>\n", module_name, i);
		printf("address: %d\n", pr->address);
		printf("index  : %d\n", pr->index);
		printf("pcrel_flag: %d\n", pr->pcrel_flag);
		printf("extern_flag: %d\n",pr->extern_flag);
		if( pr->extern_flag == 1 ) {
            printf("rel symbol: %s\n", psyms[pr->index].n_name);
		}
		else {
			printf("rel: ");
			switch( psyms->n_type & ~ N_EXT ) {
				case N_UNDF:
					printf("N_UNDF\n"); break;
				case N_ABS:
					printf("N_ABS\n"); break;
				case N_TEXT:
					printf("N_TEXT\n"); break;
				case N_DATA:
					printf("N_DATA\n"); break;
				case N_BSS:
					printf("N_BSS\n"); break;
				default:
					printf("????\n"); break;
			}
		}
		pr++;
	}
	return 0;
}

int dump_syms(struct nlist *psyms, char *string_table, int n)
{
	int i;
	for(i=0; i<n; i++) {
        psyms->n_name = &string_table[psyms->n_strx];
		printf("\nsymbol <%d>\n", i);
		printf("name :%s\n", psyms->n_name);
		printf("value : %08x\n", psyms->n_value);
		printf("type: ");
		switch( psyms->n_type & ~ N_EXT ) {
			case N_UNDF:
				printf("N_UNDF\n"); break;
			case N_ABS:
				printf("N_ABS\n"); break;
			case N_TEXT:
				printf("N_TEXT\n"); break;
			case N_DATA:
				printf("N_DATA\n"); break;
			case N_BSS:
				printf("N_BSS\n"); break;
			default:
				printf("????\n"); break;
		}
		psyms++;
	}
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
        pglosym = hash_ex_in(psyms);
		pglosym->filename = pm->filename;
        if( pglosym->defined ) {
            switch( psyms->n_type & ~ N_EXT ) {
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
			tmp = hash_ex_lookup( pglosym->name );
			while(tmp) {
				if( tmp->defined ) {
					printf("#%d %s: symbol '%s'\n", i++, tmp->filename, pglosym->name);
				}
				tmp = tmp->link;
			}
			putchar(10);
		}
		psyms++;
	}
	return 0;
}

int dump_module( struct module *pm )
{
	int n;
	if( pm == NULL ) {
		return -1;
	}
	/* dump text relocation_info */
	n = (pm->header.a_trsize)/sizeof(struct relocation_info);
	dump_reloc(pm->filename, pm->textrel, pm->symbols, n);
	/* dump data relocation_info */
	n = (pm->header.a_drsize)/sizeof(struct relocation_info);
	dump_reloc(pm->filename, pm->datarel, pm->symbols, n);
	/* dump symbol table */
	//n = (pm->header.a_syms)/sizeof(struct nlist);
	//dump_syms2(pm->symbols, pm->strings, n, pm);
	return 0;
}

static char *out_file_name = NULL;
static char *map_file_name = NULL;
static struct glosym **symhash_tab = NULL;

static void usage(char *prog_name)
{
	printf("usage: %s [-o outfile] [-M mapfile] objfiles,...\n", prog_name);
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

int main(int argc,char *argv[])
{
	int i;
	int obj_file_start_index = 1;
	bool sim_flag = true;
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
			i++;
			if( i == argc ) {
				error("no address");
				return -1;
			}
			sim_flag = false;
			if( str2num(&data_seg_load_address, argv[i]) != 0 ) {
				printf("set data segment start address to 0x%08x\n", data_seg_load_address);
			}
			else {
				error("bad address number");
				return -1;
			}
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

	//unlink("./a.out");

    total_text_size = 0;
    total_data_size = 0;
    total_bss_size = 0;

	memset(&modules_head,0,sizeof(struct module));
    symhash_tab = hash_ex_init();

	/* load modules */
	for( i=obj_file_start_index; i<argc; i++ ) {
		struct module *tp;
		if( tp = load_module( argv[i] ) ) {
			struct module *tp2 = &modules_head;
			while(tp2->next) {
				tp2 = tp2->next;
			}
			tp2->next = tp;
		}
	}

	{
		struct module *mp;
		// update load address
		text_seg_load_address = 0;
		if( sim_flag ) {
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

	/* dump modules */
    if(0)
	{
		struct module *tp;
		tp = modules_head.next;
		while(tp) {
			dump_module(tp);
			tp = tp->next;
		}

	}

    /* dump global symbols */
    if(0)
    {
        printf("dump global symbols\n");
        dump_global_symbols(0);
        printf("end dump global symbols\n");
    }

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
							fatal_error(stderr,"address overlap\n");
						}
						else {
							for(; write_cnt < pm->text_start_address; write_cnt++) {
								//printf("fill 1 byte code\n");
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
								//printf("fill %d byte text for last object file\n", n);
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
							fatal_error(stderr,"address overlap\n");
						}
						else {
							for(; write_cnt < pm->data_start_address; write_cnt++) {
								//printf("fill 1 byte data\n");
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
								//printf("fill %d byte data for last object file\n", n);
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

end:
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
    hash_ex_deinit();

	if(!error_cnt) {
		printf("text size: %d, data size: %d, bss size:%d\n",\
			total_text_size, total_data_size, total_bss_size);
	}
    
	return (error_cnt != 0);
}
