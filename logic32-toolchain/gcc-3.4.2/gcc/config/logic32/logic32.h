/* Definitions of target machine for GNU compiler.
   logic32.
   Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2019
   Free Software Foundation, Inc.

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
Boston, MA 02111-1307, USA.

Note:

*/

/*****************************************************************************
**
** Controlling the Compilation Driver, `gcc'
**
*****************************************************************************/

/*}}}*/ 
/*{{{  Run-time target specifications.  */ 

/*
#undef  ASM_SPEC
#define ASM_SPEC "%{v}"
*/

/* Define this to be a string constant containing `-D' options to define the
   predefined macros that identify this machine and system.  These macros will
   be predefined unless the `-ansi' option is specified. */
/* Names to predefine in the preprocessor for this target machine.  */
#define TARGET_CPU_CPP_BUILTINS()		\
  do						\
    {						\
      builtin_define_std ("__logic32__");		\
    }						\
  while (0)

#define TARGET_DEBUG_MASK	(1 << 0)
#define TARGET_DEBUG		(target_flags & TARGET_DEBUG_MASK)

/* Macros used in the machine description to test the flags.  */

/* Default target_flags if no switches specified.  */
#ifndef TARGET_DEFAULT
# define TARGET_DEFAULT			(1)
#endif
/*
#define TARGET_DEFAULT		0
*/

/* This declaration should be present.  */
extern int target_flags;

/* Macro to define tables used to set the flags. This is a list in braces of
   pairs in braces, each pair being { "NAME", VALUE } where VALUE is the bits
   to set or minus the bits to clear. An empty string NAME is used to
   identify the default VALUE.  */
#define TARGET_SWITCHES						\
  {{ "", TARGET_DEFAULT, 0 }}

/* Some machines may desire to change what optimizations are
   performed for various optimization levels.   This macro, if
   defined, is executed once just after the optimization level is
   determined and before the remainder of the command options have
   been parsed.  Values set in this macro are used as the default
   values for the other command line options.

   LEVEL is the optimization level specified; 2 if -O2 is
   specified, 1 if -O is specified, and 0 if neither is specified.  */
#define OPTIMIZATION_OPTIONS(LEVEL,SIZE)				\
{									\
  if (LEVEL >= 2)							\
    {									\
        flag_inline_functions		= 1;				\
      flag_omit_frame_pointer		= 1;				\
    }									\
}

/* Print subsidiary information on the compiler version in use.  */
#define TARGET_VERSION	fprintf (stderr, " (LOGIC32-ISA2)")

/*
#define CAN_DEBUG_WITHOUT_FP
*/

#undef  STARTFILE_SPEC
/* #define STARTFILE_SPEC "crt0.o%s crti.o%s crtbegin.o%s" */
#define STARTFILE_SPEC "crt1%O%s"

/* Include the OS stub library, so that the code can be simulated.
   This is not the right way to do this.  Ideally this kind of thing
   should be done in the linker script - but I have not worked out how
   to specify the location of a linker script in a gcc command line yet... */
#undef  ENDFILE_SPEC
/*
#define ENDFILE_SPEC  "%{!mno-lsim:-lsim} crtend.o%s crtn.o%s"
*/
/* We need to tell the linker the target elf format.  Just pass an
   emulation option.  This can be overridden by -Wl option of gcc.  */
/*
#ifndef LINK_SPEC
#define LINK_SPEC                                               \
#endif
*/

#ifndef LIB_SPEC
#define LIB_SPEC       ""
#endif

#ifndef CC1_SPEC
#define CC1_SPEC       ""
#endif

#ifndef CPP_SPEC
#define CPP_SPEC      ""
#endif

/*****************************************************************************
**
** Run-time Target Specification
**
*****************************************************************************/

/* Run-time compilation parameters selecting different hardware subsets.  */

extern int target_flags;

extern short *reg_renumber;	/* def in local_alloc.c */

/* Define this macro as a C expression for the initializer of an
   array of string to tell the driver program which options are
   defaults for this target and thus do not need to be handled
   specially when using `MULTILIB_OPTIONS'.  */
#ifndef MULTILIB_DEFAULTS
#define MULTILIB_DEFAULTS { "meb" }
#endif

/* This macro is similar to `TARGET_SWITCHES' but defines names of
   command options that have values.  Its definition is an
   initializer with a subgrouping for each command option.

   Each subgrouping contains a string constant, that defines the
   fixed part of the option name, and the address of a variable.  The
   variable, type `char *', is set to the variable part of the given
   option if the fixed part matches.  The actual option name is made
   by appending `-m' to the specified name.  */
#if 0
extern const char *logic32_board_name;
#define TARGET_OPTIONS {						      \
 { "board=", &logic32_board_name, N_("Specify the LOGIC32 board name"), 0} }
#endif

/* These are meant to be redefined in the host dependent files */
#if 0
#define SUBTARGET_SWITCHES
#define SUBTARGET_OPTIONS
#endif

/* Sometimes certain combinations of command options do not make
   sense on a particular target machine.  You can define a macro
   `OVERRIDE_OPTIONS' to take account of this.  This macro, if
   defined, is executed once just after all the command options have
   been parsed.

   Don't use this macro to turn on various extra optimizations for
   `-O'.  That is what `OPTIMIZATION_OPTIONS' is for.  */
#if 0
#define OVERRIDE_OPTIONS	logic32_override_options ();
#endif

/************************************************************************* */

/*}}}*/ 
/*{{{  Storage Layout.  */ 

/* Define this if most significant bit is lowest numbered
   in instructions that operate on numbered bit-fields.  */
#define BITS_BIG_ENDIAN		0

/* Define this if most significant byte of a word is the lowest numbered.  */
#define BYTES_BIG_ENDIAN 	0

/* Define this if most significant word of a multiword number is numbered.  */
#define WORDS_BIG_ENDIAN 	0

/* Width of a word, in units (bytes).  */
#define UNITS_PER_WORD		4

/*
#define PROMOTE_MODE(MODE,UNSIGNEDP,TYPE)	\
  do						\
    {						\
      if (GET_MODE_CLASS (MODE) == MODE_INT	\
	  && GET_MODE_SIZE (MODE) < 4)		\
	(MODE) = SImode;			\
    }						\
  while (0)
*/

/* Normal alignment required for function parameters on the stack, in bits.
   This can't be less than BITS_PER_WORD */
#define PARM_BOUNDARY		32

/* Boundary (bits) on which stack pointer should be aligned.  */
#define STACK_BOUNDARY		32

/* Allocation boundary (bits) for the code of a function.  */
#define FUNCTION_BOUNDARY	32

/* No data type wants to be aligned rounder than this.  */
#define BIGGEST_ALIGNMENT	32

/*
#define DATA_ALIGNMENT(TYPE, ALIGN)		\
  (TREE_CODE (TYPE) == ARRAY_TYPE		\
   && TYPE_MODE (TREE_TYPE (TYPE)) == QImode	\
   && (ALIGN) < BITS_PER_WORD ? BITS_PER_WORD : (ALIGN))
*/

/*
#define CONSTANT_ALIGNMENT(EXP, ALIGN)  \
  (TREE_CODE (EXP) == STRING_CST	\
   && (ALIGN) < BITS_PER_WORD ? BITS_PER_WORD : (ALIGN))
*/

/* Define this if instructions will fail to work if given data not
   on the nominal alignment.  If instructions will merely go slower
   in that case, do not define this macro.  */
/* All accesses must be aligned.  */
#define STRICT_ALIGNMENT	1

/* Defined in svr4.h.  */
/*
#define PCC_BITFIELD_TYPE_MATTERS 1
*/

/*}}}*/ 
/*{{{  Layout of Source Language Data Types.  */ 

/* Size (bits) of the type "short" on target machine */
#define SHORT_TYPE_SIZE			16

/* Size (bits) of the type "int" on target machine
   (If undefined, default is BITS_PER_WORD).  */
#define INT_TYPE_SIZE			32

/* Size (bits) of the type "long" on target machine */
#define LONG_TYPE_SIZE			32

/* Size (bits) of the type "long long" on target machine */
#define LONG_LONG_TYPE_SIZE     64

/* A C expression for the size in bits of the type `float' on the
   target machine. If you don't define this, the default is one word.
   Don't use default: a word is only 16.  */
#define FLOAT_TYPE_SIZE         32

/* A C expression for the size in bits of the type double on the target
   machine. If you don't define this, the default is two words.
   Be IEEE compliant.  */
#define DOUBLE_TYPE_SIZE        64

#define LONG_DOUBLE_TYPE_SIZE   64

/* Define this as 1 if `char' should by default be signed; else as 0.  */
#define DEFAULT_SIGNED_CHAR		0

/*********************************************************************/
/* Definition of size_t. */
#define SIZE_TYPE               "unsigned int"

/* A C expression for a string describing the name of the data type
   to use for the result of subtracting two pointers.  The typedef
   name `ptrdiff_t' is defined using the contents of the string. */
#define PTRDIFF_TYPE            "unsigned int"

/* Allocation boundary (bits) for storing pointers in memory.  */
#define POINTER_BOUNDARY		32

/* Alignment of field after `int : 0' in a structure.  */
#define EMPTY_FIELD_BOUNDARY	32

/* Every structure's size must be a multiple of this.  */
#define STRUCTURE_SIZE_BOUNDARY 32

/* An integer expression for the size in bits of the largest integer
   machine mode that should actually be used.  All integer machine modes of
   this size or smaller can be used for structures and unions with the
   appropriate sizes.  */
#define MAX_FIXED_MODE_SIZE	64

/* target machine storage layout */

/* Define these to avoid dependence on meaning of `int'. */
#define WCHAR_TYPE              "int"
#define WCHAR_TYPE_SIZE         32
/*********************************************************************/

/*}}}*/ 
/*{{{  REGISTER BASICS.  */ 

/* Number of actual hardware registers. The hardware registers are assigned
   numbers for the compiler from 0 to just below FIRST_PSEUDO_REGISTER. 
   All registers that the compiler knows about must be given numbers, even
   those that are not normally considered general registers.  */
#define FIRST_PSEUDO_REGISTER     32

/* Fixed register assignments: */

/* Here we do a BAD THING - reserve a register for use by the machine
   description file.  There are too many places in compiler where it
   assumes that it can issue a branch or jump instruction without
   providing a scratch register for it, and reload just cannot cope, so
   we keep a register back for these situations.  */
/*
#define COMPILER_SCRATCH_REGISTER 0
*/

/* The register that contains the result of a function call.  */
#define RETURN_VALUE_REGNUM	 2

/* The first register that can contain the arguments to a function.  */
#define FIRST_ARG_REGNUM	 4

/* A call-used register that can be used during the function prologue.  */
/*
#define PROLOGUE_TMP_REGNUM	 COMPILER_SCRATCH_REGISTER
*/
     
/* Register numbers used for passing a function's static chain pointer.  If
   register windows are used, the register number as seen by the called
   function is `STATIC_CHAIN_INCOMING_REGNUM', while the register number as
   seen by the calling function is `STATIC_CHAIN_REGNUM'.  If these registers
   are the same, `STATIC_CHAIN_INCOMING_REGNUM' need not be defined.

   The static chain register need not be a fixed register.

   If the static chain is passed in memory, these macros should not be defined;
   instead, the next two macros should be defined.  */
/*
#define STATIC_CHAIN_REGNUM 	12
*/
/* #define STATIC_CHAIN_INCOMING_REGNUM */

/* The register number of the frame pointer register, which is used to access
   automatic variables in the stack frame.  On some machines, the hardware
   determines which register this is.  On other machines, you can choose any
   register you wish for this purpose.  */
/* Base register for access to local variables of the function.  */
#define FRAME_POINTER_REGNUM	30

/* The register number of the stack pointer register, which must also be a
   fixed register according to `FIXED_REGISTERS'.  On most machines, the
   hardware determines which register this is.  */
/* Register to use for pushing function arguments.  */
#define STACK_POINTER_REGNUM	29

/* An initializer that says which registers are used for fixed purposes all
   throughout the compiled code and are therefore not available for general
   allocation.  These would include the stack pointer, the frame pointer
   (except on machines where that can be used as a general register when no
   frame pointer is needed), the program counter on machines where that is
   considered one of the addressable registers, and any other numbered register
   with a standard use.

   This information is expressed as a sequence of numbers, separated by commas
   and surrounded by braces.  The Nth number is 1 if register N is fixed, 0
   otherwise.

   The table initialized from this macro, and the table initialized by the
   following one, may be overridden at run time either automatically, by the
   actions of the macro `CONDITIONAL_REGISTER_USAGE', or by the user with the
   command options `-ffixed-REG', `-fcall-used-REG' and `-fcall-saved-REG'.  */
/* 1 for registers that have pervasive standard uses and are not available
   for the register allocator.  */
#define FIXED_REGISTERS \
  /*0,1,2,3,4,5,6,7                                    k1,k2,gp,sp,fp,ra*/ \
  { 1,1,1,1,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,1,1,1,1,1,1}

/* Like `FIXED_REGISTERS' but has 1 for each register that is clobbered (in
   general) by function calls as well as for fixed registers.  This macro
   therefore identifies the registers that are not available for general
   allocation of values that must live across function calls.

   If a register has 0 in `CALL_USED_REGISTERS', the compiler automatically
   saves it on function entry and restores it on function exit, if the register
   is used within the function.  */
/* 1 for registers not available across function calls. For our pseudo
   registers, all are available.  */
#define CALL_USED_REGISTERS \
  { 1,1,1,1,1,1,1,1,	\
	0,0,0,0,0,0,0,0,	\
	1,1,1,1,1,1,1,1,	\
	0,0,1,1,1,1,1,1}

/* A C initializer containing the assembler's names for the machine registers,
   each one as a C string constant.  This is what translates register numbers
   in the compiler into assembler language.  */
/* How to refer to registers in assembler output.  This sequence is indexed
   by compiler's hard-register-number (see above).  */
#define REGISTER_NAMES \
  { "$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3", \
  	"$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7", \
  	"$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7", \
  	"$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra", \
  	}

/* If defined, a C initializer for an array of structures containing a name and
   a register number.  This macro defines additional names for hard registers,
   thus allowing the `asm' option in declarations to refer to registers using
   alternate names.  */
/*
#define ADDITIONAL_REGISTER_NAMES 				\
{								\
  {"r13", 13}, {"r14", 14}, {"r15", 15}, {"usp", 15}, {"ps", 16}\
}
*/

/*}}}*/ 
/*{{{  How Values Fit in Registers.  */ 

/* A C expression for the number of consecutive hard registers, starting at
   register number REGNO, required to hold a value of mode MODE.  */
#define HARD_REGNO_NREGS(REGNO, MODE) ((GET_MODE_SIZE(MODE) + UNITS_PER_WORD -1)/UNITS_PER_WORD)

/* A C expression that is nonzero if it is permissible to store a value of mode
   MODE in hard register number REGNO (or in several registers starting with
   that one).  */
/* we only allow >32 bits in non-memory mapped registers */
#define HARD_REGNO_MODE_OK(REGNO, MODE) ((GET_MODE_SIZE(MODE)<=UNITS_PER_WORD))

/* A C expression that is nonzero if it is desirable to choose register
   allocation so as to avoid move instructions between a value of mode MODE1
   and a value of mode MODE2.

   If `HARD_REGNO_MODE_OK (R, MODE1)' and `HARD_REGNO_MODE_OK (R, MODE2)' are
   ever different for any R, then `MODES_TIEABLE_P (MODE1, MODE2)' must be
   zero.  */
/* Value is 1 if it is a good idea to tie two pseudo registers when one has
   mode MODE1 and one has mode MODE2.  If HARD_REGNO_MODE_OK could produce
   different values for MODE1 and MODE2, for any hard reg, then this must be
   0 for correct output.

   All modes are tieable except QImode.  */
#define MODES_TIEABLE_P(MODE1, MODE2) 1

/*}}}*/ 
/*{{{  Register Classes.  */ 

/* An enumeral type that must be defined with all the register class names as
   enumeral values.  `NO_REGS' must be first.  `ALL_REGS' must be the last
   register class, followed by one more enumeral value, `LIM_REG_CLASSES',
   which is not a register class but rather tells how many classes there are.

   Each register class has a number, which is the value of casting the class
   name to type `int'.  The number serves as an index in many of the tables
   described below.  */
/* Define the classes of registers for register constraints in the
   machine description.  Also define ranges of constants.

   One of the classes must always be named ALL_REGS and include all hard regs.
   If there is more than one class, another class must be named NO_REGS
   and contain no registers.

   The name GENERAL_REGS must be the name of a class (or an alias for
   another name such as ALL_REGS).  This is the class of registers
   that is allowed by "g" or "r" in a register constraint.
   Also, registers outside this class are allocated only when
   instructions express preferences for them.

   The classes must be numbered in nondecreasing order; that is,
   a larger-numbered class must never be contained completely
   in a smaller-numbered class.

   For any two classes, it is very desirable that there be another
   class that represents their union.  */
enum reg_class
{
    NO_REGS,
    GENERAL_REGS,
    ALL_REGS,
    LIM_REG_CLASSES
};

#define N_REG_CLASSES 	((int) LIM_REG_CLASSES)

/* An initializer containing the names of the register classes as C string
   constants.  These names are used in writing some of the debugging dumps.  */
/* Give names of register classes as strings for dump file.  */
#define REG_CLASS_NAMES \
  { "NO_REGS",                                          \
    "GENERAL_REGS",                                     \
    "ALL_REGS" }

/* An initializer containing the contents of the register classes, as integers
   which are bit masks.  The Nth integer specifies the contents of class N.
   The way the integer MASK is interpreted is that register R is in the class
   if `MASK & (1 << R)' is 1.

   When the machine has more than 32 registers, an integer does not suffice.
   Then the integers are replaced by sub-initializers, braced groupings
   containing several integers.  Each sub-initializer must be suitable as an
   initializer for the type `HARD_REG_SET' which is defined in
   `hard-reg-set.h'.  */
#define REG_CLASS_CONTENTS \
  {\
  	{0},              \
    {0xffffffff}, \
    {0xffffffff}, }

/* A C expression whose value is a register class containing hard register
   REGNO.  In general there is more than one such class; choose a class which
   is "minimal", meaning that no smaller class also contains the register.  */
/* set up a C expression whose value is a register class containing hard
   register REGNO */
#define REGNO_REG_CLASS(REGNO) (((REGNO)<FIRST_PSEUDO_REGISTER)?GENERAL_REGS:ALL_REGS)

/* A macro whose definition is the name of the class to which a valid base
   register must belong.  A base register is one used in an address which is
   the register value plus a displacement.  */
/* The class value for base registers. */
#define BASE_REG_CLASS GENERAL_REGS

/* A macro whose definition is the name of the class to which a valid index
   register must belong.  An index register is one used in an address where its
   value is either multiplied by a scale factor or added to another register
   (as well as added to a displacement).  */
/* The class value for index registers. */
#define INDEX_REG_CLASS GENERAL_REGS

/* A C expression which defines the machine-dependent operand constraint
   letters for register classes.  If CHAR is such a letter, the value should be
   the register class corresponding to it.  Otherwise, the value should be
   `NO_REGS'.  The register letter `r', corresponding to class `GENERAL_REGS',
   will not be passed to this macro; you do not need to handle it.

   The following letters are unavailable, due to being used as
   constraints:
	'0'..'9'
	'<', '>'
	'E', 'F', 'G', 'H'
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'
	'Q', 'R', 'S', 'T', 'U'
	'V', 'X'
	'g', 'i', 'm', 'n', 'o', 'p', 'r', 's' */
/* Get reg_class from a letter in the machine description.  */
#define REG_CLASS_FROM_LETTER(C) NO_REGS

/* A C expression which is nonzero if register number NUM is suitable for use
   as a base register in operand addresses.  It may be either a suitable hard
   register or a pseudo register that has been allocated such a hard register.  */
#define REGNO_OK_FOR_BASE_P(regno) \
	((regno) < FIRST_PSEUDO_REGISTER || reg_renumber[regno] >= 0)

/* A C expression which is nonzero if register number NUM is suitable for use
   as an index register in operand addresses.  It may be either a suitable hard
   register or a pseudo register that has been allocated such a hard register.

   The difference between an index register and a base register is that the
   index register may be scaled.  If an address involves the sum of two
   registers, neither one of them scaled, then either one may be labeled the
   "base" and the other the "index"; but whichever labeling is used must fit
   the machine's constraints of which registers may serve in each capacity.
   The compiler will try both labelings, looking for one that is valid, and
   will reload one or both registers only if neither labeling works.  */
/* These assume that REGNO is a hard or pseudo reg number.
   They give nonzero only if REGNO is a hard reg of the suitable class
   or a pseudo reg currently allocated to a suitable hard reg.
   Since they use reg_renumber, they are safe only once reg_renumber
   has been allocated, which happens in local-alloc.c.  */
#define REGNO_OK_FOR_INDEX_P(regno)  \
	((regno) < FIRST_PSEUDO_REGISTER || reg_renumber[regno] >= 0)

/* A C expression that places additional restrictions on the register class to
   use when it is necessary to copy value X into a register in class CLASS.
   The value is a register class; perhaps CLASS, or perhaps another, smaller
   class.  On many machines, the following definition is safe:

        #define PREFERRED_RELOAD_CLASS(X,CLASS) CLASS

   Sometimes returning a more restrictive class makes better code.  For
   example, on the 68000, when X is an integer constant that is in range for a
   `moveq' instruction, the value of this macro is always `DATA_REGS' as long
   as CLASS includes the data registers.  Requiring a data register guarantees
   that a `moveq' will be used.

   If X is a `const_double', by returning `NO_REGS' you can force X into a
   memory constant.  This is useful on certain machines where immediate
   floating values cannot be loaded into certain kinds of registers.  */
/* Given an rtx X being reloaded into a reg required to be
   in class CLASS, return the class of reg to actually use.
   In general this is just CLASS; but on some machines
   in some cases it is preferable to use a more restrictive class.  
*/
#define PREFERRED_RELOAD_CLASS(X,CLASS) GENERAL_REGS

/* A C expression for the maximum number of consecutive registers of
   class CLASS needed to hold a value of mode MODE.

   This is closely related to the macro `HARD_REGNO_NREGS'.  In fact, the value
   of the macro `CLASS_MAX_NREGS (CLASS, MODE)' should be the maximum value of
   `HARD_REGNO_NREGS (REGNO, MODE)' for all REGNO values in the class CLASS.

   This macro helps control the handling of multiple-word values in
   the reload pass.  */
/* Return the maximum number of consecutive registers needed to represent
   mode MODE in a register of class CLASS.  */
#define CLASS_MAX_NREGS(CLASS, MODE) HARD_REGNO_NREGS(CLASS, MODE)

/************************************************************************* */

/* target machine storage layout */

/* Standard register usage.  */

/* Define this macro to change register usage conditional on target flags.

   The soft-registers are disabled or enabled according to the
  -msoft-reg-count=<n> option.  */
/*
#define CONDITIONAL_REGISTER_USAGE (m68hc11_conditional_register_usage ())
*/

/* List the order in which to allocate registers.  Each register must be
   listed once, even those in FIXED_REGISTERS.  */
#define REG_ALLOC_ORDER {\
	0,1,2,3,4,5,6,7, \
	8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31, \
	}

/* General purpose registers.  */
#define GPR_FIRST       0                       /* First gpr */
#define GPR_LAST        (GPR_FIRST + 31)        /* Last gpr */
#define GPR_R0          GPR_FIRST               /* R0, constant 0 */

#define GPR_P(R)	IN_RANGE(R, GPR_FIRST, GPR_LAST)

/* Define this macro if the compiler should avoid copies to/from CCmode
   registers.  You should only define this macro if support fo copying to/from
   CCmode is incomplete.  */
#define AVOID_CCMODE_COPIES


#define GP_REG_P(REGNO) ((REGNO)<FIRST_PSEUDO_REGISTER)

/* #define SMALL_REGISTER_CLASSES 0 */

/* A C expression whose value is nonzero if pseudos that have been
   assigned to registers of class CLASS would likely be spilled
   because registers of CLASS are needed for spill registers.

   The default value of this macro returns 1 if CLASS has exactly one
   register and zero otherwise.  On most machines, this default
   should be used.  Only define this macro to some other expression
   if pseudo allocated by `local-alloc.c' end up in memory because
   their hard registers were needed for spill registers.  If this
   macro returns nonzero for those classes, those pseudos will only
   be allocated by `global.c', which knows how to reallocate the
   pseudo to another register.  If there would not be another
   register available for reallocation, you should not change the
   definition of this macro since the only effect of such a
   definition would be to slow down register allocation.  */
/* #define CLASS_LIKELY_SPILLED_P(class)  1 */

/* 'U' represents certain kind of memory indexed operand for 68HC12.
   and any memory operand for 68HC11.
   'R' represents indexed addressing mode or access to page0 for 68HC11.
   For 68HC12, it represents any memory operand.  */

/* Define this if successive arguments to a function occupy decreasing 
   addresses in the stack.  */
/* #define ARGS_GROW_DOWNWARD */

/* After the prologue, RA is at 0(AP) in the current frame.  */
#define RETURN_ADDR_RTX logic32_return_addr

/* Before the prologue, the top of the frame is at 4(sp).  */
/*
#define INCOMING_FRAME_SP_OFFSET        4
*/

/************************************************************************* */

/*}}}*/ 
/*{{{  CONSTANTS.  */ 

/* A C expression that defines the machine-dependent operand constraint letters
   (`I', `J', `K', .. 'P') that specify particular ranges of integer values.
   If C is one of those letters, the expression should check that VALUE, an
   integer, is in the appropriate range and return 1 if so, 0 otherwise.  If C
   is not one of those letters, the value should be 0 regardless of VALUE.  */
#define CONST_OK_FOR_LETTER_P(VALUE, C)							\
 (  (C) == 'I' ? IN_RANGE (VALUE,    -32768,       32767)		\
  :	(C) == 'J' ? (VALUE == 0)									\
  :	(C) == 'K' ? (VALUE != 0)									\
  : 0)

/* A C expression that defines the machine-dependent operand constraint letters
   (`G', `H') that specify particular ranges of `const_double' values.

   If C is one of those letters, the expression should check that VALUE, an RTX
   of code `const_double', is in the appropriate range and return 1 if so, 0
   otherwise.  If C is not one of those letters, the value should be 0
   regardless of VALUE.

   `const_double' is used for all floating-point constants and for `DImode'
   fixed-point constants.  A given letter can accept either or both kinds of
   values.  It can use `GET_MODE' to distinguish between these kinds.  */
/* Similar, but for floating constants, and defining letters G and H.  `G' is for 0.0.  */
#define CONST_DOUBLE_OK_FOR_LETTER_P(VALUE, C) \
  ((C) == 'G' ? (GET_MODE_CLASS (GET_MODE (VALUE)) == MODE_FLOAT \
		 && VALUE == CONST0_RTX (GET_MODE (VALUE))) : 0) 

/* A C expression that defines the optional machine-dependent constraint
   letters (`Q', `R', `S', `T', `U') that can be used to segregate specific
   types of operands, usually memory references, for the target machine.
   Normally this macro will not be defined.  If it is required for a particular
   target machine, it should return 1 if VALUE corresponds to the operand type
   represented by the constraint letter C.  If C is not defined as an extra
   constraint, the value returned should be 0 regardless of VALUE.

   For example, on the ROMP, load instructions cannot have their output in r0
   if the memory reference contains a symbolic address.  Constraint letter `Q'
   is defined as representing a memory address that does *not* contain a
   symbolic address.  An alternative is specified with a `Q' constraint on the
   input and `r' on the output.  The next alternative specifies `m' on the
   input and a register class that does not include r0 on the output.  */
/*
#define EXTRA_CONSTRAINT(VALUE, C) \
   ((C) == 'Q' ? (GET_CODE (VALUE) == MEM && GET_CODE (XEXP (VALUE, 0)) == SYMBOL_REF) : 0)
*/

/*}}}*/ 
/*{{{  Basic Stack Layout.  */ 

/* Define this macro if pushing a word onto the stack moves the stack pointer
   to a smaller address.  */
/* Define this if pushing a word on the stack
   makes the stack pointer a smaller address.  */
#define STACK_GROWS_DOWNWARD 1

/* Define this macro if the addresses of local variable slots are at negative
   offsets from the frame pointer.  */
#define FRAME_GROWS_DOWNWARD 1

/* Offset from the frame pointer to the first local variable slot to be
   allocated.

   If `FRAME_GROWS_DOWNWARD', find the next slot's offset by subtracting the
   first slot's length from `STARTING_FRAME_OFFSET'.  Otherwise, it is found by
   adding the length of the first slot to the value `STARTING_FRAME_OFFSET'.  */
/* Offset within stack frame to start allocating local variables at.
   If FRAME_GROWS_DOWNWARD, this is the offset to the END of the
   first local allocated.  Otherwise, it is the offset to the BEGINNING
   of the first local allocated.  */
#define STARTING_FRAME_OFFSET 0

/* Offset from the stack pointer register to the first location at which
   outgoing arguments are placed.  If not specified, the default value of zero
   is used.  This is the proper value for most machines.

   If `ARGS_GROW_DOWNWARD', this is the offset to the location above the first
   location at which outgoing arguments are placed.  */
/*
#define STACK_POINTER_OFFSET 0
*/

/* Offset from the argument pointer register to the first argument's address.
   On some machines it may depend on the data type of the function.

   If `ARGS_GROW_DOWNWARD', this is the offset to the location above the first
   argument's address.  */
/* Offset of first parameter from the argument pointer register value.  */
#define FIRST_PARM_OFFSET(FUNDECL) 0

/* A C expression whose value is RTL representing the location of the incoming
   return address at the beginning of any function, before the prologue.  This
   RTL is either a `REG', indicating that the return value is saved in `REG',
   or a `MEM' representing a location in the stack.

   You only need to define this macro if you want to support call frame
   debugging information like that provided by DWARF 2.  */
/*
#define INCOMING_RETURN_ADDR_RTX gen_rtx_REG (SImode, RETURN_POINTER_REGNUM)
*/

/*}}}*/ 
/*{{{  Register That Address the Stack Frame.  */ 

/* The register number of the arg pointer register, which is used to access the
   function's argument list.  On some machines, this is the same as the frame
   pointer register.  On some machines, the hardware determines which register
   this is.  On other machines, you can choose any register you wish for this
   purpose.  If this is not the same register as the frame pointer register,
   then you must mark it as a fixed register according to `FIXED_REGISTERS', or
   arrange to be able to eliminate it.  */
#define ARG_POINTER_REGNUM			27	/* fake */

/*}}}*/ 
/*{{{  Eliminating the Frame Pointer and the Arg Pointer.  */ 

/* A C expression which is nonzero if a function must have and use a frame
   pointer.  This expression is evaluated in the reload pass.  If its value is
   nonzero the function will have a frame pointer.

   The expression can in principle examine the current function and decide
   according to the facts, but on most machines the constant 0 or the constant
   1 suffices.  Use 0 when the machine allows code to be generated with no
   frame pointer, and doing so saves some time or space.  Use 1 when there is
   no possible advantage to avoiding a frame pointer.

   In certain cases, the compiler does not know how to produce valid code
   without a frame pointer.  The compiler recognizes those cases and
   automatically gives the function a frame pointer regardless of what
   `FRAME_POINTER_REQUIRED' says.  You don't need to worry about them.

   In a function that does not require a frame pointer, the frame pointer
   register can be allocated for ordinary usage, unless you mark it as a fixed
   register.  See `FIXED_REGISTERS' for more information.  */
/* #define FRAME_POINTER_REQUIRED 0 */
#define FRAME_POINTER_REQUIRED \
     (flag_omit_frame_pointer == 0 || current_function_pretend_args_size > 0)

/* If defined, this macro specifies a table of register pairs used to eliminate
   unneeded registers that point into the stack frame.  If it is not defined,
   the only elimination attempted by the compiler is to replace references to
   the frame pointer with references to the stack pointer.

   The definition of this macro is a list of structure initializations, each of
   which specifies an original and replacement register.

   On some machines, the position of the argument pointer is not known until
   the compilation is completed.  In such a case, a separate hard register must
   be used for the argument pointer.  This register can be eliminated by
   replacing it with either the frame pointer or the argument pointer,
   depending on whether or not the frame pointer has been eliminated.

   In this case, you might specify:
        #define ELIMINABLE_REGS  \
        {{ARG_POINTER_REGNUM, STACK_POINTER_REGNUM}, \
         {ARG_POINTER_REGNUM, FRAME_POINTER_REGNUM}, \
         {FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM}}

   Note that the elimination of the argument pointer with the stack pointer is
   specified first since that is the preferred elimination.  */
#define ELIMINABLE_REGS				\
{						\
  {ARG_POINTER_REGNUM,	 STACK_POINTER_REGNUM},	\
  {ARG_POINTER_REGNUM,	 FRAME_POINTER_REGNUM},	\
  {FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM}	\
}


/* A C expression that returns nonzero if the compiler is allowed to try to
   replace register number FROM with register number TO.  This macro
   need only be defined if `ELIMINABLE_REGS' is defined, and will usually be
   the constant 1, since most of the cases preventing register elimination are
   things that the compiler already knows about.  */
#define CAN_ELIMINATE(FROM, TO)						\
 ((TO) == FRAME_POINTER_REGNUM || ! frame_pointer_needed)

/* This macro is similar to `INITIAL_FRAME_POINTER_OFFSET'.  It specifies the
   initial difference between the specified pair of registers.  This macro must
   be defined if `ELIMINABLE_REGS' is defined.  */
#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET)			\
     (OFFSET) = logic32_initial_elimination_offset (FROM, TO)

/*}}}*/ 
/*{{{  Passing Function Arguments on the Stack.  */ 

/* Define this macro if an argument declared in a prototype as an integral type
   smaller than `int' should actually be passed as an `int'.  In addition to
   avoiding errors in certain cases of mismatch, it also makes for better code
   on certain machines.  */
/* Arguments declared as 'char' or 'short' in a prototype should be
   passed as 'int's.  */
/*
#define PROMOTE_PROTOTYPES 1
*/

/* If defined, the maximum amount of space required for outgoing arguments will
   be computed and placed into the variable
   `current_function_outgoing_args_size'.  No space will be pushed onto the
   stack for each call; instead, the function prologue should increase the
   stack frame size by this amount.

   Defining both `PUSH_ARGS' and `ACCUMULATE_OUTGOING_ARGS' is not
   proper.  */
#define ACCUMULATE_OUTGOING_ARGS 1

/* A C expression that should indicate the number of bytes of its own arguments
   that a function pops on returning, or 0 if the function pops no arguments
   and the caller must therefore pop them all after the function returns.

   FUNDECL is a C variable whose value is a tree node that describes the
   function in question.  Normally it is a node of type `FUNCTION_DECL' that
   describes the declaration of the function.  From this it is possible to
   obtain the DECL_ATTRIBUTES of the function.

   FUNTYPE is a C variable whose value is a tree node that describes the
   function in question.  Normally it is a node of type `FUNCTION_TYPE' that
   describes the data type of the function.  From this it is possible to obtain
   the data types of the value and arguments (if known).

   When a call to a library function is being considered, FUNTYPE will contain
   an identifier node for the library function.  Thus, if you need to
   distinguish among various library functions, you can do so by their names.
   Note that "library function" in this context means a function used to
   perform arithmetic, whose name is known specially in the compiler and was
   not mentioned in the C code being compiled.

   STACK-SIZE is the number of bytes of arguments passed on the stack.  If a
   variable number of bytes is passed, it is zero, and argument popping will
   always be the responsibility of the calling function.

   On the VAX, all functions always pop their arguments, so the definition of
   this macro is STACK-SIZE.  On the 68000, using the standard calling
   convention, no functions pop their arguments, so the value of the macro is
   always 0 in this case.  But an alternative calling convention is available
   in which functions that take a fixed number of arguments pop them but other
   functions (such as `printf') pop nothing (the caller pops all).  When this
   convention is in use, FUNTYPE is examined to determine whether a function
   takes a fixed number of arguments.  */
#define RETURN_POPS_ARGS(FUNDECL, FUNTYPE, STACK_SIZE) 0

/* Implement `va_arg'.  */
/*
#define EXPAND_BUILTIN_VA_ARG(valist, type) \
  logic32_va_arg (valist, type)
*/

/*}}}*/ 
/*{{{  Function Arguments in Registers.  */ 

/* Nonzero if we do not know how to pass TYPE solely in registers.
   We cannot do so in the following cases:

   - if the type has variable size
   - if the type is marked as addressable (it is required to be constructed
     into the stack)
   - if the type is a structure or union. */

#define MUST_PASS_IN_STACK(MODE, TYPE)				\
   (((MODE) == BLKmode)						\
    || ((TYPE) != NULL						\
         && TYPE_SIZE (TYPE) != NULL				\
         && (TREE_CODE (TYPE_SIZE (TYPE)) != INTEGER_CST	\
	     || TREE_CODE (TYPE) == RECORD_TYPE			\
	     || TREE_CODE (TYPE) == UNION_TYPE			\
	     || TREE_CODE (TYPE) == QUAL_UNION_TYPE		\
             || TREE_ADDRESSABLE (TYPE))))

/* A C expression that controls whether a function argument is passed in a
   register, and which register.

   The usual way to make the ANSI library `stdarg.h' work on a machine where
   some arguments are usually passed in registers, is to cause nameless
   arguments to be passed on the stack instead.  This is done by making
   `FUNCTION_ARG' return 0 whenever NAMED is 0.

   You may use the macro `MUST_PASS_IN_STACK (MODE, TYPE)' in the definition of
   this macro to determine if this argument is of a type that must be passed in
   the stack.  If `REG_PARM_STACK_SPACE' is not defined and `FUNCTION_ARG'
   returns nonzero for such an argument, the compiler will abort.  If
   `REG_PARM_STACK_SPACE' is defined, the argument will be computed in the
   stack and then loaded into a register.  */
#define FUNCTION_ARG(CUM, MODE, TYPE, NAMED)			\
  (  (NAMED) == 0                    ? NULL_RTX			\
   : MUST_PASS_IN_STACK (MODE, TYPE) ? NULL_RTX			\
   : logic32_function_arg(&(CUM), (MODE), (TYPE), (NAMED)) )

/* A C type for declaring a variable that is used as the first argument of
   `FUNCTION_ARG' and other related values.  For some target machines, the type
   `int' suffices and can hold the number of bytes of argument so far.

   There is no need to record in `CUMULATIVE_ARGS' anything about the arguments
   that have been passed on the stack.  The compiler has other variables to
   keep track of that.  For target machines on which all arguments are passed
   on the stack, there is no need to store anything in `CUMULATIVE_ARGS';
   however, the data structure must exist and should not be empty, so use
   `int'.  */
typedef struct {
	int words;
	int nregs;
	int regno;
} CUMULATIVE_ARGS;

/* A C expression for the number of words, at the beginning of an argument,
   must be put in registers.  The value must be zero for arguments that are
   passed entirely in registers or that are entirely pushed on the stack.

   On some machines, certain arguments must be passed partially in registers
   and partially in memory.  On these machines, typically the first N words of
   arguments are passed in registers, and the rest on the stack.  If a
   multi-word argument (a `double' or a structure) crosses that boundary, its
   first few words must be passed in registers and the rest must be pushed.
   This macro tells the compiler when this occurs, and how many of the words
   should go in registers.

   `FUNCTION_ARG' for these arguments should return the first register to be
   used by the caller for this argument; likewise `FUNCTION_INCOMING_ARG', for
   the called function.  */
/*
#define FUNCTION_ARG_PARTIAL_NREGS(CUM, MODE, TYPE, NAMED) 	\
  logic32_function_arg_partial_nregs (CUM, MODE, TYPE, NAMED)
*/
/* For an arg passed partly in registers and partly in memory,
   this is the number of registers used.
   For args passed entirely in registers or entirely in memory, zero.

   Passing an arg partly in register and memory does not work at all.
   Don't do that.  */
#define FUNCTION_ARG_PARTIAL_NREGS(CUM, MODE, TYPE, NAMED) (0)

/* A C expression that indicates when an argument must be passed by reference.
   If nonzero for an argument, a copy of that argument is made in memory and a
   pointer to the argument is passed instead of the argument itself.  The
   pointer is passed in whatever way is appropriate for passing a pointer to
   that type.

   On machines where `REG_PARM_STACK_SPACE' is not defined, a suitable
   definition of this macro might be:
        #define FUNCTION_ARG_PASS_BY_REFERENCE(CUM, MODE, TYPE, NAMED)  \
          MUST_PASS_IN_STACK (MODE, TYPE)  */
#if 1
#define FUNCTION_ARG_PASS_BY_REFERENCE(CUM, MODE, TYPE, NAMED) \
  MUST_PASS_IN_STACK (MODE, TYPE)
#else
#define FUNCTION_ARG_PASS_BY_REFERENCE(CUM, MODE, TYPE, NAMED) 0
#endif

/* A C statement (sans semicolon) for initializing the variable CUM for the
   state at the beginning of the argument list.  The variable has type
   `CUMULATIVE_ARGS'.  The value of FNTYPE is the tree node for the data type
   of the function which will receive the args, or 0 if the args are to a
   compiler support library function.  The value of INDIRECT is nonzero when
   processing an indirect call, for example a call through a function pointer.
   The value of INDIRECT is zero for a call to an explicitly named function, a
   library function call, or when `INIT_CUMULATIVE_ARGS' is used to find
   arguments for the function being compiled.

   When processing a call to a compiler support library function, LIBNAME
   identifies which one.  It is a `symbol_ref' rtx which contains the name of
   the function, as a string.  LIBNAME is 0 when an ordinary C function call is
   being processed.  Thus, each time this macro is called, either LIBNAME or
   FNTYPE is nonzero, but never both of them at once.  */
/* Initialize a variable CUM of type CUMULATIVE_ARGS for a call to a
   function whose data type is FNTYPE. For a library call, FNTYPE is 0.  */
#define INIT_CUMULATIVE_ARGS(CUM, FNTYPE, LIBNAME, INDIRECT, N_NAMED_ARGS) \
	logic32_init_cumulative_args(&(CUM), (FNTYPE), (LIBNAME), (INDIRECT))

/* A C statement (sans semicolon) to update the summarizer variable CUM to
   advance past an argument in the argument list.  The values MODE, TYPE and
   NAMED describe that argument.  Once this is done, the variable CUM is
   suitable for analyzing the *following* argument with `FUNCTION_ARG', etc.

   This macro need not do anything if the argument in question was passed on
   the stack.  The compiler knows how to track the amount of stack space used
   for arguments without any special help.  */
/* Update the data in CUM to advance over an argument of mode MODE and data
   type TYPE. (TYPE is null for libcalls where that information may not be
   available.) */
#define FUNCTION_ARG_ADVANCE(CUM, MODE, TYPE, NAMED) \
	logic32_function_arg_advance(&CUM, MODE, TYPE, NAMED)

/* A C expression that is nonzero if REGNO is the number of a hard register in
   which function arguments are sometimes passed.  This does *not* include
   implicit arguments such as the static chain and the structure-value address.
   On many machines, no registers can be used for this purpose since all
   function arguments are pushed on the stack.  */
/* 1 if REGNO is a possible register number for function argument passing. */
#define	LOGIC32_NUM_ARG_REGS		4
#define FUNCTION_ARG_REGNO_P(REGNO)		((REGNO) >= FIRST_ARG_REGNUM && ((REGNO) < FIRST_ARG_REGNUM + LOGIC32_NUM_ARG_REGS))


/*}}}*/ 
/*{{{  How Scalar Function Values are Returned.  */ 

/* A C expression to create an RTX representing the place where a function
   returns a value of data type VALTYPE.  VALTYPE is a tree node representing a
   data type.  Write `TYPE_MODE (VALTYPE)' to get the machine mode used to
   represent that type.  On many machines, only the mode is relevant.
   (Actually, on most machines, scalar values are returned in the same place
   regardless of mode).

   If `PROMOTE_FUNCTION_RETURN' is defined, you must apply the same promotion
   rules specified in `PROMOTE_MODE' if VALTYPE is a scalar type.

   If the precise function being called is known, FUNC is a tree node
   (`FUNCTION_DECL') for it; otherwise, FUNC is a null pointer.  This makes it
   possible to use a different value-returning convention for specific
   functions when all their calls are known.

   `FUNCTION_VALUE' is not used for return vales with aggregate data types,
   because these are returned in another way.  See `STRUCT_VALUE_REGNUM' and
   related macros, below.  */
#define	FUNCTION_VALUE(VALTYPE, FUNC)	gen_rtx_REG(TYPE_MODE(VALTYPE), RETURN_VALUE_REGNUM)

/* A C expression to create an RTX representing the place where a library
   function returns a value of mode MODE.  If the precise function being called
   is known, FUNC is a tree node (`FUNCTION_DECL') for it; otherwise, FUNC is a
   null pointer.  This makes it possible to use a different value-returning
   convention for specific functions when all their calls are known.

   Note that "library function" in this context means a compiler support
   routine, used to perform arithmetic, whose name is known specially by the
   compiler and was not mentioned in the C code being compiled.

   The definition of `LIBRARY_VALUE' need not be concerned aggregate data
   types, because none of the library functions returns such types.  */
#define LIBCALL_VALUE(MODE) gen_rtx_REG (MODE, RETURN_VALUE_REGNUM)

/* A C expression that is nonzero if REGNO is the number of a hard register in
   which the values of called function may come back. */
#define FUNCTION_VALUE_REGNO_P(REGNO) ((REGNO) == RETURN_VALUE_REGNUM )

/*}}}*/ 
/*{{{  How Large Values are Returned.  */ 

/* Define this macro to be 1 if all structure and union return values must be
   in memory.  Since this results in slower code, this should be defined only
   if needed for compatibility with other compilers or with an ABI.  If you
   define this macro to be 0, then the conventions used for structure and union
   return values are decided by the `RETURN_IN_MEMORY' macro.

   If not defined, this defaults to the value 1.  */
/*
#define DEFAULT_PCC_STRUCT_RETURN 1
*/

/* If the structure value address is not passed in a register, define
   `STRUCT_VALUE' as an expression returning an RTX for the place where the
   address is passed.  If it returns 0, the address is passed as an "invisible"
   first argument.  */
#define STRUCT_VALUE 0

/*}}}*/ 
/*{{{  Generating Code for Profiling.  */ 

/* A C statement or compound statement to output to FILE some assembler code to
   call the profiling subroutine `mcount'.  Before calling, the assembler code
   must load the address of a counter variable into a register where `mcount'
   expects to find the address.  The name of this variable is `LP' followed by
   the number LABELNO, so you would generate the name using `LP%d' in a
   `fprintf'.

   The details of how the address should be passed to `mcount' are determined
   by your operating system environment, not by GCC.  To figure them out,
   compile a small program for profiling using the system's installed C
   compiler and look at the assembler code that results.  */
#define FUNCTION_PROFILER(FILE, LABELNO)

/*{{{  Implementing the VARARGS Macros.  */ 

/* This macro offers an alternative to using `__builtin_saveregs' and defining
   the macro `EXPAND_BUILTIN_SAVEREGS'.  Use it to store the anonymous register
   arguments into the stack so that all the arguments appear to have been
   passed consecutively on the stack.  Once this is done, you can use the
   standard implementation of varargs that works for machines that pass all
   their arguments on the stack.

   The argument ARGS_SO_FAR is the `CUMULATIVE_ARGS' data structure, containing
   the values that obtain after processing of the named arguments.  The
   arguments MODE and TYPE describe the last named argument--its machine mode
   and its data type as a tree node.

   The macro implementation should do two things: first, push onto the stack
   all the argument registers *not* used for the named arguments, and second,
   store the size of the data thus pushed into the `int'-valued variable whose
   name is supplied as the argument PRETEND_ARGS_SIZE.  The value that you
   store here will serve as additional offset for setting up the stack frame.

   Because you must generate code to push the anonymous arguments at compile
   time without knowing their data types, `SETUP_INCOMING_VARARGS' is only
   useful on machines that have just a single category of argument register and
   use it uniformly for all data types.

   If the argument SECOND_TIME is nonzero, it means that the arguments of the
   function are being analyzed for the second time.  This happens for an inline
   function, which is not actually compiled until the end of the source file.
   The macro `SETUP_INCOMING_VARARGS' should not generate any instructions in
   this case.  */
/*
#define SETUP_INCOMING_VARARGS(ARGS_SO_FAR, MODE, TYPE, PRETEND_ARGS_SIZE, SECOND_TIME) \
  if (! SECOND_TIME) \
    logic32_setup_incoming_varargs (ARGS_SO_FAR, MODE, TYPE, & PRETEND_ARGS_SIZE)
*/

/* Define this macro if the location where a function argument is passed
   depends on whether or not it is a named argument.

   This macro controls how the NAMED argument to `FUNCTION_ARG' is set for
   varargs and stdarg functions.  With this macro defined, the NAMED argument
   is always true for named arguments, and false for unnamed arguments.  If
   this is not defined, but `SETUP_INCOMING_VARARGS' is defined, then all
   arguments are treated as named.  Otherwise, all named arguments except the
   last are treated as named.  */
#define STRICT_ARGUMENT_NAMING 0

/* LJL */

/* If the structure value address is not passed in a register, define
   `STRUCT_VALUE' as an expression returning an RTX for the place
   where the address is passed.  If it returns 0, the address is
   passed as an "invisible" first argument.  */
#define STRUCT_VALUE_INCOMING  0

/* EXIT_IGNORE_STACK should be nonzero if, when returning from a function,
   the stack pointer does not matter.  The value is tested only in functions
   that have frame pointers. No definition is equivalent to always zero.  */
#define EXIT_IGNORE_STACK	0


/* Length in units of the trampoline for entering a nested function.  */
#define TRAMPOLINE_SIZE (32+16)
/* The alignment of a trampoline, in bits.  */
#define TRAMPOLINE_ALIGNMENT  32


/* A C statement to initialize the variable parts of a trampoline.
   ADDR is an RTX for the address of the trampoline; FNADDR is an
   RTX for the address of the nested function; STATIC_CHAIN is an
   RTX for the static chain value that should be passed to the
   function when it is called.  */

#define INITIALIZE_TRAMPOLINE(TRAMP, FNADDR, CXT)  \
{									\
  emit_move_insn (gen_rtx (MEM, SImode, plus_constant ((TRAMP), 32+4)),	\
		  (CXT));						\
  emit_move_insn (gen_rtx (MEM, SImode, plus_constant ((TRAMP), 32+0)),	\
		  (FNADDR));						\
}

/* Addressing modes, and classification of registers for them.  */

/* A C expression that is 1 if the RTX X is a constant which is a valid
   address.  On most machines, this can be defined as `CONSTANT_P (X)', but a
   few machines are more restrictive in which constant addresses are supported.

   `CONSTANT_P' accepts integer-values expressions whose values are not
   explicitly known, such as `symbol_ref', `label_ref', and `high' expressions
   and `const' arithmetic expressions, in addition to `const_int' and
   `const_double' expressions.  */
#define CONSTANT_ADDRESS_P(X) CONSTANT_P (X)

/* Maximum number of registers that can appear in a valid memory address */
#define MAX_REGS_PER_ADDRESS	1

/* The macros REG_OK_FOR..._P assume that the arg is a REG rtx
   and check its validity for a certain class.
   We have two alternate definitions for each of them.
   The usual definition accepts all pseudo regs; the other rejects
   them unless they have been allocated suitable hard regs.
   The symbol REG_OK_STRICT causes the latter definition to be used.

   Most source files want to accept pseudo regs in the hope that
   they will get allocated to the class that the insn wants them to be in.
   Source files for reload pass need to be strict.
   After reload, it makes no difference, since pseudo regs have
   been eliminated by then.  */

#define LOGIC32_REG_OK_FOR_BASE_P(X, STRICT) \
  ((STRICT) \
    ? REGNO_OK_FOR_BASE_P (REGNO (X)) \
    : (GPR_P (REGNO (X)) || (REGNO (X) >= FIRST_PSEUDO_REGISTER)))

#ifdef REG_OK_STRICT

/* Nonzero if X is a hard reg that can be used as a base reg.  */
#define REG_OK_FOR_BASE_P(X) LOGIC32_REG_OK_FOR_BASE_P(X, 1)
/* Nonzero if X is a hard reg that can be used as an index.  */
#define REG_OK_FOR_INDEX_P(X) REG_OK_FOR_BASE_P (X)

#else

/* Nonzero if X is a hard reg that can be used as a base reg
   or if it is a pseudo reg.  */
#define REG_OK_FOR_BASE_P(X) LOGIC32_REG_OK_FOR_BASE_P(X, 0)
/* Nonzero if X is a hard reg that can be used as an index
   or if it is a pseudo reg.  */
#define REG_OK_FOR_INDEX_P(X) REG_OK_FOR_BASE_P (X)

#endif

/* GO_IF_LEGITIMATE_ADDRESS recognizes an RTL expression that is a
   valid memory address for an instruction. The MODE argument is the
   machine mode for the MEM expression that wants to use this address.  */
/* Go to ADDR if X is a valid address.  */
#ifndef REG_OK_STRICT
#define	GO_IF_LEGITIMATE_ADDRESS(MODE, X, ADDR)	\
	do {	\
		if ( logic32_legitimate_address( (MODE), (X), 0) ) goto ADDR;	\
	} while(0)
#else
#define	GO_IF_LEGITIMATE_ADDRESS(MODE, X, ADDR)	\
	do {	\
		if ( logic32_legitimate_address ((MODE), (X), 1) ) goto ADDR;	\
	} while(0)
#endif

/* Try machine-dependent ways of modifying an illegitimate address
   to be legitimate.  If we find one, return the new, valid address.
   This macro is used in only one place: `memory_address' in explow.c.
  
   OLDX is the address as it was before break_out_memory_refs was called.
   In some cases it is useful to look at this to decide what needs to be done.
  
   MODE and WIN are passed so that this macro can use
   GO_IF_LEGITIMATE_ADDRESS.
  
   It is always safe for this macro to do nothing.
   It exists to recognize opportunities to optimize the output.  */

#define LEGITIMIZE_ADDRESS(x, oldx, mode, win) 

/* A C statement or compound statement with a conditional `goto
   LABEL;' executed if memory address X (an RTX) can have different
   meanings depending on the machine mode of the memory reference it
   is used for.

   Autoincrement and autodecrement addresses typically have
   mode-dependent effects because the amount of the increment or
   decrement is the size of the operand being addressed.  Some
   machines have other mode-dependent addresses.  Many RISC machines
   have no mode-dependent addresses.

   You may assume that ADDR is a valid address for the machine.  */

#define GO_IF_MODE_DEPENDENT_ADDRESS(ADDR,LABEL) {}

/* A C expression that is nonzero if X is a legitimate constant for an
   immediate operand on the target machine.  You can assume that X satisfies
   `CONSTANT_P', so you need not check this.  In fact, `1' is a suitable
   definition for this macro on machines where anything `CONSTANT_P' is valid.  */
#define LEGITIMATE_CONSTANT_P(X) 1

/* Tell final.c how to eliminate redundant test instructions.  */
/* Store in cc_status the expressions
   that the condition codes will describe
   after execution of an instruction whose pattern is EXP.
   Do not alter them if the instruction would not alter the cc's.  */
#define NOTICE_UPDATE_CC(exp, insn) CC_STATUS_INIT

/* Compute extra cost of moving data between one register class
   and another.  */
/*
#define REGISTER_MOVE_COST(MODE, CLASS1, CLASS2)	2
*/

/* A C expressions returning the cost of moving data of MODE from a register to
   or from memory.  */
/*
#define MEMORY_MOVE_COST(MODE,CLASS,IN)				4
*/

/* A C expression for the cost of a branch instruction.  A value of 1
   is the default; other values are interpreted relative to that.

   Pretend branches are cheap because GCC generates sub-optimal code
   for the default value.  */
/*
#define BRANCH_COST 0
*/

/*}}}*/ 
/*{{{  Describing Relative Costs of Operations */ 

/* Define this macro as a C expression which is nonzero if accessing less than
   a word of memory (i.e. a `char' or a `short') is no faster than accessing a
   word of memory, i.e., if such access require more than one instruction or if
   there is no difference in cost between byte and (aligned) word loads.

   When this macro is not defined, the compiler will access a field by finding
   the smallest containing object; when it is defined, a fullword load will be
   used if alignment permits.  Unless bytes accesses are faster than word
   accesses, using word accesses is preferable since it may eliminate
   subsequent memory access if subsequent accesses occur to other fields in the
   same word of the structure, but to different bytes.  */
#define SLOW_BYTE_ACCESS 1

/*}}}*/ 
/*{{{  Dividing the output into sections.  */ 

/* A C expression whose value is a string containing the assembler operation
   that should precede instructions and read-only data.  Normally `".text"' is
   right.  */
/* Output before read-only data.  */
#define TEXT_SECTION_ASM_OP	("\t.section\t.text")

/* A C expression whose value is a string containing the assembler operation to
   identify the following data as writable initialized data.  Normally
   `".data"' is right.  */
/* Output before writable data.  */
#define DATA_SECTION_ASM_OP	("\t.section\t.data")

/* If defined, a C expression whose value is a string containing the
   assembler operation to identify the following data as
   uninitialized global data.  If not defined, and neither
   `ASM_OUTPUT_BSS' nor `ASM_OUTPUT_ALIGNED_BSS' are defined,
   uninitialized global data will be output in the data section if
   `-fno-common' is passed, otherwise `ASM_OUTPUT_COMMON' will be
   used.  */
/* Output before uninitialized data.  */
#define BSS_SECTION_ASM_OP 	("\t.section\t.bss")

/*}}}*/ 
/*{{{  The Overall Framework of an Assembler File.  */

/* A C string constant describing how to begin a comment in the target
   assembler language.  The compiler assumes that the comment will end at the
   end of the line.  */
#define ASM_COMMENT_START ";"

/* A C string constant for text to be output before each `asm' statement or
   group of consecutive ones.  Normally this is `"#APP"', which is a comment
   that has no effect on most assemblers but tells the GNU assembler that it
   must check the lines that follow for all valid assembler constructs.  */
/* Output to assembler file text saying following lines
   may contain character constants, extra white space, comments, etc.  */
#define ASM_APP_ON 		"; Begin inline assembler code\n#APP\n"

/* A C string constant for text to be output after each `asm' statement or
   group of consecutive ones.  Normally this is `"#NO_APP"', which tells the
   GNU assembler to resume making the time-saving assumptions that are valid
   for ordinary compiler output.  */
/* Output to assembler file text saying following lines
   no longer contain unusual constructs.  */
#define ASM_APP_OFF 		"; End of inline assembler code\n#NO_APP\n"

/*}}}*/ 
/*{{{  Output and Generation of Labels.  */ 

/* Globalizing directive for a label.  */
#define	GLOBAL_ASM_OP			"\t.globl\t"

/*}}}*/ 
/*{{{  Output of Assembler Instructions.  */ 

/* A C compound statement to output to stdio stream STREAM the assembler syntax
   for an instruction operand X.  X is an RTL expression.

   CODE is a value that can be used to specify one of several ways of printing
   the operand.  It is used when identical operands must be printed differently
   depending on the context.  CODE comes from the `%' specification that was
   used to request printing of the operand.  If the specification was just
   `%DIGIT' then CODE is 0; if the specification was `%LTR DIGIT' then CODE is
   the ASCII code for LTR.

   If X is a register, this macro should print the register's name.  The names
   can be found in an array `reg_names' whose type is `char *[]'.  `reg_names'
   is initialized from `REGISTER_NAMES'.

   When the machine description has a specification `%PUNCT' (a `%' followed by
   a punctuation character), this macro is called with a null pointer for X and
   the punctuation character for CODE.  */
#define PRINT_OPERAND(FILE, X, CODE) logic32_print_operand (FILE, X, CODE)

/* A C expression which evaluates to true if CODE is a valid punctuation
   character for use in the `PRINT_OPERAND' macro.  If
   `PRINT_OPERAND_PUNCT_VALID_P' is not defined, it means that no punctuation
   characters (except for the standard one, `%') are used in this way.  */
#define PRINT_OPERAND_PUNCT_VALID_P(CODE) (CODE == '#')

/* A C compound statement to output to stdio stream STREAM the assembler syntax
   for an instruction operand that is a memory reference whose address is X.  X
   is an RTL expression.  */
/* Print a memory operand whose address is X, on file FILE.  */
#define PRINT_OPERAND_ADDRESS(FILE, X) logic32_print_operand_address (FILE, X)

/* If defined, C string expressions to be used for the `%R', `%L', `%U', and
   `%I' options of `asm_fprintf' (see `final.c').  These are useful when a
   single `md' file must support multiple assembler formats.  In that case, the
   various `tm.h' files can define these macros differently.

   USER_LABEL_PREFIX is defined in svr4.h.  */
/*
#%efine REGISTER_PREFIX "$"
#define USER_LABEL_PREFIX ""
*/

/* The prefix for local labels.  You should be able to define this as
   an empty string, or any arbitrary string (such as ".", ".L%", etc)
   without having to make any other changes to account for the specific
   definition.  Note it is a string literal, not interpreted by printf
   and friends.  */
#define LOCAL_LABEL_PREFIX "."

/* The prefix for immediate operands.  */
#define IMMEDIATE_PREFIX "#"

/*}}}*/ 
/*{{{  Output of Dispatch Tables.  */ 

/* This macro should be provided on machines where the addresses in a dispatch
   table are relative to the table's own address.

   The definition should be a C statement to output to the stdio stream STREAM
   an assembler pseudo-instruction to generate a difference between two labels.
   VALUE and REL are the numbers of two internal labels.  The definitions of
   these labels are output using `(*targetm.asm_out.internal_label)', and they must be
   printed in the same way here.  For example,

        fprintf (STREAM, "\t.word L%d-L%d\n", VALUE, REL)  */
/* This is how to output an element of a case-vector that is relative.  */
#define ASM_OUTPUT_ADDR_DIFF_ELT(FILE, BODY, VALUE, REL) \
  fprintf (FILE, "\t%s\tL%d-L%d\n", integer_asm_op (4, TRUE), VALUE, REL)

/* This macro should be provided on machines where the addresses in a dispatch
   table are absolute.

   The definition should be a C statement to output to the stdio stream STREAM
   an assembler pseudo-instruction to generate a reference to a label.  VALUE
   is the number of an internal label whose definition is output using
   `(*targetm.asm_out.internal_label)'.  For example,

        fprintf (STREAM, "\t.word L%d\n", VALUE)  */
/* This is how to output an element of a case-vector that is absolute.  */
#define ASM_OUTPUT_ADDR_VEC_ELT(FILE, VALUE) \
  fprintf (FILE, "\t%s\t.L%d\n", integer_asm_op (4, TRUE), VALUE)

/*}}}*/ 
/*{{{  Assembler Commands for Alignment.  */ 

/* A C statement to output to the stdio stream STREAM an assembler command to
   advance the location counter to a multiple of 2 to the POWER bytes.  POWER
   will be a C expression of type `int'.  */
/* This is how to output an assembler line that says to advance the
   location counter to a multiple of 2**LOG bytes.  */
#define ASM_OUTPUT_ALIGN(FILE,LOG) \
do { if ((LOG) != 0) fprintf (FILE, "\t.align %d\n", 1 << (LOG)); } while (0)

/*}}}*/ 
/*{{{  Miscellaneous Parameters.  */ 

/* An alias for a machine mode name.  This is the machine mode that elements of
   a jump-table should have.  */
/* Specify the machine mode that this machine uses
   for the index in the tablejump instruction.  */
#define CASE_VECTOR_MODE	Pmode

/* The maximum number of bytes that a single instruction can move quickly from
   memory to memory.  */
#define MOVE_MAX  UNITS_PER_WORD

/* A C expression which is nonzero if on this machine it is safe to "convert"
   an integer of INPREC bits to one of OUTPREC bits (where OUTPREC is smaller
   than INPREC) by merely operating on it as if it had only OUTPREC bits.

   On many machines, this expression can be 1.

   When `TRULY_NOOP_TRUNCATION' returns 1 for a pair of sizes for modes for
   which `MODES_TIEABLE_P' is 0, suboptimal code can result.  If this is the
   case, making `TRULY_NOOP_TRUNCATION' return 0 in such cases may improve
   things.  */
/* Value is 1 if truncating an integer of INPREC bits to OUTPREC bits
   is done just by pretending it is already truncated.  */
#define TRULY_NOOP_TRUNCATION(OUTPREC, INPREC)	1

/* An alias for the machine mode for pointers.  On most machines, define this
   to be the integer mode corresponding to the width of a hardware pointer;
   `SImode' on 32-bit machine or `DImode' on 64-bit machines.  On some machines
   you must define this to be one of the partial integer modes, such as
   `PSImode'.

   The width of `Pmode' must be at least as large as the value of
   `POINTER_SIZE'.  If it is not equal, you must define the macro
   `POINTERS_EXTEND_UNSIGNED' to specify how pointers are extended to `Pmode'.  */
/* Specify the machine mode that pointers have. After generation of rtl, the
   compiler makes no further distinction between pointers and any other
   objects of this machine mode.  */
#define Pmode	SImode

/* An alias for the machine mode used for memory references to functions being
   called, in `call' RTL expressions.  On most machines this should be
   `QImode'.  */
/* A function address in a call instruction is a byte address (for indexing
   purposes) so give the MEM rtx a byte's mode.  */
#define FUNCTION_MODE	SImode

/* If cross-compiling, don't require stdio.h etc to build libgcc.a.  */
/*
#if defined CROSS_COMPILE && ! defined inhibit_libc
#define inhibit_libc
#endif
*/
/* As an embedded target, we have no libc.  */
#ifndef inhibit_libc
#  define inhibit_libc
#endif

/*}}}*/ 
/*{{{  Exported variables */ 

/* Define the information needed to generate branch and scc insns.  This is
   stored from the compare operation.  Note that we can't use "rtx" here
   since it hasn't been defined!  */
/*
extern struct rtx_def * logic32_compare_op0;
extern struct rtx_def * logic32_compare_op1;
*/

/*}}}*/ 
/*{{{  PERDICATE_CODES.  */ 

/*
#define PREDICATE_CODES					\
  { "stack_add_operand",	{ CONST_INT }},		\
  { "high_register_operand",	{ REG }},		\
  { "low_register_operand",	{ REG }},		\
  { "call_operand",		{ MEM }},		\
  { "fp_displacement_operand",	{ CONST_INT }},		\
  { "sp_displacement_operand",	{ CONST_INT }},		\
  { "di_operand",		{ CONST_INT, CONST_DOUBLE, REG, MEM }},	\
  { "nonimmediate_di_operand",	{ REG, MEM }},		\
  { "add_immediate_operand",	{ REG, CONST_INT }},
*/

/*}}}*/ 


/**************************************************************************/

#define	INIT_SECTION_ASM_OP		"\t.section\t.init"
#define	IDENT_ASM_OP			"\t.ident\t"
#define	SET_ASM_OP				"\t.set\t"

/* It is as good to call a constant function address as to call an address
   kept in a register.  */
#define NO_FUNCTION_CSE

/* Try a machine-dependent way of reloading an illegitimate address
   operand.  If we find one, push the reload and jump to WIN.  This
   macro is used in only one place: `find_reloads_address' in reload.c.  */
/* 
#define LEGITIMIZE_RELOAD_ADDRESS(X,MODE,OPNUM,TYPE,IND_L,WIN)
*/

/* Defining the Output Assembler Language.  */

/* A default list of other sections which we might be "in" at any given
   time.  For targets that use additional sections (e.g. .tdesc) you
   should override this definition in the target-specific file which
   includes this file.  */

/* Define the pseudo-ops used to switch to the .ctors and .dtors sections.

   Same as config/elfos.h but don't mark these section SHF_WRITE since
   there is no shared library problem.  */
#undef CTORS_SECTION_ASM_OP
#define CTORS_SECTION_ASM_OP	"\t.section\t.ctors,\"a\""

#undef DTORS_SECTION_ASM_OP
#define DTORS_SECTION_ASM_OP	"\t.section\t.dtors,\"a\""

/* If defined, a function that outputs assembler code to arrange to
   call the function referenced by SYMBOL at initialization time.  */
/*
#define TARGET_ASM_CONSTRUCTOR  logic32_asm_out_constructor
*/

/* This is like `TARGET_ASM_CONSTRUCTOR' but used for termination
   functions rather than initialization functions.  */
/*
#define TARGET_ASM_DESTRUCTOR   logic32_asm_out_destructor
*/

/* Write the extra assembler code needed to declare a function properly.
   Some svr4 assemblers need to also have something extra said about the
   function's return value.  We allow for that here.

   For 68HC12 we mark functions that return with 'rtc'.  The linker
   will ensure that a 'call' is really made (instead of 'jsr').
   The debugger needs this information to correctly compute the stack frame.

   For 68HC11/68HC12 we also mark interrupt handlers for gdb to
   compute the correct stack frame.  */


/* Output #ident as a .ident.  */

/* output external reference */
#define ASM_OUTPUT_EXTERNAL(FILE,DECL,NAME) \
  {fputs ("\t; extern\t", FILE); \
  assemble_name (FILE, NAME); \
  fputs ("\n", FILE);}

/* Assembler Commands for Exception Regions.  */

/* Default values provided by GCC should be ok. Assuming that DWARF-2
   frame unwind info is ok for this platform.  */

#undef PREFERRED_DEBUGGING_TYPE
#define PREFERRED_DEBUGGING_TYPE DWARF2_DEBUG

/* For the support of memory banks we need addresses that indicate
   the page number.  */
#define DWARF2_ADDR_SIZE 4

/* SCz 2003-07-08: Don't use as dwarf2 .file/.loc directives because
   the linker is doing relaxation and it does not adjust the debug_line
   sections when it shrinks the code.  This results in invalid addresses
   when debugging.  This does not bless too much the HC11/HC12 as most
   applications are embedded and small, hence a reasonable debug info.
   This problem is known for binutils 2.13, 2.14 and mainline.   */
#undef HAVE_AS_DWARF2_DEBUG_LINE

/* Miscellaneous Parameters.  */

/* Define the codes that are matched by predicates in logic32.c.  */

/* This flag, if defined, says the same insns that convert to a signed fixnum
   also convert validly to an unsigned one.  */
#define FIXUNS_TRUNC_LIKE_FIX_TRUNC

/* Max number of bytes we can move from memory to memory in one
   reasonably fast instruction.  */

/* MOVE_RATIO is the number of move instructions that is better than a
   block move.  Make this small on logic32, since the code size grows very
   large with each move.  */
#define MOVE_RATIO		3

/* Define if shifts truncate the shift count which implies one can omit
   a sign-extension or zero-extension of a shift count.  */
#define SHIFT_COUNT_TRUNCATED	1

/* True if VALUE is a signed 16-bit number.  */
#define SMALL_OPERAND(VALUE) \
  ((unsigned HOST_WIDE_INT) (VALUE) + 0x8000 < 0x10000)

/* True if VALUE is an unsigned 16-bit number.  */
#define SMALL_OPERAND_UNSIGNED(VALUE) \
  (((VALUE) & ~(unsigned HOST_WIDE_INT) 0xffff) == 0)

#define SMALL_INT(X) SMALL_OPERAND (INTVAL (X))
#define SMALL_INT_UNSIGNED(X) SMALL_OPERAND_UNSIGNED (INTVAL (X))

/* Define the profitability of saving registers around calls.

   Disable this because the saving instructions generated by
   caller-save need a reload and the way it is implemented,
   it forbids all spill registers at that point.  Enabling
   caller saving results in spill failure.  */
/*	LJL
#define CALLER_SAVE_PROFITABLE(REFS,CALLS) 0
*/

/* This is how to output an insn to push/pop a register on the stack.
   It need not be very fast code.  

   Don't define because we don't know how to handle that with
   the STATIC_CHAIN_REGNUM (soft register).  Saving the static
   chain must be made inside FUNCTION_PROFILER.  */

#if 0
#undef ASM_OUTPUT_REG_PUSH
#undef ASM_OUTPUT_REG_POP
#endif

/* Nonzero if type TYPE should be returned in memory.
   Blocks and data types largers than 4 bytes cannot be returned
   in the register (D + X = 4).  */
#define RETURN_IN_MEMORY(TYPE)				\
    ((TYPE_MODE (TYPE) == BLKmode)			\
     ? (int_size_in_bytes (TYPE) > 4)			\
     : (GET_MODE_SIZE (TYPE_MODE (TYPE)) > 4))

/* A C expression that indicates when it is the called function's
   responsibility to make a copy of arguments passed by invisible
   reference.  Normally, the caller makes a copy and passes the
   address of the copy to the routine being called.  When
   FUNCTION_ARG_CALLEE_COPIES is defined and is nonzero, the caller
   does not make a copy.  Instead, it passes a pointer to the "live"
   value.  The called function must not modify this value.  If it can
   be determined that the value won't be modified, it need not make a
   copy; otherwise a copy must be made.  */
/*
 * LJL
#define FUNCTION_ARG_CALLEE_COPIES(CUM, MODE, TYPE, NAMED)		\
    ((NAMED) && FUNCTION_ARG_PASS_BY_REFERENCE (CUM, MODE, TYPE, NAMED))
*/

