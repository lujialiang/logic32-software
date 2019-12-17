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

#ifndef _ASM_H_INCLUDE_
#define _ASM_H_INCLUDE_

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>

#include "func.h"
#include "parser.h"
#include "a_out.h"

#define RESERVED_REG	1

#define TEXT_AREA_SIZE (1024*64)
#define DATA_AREA_SIZE (1024*64)

#define BYTE_SIZE 1
#define HALF_SIZE 2
#define WORD_SIZE 4

#define BIT16	0
#define BIT16H	1
#define BIT30	2
#define BIT32	3
#define BIT_16H_16L			4
#define BIT_16H_NONE_16L	5

#define LINE_BUFFER_SIZE 1024
#define NCPS 80
#define HSHSIZ 2000
#define TEXT_RELOC_SIZE 1024
#define DATA_RELOC_SIZE 1024

#define LOCAL_SYMOBOL_SIZE 128

typedef struct node_s {
	int value;
} node_t, *pnode_t;

typedef struct token_s{
	char name[NCPS];
	int type;
	uint32_t value;
} token_t, *ptoken_t;

typedef struct {
	uint32_t lable;
	uint32_t value;
} local_symbol_t;

extern local_symbol_t local_symbol_table[];
extern int local_symbol_cnt;

extern char curr_file_name[];
extern uint8_t text_area[];
extern uint32_t text_size;
extern uint8_t data_area[];
extern uint32_t data_size;
extern uint8_t bss_area[];
extern uint32_t bss_size;
extern unsigned short strtab_size;

extern struct relocation_info text_reloc[];
extern uint32_t text_reloc_cnt;
extern struct relocation_info data_reloc[];
extern uint32_t data_reloc_cnt;

extern FILE *fp_in;
extern FILE *fp_out;
extern uint32_t line_num;

extern int32_t cur_seg;

extern struct nlist symbuf[];

extern char token_buf[];

extern token_t token;

typedef enum {
	sym_pseudo_op = 0,
	sym_opcode,
	sym_name,
	sym_num,
	sym_end,
	sym_newline,
	sym_gpr,
	sym_equ,
	sym_org,
	sym_global,
	sym_text,
	sym_data,
	sym_bss	,
	sym_byte,
	sym_half,
	sym_word,
	sym_ubyte,
	sym_uhalf,
	sym_uword,
	sym_comma,
	sym_semi,
	sym_eof,
	sym_lbrack,
	sym_rbrack,
	sym_align,
	sym_string,
	sym_include,
	sym_plus,
	sym_minus,
	sym_mul,
	sym_div,
	sym_mod,
	sym_lcomm,
	sym_comm,
	sym_ascii,
	sym_asciz,
	sym_local_symbol,
	sym_extern,
	sym_space,
	sym_cpr,
	sym_fpr,
	sym_unknow	= -1,
} sym_type_t;

// common.c
void dump_symbols(void);
void dump_areas(void);
void dump_strtab(void);
void dump_globals(void);

// hash.c
void hsh_init(void);
struct nlist *hshin(void);
struct nlist *lookup(void);

// utils.c
int is_num( char *s );
int str2num( unsigned long *p, char *s );
int is_name(void);

// lex.c
int mapch(char ac);
int sub_str(char sel);

// pseudo.c
int do_text(void);
int do_data(void);
int do_bss(void);
int do_equ(void);
int do_org(void);
int do_comm_lcomm(void);
int do_byte(void);
int do_half(void);
int do_word(void);
int do_ubyte(void);
int do_uhalf(void);
int do_uword(void);
int do_ascii(void);
int do_asciz(void);
int do_space(void);
int do_name(void);
int do_local_lable(void);
int do_align(void);
int do_include(void);

// global.c
int do_global(void);

// expression.c
int expression(pnode_t p);
int expression0(pnode_t p);

// strtab.c
extern char strtab[];
extern unsigned strtab_idx;
void strtab_init(void);
char *strtab_in(char *str);

// a_out.c
void out_byte(uint8_t byte);
void out_word(uint32_t word);
int out_obj( FILE *fp_obj );
int read_obj(char *);

struct relocation_info *next_text_reloc(void);
struct relocation_info *next_data_reloc(void);

extern struct nlist global_symbuf[];
extern uint32_t global_symbol_cnt;

extern int add_reloc(unsigned int address, int length, struct nlist *psym);

#endif
