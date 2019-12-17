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

#include "keyword_hash.h"
#include "asm.h"

#define KEYWORD_HASH_SIZE 977

static unsigned int keyword_hash_used = 0;
static keyword_hash_t keyword_hash_buffer[KEYWORD_HASH_SIZE];

void keyword_hash_init(void)
{
	int i;
	keyword_hash_used = 0;
	for( i=0; i<KEYWORD_HASH_SIZE; i++ ) {
		memset(&keyword_hash_buffer[i],0,sizeof(keyword_hash_t));
	}
}

keyword_hash_t *keyword_hash_in(const char *name, int type, int value, int (*pfunc)(void))
{
	int ihash;
	keyword_hash_t *rp;
	const char *sp, *np;
	ihash = 0;
	for( sp=name; *sp; ) {
		ihash += *sp++;
	}
	rp = &keyword_hash_buffer[ihash%KEYWORD_HASH_SIZE];
	while( (np = rp->name)) {
		if( strcmp(name, np) == 0 ) {
			return (rp);
		}
		if( ++rp >= &keyword_hash_buffer[KEYWORD_HASH_SIZE] ) {
			rp = keyword_hash_buffer;
		}
	}
	if( ++keyword_hash_used >= KEYWORD_HASH_SIZE) {
		error("symbol table overflow");
		exit(1);
	}
	rp->name = name;
	rp->type = type;
	rp->value = value;
	rp->pfunc = pfunc;
	return (rp);
}

keyword_hash_t *keyword_hash_lookup(const char *name)
{
	int ihash;
	keyword_hash_t *rp;
	const char *sp, *np;
	ihash = 0;
	for( sp=name; *sp; ) {
		ihash += *sp++;
	}
	rp = &keyword_hash_buffer[ihash%KEYWORD_HASH_SIZE];
	while((np = rp->name)) {
		if( strcmp(name, np) == 0 ) {
			return (rp);
		}
		if( ++rp >= &keyword_hash_buffer[KEYWORD_HASH_SIZE] ) {
			rp = keyword_hash_buffer;
		}
	}
	return NULL;
}
