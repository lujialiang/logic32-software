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

int mapch(char ac)
{
	char a;
	int n,c;
loop:
	if( peekc >= 0 ) {
		a = peekc;
		peekc = -1;
	}
	else {
		a = get_char();
	}
	if( ac == a ) {
		return -1;
	}
	switch(a) {
		case '\n':
		case '\0':
			error("Nonterminated string");
			peekc = a;
			return -1;
		case '\\':
			switch( a = get_char() ) {
				case 't':
					return '\t';
				case 'n':
					return '\n';
				case 'b':
					return '\b';
				case '0':	case '1':	case '2':	case '3':
				case '4':	case '5':	case '6':	case '7':
					n = 0;
					c = 0;
					while( ++c <=3 && a >= '0' && a <= '7' ) {
						n <<= 3;
						n += a - '0';
						a = get_char();
					}
					peekc = a;
					return n;
				case 'r':
					return '\r';
				case '\n':
					line_num++;
					a = get_char();
					goto loop;
			}
	}
	return a;
}


// sub_str('\'')
// sub_str('"')
int sub_str(char sel)
{
	char c;
	char *p = token_buf;
	int cnt = 0;
	while(1) {
		if( peekc >= 0 ) {
			c = peekc;
			peekc = -1;
		}
		else {
			c = get_char();
		}
		if( c == EOF ) {
			p[cnt] = 0;
			return -1;
		}
		if( c == sel ) {
			if( cnt && p[cnt-1] != '\\' ) {
				p[cnt] = 0;
				return 0;
			}
			p[cnt++] = c;
		}
		p[cnt++] = c;
	}
}

void lex(void)
{
	char c;
	char *p = token_buf;
	int cnt = 0;
	while(1) {
		if( peekc >= 0 ) {
			c = peekc;
			peekc = -1;
		}
		else {
			c = get_char();
		}
		switch(c) {
			case ' ':
			case '\t':
			case '\r':
			{
				if( cnt == 0 ) {
					continue;
				}
				p[cnt] = 0;
				return;
			}
			case '!':
			case '"':
			case '#':
			//case '$':
			case '%':
			case '&':
			case '\'':
			case '(':
			case ')':
			case '*':
			case '+':
			case ',':
			case '-':
			//case '.':
			//case '/':
			case ':':
			case ';':
			case '<':
			case '=':
			case '>':
			case '?':
			case '@':
			case '[':
			case '\\':
			case ']':
			case '^':
			case '`':
			case '{':
			case '|':
			case '}':
			case '\n':
			case EOF:
			{
				if( cnt == 0 ) {
					p[0] = c;
					p[1] = 0;
					return;
				}
				peekc = c;
				p[cnt] = 0;
				return;
			}
			case '/':
			{
				if( cnt == 0 ) {
					c = get_char();
					if( c == '*' ) {
						int pre_c = get_char();
						while(pre_c != EOF) {
							c = get_char();
							if( pre_c == '*' && c == '/' ) {
								break;
							}
							pre_c = c;
						}
						if( pre_c == EOF ) {
							error("incomplete multiline comment");
							p[0] = EOF;
							p[1] = 0;
							return;
						}
						continue;
					}
					//	LJL 20170523
					else if( c == '/' ) {
						p[0] = ';';	/* link a ';' line comment */
						p[1] = 0;
						return;
					}
					//
					else {
						peekc = c;
						p[0] = '/';
						p[1] = 0;
						return;
					}
				}
				else {
					peekc = c;
					p[cnt] = 0;
					return;
				}
			}
			default:
				p[cnt++] = c;
		}
	}
}

