/* tc-ldx.c -- Assemble for the LOGIC32
   Copyright 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GAS; see the file COPYING.  If not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

/* Initially created by Kuang Hwa Lin, 3/20/2002.  */

#include "safe-ctype.h"
#include "as.h"
#include "tc-logic32.h"
#include "opcode/logic32.h"

static const struct logic32_opcode logic32_opcodes[] =
{

  /* Arithmetic and Logic R-TYPE instructions.  */
    { "add",      OP_ALUR,   "a,b,c",	ALUR_ADD	},  /* ADD                          */
    { "adc",      OP_ALUR,   "a,b,c",	ALUR_ADC	},  /* ADC                          */
    { "sub",      OP_ALUR,   "a,b,c",	ALUR_SUB	},  /* SUB                          */
    { "sbc",      OP_ALUR,   "a,b,c",	ALUR_SBC	},  /* SBC                          */
    { "and",      OP_ALUR,   "a,b,c",	ALUR_AND	},  /* AND                          */
    { "orr",      OP_ALUR,   "a,b,c",	ALUR_ORR	},  /* OR                           */
    { "xor",      OP_ALUR,   "a,b,c",	ALUR_XOR	},  /* XOR                          */
    { "lsl",      OP_ALUR,   "a,b,c",	ALUR_LSL	},  /* LSL                          */
    { "lsr",      OP_ALUR,   "a,b,c",	ALUR_LSR	},  /* LSR                          */
    { "asr",      OP_ALUR,   "a,b,c",	ALUR_ASR	},  /* ASR                          */
    { "mul",      OP_ALUR,   "a,b,c",	ALUR_MUL	},  /* MUL                          */
    { "div",      OP_ALUR,   "a,b,c",	ALUR_DIV	},  /* DIV                          */
    { "mod",	  OP_ALUR,   "a,b,c",	ALUR_MOD	},  /* MOD                          */
    { "mulu",     OP_ALUR,   "a,b,c",	ALUR_MULU	},  /* MULU                          */
    { "divu",     OP_ALUR,   "a,b,c",	ALUR_DIVU	},  /* DIVU                          */
	{ "modu",     OP_ALUR,   "a,b,c",	ALUR_MODU	},  /* MODU                          */

    { "cmp",      OP_ALUR,   "b,c",		ALUR_SUB	},	/* compare                       */
    { "mov",      OP_ALUR,   "a,c",		ALUR_ORR	},	/* movement                      */
    { "ldi",      OP_ORI,    "a,K",		OP_LDIH		},	/* load effective address        */

	{ "add.f",      OP_FPU,   "a,b,c",   FPU_ADDS    },
	{ "sub.f",      OP_FPU,   "a,b,c",   FPU_SUBS    },
	{ "mul.f",      OP_FPU,   "a,b,c",   FPU_MULS    },
	{ "div.f",      OP_FPU,   "a,b,c",   FPU_DIVS    },
	{ "add.d",      OP_FPU,   "a,b,c",   FPU_ADDD    },
	{ "sub.d",      OP_FPU,   "a,b,c",   FPU_SUBD    },
	{ "muld",      OP_FPU,   "a,b,c",   FPU_MULD    },
	{ "div.d",      OP_FPU,   "a,b,c",   FPU_DIVD    },
	{ "mov.f",      OP_FPU,   "a,b",   FPU_MOVS    },
	{ "mov.d",      OP_FPU,   "a,b",   FPU_MOVD    },
	{ "cvdf",      OP_FPU,   "a,b",   FPU_CVDF    },
	{ "cvfd",      OP_FPU,   "a,b",   FPU_CVFD    },
	{ "cvwd",      OP_FPU,   "a,b",   FPU_CVWD    },
	{ "cvdw",      OP_FPU,   "a,b",   FPU_CVDW    },
	{ "neg.d",      OP_FPU,   "a,b",   FPU_NEGD    },
	{ "neg.f",      OP_FPU,   "a,b",   FPU_NEGS    },

    { "addi",      OP_ADDI,   "a,b,I",	0x0	},
    { "andi",      OP_ANDI,   "a,b,I",	0x0	},
    { "ori",       OP_ORI,   "a,b,I",	0x0	},
    { "xori",      OP_XORI,   "a,b,I",	0x0	},
    { "lsli",      OP_LSLI,   "a,b,I",	0x0	},
    { "lsri",      OP_LSRI,   "a,b,I",	0x0	},
    { "asri",      OP_ASRI,   "a,b,I",	0x0	},
    { "ldih",      OP_LDIH,   "a,i",	0x0	},

    { "jmpr",      OP_JMPR,   "a",	0x0	},
    { "jalr",      OP_JALR,   "a",	0x0	},
	
    { "jmp",       OP_AJMP,   "J",	0x0	},
    { "jal",       OP_JAL,    "J",	0x0	},

    { "beq",       OP_BRN,   "R",	brn_func_beq	},
    { "bne",       OP_BRN,   "R",	brn_func_bne	},
    { "bcs",       OP_BRN,   "R",	brn_func_bcs	},
    { "bcc",       OP_BRN,   "R",	brn_func_bcc	},
    { "bhs",       OP_BRN,   "R",	brn_func_bhs	},
    { "blo",       OP_BRN,   "R",	brn_func_blo	},
    { "bvs",       OP_BRN,   "R",	brn_func_bvs	},
    { "bvc",       OP_BRN,   "R",	brn_func_bvc	},
    { "bhi",       OP_BRN,   "R",	brn_func_bhi	},
    { "bls",       OP_BRN,   "R",	brn_func_bls	},
    { "bge",       OP_BRN,   "R",	brn_func_bge	},
    { "blt",       OP_BRN,   "R",	brn_func_blt	},
    { "bgt",       OP_BRN,   "R",	brn_func_bgt	},
    { "ble",       OP_BRN,   "R",	brn_func_ble	},
    { "brn",       OP_BRN,   "R",	brn_func_bal	},
    { "bmi",       OP_BRN,   "R",	brn_func_bmi	},

    { "bgeu",      OP_BRN,   "R",	brn_func_bhs	},
    { "bltu",       OP_BRN,   "R",	brn_func_blo	},
    { "bgtu",       OP_BRN,   "R",	brn_func_bhi	},
    { "bleu",       OP_BRN,   "R",	brn_func_bls	},

    { "lw",        OP_LDW,    "a,b,I", 0x00 },
    { "sw",        OP_STW,    "a,b,I", 0x00 },
    { "lh",        OP_LDH,    "a,b,I", 0x00 },
    { "sh",        OP_STH,    "a,b,I", 0x00 },
    { "lb",        OP_LDB,    "a,b,I", 0x00 },
    { "sb",        OP_STB,    "a,b,I", 0x00 },
    { "lbu",       OP_LDBU,    "a,b,I", 0x00 },
    { "lhu",       OP_LDHU,    "a,b,I", 0x00 },

	{ "reti",      OP_MISC,   "",   OP_MISC_RETI    },
	{ "trap",      OP_MISC,   "",   OP_MISC_TRAP    },
	{ "halt",      OP_MISC,   "",   OP_MISC_HALT    },
	{ "enai",      OP_MISC,   "",   OP_MISC_ENAI    },
	{ "disi",      OP_MISC,   "",   OP_MISC_DISI    },
	{ "mtc",       OP_MISC,   "x,b",   OP_MISC_MTC    },
	{ "mfc",       OP_MISC,   "a,y",   OP_MISC_MFC    },
	{ "not",       OP_MISC,   "a,b",   OP_MISC_NOT    },
	{ "level",     OP_MISC,   "",   OP_MISC_LEV    },

    { "nop",       OP_ALUR,   "N", 0x00     },  /* NOP */

    { "", 0x0, "", 0x0 }		/* Dummy entry, not included in NUM_OPCODES.
				   This lets code examine entry i + 1 without
				   checking if we've run off the end of the table.  */
};

const unsigned int num_logic32_opcodes = (((sizeof logic32_opcodes) / (sizeof logic32_opcodes[0])) - 1);


/* Make it easier to clone this machine desc into another one.  */
#define	machine_opcode      logic32_opcode
#define	machine_opcodes     logic32_opcodes
#define	machine_ip          logic32_ip
#define	machine_it          logic32_it

#define NO_RELOC            BFD_RELOC_NONE
//#define RELOC_LOGIC32_REL26     BFD_RELOC_LOGIC32_JMP26
#define RELOC_LOGIC32_16        BFD_RELOC_16
#define RELOC_LOGIC32_REL16     BFD_RELOC_16_PCREL_S2
#define RELOC_LOGIC32_HI16      BFD_RELOC_HI16_S
#define RELOC_LOGIC32_LO16      BFD_RELOC_LO16
#define RELOC_LOGIC32_VTINHERIT BFD_RELOC_VTABLE_INHERIT
#define RELOC_LOGIC32_VTENTRY   BFD_RELOC_VTABLE_ENTRY

#define RELOC_LOGIC32_REL24     BFD_RELOC_LOGIC32_JMP24	// LJL
#define RELOC_LOGIC32_JMP32     BFD_RELOC_LOGIC32_JMP32	// LJL
#define RELOC_LOGIC32_LDI	    BFD_RELOC_LOGIC32_LDI	// LJL
static int insn_size = 4;

/* handle of the OPCODE hash table */
static struct hash_control *op_hash = NULL;

struct machine_it
{
  char *error;
  unsigned long opcode;
  struct nlist *nlistp;
  expressionS exp;
  int pcrel;
  int size;
  int reloc_offset;		/* Offset of reloc within insn.  */
  int reloc;
  int HI;
  int LO;
  unsigned long opcode_ex;
  int ctrl_reg_nr;
}
the_insn;

/* static void print_insn PARAMS ((struct machine_it *)); */
char * parse_operand    PARAMS ((char *, expressionS *));
int md_chars_to_number  PARAMS ((unsigned char *, int));

static void machine_ip          PARAMS ((char *));
static void s_proc              PARAMS ((int));
static void insert_sreg         PARAMS ((char *, int));
static int  hilo_modifier_ok    PARAMS ((char *));
static int  is_ldst_registers   PARAMS ((char *));
static int  match_sft_register  PARAMS ((char *));
static void define_some_regs    PARAMS ((void));
static char * logic32_parse_loadop  PARAMS ((char *));
static char * logic32_parse_storeop PARAMS ((char *));
static char * fix_ld_st_operand PARAMS ((unsigned long, char *));

const pseudo_typeS

logic32_pseudo_table[] =
  {
    /* Some additional ops that are used by gcc-logic32.  */
    {"asciiz", stringer, 1},
    {"half", cons, 2},
    {"dword", cons, 8},
    {"word", cons, 4},
    {"proc", s_proc, 0},
    {"endproc", s_proc, 1},
    {NULL, 0, 0},
  };

/* This array holds the chars that always start a comment.  If the
   pre-processor is disabled, these aren't very useful.  */
const char comment_chars[] = ";";

/* This array holds the chars that only start a comment at the beginning of
   a line.  If the line seems to have the form '# 123 filename'
   .line and .file directives will appear in the pre-processed output.  */
/* Note that input_file.c hand checks for '#' at the beginning of the
   first line of the input file.  This is because the compiler outputs
   #NO_APP at the beginning of its output.  */
/* Also note that comments like this one will always work.  */
const char line_comment_chars[] = "#";

/* We needed an unused char for line separation to work around the
   lack of macros, using sed and such.  */
const char line_separator_chars[] = "@";

/* Chars that can be used to separate mant from exp in floating point nums.  */
const char EXP_CHARS[] = "eE";

/* Chars that mean this number is a floating point constant.
   As in 0f12.456
   or    0d1.2345e12.  */
const char FLT_CHARS[] = "rRsSfFdDxXpP";

static void
insert_sreg (regname, regnum)
     char *regname;
     int regnum;
{
  /* Must be large enough to hold the names of the special registers.  */
  char buf[80];
  int i;

  symbol_table_insert (symbol_new (regname, reg_section, (valueT) regnum,
				   &zero_address_frag));
  for (i = 0; regname[i]; i++)
    buf[i] = ISLOWER (regname[i]) ? TOUPPER (regname[i]) : regname[i];
  buf[i] = '\0';

  symbol_table_insert (symbol_new (buf, reg_section, (valueT) regnum,
				   &zero_address_frag));
}

/* Install symbol definitions for assorted special registers.
   See MIPS Assembly Language Programmer's Guide page 1-4   */

static void
define_some_regs ()
{
  /* Software representation.  */
#if 0
  insert_sreg ("zero",  0);
  insert_sreg ("at",    1);
  insert_sreg ("v0",    2);
  insert_sreg ("v1",    3);
  insert_sreg ("a0",    4);
  insert_sreg ("a1",    5);
  insert_sreg ("a2",    6);
  insert_sreg ("a3",    7);
  insert_sreg ("t0",    8);
  insert_sreg ("t1",    9);
  insert_sreg ("t2",    10);
  insert_sreg ("t3",    11);
  insert_sreg ("t4",    12);
  insert_sreg ("t5",    13);
  insert_sreg ("t6",    14);
  insert_sreg ("t7",    15);
  insert_sreg ("s0",    16);
  insert_sreg ("s1",    17);
  insert_sreg ("s2",    18);
  insert_sreg ("s3",    19);
  insert_sreg ("s4",    20);
  insert_sreg ("s5",    21);
  insert_sreg ("s6",    22);
  insert_sreg ("s7",    23);
  insert_sreg ("t8",    24);
  insert_sreg ("t9",    25);
  insert_sreg ("k0",    26);
  insert_sreg ("k1",    27);
  insert_sreg ("gp",    28);
  insert_sreg ("sp",    29);
  insert_sreg ("fp",    30);
  insert_sreg ("ra",    31);
  /* Special registers.  */
  insert_sreg ("pc",    0);
  insert_sreg ("npc",   1);
  insert_sreg ("iad",   2);
#endif
}

/* Subroutine check the string to match an register, */
static const char *soft_reg[] = {
	"$zero",
	"$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
	"$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
	"$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
	"$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra",
};

static int
match_sft_register (name)
     char *name;
{
	char low_name[21], *ptr;
	int idx;

	for (ptr = name,idx = 0; *ptr != '\0'; ptr++)
		low_name[idx++] = TOLOWER (*ptr);
	low_name[idx] = '\0';

	if ( strncmp(soft_reg[0], low_name, 5) == 0 ) {
		return 1;
	}
	for (idx = 1; idx < 32; idx++) {
		if ( strncmp(soft_reg[idx], low_name, 3) == 0 ) {
			return 1;
		}
	}
	return 0;
}

static int find_sft_register (const char *name, int *len)
{
	char low_name[21], *ptr;
	int idx;

	for (ptr = name,idx = 0; *ptr != '\0'; ptr++)
		low_name[idx++] = TOLOWER (*ptr);
	low_name[idx] = '\0';

	if ( strncmp(soft_reg[0], low_name, 5) == 0 ) {
		*len = 5;
		return 0;
	}

	for (idx = 1; idx < 32; idx++) {
		if ( strncmp(soft_reg[idx], low_name, 3) == 0 ) {
			*len = 3;
			return idx;
		}
	}
	return -1;
}

/* Subroutine check the string to match an register.  */
static int
is_ldst_registers (name)
     char *name;
{
  char *ptr = name;

  /* The first character of the register name got to be either %, $, r of R.  */
  if ((ptr[0] == '%' || ptr[0] == '$' || ptr[0] == 'r' || ptr[0] == 'R')
      && ISDIGIT ((unsigned char) ptr[1]))
  {
		if ( ISDIGIT((unsigned char) ptr[2])) {
			return !ISALNUM((unsigned char)ptr[3]) && ptr[3] != '_';
		}
		else {
			return !ISALNUM((unsigned char)ptr[2]) && ptr[2] != '_';
		}
  }

  /* Now check the software register representation.  */
  return match_sft_register (ptr);
}

/* Subroutine of s_proc so targets can choose a different default prefix.
   If DEFAULT_PREFIX is NULL, use the target's "leading char".  */

static void
s_proc (end_p)
     int end_p;
{
  /* Record the current function so that we can issue an error message for
     misplaced .func,.endfunc, and also so that .endfunc needs no
     arguments.  */
  static char *current_name;
  static char *current_label;

  if (end_p)
    {
      if (current_name == NULL)
	{
	  as_bad (_("missing .proc"));
	  ignore_rest_of_line ();
	  return;
	}

      current_name = current_label = NULL;
      SKIP_WHITESPACE ();
      while (!is_end_of_line[(unsigned char) *input_line_pointer])
        input_line_pointer++;
    }
  else
    {
      char *name, *label;
      char delim1, delim2;

      if (current_name != NULL)
	{
	  as_bad (_(".endfunc missing for previous .proc"));
	  ignore_rest_of_line ();
	  return;
	}

      name = input_line_pointer;
      delim1 = get_symbol_end ();
      name = xstrdup (name);
      *input_line_pointer = delim1;
      SKIP_WHITESPACE ();

      if (*input_line_pointer != ',')
	{
	  char leading_char = 0;

	  leading_char = bfd_get_symbol_leading_char (stdoutput);
	  /* Missing entry point, use function's name with the leading
	     char prepended.  */
	  if (leading_char)
	    asprintf (&label, "%c%s", leading_char, name);
	  else
	    label = name;
	}
      else
	{
	  ++input_line_pointer;
	  SKIP_WHITESPACE ();
	  label = input_line_pointer;
	  delim2 = get_symbol_end ();
	  label = xstrdup (label);
	  *input_line_pointer = delim2;
	}

      current_name = name;
      current_label = label;
    }
  demand_empty_rest_of_line ();
}

/* This function is called once, at assembler startup time.  It should
   set up all the tables, etc., that the MD part of the assembler will
   need.  */

void
md_begin ()
{
  const char *retval = NULL;
  int lose = 0;
  unsigned int i;

  /* Create a new hash table.  */
  op_hash = hash_new ();

  /* Hash up all the opcodes for fast use later.  */
  for (i = 0; i < num_logic32_opcodes; i++)
    {
      const char *name = machine_opcodes[i].name;

      retval = hash_insert (op_hash, name, (PTR) &machine_opcodes[i]);

      if (retval != NULL)
	{
	  fprintf (stderr, "internal error: can't hash `%s': %s\n",
		   machine_opcodes[i].name, retval);
	  lose = 1;
	}
    }

  if (lose)
    as_fatal (_("Broken assembler.  No assembly attempted."));

  define_some_regs ();
}

/* Assemble a single instruction.  Its label has already been handled
   by the generic front end.  We just parse opcode and operands, and
   produce the bytes of data and relocation.  */

void
md_assemble (str)
     char *str;
{
  char *toP;
  fixS *fixP;
  bit_fixS *bitP;

  know (str);
  insn_size = 4;
  machine_ip (str);
  toP = frag_more (insn_size);

  /* Put out the opcode.  */
  md_number_to_chars (toP, the_insn.opcode, 4);

  if ( insn_size == 8 ) {
	md_number_to_chars (toP+4, the_insn.opcode_ex, 4);
  }

  /* Put out the symbol-dependent stuff.  */
  if (the_insn.reloc != NO_RELOC)
    {
      fixP = fix_new_exp (frag_now,
			  (toP - frag_now->fr_literal + the_insn.reloc_offset),
			  the_insn.size, & the_insn.exp, the_insn.pcrel,
			  the_insn.reloc);

      /* Turn off complaints that the addend is
	 too large for things like foo+100000@ha.  */
      switch (the_insn.reloc)
	{
	case RELOC_LOGIC32_HI16:
	case RELOC_LOGIC32_LO16:
	  fixP->fx_no_overflow = 1;
	  break;
	default:
	  break;
	}

      switch (fixP->fx_r_type)
	{
/*
	case RELOC_LOGIC32_REL26:
	  bitP = malloc (sizeof (bit_fixS));
	  bitP->fx_bit_size = 26;
	  bitP->fx_bit_offset = 25;
	  bitP->fx_bit_base = the_insn.opcode & 0xFC000000;
	  bitP->fx_bit_base_adj = 0;
	  bitP->fx_bit_max = 0;
	  bitP->fx_bit_min = 0;
	  bitP->fx_bit_add = 0x03FFFFFF;
	  fixP->fx_bit_fixP = bitP;
	  break;
*/
	case RELOC_LOGIC32_REL24:
	  bitP = malloc (sizeof (bit_fixS));
	  bitP->fx_bit_size = 22;
	  bitP->fx_bit_offset = 21;
	  bitP->fx_bit_base = the_insn.opcode & 0xFFC00000;
	  bitP->fx_bit_base_adj = 0;
	  bitP->fx_bit_max = 0;
	  bitP->fx_bit_min = 0;
	  bitP->fx_bit_add = 0x003FFFFF;
	  fixP->fx_bit_fixP = bitP;
	  break;
	case RELOC_LOGIC32_JMP32:
	  bitP = malloc (sizeof (bit_fixS));
	  bitP->fx_bit_size = 30;
	  bitP->fx_bit_offset = 29;
	  bitP->fx_bit_base = the_insn.opcode & 0xC0000000;
	  bitP->fx_bit_base_adj = 0;
	  bitP->fx_bit_max = 0;
	  bitP->fx_bit_min = 0;
	  bitP->fx_bit_add = 0x3FFFFFFF;
	  fixP->fx_bit_fixP = bitP;
	  break;
	case RELOC_LOGIC32_LDI:
	  bitP = malloc (sizeof (bit_fixS));
	  bitP->fx_bit_size = 16;
	  bitP->fx_bit_offset = 15;
	  bitP->fx_bit_base = the_insn.opcode & 0xFFFF0000;
	  bitP->fx_bit_base_adj = 0;
	  bitP->fx_bit_max = 0;
	  bitP->fx_bit_min = 0;
	  bitP->fx_bit_add = 0x0000FFFF;
	  fixP->fx_bit_fixP = bitP;
	  break;
	case RELOC_LOGIC32_LO16:
	case RELOC_LOGIC32_REL16:
	  bitP = malloc (sizeof (bit_fixS));
	  bitP->fx_bit_size = 16;
	  bitP->fx_bit_offset = 15;
	  bitP->fx_bit_base = the_insn.opcode & 0xFFFF0000;
	  bitP->fx_bit_base_adj = 0;
	  bitP->fx_bit_max = 0;
	  bitP->fx_bit_min = 0;
	  bitP->fx_bit_add = 0x0000FFFF;
	  fixP->fx_bit_fixP = bitP;
	  break;
	case RELOC_LOGIC32_HI16:
	  bitP = malloc (sizeof (bit_fixS));
	  bitP->fx_bit_size = 16;
	  bitP->fx_bit_offset = 15;
	  bitP->fx_bit_base = the_insn.opcode & 0xFFFF0000;
	  bitP->fx_bit_base_adj = 0;
	  bitP->fx_bit_max = 0;
	  bitP->fx_bit_min = 0;
	  bitP->fx_bit_add = 0x0000FFFF;
	  fixP->fx_bit_fixP = bitP;
	  break;
	default:
	  fixP->fx_bit_fixP = (bit_fixS *)NULL;
	  break;
	}
    }
}

static int
hilo_modifier_ok (s)
     char *s;
{
  char *ptr = s;
  int   idx, count = 1;

  if (*ptr != '(')
    return 1;

  for (idx = 1; ptr[idx] != '\0' && ptr[idx] != '[' && idx < 73; idx += 1)
    {
      if (count == 0)
	return count;

      if (ptr[idx] == '(')
	count += 1;

      if (ptr[idx] == ')')
	count -= 1;
    }

  return (count == 0) ? 1:0;
}

char *
parse_operand (s, operandp)
     char *s;
     expressionS *operandp;
{
  char *save = input_line_pointer;
  char *new;

  the_insn.HI = the_insn.LO = 0;
  the_insn.ctrl_reg_nr = -1;

  /* Search for %hi and %lo, make a mark and skip it.  */
  if (strncmp (s, "%hi", 3) == 0)
    {
      s += 3;
      the_insn.HI = 1;
    }
  else
    {
      if (strncmp (s, "%lo", 3) == 0)
	{
	  s += 3;
	  the_insn.LO = 1;
	}
      else
	the_insn.LO = 0;
    }

  if (the_insn.HI || the_insn.LO)
    {
      if (!hilo_modifier_ok (s))
	as_bad (_("Expression Error for operand modifier %%hi/%%lo\n"));
    }

  if (strncmp (s, "$psw", 4) == 0)
    {
      s += 4;
      input_line_pointer = s;
	  the_insn.ctrl_reg_nr = 1;
	  operandp->X_add_number = 1;
      operandp->X_op = O_register;
    }
  else if (strncmp (s, "$epc", 4) == 0)
    {
      s += 4;
      input_line_pointer = s;
	  the_insn.ctrl_reg_nr = 4;
	  operandp->X_add_number = 4;
      operandp->X_op = O_register;
    }
  else if (strncmp (s, "$esr", 4) == 0)
    {
      s += 4;
      input_line_pointer = s;
	  the_insn.ctrl_reg_nr = 5;
	  operandp->X_add_number = 5;
      operandp->X_op = O_register;
    }
  /* Check for the % and $ register representation    */
  else if ((s[0] == '%' || s[0] == '$' || s[0] == 'r' || s[0] == 'R')
      && ISDIGIT ((unsigned char) s[1])
	  && ((!ISALNUM((unsigned char)s[2]) && s[2] != '_')
      || (ISDIGIT ((unsigned char) s[2]) && !ISALNUM((unsigned char)s[3]) && s[3] != '_'))
	  )
    {
      /* We have a numeric register expression.  No biggy.  */
      s += 1;
      input_line_pointer = s;
      (void) expression (operandp);
      if (operandp->X_op != O_constant
	  || operandp->X_add_number > 31)
	as_bad (_("Invalid expression after %%%%\n"));
      operandp->X_op = O_register;
    }
  else
    {
		int len, regnum;
		regnum = find_sft_register(s, &len);
		if ( regnum >= 0 ) {
			input_line_pointer = s + len;
			operandp->X_add_number = regnum;
			operandp->X_op = O_register;
		}
		else {
			/* Normal operand parsing.  */
			input_line_pointer = s;
			(void) expression (operandp);
		}
    }

  new = input_line_pointer;
  input_line_pointer = save;
  return new;
}

/* This function will check the opcode and return 1 if the opcode is one
   of the load/store instruction, and it will fix the operand string to
   the standard form so we can use the standard parse_operand routine.  */

#define READ_OP     0x100
#define WRITE_OP    0x200
static char iBuf[81];

static char *
logic32_parse_loadop (str)
     char * str;
{
  char *ptr = str;
  int   idx = 0;

  /* The last pair of ()/[] is the register, all other are the
     reloc displacement, and if there is a register then it ought
     to have a pair of ()/[]
     This is not necessarily true, what if the load instruction come
     without the register and with %hi/%lo modifier?  */
  for (idx = 0; idx < 72 && ptr[idx] != '\0'; idx++)
    ;

  if (idx == 72)
    {
    badoperand_load:
      as_bad (_("Bad operand for a load instruction: <%s>"), str);
      return NULL;
    }
  else
    {
      int i, pb = 0;
      int m2 = 0;
      char rs1[7], rd[7], endm, match = '0';
      char imm[72];

      idx -= 1;
      switch (str[idx])
	{
	case ')':
	  match = '(';
	  endm  = ')';
	  break;
	case ']':
	  match = '[';
	  endm  = ']';
	  break;
	default:
	  /* No register indicated, fill in zero.  */
	  rs1[0] = 'r';
	  rs1[1] = '0';
	  rs1[2] = '\0';
	  match  = 0;
	  endm = 0;
	  m2 = 1;
	}

      if (!m2)
	{
	  /* Searching for (/[ which will match the ]/).  */
	  for (pb = idx - 1; str[pb] != match; pb -= 1)
	    /* Match can only be either '[' or '(', if it is
	       '(' then this can be a normal expression, we'll treat
	       it as an operand.  */
	    if (str[pb] == endm || pb < (idx - 5))
	      goto load_no_rs1;
	  pb += 1;

	  for (i = 0; (pb + i) < idx; i++)
	    rs1[i] = str[pb+i];

	  rs1[i] = '\0';

	  if (is_ldst_registers (& rs1[0]))
	    /* Point to the last character of the imm.  */
	    pb -= 1;
	  else
	    {
	    load_no_rs1:
	      if (match == '[')
		goto badoperand_load;
	      /* No register indicated, fill in zero and restore the imm.  */
	      rs1[0] = 'r';
	      rs1[1] = '0';
	      rs1[2] = '\0';
	      m2 = 1;
	    }
	}

      /* Duplicate the first register.  */
      for (i = 0; i < 7 && str[i] != ','; i++)
	rd[i] = ptr[i];

      if (str[i] != ',')
	goto badoperand_load;
      else
	rd[i] = '\0';

      /* Copy the immd.  */
      if (m2)
	/* Put the '\0' back in.  */
	pb = idx + 1;

      for (i++, m2 = 0; i < pb; m2++,i++)
	imm[m2] = ptr[i];

      imm[m2] = '\0';

      /* Assemble the instruction to gas internal format.  */
      for (i = 0; rd[i] != '\0'; i++)
	iBuf[i] = rd[i];

      iBuf[i++] = ',';

      for (pb = 0 ; rs1[pb] != '\0'; i++, pb++)
	iBuf[i] = rs1[pb];

      iBuf[i++] = ',';

      for (pb = 0; imm[pb] != '\0'; i++, pb++)
	iBuf[i] = imm[pb];

      iBuf[i] = '\0';
      return iBuf;
    }
}

static char *
logic32_parse_storeop (str)
     char * str;
{
  char *ptr = str;
  int   idx = 0;

  /* Search for the ','.  */
  for (idx = 0; idx < 72 && ptr[idx] != ','; idx++)
    ;

  if (idx == 72)
    {
    badoperand_store:
      as_bad (_("Bad operand for a store instruction: <%s>"), str);
      return NULL;
    }
  else
    {
      /* idx now points to the ','.  */
      int i, pb = 0;
      int comma = idx;
      int m2 = 0;
      char rs1[7], rd[7], endm, match = '0';
      char imm[72];

      /* Now parse the '(' and ')', and make idx point to ')'.  */
      idx -= 1;
      switch (str[idx])
	{
	case ')':
	  match = '(';
	  endm  = ')';
	  break;
	case ']':
	  match = '[';
	  endm  = ']';
	  break;
	default:
	  /* No register indicated, fill in zero.  */
	  rs1[0] = 'r';
	  rs1[1] = '0';
	  rs1[2] = '\0';
	  match  = 0;
	  endm = 0;
	  m2 = 1;
	}

      if (!m2)
	{
	  /* Searching for (/[ which will match the ]/).  */
	  for (pb = idx - 1; str[pb] != match; pb -= 1)
	    if (pb < (idx - 5) || str[pb] == endm)
	      goto store_no_rs1;
	  pb += 1;

	  for (i = 0; (pb + i) < idx; i++)
	    rs1[i] = str[pb + i];

	  rs1[i] = '\0';

	  if (is_ldst_registers (& rs1[0]))
	    /* Point to the last character of the imm.  */
	    pb -= 1;
	  else
	    {
	    store_no_rs1:
	      if (match == '[')
		goto badoperand_store;

	      /* No register indicated, fill in zero and restore the imm.  */
	      rs1[0] = 'r';
	      rs1[1] = '0';
	      rs1[2] = '\0';
	      pb = comma;
	    }
	}
      else
	/* No register was specified.  */
	pb = comma;

      /* Duplicate the first register.  */
      for (i = comma + 1; (str[i] == ' ' || str[i] == '\t'); i++)
	;

      for (m2 = 0; (m2 < 7 && str[i] != '\0'); i++, m2++)
	{
	  if (str[i] != ' ' && str[i] != '\t')
	    rd[m2] = str[i];
	  else
	    goto badoperand_store;
	}

      if (str[i] != '\0')
	goto badoperand_store;
      else
	rd[m2] = '\0';

      /* Copy the immd.  */
      for (i = 0; i < pb; i++)
	imm[i] = ptr[i];

      imm[i] = '\0';

      /* Assemble the instruction to gas internal format.  */
      for (i = 0; rd[i] != '\0'; i++)
	iBuf[i] = rd[i];
      iBuf[i++] = ',';
      for (pb = 0 ; rs1[pb] != '\0'; i++, pb++)
	iBuf[i] = rs1[pb];
      iBuf[i++] = ',';
      for (pb = 0; imm[pb] != '\0'; i++, pb++)
	iBuf[i] = imm[pb];
      iBuf[i] = '\0';
      return iBuf;
    }
}

static char *
fix_ld_st_operand (opcode, str)
     unsigned long opcode;
     char* str;
{
  /* Check the opcode.  */
  switch ((int) opcode)
    {
    case  OP_LDW:
    case  OP_LDH:
    case  OP_LDB:
    case  OP_LDBU:
    case  OP_LDHU:
      return logic32_parse_loadop (str);
    case  OP_STW:
    case  OP_STH:
    case  OP_STB:
      return logic32_parse_storeop (str);
    default:
      return str;
    }
}

/* Instruction parsing.  Takes a string containing the opcode.
   Operands are at input_line_pointer.  Output is in the_insn.
   Warnings or errors are generated.  */

static void
machine_ip (str)
     char *str;
{
  char *s;
  const char *args;
  struct machine_opcode *insn;
  char *argsStart;
  unsigned long opcode;
  expressionS the_operand;
  expressionS *operand = &the_operand;
  unsigned int reg, reg_shift = 0;

  /* Fixup the opcode string to all lower cases, and also
     allow numerical digits.  */
  s = str;

#if 0	// LJL
  if (ISALPHA (*s))
    for (; ISALNUM (*s); ++s)
      if (ISUPPER (*s))
	*s = TOLOWER (*s);
#else
  if (ISALPHA (*s) || *s == '.' ) {
    for (; ISALNUM (*s) || *s == '.' ; ++s) {
      if (ISUPPER (*s)) {
			*s = TOLOWER (*s);
	  }
	}
  }
#endif

#if 0
	puts(str);	// mark
#endif

  switch (*s)
    {
    case '\0':
      break;

      /* FIXME-SOMEDAY more whitespace.  */
    case ' ':
      *s++ = '\0';
      break;

    default:
      as_bad (_("Unknown opcode: `%s'"), str);
      return;
    }

  /* Hash the opcode, insn will have the string from opcode table.
     also initialized the_insn struct.  */
  if ((insn = (struct machine_opcode *) hash_find (op_hash, str)) == NULL)
    {
      /* Handle the ret and return macro here.  */
      if ((strcmp (str, "ret") == 0) || (strcmp (str, "return") == 0))
	{
	  memset (&the_insn, '\0', sizeof (the_insn));
	  the_insn.reloc = NO_RELOC;
	  the_insn.pcrel = 0;
	  the_insn.opcode =
	    (unsigned long)(OP_JMPR | (31<<21));
	}
      else
	as_bad (_("Unknown opcode `%s'."), str);

      return;
    }

  argsStart = s;
  opcode = insn->opcode;
  if ( opcode == OP_ALUR || opcode == OP_FPU || opcode == OP_MISC ) {
	opcode |= insn->function;
  }
  else if ( opcode == OP_BRN ) {
	opcode |= (insn->function<<22);
  }
  memset (&the_insn, '\0', sizeof (the_insn));
  the_insn.reloc = NO_RELOC;
  the_insn.pcrel = 0;

  /* Set the sip reloc HI16 flag.  */
  if (!set_logic32_skip_hi16_flag (1))
    as_bad (_("Can not set logic32_skip_hi16_flag"));

  /* Fix the operand string if it is one of load store instructions.  */
  s = fix_ld_st_operand (opcode, s);

  /* Build the opcode, checking as we go to make sure that the
     operands match.
     If an operand matches, we modify the_insn or opcode appropriately,
     and do a "continue".  If an operand fails to match, we "break".  */
  if (insn->args[0] != '\0' && insn->args[0] != 'N')
    {
      /* Prime the pump.  */
      if (*s == '\0')
	{
	  as_bad (_("Missing arguments for opcode <%s>."), str);
	  return;
	}
      else
	s = parse_operand (s, operand);
    }
  else if (insn->args[0] == 'N')
    {
      /* Clean up the insn and done!  */
      the_insn.opcode = opcode;
      return;
    }

  /* Parse through the args (this is from opcode table), *s point to
     the current character of the instruction stream.  */
  for (args = insn->args;; ++args)
    {
      switch (*args)
	{
	  /* End of Line.  */
	case '\0':
	  /* End of args.  */
	  if (*s == '\0')
	    {
	      /* We are truly done.  */
	      the_insn.opcode = opcode;
	      /* Clean up the HI and LO mark.  */
	      the_insn.HI = 0;
	      the_insn.LO = 0;
	      return;
	    }

	  the_insn.HI = 0;
	  the_insn.LO = 0;
	  as_bad (_("Too many operands: %s"), s);
	  break;

	  /* ',' Args separator */
	case ',':
	  /* Must match a comma.  */
	  if (*s++ == ',')
	    {
	      /* Parse next operand.  */
	      s = parse_operand (s, operand);
	      continue;
	    }
	  break;

	  /* It can be a 'c' register or 'i' operand.  */
	case 'P':
	  /* Macro move operand/reg.  */
	  if (operand->X_op == O_register)
	    {
	      /* Its a register.  */
	      reg_shift = 11;
	      goto general_reg;
	    }

	  /* The immediate 16 bits literal, bit 0-15.  */
	case 'i':
	  /* offset, unsigned.  */
	case 'I':
	  /* offset, signed.  */
	  if (operand->X_op == O_constant)
	    {
	      if (the_insn.HI)
			operand->X_add_number >>= 16;

	      opcode |= operand->X_add_number & 0xFFFF;

	      if (the_insn.HI && the_insn.LO)
			as_bad (_("Both the_insn.HI and the_insn.LO are set : %s"), s);
	      else
		{
		  the_insn.HI = 0;
		  the_insn.LO = 0;
		}
	      continue;
	    }
		else if (operand->X_op == O_absent) {
	      continue;
		}
		as_bad (_("syntax error!"));	// LJL
/*
	  the_insn.reloc        = (the_insn.HI) ? RELOC_LOGIC32_HI16 
	    : (the_insn.LO ? RELOC_LOGIC32_LO16 : RELOC_LOGIC32_16);
	  the_insn.reloc_offset = 0;
	  the_insn.size         = 2;
	  the_insn.pcrel        = 0;
	  the_insn.exp          = * operand;
	  the_insn.HI           = 0;
	  the_insn.LO           = 0;
*/
	  continue;

	case 'd':
	  /* offset, signed.  */
	  if (operand->X_op == O_constant)
	    {
	      opcode |= operand->X_add_number & 0xFFFF;
	      continue;
	    }
	  the_insn.reloc        = RELOC_LOGIC32_REL16;
	  the_insn.reloc_offset = 0;    /* BIG-ENDIAN Byte 3 of insn.  */
	  the_insn.size         = 4;
	  the_insn.pcrel        = 1;
	  the_insn.exp          = *operand;
	  continue;

#if 0
	  /* The immediate 26 bits literal, bit 0-25.  */
	case 'D':
	  /* offset, signed.  */
	  if (operand->X_op == O_constant)
	    {
	      opcode |= operand->X_add_number & 0x3FFFFFF;
	      continue;
	    }
	  the_insn.reloc = RELOC_LOGIC32_REL26;
	  the_insn.reloc_offset = 0;    /* BIG-ENDIAN Byte 3 of insn.  */
	  the_insn.size  = 4;
	  the_insn.pcrel = 1;
	  the_insn.exp = *operand;
	  continue;
#endif

	  /* The immediate 24 bits literal, bit 0-23.  */
	case 'R':
	  /* offset, signed.  */
	  if (operand->X_op == O_constant)
	    {
			opcode |= operand->X_add_number & 0x03FFFFF;
			continue;
	    }
	  the_insn.reloc = RELOC_LOGIC32_REL24;
	  the_insn.reloc_offset = 0;    /* BIG-ENDIAN Byte 3 of insn.  */
	  the_insn.size  = 4;
	  the_insn.pcrel = 1;
	  the_insn.exp = *operand;
	  continue;

	  /* The immediate 32 bits literal, bit 0-29.  */
	case 'J':
	  /* offset, signed.  */
	  if (operand->X_op == O_constant)
	    {
			if ( operand->X_add_number & 3 ) {
				as_bad (_("jump to misaligned address (0x%lx)"), operand->X_add_number);	// LJL
			}
			else {
				opcode |= (operand->X_add_number>>2) & 0x3FFFFFFF;
			}
			continue;
	    }
	  the_insn.reloc = RELOC_LOGIC32_JMP32;
	  the_insn.reloc_offset = 0;    /* BIG-ENDIAN Byte 3 of insn.  */
	  the_insn.size  = 4;
	  the_insn.pcrel = 0;
	  the_insn.exp = *operand;
	  continue;

	  /* The immediate 32 bits literal  */
	case 'K':
	  /* offset, signed.  */
	  if (operand->X_op == O_constant)
	    {
			if ( (operand->X_add_number & 0xffff0000) == 0 ) {
				opcode |= operand->X_add_number;
			}
			else {
				int rd_reg = (opcode>>21)&0x1f;
				opcode &= ~(0xfc000000); 
				opcode |= insn->function;					// LDIH
				opcode |= ((operand->X_add_number>>16)&0xffff);
				the_insn.opcode_ex = insn->opcode;			// ORI
				the_insn.opcode_ex = the_insn.opcode_ex | (rd_reg<<21) | (rd_reg<<16);
				the_insn.opcode_ex |= (operand->X_add_number&0xffff);
				insn_size = 8;
			}
			continue;
	    }
{
		int rd_reg = (opcode>>21)&0x1f;
		opcode &= ~(0xfc000000); 
		opcode |= insn->function;					// LDIH
		opcode |= ((operand->X_add_number>>16)&0xffff);
		the_insn.opcode_ex = insn->opcode;			// ORI
		the_insn.opcode_ex = the_insn.opcode_ex | (rd_reg<<21) | (rd_reg<<16);
		the_insn.opcode_ex |= (operand->X_add_number&0xffff);
}
	  insn_size = 8;
	  the_insn.reloc = RELOC_LOGIC32_LDI;
	  the_insn.reloc_offset = 0;    /* LITTLE-ENDIAN Byte 0 of insn.  */
	  the_insn.size  = 8;
	  the_insn.pcrel = 0;
	  the_insn.exp = *operand;
	  continue;
	  /* Type 'a' Register.  */
	case 'a':
	  /* A general register at bits 21-25, rs1.  */
	  reg_shift = 21;
	  goto general_reg;

	  /* Type 'b' Register.  */
	case 'b':
	  /* A general register at bits 16-20, rs2/rd.  */
	  reg_shift = 16;
	  goto general_reg;

	  /* Type 'c' Register.  */
	case 'c':
	  /* A general register at bits 11-15, rd.  */
	  reg_shift = 11;
	  goto general_reg;

	  /* Type 'x' Register.  */
	case 'x':
	  /* A control register at bits 21-25.  */
	  reg_shift = 21;
	  goto general_reg;

	  /* Type 'y' Register.  */
	case 'y':
	  /* A control register at bits 16-20.  */
	  reg_shift = 16;
	  goto general_reg;

	general_reg:
	  know (operand->X_add_symbol == 0);
	  know (operand->X_op_symbol == 0);
#if 1	// LJL
	if (operand->X_op != O_register) {
		as_bad ("Invalid operand, expect a reigster.");
	}
#endif
	  reg = operand->X_add_number;
	  if (reg & 0xffffffe0)
	    as_fatal (_("failed regnum sanity check."));
	  else
	    /* Got the register, now figure out where it goes in the opcode.  */
	    opcode |= reg << reg_shift;

	  switch (*args)
	    {
	    case 'a':
	    case 'b':
	    case 'c':
	    case 'P':
	    case 'x':
	    case 'y':
	      continue;
	    }
	  as_fatal (_("failed general register sanity check."));
	  break;

	default:
	  BAD_CASE (*args);
	}

      /* Types or values of args don't match.  */
      as_bad ("Invalid operands");
      return;
    }
}

/* This is identical to the md_atof in m68k.c.  I think this is right,
   but I'm not sure.

   Turn a string in input_line_pointer into a floating point constant
   of type TYPE, and store the appropriate bytes in *LITP.  The number
   of LITTLENUMS emitted is stored in *SIZEP.  An error message is
   returned, or NULL on OK.  */
/* logic32 will not use it anyway, so I just leave it here for now.  */

/* Equal to MAX_PRECISION in atof-ieee.c.  */
#define MAX_LITTLENUMS 6

char *
md_atof (type, litP, sizeP)
     char type;
     char *litP;
     int *sizeP;
{
  int prec;
  LITTLENUM_TYPE words[MAX_LITTLENUMS];
  LITTLENUM_TYPE *wordP;
  char *t;

  switch (type)
    {
    case 'f':
    case 'F':
    case 's':
    case 'S':
      prec = 2;
      break;

    case 'd':
    case 'D':
    case 'r':
    case 'R':
      prec = 4;
      break;

    case 'x':
    case 'X':
      prec = 6;
      break;

    case 'p':
    case 'P':
      prec = 6;
      break;

    default:
      *sizeP = 0;
      return "Bad call to MD_ATOF()";
    }

  t = atof_ieee (input_line_pointer, type, words);
  if (t)
    input_line_pointer = t;

  *sizeP = prec * sizeof (LITTLENUM_TYPE);

  for (wordP = words; prec--;)
    {
      md_number_to_chars (litP, (valueT) (*wordP++), sizeof (LITTLENUM_TYPE));
      litP += sizeof (LITTLENUM_TYPE);
    }

  return 0;
}

/* Write out little-endian.  */
void
md_number_to_chars (buf, val, n)
     char *buf;
     valueT val;
     int n;
{
  number_to_chars_littleendian (buf, val, n);
}

/* md_chars_to_number:  convert from target byte order to host byte order.  */

int
md_chars_to_number (val, n)
     unsigned char *val;	/* Value in target byte order.  */
     int n;			/* Number of bytes in the input.  */
{
  int retval;

  for (retval = 0; n--;)
    {
      retval <<= 8;
      retval |= val[n];
    }

  return retval;
}

bfd_boolean
md_logic32_fix_adjustable (fixP)
   fixS *fixP;
{
  /* We need the symbol name for the VTABLE entries.  */
  return (fixP->fx_r_type != BFD_RELOC_VTABLE_INHERIT
          && fixP->fx_r_type != BFD_RELOC_VTABLE_ENTRY);
}

void
md_apply_fix3 (fixP, valP, seg)
     fixS *fixP;
     valueT *valP;
     segT seg ATTRIBUTE_UNUSED;
{
  long val = *valP;
  long val2 = 0;
  char *place = fixP->fx_where + fixP->fx_frag->fr_literal;

  switch (fixP->fx_r_type)
    {
    case RELOC_LOGIC32_LO16:
    case RELOC_LOGIC32_REL16:
      if (fixP->fx_bit_fixP != (bit_fixS *) NULL)
	{
	  val = (val & 0x0000FFFF) | fixP->fx_bit_fixP->fx_bit_base;
	  free (fixP->fx_bit_fixP);
	  fixP->fx_bit_fixP = (bit_fixS *) NULL;
	}
#ifdef DEBUG
      else
	know ((fixP->fx_bit_fixP != (bit_fixS *) NULL));
#endif
      break;

    case RELOC_LOGIC32_HI16:
      if (fixP->fx_bit_fixP != (bit_fixS *) NULL)
	{
	  val = (val >> 16) | fixP->fx_bit_fixP->fx_bit_base;
	  free (fixP->fx_bit_fixP);
	  fixP->fx_bit_fixP = (bit_fixS *)NULL;
	}
#ifdef DEBUG
      else
	know ((fixP->fx_bit_fixP != (bit_fixS *) NULL));
#endif
      break;

#if 0
    case RELOC_LOGIC32_REL26:
      if (fixP->fx_bit_fixP != (bit_fixS *) NULL)
	{
		if ( val%4 == 0 ) {
			int32_t tmp = val;
			val = tmp/4;
		}
		else {
			// error
			as_bad("@ %s: %d\n", __FILE__, __LINE__);	// LJL
			val = 0;
		}
		val = (val & 0x3FFFFFFF) | fixP->fx_bit_fixP->fx_bit_base;
		free (fixP->fx_bit_fixP);
		fixP->fx_bit_fixP = (bit_fixS *) NULL;
	}
#ifdef DEBUG
      else
	know ((fixP->fx_bit_fixP != (bit_fixS *) NULL));
#endif
      break;
#endif

    case RELOC_LOGIC32_REL24:	// LJL
      if (fixP->fx_bit_fixP != (bit_fixS *) NULL)
	{
		if ( val%4 == 0 ) {
			int32_t tmp = val;
			val = tmp/4;
		}
		else {
			// error
			as_bad("@ %s: %d\n", __FILE__, __LINE__);	// LJL
			val = 0;
		}
		val = (val & 0x003FFFFF) | fixP->fx_bit_fixP->fx_bit_base;	// LJL mark
		free (fixP->fx_bit_fixP);
		fixP->fx_bit_fixP = (bit_fixS *) NULL;
	}
#ifdef DEBUG
      else
	know ((fixP->fx_bit_fixP != (bit_fixS *) NULL));
#endif
      break;
	case RELOC_LOGIC32_JMP32:
		if (fixP->fx_bit_fixP != (bit_fixS *) NULL)
		{
			if ( val%4 == 0 ) {
				int32_t tmp = val;
				val = tmp/4;
			}
			else {
				// error
				as_bad("@ %s: %d\n", __FILE__, __LINE__);	// LJL
				val = 0;
			}
			val = (val & 0x3FFFFFFF) | fixP->fx_bit_fixP->fx_bit_base;
			free (fixP->fx_bit_fixP);
			fixP->fx_bit_fixP = (bit_fixS *) NULL;
		}
		#ifdef DEBUG
		else {
			know ((fixP->fx_bit_fixP != (bit_fixS *) NULL));
		}
		#endif
		break;

	case RELOC_LOGIC32_LDI:
		if (fixP->fx_bit_fixP != (bit_fixS *) NULL)
		{
			val2 = (val & 0x0000FFFF);
			val = ((val>>16) & 0x0000FFFF) | fixP->fx_bit_fixP->fx_bit_base;
			free (fixP->fx_bit_fixP);
			fixP->fx_bit_fixP = (bit_fixS *) NULL;
		}
		#ifdef DEBUG
		else {
			know ((fixP->fx_bit_fixP != (bit_fixS *) NULL));
		}
		#endif
		break;

    case BFD_RELOC_VTABLE_INHERIT:
      /* This borrowed from tc-ppc.c on a whim.  */
      fixP->fx_done = 0;
      if (fixP->fx_addsy
	  && !S_IS_DEFINED (fixP->fx_addsy)
	  && !S_IS_WEAK (fixP->fx_addsy))
	S_SET_WEAK (fixP->fx_addsy);
      return;

    case BFD_RELOC_VTABLE_ENTRY:
      fixP->fx_done = 0;
      return;

    default:
      break;
    }

  if (fixP->fx_size == 8) {
	number_to_chars_littleendian (place, val, 4);
	number_to_chars_littleendian (place+4, val2, 2);
  }
  else {
	number_to_chars_littleendian (place, val, fixP->fx_size);
  }
  if (fixP->fx_addsy == NULL)
    fixP->fx_done = 1;
}

const char *md_shortopts = "";

struct option md_longopts[] =
  {
    {NULL, no_argument, NULL, 0}
  };

size_t md_longopts_size = sizeof (md_longopts);

int
md_parse_option (c, arg)
     int c     ATTRIBUTE_UNUSED;
     char *arg ATTRIBUTE_UNUSED;
{
  return 0;
}

void
md_show_usage (stream)
     FILE *stream ATTRIBUTE_UNUSED;
{
}

/* This is called when a line is unrecognized.  */

int
logic32_unrecognized_line (c)
     int c;
{
  int lab;
  char *s;

  if (c != '$' || ! ISDIGIT ((unsigned char) input_line_pointer[0]))
    return 0;

  s = input_line_pointer;

  lab = 0;
  while (ISDIGIT ((unsigned char) *s))
    {
      lab = lab * 10 + *s - '0';
      ++s;
    }

  if (*s != ':')
    {
      /* Not a label definition.  */
      return 0;
    }

  if (dollar_label_defined (lab))
    {
      as_bad (_("label \"$%d\" redefined"), lab);
      return 0;
    }

  define_dollar_label (lab);
  colon (dollar_label_name (lab, 0));
  input_line_pointer = s + 1;

  return 1;
}

/* Default the values of symbols known that should be "predefined".  We
   don't bother to predefine them unless you actually use one, since there
   are a lot of them.  */

symbolS *
md_undefined_symbol (name)
     char *name ATTRIBUTE_UNUSED;
{
  return NULL;
}


/* Parse an operand that is machine-specific, the function was called
   in expr.c by operand() function, when everything failed before it
   call a quit.  */

void
md_operand (expressionP)
     expressionS* expressionP;
{
  /* Check for the #number representation    */
  if (input_line_pointer[0] == '#' &&
      ISDIGIT ((unsigned char) input_line_pointer[1]))
    {
      /* We have a numeric number expression.  No biggy.  */
      input_line_pointer += 1;	/* Skip # */

      (void) expression (expressionP);

      if (expressionP->X_op != O_constant)
	as_bad (_("Invalid expression after # number\n"));
    }

  return;
}

/* Round up a section size to the appropriate boundary.  */

valueT
md_section_align (segment, size)
     segT segment ATTRIBUTE_UNUSED;
     valueT size;
{
  /* Byte alignment is fine.  */
  return size;
}

/* Exactly what point is a PC-relative offset relative TO?
   On the 29000, they're relative to the address of the instruction,
   which we have set up as the address of the fixup too.  */

long
md_pcrel_from (fixP)
     fixS* fixP;
{
  //return 4 + fixP->fx_where + fixP->fx_frag->fr_address;
  return fixP->fx_where + fixP->fx_frag->fr_address;
}

/* Translate internal representation of relocation info to BFD target
   format.
   FIXME: To what extent can we get all relevant targets to use this?
   The above FIXME is from a29k, but I think it is also needed here.    */

arelent *
tc_gen_reloc (section, fixP)
     asection *section ATTRIBUTE_UNUSED;
     fixS *fixP;
{
  arelent * reloc;

  reloc = (arelent *) xmalloc (sizeof (arelent));
  reloc->howto = bfd_reloc_type_lookup (stdoutput, fixP->fx_r_type);

  if (reloc->howto == (reloc_howto_type *) NULL)
    {
      as_bad_where (fixP->fx_file, fixP->fx_line,
		    "internal error: can't export reloc type %d (`%s')",
		    fixP->fx_r_type,
		    bfd_get_reloc_code_name (fixP->fx_r_type));
      return NULL;
    }

  assert (!fixP->fx_pcrel == !reloc->howto->pc_relative);

  reloc->sym_ptr_ptr = (asymbol **) xmalloc (sizeof (asymbol *));
  *reloc->sym_ptr_ptr = symbol_get_bfdsym (fixP->fx_addsy);
  reloc->address = fixP->fx_frag->fr_address + fixP->fx_where;

  if (fixP->fx_r_type == BFD_RELOC_VTABLE_ENTRY)
    reloc->address = fixP->fx_offset;
  reloc->addend = 0;

  return reloc;
}

extern void pop_insert PARAMS ((const pseudo_typeS *));

void
logic32_pop_insert ()
{
  pop_insert (logic32_pseudo_table);
  return ;
}
