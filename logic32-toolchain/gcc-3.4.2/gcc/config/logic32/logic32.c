#include <stdio.h>
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
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
#include "recog.h"
#include "expr.h"
#include "libfuncs.h"
#include "toplev.h"
#include "basic-block.h"
#include "function.h"
#include "ggc.h"
#include "reload.h"
#include "target.h"
#include "target-def.h"

#define	here()	printf("@ %s:%d\n", __FILE__, __LINE__)

#define	RETURN_POINTER_REGNUM			31

struct logic32_frame_info
{
	unsigned int total_size;	/* # Bytes that the entire frame takes up. */
	unsigned int pretend_size;	/* # Bytes we push and pretend caller did. */
	unsigned int args_size;		/* # Bytes that outgoing arguments take up. */
	unsigned int reg_size;		/* # Bytes needed to store regs. */
	unsigned int var_size;		/* # Bytes that variables take up. */
	unsigned int frame_size;	/* # Bytes in current frame.  */
	unsigned int gmask;			/* Mask of saved registers. */
	unsigned int save_fp;		/* Nonzero if frame pointer must be saved. */
	unsigned int save_rp;		/* Nonzero if return pointer must be saved. */
	int          initialised;	/* Nonzero if frame size already calculated. */
};

/* Current frame information calculated by arc_compute_frame_size.  */
static struct logic32_frame_info current_frame_info;

const char *logic32_board_name = "logic32_v1";

/* Tell prologue and epilogue if register REGNO should be saved / restored.
   The return address and frame pointer are treated separately.
   Don't consider them here.  */
#define MUST_SAVE_REGISTER(regno)      \
  (   (regno) != RETURN_POINTER_REGNUM \
   && (regno) != FRAME_POINTER_REGNUM  \
   &&   regs_ever_live [regno]         \
   && ! call_used_regs [regno]         )

#define MUST_SAVE_FRAME_POINTER	 (regs_ever_live [FRAME_POINTER_REGNUM]  || frame_pointer_needed)
#define MUST_SAVE_RETURN_POINTER (regs_ever_live [RETURN_POINTER_REGNUM] || current_function_profile)

#if UNITS_PER_WORD == 4
#define WORD_ALIGN(SIZE) (((SIZE) + 3) & ~3)
#endif

/* Returns the number of bytes offset between FROM_REG and TO_REG
   for the current function.  As a side effect it fills in the 
   current_frame_info structure, if the data is available.  */
static unsigned int logic32_compute_frame_size (int from_reg, int to_reg)
{
	int 		regno;
	unsigned int 	return_value;
	unsigned int	var_size;
	unsigned int	args_size;
	unsigned int	pretend_size;
	unsigned int 	reg_size;
	unsigned int 	gmask;

	var_size	= WORD_ALIGN (get_frame_size ());
	args_size	= WORD_ALIGN (current_function_outgoing_args_size);
	pretend_size	= current_function_pretend_args_size;

	reg_size	= 0;
	gmask		= 0;

	/* Calculate space needed for registers.  */
	for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno ++)
	{
		if (MUST_SAVE_REGISTER (regno))
		{
			reg_size += UNITS_PER_WORD;
			gmask |= 1 << regno;
		}
	}

	current_frame_info.save_fp = MUST_SAVE_FRAME_POINTER;
	current_frame_info.save_rp = MUST_SAVE_RETURN_POINTER;

	reg_size += (current_frame_info.save_fp + current_frame_info.save_rp)
	       * UNITS_PER_WORD;

	/* Save computed information.  */
	current_frame_info.pretend_size = pretend_size;
	current_frame_info.var_size     = var_size;
	current_frame_info.args_size    = args_size;
	current_frame_info.reg_size	    = reg_size;
	current_frame_info.frame_size   = args_size + var_size;
	current_frame_info.total_size   = args_size + var_size + reg_size + pretend_size;
	current_frame_info.gmask	  = gmask;
	current_frame_info.initialised  = reload_completed;

	/* Calculate the required distance.  */
	return_value = 0;

	if (to_reg == STACK_POINTER_REGNUM)
		return_value += args_size + var_size;

	if (from_reg == ARG_POINTER_REGNUM)
		return_value += reg_size;

	return return_value;
}

/* Get the value of regs_ever_live[REGNO].  */
static bool df_regs_ever_live_p (unsigned int regno)
{
  return regs_ever_live[regno];
}


void logic32_init_cumulative_args(CUMULATIVE_ARGS *cum, tree funtype, rtx libname, tree fndecl)
{
	memset(cum, 0, sizeof(*cum));
	cum->nregs = LOGIC32_NUM_ARG_REGS;
	cum->words = 0;
	cum->regno = FIRST_ARG_REGNUM;
}

void logic32_function_arg_advance(CUMULATIVE_ARGS *cum, enum machine_mode mode, tree type, int named)
{
	HOST_WIDE_INT bytes, words;
	if (mode == VOIDmode) bytes = int_size_in_bytes(type);
	else bytes = GET_MODE_SIZE(mode);
	words = (bytes + UNITS_PER_WORD - 1) / UNITS_PER_WORD;

	cum->words += words;
	cum->nregs -= words;
	cum->regno += words;

	if (cum->nregs <= 0) {
		cum->nregs = 0;
		cum->regno = FIRST_ARG_REGNUM;
	}
}

rtx logic32_function_arg (CUMULATIVE_ARGS *cum, enum machine_mode omode, tree type, int named)
{
	enum machine_mode mode = omode;
	HOST_WIDE_INT bytes, words;

	if (mode == BLKmode) bytes = int_size_in_bytes(type);
	else bytes = GET_MODE_SIZE(mode);
	words = (bytes + UNITS_PER_WORD - 1) / UNITS_PER_WORD;

	if ( words <= cum->nregs) {
		int regno = cum->regno;
		return gen_rtx_REG(mode, regno);
	}

	return NULL_RTX;
}

static int calcSavedSize(void)
{
	int regno;
	int savedSize=0;
	for (regno = 0; regno < STACK_POINTER_REGNUM; regno++) {
		if (regs_ever_live[regno]&&!call_used_regs[regno]) {
			savedSize+=4;
		}
	}
	return savedSize;	 
}

/* Define the offset between two registers, one to be eliminated, and the
   other its replacement, at the start of a routine.  */
int logic32_initial_elimination_offset (int from, int to)
{
	int offset;
	/* The difference between the argument pointer and the stack pointer is
	 * the sum of the size of this function's frame, the callee register save
	 * area, and the fixed stack space needed for function calls (if any). 
	 */
	if (from == ARG_POINTER_REGNUM && to == STACK_POINTER_REGNUM) {
    	offset = current_function_outgoing_args_size + get_frame_size () + calcSavedSize();
		if ( MUST_SAVE_FRAME_POINTER ) {
			offset += UNITS_PER_WORD;
		}
		if ( MUST_SAVE_RETURN_POINTER ) {
			offset += UNITS_PER_WORD;
		}
	}
	else if (from == ARG_POINTER_REGNUM && to == FRAME_POINTER_REGNUM) {
    	offset = calcSavedSize();
		if ( MUST_SAVE_FRAME_POINTER ) {
			offset += UNITS_PER_WORD;
		}
		if ( MUST_SAVE_RETURN_POINTER ) {
			offset += UNITS_PER_WORD;
		}
    }
	else if (from == FRAME_POINTER_REGNUM && to == STACK_POINTER_REGNUM) {
		offset = current_function_outgoing_args_size + get_frame_size ();
	}
	else {
		abort();
	}
    return offset;
}

int logic32_legitimate_address(enum machine_mode mode, rtx addr_x, int strict_p)
{
	return logic32_classify_address(NULL, mode, addr_x, strict_p);
}

bool logic32_classify_address(struct logic32_address_info *addr_info, enum machine_mode mode, rtx addr_x, bool strict)
{
	rtx op0, op1;

	switch (GET_CODE(addr_x))
	{
		case REG:
		/* case SUBREG: */
			if ( LOGIC32_REG_OK_FOR_BASE_P (addr_x, strict) && (! strict || REGNO (addr_x) < FIRST_PSEUDO_REGISTER)) {
				if ( addr_info ) {
					addr_info->type = ADDRESS_REG;
					addr_info->reg = addr_x;
					addr_info->offset = const0_rtx;
				}
				return true;
			}
			return false;
		case PLUS:

           if ( GET_CODE (op0 = XEXP (addr_x, 0)) == REG \
			   && LOGIC32_REG_OK_FOR_BASE_P (op0, strict) \
			   && (GET_CODE (op1 = XEXP (addr_x, 1)) == CONST_INT) \
			   && CONST_OK_FOR_LETTER_P (INTVAL (op1), 'I')) {
				if ( addr_info ) {
					addr_info->type = BASE_ADDRESS_REG;
					addr_info->reg = XEXP(addr_x, 0);
					addr_info->offset = XEXP(addr_x, 1);
					addr_info->base_rtx = addr_info->reg;
				}
				return true;
			}
			return false;
		case CONST_INT:
			if ( CONST_OK_FOR_LETTER_P (INTVAL (addr_x), 'I') == 0 ) {
				return false;
			}
			if ( addr_info ) {
				addr_info->type = ADDRESS_CONST_INT;
			}
			return true;
		case CONST:
/*
		case LABEL_REF:
		case SYMBOL_REF:
			if ( addr_info ) {
				addr_info->type = ADDRESS_SYMBOLIC;
			}
			return true;
*/
		default:
			return false;
	}
}

/*
bool logic32_regno_mode_ok_for_base_p(int regno, enum machine_mode mode, bool strict_p)
{
	(void)mode;
	if (!HARD_REGISTER_NUM_P(regno))
	{
		if (!strict_p) return true;
		regno = reg_renumber[regno];
	}
	if (regno == ARG_POINTER_REGNUM || regno == FRAME_POINTER_REGNUM || regno == STACK_POINTER_REGNUM ) {
		return true;
	}
	return GP_REG_P(regno);
}
*/

void logic32_print_operand (FILE *file, rtx op, int letter)
{
	enum rtx_code code;
	/*gcc_assert(op);*/
	code = GET_CODE(op);
	switch(code)
	{
		case REG: fprintf(file, "%s", reg_names[REGNO(op)]); break;
		case MEM: output_address(XEXP(op, 0)); break;
		case CONST_INT: output_addr_const(file, op); break;
		case LABEL_REF: output_addr_const(file, op); break;
		case SYMBOL_REF: output_addr_const(file, op); break;
		case CONST: output_addr_const(file, op); break;
#if 1
		default:
			if ( code == CALL_INSN ) {
				fprintf (stderr, "??? code = (CALL_INSN)%x\n", code);
			}
			else if ( code == JUMP_INSN ) {
				fprintf (stderr, "??? code = (JUMP_INSN)%x\n", code);
			}
			else if ( code == CONST ) {
				fprintf (stderr, "??? code = (CONST)%x\n", code);
			}
			else {
				fprintf (stderr, "??? code = %x\n", code);
			}
			debug_rtx (op);
			output_operand_lossage ("logic32_print_operand_address: unhandled address");
#endif
	}
}

void logic32_print_operand_address(FILE *file, rtx x)
{
	struct logic32_address_info addr;
	/*int value, tst_value;*/

	if (!logic32_classify_address(&addr, word_mode, x, true)) {
		printf("bad operand address\n");
		abort();
		return;
	}

	switch(addr.type)
	{
		case ADDRESS_REG:
			fprintf(file, "(");
			logic32_print_operand(file, addr.reg, 0);
			fprintf(file, ")");
			return;
		case BASE_ADDRESS_REG:
			logic32_print_operand(file, addr.offset, 0);
			fprintf(file, "(");
			logic32_print_operand(file, addr.base_rtx, 0);
			fprintf(file, ")");
			return;
		case ADDRESS_CONST_INT:
			fprintf(file, "(");
			output_addr_const(file, x);
			fprintf(file, ")");
			return;
		case ADDRESS_SYMBOLIC:
			/*output_addr_const(file, logic32_strip_unspec_address(x));*/
			output_addr_const (file, x);
			return;
	}
	fprintf(stderr, "fatal error, unreachable here. file:%s, line:%d\n", __FILE__, __LINE__);
	abort();
	/*gcc_unreachable();*/
}

void logic32_expand_prologue(void)
{
	int regno;
	rtx insn;
	rtx sp_rtx;
	rtx fp_rtx;
	int sp_offset;

	if (reload_completed != 1)
		abort ();

	sp_rtx = gen_rtx_REG(Pmode, STACK_POINTER_REGNUM);
	fp_rtx = gen_rtx_REG (Pmode, HARD_FRAME_POINTER_REGNUM);

	logic32_compute_frame_size(0, 0);

	if ( current_frame_info.reg_size ) {
		/* add $sp, $sp, #regsize */
		insn = emit_insn (gen_addsi3(sp_rtx, sp_rtx, GEN_INT(-current_frame_info.reg_size)));
		RTX_FRAME_RELATED_P (insn) = 1;
	}

	sp_offset = 0;

	if ( current_frame_info.save_fp ) {
		/* save $bp */
		insn = emit_insn (gen_store_reg (GEN_INT(sp_offset), fp_rtx));
		sp_offset += 4;
		RTX_FRAME_RELATED_P (insn) = 1;
	}

	if ( current_frame_info.save_rp ) {
		/* save $ra */
		insn = emit_insn (gen_store_reg(GEN_INT(sp_offset), gen_rtx_REG (Pmode, RETURN_POINTER_REGNUM)));
		sp_offset = sp_offset + 4;
		RTX_FRAME_RELATED_P (insn) = 1;
	}

	/* Save callee-saved registers.  */
	if ( current_frame_info.gmask ) {
		unsigned int mask = 1<<(FIRST_PSEUDO_REGISTER-1);
		for (regno = FIRST_PSEUDO_REGISTER-1; regno >= 0; regno--) {
			if ( current_frame_info.gmask & mask ) {
				insn = emit_insn (gen_store_reg(GEN_INT(sp_offset), gen_rtx_REG (Pmode, regno)));
				sp_offset = sp_offset + 4;
				RTX_FRAME_RELATED_P (insn) = 1;
			}
			mask >>= 1;
		}
	}

	if ( current_frame_info.save_fp ) {
		/* mov $fp, $sp */
		insn = emit_insn (gen_movsi(fp_rtx, sp_rtx));
		RTX_FRAME_RELATED_P (insn) = 1;
	}

	if (current_frame_info.frame_size ) {
		/* add $sp, $sp, #-frame_size */
		insn = emit_insn (gen_addsi3(sp_rtx, sp_rtx, GEN_INT(-(int)(current_frame_info.frame_size))));
		RTX_FRAME_RELATED_P (insn) = 1;
	}
}

/* Generate a frame-related expression:

	(set REG (mem (plus (sp) (const_int OFFSET)))).

   Such expressions are used in FRAME_RELATED_EXPR notes for more complex
   instructions.  Marking the expressions as frame-related is superfluous if
   the note contains just a single set.  But if the note contains a PARALLEL
   or SEQUENCE that has several sets, each set must be individually marked
   as frame-related.  */
static rtx
logic32_dwarf_store (rtx reg, int offset)
{
  rtx set = gen_rtx_SET (VOIDmode,
			 gen_rtx_MEM (GET_MODE (reg),
				      plus_constant (stack_pointer_rtx,
						     offset)),
			 reg);
  RTX_FRAME_RELATED_P (set) = 1;
  return set;
}

/* Emit a frame-related instruction whose pattern is PATTERN.  The
   instruction is the last in a sequence that cumulatively performs the
   operation described by DWARF_PATTERN.  The instruction is marked as
   frame-related and has a REG_FRAME_RELATED_EXPR note containing
   DWARF_PATTERN.  */
static void
logic32_frame_insn (rtx pattern, rtx dwarf_pattern)
{
  rtx insn = emit_insn (pattern);
  RTX_FRAME_RELATED_P (insn) = 1;
  REG_NOTES (insn) = alloc_EXPR_LIST (REG_FRAME_RELATED_EXPR,
				      dwarf_pattern,
				      REG_NOTES (insn));
}

void logic32_expand_epilogue (void)
{
	int frameSize;
	int regno;
	rtx insn;
	rtx sp_rtx;
	rtx fp_rtx;
	int sp_offset;

	if (reload_completed != 1)
		abort ();

	sp_rtx = gen_rtx_REG(Pmode, STACK_POINTER_REGNUM);
	fp_rtx = gen_rtx_REG (Pmode, HARD_FRAME_POINTER_REGNUM);

#if 1
	// ???
	// cannot delete this instruction
	if (current_frame_info.frame_size ) {
		/* add $sp, $sp, #frame_size */
		insn = emit_insn (gen_addsi3(sp_rtx, sp_rtx, GEN_INT((int)(current_frame_info.frame_size))));
	}
#endif

	if ( current_frame_info.save_fp ) {
		/* mov $sp, $fp */
		insn = emit_insn (gen_movsi(sp_rtx, fp_rtx));
		/*
		RTX_FRAME_RELATED_P (insn) = 1;
		*/
	}

	sp_offset = 0;

	if ( current_frame_info.save_fp ) {
		/* load $fp */
		insn = emit_insn (gen_load_reg (fp_rtx, GEN_INT(sp_offset)));
		sp_offset += 4;
		/*
		RTX_FRAME_RELATED_P (insn) = 1;
		*/
	}

	if ( current_frame_info.save_rp )
	{
		/* load $ra */
		insn = emit_insn (gen_load_reg(gen_rtx_REG (Pmode, RETURN_POINTER_REGNUM), GEN_INT(sp_offset)));
		sp_offset += 4;
		/*
		RTX_FRAME_RELATED_P (insn) = 1;
		*/
	}

	/* Restore callee-saved registers.  */
	if ( current_frame_info.gmask ) {
		unsigned int mask = 1<<(FIRST_PSEUDO_REGISTER-1);
		for (regno = FIRST_PSEUDO_REGISTER-1; regno >= 0; regno--) {
			if ( current_frame_info.gmask & mask ) {
				insn = emit_insn (gen_load_reg(gen_rtx_REG (Pmode, regno), GEN_INT(sp_offset)));
				sp_offset = sp_offset + 4;
				RTX_FRAME_RELATED_P (insn) = 1;
			}
			mask >>= 1;
		}
	}

	if ( current_frame_info.reg_size ) {
		/* add $sp, $sp, #regsize */
		insn = emit_insn (gen_addsi3(sp_rtx, sp_rtx, GEN_INT((current_frame_info.reg_size))));
		/*
		RTX_FRAME_RELATED_P (insn) = 1;
		*/
	}

	emit_jump_insn (gen_return_from_func());
}

/* Returns 1 if OP is a symbol reference.  */
static int symbolic_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case SYMBOL_REF:
    case LABEL_REF:
    case CONST :
      return 1;
    default:
      return 0;
    }
}

/* Acceptable arguments to the call insn.  */
int call_address_operand (rtx op, enum machine_mode mode)
{
  return (symbolic_operand (op, mode)
	  || (GET_CODE (op) == CONST_INT && LEGITIMATE_CONSTANT_P (op))
	  || (GET_CODE (op) == REG));
}

int call_operand (rtx op, enum machine_mode mode)
{
  if (GET_CODE (op) != MEM)
    return 0;
  op = XEXP (op, 0);
  return call_address_operand (op, mode);
}

/* Return truth value of whether OP can be used as an operands
   where a register or 16 bit unsigned integer is needed.  */
int uns_arith_operand (rtx op, enum machine_mode mode)
{
  if (GET_CODE (op) == CONST_INT && SMALL_INT_UNSIGNED (op))
    return 1;

  return register_operand (op, mode);
}

/* True if OP can be treated as a signed 16-bit constant.  */
int const_arith_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return GET_CODE (op) == CONST_INT && SMALL_INT (op);
}

/* Return true if OP is a register operand or a signed 16-bit constant.  */
int arith_operand (rtx op, enum machine_mode mode)
{
  return const_arith_operand (op, mode) || register_operand (op, mode);
}

/* Implement RETURN_ADDR_RTX.  Note, we do not support moving
   back to a previous frame.  */
rtx logic32_return_addr (int count, rtx frame ATTRIBUTE_UNUSED)
{
  if (count != 0)
    return const0_rtx;

  return get_hard_reg_initial_val (Pmode, GPR_FIRST + 31);
}

struct gcc_target targetm = TARGET_INITIALIZER;


/*
#include "gt-logic32.h"
*/

