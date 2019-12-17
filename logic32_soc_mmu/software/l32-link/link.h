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

#ifndef _LINK_H_INCLUDE_
#define _LINK_H_INCLUDE_

#include "a_out.h"

struct module
{
	struct module *next;
	char *filename;
	char *local_sym_name;
	struct exec header;
	uint8_t *text;
	uint8_t *data;
	//int symseg_offset;
	struct nlist *symbols;
	int string_size;
	char *strings;
	struct relocation_info *textrel;
	struct relocation_info *datarel;
	int text_start_address;
	int data_start_address;
	int bss_start_address;
	//int local_syms_offset;
};

struct glosym
{
	struct glosym *link;
	char *name;
	#if 0
	long value;
	#else
	uint32_t value;
	#endif
	struct nlist *refs;
	//int max_common_size;
	char defined;
	//char referenced;
	unsigned char multiply_defined;
	char *filename;
};

#define MAXNAMLEN 63

struct obj_file_info {
	char filename[MAXNAMLEN+1];
	int fd;
	size_t size;
	off_t offset;
	struct obj_file_info *link;
};

int get_file_list_from_archive(char *archive, struct obj_file_info *file_list_head);
int extract_obj_files(char *archive, struct obj_file_info *file_list_head);

#endif
