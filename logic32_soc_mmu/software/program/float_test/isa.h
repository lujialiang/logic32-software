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

#ifndef _ISA_H_INCLUDE_
#define _ISA_H_INCLUDE_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define OP_AJMP		0x80000000
#define OP_JAL		0xc0000000
#define OP_ALUR		0x00
#define OP_ADDI		0x01
#define OP_FPU		0x02
#define OP_FPU_MISC	0x03
// 0x04 ???
#define OP_ANDI		0x05
#define OP_ORI		0x06
#define OP_XORI		0x07
#define OP_LSLI		0x08
#define OP_LSRI		0x09
#define OP_ASRI		0x0a
#define OP_LDIH		0x0b
#define OP_LDW		0x0c
#define OP_STW		0x0d
#define OP_LDB		0x0e
#define OP_STB		0x0f
#define OP_BRN		0x10
#define OP_JMPR		0x11
#define OP_JALR		0x12
#define OP_LDD		0x13
#define OP_LDF		0x14
#define OP_STD		0x15
#define OP_STF		0x16
//	0x17
//	0x18
#define OP_LDH		0x19
#define OP_STH		0x1a
#define OP_MISC		0x1b
#define OP_LDBU		0x1c
#define OP_LDHU		0x1d
// 0x1e ???
// 0x1f ???

enum {
	// 0x00
	OP_MISC_RETI = 0x01,
	OP_MISC_TRAP = 0x02,
	OP_MISC_HALT = 0x03,
	OP_MISC_ENAI = 0x04,
	OP_MISC_DISI = 0x05,
	OP_MISC_MTC  = 0x06,	// 20171019
	OP_MISC_MFC  = 0x07,	// 20171019
	OP_MISC_TLB_MR_W = 0x08,
	OP_MISC_TLB_MR_R = 0x09,
	OP_MISC_TLB_TR_W = 0x0a,
	OP_MISC_TLB_TR_R = 0x0b,
	OP_MISC_NOT	  = 0x0c,	// 20170503
	OP_MISC_LEV   = 0x0d,	// 20171018
	// 0x0e
	// 0x0f
};

enum {
	ALUR_ADD = 0x00,
	ALUR_ADC = 0x01,
	ALUR_SUB = 0x02,
	ALUR_SBC = 0x03,
	ALUR_AND = 0x04,
	ALUR_ORR = 0x05,
	ALUR_XOR = 0x06,
	ALUR_LSL = 0x07,
	ALUR_LSR = 0x08,
	ALUR_ASR = 0x09,
	ALUR_MUL = 0x0a,
	ALUR_DIV = 0x0b,
	ALUR_MOD = 0x0c,
	ALUR_MULU= 0x0d,	// 20171018
	ALUR_DIVU= 0x0e,	// 20171018
	ALUR_MODU= 0x0f,	// 20171018
};

enum {
	FPU_ADDS = 0x00,
	FPU_SUBS = 0x01,
	FPU_MULS = 0x02,
	FPU_DIVS = 0x03,
	FPU_ADDD = 0x04,
	FPU_SUBD = 0x05,
	FPU_MULD = 0x06,
	FPU_DIVD = 0x07,
	FPU_MOVS = 0x08,
	FPU_MOVD = 0x09,
	FPU_CVDF = 0x0a,
	FPU_CVFD = 0x0b,
	FPU_CVWD = 0x0c,
	FPU_CVDW = 0x0d,
	FPU_NEGD = 0x0e,
	FPU_NEGS = 0x0f,
};

enum {
	FPU_MISC_MFC1		= 0x00,
	FPU_MISC_MTC1		= 0x01,
	FPU_MISC_CMP_EQD	= 0x02,
	FPU_MISC_CMP_LED	= 0x03,
	FPU_MISC_CMP_LTD	= 0x04,
	FPU_MISC_CMP_EQS	= 0x05,
	FPU_MISC_CMP_LES	= 0x06,
	FPU_MISC_CMP_LTS	= 0x07,
	FPU_MISC_BC1F		= 0x08,
	FPU_MISC_BC1T		= 0x09,
	FPU_MISC_MFC1_D		= 0x0a,
	FPU_MISC_MTC1_D		= 0x0b,
};

typedef enum {
	brn_func_beq = 0,
	brn_func_bne = 1,
	brn_func_bcs = 2,
	brn_func_bcc = 3,
	brn_func_bhs = 4,
	brn_func_blo = 5,
	brn_func_bvs = 6,
	brn_func_bvc = 7,
	brn_func_bhi = 8,
	brn_func_bls = 9,
	brn_func_bge = 10,
	brn_func_blt = 11,
	brn_func_bgt = 12,
	brn_func_ble = 13,
	brn_func_bal = 14,
	brn_func_bmi = 15,
	brn_func_undefined = -1,
} brn_func_t;

#define PSW_FLT	7
#define PSW_E	6	// 1: in excption, 0: normal
#define PSW_S	5	// 1: system mode, 0: user mode
#define PSW_I	4
#define PSW_N	3
#define PSW_Z	2
#define PSW_V	1
#define PSW_C	0

#define ZERO_REG_IDX	0	// $0
#define RA_REG_IDX		2	// $0
#define K1_REG_IDX		26	// $26
#define K2_REG_IDX		27	// $27
#define SP_REG_IDX		29	// $29
#define LR_REG_IDX		31	// $31
#define CR0_REG_IDX		0	// $c0
#define PSW_REG_IDX		1	// $c1
#define EPC_REG_IDX		4	// $c4
#define ESR_REG_IDX		5	// $c5

#define TRAP_SYSTICK		(0x10)
#define VECTOR_IRQ_SYSTICK	(0x10)

#define TRAP_KEYBOARD		(0x20)
#define VECTOR_IRQ_KEYBOARD (0x20)

#define TRAP_UNDF			(0x30)
#define VECTOR_TRAP_UNDF	(0x30)

#define TRAP_SOFTWAVE		(0x40)
#define VECTOR_TRAP_SOFTWAVE (0x40)

#define TRAP_DMA_CH0		(0x50)
#define VECTOR_IRQ_DMA_CH0	(0x50)

#define TRAP_DISK_CH0		(0x60)
#define VECTOR_IRQ_DISK0	(0x60)

#define TRAP_TMR0			(0x70)
#define VECTOR_IRQ_TMR0		(0x70)

#define TRAP_DIV_BY_ZERO	(0x80)
#define VECTOR_DIV_BY_ZERO	(0x80)

#define TRAP_BUS_ERROR		(0x90)
#define VECTOR_BUS_ERROR	(0x90)

#define TRAP_ITBL_MISS		(0xa0)
#define VECTOR_ITBL_MISS	(0xa0)

#define TRAP_DTBL_MISS		(0xb0)
#define VECTOR_DTBL_MISS	(0xb0)

#define TRAP_HARD_FAULT		(0xc0)
#define VECTOR_HARD_FAULT	(0xc0)

#define TRAP_NIC_TX			(0xd0)
#define VECTOR_NIC_TX		(0xd0)

#define TRAP_NIC_RX			(0xe0)
#define VECTOR_NIC_RX		(0xe0)

void psw_set_bit(uint32_t, bool);
bool psw_get_bit(uint32_t);

#ifdef __cplusplus
}
#endif

#endif
