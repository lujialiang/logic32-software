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

struct nlist symbuf[HSHSIZ];
static int32_t hshused = 0;

void hsh_init(void)
{
	int i;
	hshused = 0;
	for( i=0; i<HSHSIZ; i++ ) {
		memset(&symbuf[i],0,sizeof(struct nlist));
	}
}

struct nlist *hshin(void)
{
	int ihash;
	struct nlist *rp;
	char *sp, *np;
	ihash = 0;
	for( sp=token_buf; *sp; ) {
		ihash += *sp++;
	}
	rp = &symbuf[ihash%HSHSIZ];
	while( (np = rp->n_name)) {
		if( strcmp(token_buf, np) == 0 ) {
			return (rp);
		}
		if( ++rp >= &symbuf[HSHSIZ] ) {
			rp = symbuf;
		}
	}
	if( ++hshused >= HSHSIZ) {
		error("symbol table overflow");
		exit(1);
	}
	rp->n_type = 0;
	rp->n_value = -1;
	rp->n_name = strtab_in(token_buf);
	return (rp);
}

struct nlist *lookup(void)
{
	int ihash;
	struct nlist *rp;
	char *sp, *np;
	ihash = 0;
	for( sp=token_buf; *sp; ) {
		ihash += *sp++;
	}
	rp = &symbuf[ihash%HSHSIZ];
	while((np = rp->n_name)) {
		if( strcmp(token_buf, np) == 0 ) {
			return (rp);
		}
		if( ++rp >= &symbuf[HSHSIZ] ) {
			rp = symbuf;
		}
	}
	return NULL;
}
