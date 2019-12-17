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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "a_out.h"
#include "_ar.h"

int error_cnt = 0;

void error(char*s,...)
{
	va_list ap;
	va_start(ap,s);
	fprintf(stderr,"error: ");
	vfprintf(stderr,s,ap);
	fprintf(stderr,"\n");
	va_end(ap);
	error_cnt++;
}

void fatal_error(char*s,...)
{
	va_list ap;
	va_start(ap,s);
	fprintf(stderr,"fatal error: ");
	vfprintf(stderr,s,ap);
	fprintf(stderr,"\n");
	va_end(ap);
	error_cnt++;
	exit(1);
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

#if 0
size_t get_file_size(const char *filename)
{
	struct stat buf;
	if( stat(filename,&buf)<0 ) {
		return 0;
	}
	return buf.st_size;
}

int is_obj_file(char *file)
{
	struct stat buf;
	struct exec exec_hdr;
	int nr;
	int fd;
	size_t size;
	// open file, read only
	fd = open(file, O_RDONLY);
	if( fd < 0 ) {
		fprintf(stderr, "%s: %s\n", file, strerror(errno));
		return 0;
	}
	// read exec header
	if( (nr = read(fd, &exec_hdr, sizeof(struct exec))) != sizeof(struct exec) ) {
		fprintf(stderr, "%s: %s\n", file, strerror(errno));
		close(fd);
		return 0;
	}
	// check magic
	if( exec_hdr.a_magic != A_MAGIC1 ) {
		fprintf(stderr, "%s: %s\n", file, "bad format");
		close(fd);
		return 0;
	}
	// get file size
	if( fstat(fd, &buf) < 0 ) {
		fprintf(stderr, "%s: %s\n", file, strerror(errno));
		close(fd);
		return 0;
	}
	// check size
	size = sizeof(struct exec);
	size += exec_hdr.a_text;
	size += exec_hdr.a_data;
	size += exec_hdr.a_syms;
	size += exec_hdr.a_trsize;
	size += exec_hdr.a_drsize;
	size += exec_hdr.a_strs;
	if( size != buf.st_size ) {
		fprintf(stderr, "%s: %s\n", file, "bad format");
		close(fd);
		return 0;
	}
	close(fd);
	return 1;
}

int is_lib_file(char *file)
{
	int fd;
	int nr;
	uint8_t buf[SARMAG];
	// open file, read only
	fd = open(file, O_RDONLY);
	if( fd < 0 ) {
		fprintf(stderr, "%s: %s\n", file, strerror(errno));
		return 0;
	}
	// check ar magic
	if( (nr = read(fd, buf, SARMAG)) != SARMAG ) {
		fprintf(stderr, "%s: %s\n", file, strerror(errno));
		close(fd);
		return 0;
	}
	if( strcmp((const char*)buf, ARMAG) ) {
		fprintf(stderr, "%s: %s\n", file, "bad magic");
		close(fd);
		return 0;
	}
	return 1;
}
#endif
