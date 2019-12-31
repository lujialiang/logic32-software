/* Prototypes for exported functions defined in m68hc11.c
   Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.
   Contributed by Stephane Carrez (stcarrez@nerim.fr)

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */



extern void logic32_expand_move (enum machine_mode, rtx, rtx);
void logic32_print_operand (FILE *file, rtx op, int letter);
int hard_register_operand (rtx op, enum machine_mode mode);
int soft_register_operand (rtx op, enum machine_mode mode);
int hard_memory_operand (rtx op, enum machine_mode mode);
int stack_pointer_operand (rtx op, enum machine_mode mode);
int any_operand (rtx op, enum machine_mode mode);
int any_nonimmediate_operand (rtx op, enum machine_mode mode);
int any_operator (rtx op, enum machine_mode mode);

int address_plus_operand (register rtx op, enum machine_mode mode);
const char *generate_jump(rtx loc, rtx insn);
#ifdef RTX_CODE
const char *generate_branch(enum rtx_code code, rtx loc, rtx insn);
#endif

const char *logic32_generate_move(rtx operands[], rtx insn);
const char *logic32_changestackpointer(rtx operands[], rtx insn);
const char *logic32_generate_if_then_else(rtx operands[], rtx insn);
const char *logic32_generate_if_zero_then_else(rtx operands[], rtx insn);
const char *logic32_generate_if_not_zero_then_else(rtx operands[], rtx insn);
const char *logic32_generate_reload(rtx operands[], rtx insn);
void logic32_prologue(FILE *file, HOST_WIDE_INT size);
void logic32_epilogue(FILE *file, HOST_WIDE_INT size);
const char *logic32_generate_binop(rtx operands[], rtx insn, const char *binop);
int logic32_legitimate_address(enum machine_mode mode, rtx x, int strict);
const char *logic32_generate_conditional_move(rtx operands[], rtx insn);
const char *logic32_jump(rtx operands[], rtx insn);
const char *logic32_jump_pcrel(rtx operands[], rtx insn);
const char *logic32_push(rtx operands[], rtx insn);
int logic32_nonimmediate(rtx op, enum machine_mode mode);
int logic32_initial_elimination_offset (int from, int to);
void logic32_expand_epilogue (void);
void logic32_expand_prologue (void);
rtx logic32_return_addr_rtx(void);
const char *logic32_call(rtx operands[], rtx insn);
const char *logic32_call_value(rtx operands[], rtx insn);

void logic32_start_asm(FILE *file);
const char * logic32_asm_multi(const char *format, rtx *operands, int byteCount);
int operand_check (char c, rtx op);
const char *logic32_table_jump(rtx operands[], rtx insn);

void logic32_expand_set(enum machine_mode mode, rtx operands[]);
const char *logic32_call(rtx operands[], rtx insn);
int logic32_binary_operator (rtx op  ATTRIBUTE_UNUSED, enum machine_mode mode ATTRIBUTE_UNUSED);
int logic32_unary_operator (rtx op  ATTRIBUTE_UNUSED, enum machine_mode mode ATTRIBUTE_UNUSED);
int logic32_return_pops_args (tree fundecl ATTRIBUTE_UNUSED, tree funtype, int size);
extern bool logic32_extra_constraint (rtx, int);
const char *logic32_generate_set(rtx insn);
int
move_double_dest_operand (rtx op, enum machine_mode mode);
int
move_double_src_operand (rtx op, enum machine_mode mode);
rtx
gen_split_move_double (rtx operands[]);




bool logic32_regno_mode_ok_for_base_p(int regno, enum machine_mode mode, bool strict_p);

enum logic32_address_type
{
	ADDRESS_REG,
	BASE_ADDRESS_REG,
	ADDRESS_CONST_INT,
	ADDRESS_SYMBOLIC
};

struct logic32_address_info
{
	enum logic32_address_type type;
	rtx reg;
	rtx offset;
	rtx base_rtx;
};

bool logic32_classify_address(struct logic32_address_info *addr_info, enum machine_mode mode, rtx addr_x, bool strict_p);

void logic32_output_function_prologue(FILE *file, HOST_WIDE_INT size);

void logic32_output_function_epilogue(FILE *file, HOST_WIDE_INT size);

#ifndef Mmode
#define Mmode enum machine_mode
#endif

int call_operand (rtx operand, Mmode mode ATTRIBUTE_UNUSED);

void logic32_init_cumulative_args(CUMULATIVE_ARGS *cum, tree funtype, rtx libname, tree fndecl);

void logic32_function_arg_advance(CUMULATIVE_ARGS *cum, enum machine_mode mode, tree type, int named);

rtx logic32_function_arg (CUMULATIVE_ARGS *cum, enum machine_mode omode, tree type, int named);

int logic32_arith_reg_operand (rtx op, enum machine_mode mode);

int call_address_operand (rtx op, enum machine_mode mode);

int call_operand (rtx op, enum machine_mode mode);

int uns_arith_operand (rtx op, enum machine_mode mode);

int arith_operand (rtx op, enum machine_mode mode);

int register_or_const0_operand (rtx op, enum machine_mode mode);

rtx logic32_return_addr (int count, rtx frame);
