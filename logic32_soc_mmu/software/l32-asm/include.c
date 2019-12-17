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

// variable definition
static struct file_info_s file_info_buffer[MAX_INCLUDES_DEPTH];
static int file_cnt = 0;

void include_init(void)
{
	file_cnt = 0;
}

int include_stack_empty(void)
{
	return (file_cnt == 0);
}

int include_push_file(char *fname, int line, FILE *fp)
{
	struct file_info_s *pfi;
	if( file_cnt >= MAX_INCLUDES_DEPTH ) {
		error("#include nested too deeply");
		exit(1);
		return -1;
	}
	pfi = &file_info_buffer[file_cnt];
	strcpy(pfi->fname, fname);
	pfi->line = line;
	pfi->fp = fp;
	file_cnt++;
	return 0;
}

struct file_info_s *include_pop_file(void)
{
	struct file_info_s *pfi;
	if( file_cnt <= 0 ) {
		error("include stack empty");
		return NULL;
	}
	file_cnt--;
	pfi = &file_info_buffer[file_cnt];
	return pfi;
}
