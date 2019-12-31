/* Definitions for the c6x target.

   Copyright (C) 2004 Jan Parthey

   This file is part of a TI TMS320 C6x target implementation for GCC, which
   shall be called "GCC C6x target" for the purposes of this statement.

   "GCC C6x target" is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   "GCC C6x target" is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with "GCC C6x target"; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#include "config.h"
#include "system.h"
#include "rtl.h"
#include "tree.h"
#include "tm_p.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "real.h"
#include "insn-config.h"
#include "conditions.h"
#include "output.h"
#include "insn-attr.h"
#include "flags.h"
#include "except.h"
#include "function.h"
#include "recog.h"
#include "expr.h"
#include "optabs.h"
#include "toplev.h"
#include "basic-block.h"
#include "ggc.h"
#include "target.h"
#include "target-def.h"
#include "langhooks.h"

/**** node: "Target Structure"
 * */

struct gcc_target targetm = TARGET_INITIALIZER;

/* Place for storing away the operands of the `cmpM' standard action. For
 * immediate use in `bCOND' and fellows. See info gccint, node "Standard
 * Names", `bCOND'. */
rtx c6x_compare_op0 = NULL_RTX;
rtx c6x_compare_op1 = NULL_RTX;


/* - modelled to some extend on `ix86_comparison_operator' 
 * - <op> is supposed to be of the form 
 *   (<comparison-type> (reg:<cc-mode> <creg>) (const_int 0)), which is 
 *   among others used as part of 
 *   (set (pc) (if_then_else <op> (label_ref <ref-no>) (pc)))
 *   for expressing a conditional jump. Note: eg. for i386, the comparison
 *   against zero is not to be understood literally, it just codes for using
 *   the flags (zero, sign, overflow, ...) in <creg> to form the condition of
 *   the jump.
 * - TODO: I haven't figured out precisely the significance of the CC modes for
 *   mechanisms moving instructions (insn scheduling?). Such mechanisms might
 *   change the condition of branch instruction following the cmp. For now, I
 *   assume it's not wrong to introduce CCEQ, CCGT, CCGTU, CCLT, and CCLTU in
 *   c6x-modes.def, corresponding to the cmp<x> instructions on the c6x and
 *   to use other md files as a model.
 * - return: 1 if OP is a valid comparison operator in valid mode.  */
int
c6x_comparison_operator (op, mode)
     register rtx op ATTRIBUTE_UNUSED;
     enum machine_mode mode ATTRIBUTE_UNUSED;
{
#if 0
REMOVE THIS FUNCTION AS IT IS NOT NEEDED ANYMORE
  enum machine_mode inmode;
  enum rtx_code code = GET_CODE (op);
  if (mode != VOIDmode && GET_MODE (op) != mode)
    return 0;
  /* <op> must be a comparison operator */
  if (GET_RTX_CLASS (code) != '<')     
    return 0;
  /* get <creg>'s mode */
  inmode = GET_MODE (XEXP (op, 0));    

  switch (code)
    {
    case EQ: case NE:
      return (inmode == CCEQmode);
    case GT: case LE:
      return (inmode == CCGTmode);
    case LT: case GE:
      return (inmode == CCLTmode);
    case GTU: case LEU:
      return (inmode == CCGTUmode);
    case LTU: case GEU:
      return (inmode == CCLTUmode);
    default:
      return 0;
    }
#endif
      return 0;
}

/* - modelled on `ix86_expand_branch' 
 * - <code> is EQ, NE, GE, etc.
 * TODO
 * allocate a new pseudo reg number <preg> and generate RTL insns with patterns:
 * */
void
c6x_expand_branch (code, label)
     enum rtx_code code ATTRIBUTE_UNUSED;
     rtx label ATTRIBUTE_UNUSED;
{
#if 0
  rtx predreg;		      /* predicate register */ 
  rtx tmp;
  enum machine_mode cmpmode;

  cmpmode = SELECT_CC_MODE (code, c6x_compare_op0, c6x_compare_op1);
  predreg = gen_reg_rtx (cmpmode);

  switch (GET_MODE (c6x_compare_op0))
    {
    case SImode:
      /* 1. (set (reg:<cmpmode> <predreg>) 
       *         (compare:<cmpmode> <c6x_compare_op0>, <c6x_compare_op1>)) */
      tmp = gen_rtx_COMPARE (cmpmode, c6x_compare_op0, c6x_compare_op1);
      emit_insn (gen_rtx_SET (VOIDmode, predreg, tmp));
      /* 2. (set (pc) (if_then_else (<code> (reg:CCEQ <preg>) (const_int 0))) 
       *              (label_ref (match_operand 0 "" ""))
       *	      (pc)) */
      tmp = gen_rtx_fmt_ee (code, VOIDmode, predreg, const0_rtx);
      tmp = gen_rtx_IF_THEN_ELSE (VOIDmode, 
				  tmp,
				  gen_rtx_LABEL_REF (VOIDmode, label),
				  pc_rtx);
      emit_jump_insn (gen_rtx_SET (VOIDmode, pc_rtx, tmp));
      return;

    default:
      abort ();
    }
#endif
}


/* Emit comparison instruction if necessary, returning the expression that
 * holds the compare result in the proper mode.
 *
 * Modelled after `ia64_expand_compare'. */

rtx
c6x_expand_compare (code, mode)
     enum rtx_code code;
     enum machine_mode mode ATTRIBUTE_UNUSED;
{
  rtx op0 = c6x_compare_op0, op1 = c6x_compare_op1;
  rtx predreg;
  /* normally NE, but EQ if the condition is to be reversed */
  enum rtx_code predcode = NE;

  /* There are no mnemonics for `ne', `ge', `le', `geu', `leu' on c6x, but we
   * handle them by emitting the reversed `code' (ie. `lt' instead of `ge')
   * and, correspondingly, EQ rather than NE as `predcode', which causes the
   * jump to be conditionalized by an inverted predicate register, eg. [!B0].
   */
  if (code == NE || code == GE || code == LE || code == GEU || code == LEU)
    {
      code = reverse_condition (code);
      predcode = EQ;
    }

  /* Emit an insn with pattern (SET (REG:BI predreg) (code:BI op0 op1)),
   * where `code' is something of GET_RTX_CLASS == '<', eg.: `GE'.
   * Note that there must be appropriate `define_insn's in the .md file,
   * recognizing (set ...) for all `rtx_code's in class '<'.  */
  predreg = gen_reg_rtx (BImode);
  emit_insn (gen_rtx_SET (VOIDmode, predreg,
			  gen_rtx_fmt_ee (code, BImode, op0, op1)));

  /* Conditional branching (see define_expand "bge" etc.) is represented as
   * `(set (pc) (if_then_else (COMPARISON) (label_ref LABEL) (pc)))'.
   * On c6x, COMPARISON must be of the form (ne PREDREG const0_rtx) in order
   * to be recognized.  
   *
   * The rationale of this is the fact that the predreg will have been set to
   * the result of the comparison by the insn emitted above.  So we want to
   * jump to the true-label iff predreg is != 0, which is expressed by the
   * notation of COMPARISON.
   *
   * On c6x, we have no `jge' command available, but only an unconditional
   * branch `b <reg>'.  In order to get conditional branching, we have to
   * use predicate registers, which we initialize by computing comparisons
   * into them.
   */
  return gen_rtx_fmt_ee (predcode, BImode, predreg, const0_rtx);
}


/* This implements `GO_IF_LEGITIMATE_ADDRESS' for c6x.  
 *
 * We accept as legitimate address:
 * - a single (REG)
 * - a (PLUS (REG) (CONST_INT ucst5)
 * - a (PLUS (REG) (REG))
 * Anything else is forced to a register by function like `memory_address'.
 *
 * `GO_IF_LEGITIMATE_ADDRESS' is used, for example, by
 * `strict_memory_address_p', which is used in `find_reloads_address' for
 * checking whether a reload is needed.  */
int
c6x_legitimate_address_p (mode, x, strict)
     enum machine_mode mode ATTRIBUTE_UNUSED;
     rtx x;
     int strict;
{
  rtx op0, op1;

  /* A reg is ok.  */
  if (GET_CODE (x) == REG && C6X_REG_OK_FOR_BASE_P (x, strict)
      && (! strict || REGNO (x) < FIRST_PSEUDO_REGISTER))
    return 1;

  /* The sum of a reg and a constant in range 'J' is ok -> "*+baseReg(ucst5)".
   * Negative 'J' is also ok -> "*-baseReg(ucst5)".  */
  else if ((GET_CODE (x) == PLUS) 
           && GET_CODE (op0 = XEXP (x, 0)) == REG
	   && C6X_REG_OK_FOR_BASE_P (op0, strict)
	   && (GET_CODE (op1 = XEXP (x, 1)) == CONST_INT)
	   && (CONST_OK_FOR_LETTER_P (INTVAL (op1), 'J')
	       || CONST_OK_FOR_LETTER_P (-INTVAL (op1), 'J')))
    return 1;

  /* The sum of two registers is ok.  (Well, I thought this, but it actually
   * is supported only for `*reg(immediate)' addresses, not for `*reg(reg)'.
   * So the following passage has been deactivated.  See also
   * `c6x_legitimize_address'.  */
#if 0
  else if ((GET_CODE (x) == PLUS) 
           && GET_CODE (op0 = XEXP (x, 0)) == REG
	   && C6X_REG_OK_FOR_BASE_P (op0, strict)
           && GET_CODE (op1 = XEXP (x, 1)) == REG
	   && C6X_REG_OK_FOR_BASE_P (op1, strict))
    {
      int regno0, regno1;

      if (! strict)
	return 1;

      regno0 = C6X_EFFECTIVE_REGNO (REGNO (op0));
      regno1 = C6X_EFFECTIVE_REGNO (REGNO (op1));
      
      /* Now that we know the register numbers, we have to make sure that both
       * operands come from the same register file, as required in
       * spru189f.pdf, p. 3-68.  */
      if ((TEST_HARD_REG_BIT (reg_class_contents[A_REGS], regno0)
	   && TEST_HARD_REG_BIT (reg_class_contents[A_REGS], regno1))
	  || (TEST_HARD_REG_BIT (reg_class_contents[B_REGS], regno0)
	      && TEST_HARD_REG_BIT (reg_class_contents[B_REGS], regno1)))
	return 1;
    }
#endif

  /* Anything else is forbidden.  */
  return 0;
}

/* Legitimize an address X.  
 *
 * Called via macro `LEGITIMIZE_ADDRESS' by function `memory_address' if the
 * preceding `GO_IF_LEGITIMATE_ADDRESS' said that X was not valid as address.
 */
rtx
c6x_legitimize_address (x, oldx, mode)
     register rtx x;
     register rtx oldx ATTRIBUTE_UNUSED;
     enum machine_mode mode ATTRIBUTE_UNUSED;
{
  rtx op0, op1, new_op1;

  /* Force the second PLUS-operand into a register if it is a constant.  */
  if ((GET_CODE (x) == PLUS) 
      && GET_CODE (op0 = XEXP (x, 0)) == REG
      /* This function is called with some pseudos yet unallocated, so we
       * cannot reject what we don't yet know. Hence, we pass 0, which means
       * non-strict checking. */
      && C6X_REG_OK_FOR_BASE_P (op0, 0)
      && GET_CODE (op1 = XEXP (x, 1)) == CONST_INT)
    {
      new_op1 = force_reg (Pmode, op1);
      return gen_rtx_PLUS (Pmode, op0, new_op1);
    }

  return x;
}

/* Implements the PRINT_OPERAND macro, used in `output_operand'.
 *
 * X -- the operator to be printed.
 * CODE -- a letter specified as %<code><digit> in the assembler template.  */
void
c6x_print_operand (file, x, code)
     FILE *file;
     rtx x;
     int code;
{
  switch (code)
    {
    case 0:
      /* Handled below.  */
      break;
    default:
      output_operand_lossage ("c6x_print_operand: unknown code");
      return;
    }

  /* Standard operator output when CODE is zero (ie. no letter specified).  */

  switch (GET_CODE (x))
    {
    case REG:
      fputs (reg_names [REGNO (x)], file);
      break;

    /* This case applies, for example, when storing something to memory such as
     * in an assignment to a local variable on the stack.  */
    case MEM:
      {
	rtx addr = XEXP (x, 0);
	/* have `SUBREG's removed and call `c6x_print_operand_address'  */
	output_address (addr);
	break;
      }

    default:
      /* handling (SYMBOL_REF ...), (LABEL_REF ...), (CONST_INT ...) etc.  */
      output_addr_const (file, x);
      break;
    }

  return;
}

void
c6x_print_operand_address (stream, addr)
     FILE * stream;
     rtx addr;
{
  /* Any SUBREGs should have been removed by `output_address', already.  */
  switch (GET_CODE (addr))
    {
    default:
      abort ();

    case REG:
      {
	fprintf (stream, "*%s", reg_names [REGNO (addr)]);
	break;
      }

    case PLUS:
      {
	/* Handle PLUS expressions as generated, for example, by `assign_temp'
	 * representing offsets into the stack.  With this feature, the compiler
	 * can save temporary registers by outputting indexed addresses.  */
	rtx op0 = XEXP (addr, 0);
	rtx op1 = XEXP (addr, 1);
	enum rtx_code code0 = GET_CODE (op0);
	enum rtx_code code1 = GET_CODE (op1);


	/* It would have been nice to have *+reg(reg) addressing, but
	 * only *+reg[reg] seems to be valid on C6x and, as it seems, we
	 * cannot get at the info as to whether the context is ldw, ldh, or
	 * ldb. :-(  See also `c6x_legitimate_address_p'.
	 *
	 * if (code0 == REG && code1 == REG)
	 *   fprintf (stream, "*%s(%s)", reg_names [REGNO (op0)], 
	 *       reg_names [REGNO (op1)]);
	 * else 
	 */

	if (code0 == REG && code1 == CONST_INT)
	  /* We need not check the range of `INTVAL (op1)', since
	   * `LEGITIMATE_CONSTANT_P' should make sure we don't get passed a
	   * constant too large to be validly used as offset, here.  */
	  if (INTVAL (op1) >= 0)
	    fprintf (stream, "*+%s(%d)", reg_names [REGNO (op0)], 
		INTVAL (op1));
	  else
	    fprintf (stream, "*-%s(%d)", reg_names [REGNO (op0)], 
		-INTVAL (op1));
	else
	  abort ();
	break;
      }
    }

  return;
}

/* Return 1 if OP is a valid operand for the MEM of a CALL insn.
 * Definition modelled on the one in ia64.c */

int
c6x_call_operand (op, mode)
     rtx op;
     enum machine_mode mode;
{
  if (mode != GET_MODE (op) && mode != VOIDmode)
    return 0;

  return (GET_CODE (op) == SYMBOL_REF || GET_CODE (op) == REG
	  || (GET_CODE (op) == SUBREG && GET_CODE (XEXP (op, 0)) == REG));
}

/* Push X onto the stack.  */
void
c6x_emit_push (x, mode)
     rtx x;
     enum machine_mode mode;
{
  enum machine_mode use_mode = (mode == VOIDmode) ? GET_MODE (x) : mode;
  int size = GET_MODE_SIZE (use_mode);

  /* `mode' is needed for CONST_INT, which doesn't have a mode itself.  */
  if ((mode == VOIDmode && GET_MODE (x) == VOIDmode)
      || (mode != VOIDmode && GET_MODE (x) != VOIDmode && mode != GET_MODE (x)))
    abort ();

  /* decrease stack pointer */
  emit_insn (gen_rtx_SET (Pmode, stack_pointer_rtx, 
      plus_constant (stack_pointer_rtx, -size)));
  /* move value onto stack */
  emit_insn (gen_rtx_SET (use_mode, 
	gen_rtx_MEM (use_mode, stack_pointer_rtx), x));

  return;
}

/* Pop X off the stack.  */
void
c6x_emit_pop (x, mode)
     rtx x;
     enum machine_mode mode;
{
  enum machine_mode use_mode = (mode == VOIDmode) ? GET_MODE (x) : mode;
  int size = GET_MODE_SIZE (use_mode);

  /* `mode' is needed for CONST_INT, which doesn't have a mode itself.  */
  if ((mode == VOIDmode && GET_MODE (x) == VOIDmode)
      || (mode != VOIDmode && GET_MODE (x) != VOIDmode && mode != GET_MODE (x)))
    abort ();

  /* move value from stack back to X */
  emit_insn (gen_rtx_SET (use_mode, x, 
	gen_rtx_MEM (use_mode, stack_pointer_rtx)));
  /* increase stack pointer */
  emit_insn (gen_rtx_SET (Pmode, stack_pointer_rtx, 
      plus_constant (stack_pointer_rtx, size)));

  return;
}

/* Emit the appropriate sequence for a call.  */
void
c6x_expand_call (retval, mem)
     rtx retval;
     rtx mem;
{
  rtx ret_label;
  rtx tmp;

  /* Put address of label after the call into `tmp'. We will push it onto
   * the stack as return address.  */
  ret_label = gen_label_rtx ();
  tmp = gen_reg_rtx (SImode);
  emit_insn (gen_rtx_SET (SImode, 
	tmp, gen_rtx_LABEL_REF (Pmode, ret_label)));
  
  /* Push return address onto the stack */
  c6x_emit_push (tmp, SImode);

  /* emit the actual `(call (mem ...) (const_int 0))' rtx.  */
  if (!retval)
    emit_call_insn (gen_rtx_CALL (VOIDmode, mem, const0_rtx));
  else
    emit_call_insn (gen_rtx_SET (GET_MODE (retval), retval,
	  gen_rtx_CALL (VOIDmode, mem, const0_rtx)));

  emit_label (ret_label);
}

/* Implements the "prologue" pattern in the .md file.  */
void
c6x_expand_prologue ()
{
  int locals_size;  /* size of local variables and temporaries on the stack */
  int actuals_size; /* size of the actual argument block (`argblock') */
  int regno;

  /* save old fp to stack */
  c6x_emit_push (hard_frame_pointer_rtx, SImode);
  /* save sp to fp */
  emit_insn (gen_rtx_SET (Pmode, hard_frame_pointer_rtx, stack_pointer_rtx)); 

  /* Local vars go at negative offsets relative to the frame pointer.  Make
   * sp skip them.  */
  locals_size = get_frame_size ();
  emit_insn (gen_rtx_SET (Pmode, stack_pointer_rtx, 
      plus_constant (stack_pointer_rtx, -locals_size)));

  /* Register Save Area:
   * Push each register that is used in the function (regs_ever_live) onto the
   * stack, unless it has been marked in `call_used_regs', which means that
   * functions are allowed to clobber it and hence need not save/restore it.  */
  for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
    if (regs_ever_live [regno] && ! call_used_regs [regno])
	c6x_emit_push (regno_reg_rtx [regno], SImode);

  /* allocate space for the outgoing arguments block (see
   * `ACCUMULATE_OUTGOING_ARGS') */
  actuals_size = current_function_outgoing_args_size;
  emit_insn (gen_rtx_SET (Pmode, stack_pointer_rtx, 
      plus_constant (stack_pointer_rtx, -actuals_size)));

  return;
}

/* Implements the "epilogue" pattern in the .md file.  */
void
c6x_expand_epilogue (sibcall_p)
     int sibcall_p ATTRIBUTE_UNUSED;
{
  int regno;
  int locals_size;   /* size of local variables and temporaries on the stack */
  int save_size = 0; /* size of register save area */
  rtx jump_insn, jump_set;

  /* Compute the size the register save area takes on the stack.  */
  for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
    if (regs_ever_live [regno] && ! call_used_regs [regno])
      save_size += GET_MODE_SIZE (SImode);
  locals_size = get_frame_size ();

  /* Restore sp up to the point at which we can pop off the registers saved
   * during the prologue.  We have to use FP as base here, because
   * calls to `alloca' might have changed the value of SP in the course of
   * the function.  */
  emit_insn (gen_rtx_SET (Pmode, stack_pointer_rtx, 
      plus_constant (hard_frame_pointer_rtx, -(locals_size + save_size))));

  /* Pop each register that was saved in c6x_expand_prologue from stack in
   * reverse order.  */
  for (regno = FIRST_PSEUDO_REGISTER - 1; regno >= 0; regno--)
    if (regs_ever_live [regno] && ! call_used_regs [regno])
        c6x_emit_pop (regno_reg_rtx [regno], SImode);

  /* Restore sp from fp, skipping the local vars.  After this insn, sp will
   * point to the dynamic link (old fp) that was saved during the function
   * prologue.  */
  emit_insn (gen_rtx_SET (Pmode, stack_pointer_rtx, hard_frame_pointer_rtx));

  /* restore old fp from stack */
  c6x_emit_pop (hard_frame_pointer_rtx, SImode);

  /* return */
  c6x_emit_pop (regno_reg_rtx [SCRATCH_REGNO], SImode);
  jump_insn = emit_jump_insn ( 
      gen_indirect_jump (regno_reg_rtx [SCRATCH_REGNO]));

  /* Show the SET in the above jump insn is a RETURN.
   * Required by `thread_prologue_and_epilogue_insns'
   * ->`commit_edge_insertions' ->`commit_one_edge_insertion'
   * ->`returnjump_p' for detecting a return jump.  Otherwise
   *  `commit_one_edge_insertion' will issue an ICE. */
  jump_set = single_set (jump_insn);
  if (! jump_set)
    abort ();
  else
    SET_IS_RETURN_P (jump_set) = 1;

  return;
}
