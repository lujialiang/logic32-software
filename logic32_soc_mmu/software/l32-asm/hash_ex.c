#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash_ex.h"
#include "my_dbg.h"

static struct glosym *symhash[NHASH];

struct glosym **hash_ex_init(void)
{
	memset(&symhash,0,sizeof(symhash));
	return symhash;
}

struct glosym *hash_ex_in(struct nlist *psym)
{
    // new global symbol
    struct glosym *ptmp = NULL;
    ptmp = (struct glosym*)malloc(sizeof(struct glosym));
    if( ptmp == NULL ) {
        perror("maloc");
        return NULL;
    }
    memset(ptmp, 0, sizeof(struct glosym));
    ptmp->link = NULL;
    ptmp->name = psym->n_name;
    ptmp->value = psym->n_value;
    ptmp->refs = psym;
    ptmp->max_common_size = 0;
    if( (psym->n_type & ~N_EXT) != N_UNDF ) {
        ptmp->defined = 1;
    }
    else {
        ptmp->defined = 0;
    }
    ptmp->referenced = 0;
    ptmp->multiply_defined = 0;
    // link new global symbol to hash table
    {
        struct glosym *pglosym = NULL;
        int ihash;
        int i, cnt;
        char *np;
        ihash = 0;
        for( np=psym->n_name; *np; ) {
            ihash += *np++;
        }
        i = ihash%NHASH;
        cnt = 0;
        while( cnt < NHASH ) {
            if( i >= NHASH ) {
                i = 0;
            }
            pglosym = symhash[ i%NHASH ];
            if( pglosym ) {
                if( strcmp( pglosym->name, psym->n_name ) == 0 ) {
                    int multiply_defined = 0;
                    if( ptmp->defined && pglosym->defined ) {
                        multiply_defined = 1;
                    }
                    while(pglosym->link) {
                        if( ptmp->defined && pglosym->link->defined ) {
                            multiply_defined++;
                        }
                        pglosym = pglosym->link;
                    }
                    ptmp->multiply_defined = multiply_defined;
                    pglosym->link = ptmp;
                    return ptmp;
                }
                else {
                    i++;
                    cnt++;
                    continue;
                }
            }
            else {
                symhash[ i%NHASH ] = ptmp;
                return ptmp;
            }
        }
        free( ptmp );
        fprintf(stderr,"hash table overflow\n");
        return NULL;
    }
}

struct glosym *hash_ex_lookup(char *name)
{	
    int ihash;
    int i, cnt;
	struct glosym *pglosym = NULL;
	char *np;
	ihash = 0;
	for( np=name; *np; ) {
		ihash += *np++;
	}
    // lookup in hash table
    i = ihash%NHASH;
    cnt = 0;
    while( cnt < NHASH ) {
        if( i >= NHASH ) {
            i = 0;
        }
        pglosym = symhash[ i%NHASH ];
        if( pglosym ) {
            if( strcmp( pglosym->name, name ) == 0 ) {
                return pglosym;
            }
            else {
                i++;
                cnt++;
            }
        }
        else {
            i++;
            cnt++;
        }
    }
	return NULL;
}

void dump_global_symbols(int all)
{
	struct glosym *pglosym = NULL;
    int i;
    for( i=0; i<NHASH; i++ )
    {
        pglosym = symhash[i];
        while( pglosym ) {
			if( all == 1 || pglosym->defined ) {
				printf("\n");
				printf("file: %s\n", pglosym->filename);
				printf("name: '%s'\n", pglosym->name );
				printf("valume: %08x\n", (unsigned)pglosym->value);
				printf("refs: %08x\n", (unsigned)pglosym->refs);
				printf("defined: %d\n", pglosym->defined);
				printf("multiply_defined: %d\n", pglosym->multiply_defined);
			}
            pglosym = pglosym->link;
        }
    }
}

void hash_ex_deinit(void)
{
	struct glosym *pglosym = NULL;
    struct glosym *ptmp = NULL;
    int i;
    for( i=0; i<NHASH; i++ )
    {
        pglosym = symhash[i];
        while( pglosym ) {
            ptmp = pglosym;
            pglosym = pglosym->link;
            free(ptmp);
        }
    }
}

