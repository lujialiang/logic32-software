/* Instruction printing code for the logic32 Microprocessor
   Copyright 2002 Free Software Foundation, Inc.
   Contributed by Kuang Hwa Lin.  Written by Kuang Hwa Lin, 03/2002.

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
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#include "sysdep.h"
#include "dis-asm.h"
#include "opcode/logic32.h"

#define R_ERROR     0x1
#define R_TYPE      0x2
#define ILD_TYPE    0x3
#define IST_TYPE    0x4
#define IAL_TYPE    0x5
#define IBR_TYPE    0x6
#define IJ_TYPE     0x7
#define IJR_TYPE    0x8
#define MISC_TYPE   0x9
#define NIL         0xa

#define OPC(x)      ((x >> 26) & 0x3F)
#define FUNC(x)     (x & 0x7FF)

unsigned char opc, rs1, rs2, rd;
unsigned long imm30, imm16, func, current_insn_addr, brn_func, imm22;

static unsigned char logic32_get_opcode PARAMS ((unsigned long));
static unsigned char logic32_get_rs1    PARAMS ((unsigned long));
static unsigned char logic32_get_rs2    PARAMS ((unsigned long));
static unsigned char logic32_get_rdR    PARAMS ((unsigned long));
static unsigned long logic32_get_func   PARAMS ((unsigned long)); 
static unsigned long logic32_get_brn_func   PARAMS ((unsigned long)); 
static unsigned long logic32_get_imm16  PARAMS ((unsigned long));
static unsigned long logic32_get_imm22  PARAMS ((unsigned long));
static unsigned long logic32_get_imm30  PARAMS ((unsigned long));
static void     operand_deliminator PARAMS ((struct disassemble_info *, char *));
static unsigned char logic32_r_type     PARAMS ((struct disassemble_info *));
static unsigned char logic32_load_type  PARAMS ((struct disassemble_info *));
static unsigned char logic32_store_type PARAMS ((struct disassemble_info *));
static unsigned char logic32_aluI_type  PARAMS ((struct disassemble_info *));
static unsigned char logic32_br_type    PARAMS ((struct disassemble_info *));
static unsigned char logic32_jmp_type   PARAMS ((struct disassemble_info *));
static unsigned char logic32_jr_type    PARAMS ((struct disassemble_info *));
static unsigned char logic32_misc_type    PARAMS ((struct disassemble_info *));

/* Print one instruction from MEMADDR on INFO->STREAM.
   Return the size of the instruction (always 4 on logic32).  */

static unsigned char
logic32_get_opcode (opcode)
     unsigned long opcode;
{
  if ( opcode & 0x80000000 ) {
	/* ajmp or ajal */
	return (unsigned char) ((opcode >> 24) & 0xC0);
  }
  return (unsigned char) ((opcode >> 26) & 0x3F);
}

static unsigned char
logic32_get_rs1 (opcode)
     unsigned long opcode;
{
  return (unsigned char) ((opcode >> 16) & 0x1F);
}

static unsigned char
logic32_get_rs2 (opcode)
     unsigned long opcode;
{
  return (unsigned char) ((opcode >> 11) & 0x1F);
}

static unsigned char
logic32_get_rdR (opcode)
     unsigned long opcode;
{
  return (unsigned char) ((opcode >> 21) & 0x1F);
}

static unsigned long
logic32_get_func (opcode)
     unsigned long opcode;
{
  return (unsigned char) (opcode & 0x2F);
}

static unsigned long
logic32_get_brn_func (opcode)
     unsigned long opcode;
{
  return (unsigned char) ((opcode >> 22) & 0x0F);
}

static unsigned long
logic32_get_imm16 (opcode)
     unsigned long opcode;
{
  return (unsigned long) (opcode & 0xFFFF);
}

static unsigned long
logic32_get_imm30 (opcode)
     unsigned long opcode;
{
  return (unsigned long) (opcode & 0x3FFFFFFF);
}

static unsigned long
logic32_get_imm22 (opcode)
     unsigned long opcode;
{
  return (unsigned long) (opcode & 0x003FFFFF);
}

/* Fill the opcode to the max length.  */
static void
operand_deliminator (info, ptr)
     struct disassemble_info *info;
     char *ptr;
{
  int difft = 8 - (int) strlen (ptr);

  while (difft > 0)
    {
      (*info->fprintf_func) (info->stream, "%c", ' ');
      difft -= 1;
    }
}

/* Process the R-type opcode.  */
static unsigned char
logic32_r_type (info)
     struct disassemble_info *info;
{
  unsigned char r_opc[] = { OPC(0x00) }; /* Fix ME */
  int r_opc_num = (sizeof r_opc) / (sizeof (char));
  struct _r_opcode
  {
    unsigned long func;
    char *name;
  }
  logic32_r_opcode[] = {
    { 0x00,     "add"    },  /* ADD                          */
    { 0x01,     "adc"    },  /* ADC                          */
    { 0x02,     "sub"    },  /* SUB                          */
    { 0x03,     "sbc"    },  /* SBC                          */
    { 0x04,     "and"    },  /* AND                          */
    { 0x05,     "orr"    },  /* ORR                          */
    { 0x06,     "xor"    },  /* XOR                          */
    { 0x07,     "lsl"    },  /* LSL                          */
    { 0x08,     "lsr"    },  /* LSR                          */
    { 0x09,     "asr"    },  /* ASR                          */
    { 0x0a,     "mul"    },  /* MUL                          */
    { 0x0b,     "div"    },  /* DIV                          */
    { 0x0c,     "mod"    },  /* MOD                          */
    { 0x0d,     "mulu"   },  /* MULU                         */
    { 0x0e,     "divu"   },  /* DIVU                         */
    { 0x0f,     "modu"   },  /* MODU                         */
  };
  int logic32_r_opcode_num = (sizeof logic32_r_opcode) / (sizeof logic32_r_opcode[0]);
  int idx;

  for (idx = 0; idx < r_opc_num; idx++) {
      if (r_opc[idx] != opc)
		continue;
      else
		break;
  }

  if (idx == r_opc_num)
    return NIL;

  for (idx = 0 ; idx < logic32_r_opcode_num; idx++) {
    if (logic32_r_opcode[idx].func == func) {
		if (func == 0x00 && rd == 0x00 && rs1 == 0x00 && rs2 == 0x00 ) {
			(*info->fprintf_func) (info->stream, "nop");
		}
		else {
			/* This is not a nop.  */
			(*info->fprintf_func) (info->stream, "%s", logic32_r_opcode[idx].name);
			operand_deliminator (info, logic32_r_opcode[idx].name);
			(*info->fprintf_func) (info->stream, "r%d,", (int)rd);
			(*info->fprintf_func) (info->stream, "r%d", (int)rs1);
			(*info->fprintf_func) (info->stream, ",r%d", (int)rs2);
		}
		return (unsigned char) R_TYPE;
	}
  }

  return (unsigned char) R_ERROR;
}

/* Process the memory read opcode.  */

static unsigned char
logic32_load_type (info)
     struct disassemble_info* info;
{
  struct _load_opcode
  {
    unsigned long opcode;
    char *name;
  }
  logic32_load_opcode[] =
    {
      { 0x0c,     "lw" },  /* load word.                     */
      { 0x0e,     "lb" },  /* load byte sign extended.       */
      { 0x19,     "lh" },  /* load half.                     */
      { 0x1c,     "lbu" }, /* load byte zero extended.       */
      { 0x1d,     "lhu" }, /* load half zero extended.       */
      { 0x0b,     "ldih"}, /* load immediate to high part of register. */
    };
  int logic32_load_opcode_num =
    (sizeof logic32_load_opcode) / (sizeof logic32_load_opcode[0]);
  int idx;

  for (idx = 0 ; idx < logic32_load_opcode_num; idx++)
    if (logic32_load_opcode[idx].opcode == opc)
      {
	if (opc == 0x0b)
	  {
	    (*info->fprintf_func) (info->stream, "%s", logic32_load_opcode[idx].name);
	    operand_deliminator (info, logic32_load_opcode[idx].name);
	    (*info->fprintf_func) (info->stream, "r%d,", (int)rd);
	    (*info->fprintf_func) (info->stream, "0x%04x", (int)imm16);
	  }
	else
	  {
	    (*info->fprintf_func) (info->stream, "%s", logic32_load_opcode[idx].name);
	    operand_deliminator (info, logic32_load_opcode[idx].name);
	    (*info->fprintf_func) (info->stream, "r%d,", (int)rd);
	    (*info->fprintf_func) (info->stream, "0x%04x(r%d)", (int)imm16, (int)rs1);
	  }

	return (unsigned char) ILD_TYPE;
    }

  return (unsigned char) NIL;
}

/* Process the memory store opcode.  */

static unsigned char
logic32_store_type (info)
     struct disassemble_info* info;
{
  struct _store_opcode
  {
    unsigned long opcode;
    char *name;
  }
  logic32_store_opcode[] =
    {
      { 0x0d,     "sw" },  /* Store word.      */
      { 0x0f,     "sb" },  /* Store byte.      */
      { 0x1a,     "sh" },  /* Store halfword.  */
    };
  int logic32_store_opcode_num =
    (sizeof logic32_store_opcode) / (sizeof logic32_store_opcode[0]);
  int idx;

  for (idx = 0 ; idx < logic32_store_opcode_num; idx++)
    if (logic32_store_opcode[idx].opcode == opc)
      {
	(*info->fprintf_func) (info->stream, "%s", logic32_store_opcode[idx].name);
	operand_deliminator (info, logic32_store_opcode[idx].name);
	(*info->fprintf_func) (info->stream, "0x%04x(r%d),", (int)imm16, (int)rs1);
	(*info->fprintf_func) (info->stream, "r%d", (int)rd);
	return (unsigned char) IST_TYPE;
      }

  return (unsigned char) NIL;
}

/* Process the Arithmetic and Logical I-TYPE opcode.  */

static unsigned char
logic32_aluI_type (info)
     struct disassemble_info* info;
{
  struct _aluI_opcode
  {
    unsigned long opcode;
    char *name;
  }
  logic32_aluI_opcode[] =
    {
      { 0x01,   "addi"  },
      { 0x05,   "andi"  },
      { 0x06,   "ori"   },
      { 0x07,   "xori"  },
      { 0x08,   "lsli"  },
      { 0x09,   "lsri"  },
      { 0x0a,   "asri"  },
    };
  int logic32_aluI_opcode_num =
    (sizeof logic32_aluI_opcode) / (sizeof logic32_aluI_opcode[0]);
  int idx;

  for (idx = 0 ; idx < logic32_aluI_opcode_num; idx++)
    if (logic32_aluI_opcode[idx].opcode == opc)
      {
	(*info->fprintf_func) (info->stream, "%s", logic32_aluI_opcode[idx].name);
	operand_deliminator (info, logic32_aluI_opcode[idx].name);
	(*info->fprintf_func) (info->stream, "r%d,", (int)rd);
	(*info->fprintf_func) (info->stream, "r%d,", (int)rs1);
	(*info->fprintf_func) (info->stream, "0x%04x", (int)imm16);

	return (unsigned char) IAL_TYPE;
      }

  return (unsigned char) NIL;
}

/* Process the branch instruction.  */

static unsigned char
logic32_br_type (info)
     struct disassemble_info* info;
{
  struct _br_opcode
  {
    unsigned long func;
    char *name;
  }
  logic32_br_opcode[] =
    {
      { 0x00, "beq" },
      { 0x01, "bne" },
      { 0x02, "bcs" },
      { 0x03, "bcc" },
      { 0x04, "bhs" },
      { 0x05, "blo" },
      { 0x06, "bvs" },
      { 0x07, "bvc" },
      { 0x08, "bhi" },
      { 0x09, "bls" },
      { 0x0a, "bge" },
      { 0x0b, "blt" },
      { 0x0c, "bgt" },
      { 0x0d, "ble" },
      { 0x0e, "brn" },
      { 0x0f, "bmi" },
    };
  int logic32_br_opcode_num =
    (sizeof logic32_br_opcode) / (sizeof logic32_br_opcode[0]);
  int idx;

  if ( opc != 0x10 ) {	/* branch */
	return (unsigned char) NIL;
  }

  for (idx = 0 ; idx < logic32_br_opcode_num; idx++)
    if (logic32_br_opcode[idx].func == brn_func)
      {
		int offset = 0;
		if (imm22 & 0x00200000) {
			offset = (imm22 | 0xFFC00000);
		}
		else {
			offset = imm22;
		}
		imm22 = current_insn_addr + offset*4;

		(*info->fprintf_func) (info->stream, "%s", logic32_br_opcode[idx].name);
		operand_deliminator (info, logic32_br_opcode[idx].name);
		(*info->fprintf_func) (info->stream, "0x%08x", imm22);

		return (unsigned char) IBR_TYPE;
      }
  return (unsigned char) NIL;
}

/* Process the jump instruction.  */

static unsigned char
logic32_jmp_type (info)
     struct disassemble_info* info;
{
  struct _jmp_opcode
  {
    unsigned long opcode;
    char *name;
  }
  logic32_jmp_opcode[] =
    {
      { 0x80,     "ajmp" },
      { 0xc0,     "ajal" },
    };
  int logic32_jmp_opcode_num =
    (sizeof logic32_jmp_opcode) / (sizeof logic32_jmp_opcode[0]);
  int idx;

  for (idx = 0 ; idx < logic32_jmp_opcode_num; idx++)
    if (logic32_jmp_opcode[idx].opcode == opc)
      {
	imm30 <<= 2;

	(*info->fprintf_func) (info->stream, "%s", logic32_jmp_opcode[idx].name);
	operand_deliminator (info, logic32_jmp_opcode[idx].name);
	(*info->fprintf_func) (info->stream, "0x%08x", (int)imm30);

	return (unsigned char) IJ_TYPE;
      }
  return (unsigned char) NIL;
}

/* Process the jump register instruction.  */

static unsigned char
logic32_jr_type (info)
     struct disassemble_info* info;
{
  struct _jr_opcode
  {
    unsigned long opcode;
    char *name;
  }
  logic32_jr_opcode[] = {
    { 0x11,			"jmpr" },
    { 0x12,			"jalr" },
  };
  int logic32_jr_opcode_num =
    (sizeof logic32_jr_opcode) / (sizeof logic32_jr_opcode[0]);
  int idx;

  for (idx = 0 ; idx < logic32_jr_opcode_num; idx++)
    if (logic32_jr_opcode[idx].opcode == opc)
      {
	(*info->fprintf_func) (info->stream, "%s", logic32_jr_opcode[idx].name);
	operand_deliminator (info, logic32_jr_opcode[idx].name);
	(*info->fprintf_func) (info->stream, "r%d", (int)rd);
	return (unsigned char) IJR_TYPE;
      }

  return (unsigned char) NIL;
}

/* Process the miscellaneous instruction.  */

static unsigned char
logic32_misc_type (info)
     struct disassemble_info* info;
{
  unsigned char r_opc[] = { 0x1b };
  int r_opc_num = (sizeof r_opc) / (sizeof (char));
  struct _r_opcode
  {
    unsigned long func;
    char *name;
  }
  logic32_misc_opcode[] = {
    { 0x01,     "reti"    },
    { 0x02,     "trap"    },
    { 0x03,     "halt"    },
    { 0x04,     "enai"    },
    { 0x05,     "disi"    },
    { 0x06,     "mtc"     },
    { 0x07,     "mfc"     },
    { 0x0c,     "not"     },
  };
  int logic32_r_opcode_num = (sizeof logic32_misc_opcode) / (sizeof logic32_misc_opcode[0]);
  int idx;

  for (idx = 0; idx < r_opc_num; idx++) {
      if (r_opc[idx] != opc)
		continue;
      else
		break;
  }

  if (idx == r_opc_num)
    return NIL;

  for (idx = 0 ; idx < logic32_r_opcode_num; idx++) {
    if (logic32_misc_opcode[idx].func == func) {
		if ( func == 0x0c ) {
			/* not */
			(*info->fprintf_func) (info->stream, "%s", logic32_misc_opcode[idx].name);
			operand_deliminator (info, logic32_misc_opcode[idx].name);
			(*info->fprintf_func) (info->stream, "r%d,", (int)rd);
			(*info->fprintf_func) (info->stream, "r%d", (int)rs1);
		}
		else if( func == 0x06 ) {
			/* MTC */
			(*info->fprintf_func) (info->stream, "%s", logic32_misc_opcode[idx].name);
			operand_deliminator (info, logic32_misc_opcode[idx].name);
			switch(rd) {
				case PSW_REG_NUMBER:
					(*info->fprintf_func) (info->stream, "psw,", (int)rd);
					break;
				case EPC_REG_NUMBER:
					(*info->fprintf_func) (info->stream, "epc,", (int)rd);
					break;
				case ESR_REG_NUMBER:
					(*info->fprintf_func) (info->stream, "esr,", (int)rd);
					break;
				default:
					(*info->fprintf_func) (info->stream, "c%d,", (int)rd);
					break;
			}
			(*info->fprintf_func) (info->stream, "r%d", (int)rs1);
		}
		else if( func == 0x07 ) {
			/* MFC */
			(*info->fprintf_func) (info->stream, "%s", logic32_misc_opcode[idx].name);
			operand_deliminator (info, logic32_misc_opcode[idx].name);
			(*info->fprintf_func) (info->stream, "r%d,", (int)rd);
			switch(rs1) {
				case PSW_REG_NUMBER:
					(*info->fprintf_func) (info->stream, "psw", (int)rd);
					break;
				case EPC_REG_NUMBER:
					(*info->fprintf_func) (info->stream, "epc", (int)rd);
					break;
				case ESR_REG_NUMBER:
					(*info->fprintf_func) (info->stream, "esr", (int)rd);
					break;
				default:
					(*info->fprintf_func) (info->stream, "c%d", (int)rd);
					break;
			}
		}
		else {
			(*info->fprintf_func) (info->stream, "%s", logic32_misc_opcode[idx].name);
			operand_deliminator (info, logic32_misc_opcode[idx].name);
		}
		return (unsigned char) MISC_TYPE;
	}
  }

  return (unsigned char) R_ERROR;
}

typedef unsigned char (* logic32_insn) PARAMS ((struct disassemble_info *));

/* This is the main logic32 insn handling routine.  */

int
print_insn_logic32 (memaddr, info)
     bfd_vma memaddr;
     struct disassemble_info* info;
{
  bfd_byte buffer[4];
  int insn_idx;
  unsigned long insn_word;
  unsigned char rtn_code;
  unsigned long logic32_insn_type[] =
    {
      (unsigned long) logic32_r_type,
      (unsigned long) logic32_load_type,
      (unsigned long) logic32_store_type,
      (unsigned long) logic32_aluI_type,
      (unsigned long) logic32_br_type,
      (unsigned long) logic32_jmp_type,
      (unsigned long) logic32_jr_type,
      (unsigned long) logic32_misc_type,
      (unsigned long) NULL
  };
  int logic32_insn_type_num = ((sizeof logic32_insn_type) / (sizeof (unsigned long))) - 1;
  int status =
    (*info->read_memory_func) (memaddr, (bfd_byte *) &buffer[0], 4, info);

  if (status != 0)
    {
      (*info->memory_error_func) (status, memaddr, info);
      return -1;
    }

  /* Now decode the insn    */
  insn_word = bfd_getl32 (buffer);
  opc  = logic32_get_opcode (insn_word);
  rs1  = logic32_get_rs1 (insn_word);
  rs2  = logic32_get_rs2 (insn_word);
  rd   = logic32_get_rdR (insn_word);
  func = logic32_get_func (insn_word);
  imm16= logic32_get_imm16 (insn_word);
  imm30= logic32_get_imm30 (insn_word);
  brn_func = logic32_get_brn_func (insn_word);
  imm22= logic32_get_imm22 (insn_word);

#if 0
  printf ("print_insn_logic32: insn_word = 0x%08x\n"
	  "                    opc = 0x%02x\n"
	  "                    rs1 = 0x%02x\n"
	  "                    rs2 = 0x%02x\n"
	  "                    rd  = 0x%02x\n"
	  "                  func  = 0x%08x\n"
	  "                 imm16  = 0x%08x\n"
	  "                 imm30  = 0x%08x\n",
	  insn_word, opc, rs1, rs2, rd, func, imm16, imm30);
#endif

  /* Scan through all the insn type and print the insn out.  */
  rtn_code = 0;
  current_insn_addr = (unsigned long) memaddr;

  for (insn_idx = 0; logic32_insn_type[insn_idx] != 0x0; insn_idx++)
    switch (((logic32_insn) (logic32_insn_type[insn_idx])) (info))
      {
	/* Found the correct opcode   */
      case R_TYPE:
      case ILD_TYPE:
      case IST_TYPE:
      case IAL_TYPE:
      case IBR_TYPE:
      case IJ_TYPE:
      case IJR_TYPE:
      case MISC_TYPE:
	return 4;

	/* Wrong insn type check next one. */
      default:
      case NIL:
	continue;

	/* All rest of the return code are not recongnized, treat it as error */
	/* we should never get here,  I hope! */
      case R_ERROR:
	return -1;
      }

  if (insn_idx ==  logic32_insn_type_num)
    /* Well, does not recoganize this opcode.  */
    (*info->fprintf_func) (info->stream, "<%s>", "Unrecognized Opcode");

  return 4;
}
