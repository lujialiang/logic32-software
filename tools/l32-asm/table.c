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
#include "keyword_hash.h"

const keyword_hash_t keyword_buffer[] =
{
	/* 通用寄存器 */
	{ "$0", sym_gpr, 0, NULL, },
	{ "$1", sym_gpr, 1, NULL, },
	{ "$2", sym_gpr, 2, NULL, },
	{ "$3", sym_gpr, 3, NULL, },
	{ "$4", sym_gpr, 4, NULL, },
	{ "$5", sym_gpr, 5, NULL, },
	{ "$6", sym_gpr, 6, NULL, },
	{ "$7", sym_gpr, 7, NULL, },
	{ "$8", sym_gpr, 8, NULL, },
	{ "$9", sym_gpr, 9, NULL, },
	{ "$10", sym_gpr, 10, NULL, },
	{ "$11", sym_gpr, 11, NULL, },
	{ "$12", sym_gpr, 12, NULL, },
	{ "$13", sym_gpr, 13, NULL, },
	{ "$14", sym_gpr, 14, NULL, },
	{"$15", sym_gpr, 15, NULL, },
	{"$16", sym_gpr, 16, NULL, },
	{"$17", sym_gpr, 17, NULL, },
	{"$18", sym_gpr, 18, NULL, },
	{"$19", sym_gpr, 19, NULL, },
	{"$20", sym_gpr, 20, NULL, },
	{"$21", sym_gpr, 21, NULL, },
	{"$22", sym_gpr, 22, NULL, },
	{"$23", sym_gpr, 23, NULL, },
	{"$24", sym_gpr, 24, NULL, },
	{"$25", sym_gpr, 25, NULL, },
	{"$26", sym_gpr, 26, NULL, },
	{"$27", sym_gpr, 27, NULL, },
	{"$28", sym_gpr, 28, NULL, },
	{"$29", sym_gpr, 29, NULL, },
	{"$30", sym_gpr, 30, NULL, },
	{"$31", sym_gpr, 31, NULL, },
	{"$zero", sym_gpr, ZERO_REG_IDX, NULL },
	{"$ra", sym_gpr, RA_REG_IDX, NULL },
	{"$k1", sym_gpr, K1_REG_IDX, NULL },
	{"$k2", sym_gpr, K2_REG_IDX, NULL },
	{"$sp", sym_gpr, SP_REG_IDX, NULL },
	{"$lr", sym_gpr, LR_REG_IDX, NULL },
	/* 浮点寄存器 */
	{"$f0", sym_fpr, 0, NULL, },
	{"$f1", sym_fpr, 1, NULL, },
	{"$f2", sym_fpr, 2, NULL, },
	{"$f3", sym_fpr, 3, NULL, },
	{"$f4", sym_fpr, 4, NULL, },
	{"$f5", sym_fpr, 5, NULL, },
	{"$f6", sym_fpr, 6, NULL, },
	{"$f7", sym_fpr, 7, NULL, },
	{"$f8", sym_fpr, 8, NULL, },
	{"$f9", sym_fpr, 9, NULL, },
	{"$f10", sym_fpr, 10, NULL, },
	{"$f11", sym_fpr, 11, NULL, },
	{"$f12", sym_fpr, 12, NULL, },
	{"$f13", sym_fpr, 13, NULL, },
	{"$f14", sym_fpr, 14, NULL, },
	{"$f15", sym_fpr, 15, NULL, },
	{"$f16", sym_fpr, 16, NULL, },
	{"$f17", sym_fpr, 17, NULL, },
	{"$f18", sym_fpr, 18, NULL, },
	{"$f19", sym_fpr, 19, NULL, },
	{"$f20", sym_fpr, 20, NULL, },
	{"$f21", sym_fpr, 21, NULL, },
	{"$f22", sym_fpr, 22, NULL, },
	{"$f23", sym_fpr, 23, NULL, },
	{"$f24", sym_fpr, 24, NULL, },
	{"$f25", sym_fpr, 25, NULL, },
	{"$f26", sym_fpr, 26, NULL, },
	{"$f27", sym_fpr, 27, NULL, },
	{"$f28", sym_fpr, 28, NULL, },
	{"$f29", sym_fpr, 29, NULL, },
	{"$f30", sym_fpr, 30, NULL, },
	{"$f31", sym_fpr, 31, NULL, },
	/* 控制寄存器 */
	{"$c0", sym_cpr, 0, NULL, },
	{"$c1", sym_cpr, 1, NULL, },
	{"$c2", sym_cpr, 2, NULL, },
	{"$c3", sym_cpr, 3, NULL, },
	{"$c4", sym_cpr, 4, NULL, },
	{"$c5", sym_cpr, 5, NULL, },
	{"$c6", sym_cpr, 6, NULL, },
	{"$c7", sym_cpr, 7, NULL, },
	{"$c8", sym_cpr, 8, NULL, },
	{"$c9", sym_cpr, 9, NULL, },
	{"$c10", sym_cpr, 10, NULL, },
	{"$c11", sym_cpr, 11, NULL, },
	{"$c12", sym_cpr, 12, NULL, },
	{"$c13", sym_cpr, 13, NULL, },
	{"$c14", sym_cpr, 14, NULL, },
	{"$c15", sym_cpr, 15, NULL, },
	{"$c16", sym_cpr, 16, NULL, },
	{"$c17", sym_cpr, 17, NULL, },
	{"$c18", sym_cpr, 18, NULL, },
	{"$c19", sym_cpr, 19, NULL, },
	{"$c20", sym_cpr, 20, NULL, },
	{"$c21", sym_cpr, 21, NULL, },
	{"$c22", sym_cpr, 22, NULL, },
	{"$c23", sym_cpr, 23, NULL, },
	{"$c24", sym_cpr, 24, NULL, },
	{"$c25", sym_cpr, 25, NULL, },
	{"$c26", sym_cpr, 26, NULL, },
	{"$c27", sym_cpr, 27, NULL, },
	{"$c28", sym_cpr, 28, NULL, },
	{"$c29", sym_cpr, 29, NULL, },
	{"$c30", sym_cpr, 30, NULL, },
	{"$c31", sym_cpr, 31, NULL, },
	{"$psw", sym_cpr, PSW_REG_IDX, NULL, },
	{"$epc", sym_cpr, EPC_REG_IDX, NULL, },
	{"$esr", sym_cpr, ESR_REG_IDX, NULL, },
	/* 伪操作符 */
	{ ".equ",    sym_pseudo_op, sym_equ,    do_equ,   },
	{ ".org",    sym_pseudo_op, sym_org,    do_org,   },
	{ ".global", sym_pseudo_op, sym_global, do_global,},
	{ ".globl",  sym_pseudo_op, sym_global, do_global,},
	{ ".extern", sym_pseudo_op, sym_extern, do_global,},
	{ ".align",  sym_pseudo_op, sym_align,  do_align, },
	{ ".include",sym_pseudo_op, sym_include,do_include,},
	{ ".byte",   sym_pseudo_op, sym_byte,  do_byte, },
	{ ".half",   sym_pseudo_op, sym_half,  do_half, },
	{ ".word",   sym_pseudo_op, sym_word,  do_word, },
	{ ".ubyte",  sym_pseudo_op, sym_ubyte, do_ubyte,},
	{ ".uhalf",  sym_pseudo_op, sym_uhalf, do_uhalf,},
	{ ".uword",  sym_pseudo_op, sym_uword, do_uword,},
	{ ".lcomm",  sym_pseudo_op, sym_lcomm, do_comm_lcomm,},
	{ ".comm",   sym_pseudo_op, sym_comm,  do_comm_lcomm,},
	{ ".ascii",  sym_pseudo_op, sym_ascii, do_ascii,},
	{ ".asciz",  sym_pseudo_op, sym_asciz, do_asciz,},
	{ ".space",  sym_pseudo_op, sym_space, do_space,},
	{ ".text",   sym_pseudo_op, sym_text,  do_text,	},
	{ ".data",   sym_pseudo_op, sym_data,  do_data, },
	{ ".rdata",  sym_pseudo_op, sym_data,  do_data, },
	{ ".bss",    sym_pseudo_op, sym_bss,   do_bss,  },
	{ ".end",    sym_end, 0, 0,	},
};

int get_keyhash_buffer_size(void)
{
	return sizeof(keyword_buffer)/sizeof(keyword_hash_t); 
}
