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

// E  -> T E'
// E' -> + T E'
// E' ->

// T  -> F T'
// T' -> * F T'
// T' -> 

// F  -> i

static int E(pnode_t p);
static int E_(pnode_t p);
static int T(pnode_t p);
static int T_(pnode_t p);
static int F(pnode_t p);

// E  -> T E'
static int E(pnode_t p)
{
	if( T(p) ) {
		return -1;
	}
	return E_(p);
}

// E' -> + T E'
// E' -> - T E'
// E' ->
static int E_(pnode_t p)
{
	node_t node;
	int ret;
	while(1) {
		if( token.type == sym_plus ) {
			next_token();
			ret = T(&node);
			p->value += node.value;
			if( ret ) {
				return ret;
			}
		}
		else if( token.type == sym_minus ) {
			next_token();
			ret = T(&node);
			p->value -= node.value;
			if( ret ) {
				return ret;
			}
		}
		else {
			return 0;
		}
	}
}
		
// T  -> F T'
static int T(pnode_t p)
{
	if( F(p) ) {
		return -1;
	}
	return T_(p);
}

// T' -> * F
// T' -> / F
// T' -> % F
// T' -> 
static int T_(pnode_t p)
{
	node_t node;
	int ret;
	while(1) {
		if( token.type == sym_mul ) {
			next_token();
			ret = F(&node);
			p->value *= node.value;
			if( ret ) {
				return ret;
			}
		}
		else if( token.type == sym_div ) {
			next_token();
			ret = F(&node);
			p->value /= node.value;
			if( ret ) {
				return ret;
			}
		}
		else if( token.type == sym_mod ) {
			next_token();
			ret = F(&node);
			p->value %= node.value;
			if( ret ) {
				return ret;
			}
		}
		else {
			return 0;
		}
	}
}
		
// F  -> i
static int F(pnode_t p)
{
	int minus_flag = 0;
	p->value = 0;
	#if 0
	if( token.type == sym_minus ) {
		minus_flag = 1;
		next_token();
	}
	else if( token.type == sym_plus ) {
		next_token();
	}
	#endif
	if( token.type == sym_num ) {
		#if 0
		if( minus_flag ) {
			p->value = -token.value;
		}
		else {
			p->value = token.value;
		}
		#endif
		p->value = token.value;
		next_token();
		return 0;
	}
	else if( token.type == sym_name ) {
	#if 0
		if( pass == 2 ) {
			struct nlist *hp;
			hp = lookup();
			if( hp ) {
				if( hp->n_type & N_EXT ) {
					if( (hp->n_type & ~N_EXT) == N_UNDF ) {
						error("undefined symbol '%s'",hp->n_name);
						return -1;
					}
					/*
					else if( (hp->n_type & ~N_EXT) != N_ABS ) {
						error("unsolved symbol '%s'",hp->n_name);
						return -1;
					}
					*/
				}
				if( minus_flag ) {
					p->value = -hp->n_value;
				}
				else {
					p->value = hp->n_value;
				}
				next_token();
				return 0;
			}
			else {
				error("undefined symbol '%s'",symbuf);
				return -1;
			}
		}
		else {
			p->value = 0;
		}
		next_token();
		return 0;
	#else
		struct nlist *hp;
		hp = lookup();
		if( hp ) {
			if( (hp->n_type & ~N_EXT) == N_UNDF ) {
				error("unsolved symbol '%s'",hp->n_name);
				return -1;
			}
			if( (hp->n_type & ~N_EXT) != N_ABS ) {
				error("symbol '%s' not a absolute symbol",hp->n_name);
				return -1;
			}
			if( minus_flag ) {
				p->value = -hp->n_value;
			}
			else {
				p->value = hp->n_value;
			}
			next_token();
			return 0;
		}
		else {
			error("undefined symbol '%s'",symbuf);
			return -1;
		}
	#endif
	}
	else
	{
		error("syntax error");
		return -1;
	}
}

int const_expression(pnode_t p)
{
	int ret;
	next_token();
	ret = E(p);
	//printf("const_expression: ret = %d, value = %d\n", ret, p->value);
	return ret;
}
