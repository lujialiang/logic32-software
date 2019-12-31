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
#include "keyword_hash.h"

int32_t cur_seg = N_TEXT;
uint32_t *pcur_loc = NULL;
uint32_t text_loc = 0;
uint32_t data_loc = 0;
uint32_t bss_loc  = 0;

char token_buf[NCPS];
int32_t peekc = -1;
int32_t read_ptr = 0;

int32_t pass = 1;

char line_buffer[LINE_BUFFER_SIZE];
int32_t remain_char = 0;
int32_t total_char = 0;
int32_t char_index = 0;

int inst_size = 4;
int (*do_opcode)(void);
token_t token;

int32_t error_cnt = 0;

void error(char*s,...)
{
	int32_t i;
	va_list ap;
	va_start(ap,s);
	fprintf(stderr,"error: (%s:%d): ",curr_file_name, line_num);
	vfprintf(stderr,s,ap);
	fprintf(stderr,"\n");
	va_end(ap);
	// --
	line_buffer[total_char-1] = 0;
	fprintf(stderr,"%s\n",line_buffer);	// ++
	for(i=0;i<char_index;i++) {
		fprintf(stderr," ");
	}
	fprintf(stderr,"^\n");
	// --
	load_line();
	error_cnt++;
}

void warning(char*s,...)
{
	int32_t i;
	va_list ap;
	va_start(ap,s);
	fprintf(stderr,"warning(%d):",line_num);
	vfprintf(stderr,s,ap);
	fprintf(stderr,"\n");
	va_end(ap);
	// --
	line_buffer[total_char-1] = 0;
	fprintf(stderr,"%s\n",line_buffer);	// ++
	for(i=0;i<char_index;i++) {
		fprintf(stderr," ");
	}
	fprintf(stderr,"^\n");
	// --
}

int32_t load_line(void)
{
	char_index = 0;
	int32_t i;
	line_num++;
	peekc = -1;
	for(i=0;i<LINE_BUFFER_SIZE;i++) {
		line_buffer[i] = getc(fp_in);
		if(line_buffer[i] == '\n'|| line_buffer[i] == EOF ) {
			remain_char = i+1;
			total_char = remain_char;
			line_buffer[i+1] = 0;
			return 1;
		}
	}
	remain_char = 0;
	total_char = remain_char;
	error("line too long");
	return 0;
}

int32_t get_char(void)
{
	if( remain_char == 0 ) {
		load_line();
	}
	remain_char--;
	return line_buffer[char_index++];
}

int32_t match(char *s)
{
	char *t = token_buf;
	while(*s == *t ) {
		if(*s == '\0') {
			return 1;
		}
		s++;
		t++;
	}
	return 0;
}

int32_t next_token(void)
{
	int loop = 1;
	while(loop) {
		lex();
		switch(token_buf[0]) {
			case '\n':
				token.type = sym_newline;
				return sym_newline;
			case ',':
				token.type = sym_comma;
				return sym_comma;
			case ':':
				token.type = sym_semi;
				return sym_semi;
			case ';':
				load_line();
				return sym_newline;
			case '(':
				token.type = sym_lbrack;
				return sym_lbrack;
			case ')':
				token.type = sym_rbrack;
				return sym_rbrack;
			case '[':
				break;
			case ']':
				break;
			case '+':
				token.type = sym_plus;
				return sym_plus;
			case '-':
				token.type = sym_minus;
				return sym_minus;
			case '*':
				token.type = sym_mul;
				return sym_mul;
			case '/':
				token.type = sym_div;
				return sym_div;
			case '%':
				token.type = sym_mod;
				return sym_mod;
			case '\'':
				{
					int32_t c;
					int32_t n = 0;
					while( (c = mapch('\'')) >= 0 ) {
						token_buf[n++] = c;
					}
					if( n > 1 ) {
						error("Long character constant");
					}
					token.value = token_buf[0];
					token.type = sym_num;
					return sym_num;
				}
			case '"':
				{
					int32_t c;
					int32_t n = 0;
					while( (c = mapch('\"')) >= 0 ) {
						token_buf[n++] = c;
					}
					token_buf[n] = 0;
					token.value = n;
					token.type = sym_string;
					return sym_string;
				}
			case EOF:
				{
					if( include_stack_empty() ) {
						token.type = sym_eof;
						return sym_eof;
					}
					else {
						extern char curr_file_name[];
						struct file_info_s *pinfo;
						pinfo = include_pop_file();
						fp_in = pinfo->fp;
						line_num = pinfo->line;
						strcpy(curr_file_name, pinfo->fname);
						load_line();
					}
					break;
				}
			default:
				loop = 0;
		}
	}
	const keyword_hash_t *p = keyword_hash_lookup(token_buf);
	if( p ) {
		token.type = p->type;
		token.value = p->value;
		do_opcode = p->pfunc;
		return token.type;
	}
	if( is_name() ) {
		token.type = sym_name;
		strcpy(token.name,token_buf);
		return sym_name;
	}
	if( is_num(token_buf) ) {
		unsigned long tmp;
		token.type = sym_num;
		str2num(&tmp,token_buf);
		token.value = tmp;
		return sym_num;
	}
	{
		int l = strlen(token_buf);
		if( l >= 2 ) {
			unsigned long tmp;
			int i;
			l--;
			for(i=0;i<l;i++) {
				if(token_buf[i] < '0' && token_buf[i] > '9' ) {
					token.type = sym_unknow;
					return sym_unknow;
				}
			}
			if( token_buf[i] == 'b' || token_buf[i] == 'B' ) {
				token.type = sym_local_symbol;
				token.name[0] = 'b';
				token.name[1] = 0;
				token_buf[i] = 0;
				str2num(&tmp,token_buf);
				token.value = tmp;
				return sym_local_symbol;
			}
			if( token_buf[i] == 'f' || token_buf[i] == 'F' ) {
				token.type = sym_local_symbol;
				token.name[0] = 'f';
				token.name[1] = 0;
				token_buf[i] = 0;
				str2num(&tmp,token_buf);
				token.value = tmp;
				return sym_local_symbol;
			}

		}
	}
	token.type = sym_unknow;
	return sym_unknow;
}

int32_t parser(void)
{
	sym_type_t sym_type;
	if( pass != 1 && pass != 2 ) {
		error("pass error!!!");
		return -1;
	}
	// init
	rewind(fp_in);
	error_cnt = 0;
	peekc = -1;
	line_num = 0;
	cur_seg = N_TEXT;
	pcur_loc = &text_loc;
	text_loc = 0;
	data_loc = 0;
	bss_loc  = 0;
	while( (sym_type = next_token()) != sym_eof ) {
		switch(sym_type) {
			case sym_pseudo_op:
				do_opcode();
				break;
			case sym_opcode:
				inst_size = 4;
				do_opcode();
				*pcur_loc += inst_size;
				break;
			case sym_name:
				do_name();
				break;
			case sym_num:
				do_local_lable();
				break;
			case sym_end:
				return 0;
			case sym_newline:
				break;
			default:
				error("Line %d, %d '%s'", line_num, (int32_t)sym_type, token_buf);
		}
	}
	return 0;
}
