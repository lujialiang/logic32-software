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

#ifndef _A_OUT_H_INCLUDE
#define _A_OUT_H_INCLUDE

#include <stdint.h>

struct	exec {	/* a.out header */
	uint32_t	a_magic;	/* magic number */
	uint32_t	a_text;		/* size of text segment */
	uint32_t	a_data;		/* size of initialized data */
	uint32_t	a_bss;		/* size of unitialized data */
	uint32_t	a_syms;		/* size of symbol table */
	uint32_t	a_entry;	/* entry point */
	uint32_t	a_trsize;
	uint32_t	a_drsize;
	uint32_t	a_strs;
};

#define	A_MAGIC1	0407		/* normal */
#define	A_MAGIC2	0410		/* read-only text */
#define	A_MAGIC3	0411		/* separated I&D */
#define	A_MAGIC4	0405		/* overlay */

struct  relocation_info {
	uint32_t	address;
	uint32_t	index :24,
	pcrel_flag	:1,
	length		:3,
	extern_flag	:1,
	space		:3;
};

struct	nlist {	/* symbol table entry */
	union {
		char *n_name;
		struct nlist *n_next;
		uint32_t n_strx;
	};
	uint8_t		n_type;		/* type flag */
	int8_t		n_other;
	int16_t		n_desc;
	uint32_t	n_value;	/* value */
};

/* values for type flag */
#define	N_UNDF	0x00	/* undefined */
#define	N_ABS	0x02	/* absolute */
#define	N_TEXT	0x04	/* text symbol */
#define	N_DATA	0x06	/* data symbol */
#define	N_BSS	0x08	/* bss symbol */

#define	N_EXT	0x01	/* external bit, or'ed in */

#define	FORMAT	"%08x"	/* to print a value */

#endif
