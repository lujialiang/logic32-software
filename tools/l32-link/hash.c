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
#include <errno.h>

#include "link.h"
#include "hash.h"

static struct glosym *symhash[NHASH];

struct glosym **hash_init(void)
{
	memset(&symhash,0,sizeof(symhash));
	return symhash;
}

struct glosym *hash_in(struct nlist *psym, int defined)
{
    // new global symbol
    struct glosym *ptmp = NULL;
    ptmp = (struct glosym*)malloc(sizeof(struct glosym));
    if( ptmp == NULL ) {
		fprintf(stderr, "%s: %s\n", "malloc()", strerror(errno));
        return NULL;
    }
    memset(ptmp, 0, sizeof(struct glosym));
    ptmp->link = NULL;
    ptmp->name = psym->n_name;
    ptmp->value = psym->n_value;
    ptmp->refs = psym;
    //ptmp->max_common_size = 0;
	ptmp->defined = defined;
    //ptmp->referenced = 0;
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

struct glosym *hash_lookup(char *name)
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

void hash_deinit(void)
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
