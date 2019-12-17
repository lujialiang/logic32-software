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
#include "include.h"

extern int const_expression(pnode_t p);

int do_text(void)
{
	cur_seg = N_TEXT;
	pcur_loc = &text_loc;
	return 0;
}

int do_data(void)
{
	cur_seg = N_DATA;
	pcur_loc = &data_loc;
	return 0;
}

int do_bss(void)
{
	cur_seg = N_BSS;
	pcur_loc = &bss_loc;
	return 0;
}

int do_equ(void)
{
	struct nlist *sp = NULL;	
	node_t node;
	if( pass == 2 ) {
		load_line();
		return 0;
	}
	next_token();
	if( token.type != sym_name ) {
		error(".equ: '%s',  expect a symbol",token_buf);
		return -1;
	}
	else {
		sp = lookup();
		if( sp == NULL ) {
			sp = hshin();
			if( sp == NULL ) {
				return -1;
			}
			sp->n_type = N_ABS;
			if( const_expression(&node) >= 0 ) {
				sp->n_value = node.value;
				return 0;
			}
			else {
				sp->n_value = -1;
				return -1;
			}
		}
		else if( ( sp->n_type & ~N_EXT ) == N_UNDF ) {
			sp->n_type = N_ABS | N_EXT;
			if( const_expression(&node) >= 0 ) {
				sp->n_value = node.value;
				return 0;
			}
			else {
				sp->n_value = -1;
				return -1;
			}
		}
		else {
			error(".equ: redefined symbol '%s'",token_buf);
			return -1;
		}
	}
}

int do_org(void)
{
	int i;
	node_t node;
	node.value = 0;
	next_token();
	if( token.type == sym_num ) {
		node.value = token.value;
	}
	else if( token.type == sym_name ) {
		struct nlist *sp;
		sp = lookup();
		if( sp == NULL ) {
			error(".org: undefined symbol '%s'",token_buf);
			return -1;
		}
		if( ( sp->n_type & ~N_EXT ) == N_ABS ) {
			node.value = sp->n_value;
		}
		else {
			error(".org: symbol '%s' not const value",token_buf);
			return -1;
		}
	}
	if( cur_seg == N_TEXT ) {
		if( node.value < *pcur_loc ) {
			error("new value (%08x) less then current location(%08x)",node.value, *pcur_loc);
			return -1;
		}
		if( pass == 2 ) {
			for( i = text_loc; i < node.value; i++ ) {
				out_byte(0);
			}
		}
		*pcur_loc = node.value;
		return 0;
	}
	else {
		if( node.value < *pcur_loc ) {
			error("new value (%08x) less then current location(%08x)",node.value, *pcur_loc);
			return -1;
		}
		if( pass == 2 ) {
			for( i = *pcur_loc; i < node.value; i++ ) {
				out_byte(0);
			}
		}
		*pcur_loc = node.value;
		return 0;
	}
}

int do_space(void)
{
	node_t node;
	if( const_expression(&node) >= 0 ) {
		*pcur_loc += node.value * BYTE_SIZE;
		if( pass == 2 ) {
			int n = node.value;
			while(n) {
				out_byte(0);
				n--;
			}
		}
		return 0;
	}
	return -1;
}

int do_comm_lcomm(void)
{
	const char *inst;
	int comm_flag = 0;
	if( token.value == sym_lcomm ) {
		inst = ".lcomm";
	}
	else if( token.value == sym_comm ) {
		inst = ".comm";
		comm_flag = 1;
	}
	else {
		error("unknow internal error");
		return -1;
	}
	if( next_token() != sym_name ) {
		error("%s: syntax error", inst);
		return -1;
	}
	if( pass == 1 ) {
		if( comm_flag ) {
			extern int add_global_symbol(char *name);
			add_global_symbol(token_buf);
		}
		struct nlist *sp = NULL;
		sp = lookup();
		if( sp == NULL ) {
			sp = hshin();
			if( sp ) {
				sp->n_type = N_BSS;
				sp->n_value = bss_loc;
			}
			else {
				error("%s: hash table overflow!", inst);
				exit(1);
				return -1;
			}
		}
		else {
			if( (sp->n_type & ~N_EXT) == N_UNDF ) {
				sp->n_type = N_BSS | (sp->n_type & N_EXT);
				sp->n_value = bss_loc;
			}
			else {
				error("%s: redefined symbol '%s'", inst, token_buf);
				return -1;
			}
		}
	}
	if( next_token() != sym_comma ) {
		error("%s: expect ','", inst);
		return -1;
	}
	node_t node;
	if( const_expression(&node) >= 0 ) {
		bss_loc += node.value * BYTE_SIZE;
		if( pass ==  2 ) {
			bss_size += node.value * BYTE_SIZE;
		}
		return 0;
	}
	return -1;
}

int do_name(void)
{
	struct nlist *sp = NULL;
	char tmp[NCPS];
	strcpy(tmp,token_buf);
	if( next_token() != sym_semi ) {
		error("do_name: syntax error");
		return -1;
	}
	if( pass == 2 ) {
		return 0;
	}
	strcpy(token_buf,tmp);
	sp = lookup();
	if( sp == NULL ) {
		sp = hshin();
		if( sp ) {
			sp->n_type = cur_seg;
			if( cur_seg == N_TEXT ) {
				sp->n_value = *pcur_loc;
			}
			else {
				sp->n_value = *pcur_loc;
			}
			return 0;
		}
		return -1;
	}
	if( (sp->n_type & ~N_EXT) == N_UNDF ) {
		sp->n_type = cur_seg | (sp->n_type & N_EXT);
		if( cur_seg == N_TEXT ) {
			sp->n_value = *pcur_loc;
		}
		else {
			sp->n_value = *pcur_loc;
		}
		return 0;
	}
	else {
		error("do_name: redefined symbol '%s'",token_buf);
		return -1;
	}
}

int do_local_lable(void)
{
	uint32_t lable_value = token.value;
	if( next_token() != sym_semi ) {
		error("do_local_lable: syntax error");
		return -1;
	}
	if( pass == 2 ) {
		return 0;
	}
	if( cur_seg == N_TEXT ) {
		if( local_symbol_cnt >= LOCAL_SYMOBOL_SIZE ) {
			error("fatal error: local symbol table overflow");
			exit(1);
			return -1;
		}
		local_symbol_table[local_symbol_cnt].lable = lable_value;
		local_symbol_table[local_symbol_cnt].value = *pcur_loc;
		local_symbol_cnt++;
	}
	else {
		error("local symbol only allowed in .text segment");
		return -1;
	}
	return 0;
}

int do_align(void)
{
	node_t node;
	if( const_expression(&node) >= 0 ) {
		switch( node.value ) {
			case 1:
			case 2:
			case 4:
			case 8:
			case 16:
			{
				if( *pcur_loc % node.value == 0 ) {
					return 0;
				}
				int n = node.value - (*pcur_loc % node.value);
				*pcur_loc += n;
				if( pass == 2 ) {
					while(n) {
						out_byte(0);
						n--;
					}
				}
				return 0;
			}
			default:
				error(".align: syntax error");
				return -1;
		}
	}
	return -1;
}

int do_include(void)
{
	next_token();
	if( token.type != sym_string ) {
		error(".include: expect a file name");
		return -1;
	}
	else {
		FILE *fp_tmp;
		fp_tmp = fopen(token_buf, "rt");
		if( fp_tmp == NULL ) {
			error("cannot open include file %s", token_buf);
			return -1;
		}
		else {
			if( include_push_file(curr_file_name, line_num, fp_in) == 0 )
			{
				fp_in = fp_tmp;
				strcpy(curr_file_name, token_buf);
				line_num = 0;
			}
			else {
				fclose(fp_tmp);
			}
		}
	}
	// next_token
	load_line();
	return 0;
}

int do_ascii(void)
{
	next_token();
	if( token.type == sym_string ) {
		if( pass == 2 ) {
			char *p = token_buf;
			int i = token.value;
			while(i--) {
				out_byte(*p++);
			}
		}
		*pcur_loc += token.value;
		next_token();
		return 0;
	}
	else {
		error(".ascii, syntax error");
		return -1;
	}
}

int do_asciz(void)
{
	next_token();
	if( token.type == sym_string ) {
		if( pass == 2 ) {
			char *p = token_buf;
			int i = token.value;
			while(i--) {
				out_byte(*p++);
			}
			out_byte(0);
		}
		*pcur_loc += token.value + 1;
		next_token();
		return 0;
	}
	else {
		error(".asciz, syntax error");
		return -1;
	}
}

#include "asm.h"

extern int add_reloc(uint32_t address, int length, struct nlist *psym);

// E => T E_
// E_ => , T E_
// E_ =>
// T => i
static int E(void);
static int E_(void);
static int T(void);

static int item_size = 4;
static int unsigned_flag = 0;

int do_byte(void)
{
	item_size = 1;
	unsigned_flag = 0;
	next_token();
	return E();
}

int do_half(void)
{
	item_size = 2;
	unsigned_flag = 0;
	next_token();
	return E();
}

int do_word(void)
{
	item_size = 4;
	unsigned_flag = 0;
	next_token();
	return E();
}

int do_ubyte(void)
{
	item_size = 1;
	unsigned_flag = 1;
	next_token();
	return E();
}

int do_uhalf(void)
{
	item_size = 2;
	unsigned_flag = 1;
	next_token();
	return E();
}

int do_uword(void)
{
	item_size = 4;
	unsigned_flag = 1;
	next_token();
	return E();
}

// E => T E'
static int E(void)
{
	if( T() ) {
		return -1;
	}
	return E_();
}

// E' => , T E'
// E' => 
static int E_(void)
{
	if( token.type  == sym_comma ) {
		next_token();
		if( T() ) {
			return -1;
		}
		if( E_() ) {
			return -1;
		}
	}
	return 0;
}

// T => i
static int T(void)
{
	int minus_flag = 0;
	if( token.type == sym_minus ) {
		minus_flag = 1;
		next_token();
	}

	if( item_size != 1 && token.type == sym_string ) {
		error("syntax error");
		return -1;
	}
	if( minus_flag == 0 && token.type == sym_string ) {
		if( pass == 2 ) {
			char *p = token_buf;
			int i = token.value;
			while(i--) {
				out_byte(*p++);
			}
		}
		*pcur_loc += token.value;
		next_token();
		return 0;
	}
	else if( token.type == sym_num ) {
		int value = (minus_flag == 0) ? token.value : -token.value;
		if( item_size == 1 ) {
			if( unsigned_flag ) {
				// .ubyte
				if( value & 0xffffff00 ) {
					error("immediate number (=%d, =0x%x) overflow, must in range 0 to 255",\
						value, value);
					return -1;
				}
			}
			else {
				// .byte
				if( value < -128 || value > 127 ) {
					error("immediate number (=%d, =0x%x) overflow, must in range -128 to 127",
						value, value);
					return -1;
				}
			}
			if( pass == 2 ) {
				out_byte(value);
			}
			*pcur_loc += BYTE_SIZE;
		}
		else if( item_size == 2 ) {
			if( unsigned_flag ) {
				// .uhalf
				if( value & 0xffff0000 ) {
					error("immediate number (=%d, =0x%x) overflow, must in range 0 to 65535",\
						value, value);
					return -1;
				}
			}
			else {
				// .half
				if( value < -32768 || value > 32767 ) {
					error("immediate number (=%d, =0x%x) overflow, must in range -32768 to 32767",
						value, value);
					return -1;
				}
			}
			if( pass == 2 ) {
				uint16_t tmp = value;
				out_byte(tmp);
				out_byte(tmp>>8);
			}
			*pcur_loc += HALF_SIZE;
		}
		else if( item_size == 4 ) {
			if( pass == 2 ) {
				out_word(value);
			}
			*pcur_loc += WORD_SIZE;
		}
		else {
			error("%s:%d internal error", __FILE__, __LINE__);
			exit(1);
			return -1;
		}
		next_token();
		return 0;
	}
	else if( minus_flag == 0 && token.type == sym_name ) {
		struct nlist *sp = NULL;
		uint32_t tmp_value = 0;
		if( pass == 2 ) {
			if( (sp = lookup()) == NULL ) {
				error("undefined symbol '%s'",token_buf);
				return -1;
			}
		}
		if( next_token() == sym_plus ) {
			node_t node;
			if( const_expression(&node) < 0 ) {
				return -1;
			}
			tmp_value = node.value;
		}
		if( pass == 1 ) {
			if( item_size == 1 ) {
				*pcur_loc += BYTE_SIZE;
			}
			else if( item_size == 2 ) {
				*pcur_loc += HALF_SIZE;
			}
			else if( item_size == 4 ) {
				*pcur_loc += WORD_SIZE;
			}
			else {
				error("%s:%d internal error", __FILE__, __LINE__);
				exit(1);
				return -1;
			}
			return 0;
		}
		else if( pass == 2 ) {
			if( ( sp->n_type & ~N_EXT ) == N_ABS ) {
				int tmp = sp->n_value;
				tmp += tmp_value;	// LJL 20171031
				if( item_size == 1 ) {
					if( unsigned_flag ) {
						// .ubyte
						if( tmp & 0xffffff00 ) {
							error("symbol %s's value (=%d, =0x%x), must in range 0 to 255",\
								token.name, tmp, tmp);
							return -1;
						}
					}
					else {
						// .byte
						if( tmp < -128 || tmp > 127 ) {
							error("immediate number (=%d, =0x%x) overflow, must in range -128 to 127",
								tmp, tmp);
							return -1;
						}
					}
					//out_byte(sp->n_value);
					out_byte(tmp);
					*pcur_loc += BYTE_SIZE;
					return 0;
				}
				else if( item_size == 2 ) {
					if( unsigned_flag ) {
						// .uhalf
						if( tmp & 0xffff0000 ) {
							error("symbol %s's value (=%d, =0x%x), must in range 0 to 65535",\
								token.name, tmp, tmp);
							return -1;
						}
					}
					else {
						// .half
						if( tmp < -32768 || tmp > 32767 ) {
							error("symbol %s's value (=%d, =0x%x), must in range -32768 to 32767",\
								token.name, tmp, tmp);
							return -1;
						}
					}
					out_byte(tmp);
					out_byte(tmp>>8);
					*pcur_loc += HALF_SIZE;
					return 0;
				}
				else if( item_size == 4 ) {
					//out_word(sp->n_value);
					out_word(tmp);
					*pcur_loc += WORD_SIZE;
					return 0;
				}
				error("%s:%d internal error", __FILE__, __LINE__);
				exit(1);
				return -1;
			}
			else {
				if( item_size == 1 ) {
					error(".byte/.ubyte '%s', unsupport external symbol reference", token.name);
					return -1;
				}
				else if( item_size == 2 ) {
					error(".half/.uhalf '%s', unsupport external symbol reference", token.name);
					return -1;
				}
				else if( item_size == 4 ) {
					int tmp = sp->n_value;
					tmp += tmp_value;	// LJL 20171031
					add_reloc(*pcur_loc,BIT32,sp);
					//out_word(sp->n_value);
					out_word(tmp);
					*pcur_loc += WORD_SIZE;
					return 0;
				}
				error("%s:%d internal error", __FILE__, __LINE__);
				exit(1);
				return -1;
			}
		}
		error("%s:%d internal error", __FILE__, __LINE__);
		exit(1);
		return -1;
	}
	error("sytax error, expect a number or a indentify");
	return -1;
}

