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

#include <string.h>

int is_name(void)
{
	extern char token_buf[];
	char *p = token_buf;
	if( (*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p == '_') ) {
		p++;
		for(;*p;p++) {
			if( *p >= 'a' && *p <= 'z') {
				continue;
			}
			else if( *p >= 'A' && *p <= 'Z') {
				continue;
			}
			else if( *p >= '0' && *p <= '9') {
				continue;
			}
			else if( *p == '_' ) {
				continue;
			}
			else if( *p == '.' ) {
				continue;
			}
			return 0;
		}
		return 1;
	}
	return 0;
}
int is_num(char *s)
{
	if(s[0] == '0') {
		if(s[1] == 'x' || s[1] == 'X') {
			s += 2;
		   if(*s == '\0') {
				return 0;
		   }
		   while(1) {
				if((*s >= '0' && *s <= '9') || ( *s >= 'a' && *s <= 'f')  || (*s >= 'A' && *s <= 'F' )) {
					s++;
				}
				else if(*s == '\0') {
					return 1;
				}
				else {
					return 0;
				}
		   }
		}
		else {
			s++;
			while(1) {
				if(*s >= '0' && *s <= '7') {
					s++;
				}
				else if(*s == '\0') {
					return 1;
				}
				else {
					return 0;
				}
			}
		}

	}
	else {
		while(1) {
			if(*s >= '0' && *s <= '9') {
				s++;
			}
			else if(*s == '\0') {
				return 1;
			}
			else {
				return 0;
			}
		}
	}
}

int str2num(unsigned long *p, char *s)
{
	unsigned long v;
	v = 0;
	if(s[0] == '0') {
		if(s[1] == 'x' || s[1] == 'X') {
			s += 2;
		   if(*s == '\0') {
				return 0;
		   }
		   while(1) {
				if( (*s >= '0' && *s <= '9') ) {
					v = v*16 + (*s++ - '0');
				} else if( *s >= 'a' && *s <= 'f') {
					v = v*16 + (*s++ - 'a') + 10;
				}
				else if(*s >= 'A' && *s <= 'F' ) {
					v = v*16 + (*s++ - 'A') + 10;
				}
				else if( *s == '\0' ) {
					*p = v;
					return 1;
				}
				else {
					return 0;
				}
		   }
		}
		else {
			s++;
			while(1) {
				if(*s >= '0' && *s <= '7') {
					v = v*8 + (*s++ - '0');
				}
				else if(*s == '\0') {
					*p = v;
					return 1;
				}
				else {
					return 0;
				}
			}
		}

	}
	else {
		while(1) {
			if(*s >= '0' && *s <= '9') {
				v = v*10 + (*s++ - '0');
			}
			else if(*s == '\0') {
				*p = v;
				return 1;
			}
			else {
				return 0;
			}
		}
	}
}
