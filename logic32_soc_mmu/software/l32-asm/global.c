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

int add_global_symbol(char *name);

static int global_list(void);
static int global_list2(void);

// global_list: idt global_list2
static int global_list(void)
{
	if( token.type == sym_name ) {
		add_global_symbol(token_buf);
		next_token();
		return global_list2();
	}
	else
	{
		error("global_list():Syntax error");
		return -1;
	}
}

// global_list2: , global_list
// global_list2: nill
static int global_list2(void)
{
	if( token.type == sym_comma ) {
		next_token();
		return global_list();
	}
	return 0;
}

int add_global_symbol(char *name)
{
	struct nlist *hp = NULL;
	hp = lookup();
	if( hp == NULL ) {
		hp = hshin();
		if( hp ) {
			hp->n_type = ( N_UNDF | N_EXT );
			hp->n_value = 0;
		}
	}
	else {
		hp->n_type |= N_EXT;
	}
	return 0;
}

int do_global(void)
{
	next_token();
	return global_list();
}
