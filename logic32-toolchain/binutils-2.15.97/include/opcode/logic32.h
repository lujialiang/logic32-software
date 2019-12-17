/* Table of opcodes for the logic32 microprocess.
   Copyright 2002 Free Software Foundation, Inc.

   This file is part of GDB and GAS.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

   Initially created by Kuang Hwa Lin, 2002.   */

#ifndef __OPCODE_LOGIC32_H__
#define __OPCODE_LOGIC32_H__

#define OP_AJMP		0x80000000
#define OP_JAL		0xc0000000
#define OP_ALUR		(0x00<<26)
#define OP_ADDI		(0x01<<26)
#define OP_FPU		(0x02<<26)
#define OP_FPU_MISC	(0x03<<26)
// 0x04 ???
#define OP_ANDI		(0x05<<26)
#define OP_ORI		(0x06<<26)
#define OP_XORI		(0x07<<26)
#define OP_LSLI		(0x08<<26)
#define OP_LSRI		(0x09<<26)
#define OP_ASRI		(0x0a<<26)
#define OP_LDIH		(0x0b<<26)
#define OP_LDW		(0x0c<<26)
#define OP_STW		(0x0d<<26)
#define OP_LDB		(0x0e<<26)
#define OP_STB		(0x0f<<26)
#define OP_BRN		(0x10<<26)
#define OP_JMPR		(0x11<<26)
#define OP_JALR		(0x12<<26)
#define OP_LDD		(0x13<<26)
#define OP_LDF		(0x14<<26)
#define OP_STD		(0x15<<26)
#define OP_STF		(0x16<<26)
//	0x17
//	0x18
#define OP_LDH		(0x19<<26)
#define OP_STH		(0x1a<<26)
#define OP_MISC		(0x1b<<26)
#define OP_LDBU		(0x1c<<26)
#define OP_LDHU		(0x1d<<26)
// 0x1e ???
// 0x1f ???

#define	OP_LDI		(0xff)		// not a real opcode

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

struct logic32_opcode
{
  /* Name of the instruction.  */
  char *name;

  /* Opcode word.  */
  unsigned long opcode;

  /* A string of characters which describe the operands.
     Valid characters are:
     ,        Itself.  The character appears in the assembly code.
     a        rs1      The register number is in bits 21-25 of the instruction.
     b        rs2/rd   The register number is in bits 16-20 of the instruction.
     c        rd.      The register number is in bits 11-15 of the instruction.
     f        FUNC bits 0-10 of the instruction.
     i        An immediate operand is in bits 0-16 of the instruction. 0 extended
     I        An immediate operand is in bits 0-16 of the instruction. sign extended
     d	      An 16 bit PC relative displacement.
     D	      An immediate operand is in bits 0-25 of the instruction.
     J	      An 32bit immediate operand is in bits 0-29 of the instruction.
     K	      An 32bit immediate operand
     R	      An 24bit immediate operand is in bits 0-21 of the instruction.
     N	      No opperands needed, for nops.
     P	      it can be a register or a 16 bit operand.  */
  char *args;

  /* function word.  */
  unsigned long function;
};

#define	PSW_REG_NUMBER	1
#define	EPC_REG_NUMBER	4
#define	ESR_REG_NUMBER	5

#endif __OPCODE_LOGIC32_H__
