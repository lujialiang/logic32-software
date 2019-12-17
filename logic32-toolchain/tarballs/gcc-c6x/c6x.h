/* Target macro definitions for C6x.

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


/* TODO: 
 * - define `CONDITIONAL_REGISTER_USAGE' for c62x and c67x
 * - change CALL_USED_REGISTERS so that registers are by default assumed not to
 *   be clobbered by function calls.  If we do this, we have to arrange for
 *   them to be saved in the prologue and restored in the epilogue of each
 *   function.
 * - defining EXIT_IGNORE_STACK might be save
 * - add support for trampolines: macros `TRAMPOLINE_TEMPLATE',
 *   `INITIALIZE_TRAMPOLINE'
 * - see `MULSI3_LIBCALL' et al and define them if appropriate
 * - reconsider if we cannot find a better definition for `CONSTANT_ADDRESS_P'
 * - Find out whether GO_IF_MODE_DEPENDENT_ADDRESS ever needs to return 1 on
 *   c6x, because base register increment might always be 4, regardless of
 *   the machine mode (ie. LDB, LDH, or LDW).
 * - Check whether `#define LEGITIMATE_CONSTANT_P  1' is ok
 * - Implement support for PIC, if needed (see node PIC).
 * - ASM_OUTPUT_ASCII default def uses octal escapes in the generated strings.
 *   Supported by TI-asm? By gas?
 * - check ASM_OUTPUT_COMMON
 * - consider defining `ASSEMBLER_DIALECT'
 * - see `ASM_OUTPUT_INTERNAL_LABEL', what is the convention on c6x?
 * - support for debugging: see default def of `DWARF2_UNWIND_INFO'
 */


/* stack frame layout we use for c6x
 *
 *     |                            |
 *     |            ...             | ^ <higher addresses, deeper in stack>
 *     |                            |
 *     |                            |
 *     |============================| <- current FP (in the caller)
 *     |                            |    - `FRAME_GROWS_DOWNWARD' is defined
 *     |         LOCAL VARS         |      -> local vars are allocated at 
 *     |                            |         negative offsets
 *     |                            |    - function prologue allocates space
 *     |                            |      by adjusting SP
 *     |                            |
 *     |                            |
 *     |----------------------------|
 *     |                            |	 - function prologue allocates space 
 *     |     REGISTER SAVE AREA     |	   by adjusting SP                   
 *     |                            |	 - size is computed based on 
 *     |  saved registers that are  |	   `regs_ever_live' and `call_used_regs'
 *     |  not in `call_used_regs'   |
 *     |                            |
 *     |----------------------------|
 *     |                            |
 *     |     DYNAMIC VARIABLES      |   
 *     |                            |
 *     |  as allocated by `alloca'  |
 *     |                            |
 *     |----------------------------| <- `virtual_stack_dynamic_rtx'
 *     |                            |    - instanitated to SP +
 *     |      ACTUAL ARGUMENTS      |      `STACK_DYNAMIC_OFFSET'
 *     |                            |    - `STACK_DYNAMIC_OFFSET' depends on
 *     |       aka `argblock'       |      `current_function_outgoing_args_size'
 *     |                            |      which is the maximum size required
 *     |                            |      for argument passing in this func
 *     |                            |
 *     |----------------------------| <- SP at the normal level in the caller
 *     |                            |    -> `virtual_outgoing_args_rtx' == SP
 *     |                            |    - `virtual_incoming_args_rtx', used
 *     |       RETURN ADDRESS       |       in the callee, must also point to
 *     |                            |       this location, but using FP as the
 *     |                            |       base register, as SP is needed for
 *     |----------------------------|       other purposes in the new frame.
 *     |        DYNAMIC LINK        |       Hence, this equals FP + 2 words.
 *     |                            |
 *     |     a link to caller's     |
 *     |   dynamic link on stack    |
 *     |============================| <- new FP (in the callee)
 *     |                            |    - initialized by prologue of callee
 *     |    LOCAL VARS (callee)     |      to SP after pushing the dynamic link
 *     |                            |
 *     |            ...             |
 */


/**** extern declarations
 */

/* Place for storing away the operands of the `cmpM' standard action. For
 * immediate use in `bCOND' and fellows. See info gccint, node "Standard
 * Names", `bCOND'. */
extern rtx c6x_compare_op0;
extern rtx c6x_compare_op1;


/**** helper definitions
 */

/* make `REG_OK_STRICT' a C expression that can be used in if-statements */

#ifdef REG_OK_STRICT
#define C6X_REG_OK_STRICT 1
#else
#define C6X_REG_OK_STRICT 0
#endif


#define REGNO_A_REGS(NUM)  (NUM)
/* Even though there are only 16 A_REGS on c62x and c67x, rather than 32 on
 * c64x, the numbering remains the same, so the B_REGS have an offset of 32.  */
#define REGNO_B_REGS(NUM)  ((NUM) + 32)


/* This is used whenever we need some value and want to express that it can
 * be chosen arbitrarily, because it will actually be ignored.  */
#define IGNORED  0

/* This is used whenever we want to show that we deliberately defined somthing
 * to be empty.  */
#define EMPTY

/* The register to pass return values in (both scalar values and addresses to 
 * structs (aka aggregate values).  
 *
 * On c6x, we use A4. See spru187i.pdf, p. 8-17.  */

#define RETVAL_REGNO  REGNO_A_REGS(4)

/* A register to use as scratch reg when no pseudos can be allocated.  */

#define SCRATCH_REGNO  REGNO_A_REGS(14)

/* Expands to (1) REGNO, if given the number of a hard register, or (2) the
 * hard register that the pseudo REGNO is allocated to.  */

#define C6X_EFFECTIVE_REGNO(REGNO) \
  (HARD_REGISTER_NUM_P (REGNO) \
   ? REGNO\
   : reg_renumber[REGNO])

/**** node: Driver
 *
 * - Controlling how the driver GCC runs the compilation passes.  */


/**** node: Run-time Target
 */

/* Run-time compilation parameters selecting different hardware subsets.  */

extern int target_flags;

/* - This series of macros is to allow compiler command arguments to enable or
 *   disable the use of optional features of the target machine.  
 * - used with `target_flags' */

#define MASK_BIG_ENDIAN 0x00000001  /* Generate big endian code.  */

#define TARGET_BIG_ENDIAN	(target_flags & MASK_BIG_ENDIAN)

/* This macro defines names of command options to set and clear bits in
   `target_flags'.  Its definition is an initializer with a subgrouping for
   each command option. See function `set_target_switch' for how switches
   are decoded in general and how option name "" is decoded in particular.   */
/* TODO: add detailed descriptions for switches and options to `invoke.texi'  */

#define TARGET_SWITCHES							\
{									\
  { "big-endian",	MASK_BIG_ENDIAN,				\
      N_("Generate big endian code") },					\
  { "little-endian",	-MASK_BIG_ENDIAN,				\
      N_("Generate little endian code") },				\
  SUBTARGET_SWITCHES							\
  { "",			TARGET_DEFAULT | TARGET_CPU_DEFAULT,		\
      NULL }								\
}

/* Defaults for the extra and special `TARGET_SWITCHES' entries  */

#ifndef TARGET_DEFAULT
#define TARGET_DEFAULT 0
#endif

#ifndef TARGET_CPU_DEFAULT
#define TARGET_CPU_DEFAULT 0
#endif

#ifndef SUBTARGET_SWITCHES
#define SUBTARGET_SWITCHES
#endif

/* Prints the particular machine description choice to stderr.  */
#define TARGET_VERSION fprintf (stderr, " (GNU assembler syntax, COFF)");

/* `TARGET_OPTIONS':
   This macro is similar to `TARGET_SWITCHES' but defines names of command
   options that have values.  Its definition is an initializer with a
   subgrouping for each command option.  */

/* eg:
	extern const char *ia64_fixed_range_string;
	extern const char *ia64_tls_size_string;
	#define TARGET_OPTIONS \
	{									\
	  { "fixed-range=", 	&ia64_fixed_range_string,			\
	      N_("Specify range of registers to make fixed")},			\
	  { "tls-size=",	&ia64_tls_size_string,				\
	      N_("Specify bit size of immediate TLS offsets")},			\
	}
*/

/* Sometimes certain combinations of command options do not make sense on a
   particular target machine.  You can define a macro `OVERRIDE_OPTIONS' to
   take account of this.  This macro, if defined, is executed once just after
   all the command options have been parsed.  */

/*
 * eg: #define OVERRIDE_OPTIONS ia64_override_options ()
*/

/* Some machines may desire to change what optimizations are performed for
   various optimization levels.  This macro, if defined, is executed once just
   after the optimization level is determined and before the remainder of the
   command options have been parsed.  Values set in this macro are used as the
   default values for the other command line options.  */

/* #define OPTIMIZATION_OPTIONS(LEVEL,SIZE) */


/**** node: Per-Function Data
 *
 * - Defining data structures for per-function information.
 *
 * TODO
 * keywords: 
 * - machine_function
 * - INIT_EXPANDERS
 * - init_machine_status
 * - __builtin_return_address
 * - eg: arm_current_func_type
 */


/**** node: Storage Layout
 *
 * - Defining sizes and alignments of data.  */

/* set this to 1, iff bit 0 is the _most_ significant one in a byte
 * - cf.: TI spru189f, p.36 (in section 2.1) -- talks about the 24MSBs, which
 *   are numbered from 8 through to 31 -> BITS_BIG_ENDIAN must be 0
 */
#define BITS_BIG_ENDIAN 0

/* endianness
 * - cf.: TI spru190d, p. 75 -- both little and big endianness are supported
 *   and can be selected by the LENDIAN pin on the device
 * - BYTES_BIG_ENDIAN and WORDS_BIG_ENDIAN may be non-constant values and can
 *   thus be offered as a gcc runtime option
 */

/* 1, if most significant byte of a word is the lowest numbered.  */
#define BYTES_BIG_ENDIAN (TARGET_BIG_ENDIAN != 0)

/* 1, if most significant word of a multiword number is the lowest
 * numbered.  */
#define WORDS_BIG_ENDIAN (TARGET_BIG_ENDIAN != 0)

/* - used only when compiling `libgcc2.c'
 * - same meaning as `WORDS_BIG_ENDIAN', but must be a compile-time constant 
 * - value is typically based on preprocessor defines
 * - FIXME: 
 *   - `MULTILIB_OPTIONS' (info gccint node "Target Fragment") might be useful
 *     for having two versions of libgcc.a compiled (little and big endian)
 *   - a, say, `-mlittle-endian' option to gcc could be translated by some 
 *     specs into a `-D__LITTLE_ENDIAN__' to the preprocessor when compiling
 *     `libgcc.a' (see config/sparc/sparc.h)  */

/* FIXME */
#define LIBGCC2_WORDS_BIG_ENDIAN 0
/* 
  #if defined (__LITTLE_ENDIAN__)
  #define LIBGCC2_WORDS_BIG_ENDIAN 0
  #else
  #define LIBGCC2_WORDS_BIG_ENDIAN 1
  #endif
*/

/* FIXME */
#define FLOAT_WORDS_BIG_ENDIAN 0

/* The number of bits in an addressable storage unit (byte)  */
#define BITS_PER_UNIT 8

/* Width in bits of a "word", which is the contents of a machine register.
   Note that this is not necessarily the width of data type `int'.  */
#define BITS_PER_WORD 32

/* we use the default  */

/* #define MAX_BITS_PER_WORD BITS_PER_WORD  */

/* Width of a word, in units (bytes).  */
#define UNITS_PER_WORD  (BITS_PER_WORD / BITS_PER_UNIT)

/* we use the default  */
/* #define MIN_UNITS_PER_WORD UNITS_PER_WORD  */

/* Width of a pointer, in bits; the general 32bit-registers are used 
 * for memory addressing with TI-C6x
 * - see also: convert_memory_address
 * - we use the default, which is `BITS_PER_WORD'  */

/* #define POINTER_SIZE BITS_PER_WORD */

/* we do not have to define POINTERS_EXTEND_UNSIGNED as `POINTER_SIZE' equals
 * size of `Pmode'  */

/* don't promote anything for the moment  */
/* #define PROMOTE_MODE ...  */
/* #define PROMOTE_FUNCTION_ARGS ...  */
/* #define PROMOTE_FUNCTION_RETURN ...  */
/* #define PROMOTE_FOR_CALL_ONLY ...  */

/* Allocation boundary (in *bits*) for storing arguments in argument list.  */
#define PARM_BOUNDARY 32

/* The minimum alignment (in bits) enforced by hardware for the stack pointer
 * on this machine (For the c6x, we choose 32 bits, which is the amount a 
 * register is changed by when pre-/post-incrementing, eg. *A4++[ucst15])  */

#define STACK_BOUNDARY 32

/* Define this macro if you wish to preserve a certain alignment for the stack
 * pointer, greater than what the hardware enforces  */

/* #define PREFERRED_STACK_BOUNDARY  */

/* A C expression that evaluates true if `PREFERRED_STACK_BOUNDARY' is not
 * guaranteed by the runtime and we should emit code to align the stack at the
 * beginning of `main'.  */

/* #define FORCE_PREFERRED_STACK_BOUNDARY_IN_MAIN  */

/* Alignment required for a function entry point, in bits.  For c6x we use 
 * 256 bits (8*32 bits), the width of a fetch packet. */

#define FUNCTION_BOUNDARY 256

/* Biggest alignment that any data type can require on this machine, in bits.
 * Used for BLKmode (eg, see: function.c:`assign_stack_temp_for_type')  */

#define BIGGEST_ALIGNMENT 32

/* This may be left undefined. It is only used in the ADA front end.  */
/* #define MINIMUM_ATOMIC_ALIGNMENT 8  */

/* Biggest alignment that any structure or union field can require on this
 * machine, in bits.  */

#define BIGGEST_FIELD_ALIGNMENT BIGGEST_ALIGNMENT

/* An expression for the alignment of a structure field FIELD, overriding the
 * the normal alignment computation. Not used if alignment has been set by
 * user, using the `__attribute__ ((aligned (N)))' construct.  */

/* #define ADJUST_FIELD_ALIGN(FIELD, COMPUTED) ...   */

/* use the default */
/* #define MAX_OFILE_ALIGNMENT BIGGEST_ALIGNMENT */

/* A C expression to compute the alignment for a variable in the static store.
 * We leave it undefined and hence always use BASIC-ALIGN.  */

/* #define DATA_ALIGNMENT(TYPE, BASIC-ALIGN) ... */

/* A C expression to compute the alignment given to a constant that is being
 * placed in memory.  We leave it undefined and hence always use BASIC-ALIGN.
 */

/* #define CONSTANT_ALIGNMENT(CONSTANT, BASIC-ALIGN) ... */

/* A C expression to compute the alignment for a variable in the local store.
 * We leave it undefined and hence always use BASIC-ALIGN.  */

/* #define LOCAL_ALIGNMENT(TYPE, BASIC-ALIGN) ... */

/* Alignment in bits to be given to a structure bit-field that follows an empty
 * field such as `int : 0;'. Undefined => no special alignment in this case.  */

/* #define EMPTY_FIELD_BOUNDARY ... */

/* Number of bits which any structure or union's size must be a multiple of.
 * Each structure or union's size is rounded up to a multiple of this. Need not
 * be a constant, see: `config/arm/arm.h'. We arbitrarily choose rounding to 
 * word size, here.  */

#define STRUCTURE_SIZE_BOUNDARY BITS_PER_WORD

/* 1 if instructions will fail to work if given data is not on the nominal
 * alignment.  0 if instructions will merely go slower in that case.  */

#define STRICT_ALIGNMENT 1

/* Do imitate the way many other C compilers handle alignment of bit-fields and
 * the structures that contain them. The type written for a bit-field (`int',
 * `short', ...) imposes an alignment for the entire structure, as if the
 * structure really did contain an ordinary field of that type.  */

#define PCC_BITFIELD_TYPE_MATTERS 1

/* FIXME: the meaning of this macro is unclear to me, but it's used only once
 * in `stor-layout.c', so leave it undefined as most of the backends do.  */

/* #define BITFIELD_NBYTES_LIMITED ... */

/* Return 1 if a structure or array containing FIELD should be accessed using
 * `BLKMODE'. See `c4x.h' for an example.  */

/* #define MEMBER_TYPE_FORCES_BLK ... */

/* The default is to round COMPUTED up to a multiple of SPECIFIED. Leave it at
 * that as not a single backend seems to define this macro.  */

/* #define ROUND_TYPE_SIZE ... */

/* Similar to `ROUND_TYPE_SIZE', but sizes and alignments are specified in
 * units (bytes). If you define `ROUND_TYPE_SIZE', you must also define this
 * macro and they must be defined consistently with each other.  */

/* #define ROUND_TYPE_SIZE_UNIT ... */

/* An expression for the alignment of a type (given by TYPE as a tree node) if
 * the alignment computed in the usual way is COMPUTED and the alignment
 * explicitly specified was SPECIFIED. We keep that sort of fine-grained ABI
 * specification for later and leave it undefined.  */

/* #define ROUND_TYPE_ALIGN(TYPE, COMPUTED, SPECIFIED) ... */

/* An integer expression for the size in bits of the largest integer machine
 * mode that should actually be used.  All integer machine modes of this size
 * or smaller can be used for structures and unions with the appropriate sizes.
 * If this macro is undefined, `GET_MODE_BITSIZE (DImode)' is assumed.  */
/* TODO: This should be extended to 64 bits for c64x as it supports the LDDW
 * and STDW instructions for double word loads and stores.  */

#define MAX_FIXED_MODE_SIZE 32

/* Define this macro to be nonzero if the port is prepared to handle insns
 * involving vector mode MODE.  At the very least, it must have move patterns
 * for this mode.  */
/* Many backends do not define this and neither do we, hoping that gcc finds
 * a good workaround.  */

/* #define VECTOR_MODE_SUPPORTED_P ... */

/* If defined, an expression of type `enum machine_mode' that specifies the
 * mode of the save area operand of a `save_stack_LEVEL' named pattern (*note
 * Standard Names::). You would most commonly define this macro if the
 * `save_stack_LEVEL' patterns need to support both a 32- and a 64-bit mode.
 * The default is `Pmode'.  */

/* #define STACK_SAVEAREA_MODE(SAVE_LEVEL) ... */

/* If defined, an expression of type `enum machine_mode' that specifies the
 * mode of the size increment operand of an `allocate_stack' named pattern
 * (*note Standard Names::). You would most commonly define this macro if the
 * `allocate_stack' pattern needs to support both a 32- and a 64-bit mode.  
 * The default is `word_mode'.  */

/* #define STACK_SIZE_MODE ... */

/*  A code distinguishing the floating point format of the target machine.
 *  There is no need to define this macro when the format is IEEE (default).  */

/* #define TARGET_FLOAT_FORMAT IEEE_FLOAT_FORMAT */

/* When defined, this macro should be true if MODE has a NaN representation.
 * By default, this macro is true if MODE is a floating-point mode and the
 * target floating-point format is IEEE.  */

/* #define MODE_HAS_NANS(MODE) ... */

/* This macro should be true if MODE can represent infinity.  By default, the
 * macro is true when MODE is a floating-point mode and the target format is
 * IEEE.  */

/* #define MODE_HAS_INFINITIES */

/* True if MODE distinguishes between positive and negative zero. The default
 * definition is true if MODE is a floating-point mode and the target format is
 * IEEE. */

/* #define MODE_HAS_SIGNED_ZEROS */

/* If defined, this macro should be true for MODE if it has at least one
 * rounding mode in which `x' and `-x' can be rounded to numbers of different
 * magnitude.  Two such modes are towards -infinity and towards +infinity. 
 * The default definition of this macro is true if MODE is a floating-point
 * mode and the target format is IEEE.  */

/* #define MODE_HAS_SIGN_DEPENDENT_ROUNDING ... */

/* If defined, this macro should be true if the prevailing rounding mode is
 * towards zero. Not defining this macro is equivalent to returning zero.  */

/* #define ROUND_TOWARDS_ZERO ... */

/* This macro should return true if floats with SIZE bits do not have a NaN or
 * infinity representation, but use the largest exponent for normal numbers
 * instead.  */

/* #define LARGEST_EXPONENT_IS_NORMAL ... */

/* This target hook returns `true' if bit-fields in the given RECORD_TYPE are
 * to be laid out following the rules of Microsoft Visual C/C++ (see info gcc
 * "Storage Layout" for more details).  */

/* #define TARGET_MS_BITFIELD_LAYOUT_P ... */


/**** node: Type Layout
 *
 * - Defining sizes and properties of basic user data types.
 *
 * - part of the ABI specification
 * - see defaults.h for default definitions
 *     - defs are partly based on `BITS_PER_UNIT' and `UNITS_PER_WORD'
 */

/* A C expression for the size in bits of the type `int' on the target machine.
 * If you don't define this, the default is one word.  */

#define INT_TYPE_SIZE 32


/* A C expression for the size in bits of the type `short' on the target
 * machine.  If you don't define this, the default is half a word.  (If this
 * would be less than one storage unit, it is rounded up to one unit.)  */

#define SHORT_TYPE_SIZE 16


/* A C expression for the size in bits of the type `long' on the target
 * machine.  If you don't define this, the default is one word.  */
/* FIXME: Maybe we should make use of the 40bit add instructions at some point,
 * which would probably require us to introduce a new machine mode.
 * Keep it simple for now and use only 32 bits, which is a nice power of two
 * and should make no problems.  */

#define LONG_TYPE_SIZE 32


/* Define this if the size used for the type `long' by a native Ada compiler
 * differs from that used by C. Default: LONG_TYPE_SIZE  */

/* #define ADA_LONG_TYPE_SIZE 32 */


/* Maximum number for the size in bits of the type `long' on the target
 * machine.  If this is undefined, the default is `LONG_TYPE_SIZE'.  */

/* #define MAX_LONG_TYPE_SIZE ... */


/* A C expression for the size in bits of the type `long long' on the target
machine.  */

#define LONG_LONG_TYPE_SIZE 64


/* A C expression for the size in bits of the type `char' on the target
 * machine.  If you don't define this, the default is `BITS_PER_UNIT'.  */

/* #define CHAR_TYPE_SIZE BITS_PER_UNIT */


/* A C expression for the size in bits of the C++ type `bool' and C99 type
 * `_Bool' on the target machine.  If you don't define this, and you probably
 * shouldn't, the default is `CHAR_TYPE_SIZE'.  */

/* #define BOOL_TYPE_SIZE */


/* A C expression for the size in bits of the type `float' on the target
 * machine.  If you don't define this, the default is one word.  */

/* #define FLOAT_TYPE_SIZE ... */


/* A C expression for the size in bits of the type `double' on the target
 * machine.  If you don't define this, the default is two words.  */

/* #define DOUBLE_TYPE_SIZE ... */


/* A C expression for the size in bits of the type `long double' on the target
 * machine.  If you don't define this, the default is two words.  */

/* #define LONG_DOUBLE_TYPE_SIZE ... */


/* What is the meaning of this? It seems to be used hardly anywhere, so
 * leave it undefined.  */

/* #define TARGET_FLT_EVAL_METHOD ... */


/* Hardly used anywhere in gcc, so we stick with the default.  */

/* #define WIDEST_HARDWARE_FP_SIZE ... */


/* Mandatory.  An expression whose value is 1 or 0, according to whether the
 * type `char' should be signed or unsigned by default.  The user can always
 * override this default with the options `-fsigned-char' and
 * `-funsigned-char'.  */

#define DEFAULT_SIGNED_CHAR  1


/* Optional.  A C expression to determine whether to give an `enum' type only
 * as many bytes as it takes to represent the range of possible values of that
 * type.  Default: 0 (don't do it, but use `int')  */

/* #define DEFAULT_SHORT_ENUMS ... */


/* A C expression for a string describing the name of the data type to use for
 * size values.  The typedef name `size_t' is defined using the contents of the
 * string.  On c6x, we use the default "long unsigned int".  */

/* #define SIZE_TYPE "long unsigned int" */


/* A C expression for a string describing the name of the data type to use for
 * the result of subtracting two pointers.  The typedef name `ptrdiff_t' is
 * defined using the contents of the string. Use default.  */

/* #define PTRDIFF_TYPE "long int" */


/* A C expression for a string describing the name of the data type to use for
 * wide characters.  The typedef name `wchar_t' is defined using the contents
 * of the string. Use default.  */

/* #define WCHAR_TYPE "int" */


/* A C expression for the size in bits of the data type for wide characters.
 * This is used in `cpp', which cannot make use of `WCHAR_TYPE'.  */

/* #define WCHAR_TYPE_SIZE INT_TYPE_SIZE */


/* Maximum number for the size in bits of the data type for wide characters.
 * Use default.  */

/* #define MAX_WCHAR_TYPE_SIZE WCHAR_TYPE_SIZE */


/* A C expression for the size in bits of the type used for gcov counters on
 * the target machine.  If you don't define this, the default is one
 * `LONG_TYPE_SIZE' in case it is greater or equal to 64-bit and
 * `LONG_LONG_TYPE_SIZE' otherwise.  You may want to re-define the type to
 * ensure atomicity for counters in multithreaded programs.  */ 
/* FIXME: might become important at some point. For now, leave it undefined.  */

/* #define GCOV_TYPE_SIZE ... */


/* A C expression for a string describing the name of the data type to use for
 * wide characters passed to `printf' and returned from `getwc'.  The typedef
 * name `wint_t' is defined using the contents of the string.  See `SIZE_TYPE'
 * above for more information. Use default. */

/* #define WINT_TYPE "unsigned int" */


/* A C expression for a string describing the name of the data type that can
 * represent any value of any standard or extended signed integer type.  The
 * typedef name `intmax_t' is defined using the contents of the string.  See
 * `SIZE_TYPE' above for more information. Use default.  */

/* #define INTMAX_TYPE ... */


/* Similar to INTMAX_TYPE, but for unsigned integers. Use default.  */

/* #define UINTMAX_TYPE ... */


/* Tells gcc where to store the bit indicating whether a function called
 * through a pointer-to-member-function is virtual. This is somewhat involved,
 * see info "Type Layout" for doc. Use default.  */

/* #define TARGET_PTRMEMFUNC_VBIT_LOCATION ... */


/* This macro allows the target to change to use "function descriptors" rather
 * than function pointers in vtables.  */
/* FIXME: I'm not deep enough into vtables and "function descriptors" to tell
 * whether we need this. So leave undefined for now.  */

/* #define TARGET_VTABLE_USES_DESCRIPTORS ... */


/* By default, the vtable entries are void pointers, the so the alignment is
 * the same as pointer alignment.  The value of this macro specifies the
 * alignment of the vtable entry in bits.  We don't need special alignment,
 * so we leave it undefined.  */

/* #define TARGET_VTABLE_ENTRY_ALIGN ... */


/* There are a few non-descriptor entries in the vtable at offsets below zero.
 * If these entries must be padded (say, to preserve the alignment specified by
 * `TARGET_VTABLE_ENTRY_ALIGN'), set this to the number of words in each data
 * entry.  We don't need this for now, so leave undefined.  */

/* #define TARGET_VTABLE_DATA_ENTRY_DISTANCE ... */


/*** Escape Sequences
 * - By default, GCC assumes that the C character escape sequences take on
 *   their ASCII values for the target.  If this is not correct, you must
 *   explicitly define all of the macros below.
 * - We don't have to do this for c6x.
 */

/* The default definitions: 
#  define TARGET_BELL 007
#  define TARGET_BS 010
#  define TARGET_TAB 011
#  define TARGET_NEWLINE 012
#  define TARGET_VT 013
#  define TARGET_FF 014
#  define TARGET_CR 015
#  define TARGET_ESC 033
*/


/**** node: Registers
 *
 * - Naming and describing the hardware registers.  */


/*** subnode: Register Basics
 *
 * - Number and kinds of registers.
 * - see TI spru189f: section "Features and Options of the TMS320C62x/C64x/C67x"
 *   - C62x,C67x: 
 *         - 32 general purpose registers of 32 bit word length
 *       - C64x:
 *         - 64 general purpose registers of 32 bit word length
 */

/* Number of hardware registers known to the compiler (get numbers 0 through
 * `FIRST_PSEUDO_REGISTER-1').  Apparently, this has to be a compile-time
 * constant as it is used for the number of elements in array definitions.
 * The `c62x' and `c67x' actually don't have 64 general purpose regs, but only
 * 32. To be able to support runtime-selection of one of c62x, c67x, or c64x,
 * we have to indicate the register usage to gcc depending on the target flags.
 * The macro `CONDITIONAL_REGISTER_USAGE' allows for this.
 *
 * Note that this is not the number of the first register to use for
 * `gen_reg_rtx'.  The first number to use is LAST_VIRTUAL_REGISTER + 1 (see
 * variable `reg_rtx_no'), which accounts for the so called virtual registers,
 * a special kind of pseudo registers that are used for fixed purposes and
 * are not available for dynamic allocation.  One of them is
 * VIRTUAL_STACK_VARS_REGNUM, representing the base address of local variables
 * on the stack during RTX generation.  Any references to this virtual register 
 * will be instantiated to (substituted by) references to hard registers as
 * soon as GCC can compute the necessary offsets.
 */

#define FIRST_PSEUDO_REGISTER 64

/* An initializer that says which registers are used for fixed purposes all
 * throughout the compiled code and are therefore not available for general
 * allocation. The Nth number is 1 if register N is fixed, 0 otherwise. 
 * May be overridden by macro `CONDITIONAL_REGISTER_USAGE' or at run-time by
 * option `-ffixed-REG'.
 *
 * The asm names for the fixed registers must be specified by `REGISTER_NAMES'
 * and optionally by `ADDITIONAL_REGISTER_NAMES', below.
 *
 * FIXME: look at which reg the TI-CCS compiler uses for the stack pointer.
 * For now, we use REGNO_B_REGS(15) as stack pointer, which must be marked as
 * fixed according to info gccint, doc for `STACK_POINTER_REGNUM'.
 *
 * In order to increase/decrease the stack pointer in the prologue/epilogue,
 * we need a scratch register on top of that.  Since we are forbidden to
 * allocate new pseudos (`no_new_pseudos'), we dedicate `SCRATCH_REGNO' (A14)
 * to this special purpose and mark it fixed and call-used.
 *
 * The FRAME_POINTER_REGNUM must be marked as fixed and call-used, as well.
 * The return register `RETVAL_REGNO' (A4) need not be fixed, but only
 * call-used in order to exclude it from being saved at the beginning of the
 * function. The register to pass the return value in is automatically marked
 * live by `diddle_return_value' and need not be handled in `EPILOGUE_USES'.
 *
 * Note: CALL_USED_REGISTERS must be a superset of FIXED_REGISTERS, so mark
 * the affected regs there, too.  */

#define FIXED_REGISTERS {                        \
    /* A_REGS */                                 \
    0, 0, 0, 0, 0, 0, 0, 0, /*  0- 7 */          \
    0, 0, 0, 0, 0, 0, 1, 1, /*  8-15 */          \
    0, 0, 0, 0, 0, 0, 0, 0, /* 16-23 */          \
    0, 0, 0, 0, 0, 0, 0, 0, /* 24-31 */          \
    /* B_REGS */                                 \
    0, 0, 0, 0, 0, 0, 0, 0, /*  0- 7 */          \
    0, 0, 0, 0, 0, 0, 0, 1, /*  8-15 */          \
    0, 0, 0, 0, 0, 0, 0, 0, /* 16-23 */          \
    0, 0, 0, 0, 0, 0, 0, 0, /* 24-31 */          \
}

/* An initializer of which the Nth number is `1' if the corresponding register
 * is normally clobbered by function calls or is a fixed register.  This macro
 * therefore identifies the registers that are not available for general
 * allocation of values that must live across function calls.  For `0', the
 * compiler automatically saves the register on function entry and restores it
 * on function exit, if the register is used within the function.
 *
 * This macro may be overridden by defining `CONDITIONAL_REGISTER_USAGE' or at
 * run-time by option `-fcall-used-REG'.
 *
 * The convention on c6x is, to caller-save A0-A9, B0-B9 and (on c64x) A16-A31
 * and B16-B31. See spru187i.pdf, p. 8-19.  */

#define CALL_USED_REGISTERS {                    \
    /* A_REGS */                                 \
    1, 1, 1, 1, 1, 1, 1, 1, /*  0- 7 */          \
    1, 1, 0, 0, 0, 0, 1, 1, /*  8-15 */          \
    1, 1, 1, 1, 1, 1, 1, 1, /* 16-23 */          \
    1, 1, 1, 1, 1, 1, 1, 1, /* 24-31 */          \
    /* B_REGS */                                 \
    1, 1, 1, 1, 1, 1, 1, 1, /*  0- 7 */          \
    1, 1, 0, 0, 0, 0, 0, 1, /*  8-15 */          \
    1, 1, 1, 1, 1, 1, 1, 1, /* 16-23 */          \
    1, 1, 1, 1, 1, 1, 1, 1, /* 24-31 */          \
}

/* Like `CALL_USED_REGISTERS' except this macro doesn't require that the entire
 * set of `FIXED_REGISTERS' be included.  (`CALL_USED_REGISTERS' must be a
 * superset of `FIXED_REGISTERS').  This macro is optional.  If not specified,
 * it defaults to the value of `CALL_USED_REGISTERS'.  */

/* #define CALL_REALLY_USED_REGISTERS ... */

/* A C expression that is nonzero if it is not permissible to store a value of
 * mode MODE in hard register number REGNO across a call without some part of
 * it being clobbered.  For most machines this macro need not be defined.  It
 * is only required for machines that do not preserve the entire contents of a
 * register across a call.  */

/* #define HARD_REGNO_CALL_PART_CLOBBERED(REGNO, MODE) ... */

/* Zero or more C statements that may conditionally modify five variables
 * `fixed_regs', `call_used_regs', `global_regs', `reg_names', and
 * `reg_class_contents', to take into account any dependence of these register
 * sets on target flags. For details, see info gccint "Register Basics".  */
/* FIXME: for the moment, no special provisions for c62x/c67x are made, but
 * they should be at some point.  */

/* #define CONDITIONAL_REGISTER_USAGE ... */

/* If this C expression evaluates to nonzero, `setjmp' and related functions
 * won't save and `longjmp' won't restore the registers.  To compensate, the
 * compiler avoids putting variables in registers in functions that use
 * `setjmp'.  Like most of the backends we use the default, which is to save
 * the registers.  */

/* #define NON_SAVING_SETJMP 0 */

/* Macros for target machines with register windows.  We don't need them.  */

/*
  #define INCOMING_REGNO(OUT) ...
  #define OUTGOING_REGNO(IN) ...
  #define LOCAL_REGNO(REGNO) ...
*/


/*** subnode: Allocation Order
 *
 * - Order in which registers are allocated.
 *
 * FIXME: Maybe, we could prevent gcc from allocating registers for general
 * purposes which would be of more benefit if used as condition registers
 * ([A0], A1, A2, B0, B1, B2) or for circular addressing (A4-A7, B4-B7).
 * But for now, do nothing.  */

/* List the order in which to allocate registers.  Each register must be listed
 * once, even those in FIXED_REGISTERS. Eg. see `config/alpha/alpha.h'  */

/* #define REG_ALLOC_ORDER ... */

/* A C statement (sans semicolon) to choose the order in which to allocate hard
 * registers for pseudo-registers local to a basic block.  */

/* #define ORDER_REGS_FOR_LOCAL_ALLOC ... */


/*** subnode: Values in Registers
 *
 * - What kinds of values each reg can hold.
 *
 * Macros that describe which kinds of values (specifically, which machine
 * modes) each register can hold, and how many consecutive registers are needed
 * for a given mode.  */


/* A C expression for the number of consecutive hard registers, starting at
 * register number REGNO, required to hold a value of mode MODE. We have to
 * round up to the number of either fully or partially used registers.  */

#define HARD_REGNO_NREGS(REGNO, MODE) \
    ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD)


/* Value is 1 if hard register REGNO can hold a value of machine-mode MODE.  
 * 40-bit values must be kept in an `(even, odd)' register pair, see spru189f,
 * p.36.  */
  
#define HARD_REGNO_MODE_OK(REGNO, MODE)    \
  (GET_MODE_SIZE (MODE) <= UNITS_PER_WORD   \
   ? 1                                      \
   : ((REGNO) & 1) == 0)


/* A C expression that is nonzero if a value of mode MODE1 is accessible in
 * mode MODE2 without copying.
 *
 * If `HARD_REGNO_MODE_OK (R, MODE1)' and `HARD_REGNO_MODE_OK (R, MODE2)' are
 * always the same for any R, then `MODES_TIEABLE_P (MODE1, MODE2)' should be
 * nonzero. Otherwise it must return zero.  */

#define MODES_TIEABLE_P(MODE1, MODE2)					  \
  ((MODE1) == (MODE2)							  \
   || (((MODE1) == QImode || (MODE1) == HImode || (MODE1) == SImode)	  \
       &&                                                                 \
       ((MODE2) == QImode || (MODE2) == HImode || (MODE2) == SImode)))


/* Define this macro if the compiler should avoid copies to/from `CCmode'
 * registers.  You should only define this macro if support for copying to/from
 * `CCmode' is incomplete.  */
/* FIXME: This should probably undefined, since we do not use CCmode, but
 * BImode on C6x (see function `c6x_expand_compare').  */

#define AVOID_CCMODE_COPIES 1



/*** subnode: Leaf Functions
 *
 * - Renumbering registers for leaf functions.  */


/* Name of a char vector, indexed by hard register number, which contains 1 for
 * a register that is allowable in a candidate for leaf function treatment. For
 * details, see info gccint.  Define this macro only if the target machine
 * offers a way to optimize the treatment of leaf functions.  For c6x,
 * we currently don't concern ourselves with optimizations like this.  */ 

/* #define `LEAF_REGISTERS' */


/* If defined, a C expression whose value is the register number to which REGNO
 * should be renumbered, when a function is treated as a leaf function.  Must
 * only be defined when `LEAF_REGISTERS' is defined.   */

/* #define `LEAF_REG_REMAP(REGNO)' */



/*** subnode: Stack Registers
 *
 * - Handling a register stack such as 80387.
 * - see: comment at top of `reg-stack.c' for detailed explanation  */


/* Define this if the machine has any stack-like registers.  For c6x, we
 * haven't any such thing.  */ 

/* #define `STACK_REGS' */


/* The number of the first stack-like register.  This one is the top of the
 * stack.  */ 

/* #define `FIRST_STACK_REG' */


/* The number of the last stack-like register.  This one is the bottom of the
 * stack.  */ 

/* #define `LAST_STACK_REG' */



/**** node: Register Classes
 *
 * - Defining the classes of hardware registers.  
 * - reads:
 *     - general registers
 *         - spru189f.pdf, section 2.1 "General-Purpose Register-Files"
 *     - addressing: 
 *         - spru189f.pdf, section 3.8 "Addressing Modes"
 *         - all registers can at least perform linear (ie "normal") addressing
 *         - for regs A4-A7 and B4-B7 not to be in circular addressing mode,
 *           the AMR register must have its default value, cf. section 3.8
 *         - section 3.8.3: "Syntax for Load/Store Address Generation"
 *	       - address offsets
 *	       - pre/post-increment of base address register
 *     - condition registers (for conditional execution): 
 *         spru189f.pdf, section 3.6 "Conditional Operations"
 * - register numbering in c6x.h:
 *   reg name	: A0 A1 ... A31 B0 B1 ... B31
 *   hard regno	:  0  1      31 32 33      63
 * - On c62x and c67x, we have only registers A0-A15 and B0-B15.  We don't
 *   change the numbering, here, but will disable A16-A31 and B16-B31 by means
 *   of macro `CONDITIONAL_REGISTER_USAGE' on these targets.
 */


/**** start of private definitions for c6x, node Register Classes ****/

/* Nonzero, if REGNO is in the GENERAL_REGS class (see enum `reg_class').
 *
 * NB: we ignore the warnings like `comparison of unsigned expression >= 0 is
 * always true', because both signed and unsigned REGNO variables occur in the
 * compiler. Sometimes, REGNO is -1, if reg_renumber says that some pseudo
 * has not been allocated to any hard register.  We don't want to return `true'
 * for those regs and hence need to preserve the `>= 0'-comparison.
 */

#define GENERAL_REGNO_P(REGNO) \
  ((REGNO) >= 0 && (REGNO) <= 63)

#define A_REGS_REGNO_P(REGNO) \
  ((REGNO) >= 0 && (REGNO) <= 31)

/**** end of private definitions for c6x, node Register Classes ****/


/* An enumeral type that must be defined with all the register class names as
 * enumeral values.  `NO_REGS' must be first.  `ALL_REGS' must be the last
 * register class, followed by one more enumeral value, `LIM_REG_CLASSES',
 * which is not a register class but rather tells how many classes there are.
 * A subclass must come earlier than any of its superclasses in reg_class.
 * For more details, see: info gccint, node "Register Classes".  */

enum reg_class
{
  NO_REGS,
  /* spru189f.pdf, section 3.6 "Conditional Operations":
   * - valid condition regs: B0, B1, B2, A1, A2  */
  CONDITION_A_REGS,   
  CONDITION_B_REGS,   
  A_REGS,
  B_REGS,
  /* One of the classes must be named `GENERAL_REGS'.  There is nothing terribly
   * special about the name, but the operand constraint letters `r' and `g'
   * specify this class.  If `GENERAL_REGS' is the same as `ALL_REGS', just
   * define it as a macro which expands to `ALL_REGS'.  */
#define GENERAL_REGS  ALL_REGS
  ALL_REGS,
  LIM_REG_CLASSES
};


/* Must be defined to the number of distinct register classes.  */

#define N_REG_CLASSES  (int) LIM_REG_CLASSES


/* An initializer containing the names of the register classes as C string
 * constants.  These names are used in writing some of the debugging dumps.
 * Mandatory.  */ 

#define REG_CLASS_NAMES \
{                       \
  "NO_REGS",            \
  "CONDITION_A_REGS",   \
  "CONDITION_B_REGS",   \
  "A_REGS",             \
  "B_REGS",             \
  "ALL_REGS",           \
}


/* Mandatory. An initializer containing the contents of the register classes,
 * as integers which are bit masks.  The initializer constists of
 * `N_REG_CLASSES' many groups containing `N_REG_INTS' (see def in regclass.c)
 * many unsigned integers. The Nth group specifies the contents of class N.
 * The integers are masks of 32 (hard coded, not HOST_BITS_PER_INT) bit width
 * each, with the first mask being least significant.  If Bit 0 (rightmost) of
 * mask 3 (which is the fourth) is set, then hard register 3 * 32 + 0 is member
 * of the group we are about to define.
 *
 * For c6x, all 64 registers of c64x must be specified.  Hence, we need two 32
 * bit masks per class.  On c62x and c67x, where only A0-A15 and B0-B15 are
 * supported, we will disable the excess registers by means of defining
 * `CONDITIONAL_REGISTER_USAGE' appropriately.
 *
 * See info gccint, node "Register Classes" for more explanations, among others
 * regarding the required order of definition of the class contents.  */ 

#define REG_CLASS_CONTENTS \
{                                                                    \
  {0x00000000, 0x00000000},   /* NO_REGS */                      \
  {0x00000006, 0x00000000},   /* CONDITION_A_REGS:     A1, A2 */ \
  {0x00000000, 0x00000007},   /* CONDITION_B_REGS: B0, B1, B2 */ \
  {0xFFFFFFFF, 0x00000000},   /* A_REGS */                       \
  {0x00000000, 0xFFFFFFFF},   /* B_REGS */                       \
  {0xFFFFFFFF, 0xFFFFFFFF},   /* ALL_REGS */                     \
}


/* Mandatory.  A C expression whose value is a register class containing hard
 * register REGNO.  In general there is more than one such class; choose a
 * class which is "minimal", meaning that no smaller class also contains the
 * register.  */ 

#define REGNO_REG_CLASS(REGNO) \
  (  (REGNO) >=  1       && (REGNO) <=  2	? CONDITION_A_REGS   \
   : (REGNO) >= 32       && (REGNO) <= 34	? CONDITION_B_REGS   \
   : (REGNO) >= 0        && (REGNO) <= 31	? A_REGS             \
   : (REGNO) >= 32       && (REGNO) <= 63	? B_REGS             \
   : NO_REGS)


/* Mandatory.  Must expand to one of the classes of enum `reg_class'.
 * Should be the biggest class in which all registers can validly be used as
 * base registers.  A base register is one used in an address which is the
 * register value plus a displacement.  
 *
 * On c6x, all registers can in principle serve as base regs.
 * See also: `INDEX_REG_CLASS', `REGNO_OK_FOR_BASE_P', 'REGNO_OK_FOR_INDEX_P',
 * and `C6X_REG_OK_FOR_BASE_P', below.  */ 

#define BASE_REG_CLASS	ALL_REGS


/* Optional.  This is a variation of the `BASE_REG_CLASS' macro which allows
 * the selection of a base register in a mode dependent manner.  The
 * default is `BASE_REG_CLASS', which we use for c6x.  */ 

/* #define `MODE_BASE_REG_CLASS(MODE)' */


/* A macro whose definition is the name of the class to which a valid index
 * register must belong.  An index register is one used in an address where its
 * value is either multiplied by a scale factor or added to another register
 * (as well as added to a displacement).  */ 

#define INDEX_REG_CLASS  ALL_REGS


/* A C expression which defines the machine-dependent operand constraint
 * letters for register classes.  If C is such a letter, the value should be
 * the register class corresponding to it.  Otherwise, the value should be
 * `NO_REGS'.
 *
 * The register letter `r', corresponding to class `GENERAL_REGS',
 * will not be passed to this macro; you do not need to handle it.  */ 

#define REG_CLASS_FROM_LETTER(C) \
  (  (C) == 'a' ? A_REGS                                             \
   : (C) == 'b' ? B_REGS                                             \
   : (C) == 'A' ? CONDITION_A_REGS                                   \
   : (C) == 'B' ? CONDITION_B_REGS                                   \
   : NO_REGS)
   

/* A C expression which is nonzero if register number REGNO is suitable for use
 * as a base register in operand addresses.  It may be either a suitable hard
 * register or a pseudo register that has been allocated such a hard register.
 * Need not be defined, if `REGNO_MODE_OK_FOR_BASE_P', below, is defined.
 *
 * See also: `C6X_REG_OK_FOR_BASE_P' */ 

#define REGNO_OK_FOR_BASE_P(REGNO) \
  (GENERAL_REGNO_P (REGNO) || GENERAL_REGNO_P (reg_renumber[REGNO]))


/* A C expression that is just like `REGNO_OK_FOR_BASE_P', except that that
 * expression may examine the mode of the memory reference in MODE.  For c6x,
 * the mode is irrelevant in this respect, so use the default.  */

/* #define REGNO_MODE_OK_FOR_BASE_P(NUM, MODE)  REGNO_OK_FOR_BASE_P(NUM)  */


/* A C expression which is nonzero if register number REGNO is suitable for use
 * as an index register in operand addresses.  It may be either a suitable hard
 * register or a pseudo register that has been allocated such a hard register.
 *
 * The difference between an index register and a base register is that the
 * index register may be scaled.  If an address involves the sum of two
 * registers, neither one of them scaled, then either one may be labeled the
 * "base" and the other the "index"; but whichever labeling is used must fit
 * the machine's constraints of which registers may serve in each capacity.
 * The compiler will try both labelings, looking for one that is valid, and
 * will reload one or both registers only if neither labeling works.  */ 

#define REGNO_OK_FOR_INDEX_P(REGNO) \
  (GENERAL_REGNO_P (REGNO) || GENERAL_REGNO_P (reg_renumber[REGNO]))


/* A C expression that places additional restrictions on the register class to
 * use when it is necessary to copy value X into a register in class CLASS.
 * The value is a register class; perhaps CLASS, or perhaps another, smaller
 * class.  Sometimes returning a more restrictive class makes better code. For
 * more details, see info gccint, node "Register Classes".  For c6x, we refrain
 * from imposing additional restrictions.  */ 

#define PREFERRED_RELOAD_CLASS(X, CLASS)  (CLASS)


/* Like `PREFERRED_RELOAD_CLASS', but for output reloads instead of input
 * reloads.  If you don't define this macro, the default is to use CLASS,
 * unchanged.  */ 

/* #define PREFERRED_OUTPUT_RELOAD_CLASS(X, CLASS) */


/* A C expression that places additional restrictions on the register class to
 * use when it is necessary to be able to hold a value of mode MODE in a reload
 * register for which class CLASS would ordinarily be used.  Don't define this
 * macro unless the target machine has limitations which require the macro to
 * do something nontrivial.  For c6x, we don't need any such limitations.  */ 

/* #define LIMIT_RELOAD_CLASS (MODE, CLASS) */


/* Many machines have some registers that cannot be copied directly to or from
 * memory or even from other types of registers.  If copying X to a register
 * CLASS in MODE requires an intermediate register, you should define
 * this macro to return the largest register class all of whose registers can
 * be used as intermediate registers or scratch registers. For c6x, we don't
 * need this as every register in GENERAL_REGS can be copied to and from
 * memory directly.  */

/* #define SECONDARY_INPUT_RELOAD_CLASS(CLASS, MODE, X) */


/* Optional. Like SECONDARY_INPUT_RELOAD_CLASS, but for output reloads.  */

/* #define SECONDARY_OUTPUT_RELOAD_CLASS(CLASS, MODE, X) */


/* If secondary reloads are required and the same class would always be
 * returned for input and output reloads, you should define this macro
 * instead of the two macros above.  The the default definition of these
 * two macros will be made equal to this macro.  */

/* #define SECONDARY_RELOAD_CLASS(CLASS, MODE, X) */


/* Certain machines have the property that some registers cannot be copied to
 * some other registers without using memory.  For c6x, this isn't the case, so
 * we leave this undefined.  */ 

/* #define SECONDARY_MEMORY_NEEDED(CLASS1, CLASS2, M) */

/* Normally when `SECONDARY_MEMORY_NEEDED' is defined, the compiler allocates a
 * stack slot for a memory location needed for register copies.  If this macro
 * is defined, the compiler instead uses the memory location defined by this
 * macro.  We haven't defined `SECONDARY_MEMORY_NEEDED' and thus need not
 * define this macro either.  */ 

/* #define SECONDARY_MEMORY_NEEDED_RTX (MODE) */


/* We haven't defined `SECONDARY_MEMORY_NEEDED' and thus need not
 * define this macro either.  For the semantics of this macro, see info gccint,
 * node "Register Classes".  */ 

/* #define `SECONDARY_MEMORY_NEEDED_MODE (MODE)' */


/* Optional.  On some machines, it is risky to let hard registers live across
 * arbitrary insns.  Typically, these machines have instructions that require
 * values to be in specific registers (like an accumulator), and reload will
 * fail if the required hard register is used for another purpose across such
 * an insn.
 *
 * Define `SMALL_REGISTER_CLASSES' to be an expression with a nonzero value on
 * these machines.  When this macro has a nonzero value, the compiler will try
 * to minimize the lifetime of hard registers.
 *
 * It is always safe to define this macro with a nonzero value, but if you
 * unnecessarily define it, you will reduce the amount of optimizations that
 * can be performed in some cases.  If you do not define this macro with a
 * nonzero value when it is required, the compiler will run out of spill
 * registers and print a fatal error message.  For most machines, you should
 * not define this macro at all.  */

/* #define SMALL_REGISTER_CLASSES */


/* Optional.  A C expression whose value is nonzero if pseudos that have been
 * assigned to registers of class CLASS would likely be spilled because
 * registers of CLASS are needed for spill registers.  
 *
 * The default value of this macro returns 1 if CLASS has exactly one register
 * and zero otherwise.  On most machines, this default should be used.  
 *
 * Only define this macro to some other expression if pseudos allocated by
 * `local-alloc.c' end up in memory because their hard registers were needed
 * for spill registers.  If this macro returns nonzero for those classes, those
 * pseudos will only be allocated by `global.c', which knows how to reallocate
 * the pseudo to another register.  If there would not be another register
 * available for reallocation, you should not change the definition of this
 * macro since the only effect of such a definition would be to slow down
 * register allocation.  */ 

/* #define CLASS_LIKELY_SPILLED_P(CLASS) */


/* A C expression for the maximum number of consecutive registers of class
 * CLASS needed to hold a value of mode MODE.
 *
 * This is closely related to the macro `HARD_REGNO_NREGS'.  In fact, the value
 * of the macro `CLASS_MAX_NREGS (CLASS, MODE)' should be the maximum value of
 * `HARD_REGNO_NREGS (REGNO, MODE)' for all REGNO values in the class CLASS.
 *
 * This macro helps control the handling of multiple-word values in the reload
 * pass.  */

#define CLASS_MAX_NREGS(CLASS, MODE) \
  ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD)


/* If defined, a C expression that returns nonzero for a CLASS for which a
 * change from mode FROM to mode TO is invalid.
 *
 * For the example, loading 32-bit integer or floating-point objects into
 * floating-point registers on the Alpha extends them to 64 bits.  Therefore
 * loading a 64-bit object and then storing it as a 32-bit object does not
 * store the low-order 32 bits, as would be the case for a normal register.
 * Use this macro to prevent this.  See info gccint, node "Register Classes"
 * for more details.  */

/* #define CANNOT_CHANGE_MODE_CLASS(FROM, TO, CLASS) */


/* Mandatory.  A C expression that defines the machine-dependent operand
 * constraint letters (`I', `J', `K', ... `P') that specify particular ranges
 * of integer values.  If C is one of those letters, the expression should
 * check that VALUE, an integer, is in the appropriate range and return 1 if
 * so, 0 otherwise.  If C is not one of those letters, the value should be 0
 * regardless of VALUE.
 *
 * On c6x, we need the following value ranges ( ucst<n> runs from 0 to 2^n - 1,
 * scst<n> runs from -2^(n-1) to 2^(n-1) - 1 ):
 * - general (format: letter, range, some of the depending Asm commands):
 *     I: ucst4	   CMPGT(U), NOP
 *     J: ucst5	   SET, SHL/SHR/SHRU, STB/STH/STW, LDB/LDH/LDW
 *     K: ucst15   LDB/LDH/LDW (for index reg, base reg must be B14 or B15)
 *     L: scst5	   ADD, SADD, AND
 *     M: scst16   ADDK, MVK, MVKL
 *     N: scst21   B
 * - C64-specific (we don't assign letters for those yet):
 *     scst7	ADDKPC
 *     ucst3	ADDKPC
 *     scst12	BNOP
 *     scst10	BDEC, BPOS  */

#define CONST_OK_FOR_LETTER_P(VALUE, C)	\
  (  (C) == 'I' ? (VALUE) >=  0x0000 && (VALUE) <=  0x000F               \
   : (C) == 'J' ? (VALUE) >=  0x0000 && (VALUE) <=  0x001F               \
   : (C) == 'K' ? (VALUE) >=  0x0000 && (VALUE) <=  0x7FFF               \
   : (C) == 'L' ? (VALUE) >= -0x0010 && (VALUE) <=  0x000F               \
   : (C) == 'M' ? (VALUE) >= -0x8000 && (VALUE) <=  0x7FFF               \
   : (C) == 'N' ? (VALUE) >= -0x100000 && (VALUE) <=  0x0FFFFF           \
   : 0)


/* A C expression that defines the machine-dependent operand constraint letters
 * that specify particular ranges of `const_double' values (`G' or `H').
 *
 * If C is one of those letters, the expression should check that VALUE, an RTX
 * of code `const_double', is in the appropriate range and return 1 if so, 0
 * otherwise.  If C is not one of those letters, the value should be 0
 * regardless of VALUE.
 *
 * `const_double' is used for all floating-point constants and for `DImode'
 * fixed-point constants.  A given letter can accept either or both kinds of
 * values.  It can use `GET_MODE' to distinguish between these kinds.  
 *
 * FIXME: maybe change this as soon as we add support for floating point.  */

#define CONST_DOUBLE_OK_FOR_LETTER_P(VALUE, C)  0


/* Optional.  A C expression that defines the optional machine-dependent
 * constraint letters that can be used to segregate specific types of operands,
 * usually memory references, for the target machine.  Any letter that is not
 * elsewhere defined and not matched by `REG_CLASS_FROM_LETTER' may be used.
 *
 * If it is required for a particular target machine, it should return 1 if
 * VALUE corresponds to the operand type represented by the constraint letter
 * C.  If C is not defined as an extra constraint, the value returned should be
 * 0 regardless of VALUE.
 *
 * For c6x, we don't need any extra constraints.  */

/* #define EXTRA_CONSTRAINT(VALUE, C) */


/* A C expression that defines the optional machine-dependent constraint
 * letters, amongst those accepted by `EXTRA_CONSTRAINT', that should be
 * treated like memory constraints by the reload pass.  See info gccint, node
 * "Register Classes" for details.  */

/* #define EXTRA_MEMORY_CONSTRAINT(C) */


/* A C expression that defines the optional machine-dependent constraint
 * letters, amongst those accepted by `EXTRA_CONSTRAINT', that should be
 * treated like address constraints by the reload pass.  See info gccint, node
 * "Register Classes" for details.  */

/* #define EXTRA_ADDRESS_CONSTRAINT(C) */



/**** node: Stack and Calling
 *
 * - Defining which way the stack grows and by how much.  
 * - see top of this file for stack frame layout of c6x
 */



/*** subnode: Frame Layout
 */


/* Define this macro if pushing a word onto the stack moves the stack pointer
 * to a smaller address.  */ 

#define STACK_GROWS_DOWNWARD


/* This macro defines the operation used when something is pushed on the stack.
 * In RTL, a push operation will be `(set (mem (STACK_PUSH_CODE (reg sp))) ...)'
 * (see function `emit_single_push_insn').
 * On c6x, we want SP to always point to the element currently on top. Hence,
 * we use PRE_DEC in order to point to a fresh slot before storing the value.
 */

#define STACK_PUSH_CODE  PRE_DEC


/* If defined, frame pointer + `STARTING_FRAME_OFFSET' must point to the first
 * free byte above the local variables after the function prologue is finished.
 * Otherwise it must point to the lowest address occupied by local variables.
 * For details of static stack slot allocation see `assign_stack_local_1'.
 */ 

#define FRAME_GROWS_DOWNWARD


/* If defined, argument pointer (*) + `FIRST_PARM_OFFSET' must point to the
 * first free byte above the arguments the caller put on the stack after the
 * function prologue is finished.  Otherwise this sum must point to the lowest
 * address occupied by these args.
 * (*) AP may be defined to be an alias for FP
 *
 * On c6x, the callee sees its incoming args at a defined number of words above
 * SP, which is assigned to FP/AP (we use FP as AP as well on c6x, because
 * there is no offset problem preventing us from doing so) in the prologue. 
 * We don't want to concern ourselves with the size of the argument block, so
 * we chose to have args growing upward with FP pointing to the first slot
 * in the arg block (ie. to the lowest address).  */

/* #define ARGS_GROW_DOWNWARD */


/* Offset from the frame pointer to below (when not `FRAME_GROWS_DOWNWARD') or
 * above (when `FRAME_GROWS_DOWNWARD') the first local variable slot to be
 * allocated.  For details of allocation see function `assign_stack_local_1'.
 * On c6x, the static local vars start directly below FP, so no offset
 * required.  */

#define STARTING_FRAME_OFFSET  0


/* This is the amount by which the virtual register `virtual_outgoing_args_rtx'
 * is offset from the real stack pointer (`stack_pointer_rtx').  When outgoing
 * arguments are put onto the stack, `virtual_outgoing_args_rtx' is used as
 * base register, see: initialization of variable `argblock' in function
 * `expand_call'. (Note: this is not true when PUSH_ARGS is defined -- SP is
 * used directly in that case, see call to `emit_single_push_insn' in
 * `emit_push_insn').  This allows for a scratch area between these two
 * pointers which could be of use for saving extra information related to
 * calls. For c6x, we don't need it.  */

#define STACK_POINTER_OFFSET  0


/* See discussion at `ARGS_GROW_DOWNWARD'.  For c6x, the AP points 2 words
 * above the FP after the function prologue.  In between on the stack are the
 * return address and the dynamic chain pointer.  */

#define AP_FP_OFFSET  (2 * (UNITS_PER_WORD))

#define FIRST_PARM_OFFSET(FUNDECL)   (AP_FP_OFFSET)


/* Offset from the stack pointer to an item dynamically allocated on the stack,
 * e.g., by `alloca' (see internal function for details
 * `allocate_dynamic_stack_space').
 *
 * There is a default definition, which is correct on most machines including
 * c6x.  As we use `ACCUMULATE_OUTGOING_ARGS' (prologue allocates space for
 * outgoing parms) and `OUTGOING_REG_PARM_STACK_SPACE' (the allocated space
 * includes space for register parms), the default def returns the value
 * `current_function_outgoing_args_size' + `STACK_POINTER_OFFSET', the offset
 * from SP to the bottom of dynamic variables that have been allocated so far.
 *
 * Each time, a new dynamic variable is allocated, code is emitted to adjust SP
 * by the requested size and to move `virtual_stack_dynamic_rtx'
 * (== SP + STACK_DYNAMIC_OFFSET), which now points to the bottom of the new
 * variable, to a pseudo reg in order to stabilize the address to the variable.
 * */

/* #define STACK_DYNAMIC_OFFSET(FUNDECL) */


/* Optional.  A C expression whose value is RTL representing the address in a
 * stack frame where the pointer to the caller's frame is stored.  Assume that
 * FRAMEADDR is an RTL expression for the address of the stack frame itself.
 *
 * On c6x, we use the default, which is to return the value of FRAMEADDR--that
 * is, the stack frame address is also the address of the stack word that
 * points to the previous frame.  */ 

/* #define DYNAMIC_CHAIN_ADDRESS(FRAMEADDR) */


/* Optional.  A C expression that produces the machine-specific code to setup
 * the stack so that arbitrary frames can be accessed.  For example, on the
 * SPARC, we must flush all of the register windows to the stack before we can
 * access arbitrary stack frames.  On c6x, we don't need this.  */ 

/* #define SETUP_FRAME_ADDRESSES */


/* Optional.  A C expression that contains an rtx that is used to store the
 * address of the current frame into the built in `setjmp' buffer.  The default
 * is `virtual_stack_vars_rtx'. On c6x, this equals FP + 0 and is correct.  */ 

/* #define BUILTIN_SETJMP_FRAME_VALUE */


/* A C expression whose value is RTL representing the value of the return
 * address for the frame COUNT steps up from the current frame, after the
 * prologue.  FRAMEADDR is the hard frame pointer of the COUNT frame, or the
 * hard frame pointer of the COUNT - 1 frame if `RETURN_ADDR_IN_PREVIOUS_FRAME'
 * is defined.  
 *
 * The value of the expression must always be the correct address
 * when COUNT is zero, but may be `NULL_RTX' if there is not way to determine
 * the return address of other frames.
 *
 * On c6x, the return address is the next word above the dynamic link on the
 * stack.  */ 

#define RETURN_ADDR_RTX(COUNT, FRAMEADDR) \
  gen_rtx_MEM (Pmode, plus_constant (FRAMEADDR, UNITS_PER_WORD))
  

/* Define this if the return address of a particular stack frame is accessed
 * from the frame pointer of the previous stack frame.  Not the case on c6x.
 * */ 

/* #define RETURN_ADDR_IN_PREVIOUS_FRAME */


/* Optional.  A C expression whose value is RTL representing the location of
 * the incoming return address at the beginning of any function, before the
 * prologue.  This RTL is either a `REG', indicating that the return value is
 * saved in `REG', or a `MEM' representing a location in the stack.
 *
 * You only need to define this macro if you want to support call frame
 * debugging information like that provided by DWARF 2.
 *
 * If this RTL is a `REG', you should also define `DWARF_FRAME_RETURN_COLUMN'
 * to `DWARF_FRAME_REGNUM (REGNO)'.
 *
 * On c6x, support for debugging will be added later.
 * */

/* #define INCOMING_RETURN_ADDR_RTX */


/* Optional.  A C expression whose value is an integer giving the offset, in
 * bytes, from the value of the stack pointer register to the top of the stack
 * frame at the beginning of any function, before the prologue.  The top of the
 * frame is defined to be the value of the stack pointer in the previous frame,
 * just before the call instruction.  
 *
 * You only need to define this macro if you want to support call frame
 * debugging information like that provided by DWARF 2.
 *
 * On c6x, support for debugging will be added later.
 * */ 

/* #define INCOMING_FRAME_SP_OFFSET */


/* A C expression whose value is an integer giving the offset, in bytes, from
 * the argument pointer to the canonical frame address (cfa).  The final value
 * should coincide with that calculated by `INCOMING_FRAME_SP_OFFSET'.  Which
 * is unfortunately not usable during virtual register instantiation.  
 *
 * The default value for this macro is `FIRST_PARM_OFFSET (fundecl)', which is
 * correct for most machines; in general, the arguments are found immediately
 * before the stack frame.  Note that this is not the case on some targets that
 * save registers into the caller's frame, such as SPARC and rs6000, and so
 * such targets need to define this macro.
 *
 * You only need to define this macro if the default is incorrect, and you want
 * to support call frame debugging information like that provided by DWARF 2.
 *
 * On c6x, support for debugging will be added later.
 * */ 

/* #define ARG_POINTER_CFA_OFFSET(FUNDECL) */


/* Define this macro if the stack size for the target is very small.  This has
 * the effect of disabling gcc's built-in `alloca', though `__builtin_alloca'
 * is not affected.  Not the case on c6x.  */ 

/* #define SMALL_STACK */



/*** subnode: Exception Handling
 */

/* TODO */

/* See, if we can get away without definitions for this. */


/*** subnode: Stack Checking
 */

/* TODO */

/* Hopefully the defaults suffice for the moment.  */


/*** subnode: Frame Registers
 */

/* The register number of the stack pointer register, which must also be a
 * fixed register according to `FIXED_REGISTERS'.  On most machines, the
 * hardware determines which register this is.
 *
 * On c6x, we use REGNO_B_REGS(15), see spru187i.pdf, p. 8-17.  */

#define STACK_POINTER_REGNUM  REGNO_B_REGS(15)


/* The register number of the frame pointer register, which is used to access
 * automatic variables in the stack frame.  On c6x, we use REGNO_A_REGS(15)
 * (no conditional reg and no circular addressing reg).  The reg need not be in
 * `FIXED_REGISTERS', because `global_alloc' ensures automatically that 
 * `FRAME_POINTER_REGNUM' is not used as an ordinary reg if it cannot be
 * eliminated.
 *
 * On c6x, we use REGNO_A_REGS(15), see spru187i.pdf, p. 8-17.  */

#define FRAME_POINTER_REGNUM  REGNO_A_REGS(15)


/* Only needed, when it is not possible to calculate the offset between the
 * frame pointer and the automatic variables until after register allocation
 * has been completed.  This can happen when the register save area for regs
 * used in the function is between hard FP and the automatic variables on the
 * stack.  Its size is not known until after register allocation has been
 * completed. On c6x, the automatic variables start directly below the hard
 * FP, so we don't need the FP elimination mechanism but use just FP to stand
 * for both FP and hard FP.  */

/* #define HARD_FRAME_POINTER_REGNUM */


/* The register number of the arg pointer register, which is used to access the
 * function's argument list.  GCC allows to make this equal to
 * FRAME_POINTER_REGNUM, which we do on c6x.  
 * See also: enum `global_rtl_index' in rtl.h.
 *
 * `instantiate_virtual_regs' (called by `rest_of_compilation') instantiates
 * "virtual register" `virtual_incoming_args_rtx' to `arg_pointer_rtx' +
 * FIRST_PARM_OFFSET(fndecl).  Now that AP = FP, `FIRST_PARM_OFFSET' must be
 * defined relative to FP.  */

#define ARG_POINTER_REGNUM  FRAME_POINTER_REGNUM
     

/* On some machines, the return address is not at a fixed offset from the frame
 * pointer or stack pointer or argument pointer.  This register can be defined
 * to point to the return address on the stack, and then be converted by
 * `ELIMINABLE_REGS' into either the frame pointer or stack pointer.
 *
 * Do not define this macro unless there is no other way to get the return
 * address from the stack.  On c6x, the return address is the next word above
 * the dynamic link on the stack (see def of `RETURN_ADDR_RTX').  No need to
 * define this.  */ 

/* #define RETURN_ADDRESS_POINTER_REGNUM */


/* Register number used for passing a function's static chain pointer.  The
 * static chain register need not be a fixed register.  For details, see info
 * gccint, node "Frame Registers".  
 *
 * On c6x, we use A10 as the static chain register.
 */ 

#define STATIC_CHAIN_REGNUM  REGNO_A_REGS(10)


/* Register number, as seen by the callee, used for passing a function's static
 * chain pointer.  Needed only when on a machine there are different reg
 * numbers for incoming regs and their corresponding outgoing regs (the case on
 * machines with register windows).  Not needed on c6x.  */

/* #define STATIC_CHAIN_INCOMING_REGNUM */


/* Only needed, if the static chain is passed in memory.  On c6x, we define
 * `STATIC_CHAIN_REGNUM' instead.  */ 

/* #define STATIC_CHAIN */
/* #define STATIC_CHAIN_INCOMING */


/* This macro specifies the maximum number of hard registers that can be saved
 * in a call frame.  This is used to size data structures used in DWARF2
 * exception handling.  For details, see info gccint, node "Frame Registers".
 * On c6x, debugging support will be implemented later.  For now, we use the
 * default, which is FIRST_PSEUDO_REGISTER.  */ 

/* #define DWARF_FRAME_REGISTERS  FIRST_PSEUDO_REGISTER */


/* This macro is similar to `DWARF_FRAME_REGISTERS', but is provided for
 * backward compatibility in pre GCC 3.0 compiled code.  On c6x, use the
 * default.  */ 

/* #define PRE_GCC3_DWARF_FRAME_REGISTERS  DWARF_FRAME_REGISTERS */



/*** subnode: Elimination
 */


/* A C expression which is nonzero if a function must have and use a frame
 * pointer.  If its value is nonzero the function will have a frame pointer.
 * If not, FP might be eliminated by `eliminate_regs_in_insn', called in
 * `reload_as_needed', called in `reload'.  
 *
 * The expression can in principle examine the current function and decide
 * according to the facts, but using constant 0 or 1 suffices as well.
 *
 * On c6x, we cannot handle the change of offsets when FP is missing and
 * therefore isn't pushed on the stack in the prologue.  We avoid this problem
 * by making FP mandatory, at least for the moment.  See definition of
 * `FRAME_POINTER_REGNUM' for more info.  */

#define FRAME_POINTER_REQUIRED  1


/* A C statement to store in the variable DEPTH-VAR the difference between the
 * frame pointer and the stack pointer values immediately after the function
 * prologue.  The value would be computed from information such as the result
 * of `get_frame_size ()' and the tables of registers `regs_ever_live' and
 * `call_used_regs'.  
 *
 * Mandatory, unless `ELIMINABLE_REGS' is defined.  If `FRAME_POINTER_REQUIRED'
 * is defined to always be true, you may set FP_SP_DIFF to anything.
 *
 * For c6x, `FRAME_POINTER_REQUIRED' is currently always 1, so define this to
 * set FP_SP_DIFF to an arbitrary value that won't be used anywhere.
 * */ 
#define INITIAL_FRAME_POINTER_OFFSET(FP_SP_DIFF)  (FP_SP_DIFF) = IGNORED


/* Optional.  If defined, this macro specifies a table of register pairs used
 * to eliminate unneeded registers that point into the stack frame.  For
 * details, see info gccint, node Elimination.   */

/* #define ELIMINABLE_REGS */


/* Optional, unless `ELIMINABLE_REGS' is defined.  A C expression that returns
 * nonzero if the compiler is allowed to try to replace register number
 * FROM-REG with register number TO-REG.  */ 

/* #define CAN_ELIMINATE(FROM-REG, TO-REG) */


/* Optional, unless `ELIMINABLE_REGS' is defined.  The macro sets OFFSET-VAR to
 * the initial difference between the specified pair of registers.  */

/* #define INITIAL_ELIMINATION_OFFSET(FROM-REG, TO-REG, OFFSET-VAR) */



/*** subnode: Stack Arguments
 */


/* A C expression whose value is nonzero if an argument declared in a prototype
 * as an integral type smaller than `int' should actually be passed as an
 * `int'.  In addition to avoiding errors in certain cases of mismatch, it also
 * makes for better code on certain machines.  
 *
 * On c6x, we use the default, which is 0.  */ 

/* #define PROMOTE_PROTOTYPES  0 */


/* Optional.  If nonzero, `emit_push_insn' will emit push insns will be used to pass outgoing arguments
 * (except for some cases in which `must_preallocate' is set).
 * On c6x, we want to define `ACCUMULATE_OUTGOING_ARGS', which conflicts with
 * this macro, so leave it undefined.  */ 

/* #define PUSH_ARGS */


/* A C expression that is the number of bytes actually pushed onto the stack
 * when an instruction attempts to push BYTES bytes.  On c6x, we always push a
 * multiple of four bytes, regardless of the number of bytes requested.  */ 

#define PUSH_ROUNDING(BYTES)  (((BYTES) + 3) & ~3)


/* Must be defined to a C expression. If nonzero, conflicts with PUSH_ARGS.
 *
 * If nonzero, the maximum amount of space required for outgoing arguments (by
 * any function calls made in the current function) will be computed and placed
 * into the variable `current_function_outgoing_args_size'.  No space will be
 * pushed onto the stack for each call; instead, the function prologue must
 * increase the stack frame size by this amount.  
 *
 * If nonzero, SP won't change throughout the whole function. When arguments
 * must be put onto the stack, they will be put to the `argblock' at
 * `virtual_outgoing_args_rtx', which is the sum of the constant SP and
 * `STACK_POINTER_OFFSET'.  When functions must be called in order to compute
 * an argument for a call just being prepared, the _same_ argblock is used.
 * This can cause problems, because of arguments already initialized and now
 * being overwritten by the args for the subcall.  To avert this, GCC maintains
 * a `stack_usage_map', which it uses to save away parms in time that would
 * otherwise be clobbered.  */ 

#define ACCUMULATE_OUTGOING_ARGS  1


/* Optional.  Define this macro if functions should assume that stack space has
 * been allocated for arguments even when their values are passed in registers.
 * For details, see info gccint, node "Stack Arguments".
 *
 * On c6x, as for now, we don't pass arguments in registers and hence don't
 * need this.  */ 

/* #define REG_PARM_STACK_SPACE(FNDECL) */


/* Optional.  Similar to REG_PARM_STACK_SPACE, above.  For details, see info
 * gccint, node "Stack Arguments".
 *
 * On c6x, as for now, we don't pass arguments in registers and hence don't
 * need this.  */ 


/* #define MAYBE_REG_PARM_STACK_SPACE */
/* #define FINAL_REG_PARM_STACK_SPACE(CONST_SIZE, VAR_SIZE) */


/* Define this in order to instruct gcc to allocate the stack area for
 * arguments also passed in registers as part of the argblock it allocates for
 * normal parms.  The space for these arguments is counted in
 * `current_function_outgoing_args_size' in this case.
 *
 * If not defined, it would presumably be the responsibility 
 * of the function prologue to allocate `reg_parm_stack_space'.  The space for
 * reg parms on stack is subtracted from `current_function_outgoing_args_size'
 * (indirectly) by the function `compute_argument_block_size'.
 *
 * ??? Who stores the reg+stack args in this case and are there constraints as
 * to where this `reg_parm_stack_space' has to be located? Does it have to be
 * adjacent to the argblock? (I would guess so.) The default def of
 * `STACK_DYNAMIC_OFFSET' suggests that dynamic vars are start (in case of
 * downwards growing stack) above the argblock _and_ the reg_parm_stack_space,
 * even when counted outside of `outgoing_args_size'  
 *
 * On c6x, we want to have GCC do the work for us, so define it.  */

#define OUTGOING_REG_PARM_STACK_SPACE


/* Optional.  If defined, `initialize_argument_information' doesn't skip the
 * `reg_parm_stack_space' area when calling `locate_and_pad_parm' for
 * stack-only parameters.  Normally, a stack-only parameter is placed on the
 * stack beyond the `reg_parm_stack_space' area.
 *
 * Irrelevant for c6x, as nothing is passe in registers and there is no
 * `reg_parm_stack_space' right now.  */ 

/* #define STACK_PARMS_IN_REG_PARM_AREA */


/* A C expression that should indicate the number of bytes of its own arguments
 * that a function pops on returning, or 0 if the function pops no arguments
 * and the caller must therefore pop them all after the function returns.  For
 * details, see info gccint, node "Stack Arguments".
 *
 * On c6x, the function doesn't pop any args.  */ 

#define RETURN_POPS_ARGS(FUNDECL, FUNTYPE, STACKSIZE)  0


/* Optional.  A C expression that indicates the number of bytes a call sequence
 * pops off the stack.  It is added to the value of `RETURN_POPS_ARGS' when
 * compiling a function call.  CUM is the variable in which all arguments to
 * the called function have been accumulated.  */

/* #define CALL_POPS_ARGS(CUM) */



/*** subnode: Register Arguments
 */


/* Mandatory.  A C expression that controls whether a function argument is
 * passed in a register, and which register. For details, see info gccint, node
 * "Register Arguments".
 *
 * On c6x, we return NULL in order to pass the argument exclusively on the
 * stack.  */ 

#define FUNCTION_ARG(CUM, MODE, TYPE, NAMED)  NULL


/* Mandatory.  A C expression that evaluates to nonzero if we have to pass
 * this arg at least on the stack (This doesn't prevent it from being 
 * passed in registers as well as on the stack).  For details, see info gccint,
 * node "Register Arguments".
 *
 * On c6x, we never need this nonzero, because we currently don't use registers
 * for argument passing.  */

#define MUST_PASS_IN_STACK(MODE, TYPE)  0


/* Optional.  Define this macro if the target machine has "register windows",
 * so that the register in which a function sees an arguments is not
 * necessarily the same as the one in which the caller passed the argument.  
 *
 * Not the case on c6x, so we leave it undefined.  */ 

/* #define FUNCTION_INCOMING_ARG(CUM, MODE, TYPE, NAMED) */


/* Optional.  A C expression for the number of words, at the beginning of an
 * argument, that must be put in registers.  The value must be zero for
 * arguments that are passed entirely in registers or that are entirely pushed
 * on the stack.  
 *
 * Currently not needed on c6x.  */ 

/* #define FUNCTION_ARG_PARTIAL_NREGS(CUM, MODE, TYPE, NAMED) */


/* Optional.  A C expression that indicates when an argument must be passed by
 * reference.  If nonzero for an argument, a copy of that argument is made in
 * memory and a pointer to the argument is passed instead of the argument
 * itself.  The pointer is passed in whatever way is appropriate for passing a
 * pointer to that type.  If there are other reasons for which an argument
 * should be passed in memory, they will be, even when this macro is not
 * defined. 
 *
 * On c6x, we don't have any own reasons to add, so we leave it undefined.  */

/* FUNCTION_ARG_PASS_BY_REFERENCE(CUM, MODE, TYPE, NAMED) */


/* Optional.  If defined, a C expression that indicates when it is the called
 * function's responsibility to make a copy of arguments passed by invisible
 * reference.  Normally, the caller makes a copy and passes the address of the
 * copy to the routine being called.  When `FUNCTION_ARG_CALLEE_COPIES' is
 * defined and is nonzero, the caller does not make a copy.  Instead, it passes
 * a pointer to the "live" value.  The called function must not modify this
 * value.  If it can be determined that the value won't be modified, it need
 * not make a copy; otherwise a copy must be made.  
 *
 * This would be an optimization to consider at a later time.  For now, we
 * leave this undefined, having the caller make the copy of any value that
 * is passed by reference.  */ 

/* #define `FUNCTION_ARG_CALLEE_COPIES (CUM, MODE, TYPE, NAMED)' */


/* A C type for declaring a variable that is used as the first argument of
 * `FUNCTION_ARG' and other related values.  For some target machines, the type
 * `int' suffices and can hold the number of bytes of argument so far.  
 *
 * There is no need to record in `CUMULATIVE_ARGS' anything about the arguments
 * that have been passed on the stack.  The compiler has other variables to
 * keep track of that.  For target machines on which all arguments are passed
 * on the stack, there is no need to store anything in `CUMULATIVE_ARGS';
 * however, the data structure must exist and should not be empty, so use `int'.
 *
 * The latter applies to c6x, so use `int'.
 * */ 

#define CUMULATIVE_ARGS  int


/* A C statement (sans semicolon) for initializing the variable CUM for the
 * state at the beginning of the argument list.  The variable has type
 * `CUMULATIVE_ARGS'.  For details, see info gccint, node "Register Arguments".
 *
 * On c6x, we assign IGNORED, because we don't use `CUMULATIVE_ARGS'.  */

#define INIT_CUMULATIVE_ARGS(CUM, FNTYPE, LIBNAME, INDIRECT)         \
do {								     \
  (CUM) = IGNORED;						     \
} while (0)
     

/* Optional.  Like `INIT_CUMULATIVE_ARGS' but only used for outgoing libcalls.
 * For details, see info gccint, node "Register Arguments".  
 * Currently not needed on c6x.  */

/* #define INIT_CUMULATIVE_LIBCALL_ARGS(CUM, MODE, LIBNAME) */


/* Optional.  Like `INIT_CUMULATIVE_ARGS' but overrides it for the purposes of
 * finding the arguments for the function being compiled.  Default is to use
 * `INIT_CUMULATIVE_ARGS'.  */ 

/* #define INIT_CUMULATIVE_INCOMING_ARGS(CUM, FNTYPE, LIBNAME) */


/* Mandatory.  A C statement (sans semicolon) to update the summarizer variable
 * CUM to advance past an argument in the argument list.  The values MODE, TYPE
 * and NAMED describe that argument.  Once this is done, the variable CUM is
 * suitable for analyzing the _following_ argument with `FUNCTION_ARG', etc.
 *
 * On c6x, this macro doesn't need to do anything, because all args are
 * currently passed on the stack. The compiler knows how to track the amount of
 * stack space used for arguments without any special help.
 * */ 

#define FUNCTION_ARG_ADVANCE(CUM, MODE, TYPE, NAMED)


/* If defined, a C expression which determines whether, and in which direction,
 * to pad out an argument with extra space.  The value should be of type `enum
 * direction': either `upward' to pad above the argument, `downward' to pad
 * below, or `none' to inhibit padding.
 *
 * This macro does not control the _amount_ of padding, which is always just
 * enough to reach the next multiple of `FUNCTION_ARG_BOUNDARY'. 
 *
 * This macro has a default definition which is right for most systems.  For
 * little-endian machines, the default is to pad upward.  For big-endian
 * machines, the default is to pad downward for an argument of constant size
 * shorter than an `int', and upward otherwise.
 *
 * On c6x, we use the default.  */

/* #define FUNCTION_ARG_PADDING(MODE, TYPE) */


/* If defined, a C expression which determines whether the default
 * implementation of va_arg will attempt to pad down before reading the next
 * argument, if that argument is smaller than its aligned space as controlled
 * by `PARM_BOUNDARY'.  The default is to pad all such arguments down if
 * `BYTES_BIG_ENDIAN' is true.
 *
 * On c6x, we use the default.  */

/* #define PAD_VARARGS_DOWN */


/* If defined, a C expression that gives the alignment boundary, in bits, of an
 * argument with the specified mode and type.  The default is to use
 * `PARM_BOUNDARY' for all arguments.
 *
 * On c6x, we use the default.  */

/* #define FUNCTION_ARG_BOUNDARY(MODE, TYPE) */


/* A C expression that is nonzero if REGNO is the number of a hard register in
 * which function arguments are sometimes passed.  This does _not_ include
 * implicit arguments such as the static chain and the structure-value address.
 * On many machines, no registers can be used for this purpose since all
 * function arguments are pushed on the stack.
 *
 * c6x doesn't currently use registers, so always evaluate to zero.  */ 

#define FUNCTION_ARG_REGNO_P(REGNO)  0

/* If defined, the order in which arguments are loaded into their respective
 * argument registers is reversed so that the last argument is loaded first.
 * This macro only affects arguments passed in registers.
 *
 * Not needed on c6x.  */ 

/* #define LOAD_ARGS_REVERSED */



/*** subnode: Scalar Return
 *
 * - controlling scalars as return values (fit in registers)
 */

/* Mandatory.  Define how to find the value returned by a function.
 * VALTYPE is the data type of the value (as a tree).
 * If the precise function being called is known, FUNC is its FUNCTION_DECL;
 * otherwise, FUNC is 0.  For more details, see info gccint, node 
 * "Register Arguments".  See also: `FUNCTION_VALUE_REGNO_P'.
 *
 * If you were to avoid returning particular values in registers, you might
 * take a closer look at `DEFAULT_PCC_STRUCT_RETURN', below, which is used in
 * `aggregate_value_p', which in turn decides whether a particular function
 * must pass its return value in memory.  But this method still needs a
 * register to pass the address to the return value in memory.
 *
 * On c6x, we use RETVAL_REGNO.  See also `RETURN_IN_MEMORY' for what happens
 * to DImode values and other modes wider than SImode.  */

#define FUNCTION_VALUE(VALTYPE, FUNC) \
  gen_rtx_REG (TYPE_MODE (VALTYPE), RETVAL_REGNO)


/* Define this macro if the target machine has "register windows" so that the
 * register in which a function returns its value is not the same as the one in
 * which the caller sees the value.
 *
 * Not needed on c6x, since we don't have register windows.  */ 

/* #define FUNCTION_OUTGOING_VALUE(VALTYPE, FUNC) */


/* A C expression to create an RTX representing the place where a library
 * function returns a value of mode MODE.  If the precise function being called
 * is known, FUNC is a tree node (`FUNCTION_DECL') for it; otherwise, FUNC is a
 * null pointer.  This makes it possible to use a different value-returning
 * convention for specific functions when all their calls are known.
 *
 * Note that "library function" in this context means a compiler support
 * routine, used to perform arithmetic, whose name is known specially by the
 * compiler and was not mentioned in the C code being compiled.
 *
 * The definition of `LIBRARY_VALUE' need not be concerned with aggregate data
 * types, because none of the library functions returns such types.
 *
 * On c6x, we use the same register as macro `FUNCTION_VALUE' does.  */

#define LIBCALL_VALUE(MODE) \
  gen_rtx_REG (MODE, RETVAL_REGNO)


/* A C expression that is nonzero if REGNO is the number of a hard register in
 * which the values of called function may come back.
 *
 * A register whose use for returning values is limited to serving as the
 * second of a pair (for a value of type `double', say) need not be recognized
 * by this macro.
 *
 * If the machine has register windows, so that the caller and the called
 * function use different registers for the return value, this macro should
 * recognize only the caller's register numbers.
 *
 * Function `keep_with_call_p' ensures that SET insns, which copy from a
 * `FUNCTION_VALUE_REGNO_P'-reg into some pseudo, are always kept next to the
 * CALL insn.
 */

#define FUNCTION_VALUE_REGNO_P(REGNO) \
  ((REGNO) == RETVAL_REGNO)


/* Define this macro if `untyped_call' and `untyped_return' need more space
 * than is implied by `FUNCTION_VALUE_REGNO_P' for saving and restoring an
 * arbitrary return value.
 *
 * Not needed on c6x.  */ 

/* #define APPLY_RESULT_SIZE */



/*** subnode: Aggregate Return
 */


/* A C expression which can inhibit the returning of certain function values in
 * registers, based on the type of value.  A nonzero value says to return the
 * function value in memory, just as large structures are always returned.
 * Here TYPE will be a C expression of type `tree', representing the data type
 * of the value. See info gccint, node "Aggregate Return" for more details.
 *
 * The default is to use 1 for `BLKmode' values, and 0 otherwise. 
 *
 * On c6x, we also want to pass modes wider than SImode in memory rather than
 * in registers.  If we allowed wider modes to be passed in registers, we
 * would have to mark not only RETVAL_REGNO, but also the RETVAL_REGNO + 1
 * in `CALL_USED_REGISTERS'.  */

#define RETURN_IN_MEMORY(TYPE)                                       \
  (TYPE_MODE (TYPE) == BLKmode                                       \
   || GET_MODE_SIZE (TYPE_MODE (TYPE)) > UNITS_PER_WORD)


/* Define this macro to be 1 if all structure and union return values must be
 * in memory.  Since this results in slower code, this should be defined only
 * if needed for compatibility with other compilers or with an ABI.  If you
 * define this macro to be 0, then the conventions used for structure and union
 * return values are decided by the `RETURN_IN_MEMORY' macro.  
 *
 * On c6x, we use the default, which is 1.  */

/* #define DEFAULT_PCC_STRUCT_RETURN  1 */


/* Optional.  Define it to the number of the register to pass the address of
 * aggregate return values in.  If undefined, `STRUCT_VALUE' will be used to
 * initialize `struct_value_rtx' instead.
 *
 * On c6x, we use the same register as for passing scalar return values (see
 * `FUNCTION_VALUE', above).
 * */ 

#define STRUCT_VALUE_REGNUM  (RETVAL_REGNO)


/* If the structure value address is not passed in a register, define
 * `STRUCT_VALUE' as an expression returning an RTX for the place where the
 * address is passed.  If it returns 0, the address is passed as an "invisible"
 * first argument.  
 *
 * Not needed on c6x, since we define `STRUCT_VALUE_REGNUM'.  */

/* #define STRUCT_VALUE */


/* On some architectures the place where the structure value address is found
 * by the called function is not the same place that the caller put it.  This
 * can be due to register windows, or it could be because the function prologue
 * moves it to a different place.  If the incoming location of the structure
 * value address is in a register, define this macro as the register number.
 *
 * Not needed on c6x.  */ 

/* #define STRUCT_VALUE_INCOMING_REGNUM */


/* If the incoming location is not a register, then you should define
 * `STRUCT_VALUE_INCOMING' as an expression for an RTX for where the called
 * function should find the value.  If it should find the value on the stack,
 * define this to create a `mem' which refers to the frame pointer.  A
 * definition of 0 means that the address is passed as an "invisible" first
 * argument.  
 *
 * On c6x, the incoming location is a register, so we don't need any def.  */ 

/* #define STRUCT_VALUE_INCOMING */


/* Define this macro if the usual system convention on the target machine for
 * returning structures and unions is for the called function to return the
 * address of a static variable containing the value.
 *
 * Do not define this if the usual system convention is for the caller to pass
 * an address to the subroutine.  
 *
 * This macro has effect in `-fpcc-struct-return' mode, but it does nothing
 * when you use `-freg-struct-return' mode.  */ 

/* #define PCC_STATIC_STRUCT_RETURN */



/*** subnode: Caller Saves
 *
 * If you enable it, GCC can save registers around function calls.  This makes
 * it possible to use call-clobbered registers to hold variables that must live
 * across calls.
 */


/* Define this macro if function calls on the target machine do not preserve
 * any registers; in other words, if `CALL_USED_REGISTERS' has 1 for all
 * registers.  When defined, this macro enables `-fcaller-saves' by default for
 * all optimization levels.  It has no effect for optimization levels 2 and
 * higher, where `-fcaller-saves' is the default.
 *
 * On c6x, we use `CALL_USED_REGISTERS' to specify which registers are call
 * clobbered and must be caller-saved by GCC.  */ 

/* #define DEFAULT_CALLER_SAVES */


/* A C expression to determine whether it is worthwhile to consider placing a
 * pseudo-register in a call-clobbered hard register and saving and restoring
 * it around each function call.  The expression should be 1 when this is worth
 * doing, and 0 otherwise.  
 *
 * If you don't define this macro, a default is used which is good on most
 * machines: `4 * CALLS < REFS'.  On c6x, we just use the default.  */ 

/* #define CALLER_SAVE_PROFITABLE (REFS, CALLS) */


/* A C expression specifying which mode is required for saving NREGS of a
 * pseudo-register in call-clobbered hard register REGNO.  If REGNO is
 * unsuitable for caller save, `VOIDmode' should be returned.  For most
 * machines this macro need not be defined since GCC will select the smallest
 * suitable mode.  */ 

/* #define HARD_REGNO_CALLER_SAVE_MODE(REGNO, NREGS, MODE) */



/*** subnode: Function Entry
 */

/* Optional target hook.
 * If defined, a function that outputs the assembler code for entry to a
 * function.  The prologue is responsible for setting up the stack frame,
 * initializing the frame pointer register, saving registers that must be
 * saved, and allocating SIZE additional bytes of storage for the local
 * variables.
 *
 * On c6x, we use the "prologue" pattern in c6x.md instead of this.  The most
 * comprehensive doc available for the "prologue" pattern can be found at info
 * gccint, node "Function Entry", section `TARGET_ASM_FUNCTION_PROLOGUE'.  Only
 * some doc is available at info gccint, node "Standard Names", `prologue'.
 */

/* void TARGET_ASM_FUNCTION_PROLOGUE (FILE *FILE, HOST_WIDE_INT SIZE) */


/* Optional target hook.
 * If defined, a function that outputs assembler code at the end of a prologue.
 * This should be used when the function prologue is being emitted as RTL, and
 * you have some extra assembler that needs to be emitted.  */

/* void TARGET_ASM_FUNCTION_END_PROLOGUE (FILE *FILE) */


/* Optional target hook.
 * If defined, a function that outputs assembler code at the start of an
 * epilogue.  This should be used when the function epilogue is being emitted
 * as RTL, and you have some extra assembler that needs to be emitted.  */

/* void TARGET_ASM_FUNCTION_BEGIN_EPILOGUE (FILE *FILE) */


/* Optional target hook.
 * If defined, a function that outputs the assembler code for exit from a
 * function.  The epilogue is responsible for restoring the saved registers and
 * stack pointer to their values when the function was called, and returning
 * control to the caller.
 *
 * On c6x, we use the "epilogue" pattern in c6x.md instead of this.  The most
 * comprehensive doc available for the "epilogue" pattern can be found at info
 * gccint, node "Function Entry", section `TARGET_ASM_FUNCTION_EPILOGUE'.  Only
 * some doc is available at info gccint, node "Standard Names", `prologue'.
 */

/* void TARGET_ASM_FUNCTION_EPILOGUE (FILE *FILE, HOST_WIDE_INT SIZE) */


/* Optional.  Define this macro as a C expression that is nonzero if the return
 * instruction or the function epilogue ignores the value of the stack pointer;
 * in other words, if it is safe to delete an instruction to adjust the stack
 * pointer before a return from the function.  
 *
 * Note that this macro's value is relevant only for functions for which frame
 * pointers are maintained.  It is never safe to delete a final stack
 * adjustment in a function that has no frame pointer, and the compiler knows
 * this regardless of `EXIT_IGNORE_STACK'.
 *
 * On c6x, we are conservative and waive this optimization for now. TODO: It
 * might probably be added later on, however.  */ 

/* #define EXIT_IGNORE_STACK */


/* Define this macro as a C expression that is nonzero for registers that are
 * used by the epilogue or the `return' pattern.  The stack and frame pointer
 * registers are already be assumed to be used as needed. For example, this
 * macro is used in `mark_regs_live_at_end' (flow.c).
 *
 * Note that other special registers that need to be live at the end of each
 * function are handled elsewhere, eg. the register to pass the return value in
 * is marked live by `diddle_return_value' (see `RETVAL_REGNO', above).  
 *
 * Currently, no such extra registers are needed for c6x.  */ 

/* #define EPILOGUE_USES(REGNO) */


/* Define this macro as a C expression that is nonzero for registers that are
 * used by the exception handling mechanism, and so should be considered live
 * on entry to an exception edge.
 *
 * Currently only defined on ia64.  Not needed for c6x.  */

/* #define EH_USES(REGNO) */


/* Optional.  Define this macro if the function epilogue contains delay slots
 * to which instructions from the rest of the function can be "moved".  The
 * definition should be a C expression whose value is an integer representing
 * the number of delay slots there.
 *
 * An optimization we don't yet take advantage of on c6x.  */ 

/* #define DELAY_SLOTS_FOR_EPILOGUE */


/* A C expression that returns 1 if INSN can be placed in delay slot number N
 * of the epilogue.  This macro is need not be defined unless
 * `DELAY_SLOTS_FOR_EPILOGUE' is.  See info gccint, node "Function Entry" for
 * the details.  */

/* #define ELIGIBLE_FOR_EPILOGUE_DELAY(INSN, N) */


/* Required for vararg support with multiple inheritance. Otherwise optional.
 *
 * A target hook that outputs the assembler code for a thunk function, used to
 * implement C++ virtual function calls with multiple inheritance.  The thunk
 * acts as a wrapper around a virtual function, adjusting the implicit object
 * parameter before handing control off to the real function.
 *
 * If you do not define this macro, the target-independent code in the C++
 * front end will generate a less efficient heavyweight thunk that calls
 * FUNCTION instead of jumping to it.  However, this generic approach does not
 * support varargs.  
 *
 * TODO: Implement a proper function for this or for
 * `TARGET_ASM_OUTPUT_MI_VCALL_THUNK', below, on c6x.  */

/* void TARGET_ASM_OUTPUT_MI_THUNK (FILE *FILE, tree
          THUNK_FNDECL, HOST_WIDE_INT DELTA, tree FUNCTION) */


/* A function like `TARGET_ASM_OUTPUT_MI_THUNK', except that if VCALL_OFFSET is
 * nonzero, an additional adjustment should be made after adding `delta'.  In
 * particular, if P is the adjusted pointer, the following adjustment should be
 * made: 
 *       p += (*((ptrdiff_t **)p))[vcall_offset/sizeof(ptrdiff_t)]
 *
 * If this function is defined, it will always be used in place of
 * `TARGET_ASM_OUTPUT_MI_THUNK'.
 * */

/* void TARGET_ASM_OUTPUT_MI_VCALL_THUNK (FILE *FILE, tree THUNK_FNDECL, 
           HOST_WIDE_INT DELTA, int VCALL_OFFSET, tree FUNCTION) */



/*** subnode: Profiling
 */


/* Mandatory.  A C statement or compound statement to output to FILE some
 * assembler code to call the profiling subroutine `mcount'.  See info gccint,
 * node "Profiling" for more details.  
 *
 * On c6x, there is currently no support for profiling. We define dummies for
 * the required macros.  */

#define FUNCTION_PROFILER(FILE, LABELNO)  EMPTY


/* Optional.  A C statement or compound statement to output to FILE some
 * assembly code to call the profiling subroutine `mcount' even the target does
 * not support profiling.  */

/* #define PROFILE_HOOK */


/* Define this macro if the `mcount' subroutine on your system does not need a
 * counter variable allocated for each function.  This is true for almost all
 * modern implementations.  If you define this macro, you must not use the
 * LABELNO argument to `FUNCTION_PROFILER'.
 *
 * On c6x, we don't need counters, because there is no profiling at the moment.
 * ;-)  */ 

#define NO_PROFILE_COUNTERS


/* Define this macro if the code for function profiling should come before the
 * function prologue.  Normally, the profiling code comes after.  */

/* #define PROFILE_BEFORE_PROLOGUE */



/*** subnode: Tail Calls
 */


/* A C expression that evaluates to true if it is ok to perform a sibling call
 * to DECL from the current function.
 *
 * On c6x, we want to avoid any possible sources of erroneous behaviour.  So
 * we currently support only normal calls.  */

#define FUNCTION_OK_FOR_SIBCALL(DECL)  0



/**** node: Varargs
 *
 * - controling the behaviour for argument passing in the case of variable
 *   number of args
 * - GCC comes with an implementation of `<varargs.h>' and `<stdarg.h>' that
 *   work without change on machines that pass arguments on the stack.
 *   On c6x, we currently pass all args on the stack and hence use the 
 *   standard implementation.
 * */


/* If defined, is a C expression that produces the machine-specific code for a
 * call to `__builtin_saveregs'.  This code will be emitted at the very
 * beginning of the function, before any parameter access are made.  The return
 * value of this function should be an RTX that contains the value to use as
 * the return of `__builtin_saveregs'.
 *
 * C6x passes all its args exclusively on the stack, so this macro should never
 * be used at the moment.  If used anyway, `expand_builtin_saveregs' will issue
 * an error message.  */

/* #define EXPAND_BUILTIN_SAVEREGS() */


/* This macro offers an alternative to using `__builtin_saveregs' and defining
 * the macro `EXPAND_BUILTIN_SAVEREGS'.  Use it to store the anonymous register
 * arguments into the stack so that all the arguments appear to have been
 * passed consecutively on the stack.  Once this is done, you can use the
 * standard implementation of varargs that works for machines that pass all
 * their arguments on the stack.
 *
 * `SETUP_INCOMING_VARARGS' is only useful on machines that have just a single
 * category of argument register and use it uniformly for all data types.
 * See info gccint, node "Varargs" for more details.
 * */

/* #define SETUP_INCOMING_VARARGS(ARGS_SO_FAR, MODE, TYPE, PRETEND_ARGS_SIZE, SECOND_TIME) */


/* Define this macro to be a nonzero value if the location where a function
 * argument is passed depends on whether or not it is a named argument.
 *
 * This macro controls how the NAMED argument to `FUNCTION_ARG' is set for
 * varargs and stdarg functions.  If this macro returns a nonzero value, the
 * NAMED argument is always true for named arguments, and false for unnamed
 * arguments.  If it returns a value of zero, but `SETUP_INCOMING_VARARGS' is
 * defined, then all arguments are treated as named.  Otherwise, all named
 * arguments except the last are treated as named.
 *
 * The default is 0, which we use on c6x, since all args are exclusively on
 * the stack and no decision is needed on where to pass them.  */

/* #define STRICT_ARGUMENT_NAMING */


/* If you need to conditionally change ABIs so that one works with
 * `SETUP_INCOMING_VARARGS', but the other works like neither
 * `SETUP_INCOMING_VARARGS' nor `STRICT_ARGUMENT_NAMING' was defined, then
 * define this macro to return nonzero if `SETUP_INCOMING_VARARGS' is used,
 * zero otherwise.  Otherwise, you should not define this macro.
 *
 * Not needed on c6x.  */

/* #define PRETEND_OUTGOING_VARARGS_NAMED */



/**** node: Trampolines
 *
 * - When entering a nested function through an indirect call, the static
 *   link (pointer to the frame of the containing function) has to be set up.
 * - This is done by making nested function pointers point to a "trampoline",
 *   which has code to do the setup and then hand over to the actual function.
 */


/* Optional.  A C statement to output, on the stream FILE, assembler code for a
 * block of data that contains the constant parts of a trampoline.  This code
 * should not include a label--the label is taken care of automatically.  
 *
 * If you do not define this macro, it means no template is needed for the
 * target.  Do not define this macro on systems where the block move code to
 * copy the trampoline into place would be larger than the code to generate it
 * on the spot.
 * */

/* #define TRAMPOLINE_TEMPLATE(FILE) */
  

/* Optional.  The name of a subroutine to switch to the section in which the
 * trampoline template is to be placed (*note Sections::).  The default is a
 * value of `readonly_data_section', which places the trampoline in the section
 * containing read-only data.  */

/* #define TRAMPOLINE_SECTION */


/* Mandatory.  A C expression for the size in bytes of the trampoline, as an
 * integer.
 *
 * On c6x, we currently define the size to be zero, but this will change as
 * soon as trampolines will be supported.  */

#define TRAMPOLINE_SIZE  0


/* Alignment required for trampolines, in bits.  
 *
 * If you don't define this macro, the value of `BIGGEST_ALIGNMENT' is used for
 * aligning trampolines.  */

/* #define TRAMPOLINE_ALIGNMENT */


/* Mandatory.  A C statement to initialize the variable parts of a trampoline.
 * ADDR is an RTX for the address of the trampoline; FNADDR is an RTX for the
 * address of the nested function; STATIC_CHAIN is an RTX for the static chain
 * value that should be passed to the function when it is called.  */

#define INITIALIZE_TRAMPOLINE(ADDR, FNADDR, STATIC_CHAIN) \
  fatal_error ("INITIALIZE_TRAMPOLINE: Trampolines are not yet supported on the c6x target.");


/* A C statement that should perform any machine-specific adjustment in the
 * address of the trampoline.  Its argument contains the address that was
 * passed to `INITIALIZE_TRAMPOLINE'.  In case the address to be used for a
 * function call should be different from the address in which the template was
 * stored, the different address should be assigned to ADDR.  If this macro is
 * not defined, ADDR will be used for function calls.  */

/* #define TRAMPOLINE_ADJUST_ADDRESS(ADDR) */


/* A C expression to allocate run-time space for a trampoline.  The expression
 * value should be an RTX representing a memory reference to the space for the
 * trampoline.
 *
 * If this macro is not defined, by default the trampoline is allocated as a
 * stack slot.  This default is right for most machines.  See info gccint, node
 * "Trampolines" for more details.  */

/* #define ALLOCATE_TRAMPOLINE(FP) */


/* Note: Implementing trampolines is difficult on many machines because they
 * have separate instruction and data caches.  Writing into a stack location
 * fails to clear the memory in the instruction cache, so when the program
 * jumps to that location, it executes the old contents.  Therefore, GCC
 * provides you with some extra macros that can be used for solving such cache
 * problems.  Refer to info gccint, node `Trampolines' for details. Macros
 * not further mentioned here include:
 * - `INSN_CACHE_SIZE', 
 * - `INSN_CACHE_LINE_WIDTH'
 * - `INSN_CACHE_DEPTH'
 * - `CLEAR_INSN_CACHE'
 * - `TRANSFER_FROM_TRAMPOLINE'
 * */


/**** node: Library Calls
 *
 * - Controlling how library routines are implicitly called.  */


/* A C string constant giving the name of the function to call for
 * multiplication of one signed full-word by another.  If you do not define
 * this macro, the default name is used, which is `__mulsi3', a function
 * defined in `libgcc.a'.  */

/* #define MULSI3_LIBCALL */


/* A C string constant giving the name of the function to call for division of
 * one signed full-word by another.  If you do not define this macro, the
 * default name is used, which is `__divsi3', a function defined in `libgcc.a'.
 * */

/* #define DIVSI3_LIBCALL */
 

/* A C string constant giving the name of the function to call for division of
 * one unsigned full-word by another.  If you do not define this macro, the
 * default name is used, which is `__udivsi3', a function defined in
 * `libgcc.a'.  */

/* #define UDIVSI3_LIBCALL */


/* A C string constant giving the name of the function to call for the
 * remainder in division of one signed full-word by another.  If you do not
 * define this macro, the default name is used, which is `__modsi3', a function
 * defined in `libgcc.a'.  */

/* #define MODSI3_LIBCALL */
 

/* A C string constant giving the name of the function to call for the
 * remainder in division of one unsigned full-word by another.  If you do not
 * define this macro, the default name is used, which is `__umodsi3', a
 * function defined in `libgcc.a'.  */

/* #define UMODSI3_LIBCALL */
 

/* A C string constant giving the name of the function to call for
 * multiplication of one signed double-word by another.  If you do not define
 * this macro, the default name is used, which is `__muldi3', a function
 * defined in `libgcc.a'.  */

/* #define MULDI3_LIBCALL */
 

/* A C string constant giving the name of the function to call for division of
 * one signed double-word by another.  If you do not define this macro, the
 * default name is used, which is `__divdi3', a function defined in `libgcc.a'.
 * */

/* #define DIVDI3_LIBCALL */
 

/* A C string constant giving the name of the function to call for division of
 * one unsigned full-word by another.  If you do not define this macro, the
 * default name is used, which is `__udivdi3', a function defined in
 * `libgcc.a'.  */

/* #define UDIVDI3_LIBCALL */
 

/* A C string constant giving the name of the function to call for the
 * remainder in division of one signed double-word by another.  If you do not
 * define this macro, the default name is used, which is `__moddi3', a function
 * defined in `libgcc.a'.  */

/* #define MODDI3_LIBCALL */
 

/* A C string constant giving the name of the function to call for the
 * remainder in division of one unsigned full-word by another.  If you do not
 * define this macro, the default name is used, which is `__umoddi3', a
 * function defined in `libgcc.a'.  */

/* #define UMODDI3_LIBCALL */
 

/* Optional. This macro, if defined, should expand to a piece of C code that
 * will get expanded when compiling functions for libgcc.a.  It can be used to
 * provide alternate names for gcc's internal library functions if there are
 * ABI-mandated names that the compiler should provide.  Currently only used
 * in `config/frv/frv-abi.h'  */

/* #define DECLARE_LIBRARY_RENAMES */
 

/* Optional.  Define this macro as a C statement that declares additional
 * library routines renames existing ones.  `init_optabs' calls this macro
 * after initializing all the normal library routines.  */

/* #define INIT_TARGET_OPTABS */
 

/* Optional.  Define this macro as a C statement that returns nonzero if a call
 * to the floating point comparison library function will return a boolean
 * value that indicates the result of the comparison.  It should return zero if
 * one of gcc's own libgcc functions is called.
 *
 * Most ports don't need to define this macro.  */

/* #define FLOAT_LIB_COMPARE_RETURNS_BOOL */
 

/* Optional.  The value of `EDOM' on the target machine, as a C integer
 * constant expression.  If you don't define this macro, GCC does not attempt
 * to deposit the value of `EDOM' into `errno' directly.  Look in
 * `/usr/include/errno.h' to find the value of `EDOM' on your system. 
 *
 * If you do not define `TARGET_EDOM', then compiled code reports domain errors
 * by calling the library function and letting it report the error.  If
 * mathematical functions on your system use `matherr' when there is an error,
 * then you should leave `TARGET_EDOM' undefined so that `matherr' is used
 * normally.  */

/* #define TARGET_EDOM */
 

/* Optional.  Define this macro as a C expression to create an rtl expression
 * that refers to the global "variable" `errno'.  (On certain systems, `errno'
 * may not actually be a variable.)  If you don't define this macro,
 * (MEM:SImode (SYMBOL_REF:Pmode "errno")) will be used.  */

/* #define GEN_ERRNO_RTX */
 

/* Define this macro if GCC should generate calls to the ISO C (and System V)
 * library functions `memcpy', `memmove' and `memset' rather than the BSD
 * functions `bcopy' and `bzero'.  */

#define TARGET_MEM_FUNCTIONS
 

/* Define this macro if `float' arguments cannot be passed to library routines
 * (so they must be converted to `double').  Currently not defined by any
 * target.  */

/* #define LIBGCC_NEEDS_DOUBLE */
 

/* Define this macro to generate code for Objective-C message sending using the
 * calling convention of the NeXT system.  This calling convention involves
 * passing the object, the selector and the method arguments all at once to the
 * method-lookup library function. 
 *
 * The default calling convention passes just the object and the selector to
 * the lookup function, which returns a pointer to the method.
 * */

/* #define NEXT_OBJC_RUNTIME */


/* TODO: Check, whether up-to-date docs comply with what I used on "marvin".
 * Are the macro names and arguments identical? They should be!  */


/**** node: Addressing Modes
 *
 * - Defining addressing modes valid for memory operands.  */

/* Optional.  A C expression that is nonzero if the machine supports
 * pre-increment, pre-decrement, post-increment, or post-decrement addressing
 * respectively.  
 *
 * For c6x, see spru189f.pdf, section 3.8.3: "Syntax for load/store address
 * generation".  */

#define HAVE_PRE_INCREMENT 1
#define HAVE_PRE_DECREMENT 1
#define HAVE_POST_INCREMENT 1
#define HAVE_POST_DECREMENT 1


/* Optional.  A C expression that is nonzero if the machine supports pre- or
 * post-address side-effect generation involving constants other than the size
 * of the memory operand.  */

/* #define HAVE_PRE_MODIFY_DISP */
/* #define HAVE_POST_MODIFY_DISP */


/* Optional.  A C expression that is nonzero if the machine supports pre- or
 * post-address side-effect generation involving a register displacement.  */

/* #define HAVE_PRE_MODIFY_REG */


/* Optional.  A C expression that is nonzero if the machine supports pre- or
 * post-address side-effect generation involving a register displacement.  
 * Apparantly used nowhere.  */

/* #define HAVE_POST_MODIFY_REG */


/* A C expression that is 1 if the RTX X is a constant which is a valid
 * address.  On most machines, this can be defined as `CONSTANT_P (X)', but a
 * few machines are more restrictive in which constant addresses are supported.
 *
 * Function `offsettable_address_p', called by `offsettable_memref_p', called
 * by `constrain_operands' and `find_reloads', uses this macro in order to
 * determine whether some (CONST_INT ...), (SYMBOL_REF ...), (LABEL_REF ...) or
 * similar constant address constructs can be directly accepted as offsettable
 * address and do not need to be reloaded into a register.
 *
 * For c6x, we are conservative and say that a constant can never directly be
 * used as an address.  So we force reload to use a register for satisfying `V'
 * and `o' constraints.
 * */

#define CONSTANT_ADDRESS_P(X)  0


/* A number, the maximum number of registers that can appear in a valid memory
 * address.  Note that it is up to you to specify a value equal to the maximum
 * number that `GO_IF_LEGITIMATE_ADDRESS' would ever accept.
 *
 * On c6x, addresses can have a base and an offset register.  */

#define MAX_REGS_PER_ADDRESS  2


/* A C compound statement with a conditional `goto LABEL;' executed if X (an
 * RTX) is a legitimate memory address on the target machine for a memory
 * operand of mode MODE.  A discussion of the details of this macro can be
 * found at info gccint, node "Addressing Modes".
 *
 * This macro must exist in two variants: a strict variant and a non-strict
 * one.  The strict variant is used in the reload pass.  It must be defined so
 * that any pseudo-register that has not been allocated a hard register is
 * considered a memory reference.  In contexts where some kind of register is
 * required, a pseudo-register with no hard register must be rejected.  
 *
 * The non-strict variant is used in other passes.  It must be defined to
 * accept all pseudo-registers in every context where some kind of register is
 * required.
 *
 * This definition can be as minimal as accepting only registers that the
 * target allows as base regs in addresses.  On c6x, we slightly extended on
 * that.  See stormy16.h for another relatively basic definition.
 *
 * Every form of address you reject here will be reloaded into more and more
 * basic forms and finally will end up in a base register.  The simpler the
 * types of acceptable addresses the smaller `PRINT_OPERAND_ADDRESS' can be
 * kept, because it need not be concerned with `base + index' constructs or
 * constants as addresses and the like.  */

#define GO_IF_LEGITIMATE_ADDRESS(MODE, X, LABEL)	             \
do {							             \
  if (c6x_legitimate_address_p ((MODE), (X), C6X_REG_OK_STRICT))     \
    goto LABEL;						             \
} while (0)


/* A C expression that is nonzero if X (assumed to be a `reg' RTX) is valid for
 * use as a base register.  For hard registers, it should always accept those
 * which the hardware permits and reject the others.  Whether the macro accepts
 * or rejects pseudo registers must be controlled by `REG_OK_STRICT' as
 * described above.  */

#define REG_OK_FOR_BASE_P(X)  (C6X_REG_OK_FOR_BASE_P(X, C6X_REG_OK_STRICT))

#define C6X_REG_OK_FOR_BASE_P(X, STRICT) \
  ((STRICT) \
    ? REGNO_OK_FOR_BASE_P (REGNO (X)) \
    : (GENERAL_REGNO_P (REGNO (X)) || (REGNO (X) >= FIRST_PSEUDO_REGISTER)))


/* A C expression that is just like `REG_OK_FOR_BASE_P', except that that
 * expression may examine the mode of the memory reference in MODE.  You should
 * define this macro if the mode of the memory reference affects whether a
 * register may be used as a base register.  If you define this macro, the
 * compiler will use it instead of `REG_OK_FOR_BASE_P'.
 *
 * The mode should not make a difference on c6x, so define `REG_OK_FOR_BASE_P'
 * instead of this.  */

/* #define REG_MODE_OK_FOR_BASE_P(X, MODE) */


/* A C expression that is nonzero if X (assumed to be a `reg' RTX) is valid for
 * use as an index register.
 *
 * The difference between an index register and a base register is that the
 * index register may be scaled.  If an address involves the sum of two
 * registers, neither one of them scaled, then either one may be labeled the
 * "base" and the other the "index"; but whichever labeling is used must fit
 * the machine's constraints of which registers may serve in each capacity.
 * The compiler will try both labelings, looking for one that is valid, and
 * will reload one or both registers only if neither labeling works.  */

#define REG_OK_FOR_INDEX_P(X)  (REG_OK_FOR_BASE_P (X))


/* Optional.  A C expression to determine the base term of address X.  This
 * macro is used only in alias.c.  There is a sensible default behaviour.
 *
 * The typical use of this macro is to handle addresses containing a label_ref
 * or symbol_ref within an UNSPEC.  */

/* #define FIND_BASE_TERM(X) */
     

/* Required, but empty def is ok.  A C compound statement that attempts to
 * replace X with a valid memory address for an operand of mode MODE.  WIN will
 * be a C statement label elsewhere in the code; the macro definition may use 
 *         GO_IF_LEGITIMATE_ADDRESS (MODE, X, WIN);
 * to avoid further processing if the address has become legitimate.
 *
 * X will always be the result of a call to `break_out_memory_refs', and OLDX
 * will be the operand that was given to that function to produce X.
 *
 * The code generated by this macro should not alter the substructure of X.  If
 * it transforms X into a more legitimate form, it should assign X (which will
 * always be a C variable) a new value.
 *
 * It is safe for this macro to do nothing, because the compiler implements
 * standard behaviour, but the result may be slower.
 *
 * We often get rtx like (plus:SI (reg:SI virtual-stack-vars) (const_int -2)),
 * which the compiler doesn't seem to handle properly but to force into a
 * register, generating an addition.  On c6x, the addition can be performed
 * by ldw/stw, if given two regs. So we force the CONST_INT into a register. 
 */

#define LEGITIMIZE_ADDRESS(X, OLDX, MODE, WIN)

#if 0
/* currently deactivated, see comment in `c6x_legitimate_address_p' for the
 * "sum of two registers" case */
#define LEGITIMIZE_ADDRESS(X, OLDX, MODE, WIN)				\
  do {									\
    (X) = c6x_legitimize_address ((X), (OLDX), (MODE));			\
    if (memory_address_p ((MODE), (X)))					\
      goto WIN;								\
  } while (0)
#endif


/* Optional.  A C compound statement that attempts to replace X, which is an
 * address that needs reloading, with a valid memory address for an operand of
 * mode MODE.  WIN will be a C statement label elsewhere in the code.  It is
 * not necessary to define this macro, but it might be useful for performance
 * reasons.  For details, see info gccint, node "Addressing Modes".  */

/* #define LEGITIMIZE_RELOAD_ADDRESS(X, MODE, OPNUM, TYPE, IND_LEVELS, WIN) */


/* A C statement or compound statement with a conditional `goto LABEL;'
 * executed if memory address X (an RTX) can have different meanings depending
 * on the machine mode of the memory reference it is used for or if the address
 * is valid for some modes but not others.
 *
 * Autoincrement and autodecrement addresses typically have mode-dependent
 * effects because the amount of the increment or decrement is the size of the
 * operand being addressed.  
 *
 * You may assume that ADDR is a valid address for the machine.  */

#define GO_IF_MODE_DEPENDENT_ADDRESS(ADDR, LABEL)		     \
  if (   GET_CODE (ADDR) == PRE_INC                                  \
      || GET_CODE (ADDR) == PRE_DEC                                  \
      || GET_CODE (ADDR) == POST_INC                                 \
      || GET_CODE (ADDR) == POST_DEC                                 \
      )	                                                             \
    goto LABEL;


/* A C expression that is nonzero if X is a legitimate constant for an
 * immediate operand on the target machine.  You can assume that X satisfies
 * `CONSTANT_P', so you need not check this.  In fact, `1' is a suitable
 * definition for this macro on machines where anything `CONSTANT_P' is valid.
 *
 * LEGITIMATE_CONSTANT_P is (among others) used in `find_reloads_address_part',
 * where it has to tell whether X can legitimately used as part of an address
 * without need to reload it into a register.  It is also used in function
 * `general_operand'.
 */

#define LEGITIMATE_CONSTANT_P(X)  1



/**** node: Condition Code
 *
 * - Defining how insns update the condition code.
 * - On c6x, we do not use (cc0), the condition code register, but use
 *   alternative means of handling conditional jumps, moves, and so on.
 *   Many of the following macros will hence be irrelevant for us.  
 */


/* Optional.  C code for a data type which is used for declaring the `mdep'
 * component of `cc_status'.  It defaults to `int'.
 *
 * Not relevant for c6x, since we don't use `cc0'.  */

/* #define CC_STATUS_MDEP */


/* Optional.  A C expression to initialize the `mdep' field to "empty".  The
 * default definition does nothing, since most machines don't use the field
 * anyway.  If you want to use the field, you should probably define this macro
 * to initialize it.
 *
 * Not relevant for c6x, since we don't use `cc0'.  */

/* #define CC_STATUS_MDEP_INIT */


/* Optional.  A C compound statement to set the components of `cc_status'
 * appropriately for an insn INSN whose body is EXP.  It is this macro's
 * responsibility to recognize insns that set the condition code as a byproduct
 * of other activity as well as those that explicitly set `(cc0)'.  
 * For details, see info gccint, node "Condition Code".
 *
 * Not relevant for c6x, since we don't use `cc0'.  */

/* #define NOTICE_UPDATE_CC(EXP, INSN) */


/* Condition codes are represented in registers by machine modes of class
 * `MODE_CC'.  By default, there is just one mode, `CCmode', with this class.
 * If you need more such modes, see info gccint, node "Condition Code" for
 * details.  */

/* #define EXTRA_CC_MODES */
     

/* Optional.  Returns a mode from class `MODE_CC' to be used when comparison
 * operation code OP is applied to rtx X and Y.  For an example, see info
 * gccint, node "Condition Code".  
 *
 * You need not define this macro if `EXTRA_CC_MODES' is not defined.  */

/* #define SELECT_CC_MODE(OP, X, Y) */


/* On some machines not all possible comparisons are defined, but you can
 * convert an invalid comparison into a valid one.  For example, the Alpha does
 * not have a `GT' comparison, but you can use an `LT' comparison instead and
 * swap the order of the operands.
 *
 * On such machines, define this macro to be a C statement to do any required
 * conversions.  CODE is the initial comparison code and OP0 and OP1 are the
 * left and right operands of the comparison, respectively.  You should modify
 * CODE, OP0, and OP1 as required.  
 *
 * GCC will not assume that the comparison resulting from this macro is valid
 * but will see if the resulting insn matches a pattern in the `md' file.
 *
 * You need not define this macro if it would never change the comparison code
 * or operands.
 */

/* #define CANONICALIZE_COMPARISON(CODE, OP0, OP1) */


/* Optional.  A C expression whose value is one if it is always safe to reverse
 * a comparison whose mode is MODE.  If `SELECT_CC_MODE' can ever return MODE
 * for a floating-point inequality comparison, then `REVERSIBLE_CC_MODE (MODE)'
 * must be zero.  For details, see info gccint, node "Condition Code".
 *
 * For c6x, maybe look for a better definition at some time.  */

/* #define REVERSIBLE_CC_MODE(MODE) */


/* A C expression whose value is reversed condition code of the CODE for
 * comparison done in CC_MODE MODE.  The macro is used only in case
 * `REVERSIBLE_CC_MODE (MODE)' is nonzero.  For details, see info gccint, node
 * "Condition Code".  */

/* #define REVERSE_CONDITION(CODE, MODE) */


/* Optional.  A C expression that returns true if the conditional execution
 * predicate CODE1 is the inverse of CODE2 and vice versa.  Define this to
 * return 0 if the target has conditional execution predicates that cannot be
 * reversed safely.  
 *
 * On c6x, we use the default, which is shown below. 
 */

/* #define REVERSE_CONDEXEC_PREDICATES_P(x, y) \
 *   ((x) == reverse_condition (y))  */



/**** node: Costs
 *
 * - Defining relative costs of different operations.  */


/* Optional.  A part of a C `switch' statement that describes the relative
 * costs of constant RTL expressions.  It must contain `case' labels for
 * expression codes `const_int', `const', `symbol_ref', `label_ref' and
 * `const_double'.  Each case must ultimately reach a `return' statement to
 * return the relative cost of the use of that kind of constant value in an
 * expression.  The cost may depend on the precise value of the constant, which
 * is available for examination in X, and the rtx code of the expression in
 * which it is contained, found in OUTER_CODE.  */

/* #define CONST_COSTS(X, CODE, OUTER_CODE) */
  

/* Optional.  Like `CONST_COSTS' but applies to nonconstant RTL expressions.
 * This can be used, for example, to indicate how costly a multiply instruction
 * is.  In writing this macro, you can use the construct `COSTS_N_INSNS (N)' to
 * specify a cost equal to N fast instructions.  OUTER_CODE is the code of the
 * expression in which X is contained.  
 *
 * This macro is optional; do not define it if the default cost assumptions are
 * adequate for the target machine.  */

/* #define RTX_COSTS(X, CODE, OUTER_CODE) */


/* Optional.  This macro, if defined, is called for any case not handled by the
 * `RTX_COSTS' or `CONST_COSTS' macros.  This eliminates the need to put case
 * labels into the macro, but the code, or any functions it calls, must assume
 * that the RTL in X could be of any type that has not already been handled.
 * The arguments are the same as for `RTX_COSTS', and the macro should execute
 * a return statement giving the cost of any RTL expressions that it can
 * handle.  The default cost calculation is used for any RTL for which this
 * macro does not return a value.  
 *
 * On c6x, we use the defaults already implemented in function `rtx_cost'.  */

/* #define DEFAULT_RTX_COSTS(X, CODE, OUTER_CODE) */


/* Optional.  An expression giving the cost of an addressing mode that contains
 * ADDRESS.  If not defined, the cost is computed from the ADDRESS expression
 * and the `CONST_COSTS' values.
 *
 * For discussion when to use this macro, see info gccint, node "Costs".  
 * On c6x, we use the default, which is to let `rtx_cost (x, MEM)' do the
 * computation of costs for addresses.  */

/* #define ADDRESS_COST(ADDRESS) */


/* Optional.  A C expression for the cost of moving data of mode MODE from a
 * register in class FROM to one in class TO.  The classes are expressed using
 * the enumeration values such as `GENERAL_REGS'.  A value of 2 is the default;
 * other values are interpreted relative to that.
 *
 * It is not required that the cost always equal 2 when FROM is the same as TO;
 * on some machines it is expensive to move between registers if they are not
 * general registers.
 *
 * If reload sees an insn consisting of a single `set' between two hard
 * registers, and if `REGISTER_MOVE_COST' applied to their classes returns a
 * value of 2, reload does not check to ensure that the constraints of the insn
 * are met.  Setting a cost of other than 2 will allow reload to verify that
 * the constraints are met.  You should do this if the `movM' pattern's
 * constraints do not allow such copying.  
 *
 * On c6x, we use the default.  */

/* #define REGISTER_MOVE_COST(MODE, FROM, TO)  2 */


/* Optional.  A C expression for the cost of moving data of mode MODE between a
 * register of class CLASS and memory; IN is zero if the value is to be written
 * to memory, nonzero if it is to be read in.  This cost is relative to those
 * in `REGISTER_MOVE_COST'.  If moving between registers and memory is more
 * expensive than between two registers, you should define this macro to
 * express the relative cost.  
 *
 * If you do not define this macro, GCC uses a default cost of 4 plus the cost
 * of copying via a secondary reload register, if one is needed.
 *
 * Refer to info gccint, node "Costs" for more details.
 * On c6x, we use the default.  */

/* #define MEMORY_MOVE_COST(MODE, CLASS, IN)  4 */


/* A C expression for the cost of a branch instruction.  A value of 1 is the
 * default; other values are interpreted relative to that.
 *
 * On c6x, we use 1, even though branching is currently rather expensive
 * because of the lacking delay slot scheduling.  */

/* #define BRANCH_COST  1 */


/* Here are additional macros which do not specify precise relative costs, but
 * only that certain actions are more expensive than GCC would ordinarily
 * expect.  */


/* Define this macro as a C expression which is nonzero if accessing less than
 * a word of memory (i.e. a `char' or a `short') is no faster than accessing a
 * word of memory, i.e., if such access require more than one instruction or if
 * there is no difference in cost between byte and (aligned) word loads.  
 *
 * When this macro is not defined, the compiler will access a field by finding
 * the smallest containing object; when it is defined, a fullword load will be
 * used if alignment permits.  Unless bytes accesses are faster than word
 * accesses, using word accesses is preferable since it may eliminate
 * subsequent memory access if subsequent accesses occur to other fields in the
 * same word of the structure, but to different bytes.  */

#define SLOW_BYTE_ACCESS  1


/* Define this macro to be the value 1 if memory accesses described by the MODE
 * and ALIGNMENT parameters have a cost many times greater than aligned
 * accesses, for example if they are emulated in a trap handler.
 *
 * When this macro is nonzero, the compiler will act as if `STRICT_ALIGNMENT'
 * were nonzero when generating code for block moves.  This can cause
 * significantly more instructions to be produced.  Therefore, do not set this
 * macro nonzero if unaligned accesses only add a cycle or two to the time for
 * a memory access.  
 *
 * If the value of this macro is always zero, it need not be defined.  If this
 * macro is defined, it should produce a nonzero value when `STRICT_ALIGNMENT'
 * is nonzero.  */

/* #define SLOW_UNALIGNED_ACCESS(MODE, ALIGNMENT) */


/* Define this macro to inhibit strength reduction of memory addresses.  (On
 * some machines, such strength reduction seems to do harm rather than good.)
 */

/* #define DONT_REDUCE_ADDR */


/* The threshold of number of scalar memory-to-memory move insns, _below_ which
 * a sequence of insns should be generated instead of a string move insn or a
 * library call.  Increasing the value will always make code faster, but
 * eventually incurs high cost in increased code size.
 *
 * Note that on machines where the corresponding move insn is a `define_expand'
 * that emits a sequence of insns, this macro counts the number of such
 * sequences.
 *
 * If you don't define this, a reasonable default is used.  */

/* #define MOVE_RATIO */


/* A C expression that is nonzero if `move_by_pieces' should be used to copy a
 * chunk of memory, rather than using a block move mechanism.  Defaults to 1 if
 * `move_by_pieces_ninsns' returns less than `MOVE_RATIO'.  */

/* #define MOVE_BY_PIECES_P(SIZE, ALIGNMENT) */


/* A C expression used by `move_by_pieces' to determine the largest unit a load
 * or store used to copy memory is.  Defaults to `MOVE_MAX'.  */

/* #define MOVE_MAX_PIECES */


/* The threshold of number of scalar move insns, _below_ which a sequence of
 * insns should be generated to clear memory instead of a string clear insn or
 * a library call.  Increasing the value will always make code faster, but
 * eventually incurs high cost in increased code size.  
 *
 * If you don't define this, a reasonable default is used.  */

/* #define CLEAR_RATIO */


/* A C expression used to determine whether `clear_by_pieces' will be used to
 * clear a chunk of memory, or whether some other block clear mechanism will be
 * used.  Defaults to 1 if `move_by_pieces_ninsns' returns less than
 * `CLEAR_RATIO'.  */ 

/* #define CLEAR_BY_PIECES_P(SIZE, ALIGNMENT) */


/* A C expression used to determine whether a load postincrement is a good
 * thing to use for a given mode.  Defaults to the value of
 * `HAVE_POST_INCREMENT'.  */ 

/* #define USE_LOAD_POST_INCREMENT(MODE) */


/* A C expression used to determine whether a load postdecrement is a good
 * thing to use for a given mode.  Defaults to the value of
 * `HAVE_POST_DECREMENT'.  */

/* #define USE_LOAD_POST_DECREMENT(MODE) */


/* A C expression used to determine whether a load preincrement is a good thing
 * to use for a given mode.  Defaults to the value of `HAVE_PRE_INCREMENT'.  */

/* #define USE_LOAD_PRE_INCREMENT(MODE) */


/* A C expression used to determine whether a load predecrement is a good thing
 * to use for a given mode.  Defaults to the value of `HAVE_PRE_DECREMENT'.  */

/* #define USE_LOAD_PRE_DECREMENT(MODE) */


/* A C expression used to determine whether a store postincrement is a good
 * thing to use for a given mode.  Defaults to the value of
 * `HAVE_POST_INCREMENT'.  */

/* #define USE_STORE_POST_INCREMENT(MODE) */


/* A C expression used to determine whether a store postdecrement is a good
 * thing to use for a given mode.  Defaults to the value of
 * `HAVE_POST_DECREMENT'.  */

/* #define USE_STORE_POST_DECREMENT(MODE) */


/* This macro is used to determine whether a store preincrement is a good thing
 * to use for a given mode.  Defaults to the value of `HAVE_PRE_INCREMENT'.  */

/* #define USE_STORE_PRE_INCREMENT(MODE) */


/* This macro is used to determine whether a store predecrement is a good thing
 * to use for a given mode.  Defaults to the value of `HAVE_PRE_DECREMENT'.  */

/* #define USE_STORE_PRE_DECREMENT(MODE) */


/* Define this macro if it is as good or better to call a constant function
 * address than to call an address kept in a register.  */

/* #define NO_FUNCTION_CSE */


/* Define this macro if it is as good or better for a function to call itself
 * with an explicit address than to call an address kept in a register.  */

/* #define NO_RECURSIVE_FUNCTION_CSE */



/**** node: Sections
 *
 * - Dividing storage into text, data, and other sections.
 * - see spru186i.pdf, section 4.2: "Directives that define sections"
 *
 * An object file is divided into sections containing different types of
 * data.  In the most common case, there are three sections: the "text
 * section", which holds instructions and read-only data; the "data
 * section", which holds initialized writable data; and the "bss section",
 * which holds uninitialized data.  */


/* The compiler must tell the assembler when to switch sections.  These macros
 * control what commands to output to tell the assembler this.  You can also
 * define additional sections.  */


/* A C expression whose value is a string, including spacing, containing the
 * assembler operation that should precede instructions and read-only data.  */

#define TEXT_SECTION_ASM_OP "\t.text"


/* Optional.  A C statement that switches to the default section containing
 * instructions.  Normally this is not needed, as simply defining
 * `TEXT_SECTION_ASM_OP' is enough.  The MIPS port uses this to sort all
 * functions after all data declarations.  */

/* #define TEXT_SECTION */


/* If defined, a C string constant for the name of the section containing most
 * frequently executed functions of the program.  On c6x, we use the default
 * definition.  */

/* #define HOT_TEXT_SECTION_NAME */


/* If defined, a C string constant for the name of the section containing
 * unlikely executed functions in the program.  On c6x, we use the default
 * definition.  */

/* #define UNLIKELY_EXECUTED_TEXT_SECTION_NAME */


/* A C expression whose value is a string, including spacing, containing the
 * assembler operation to identify the following data as writable initialized
 * data.  */

#define DATA_SECTION_ASM_OP  "\t.data"


/* Optional.  A C expression whose value is a string, including spacing,
 * containing the assembler operation to identify the following data as
 * read-only initialized data.  */

/* #define READONLY_DATA_SECTION_ASM_OP */


/* Optional.  A macro naming a function to call to switch to the proper section
 * for read-only data.  The default is to use `READONLY_DATA_SECTION_ASM_OP' if
 * defined, else fall back to `text_section'.
 *
 * The most common definition will be `data_section', if the target does not
 * have a special read-only data section, and does not put data in the text
 * section.  */

/* #define READONLY_DATA_SECTION */


/* If defined, a C expression whose value is a string, including spacing,
 * containing the assembler operation to identify the following data as shared
 * data.  If not defined, `DATA_SECTION_ASM_OP' will be used.  */

/* #define SHARED_SECTION_ASM_OP */


/* Optional.  If defined, a C expression whose value is a string, including
 * spacing, containing the assembler operation to identify the following data
 * as uninitialized global data.  
 *
 * If not defined, and neither `ASM_OUTPUT_BSS' nor `ASM_OUTPUT_ALIGNED_BSS'
 * are defined, uninitialized global data will be output in the data section if
 * `-fno-common' is passed, otherwise `ASM_OUTPUT_COMMON' will be used.
 *
 * Leave this undefined on c6x, as there is no `.bss' directive, that switches
 * the section like `.text' does, but `.bss' always works for just 1 symbol,
 * of which the name and size must be provided after the `.bss' keyword.  See
 * spru186i.pdf, section "Directives reference".  So we cannot persistently
 * switch to the .bss section.  */

/* #define BSS_SECTION_ASM_OP  "\t.bss" */


/* If defined, a C expression whose value is a string, including spacing,
 * containing the assembler operation to identify the following data as
 * uninitialized global shared data.  If not defined, and `BSS_SECTION_ASM_OP'
 * is, the latter will be used.  */

/* #define SHARED_BSS_SECTION_ASM_OP */


/* If defined, a C expression whose value is a string, including spacing,
 * containing the assembler operation to identify the following data as
 * initialization code.  If not defined, GCC will assume such a section does
 * not exist.  */

/* #define INIT_SECTION_ASM_OP */


/* If defined, a C expression whose value is a string, including spacing,
 * containing the assembler operation to identify the following data as
 * finalization code.  If not defined, GCC will assume such a section does not
 * exist.  */

/* #define FINI_SECTION_ASM_OP */


/* If defined, an ASM statement that switches to a different section via
 * SECTION_OP, calls FUNCTION, and switches back to the text section.  This is
 * used in `crtstuff.c' if `INIT_SECTION_ASM_OP' or `FINI_SECTION_ASM_OP' to
 * calls to initialization and finalization functions from the init and fini
 * sections.  By default, this macro uses a simple function call.  Some ports
 * need hand-crafted assembly code to avoid dependencies on registers
 * initialized in the function prologue or to ensure that constant pools don't
 * end up too far way in the text section.  */

/* #define CRT_CALL_STATIC_FUNCTION (SECTION_OP, FUNCTION) */


/* If defined, an ASM statement that aligns a code section to some arbitrary
 * boundary.  This is used to force all fragments of the `.init' and `.fini'
 * sections to have the same alignment and thus prevent the linker from having
 * to add any padding. The default definition is empty.  */

/* #define FORCE_CODE_SECTION_ALIGN */


/* A list of names for sections other than the standard two, which are
 * `in_text' and `in_data'.  You need not define this macro on a system with no
 * other sections (that GCC needs to use).  */

/* #define EXTRA_SECTIONS */


/* One or more functions to be defined in `varasm.c'.  These functions should
 * do jobs analogous to those of `text_section' and `data_section', for your
 * additional sections.  Do not define this macro if you do not define
 * `EXTRA_SECTIONS'.  */

/* #define EXTRA_SECTION_FUNCTIONS */


/* Define this macro to be an expression with a nonzero value if jump tables
 * (for `tablejump' insns) should be output in the text section, along with the
 * assembler instructions.  Otherwise, the readonly data section is used.
 *
 * This macro is irrelevant if there is no separate readonly data section.  */

#define JUMP_TABLES_IN_TEXT_SECTION  1


/* Target Hook: void TARGET_ASM_SELECT_SECTION (tree EXP, int RELOC,
 *        unsigned HOST_WIDE_INT ALIGN)
 *
 * Switches to the appropriate section for output of EXP.  You can assume that
 * EXP is either a `VAR_DECL' node or a constant of some sort.  RELOC indicates
 * whether the initial value of EXP requires link-time relocations.  Bit 0 is
 * set when variable contains local relocations only, while bit 1 is set for
 * global relocations.  Select the section by calling `data_section' or one of
 * the alternatives for other sections.  ALIGN is the constant alignment in
 * bits.  
 *
 * The default version of this function takes care of putting read-only
 * variables in `readonly_data_section'.  */

/* #define TARGET_ASM_SELECT_SECTION  default_select_section */


/* Target Hook: void TARGET_ASM_UNIQUE_SECTION (tree DECL, int RELOC)
 *
 * Build up a unique section name, expressed as a `STRING_CST' node, and assign
 * it to `DECL_SECTION_NAME (DECL)'.  As with `TARGET_ASM_SELECT_SECTION',
 * RELOC indicates whether the initial value of EXP requires link-time
 * relocations.  
 *
 * The default version of this function appends the symbol name to the ELF
 * section name that would normally be used for the symbol.  For example, the
 * function `foo' would be placed in `.text.foo'.  Whatever the actual target
 * object format, this is often good enough.  */

/* #define TARGET_ASM_UNIQUE_SECTION  default_unique_section */


/* Target Hook: void TARGET_ASM_SELECT_RTX_SECTION (enum machine_mode
 *        MODE, rtx X, unsigned HOST_WIDE_INT ALIGN)
 *
 * Switches to the appropriate section for output of constant pool entry X in
 * MODE.  You can assume that X is some kind of constant in RTL.  The argument
 * MODE is redundant except in the case of a `const_int' rtx.  Select the
 * section by calling `readonly_data_section' or one of the alternatives for
 * other sections.  ALIGN is the constant alignment in bits.
 *
 * The default version of this function takes care of putting symbolic
 * constants in `flag_pic' mode in `data_section' and everything else in
 * `readonly_data_section'.  */

/* #define TARGET_ASM_SELECT_RTX_SECTION  default_select_rtx_section */


/* Target Hook: void TARGET_ENCODE_SECTION_INFO (tree DECL, int
 *        NEW_DECL_P)
 *
 * Define this hook if references to a symbol or a constant must be treated
 * differently depending on something about the variable or function named by
 * the symbol (such as what section it is in).
 *
 * The hook is executed under two circumstances.  One is immediately after the
 * rtl for DECL that represents a variable or a function has been created and
 * stored in `DECL_RTL(DECL)'.  The value of the rtl will be a `mem' whose
 * address is a `symbol_ref'.  The other is immediately after the rtl for DECL
 * that represents a constant has been created and stored in `TREE_CST_RTL
 * (DECL)'.  The macro is called once for each distinct constant in a source
 * file.
 *
 * The NEW_DECL_P argument will be true if this is the first time that
 * `ENCODE_SECTION_INFO' has been invoked on this decl.  It will be false for
 * subsequent invocations, which will happen for duplicate declarations.
 * Whether or not anything must be done for the duplicate declaration depends
 * on whether the hook examines `DECL_ATTRIBUTES'.
 *
 * The usual thing for this hook to do is to record a flag in the `symbol_ref'
 * (such as `SYMBOL_REF_FLAG') or to store a modified name string in the
 * `symbol_ref' (if one bit is not enough information).  */

/* #define TARGET_ENCODE_SECTION_INFO  hook_void_tree_int */


/* Target Hook: const char *TARGET_STRIP_NAME_ENCODING (const char
 *        *name)
 *
 * Decode NAME and return the real name part, sans the characters that
 * `TARGET_ENCODE_SECTION_INFO' may have added.  */

/* #define TARGET_STRIP_NAME_ENCODING  default_strip_name_encoding */


/* Target Hook: bool TARGET_IN_SMALL_DATA_P (tree EXP)
 *
 * Returns true if EXP should be placed into a "small data" section.  The
 * default version of this hook always returns false.  */

/* #define TARGET_IN_SMALL_DATA_P  hook_bool_tree_false */


/* Variable: Target Hook bool TARGET_HAVE_SRODATA_SECTION
 *
 * Contains the value true if the target places read-only "small data" into a
 * separate section.  The default value is false.  */

/* #define TARGET_HAVE_SRODATA_SECTION  false */


/* Target Hook: bool TARGET_BINDS_LOCAL_P (tree EXP)
 *
 * Returns true if EXP names an object for which name resolution rules must
 * resolve to the current "module" (dynamic shared library or executable
 * image).
 *
 * The default version of this hook implements the name resolution rules for
 * ELF, which has a looser model of global name binding than other currently
 * supported object file formats.  */

/* #define TARGET_BINDS_LOCAL_P default_binds_local_p */


/* Variable: Target Hook bool TARGET_HAVE_TLS
 *
 * Contains the value true if the target supports thread-local storage.  The
 * default value is false.  */

/* #define TARGET_HAVE_TLS false */



/**** node: PIC
 *
 * - Macros for position independent code.
 *
 * Simply defining these macros is not enough to generate valid PIC; you must
 * also add support to the macros `GO_IF_LEGITIMATE_ADDRESS' and
 * `PRINT_OPERAND_ADDRESS', as well as `LEGITIMIZE_ADDRESS'.  You must modify
 * the definition of `movsi' to do something appropriate when the source
 * operand contains a symbolic address.  You may also need to alter the
 * handling of switch statements so that they use relative addresses.  */


/* The register number of the register used to address a table of static data
 * addresses in memory.  In some cases this register is defined by a
 * processor's "application binary interface" (ABI).  When this macro is
 * defined, RTL is generated for this register once, as with the stack pointer
 * and frame pointer registers.  If this macro is not defined, it is up to the
 * machine-dependent files to allocate such a register (if necessary).  Note
 * that this register must be fixed when in use (e.g.  when `flag_pic' is
 * true).
 *
 * On c6x, we leave it undefined and use the default, which is to disable PIC
 * support.  */

/* #define PIC_OFFSET_TABLE_REGNUM  INVALID_REGNUM */


/* Define this macro if the register defined by `PIC_OFFSET_TABLE_REGNUM' is
 * clobbered by calls.  Do not define this macro if `PIC_OFFSET_TABLE_REGNUM'
 * is not defined.  */

/* #define PIC_OFFSET_TABLE_REG_CALL_CLOBBERED */


/* By generating position-independent code, when two different programs (A and
 * B) share a common library (libC.a), the text of the library can be shared
 * whether or not the library is linked at the same address for both programs.
 * In some of these environments, position-independent code requires not only
 * the use of different addressing modes, but also special code to enable the
 * use of these addressing modes.
 *
 * The `FINALIZE_PIC' macro serves as a hook to emit these special codes once
 * the function is being compiled into assembly code, but not before.  (It is
 * not done before, because in the case of compiling an inline function, it
 * would lead to multiple PIC prologues being included in functions which used
 * inline functions and were compiled to assembly language.)  */

/* #define FINALIZE_PIC */


/* A C expression that is nonzero if X is a legitimate immediate operand on
 * the target machine when generating position independent code.  You can
 * assume that X satisfies `CONSTANT_P', so you need not check this.  You can
 * also assume FLAG_PIC is true, so you need not check it either.  You need
 * not define this macro if all constants (including `SYMBOL_REF') can be
 * immediate operands when generating position independent code.  */

/* #define LEGITIMATE_PIC_OPERAND_P(X) */



/**** node: Assembler Format
 *
 * - Defining how to write insns and pseudo-ops to output.  */



/*** subnode: File Framework
 * - Structural information for the assembler file.  */


/* A C expression which outputs to the stdio stream STREAM some appropriate
 * text to go at the start of an assembler file.
 *
 * Normally this macro is defined to output a line containing `#NO_APP', which
 * is a comment that has no effect on most assemblers but tells the GNU
 * assembler that it can save time by not checking for certain assembler
 * constructs.
 *
 * On systems that use SDB, it is necessary to output certain commands; see
 * `attasm.h'.  */

/* #define ASM_FILE_START(STREAM) */


/*A C expression which outputs to the stdio stream STREAM some appropriate text
 * to go at the end of an assembler file.
 *
 * If this macro is not defined, the default is to output nothing special at
 * the end of the file.  Most systems don't require any definition.
 *
 * On systems that use SDB, it is necessary to output certain commands; see
 * `attasm.h'.  */

#define ASM_FILE_END(STREAM)


/* A C string constant describing how to begin a comment in the target
 * assembler language.  The compiler assumes that the comment will end at the
 * end of the line.  
 *
 * For c6x, see spru186i.pdf, section 3.5.5, "Comment Field".  */

#define ASM_COMMENT_START ";"


/* A C string constant for text to be output before each `asm' statement or
 * group of consecutive ones.  Normally this is `"#APP"', which is a comment
 * that has no effect on most assemblers but tells the GNU assembler that it
 * must check the lines that follow for all valid assembler constructs.  */

#define ASM_APP_ON "\t; #APP \n"


/* A C string constant for text to be output after each `asm' statement or
 * group of consecutive ones.  Normally this is `"#NO_APP"', which tells the
 * GNU assembler to resume making the time-saving assumptions that are valid
 * for ordinary compiler output.  */

#define ASM_APP_OFF "\t; #NO_APP \n"


/* A C statement to output COFF information or DWARF debugging information
 * which indicates that filename NAME is the current source file to the stdio
 * stream STREAM.
 *
 * If not defined, a `.file' directive is output, see `output_file_directive'.
 */

/* #define ASM_OUTPUT_SOURCE_FILENAME(STREAM, NAME) */


/* A C statement to output the string STRING to the stdio stream STREAM.  If
 * you do not call the function `output_quoted_string' in your config files,
 * GCC will only call it to output filenames to the assembler source.  So you
 * can use it to canonicalize the format of the filename using this macro.
 *
 * On c6x, we use the default.  */

/* #define OUTPUT_QUOTED_STRING(STREAM, STRING) */


/* A C statement to output DBX or SDB debugging information before code for
 * line number LINE of the current source file to the stdio stream STREAM.
 *
 * This macro need not be defined if the standard form of debugging information
 * for the debugger in use is appropriate. See `dbxout.c' `sdbout.c,' or
 * `xcoffout.c', respectively.  */

/* #define ASM_OUTPUT_SOURCE_LINE(STREAM, LINE) */


/* A C statement to output something to the assembler file to handle a `#ident'
 * directive containing the text STRING.  If this macro is not defined, nothing
 * is output for a `#ident' directive.  */

/* #define ASM_OUTPUT_IDENT(STREAM, STRING) */

/* A C statement to output any assembler statements which are required to
 * precede any Objective-C object definitions or message sending.  The
 * statement is executed only when compiling an Objective-C program.  */

/* #define OBJC_PROLOGUE */


/* Target Hook: void TARGET_ASM_NAMED_SECTION (const char *NAME,
 *        unsigned int FLAGS, unsigned int ALIGN)
 *
 * Output assembly directives to switch to section NAME.  The section should
 * have attributes as specified by FLAGS, which is a bit mask of the
 * `SECTION_*' flags defined in `output.h'.  If ALIGN is nonzero, it contains
 * an alignment in bytes to be used for the section, otherwise some target
 * default should be used.  Only targets that must specify an alignment within
 * the section directive need pay attention to ALIGN - we will still use
 * `ASM_OUTPUT_ALIGN'.  */

/* #define TARGET_ASM_NAMED_SECTION default_no_named_section */


/* Target Hook: bool TARGET_HAVE_NAMED_SECTIONS
 *   This flag is always defined to be true if the target has a definition
 *   for `TARGET_ASM_NAMED_SECTION'.  */


/* Target Hook: unsigned int TARGET_SECTION_TYPE_FLAGS (tree DECL,
 *        const char *NAME, int RELOC)
 *
 * Choose a set of section attributes for use by `TARGET_ASM_NAMED_SECTION'
 * based on a variable or function decl, a section name, and whether or not the
 * declaration's initializer may contain runtime relocations.  DECL may be
 * null, in which case read-write data should be assumed.
 *
 * The default version if this function handles choosing code vs data,
 * read-only vs read-write data, and `flag_pic'.  You should only need to
 * override this if your target has special flags that might be set via
 * `__attribute__'.  */

/* #define TARGET_SECTION_TYPE_FLAGS default_section_type_flags */



/*** subnode: Data Output
 * - Output of constants (numbers, strings, addresses).  */


/* The following 9 hooks specify assembly directives for creating certain kinds
 * of integer object.  The `TARGET_ASM_BYTE_OP' directive creates a byte-sized
 * object, the `TARGET_ASM_ALIGNED_HI_OP' one creates an aligned two-byte
 * object, and so on.  Any of the hooks may be `NULL', indicating that no
 * suitable directive is available.  Function `assemble_integer' will try to
 * split the values into smaller units in this case.
 *
 * The compiler will print these strings at the start of a new line, followed
 * immediately by the object's initial value.  In most cases, the string should
 * contain a tab, a pseudo-op, and then another tab.
 *
 * For c6x-specific documentation, see: spru186i.pdf, 
 * section "Directives Reference" */

/* Target Hook: const char * TARGET_ASM_BYTE_OP  */
/* #define TARGET_ASM_BYTE_OP "\t.byte\t" */

/* Target Hook: const char * TARGET_ASM_ALIGNED_HI_OP  */
/* #define TARGET_ASM_ALIGNED_HI_OP "\t.short\t" */

/* Target Hook: const char * TARGET_ASM_ALIGNED_SI_OP  */
/* #define TARGET_ASM_ALIGNED_SI_OP "\t.word\t" */

/* Target Hook: const char * TARGET_ASM_ALIGNED_DI_OP  */
/* #define TARGET_ASM_ALIGNED_DI_OP NULL */

/* Target Hook: const char * TARGET_ASM_ALIGNED_TI_OP  */
/* #define TARGET_ASM_ALIGNED_TI_OP NULL */

/* Target Hook: const char * TARGET_ASM_UNALIGNED_HI_OP  */
/* #define TARGET_ASM_UNALIGNED_HI_OP "\t.space 2\t" */

/* Target Hook: const char * TARGET_ASM_UNALIGNED_SI_OP  */
/* #define TARGET_ASM_UNALIGNED_SI_OP "\t.space 4\t" */

/* Target Hook: const char * TARGET_ASM_UNALIGNED_DI_OP  */
/* #define TARGET_ASM_UNALIGNED_DI_OP "\t.space 8\t" */

/* Target Hook: const char * TARGET_ASM_UNALIGNED_TI_OP  */
/* #define TARGET_ASM_UNALIGNED_TI_OP "\t.space 16\t" */


/* Target Hook: bool TARGET_ASM_INTEGER (rtx X, unsigned int SIZE, int
 *        ALIGNED_P)
 *
 * The `assemble_integer' function uses this hook to output an integer object.
 * X is the object's value, SIZE is its size in bytes and ALIGNED_P indicates
 * whether it is aligned.  The function should return `true' if it was able to
 * output the object.  If it returns false, `assemble_integer' will try to
 * split the object into smaller parts.
 *
 * The default implementation of this hook will use the `TARGET_ASM_BYTE_OP'
 * family of strings, returning `false' when the relevant string is `NULL'.  */

/* #define TARGET_ASM_INTEGER  default_assemble_integer */


/* A C statement to recognize RTX patterns that `output_addr_const' can't deal
 * with, and output assembly code to STREAM corresponding to the pattern X.
 * This may be used to allow machine-dependent `UNSPEC's to appear within
 * constants.
 *
 * If `OUTPUT_ADDR_CONST_EXTRA' fails to recognize a pattern, it must `goto
 * fail', so that a standard error message is printed.  If it prints an error
 * message itself, by calling, for example, `output_operand_lossage', it may
 * just complete normally.  */

/* #define OUTPUT_ADDR_CONST_EXTRA(STREAM, X, FAIL) */


/* A C statement to output to the stdio stream STREAM an assembler instruction
 * to assemble a string constant containing the LEN bytes at PTR.  PTR will be
 * a C expression of type `char *' and LEN a C expression of type `int'.
 *
 * If the assembler has a `.ascii' pseudo-op as found in the Berkeley Unix
 * assembler, do not define the macro `ASM_OUTPUT_ASCII'.  */

/* #define ASM_OUTPUT_ASCII(STREAM, PTR, LEN) */


/* A C statement to output word N of a function descriptor for DECL.  This must
 * be defined if `TARGET_VTABLE_USES_DESCRIPTORS' is defined, and is otherwise
 * unused.  */

/* #define ASM_OUTPUT_FDESC(STREAM, DECL, N) */


/* Define this macro as C expression that expands to zero, if GCC should output
 * the constant pool for a function _after_ the code for the function.
 * Otherwisse, if the macro is undefined or has a nonzero value, the constant
 * pool will be output _before_ the function.  */

/* #define CONSTANT_POOL_BEFORE_FUNCTION */


/* A C statement to output assembler commands to define the start of the
 * constant pool for a function.  FUNNAME is a string giving the name of the
 * function.  Should the return type of the function be required, it can be
 * obtained via FUNDECL.  SIZE is the size, in bytes, of the constant pool that
 * will be written immediately after this call.
 *
 * If no constant-pool prefix is required, the usual case, this macro need not
 * be defined.  */

/* #define ASM_OUTPUT_POOL_PROLOGUE(FILE, FUNNAME, FUNDECL, SIZE) */


/* Optional.  A C statement (with or without semicolon) to output a constant in
 * the constant pool, if it needs special treatment.  (This macro need not do
 * anything for RTL expressions that can be output normally.)  For more
 * details, see info gccint, node "Data Output".  */

/* #define ASM_OUTPUT_SPECIAL_POOL_ENTRY(FILE, X, MODE, ALIGN, LABELNO, JUMPTO) */


/* Define this macro as a C expression which is nonzero if the constant EXP, of
 * type `tree', should be output after the code for a function.  The compiler
 * will normally output all constants before the function; you need not define
 * this macro if this is OK.  */

/* #define CONSTANT_AFTER_FUNCTION_P(EXP) */


/* A C statement to output assembler commands to at the end of the constant
 * pool for a function.  FUNNAME is a string giving the name of the function.
 * Should the return type of the function be required, you can obtain it via
 * FUNDECL.  SIZE is the size, in bytes, of the constant pool that GCC wrote
 * immediately before this call.
 *
 * If no constant-pool epilogue is required, the usual case, you need not
 * define this macro.  */

/* #define ASM_OUTPUT_POOL_EPILOGUE(FILE FUNNAME FUNDECL SIZE) */


/* Define this macro as a C expression which is nonzero if C is used as a
 * logical line separator by the assembler.
 *
 * If you do not define this macro, the default is that only the character `;'
 * (and `\n', see function `asm_insn_count') is treated as a logical line
 * separator.  */

/* #define IS_ASM_LOGICAL_LINE_SEPARATOR(C) ((C) == ';') */


/* The two following target hooks are C string constants, describing the syntax
 * in the assembler for grouping arithmetic expressions.  If not overridden,
 * they default to normal parentheses, which is correct for most assemblers.
 * */

/* Target Hook: const char * TARGET_ASM_OPEN_PAREN  */
/* #define TARGET_ASM_OPEN_PAREN "(" */

/* Target Hook: const char * TARGET_ASM_CLOSE_PAREN  */
/* #define TARGET_ASM_CLOSE_PAREN ")" */


/* The following 3 macros are provided by `real.h' for writing the definitions
 * of `ASM_OUTPUT_DOUBLE' and the like.
 *
 * They translate X, of type `REAL_VALUE_TYPE', to the target's floating point
 * representation, and store its bit pattern in the variable L.  For
 * `REAL_VALUE_TO_TARGET_SINGLE', this variable should be a simple `long int'.
 * For the others, it should be an array of `long int'.  The number of elements
 * in this array is determined by the size of the desired target floating point
 * data type: 32 bits of it go in each `long int' array element.  Each array
 * element holds 32 bits of the result, even if `long int' is wider than 32
 * bits on the host machine.
 *
 * The array element values are designed so that you can print them out using
 * `fprintf' in the order they should appear in the target machine's memory.  */

/* REAL_VALUE_TO_TARGET_SINGLE(X, L) */
/* REAL_VALUE_TO_TARGET_DOUBLE (X, L) */
/* REAL_VALUE_TO_TARGET_LONG_DOUBLE(X, L) */



/*** subnode: Uninitialized Data
 * - Output of uninitialized variables.
 *
 * Each of the macros in this section is used to do the whole job of outputting
 * a single uninitialized variable.  */


/* Mandatory.  A C statement (sans semicolon) to output to the stdio stream
 * STREAM the assembler definition of a common-label named NAME whose size is
 * SIZE bytes.  The variable ROUNDED is the size rounded up to whatever
 * alignment the caller wants.
 *
 * Use the expression `assemble_name (STREAM, NAME)' to output the name itself;
 * before and after that, output the additional assembler syntax for defining
 * the name, and a newline.
 *
 * This macro controls how the assembler definitions of uninitialized common
 * global variables are output.
 *
 *
 * ??? On c6x, there was a linker error in function `asm_emit_uninitialised'
 * when I left this macro undefined.  Where is the definition on ia64?
 * The only def I could find was in svr3.h, but is it included
 */

#define ASM_OUTPUT_COMMON(STREAM, NAME, SIZE, ROUNDED) \
  do								\
    {								\
      fprintf (STREAM, "\t.bss %s, %d, %d:\n", 			\
	  (NAME), (SIZE), BIGGEST_ALIGNMENT);			\
    }								\
  while (0)


/* Like `ASM_OUTPUT_COMMON' except takes the required alignment as a separate,
 * explicit argument.  If you define this macro, it is used in place of
 * `ASM_OUTPUT_COMMON', and gives you more flexibility in handling the required
 * alignment of the variable.  The alignment is specified as the number of
 * bits.  */

/* #define ASM_OUTPUT_ALIGNED_COMMON(STREAM, NAME, SIZE, ALIGNMENT) */


/* Like `ASM_OUTPUT_ALIGNED_COMMON' except that DECL of the variable to be
 * output, if there is one, or `NULL_TREE' if there is no corresponding
 * variable.  If you define this macro, GCC will use it in place of both
 * `ASM_OUTPUT_COMMON' and `ASM_OUTPUT_ALIGNED_COMMON'.  Define this macro when
 * you need to see the variable's decl in order to chose what to output.  */

/* #define ASM_OUTPUT_ALIGNED_DECL_COMMON(STREAM, DECL, NAME, SIZE, ALIGNMENT) */


/* If defined, it is similar to `ASM_OUTPUT_COMMON', except that it is used
 * when NAME is shared.  If not defined, `ASM_OUTPUT_COMMON' will be used.  */

/* #define ASM_OUTPUT_SHARED_COMMON(STREAM, NAME, SIZE, ROUNDED) */


/* A C statement (sans semicolon) to output to the stdio stream STREAM the
 * assembler definition of uninitialized global DECL named NAME whose size is
 * SIZE bytes.  The variable ROUNDED is the size rounded up to whatever
 * alignment the caller wants.
 *
 * Try to use function `asm_output_bss' defined in `varasm.c' when defining
 * this macro.  If unable, use the expression `assemble_name (STREAM, NAME)' to
 * output the name itself; before and after that, output the additional
 * assembler syntax for defining the name, and a newline.
 *
 * This macro controls how the assembler definitions of uninitialized global
 * variables are output.  This macro exists to properly support languages like
 * C++ which do not have `common' data.  However, this macro currently is not
 * defined for all targets.  If this macro and `ASM_OUTPUT_ALIGNED_BSS' are not
 * defined then `ASM_OUTPUT_COMMON' or `ASM_OUTPUT_ALIGNED_COMMON' or
 * `ASM_OUTPUT_ALIGNED_DECL_COMMON' is used.  */

/* #define ASM_OUTPUT_BSS(STREAM, DECL, NAME, SIZE, ROUNDED) */


/* Like `ASM_OUTPUT_BSS' except takes the required alignment as a separate,
 * explicit argument.  If you define this macro, it is used in place of
 * `ASM_OUTPUT_BSS', and gives you more flexibility in handling the required
 * alignment of the variable.  The alignment is specified as the number of
 * bits.  */

#define ASM_OUTPUT_ALIGNED_BSS(STREAM, DECL, NAME, SIZE, ALIGN) \
  do								\
    {								\
      fprintf (STREAM, "\t.bss %s, %d, %d:\n",			\
	  (NAME), (SIZE), BIGGEST_ALIGNMENT);			\
    }								\
  while (0)


/* If defined, it is similar to `ASM_OUTPUT_BSS', except that it is used when
 * NAME is shared.  If not defined, `ASM_OUTPUT_BSS' will be used.  */

/* #define ASM_OUTPUT_SHARED_BSS(STREAM, DECL, NAME, SIZE, ROUNDED) */


/* A C statement (sans semicolon) to output to the stdio stream STREAM the
 * assembler definition of a local-common-label named NAME whose size is SIZE
 * bytes.  The variable ROUNDED is the size rounded up to whatever alignment
 * the caller wants.
 *
 * Use the expression `assemble_name (STREAM, NAME)' to output the name itself;
 * before and after that, output the additional assembler syntax for defining
 * the name, and a newline.
 *
 * This macro controls how the assembler definitions of uninitialized static
 * variables are output.  */

#define ASM_OUTPUT_LOCAL(STREAM, NAME, SIZE, ROUNDED) \
  do								\
    {								\
      fprintf (STREAM, "\t.bss %s, %d, %d:\n",			\
	  (NAME), (SIZE), BIGGEST_ALIGNMENT);			\
    }								\
  while (0)
  


/* Like `ASM_OUTPUT_LOCAL' except takes the required alignment as a separate,
 * explicit argument.  If you define this macro, it is used in place of
 * `ASM_OUTPUT_LOCAL', and gives you more flexibility in handling the required
 * alignment of the variable.  The alignment is specified as the number of
 * bits.  */

/* #define ASM_OUTPUT_ALIGNED_LOCAL(STREAM, NAME, SIZE, ALIGNMENT) */


/* Like `ASM_OUTPUT_ALIGNED_DECL' except that DECL of the variable to be
 * output, if there is one, or `NULL_TREE' if there is no corresponding
 * variable.  If you define this macro, GCC will use it in place of both
 * `ASM_OUTPUT_DECL' and `ASM_OUTPUT_ALIGNED_DECL'.  Define this macro when you
 * need to see the variable's decl in order to chose what to output.  */

/* #define ASM_OUTPUT_ALIGNED_DECL_LOCAL(STREAM, DECL, NAME, SIZE, ALIGNMENT) */


/* If defined, it is similar to `ASM_OUTPUT_LOCAL', except that it is used when
 * NAME is shared.  If not defined, `ASM_OUTPUT_LOCAL' will be used.  */

/* #define ASM_OUTPUT_SHARED_LOCAL(STREAM, NAME, SIZE, ROUNDED) */



/*** subnode: Label Output
 * - Output and generation of labels.  */


/* A C statement (sans semicolon) to output to the stdio stream STREAM the
 * assembler definition of a label named NAME.  Use the expression
 * `assemble_name (STREAM, NAME)' to output the name itself; before and after
 * that, output the additional assembler syntax for defining the name, and a
 * newline.  A default definition of this macro is provided which is correct
 * for most systems.  */

/* #define ASM_OUTPUT_LABEL(STREAM, NAME) */


/* A C string containing the appropriate assembler directive to specify the
 * size of a symbol, without any arguments.  On systems that use ELF, the
 * default (in `config/elfos.h') is `"\t.size\t"'; on other systems, the
 * default is not to define this macro.
 *
 * Define this macro only if it is correct to use the default definitions of
 * `ASM_OUTPUT_SIZE_DIRECTIVE' and `ASM_OUTPUT_MEASURED_SIZE' for your system.
 * If you need your own custom definitions of those macros, or if you do not
 * need explicit symbol sizes at all, do not define this macro.  */

/* #define SIZE_ASM_OP */


/* Optional.  A C statement (sans semicolon) to output to the stdio stream
 * STREAM a directive telling the assembler that the size of the symbol NAME is
 * SIZE.  SIZE is a `HOST_WIDE_INT'.  
 *
 * This macro is used in the definition of `ASM_DECLARE_OBJECT_NAME' and 
 * `ASM_FINISH_DECLARE_OBJECT' in `elfos.h', `netbsd-aout.h', `openbsd.h',
 * and by some targets.  These two macros are optional -- they are used with
 * #ifdef around or get an empty default definition.  So
 * `ASM_OUTPUT_SIZE_DIRECTIVE' (this macro) is optional as well.
 *
 * If you define `SIZE_ASM_OP', a default definition of this macro is provided,
 * otherwise there is none.  */

/* #define ASM_OUTPUT_SIZE_DIRECTIVE(STREAM, NAME, SIZE) */


/* Optional.  A C statement (sans semicolon) to output to the stdio stream
 * STREAM a directive telling the assembler to calculate the size of the symbol
 * NAME by subtracting its address from the current address.
 *
 * If you define `SIZE_ASM_OP', a default definition of this macro is provided.
 * The default assumes that the assembler recognizes a special `.' symbol as
 * referring to the current address, and can calculate the difference between
 * this and another symbol.  If your assembler does not recognize `.' or cannot
 * do calculations with it, you will need to redefine
 * `ASM_OUTPUT_MEASURED_SIZE' to use some other technique.  */

/* #define ASM_OUTPUT_MEASURED_SIZE(STREAM, NAME) */


/* A C string containing the appropriate assembler directive to specify the
 * type of a symbol, without any arguments.  On systems that use ELF, the
 * default (in `config/elfos.h') is `"\t.type\t"'; on other systems, the
 * default is not to define this macro.
 *
 * Define this macro only if it is correct to use the default definition of
 * `ASM_OUTPUT_TYPE_DIRECTIVE' for your system.  If you need your own custom
 * definition of this macro, or if you do not need explicit symbol types at
 * all, do not define this macro.  */

/* #define TYPE_ASM_OP */


/* A C string which specifies (using `printf' syntax) the format of the second
 * operand to `TYPE_ASM_OP'.  On systems that use ELF, the default (in
 * `config/elfos.h') is `"@%s"'; on other systems, the default is not to define
 * this macro.
 *
 * Define this macro only if it is correct to use the default definition of
 * `ASM_OUTPUT_TYPE_DIRECTIVE' for your system.  If you need your own custom
 * definition of this macro, or if you do not need explicit symbol types at
 * all, do not define this macro.  */

/* #define TYPE_OPERAND_FMT */


/* Optional.  A C statement (sans semicolon) to output to the stdio stream
 * STREAM a directive telling the assembler that the type of the symbol NAME is
 * TYPE.  TYPE is a C string; currently, that string is always either
 * `"function"' or `"object"', but you should not count on this.
 *
 * If you define `TYPE_ASM_OP' and `TYPE_OPERAND_FMT', a default definition of
 * this macro is provided.  */

/* #define ASM_OUTPUT_TYPE_DIRECTIVE(STREAM, TYPE) */


/* Optional.  A C statement (sans semicolon) to output to the stdio stream
 * STREAM any text necessary for declaring the name NAME of a function which is
 * being defined.  This macro is responsible for outputting the label
 * definition (perhaps using `ASM_OUTPUT_LABEL').  The argument DECL is the
 * `FUNCTION_DECL' tree node representing the function.
 *
 * If this macro is not defined, then the function name is defined in the usual
 * manner as a label (by means of `ASM_OUTPUT_LABEL').  */

/* #define ASM_DECLARE_FUNCTION_NAME(STREAM, NAME, DECL) */


/* Optional.  A C statement (sans semicolon) to output to the stdio stream
 * STREAM any text necessary for declaring the size of a function which is
 * being defined.  The argument NAME is the name of the function.  The argument
 * DECL is the `FUNCTION_DECL' tree node representing the function.
 *
 * If this macro is not defined, then the function size is not defined.  */

/* #define ASM_DECLARE_FUNCTION_SIZE(STREAM, NAME, DECL) */


/* Optional.  A C statement (sans semicolon) to output to the stdio stream
 * STREAM any text necessary for declaring the name NAME of an initialized
 * variable which is being defined.  This macro must output the label
 * definition (perhaps using `ASM_OUTPUT_LABEL').  The argument DECL is the
 * `VAR_DECL' tree node representing the variable.
 *
 * If this macro is not defined, then the variable name is defined in the usual
 * manner as a label (by means of `ASM_OUTPUT_LABEL').  */

/* #define ASM_DECLARE_OBJECT_NAME(STREAM, NAME, DECL) */


/* Optional.  A C statement (sans semicolon) to output to the stdio stream
 * STREAM any text necessary for claiming a register REGNO for a global
 * variable DECL with name NAME.
 *
 * If you don't define this macro, that is equivalent to defining it to do
 * nothing.  */

/* #define ASM_DECLARE_REGISTER_GLOBAL(STREAM, DECL, REGNO, NAME) */


/* A C statement (sans semicolon) to finish up declaring a variable name once
 * the compiler has processed its initializer fully and thus has had a chance
 * to determine the size of an array when controlled by an initializer.  This
 * is used on systems where it's necessary to declare something about the size
 * of the object.
 *
 * Not defining this macro is equivalent to defining it to do nothing.
 *
 * You may wish to use `ASM_OUTPUT_SIZE_DIRECTIVE' and/or
 * `ASM_OUTPUT_MEASURED_SIZE' in the definition of this macro.  */

/* #define ASM_FINISH_DECLARE_OBJECT(STREAM, DECL, TOPLEVEL, ATEND) */


/* The directive to output before the name of a label that is needed to make it
 * global.  Used in the default implementation of TARGET_ASM_GLOBALIZE_LABEL.
 */

#define GLOBAL_ASM_OP "\t.global\t"


/* Optional.  
 * Target Hook: void TARGET_ASM_GLOBALIZE_LABEL (FILE *STREAM, const
 *        char *NAME)
 *
 * This target hook is a function to output to the stdio stream STREAM some
 * commands that will make the label NAME global; that is, available for
 * reference from other files.
 *
 * The default implementation relies on a proper definition of `GLOBAL_ASM_OP'.
 */

/* #define TARGET_ASM_GLOBALIZE_LABEL default_globalize_label */


/* Optional.  A C statement (sans semicolon) to output to the stdio stream
 * STREAM some commands that will make the label NAME weak; that is, available
 * for reference from other files but only used if no other definition is
 * available.  Use the expression `assemble_name (STREAM, NAME)' to output the
 * name itself; before and after that, output the additional assembler syntax
 * for making that name weak, and a newline.
 *
 * If you don't define this macro or `ASM_WEAKEN_DECL', GCC will not support
 * weak symbols and you should not define the `SUPPORTS_WEAK' macro.  */

/* #define ASM_WEAKEN_LABEL(STREAM, NAME) */


/* Optional.  Combines (and replaces) the function of `ASM_WEAKEN_LABEL' and
 * `ASM_OUTPUT_WEAK_ALIAS', allowing access to the associated function or
 * variable decl.  If VALUE is not `NULL', this C statement should output to
 * the stdio stream STREAM assembler code which defines (equates) the weak
 * symbol NAME to have the value VALUE.  If VALUE is `NULL', it should output
 * commands to make NAME weak.  */

/* #define ASM_WEAKEN_DECL(STREAM, DECL, NAME, VALUE) */


/* A C expression which evaluates to true if the target supports weak symbols.
 *
 * If you don't define this macro, `defaults.h' provides a default definition.
 * If either `ASM_WEAKEN_LABEL' or `ASM_WEAKEN_DECL' is defined, the default
 * definition is `1'; otherwise, it is `0'.  Define this macro if you want to
 * control weak symbol support with a compiler flag such as `-melf'.  */

/* #define SUPPORTS_WEAK */


/* Optional.  A C statement (sans semicolon) to mark DECL to be emitted as a
 * public symbol such that extra copies in multiple translation units will be
 * discarded by the linker.  Define this macro if your object file format
 * provides support for this concept, such as the `COMDAT' section flags in the
 * Microsoft Windows PE/COFF format, and this support requires changes to DECL,
 * such as putting it in a separate section.  */

/* #define MAKE_DECL_ONE_ONLY */


/* Optional.  A C expression which evaluates to true if the target supports
 * one-only semantics.
 *
 * If you don't define this macro, `varasm.c' provides a default definition.
 * If `MAKE_DECL_ONE_ONLY' is defined, the default definition is `1';
 * otherwise, it is `0'.  Define this macro if you want to control one-only
 * symbol support with a compiler flag, or if setting the `DECL_ONE_ONLY' flag
 * is enough to mark a declaration to be emitted as one-only.  */

/* #define SUPPORTS_ONE_ONLY */


/* Target Hook: void TARGET_ASM_ASSEMBLE_VISIBILITY (tree DECL,
 *                      const char *VISIBILITY)
 *
 * This target hook is a function to output to ASM_OUT_FILE some commands that
 * will make the symbol(s) associated with DECL have hidden, protected or
 * internal visibility as specified by VISIBILITY.  */

/* #define TARGET_ASM_ASSEMBLE_VISIBILITY default_assemble_visibility */


/* Optional.  A C statement (sans semicolon) to output to the stdio stream
 * STREAM any text necessary for declaring the name of an external symbol named
 * NAME which is referenced in this compilation but not defined.  The value of
 * DECL is the tree node for the declaration.
 *
 * This macro need not be defined if it does not need to output anything.  The
 * GNU assembler and most Unix assemblers don't require anything.  */

/* #define ASM_OUTPUT_EXTERNAL(STREAM, DECL, NAME) */


/* A C statement (sans semicolon) to output on STREAM an assembler pseudo-op to
 * declare a library function name external.  The name of the library function
 * is given by SYMREF, which has type `rtx' and is a `symbol_ref'.
 *
 * This macro need not be defined if it does not need to output anything.  The
 * GNU assembler and most Unix assemblers don't require anything.
 *
 * On ELF systems, by default, this macro uses
 * `targetm.asm_out.globalize_label' (hook `TARGET_ASM_GLOBALIZE_LABEL'),
 * which relies on `GLOBAL_ASM_OP' being defined.  */

/* #define ASM_OUTPUT_EXTERNAL_LIBCALL(STREAM, SYMREF) */


/* Optional.  A C statement (sans semicolon) to output to the stdio stream
 * STREAM a reference in assembler syntax to a label named NAME.  This should
 * add `_' to the front of the name, if that is customary on your operating
 * system, as it is in most Berkeley Unix systems.  This macro is used in
 * `assemble_name'.  */

/* #define ASM_OUTPUT_LABELREF(STREAM, NAME) */


/* Optional.  A C statement (sans semicolon) to output a reference to
 * `SYMBOL_REF' SYM.  If not defined, `assemble_name' will be used to output
 * the name of the symbol.  This macro may be used to modify the way a symbol
 * is referenced depending on information encoded by
 * `TARGET_ENCODE_SECTION_INFO'.  */

/* #define ASM_OUTPUT_SYMBOL_REF(STREAM, SYM) */


/* Optional.  A C statement (sans semicolon) to output a reference to BUF, the
 * result of `ASM_GENERATE_INTERNAL_LABEL'.  If not defined, `assemble_name'
 * will be used to output the name of the symbol.  This macro is not used by
 * `output_asm_label', or the `%l' specifier that calls it; the intention is
 * that this macro should be set when it is necessary to output a label
 * differently when its address is being taken.  */

/* #define ASM_OUTPUT_LABEL_REF(STREAM, BUF) */


/* A C statement to output to the stdio stream STREAM a label whose name is
 * made from the string PREFIX and the number NUM.
 *
 * It is absolutely essential that these labels be distinct from the labels
 * used for user-level functions and variables.  Otherwise, certain programs
 * will have name conflicts with internal labels.
 *
 * It is desirable to exclude internal labels from the symbol table of the
 * object file.  Most assemblers have a naming convention for labels that
 * should be excluded; on many systems, the letter `L' at the beginning of a
 * label has this effect.  You should find out what convention your system
 * uses, and follow it.  */

#define ASM_OUTPUT_INTERNAL_LABEL(FILE, PREFIX, NUM)		\
  do								\
    {								\
      fprintf (FILE, "%s%u:\n", PREFIX, (unsigned) (NUM));	\
    }								\
  while (0)


/* Optional.  A C statement to output to the stdio stream STREAM a debug info
 * label whose name is made from the string PREFIX and the number NUM.  This is
 * useful for VLIW targets, where debug info labels may need to be treated
 * differently than branch target labels.  On some systems, branch target
 * labels must be at the beginning of instruction bundles, but debug info
 * labels can occur in the middle of instruction bundles.
 *
 * If this macro is not defined, then `ASM_OUTPUT_INTERNAL_LABEL' will be used.
 */

/* #define ASM_OUTPUT_DEBUG_LABEL(STREAM, PREFIX, NUM) */


/* A C statement to store into the string STRING a label whose name is made
 * from the string PREFIX and the number NUM.
 *
 * This string, when output subsequently by `assemble_name', should produce the
 * output that `ASM_OUTPUT_INTERNAL_LABEL' would produce (but without the
 * trailing colon and newline) with the same PREFIX and NUM .
 *
 * If the string begins with `*', then `assemble_name' will output the rest of
 * the string unchanged.  It is often convenient for
 * `ASM_GENERATE_INTERNAL_LABEL' to use `*' in this way.
 */

#define ASM_GENERATE_INTERNAL_LABEL(LABEL, PREFIX, NUM)              \
  do                                                                 \
    {								     \
      sprintf (LABEL, "*%s%u", PREFIX, (unsigned) (NUM));	     \
    }                                                                \
  while (0)


/* A C expression to assign to OUTVAR (which is a variable of type `char *') a
 * newly allocated string made from the string NAME and the number NUMBER, with
 * some suitable punctuation added.  Use `alloca' to get space for the string.
 *
 * The string will be used as an argument to `ASM_OUTPUT_LABELREF' to produce
 * an assembler label for an internal static variable whose name is NAME.
 * Therefore, the string must be such as to result in valid assembler code.
 * The argument NUMBER is different each time this macro is executed; it
 * prevents conflicts between similarly-named internal static variables in
 * different scopes.
 *
 * Ideally this string should not be a valid C identifier, to prevent any
 * conflict with the user's own symbols.  Most assemblers allow periods or
 * percent signs in assembler symbols; putting at least one of these between
 * the name and the number will suffice.  
 *
 * The default definition puts a period at the end of NAME.  */

/* #define ASM_FORMAT_PRIVATE_NAME(OUTVAR, NAME, NUMBER) */


/* The directive to equate a constant value to a symbol.  */

#define SET_ASM_OP  "\t.set\t"


/* A C statement to output to the stdio stream STREAM assembler code which
 * defines (equates) the symbol NAME to have the value VALUE.
 *
 * If `SET_ASM_OP' is defined, a default definition is provided which is
 * correct for most systems.  */

#define ASM_OUTPUT_DEF(STREAM, NAME, VALUE)                          \
  do                                                                 \
    {                                                                \
      assemble_name (STREAM, NAME);				     \
      fprintf ((STREAM), "%s", SET_ASM_OP);			     \
      assemble_name (STREAM, VALUE);				     \
      fprintf (STREAM, "\n");					     \
    }                                                                \
  while (0)


/* A C statement to output to the stdio stream STREAM assembler code which
 * defines (equates) the symbol whose tree node is DECL_OF_NAME to have the
 * value of the tree node DECL_OF_VALUE.  If undefined, `ASM_OUTPUT_DEF' 
 * will be used instead.  */

/* #define ASM_OUTPUT_DEF_FROM_DECLS(STREAM, DECL_OF_NAME, DECL_OF_VALUE) */


/* A C statement to output to the stdio stream STREAM assembler code which
 * defines (equates) the weak symbol NAME to have the value VALUE.  If VALUE is
 * `NULL', it defines NAME as an undefined weak symbol.
 *
 * Define this macro if the target only supports weak aliases; define
 * `ASM_OUTPUT_DEF' instead if possible.  */

/* #define ASM_OUTPUT_WEAK_ALIAS(STREAM, NAME, VALUE) */


/* Define this macro to override the default assembler names used for
 * Objective-C methods.
 *
 * The default name is a unique method number followed by the name of the class
 * (e.g. `_1_Foo').  For methods in categories, the name of the category is
 * also included in the assembler name (e.g.  `_1_Foo_Bar').
 *
 * These names are safe on most systems, but make debugging difficult since the
 * method's selector is not present in the name.  Therefore, particular systems
 * define other ways of computing names.
 *
 * BUF is an expression of type `char *' which gives you a buffer in which to
 * store the name; its length is as long as CLASS_NAME, CAT_NAME and SEL_NAME
 * put together, plus 50 characters extra.
 *
 * The argument IS_INST specifies whether the method is an instance method or a
 * class method; CLASS_NAME is the name of the class; CAT_NAME is the name of
 * the category (or `NULL' if the method is not in a category); and SEL_NAME is
 * the name of the selector.
 *
 * On systems where the assembler can handle quoted names, you can use this
 * macro to provide more human-readable names.  */

/* #define OBJC_GEN_METHOD_LABEL(BUF, IS_INST, CLASS_NAME, CAT_NAME, SEL_NAME) */


/* A C statement (sans semicolon) to output to the stdio stream STREAM commands
 * to declare that the label NAME is an Objective-C class reference.  This is
 * only needed for targets whose linkers have special support for NeXT-style
 * runtimes.  */

/* #define ASM_DECLARE_CLASS_REFERENCE(STREAM, NAME) */


/* A C statement (sans semicolon) to output to the stdio stream STREAM commands
 * to declare that the label NAME is an unresolved Objective-C class reference.
 * This is only needed for targets whose linkers have special support for
 * NeXT-style runtimes.  */

/* #define ASM_DECLARE_UNRESOLVED_REFERENCE(STREAM, NAME) */



/*** subnode: Initialization
 * - General principles of initialization and termination routines.
 * - This is a documentation-only node.  Macros are in the next node.
 *
 * The compiled code for certain languages includes "constructors" (also called
 * "initialization routines")--functions to initialize data in the program when
 * the program is started.  These functions need to be called before the
 * program is "started"--that is to say, before `main' is called.
 *
 * Compiling some languages generates "destructors" (also called "termination
 * routines") that should be called when the program terminates.
 *
 * To make the initialization and termination functions work, the compiler must
 * output something in the assembler code to cause those functions to be called
 * at the appropriate time.  When you port the compiler to a new system, you
 * need to specify how to do this.  
 *
 * For more details on __CTOR_LIST__, __DTOR_LIST__, `crtstuff.c', `libgcc2.c'
 * see info gccint, node "Initialization" and "Macros for Initialization".
 * */



/*** subnode: Macros for Initialization
 * - Specific macros that control the handling of initialization and
 *   termination routines.  */


/* If defined, a C string constant, including spacing, for the assembler
 * operation to identify the following data as initialization code.  If not
 * defined, GCC will assume such a section does not exist.  When you are using
 * special sections for initialization and termination functions, this macro
 * also controls how `crtstuff.c' and `libgcc2.c' arrange to run the
 * initialization functions.
 *
 * If you want to use crtstuff.c, you need to define either
 * `INIT_SECTION_ASM_OP' or `HAS_INIT_SECTION'.
 *
 */

#define INIT_SECTION_ASM_OP  "\t.sect\t\".init\""


/* Similar to `INIT_SECTION_ASM_OP', but for termination code.  If the latter
 * is defined, `crtstuff.c' requires this to be defined, too.  */

#define FINI_SECTION_ASM_OP  "\t.sect\t\".fini\""


/* If defined, `main' will not call `__main' as described above.  This macro
 * should be defined for systems that control start-up code on a
 * symbol-by-symbol basis, such as OSF/1, and should not be defined explicitly
 * for systems that support `INIT_SECTION_ASM_OP'.
 */

/* #define HAS_INIT_SECTION */


/* If defined, a C string constant for a switch that tells the linker that the
 * following symbol is an initialization routine.
 */

/* #define LD_INIT_SWITCH */


/* If defined, a C string constant for a switch that tells the linker that the
 * following symbol is a finalization routine.
 */

/* #define LD_FINI_SWITCH */


/* If defined, a C statement that will write a function that can be
 * automatically called when a shared library is loaded.  The function should
 * call FUNC, which takes no arguments.  If not defined, and the object format
 * requires an explicit initialization function, then a function called
 * `_GLOBAL__DI' will be generated.
 *
 * This function and the following one are used by collect2 when linking a
 * shared library that needs constructors or destructors, or has DWARF2
 * exception tables embedded in the code.  */

/* #define COLLECT_SHARED_INIT_FUNC(STREAM, FUNC) */


/* If defined, a C statement that will write a function that can be
 * automatically called when a shared library is unloaded.  The function should
 * call FUNC, which takes no arguments.  If not defined, and the object format
 * requires an explicit finalization function, then a function called
 * `_GLOBAL__DD' will be generated.
 */

/* #define COLLECT_SHARED_FINI_FUNC(STREAM, FUNC) */


/* If defined, `main' will call `__main' despite the presence of
 * `INIT_SECTION_ASM_OP'.  This macro should be defined for systems where the
 * init section is not actually run automatically, but is still useful for
 * collecting the lists of constructors and destructors.
 */

/* #define INVOKE__main */


/* If nonzero, the C++ `init_priority' attribute is supported and the compiler
 * should emit instructions to control the order of initialization of objects.
 * If zero, the compiler will issue an error message upon encountering an
 * `init_priority' attribute.
 */

/* #define SUPPORTS_INIT_PRIORITY */


/* Target Hook: bool TARGET_HAVE_CTORS_DTORS
 *
 * This value is true if the target supports some "native" method of collecting
 * constructors and destructors to be run at startup and exit.  It is false if
 * we must use `collect2'.
 *
 * The default is true, if `TARGET_ASM_CONSTRUCTOR' and `TARGET_ASM_DESTRUCTOR'
 * are defined.
 *
 * If `TARGET_HAVE_CTORS_DTORS' (targetm.`have_ctors_dtors') is true, the
 * initialization routine generated for the generated object file will have
 * static linkage.
 * */

/* #define TARGET_HAVE_CTORS_DTORS */


/* Target Hook: void TARGET_ASM_CONSTRUCTOR (rtx SYMBOL, int PRIORITY)
 *
 * If defined, a function that outputs assembler code to arrange to call the
 * function referenced by SYMBOL at initialization time.
 *
 * Assume that SYMBOL is a `SYMBOL_REF' for a function taking no arguments and
 * with no return value.  If the target supports initialization priorities,
 * PRIORITY is a value between 0 and `MAX_INIT_PRIORITY'; otherwise it must be
 * `DEFAULT_INIT_PRIORITY'.
 *
 * If this macro is not defined by the target, a suitable default will be
 * chosen if (1) the target supports arbitrary section names, (2) the target
 * defines `CTORS_SECTION_ASM_OP', or (3) `USE_COLLECT2' is not defined.  */

/* #define TARGET_ASM_CONSTRUCTOR */ 


/* Target Hook: void TARGET_ASM_DESTRUCTOR (rtx SYMBOL, int PRIORITY)
 *
 * This is like `TARGET_ASM_CONSTRUCTOR' but used for termination functions
 * rather than initialization functions.  */

/* #define TARGET_ASM_DESTRUCTOR */ 


/* If your system uses `collect2' as the means of processing constructors, then
 * that program normally uses `nm' to scan an object file for constructor
 * functions to be called.
 *
 * On certain kinds of systems, you can define the following macros to make
 * `collect2' work faster (and, in some cases, make it work at all).
 *
 * These macros are effective only in a native compiler; `collect2' as part
 * of a cross compiler always uses `nm' for the target machine. See
 * `#undef OBJECT_FORMAT_COFF' in `collect2.c'.
 * */


/* Define this macro if the system uses COFF (Common Object File Format) object
 * files, so that `collect2' can assume this format and scan object files
 * directly for dynamic constructor/destructor functions.
 */

#define OBJECT_FORMAT_COFF


/* Define this macro if the system uses ROSE format object files, so that
 * `collect2' can assume this format and scan object files directly for dynamic
 * constructor/destructor functions.
 */

/* #define OBJECT_FORMAT_ROSE */


/* Define this macro as a C string constant containing the file name to use to
 * execute `nm'.  The default is to search the path normally for `nm'.
 * A possible definition will be cancelled in collect2.c, if `CROSS_COMPILE'
 * is defined.
 */

/* #define REAL_NM_FILE_NAME */


/* If your system supports shared libraries and has a program to list the
 * dynamic dependencies of a given library or executable, you can define these
 * macros to enable support for running initialization and termination
 * functions in shared libraries:  */


/* Define this macro to a C string constant containing the name of the program
 * which lists dynamic dependencies, like `"ldd"' under SunOS 4.
 */

/* #define LDD_SUFFIX */


/* Define this macro to be C code that extracts filenames from the output of
 * the program denoted by `LDD_SUFFIX'.  PTR is a variable of type `char *'
 * that points to the beginning of a line of output from `LDD_SUFFIX'.  If the
 * line lists a dynamic dependency, the code must advance PTR to the beginning
 * of the filename on that line.  Otherwise, it must set PTR to `NULL'.
 *
 * Need not be defined if `LDD_SUFFIX' is not defined.
 */

/* #define PARSE_LDD_OUTPUT(PTR) */



/*** subnode: Instruction Output
 * - Output of actual assembler instructions.  */


/* Mandatory.  A C initializer containing the assembler's names for the machine
 * registers (ie. from index 0 to FIRST_PSEUDO_REGISTER - 1), each one as a C
 * string constant.  This is what translates register numbers in the compiler
 * into assembler language.  
 *
 * On c6x, always 64 entries here, even though c62x and c67x have only 32 hard
 * regs.  See `CONDITIONAL_REGISTER_USAGE' for how to hide part of the
 * registers from the register allocator (config/alpha/alpha.h has an example).
 */

#define REGISTER_NAMES { \
   "a0",  "a1",  "a2",  "a3",  "a4",  "a5",  "a6",  "a7",  \
   "a8",  "a9", "a10", "a11", "a12", "a13", "a14", "a15",  \
  "a16", "a17", "a18", "a19", "a20", "a21", "a22", "a23",  \
  "a24", "a25", "a26", "a27", "a28", "a29", "a30", "a31",  \
   "b0",  "b1",  "b2",  "b3",  "b4",  "b5",  "b6",  "b7",  \
   "b8",  "b9", "b10", "b11", "b12", "b13", "b14", "b15",  \
  "b16", "b17", "b18", "b19", "b20", "b21", "b22", "b23",  \
  "b24", "b25", "b26", "b27", "b28", "b29", "b30", "b31",  \
}

/* If defined, a C initializer for an array of structures containing a name and
 * a register number.  This macro defines additional names for hard registers,
 * thus allowing the `asm' option in declarations to refer to registers using
 * other names than specified already in `REGISTER_NAMES' (`reg_names').
 */

/* #define ADDITIONAL_REGISTER_NAMES */


/* Optional.  Define this macro if you are using an unusual assembler that
 * requires the names for the machine instructions to be different from
 * the form used internally in the machine description.
 * For details, see info gccint, node "Instruction Output".
 */

/* #define ASM_OUTPUT_OPCODE(STREAM, PTR) */


/* If defined, a C statement to be executed just prior to the output of
 * assembler code for INSN, to modify the extracted operands so they will be
 * output differently.
 *
 * Here the argument OPVEC is the vector containing the operands extracted from
 * INSN, and NOPERANDS is the number of elements of the vector which contain
 * meaningful data for this insn.  The contents of this vector are what will be
 * used to convert the insn template into assembler code, so you can change the
 * assembler output by changing the contents of the vector.
 *
 * This macro is useful when various assembler syntaxes share a single file of
 * instruction patterns; by defining this macro differently, you can cause a
 * large class of instructions to be output differently (such as with
 * rearranged operands).  Naturally, variations in assembler syntax affecting
 * individual insn patterns ought to be handled by writing conditional output
 * routines in those patterns.
 *
 * If this macro is not defined, it is equivalent to a null statement.  */

/* #define FINAL_PRESCAN_INSN(INSN, OPVEC, NOPERANDS) */


/* If defined, `FINAL_PRESCAN_INSN' will be called on each `CODE_LABEL'.  In
 * that case, OPVEC will be a null pointer and NOPERANDS will be zero.
 */

/* #define FINAL_PRESCAN_LABEL */


/* Mandatory.  A C compound statement to output to stdio stream STREAM the
 * assembler syntax for an instruction operand X.  X is an RTL expression.
 *
 * CODE is a value that can be used to specify one of several ways of printing
 * the operand.  It is used when identical operands must be printed differently
 * depending on the context.  CODE comes from the `%' specification that was
 * used to request printing of the operand.  If the specification was just
 * `%DIGIT' then CODE is 0; if the specification was `%LTR DIGIT' then CODE is
 * the ASCII code for LTR.
 *
 * If X is a register, this macro should print the register's name.  The names
 * can be found in an array `reg_names' whose type is `char *[]'.  `reg_names'
 * is initialized from `REGISTER_NAMES'.
 *
 * When the machine description has a specification `%PUNCT' (a `%' followed by
 * a punctuation character), this macro is called with a null pointer for X and
 * the punctuation character for CODE.  See `PRINT_OPERAND_PUNCT_VALID_P'.  */

#define PRINT_OPERAND(FILE, X, CODE)  \
  c6x_print_operand ((FILE), (X), (CODE))


/* Optional.  A C expression which evaluates to true if CODE is a valid
 * punctuation character for use in the `PRINT_OPERAND' macro.  If
 * `PRINT_OPERAND_PUNCT_VALID_P' is not defined, it means that no punctuation
 * characters (except for the standard one, `%') are used in this way.
 */

/* #define PRINT_OPERAND_PUNCT_VALID_P(CODE) */


/* A C compound statement to output to stdio stream STREAM the assembler syntax
 * for an instruction operand that is a memory reference whose address is X.  X
 * is an RTL expression.
 *
 * On some machines, the syntax for a symbolic address depends on the section
 * that the address refers to.  On these machines, define the hook
 * `TARGET_ENCODE_SECTION_INFO' to store the information into the `symbol_ref',
 * and then check for it here.  *Note Assembler Format::.  */

#define PRINT_OPERAND_ADDRESS(STREAM, X) \
  c6x_print_operand_address (STREAM, X)


/* Optional.  A C statement, to be executed after all slot-filler instructions
 * have been output.  For details, see info gccint, node "Instruction Output".
 * */

/* #define DBR_OUTPUT_SEQEND(FILE) */


/* Optional.  If defined, C string expressions to be used for the `%R', `%L',
 * `%U', and `%I' options of `asm_fprintf' (see `final.c').  These are useful
 * when a single `md' file must support multiple assembler formats.  In that
 * case, the various `tm.h' files can define these macros differently.
 */

/* #define REGISTER_PREFIX */

/* #define LOCAL_LABEL_PREFIX */

/* #define USER_LABEL_PREFIX */

/* #define IMMEDIATE_PREFIX */


/* Optional.  If defined this macro should expand to a series of `case'
 * statements which will be parsed inside the `switch' statement of the
 * `asm_fprintf' function.  This allows targets to define extra printf formats
 * which may useful when generating their assembler statements.  Note that
 * upper case letters are reserved for future generic extensions to
 * asm_fprintf, and so are not available to target specific code.  The output
 * file is given by the parameter FILE.  The varargs input pointer is ARGPTR
 * and the rest of the format string, starting the character after the one that
 * is being switched upon, is pointed to by FORMAT.
 */

/* #define ASM_FPRINTF_EXTENSIONS(FILE, ARGPTR, FORMAT) */


/* If your target supports multiple dialects of assembler language (such as
 * different opcodes), define this macro as a C expression that gives the
 * numeric index of the assembler language dialect to use, with zero as the
 * first variant.
 *
 * If this macro is defined, you may use constructs of the form
 * `{option0|option1|option2...}'
 *
 * in the output templates of patterns (*note Output Template::) or in the
 * first argument of `asm_fprintf'.  This construct outputs `option0',
 * `option1', `option2', etc., if the value of `ASSEMBLER_DIALECT' is zero,
 * one, two, etc.  Any special characters within these strings retain their
 * usual meaning.  If there are fewer alternatives within the braces than the
 * value of `ASSEMBLER_DIALECT', the construct outputs nothing.
 *
 * If you do not define this macro, the characters `{', `|' and `}' do not have
 * any special meaning when used in templates or operands to `asm_fprintf'.
 *
 * Define the macros `REGISTER_PREFIX', `LOCAL_LABEL_PREFIX',
 * `USER_LABEL_PREFIX' and `IMMEDIATE_PREFIX' if you can express the variations
 * in assembler language syntax with that mechanism.  Define
 * `ASSEMBLER_DIALECT' and use the `{option0|option1}' syntax if the syntax
 * variant are larger and involve such things as different opcodes or operand
 * order.
 */

/* #define ASSEMBLER_DIALECT */


/* Optional.  A C expression to output to STREAM some assembler code which will
 * push hard register number REGNO onto the stack.  The code need not be
 * optimal, since this macro is used only when profiling.
 */

/* #define ASM_OUTPUT_REG_PUSH(STREAM, REGNO)                           \
    do {								     \
      asm_fprintf ((STREAM), "stw.D1\t%s, *--%s\n",		     \
	  reg_names[(REGNO)], reg_names[STACK_POINTER_REGNUM]);        \
    } while (0)
*/


/* Optional.  A C expression to output to STREAM some assembler code which will
 * pop hard register number REGNO off of the stack.  The code need not be
 * optimal, since this macro is used only when profiling.  */

/* #define ASM_OUTPUT_REG_POP(STREAM, REGNO) */



/*** subnode: Dispatch Tables
 * - Output of jump/dispatch tables.  */


/* A C statement to output to the stdio stream STREAM an assembler
 * pseudo-instruction to generate a difference between two labels.  VALUE and
 * REL are the numbers of two internal labels.  The definitions of these labels
 * are output using `ASM_OUTPUT_INTERNAL_LABEL', and they must be printed in
 * the same way here.  For example,
 *
 *        fprintf (STREAM, "\t.word L%d-L%d\n",
 *                 VALUE, REL)
 *
 * You must provide this macro on machines where the addresses in a dispatch
 * table are relative to the table's own address.  If defined, GCC will also
 * use this macro on all machines when producing PIC (`flag_pic').  BODY is the
 * body of the `ADDR_DIFF_VEC'; it is provided so that the mode and flags can
 * be read.  */

/* #define ASM_OUTPUT_ADDR_DIFF_ELT(STREAM, BODY, VALUE, REL) */


/* This macro should be provided on machines where the addresses in a dispatch
 * table are absolute.
 *
 * The default definition provided in gcc/defaults.h seems to be buggy in that
 * it passes "POINTER_SIZE / UNITS_PER_WORD" to `integer_asm_op', but should
 * rather pass "POINTER_SIZE / BITS_PER_UNIT"
 */

#define ASM_OUTPUT_ADDR_VEC_ELT(STREAM, VALUE)                              \
  do								            \
    {                                                                       \
      fputs (integer_asm_op (POINTER_SIZE / BITS_PER_UNIT, TRUE), STREAM);  \
      fprintf (STREAM, "L%d\n", (VALUE));                                   \
    }                                                                       \
  while (0)


/* Define this if the label before a jump-table needs to be output specially.
 * The first three arguments are the same as for `ASM_OUTPUT_INTERNAL_LABEL';
 * the fourth argument is the jump-table which follows (a `jump_insn'
 * containing an `addr_vec' or `addr_diff_vec').
 *
 * This feature is used on system V to output a `swbeg' statement for the
 * table.
 *
 * If this macro is not defined, these labels are output with
 * `ASM_OUTPUT_INTERNAL_LABEL'.
 */

/* #define ASM_OUTPUT_CASE_LABEL(STREAM, PREFIX, NUM, TABLE) */


/* Optional.  Define this if something special must be output at the end of a
 * jump-table.  The definition should be a C statement to be executed after the
 * assembler code for the table is written.  It should write the appropriate
 * code to stdio stream STREAM.  The argument TABLE is the jump-table insn, and
 * NUM is the label-number of the preceding label.  */

/* #define ASM_OUTPUT_CASE_END(STREAM, NUM, TABLE) */



/*** subnode: Exception Region Output
 * - This describes commands marking the start and the end of an exception
 *   region.  */


/* Optional.  If defined, a C string constant for the name of the section
 * containing exception handling frame unwind information.  If not defined, GCC
 * will provide a default definition if the target supports named sections.
 * `crtstuff.c' uses this macro to switch to the appropriate section.
 *
 * You should define this symbol if your target supports DWARF 2 frame unwind
 * information and the default definition does not work.  */

/* #define EH_FRAME_SECTION_NAME */


/* If defined, DWARF 2 frame unwind information will be placed in the data
 * section even though the target supports named sections.  This might be
 * necessary, for instance, if the system linker does garbage collection and
 * sections cannot be marked as not to be collected.
 *
 * Do not define this macro unless `TARGET_ASM_NAMED_SECTION' is also defined.
 * */

/* #define EH_FRAME_IN_DATA_SECTION */


/* An rtx used to mask the return address found via `RETURN_ADDR_RTX', so that
 * it does not contain any extraneous set bits in it.
 */

/* #define MASK_RETURN_ADDR */


/* Optional.  Define this macro to 0 if your target supports DWARF 2 frame
 * unwind information, but it does not yet work with exception handling.
 * Otherwise, if your target supports this information (if it defines
 * `INCOMING_RETURN_ADDR_RTX' and either `UNALIGNED_INT_ASM_OP' or
 * `OBJECT_FORMAT_ELF'), GCC will provide a default definition of 1.
 *
 * If this macro is defined to 1, the DWARF 2 unwinder will be the default
 * exception handling mechanism; otherwise, `setjmp'/`longjmp' will be used by
 * default.
 *
 * If this macro is defined to anything, the DWARF 2 unwinder will be used
 * instead of inline unwinders and `__unwind_function' in the non-`setjmp'
 * case.  */

/* #define DWARF2_UNWIND_INFO */


/* This macro need only be defined if the target might save registers in the
 * function prologue at an offset to the stack pointer that is not aligned to
 * `UNITS_PER_WORD'.  The definition should be the negative minimum alignment
 * if `STACK_GROWS_DOWNWARD' is defined, and the positive minimum alignment
 * otherwise.  *Note SDB and DWARF::.  Only applicable if the target supports
 * DWARF 2 frame unwind information.
 */

/* #define DWARF_CIE_DATA_ALIGNMENT */


/* Target Hook: void TARGET_ASM_EXCEPTION_SECTION ()
 *
 * If defined, a function that switches to the section in which the main
 * exception table is to be placed (*note Sections::).  The default is a
 * function that switches to a section named `.gcc_except_table' on machines
 * that support named sections via `TARGET_ASM_NAMED_SECTION', otherwise if
 * `-fpic' or `-fPIC' is in effect, the `data_section', otherwise the
 * `readonly_data_section'.  */

/* #define TARGET_ASM_EXCEPTION_SECTION */


/* Target Hook: void TARGET_ASM_EH_FRAME_SECTION ()
 *
 * If defined, a function that switches to the section in which the DWARF 2
 * frame unwind information to be placed (*note Sections::).  The default is a
 * function that outputs a standard GAS section directive, if
 * `EH_FRAME_SECTION_NAME' is defined, or else a data section directive
 * followed by a synthetic label.  */

/* #define TARGET_ASM_EH_FRAME_SECTION */


/* Variable: Target Hook bool TARGET_TERMINATE_DW2_EH_FRAME_INFO
 *
 * Contains the value true if the target should add a zero word onto the end of
 * a Dwarf-2 frame info section when used for exception handling.  Default
 * value is false if `EH_FRAME_SECTION_NAME' is defined, and true otherwise.
 */

/* #define TARGET_TERMINATE_DW2_EH_FRAME_INFO */



/*** subnode: Alignment Output
 * - Pseudo ops for alignment and skipping data.  */


/* Optional.  The alignment (log base 2) to put in front of LABEL, which is a
 * common destination of jumps and has no fallthru incoming edge.
 *
 * This macro need not be defined if you don't want any special alignment to be
 * done at such a time.  Most machine descriptions do not currently define the
 * macro.
 *
 * Unless it's necessary to inspect the LABEL parameter, it is better to set
 * the variable ALIGN_JUMPS in the target's `OVERRIDE_OPTIONS'.  Otherwise, you
 * should try to honor the user's selection in ALIGN_JUMPS in a `JUMP_ALIGN'
 * implementation.  */

/* #define JUMP_ALIGN(LABEL) */


/* The alignment (log base 2) to put in front of LABEL, which follows a
 * `BARRIER'.
 *
 * This macro need not be defined if you don't want any special alignment to be
 * done at such a time.  Most machine descriptions do not currently define the
 * macro.  */

/* #define LABEL_ALIGN_AFTER_BARRIER(LABEL) */


/* The maximum number of bytes to skip when applying
 * `LABEL_ALIGN_AFTER_BARRIER'.  This works only if `ASM_OUTPUT_MAX_SKIP_ALIGN'
 * is defined.
 */

/* #define LABEL_ALIGN_AFTER_BARRIER_MAX_SKIP */


/* The alignment (log base 2) to put in front of LABEL, which follows a
 * `NOTE_INSN_LOOP_BEG' note.
 *
 * This macro need not be defined if you don't want any special alignment to be
 * done at such a time.  Most machine descriptions do not currently define the
 * macro.
 *
 * Unless it's necessary to inspect the LABEL parameter, it is better to set
 * the variable `align_loops' in the target's `OVERRIDE_OPTIONS'.  Otherwise,
 * you should try to honor the user's selection in `align_loops' in a
 * `LOOP_ALIGN' implementation.  */

/* #define LOOP_ALIGN(LABEL) */


/* The maximum number of bytes to skip when applying `LOOP_ALIGN'.  This works
 * only if `ASM_OUTPUT_MAX_SKIP_ALIGN' is defined.
 */

/* #define LOOP_ALIGN_MAX_SKIP */


/* The alignment (log base 2) to put in front of LABEL.  If
 * `LABEL_ALIGN_AFTER_BARRIER' / `LOOP_ALIGN' specify a different alignment,
 * the maximum of the specified values is used.
 *
 * Unless it's necessary to inspect the LABEL parameter, it is better to set
 * the variable `align_labels' in the target's `OVERRIDE_OPTIONS'.  Otherwise,
 * you should try to honor the user's selection in `align_labels' in a
 * `LABEL_ALIGN' implementation.  */

/* #define LABEL_ALIGN(LABEL) */


/* The maximum number of bytes to skip when applying `LABEL_ALIGN'.  This works
 * only if `ASM_OUTPUT_MAX_SKIP_ALIGN' is defined.
 */

/* #define LABEL_ALIGN_MAX_SKIP */


/* A C statement to output to the stdio stream STREAM an assembler instruction
 * to advance the location counter by NBYTES bytes.  Those bytes should be zero
 * when loaded.  NBYTES will be a C expression of type `int'.
 *
 * On c6x, use the ".space" directive, see spru186i.pdf, section 4.10
 * "Directives reference", p. 4-63
 */

#define ASM_OUTPUT_SKIP(STREAM, NBYTES)  \
  do                                                                 \
    {                                                                \
      fprintf (STREAM, "\t.space %u\n", (NBYTES));		     \
    }                                                                \
  while (0)
      


/* Define this macro if `ASM_OUTPUT_SKIP' should not be used in the text
 * section because it fails to put zeros in the bytes that are skipped.  This
 * is true on many Unix systems, where the pseudo-op to skip bytes produces
 * no-op instructions rather than zeros when used in the text section.
 */

/* #define ASM_NO_SKIP_IN_TEXT */


/* A C statement to output to the stdio stream STREAM an assembler command to
 * advance the location counter to a multiple of 2 to the POWER bytes.  POWER
 * will be a C expression of type `int'.
 */

#define ASM_OUTPUT_ALIGN(STREAM, POWER)	\
  do								     \
    {								     \
      if (POWER)						     \
	fprintf ((STREAM), "\t.align %d\n", 1 << (POWER));	     \
    }								     \
  while (0)


/* Like `ASM_OUTPUT_ALIGN', except that the "nop" instruction is used for
 * padding, if necessary.
 */

/* #define ASM_OUTPUT_ALIGN_WITH_NOP(STREAM, POWER) */


/* A C statement to output to the stdio stream STREAM an assembler command to
 * advance the location counter to a multiple of 2 to the POWER bytes, but only
 * if MAX_SKIP or fewer bytes are needed to satisfy the alignment request.
 * POWER and MAX_SKIP will be a C expression of type `int'.
 */

/* #define ASM_OUTPUT_MAX_SKIP_ALIGN(STREAM, POWER, MAX_SKIP) */



/**** node: Debugging Info
 *
 * - Defining the format of debugging output.  */



/*** subnode: All Debuggers
 * - Macros that affect all debugging formats uniformly.  */


/* A C expression that returns the DBX register number for the compiler
 * register number REGNO.  In the default macro provided, the value of this
 * expression will be REGNO itself.  But sometimes there are some registers
 * that the compiler knows about and DBX does not, or vice versa.  In such
 * cases, some register may need to have one number in the compiler and another
 * for DBX.
 *
 * If two registers have consecutive numbers inside GCC, and they can be used
 * as a pair to hold a multiword value, then they _must_ have consecutive
 * numbers after renumbering with `DBX_REGISTER_NUMBER'.  Otherwise, debuggers
 * will be unable to access such a pair, because they expect register pairs to
 * be consecutive in their own numbering scheme.
 *
 * If you find yourself defining `DBX_REGISTER_NUMBER' in way that does not
 * preserve register pairs, then what you must do instead is redefine the
 * actual register numbering scheme.  */

/* #define DBX_REGISTER_NUMBER(REGNO) */


/* A C expression that returns the integer offset value for an automatic
 * variable having address X (an RTL expression).  The default computation
 * assumes that X is based on the frame-pointer and gives the offset from the
 * frame-pointer.  This is required for targets that produce debugging output
 * for DBX or COFF-style debugging output for SDB and allow the frame-pointer
 * to be eliminated when the `-g' options is used.  */

/* #define DEBUGGER_AUTO_OFFSET(X) */


/* A C expression that returns the integer offset value for an argument having
 * address X (an RTL expression).  The nominal offset is OFFSET.  */

/* #define DEBUGGER_ARG_OFFSET(OFFSET, X) */


/* A C expression that returns the type of debugging output GCC should produce
 * when the user specifies just `-g'.  Define this if you have arranged for GCC
 * to support more than one format of debugging output.  Currently, the
 * allowable values are `DBX_DEBUG', `SDB_DEBUG', `DWARF_DEBUG',
 * `DWARF2_DEBUG', `XCOFF_DEBUG', `VMS_DEBUG', and `VMS_AND_DWARF2_DEBUG'.
 *
 * When the user specifies `-ggdb', GCC normally also uses the value of this
 * macro to select the debugging output format, but with two exceptions.  If
 * `DWARF2_DEBUGGING_INFO' is defined and `LINKER_DOES_NOT_WORK_WITH_DWARF2' is
 * not defined, GCC uses the value `DWARF2_DEBUG'.  Otherwise, if
 * `DBX_DEBUGGING_INFO' is defined, GCC uses `DBX_DEBUG'.
 *
 * The value of this macro only affects the default debugging output; the user
 * can always get a specific type of output by using `-gstabs', `-gcoff',
 * `-gdwarf-1', `-gdwarf-2', `-gxcoff', or `-gvms'.  */

/* #define PREFERRED_DEBUGGING_TYPE */



/*** subnode: DBX Options
 * - These are specific options for DBX output.  */
   

/* Define this macro if GCC should produce debugging output for DBX in response
 * to the `-g' option.  */

/* #define DBX_DEBUGGING_INFO */


/* Define this macro if GCC should produce XCOFF format debugging output in
 * response to the `-g' option.  This is a variant of DBX format.  */

/* #define XCOFF_DEBUGGING_INFO */


/* Optional.  Define this macro to control whether GCC should by default
 * generate GDB's extended version of DBX debugging information (assuming
 * DBX-format debugging information is enabled at all).  Default is 1.
 */

/* #define DEFAULT_GDB_EXTENSIONS */


/* Define this macro if all `.stabs' commands should be output while in the
 * text section.  Currently this doesn't seem to be used anywhere.  */

/* #define DEBUG_SYMS_TEXT */


/* A C string constant, including spacing, naming the assembler pseudo op to
 * use instead of `"\t.stabs\t"' to define an ordinary debugging symbol.  If
 * you don't define this macro, `"\t.stabs\t"' is used.  This macro applies
 * only to DBX debugging information format.  */

/* #define ASM_STABS_OP */


/* A C string constant, including spacing, naming the assembler pseudo op to
 * use instead of `"\t.stabd\t"' to define a debugging symbol whose value is
 * the current location.  If you don't define this macro, `"\t.stabd\t"' is
 * used.  This macro applies only to DBX debugging information format.  */

/* #define ASM_STABD_OP */


/* A C string constant, including spacing, naming the assembler pseudo op to
 * use instead of `"\t.stabn\t"' to define a debugging symbol with no name.  If
 * you don't define this macro, `"\t.stabn\t"' is used.  This macro applies
 * only to DBX debugging information format.  */

/* #define ASM_STABN_OP */


/* Define this macro if DBX on your system does not support the construct
 * `xsTAGNAME'.  On some systems, this construct is used to describe a forward
 * reference to a structure named TAGNAME.  On other systems, this construct is
 * not supported at all.  */

/* #define DBX_NO_XREFS */


/* A symbol name in DBX-format debugging information is normally continued
 * (split into two separate `.stabs' directives) when it exceeds a certain
 * length (by default, 80 characters).  On some operating systems, DBX requires
 * this splitting; on others, splitting must not be done.  You can inhibit
 * splitting by defining this macro with the value zero.  You can override the
 * default splitting-length by defining this macro as an expression for the
 * length you desire.  */

/* #define DBX_CONTIN_LENGTH */


/* Normally continuation is indicated by adding a `\' character to the end of a
 * `.stabs' string when a continuation follows.  To use a different character
 * instead, define this macro as a character constant for the character you
 * want to use.  Do not define this macro if backslash is correct for your
 * system.  */

/* #define DBX_CONTIN_CHAR */


/* Define this macro if it is necessary to go to the data section before
 * outputting the `.stabs' pseudo-op for a non-global static variable.  */

/* #define DBX_STATIC_STAB_DATA_SECTION */


/* The value to use in the "code" field of the `.stabs' directive for a
 * typedef.  The default is `N_LSYM'.  */

/* #define DBX_TYPE_DECL_STABS_CODE */


/* The value to use in the "code" field of the `.stabs' directive for a static
 * variable located in the text section.  DBX format does not provide any
 * "right" way to do this.  The default is `N_FUN'.  */

/* #define DBX_STATIC_CONST_VAR_CODE */


/* The value to use in the "code" field of the `.stabs' directive for a
 * parameter passed in registers.  DBX format does not provide any "right" way
 * to do this.  The default is `N_RSYM'.  */

/* #define DBX_REGPARM_STABS_CODE */


/* The letter to use in DBX symbol data to identify a symbol as a parameter
 * passed in registers.  DBX format does not customarily provide any way to do
 * this.  The default is `'P''.  */

/* #define DBX_REGPARM_STABS_LETTER */


/* The letter to use in DBX symbol data to identify a symbol as a stack
 * parameter.  The default is `'p''.  */

/* #define DBX_MEMPARM_STABS_LETTER */


/* Define this macro if the DBX information for a function and its arguments
 * should precede the assembler code for the function.  Normally, in DBX
 * format, the debugging information entirely follows the assembler code.  */

/* #define DBX_FUNCTION_FIRST */


/* Define this macro if the `N_LBRAC' symbol for a block should precede the
 * debugging information for variables and functions defined in that block.
 * Normally, in DBX format, the `N_LBRAC' symbol comes first.  */

/* #define DBX_LBRAC_FIRST */


/* Define this macro if the value of a symbol describing the scope of a block
 * (`N_LBRAC' or `N_RBRAC') should be relative to the start of the enclosing
 * function.  Normally, GCC uses an absolute address.  */

/* #define DBX_BLOCKS_FUNCTION_RELATIVE */


/* Define this macro if GCC should generate `N_BINCL' and `N_EINCL' stabs for
 * included header files, as on Sun systems.  This macro also directs GCC to
 * output a type number as a pair of a file number and a type number within the
 * file.  Normally, GCC does not generate `N_BINCL' or `N_EINCL' stabs, and it
 * outputs a single number for a type number.  */

/* #define DBX_USE_BINCL */



/*** subnode: DBX Hooks
 * - These are hooks for DBX format.  */


/* Define this macro to say how to output to STREAM the debugging information
 * for the start of a scope level for variable names.  The argument NAME is the
 * name of an assembler symbol (for use with `assemble_name') whose value is
 * the address where the scope begins.  */

/* #define DBX_OUTPUT_LBRAC(STREAM, NAME) */


/* Like `DBX_OUTPUT_LBRAC', but for the end of a scope level.    */

/* #define DBX_OUTPUT_RBRAC(STREAM, NAME) */


/* Define this macro if the target machine requires special handling to output
 * an `N_FUN' entry for the function DECL.  */

/* #define DBX_OUTPUT_NFUN(STREAM, LSCOPE_LABEL, DECL) */


/* Define this macro if the target machine requires special handling to output
 * an enumeration type.  The definition should be a C statement (sans
 * semicolon) to output the appropriate information to STREAM for the type
 * TYPE.  */

/* #define DBX_OUTPUT_ENUM(STREAM, TYPE) */


/* Define this macro if the target machine requires special output at the end
 * of the debugging information for a function.  The definition should be a C
 * statement (sans semicolon) to output the appropriate information to STREAM.
 * FUNCTION is the `FUNCTION_DECL' node for the function.  */

/* #define DBX_OUTPUT_FUNCTION_END(STREAM, FUNCTION) */


/* Define this macro if you need to control the order of output of the standard
 * data types at the beginning of compilation.  The argument SYMS is a `tree'
 * which is a chain of all the predefined global symbols, including names of
 * data types.
 *
 * Normally, DBX output starts with definitions of the types for integers and
 * characters, followed by all the other predefined types of the particular
 * language in no particular order.
 *
 * On some machines, it is necessary to output different particular types
 * first.  To do this, define `DBX_OUTPUT_STANDARD_TYPES' to output those
 * symbols in the necessary order.  For details, see info gccint, node "DBX
 * Hooks".  */

/* #define DBX_OUTPUT_STANDARD_TYPES(SYMS) */


/* Some stabs encapsulation formats (in particular ECOFF), cannot handle the
 * `.stabs "",N_FUN,,0,0,Lscope-function-1' gdb dbx extension construct.  On
 * those machines, define this macro to turn this feature off without
 * disturbing the rest of the gdb extensions.  */

/* #define NO_DBX_FUNCTION_END */



/*** subnode: File Names and DBX
 * - Macros controlling output of file names in DBX format.  */


/* Define this if DBX wants to have the current directory recorded in each
 * object file.
 *
 * Note that the working directory is always recorded if GDB extensions are
 * enabled.  */

/* #define DBX_WORKING_DIRECTORY */


/* A C statement to output DBX debugging information to the stdio stream STREAM
 * which indicates that file NAME is the main source file--the file specified
 * as the input file for compilation.  This macro is called only once, at the
 * beginning of compilation.
 *
 * This macro need not be defined if the standard form of output for DBX
 * debugging information is appropriate.  */

/* #define DBX_OUTPUT_MAIN_SOURCE_FILENAME(STREAM, NAME) */


/* A C statement to output DBX debugging information to the stdio stream STREAM
 * which indicates that the current directory during compilation is named NAME.
 *
 * This macro need not be defined if the standard form of output for DBX
 * debugging information is appropriate.  */

/* #define DBX_OUTPUT_MAIN_SOURCE_DIRECTORY(STREAM, NAME) */


/* A C statement to output DBX debugging information at the end of compilation
 * of the main source file NAME.
 *
 * If you don't define this macro, nothing special is output at the end of
 * compilation, which is correct for most machines.  */

/* #define DBX_OUTPUT_MAIN_SOURCE_FILE_END(STREAM, NAME) */


/* A C statement to output DBX debugging information to the stdio stream STREAM
 * which indicates that file NAME is the current source file.  This output is
 * generated each time input shifts to a different source file as a result of
 * `#include', the end of an included file, or a `#line' command.
 *
 * This macro need not be defined if the standard form of output for DBX
 * debugging information is appropriate.  */

/* #define DBX_OUTPUT_SOURCE_FILENAME(STREAM, NAME) */



/*** subnode: SDB and DWARF
 * - Macros for SDB (COFF) and DWARF formats.  */


/* Define this macro if GCC should produce COFF-style debugging output for SDB
 * in response to the `-g' option.  */

/* #define SDB_DEBUGGING_INFO */


/* Define this macro if GCC should produce dwarf format debugging output in
 * response to the `-g' option.  */

/* #define DWARF_DEBUGGING_INFO */


/* Define this macro if GCC should produce dwarf version 2 format debugging
 * output in response to the `-g' option.
 *
 * To support optional call frame debugging information, you must also define
 * `INCOMING_RETURN_ADDR_RTX' and either set `RTX_FRAME_RELATED_P' on the
 * prologue insns if you use RTL for the prologue, or call `dwarf2out_def_cfa'
 * and `dwarf2out_reg_save' as appropriate from `TARGET_ASM_FUNCTION_PROLOGUE'
 * if you don't.  */

/* #define DWARF2_DEBUGGING_INFO */


/* Define this macro to a nonzero value if GCC should always output Dwarf 2
 * frame information.  If `DWARF2_UNWIND_INFO' (*note Exception Region Output::
 * is nonzero, GCC will output this information not matter how you define
 * `DWARF2_FRAME_INFO'.    */

/* #define DWARF2_FRAME_INFO */


/* Define this macro if the linker does not work with Dwarf version 2.
 * Normally, if the user specifies only `-ggdb' GCC will use Dwarf version 2 if
 * available; this macro disables this.  See the description of the
 * `PREFERRED_DEBUGGING_TYPE' macro for more details.
 *
 * Seems neither used nor defined anywhere.  */

/* #define LINKER_DOES_NOT_WORK_WITH_DWARF2  */


/* By default, the Dwarf 2 debugging information generator will generate a
 * label to mark the beginning of the text section.  If it is better simply to
 * use the name of the text section itself, rather than an explicit label, to
 * indicate the beginning of the text section, define this macro to zero.  */

/* #define DWARF2_GENERATE_TEXT_SECTION_LABEL */


/* Define this macro to be a nonzero value if the assembler can generate Dwarf
 * 2 line debug info sections.  This will result in much more compact line
 * number tables, and hence is desirable if it works.  */

/* #define DWARF2_ASM_LINE_DEBUG_INFO */


/* A C statement to issue assembly directives that create a difference between
 * the two given labels, using an integer of the given size.  */

/* #define ASM_OUTPUT_DWARF_DELTA(STREAM, SIZE, LABEL1, LABEL2) */


/* A C statement to issue assembly directives that create a section-relative
 * reference to the given label, using an integer of the given size.  */

/* #define ASM_OUTPUT_DWARF_OFFSET(STREAM, SIZE, LABEL) */


/* A C statement to issue assembly directives that create a self-relative
 * reference to the given label, using an integer of the given size.  */

/* ASM_OUTPUT_DWARF_PCREL(STREAM, SIZE, LABEL) */



/* PUT_SDB_...  Define these macros to override the assembler syntax for the
 * special SDB assembler directives.  Default definitions are provided in
 * `sdbout.c'.  */

/* #define PUT_SDB_SCL(a) */
/* #define PUT_SDB_INT_VAL(a) */
/* #define PUT_SDB_VAL(a) */
/* #define PUT_SDB_DEF(a) */
/* #define PUT_SDB_PLAIN_DEF(a) */
/* #define PUT_SDB_ENDEF */
/* #define PUT_SDB_TYPE(a) */
/* #define PUT_SDB_SIZE(a) */
/* #define PUT_SDB_START_DIM */
/* #define PUT_SDB_NEXT_DIM(a) */
/* #define PUT_SDB_LAST_DIM(a) */
/* #define PUT_SDB_TAG(a) */
/* #define PUT_SDB_BLOCK_START(LINE) */
/* #define PUT_SDB_BLOCK_END(LINE) */
/* #define PUT_SDB_FUNCTION_START(LINE) */
/* #define PUT_SDB_FUNCTION_END(LINE) */
/* #define PUT_SDB_SRC_FILE(FILENAME) */


/* Some assemblers do not support a semicolon as a delimiter, even between SDB
 * assembler directives.  In that case, define this macro to be the delimiter
 * to use (usually `\n').  It is not necessary to define a new set of
 * `PUT_SDB_OP' macros if this is the only change required.  */

/* #define SDB_DELIM */


/* Define this macro to override the usual method of constructing a dummy name
 * for anonymous structure and union types.  See `sdbout.c' for more
 * information.  */

/* #define SDB_GENERATE_FAKE */


/* Define this macro to allow references to unknown structure, union, or
 * enumeration tags to be emitted.  Standard COFF does not allow handling of
 * unknown references, MIPS ECOFF has support for it.  */

/* #define SDB_ALLOW_UNKNOWN_REFERENCES */


/* Define this macro to allow references to structure, union, or enumeration
 * tags that have not yet been seen to be handled.  Some assemblers choke if
 * forward tags are used, while some require it.  */

/* #define SDB_ALLOW_FORWARD_REFERENCES */



/**** node: VMS Debug
 *
 * - Here are macros for VMS debug format.  */

/* Define this macro if GCC should produce debugging output for VMS in response
 * to the `-g' option.  The default behavior for VMS is to generate minimal
 * debug info for a traceback in the absence of `-g' unless explicitly
 * overridden with `-g0'.  This behavior is controlled by
 * `OPTIMIZATION_OPTIONS' and `OVERRIDE_OPTIONS'.  */

/* #define VMS_DEBUGGING_INFO */



/**** node: Floating Point
 *
 * While all modern machines use twos-complement representation for integers,
 * there are a variety of representations for floating point numbers.  This
 * means that in a cross-compiler the representation of floating point numbers
 * in the compiled program may be different from that used in the machine doing
 * the compilation.
 *
 * Because different representation systems may offer different amounts of
 * range and precision, all floating point constants must be represented in the
 * target machine's format.  Therefore, the cross compiler cannot safely use
 * the host machine's floating point arithmetic; it must emulate the target's
 * arithmetic.  To ensure consistency, GCC always uses emulation to work with
 * floating point values, even when the host and target floating point formats
 * are identical.
 *
 * The following macros are provided by `real.h' for the compiler to use.  All
 * parts of the compiler which generate or optimize floating-point calculations
 * must use these macros.  They may evaluate their operands more than once, so
 * operands must not have side effects.  */

/* See info gccint, node "Floating Point" for descriptions of these macros.
 *
 * `REAL_VALUE_TYPE'
 * `REAL_VALUES_EQUAL'
 * `REAL_VALUES_LESS'
 * `REAL_VALUE_FIX'
 * `REAL_VALUE_UNSIGNED_FIX'
 * `REAL_VALUE_ATOF'
 * `REAL_VALUE_NEGATIVE'
 * `REAL_VALUE_ISINF'
 * `REAL_VALUE_ISNAN'
 * `REAL_ARITHMETIC'
 * `REAL_VALUE_NEGATE'
 * `REAL_VALUE_ABS'
 * `REAL_VALUE_TRUNCATE'
 * `REAL_VALUE_TO_INT'
 * `REAL_VALUE_FROM_INT'
 */


/**** node: Mode Switching
 *
 * - The following macros control mode switching optimizations.
 * - if `OPTIMIZE_MODE_SWITCHING' is left undefined, all other macros in this
 *   section need not be defined either.  */
   

/* Define this macro if the port needs extra instructions inserted for mode
 * switching in an optimizing compilation.
 *
 * For an example, the SH4 can perform both single and double precision
 * floating point operations, but to perform a single precision operation, the
 * FPSCR PR bit has to be cleared, while for a double precision operation, this
 * bit has to be set.  Changing the PR bit requires a general purpose register
 * as a scratch register, hence these FPSCR sets have to be inserted before
 * reload, i.e. you can't put this into instruction emitting or
 * `MACHINE_DEPENDENT_REORG'.
 *
 * You can have multiple entities that are mode-switched, and select at run
 * time which entities actually need it.  `OPTIMIZE_MODE_SWITCHING' should
 * return nonzero for any ENTITY that needs mode-switching.  If you define this
 * macro, you also have to define `NUM_MODES_FOR_MODE_SWITCHING',
 * `MODE_NEEDED', `MODE_PRIORITY_TO_MODE' and `EMIT_MODE_SET'.  `NORMAL_MODE'
 * is optional.  */

/* #define OPTIMIZE_MODE_SWITCHING(ENTITY) */


/* If you define `OPTIMIZE_MODE_SWITCHING', you have to define this as
 * initializer for an array of integers.  Each initializer element N refers to
 * an entity that needs mode switching, and specifies the number of different
 * modes that might need to be set for this entity.  The position of the
 * initializer in the initializer - starting counting at zero - determines the
 * integer that is used to refer to the mode-switched entity in question.  In
 * macros that take mode arguments / yield a mode result, modes are represented
 * as numbers 0 ... N - 1.  N is used to specify that no mode switch is needed
 * / supplied.  */

/* #define NUM_MODES_FOR_MODE_SWITCHING */


/* ENTITY is an integer specifying a mode-switched entity.  If
 * `OPTIMIZE_MODE_SWITCHING' is defined, you must define this macro to return
 * an integer value not larger than the corresponding element in
 * `NUM_MODES_FOR_MODE_SWITCHING', to denote the mode that ENTITY must be
 * switched into prior to the execution of INSN.  */

/* #define MODE_NEEDED(ENTITY, INSN) */


/* If this macro is defined, it is evaluated for every ENTITY that needs mode
 * switching.  It should evaluate to an integer, which is a mode that ENTITY is
 * assumed to be switched to at function entry and exit.  */

/* #define NORMAL_MODE(ENTITY) */


/* This macro specifies the order in which modes for ENTITY are processed.  0
 * is the highest priority, `NUM_MODES_FOR_MODE_SWITCHING[ENTITY] - 1' the
 * lowest.  The value of the macro should be an integer designating a mode for
 * ENTITY.  For any fixed ENTITY, `mode_priority_to_mode' (ENTITY, N) shall be
 * a bijection in 0 ...  `num_modes_for_mode_switching[ENTITY] - 1'.  */

/* #define MODE_PRIORITY_TO_MODE(ENTITY, N) */


/* Generate one or more insns to set ENTITY to MODE.  HARD_REG_LIVE is the set
 * of hard registers live at the point where the insn(s) are to be inserted.  */

/* #define EMIT_MODE_SET(ENTITY, MODE, HARD_REGS_LIVE) */



/**** node: Target Attributes
 *
 * - Defining target-specific uses of `__attribute__'  
 * - Target-specific attributes may be defined for functions, data and types.
 *   These are described using the following target hooks; they also need to be
 *   documented in `extend.texi'.  */
   

/* Target Hook: const struct attribute_spec * TARGET_ATTRIBUTE_TABLE
 *
 * If defined, this target hook points to an array of `struct attribute_spec'
 * (defined in `tree.h') specifying the machine specific attributes for this
 * target and some of the restrictions on the entities to which these
 * attributes are applied and the arguments they take.  The last element must
 * be a NULL-element, as for example in `ix86_attribute_table'.  */

/* Target Hook: int TARGET_COMP_TYPE_ATTRIBUTES (tree TYPE1, tree TYPE2)
 *
 * If defined, this target hook is a function which returns zero if the
 * attributes on TYPE1 and TYPE2 are incompatible, one if they are compatible,
 * and two if they are nearly compatible (which causes a warning to be
 * generated).  If this is not defined, machine-specific attributes are
 * supposed always to be compatible.  */

/* Target Hook: void TARGET_SET_DEFAULT_TYPE_ATTRIBUTES (tree TYPE)
 *
 * If defined, this target hook is a function which assigns default attributes
 * to newly defined TYPE.  */

/* Target Hook: tree TARGET_MERGE_TYPE_ATTRIBUTES (tree TYPE1, tree
 *        TYPE2)
 *
 * Define this target hook if the merging of type attributes needs special
 * handling.  If defined, the result is a list of the combined
 * `TYPE_ATTRIBUTES' of TYPE1 and TYPE2.  It is assumed that `comptypes' has
 * already been called and returned 1.  This function may call
 * `merge_attributes' to handle machine-independent merging.  */

/* Target Hook: tree TARGET_MERGE_DECL_ATTRIBUTES (tree OLDDECL, tree
 *        NEWDECL)
 *
 * Define this target hook if the merging of decl attributes needs special
 * handling.  If defined, the result is a list of the combined
 * `DECL_ATTRIBUTES' of OLDDECL and NEWDECL.  NEWDECL is a duplicate
 * declaration of OLDDECL.  Examples of when this is needed are when one
 * attribute overrides another, or when an attribute is nullified by a
 * subsequent definition.  This function may call `merge_attributes' to handle
 * machine-independent merging.
 *
 * If the only target-specific handling you require is `dllimport' for Windows
 * targets, you should define the macro `TARGET_DLLIMPORT_DECL_ATTRIBUTES'.
 * This links in a function called `merge_dllimport_decl_attributes' which can
 * then be defined as the expansion of `TARGET_MERGE_DECL_ATTRIBUTES'.  This is
 * done in `i386/cygwin.h' and `i386/i386.c', for example.  */

/* Target Hook: void TARGET_INSERT_ATTRIBUTES (tree NODE, tree
 *        *ATTR_PTR)
 *
 * Define this target hook if you want to be able to add attributes to a decl
 * when it is being created.  This is normally useful for back ends which wish
 * to implement a pragma by using the attributes which correspond to the
 * pragma's effect.  The NODE argument is the decl which is being created.  The
 * ATTR_PTR argument is a pointer to the attribute list for this decl.  The
 * list itself should not be modified, since it may be shared with other decls,
 * but attributes may be chained on the head of the list and `*ATTR_PTR'
 * modified to point to the new attributes, or a copy of the list may be made
 * if further changes are needed.  */

/* Target Hook: bool TARGET_FUNCTION_ATTRIBUTE_INLINABLE_P (tree FNDECL)
 *
 * This target hook returns `true' if it is ok to inline FNDECL into the
 * current function, despite its having target-specific attributes, `false'
 * otherwise.  By default, if a function has a target specific attribute
 * attached to it, it will not be inlined.  */



/**** node: MIPS Coprocessors
 *
 * - Defining coprocessor specifics for MIPS targets.
 * - See info gccint, node "MIPS Coprocessors" for discussion of the mechanism.
 */

/* A comma-separated list (with leading comma) of pairs describing the
 * alternate names of coprocessor registers.  The format of each entry should
 * be { ALTERNATENAME, REGISTER_NUMBER}.  Default: empty.  */

/* #define ALL_COP_ADDITIONAL_REGISTER_NAMES */



/**** node: Misc
 *
 * - Everything else.  */


/*
 * Helps generate a more compact decision tree for RTL operator matching with
 * targets that define their own predicate functions (info gccint, node "RTL
 * Template").  Can be left undefined without consequences apart from
 * compiling performance.  */

/* example:
 * #define PREDICATE_CODES \
 *   {"gen_reg_rtx_operand", {SUBREG, REG}},  \
 *   {"reg_or_short_cint_operand", {SUBREG, REG, CONST_INT}},
 */


/* Genrecog will warn about certain forms of `match_operand' without a mode; if
 * the operand predicate is listed in `SPECIAL_MODE_PREDICATES', the warning
 * will be suppressed.  Can be left undefined.  */

/* example (i386):
 * #define SPECIAL_MODE_PREDICATES \
 *   "ext_register_operand",
 */


/* Must be defined to the name of the machine mode that elements of a
 * jump-table should have. TODO: Check that Pmode is indeed ok for c6x.  */

#define CASE_VECTOR_MODE  Pmode


/* Enables `shorten_branches' to deal with `addr_diff_vec'.  Optional.  */

/* #define CASE_VECTOR_SHORTEN_MODE(MIN_OFFSET, MAX_OFFSET, BODY) ...  */


/* Define this macro to be a nonzero C expression to indicate when jump-tables
 * should contain relative addresses.  Default is zero, so no rel addrs.  */

/* #define CASE_VECTOR_PC_RELATIVE ...  */


/* Only define this if the case instruction should drop through after the table
   when the index is out of range (currently only on the `sh').  This means the
   specified default-label is actually ignored by the `case' insn proper.
   Don't define it if the case insn should jump to the default label instead.  */

/* #define CASE_DROPS_THROUGH */


/* The smallest number of different values for which it is best to use a
 * jump-table instead of a tree of conditional branches.  The default is 4
 * for machines with a `casesi' instruction and 5 otherwise.  */

/* #define CASE_VALUES_THRESHOLD */

/* Define this macro if operations between registers with integral mode
 * smaller than a word are always performed on the entire register.
 * Most RISC machines have this property and most CISC machines do not.
 * ??? If unsure, IMO it's better not to define this, even though it 
 * prevents some optimizations in `combine.c'.
 * TODO: For c6x, it might be ok to define this macro. Take a look at MVK,
 * MVKH, ADD(U) and other commands to find out.  */

/* #define WORD_REGISTER_OPERATIONS  */

/* Return `SIGN_EXTEND' if  bytes or half words are always sign-extended when
 * `load'ed from memory into a register. Likewise for `ZERO_EXTEND'.
 * For c6x, there are the LDH and LDB commands (sign extention), but also
 * LDH(U) and LDB(U) (unsigned, hence zero extention). So leave this macro
 * undefined as no constistent definition is possible.  */

/* #define LOAD_EXTEND_OP(MODE) `SIGN_EXTEND' */


/* Define this macro if loading short immediate values into registers sign
 * extends. For c6x, this is true -- see `MVK' command.  */

#define SHORT_IMMEDIATES_SIGN_EXTEND


 /* Define this macro if the same instructions that convert a floating point
  * number to a signed fixed point number also convert validly to an unsigned
  * one.  For c6x, no support for floating point as yet, so leave undefined.  */

/* #define FIXUNS_TRUNC_LIKE_FIX_TRUNC */


/* The maximum number of bytes that a single instruction can move quickly
 * between memory and registers or between two memory locations.
 * For c6x, this should be 4, using commands `LDW', `STW'.  */

#define MOVE_MAX  4


/* A constant value equal to the largest value that `MOVE_MAX' can have at
 * run-time.  Default is `MOVE_MAX' if undefined.  */

/* #define MAX_MOVE_MAX */

/* When this macro is nonzero, the compiler will assume that it is safe to omit
 * a sign-extend, zero-extend, and certain bitwise `and' instructions that
 * truncates the count of a shift operation. For c6x, be conservative for
 * the moment and assume, it is not safe.  */

/* #define SHIFT_COUNT_TRUNCATED 1 */


/* A C expression which is nonzero if on this machine it is safe to "convert"
 * an integer of INPREC bits to one of OUTPREC bits (where OUTPREC is smaller
 * than INPREC) by merely operating on it as if it had only OUTPREC bits.  */

#define TRULY_NOOP_TRUNCATION(OUTPREC, INPREC)  1


/* A C expression describing the value returned by a comparison operator with
 * an integral mode and stored by a store-flag instruction (`sCOND') when the
 * condition is true.  For c6x, see commands `CMPGT', `CMPEQ', and `CMPLT'.  */

#define STORE_FLAG_VALUE  1


/* A C expression that gives a nonzero `REAL_VALUE_TYPE' value that is returned
 * when comparison operators with floating-point results are true.  Define this
 * macro on machine that have comparison operations that return floating-point
 * values.  If there are no such operations, do not define this macro.  */

/* #define FLOAT_STORE_FLAG_VALUE(MODE) */


/* Specify the machine mode that pointers have.  After generation of rtl, the
 * compiler makes no further distinction between pointers and any other objects
 * of this machine mode.  */

#define Pmode  SImode


/* An alias for the machine mode used for memory references to functions being
 * called, in `call' RTL expressions.  On most machines this should be
 * `QImode'.  */

#define FUNCTION_MODE  QImode


/* A C expression for the maximum number of instructions above which the
 * function DECL should not be inlined.  DECL is a `FUNCTION_DECL' node.  The
 * default definition of this macro is 64 plus 8 times the number of arguments
 * that the function accepts.  Some people think a larger threshold should be
 * used on RISC machines. For c6x, we go for the default.  */

/* #define INTEGRATE_THRESHOLD(DECL) */


/* In normal operation, the preprocessor expands `__STDC__' to the constant 1,
 * to signify that GCC conforms to ISO Standard C.  On some hosts, like
 * Solaris, the system compiler uses a different convention, where `__STDC__'
 * is normally 0, but is 1 if the user specifies strict conformance to the C
 * Standard.  Defining `STDC_0_IN_SYSTEM_HEADERS' makes GNU CPP follows the
 * host convention when processing system header files, but when processing
 * user files `__STDC__' will always expand to 1.  */

/* #define STDC_0_IN_SYSTEM_HEADERS */


/* Define this macro if the system header files support C++ as well as C.  This
 * macro inhibits the usual method of using system header files in C++, which
 * is to pretend that the file's contents are enclosed in `extern "C" {...}'.
 * */

/* #define NO_IMPLICIT_EXTERN_C */


/* This macro is no longer supported.  You must use `REGISTER_TARGET_PRAGMAS'
 * instead.  */

/* #define HANDLE_PRAGMA(GETC, UNGETC, NAME) */


/* Define this macro if you want to implement any target-specific pragmas.  For
 * details, see info gccint.  For c6x, we don't need this.  */

/* #define REGISTER_TARGET_PRAGMAS(PFILE) */


/* Define this macro (to a value of 1) if you want the System V style pragmas
 * `#pragma pack(<n>)' and `#pragma weak <name> [=<value>]' to be supported by
 * gcc.  For details, see info gccint.  */

/* #define HANDLE_SYSV_PRAGMA */
     

/* Define this macro (to a value of 1) if you want to support the Win32 style
 * pragmas `#pragma pack(push,N)' and `#pragma pack(pop)'.  For details, see
 * info gccint.  */

/* #define HANDLE_PRAGMA_PACK_PUSH_POP */
     

/* Define this macro to control use of the character `$' in identifier names.
 * 0 means `$' is not allowed by default; 1 means it is allowed.  1 is the
 * default; there is no need to define this macro in that case.  This macro
 * controls the compiler proper; it does not affect the preprocessor.  For c6x,
 * we go for the default.  */

/* #define DOLLARS_IN_IDENTIFIERS */


/* Define this macro if the assembler does not accept the character `$' in
 * label names.  By default constructors and destructors in G++ have `$' in the
 * identifiers.  If this macro is defined, `.' is used instead.  */

/* #define NO_DOLLAR_IN_LABEL */


/* Define this macro if the assembler does not accept the character `.' in
 * label names.  By default constructors and destructors in G++ have names that
 * use `.'.  If this macro is defined, these names are rewritten to avoid `.'.
 * */ 

/* #define NO_DOT_IN_LABEL */


/* Define this macro if the target system expects every program's `main'
 * function to return a standard "success" value by default (if no other value
 * is explicitly returned).  The definition should be a C statement (sans
 * semicolon) to generate the appropriate rtl instructions.  It is used only
 * when compiling the end of `main'.  */ 

/* #define DEFAULT_MAIN_RETURN */


/* Define this if the target system lacks the function `atexit' from the ISO C
 * standard.  If this macro is defined, a default definition will be provided
 * to support C++.  If `ON_EXIT' is not defined, a default `exit' function will
 * also be provided.  */ 

/* #define NEED_ATEXIT */


/* Define this macro if the target has another way to implement atexit
 * functionality without replacing `exit'.  For instance, SunOS 4 has a similar
 * `on_exit' library function.  The definition should be a functional macro
 * which can be used just like the `atexit' function.  */ 

/* #define ON_EXIT */


/* Define this if your `exit' function needs to do something besides calling an
 * external function `_cleanup' before terminating with `_exit'.  The
 * `EXIT_BODY' macro is only needed if `NEED_ATEXIT' is defined and `ON_EXIT'
 * is not defined.  */ 

/* #define EXIT_BODY */


/* If defined, a C expression that is nonzero if it is safe for the delay slot
 * scheduler to place instructions in the delay slot of INSN, even if they
 * appear to use a resource set or clobbered in INSN.  INSN is always a
 * `jump_insn' or an `insn'; GCC knows that every `call_insn' has this
 * behavior.  On machines where some `insn' or `jump_insn' is really a function
 * call and hence has this behavior, you should define this macro.  For c6x:
 * this is an optimization which we can consider at a later time.  */ 

/* #define INSN_SETS_ARE_DELAYED(INSN) */


/* If defined, a C expression that is nonzero if it is safe for the delay slot
 * scheduler to place instructions in the delay slot of INSN, even if they
 * appear to set or clobber a resource referenced in INSN.  INSN is always a
 * `jump_insn' or an `insn'.  On machines where some `insn' or `jump_insn' is
 * really a function call and its operands are registers whose use is actually
 * in the subroutine it calls, you should define this macro.  Doing so allows
 * the delay slot scheduler to move instructions which copy arguments into the
 * argument registers into the delay slot of INSN.  TODO: For c6x: This is an
 * optimization which we can consider at a later time.  */ 

/* #define INSN_REFERENCES_ARE_DELAYED(INSN) */


/* In rare cases, correct code generation requires extra machine dependent
 * processing between the second jump optimization pass and delayed branch
 * scheduling.  On those machines, define this macro as a C statement to act on
 * the code starting at INSN.  TODO: For c6x: evaluate later, if we need this.
 * For now, scheduling should be off, so we shouldn't need it.  */ 

/* #define MACHINE_DEPENDENT_REORG(INSN) */


/* Define this macro if in some cases global symbols from one translation unit
 * may not be bound to undefined symbols in another translation unit without
 * user intervention.  For instance, under Microsoft Windows symbols must be
 * explicitly imported from shared libraries (DLLs). TODO: For c6x: Do we need
 * this?  */ 

/* #define MULTIPLE_SYMBOL_SPACES */


/* A C statement that adds to CLOBBERS `STRING_CST' trees for any hard regs the
 * port wishes to automatically clobber for all asms.  (An example would be the
 * flags register on i386.) For c6x: We don't have such registers, so leave
 * undefined.   */

/* #define MD_ASM_CLOBBERS(CLOBBERS) */


/* Define this to the largest integer machine mode which can be used for
 * operations other than load, store and copy operations.  You need only define
 * this macro if the target holds values larger than `word_mode' in general
 * purpose registers.  Most targets should not define this macro.  Doesn't
 * seem to be well supported by GCC at present.  No target currently defines
 * this, so for c6x, we don't either.  */ 

/* #define MAX_INTEGER_COMPUTATION_MODE */


/* Define this macro as a C string constant for the linker argument to link in
 * the system math library, or `""' if the target does not have a separate math
 * library.  You need only define this macro if the default of `"-lm"' is
 * wrong.  */ 

/* #define MATH_LIBRARY */


/* Define this macro as a C string constant for the environment variable that
 * specifies where the linker should look for libraries.  You need only define
 * this macro if the default of `"LIBRARY_PATH"' is wrong.  */ 

/* #define LIBRARY_PATH_ENV */


/* Define this macro if the target supports file locking with fcntl / F_SETLKW.
 * Note that this functionality is part of POSIX.  Defining
 * `TARGET_HAS_F_SETLKW' will enable the test coverage code to use file locking
 * when exiting a program, which avoids race conditions if the program has
 * forked.  For c6x, we cannot enable this until we have an operating
 * system running.  */ 

/* #define TARGET_HAS_F_SETLKW */


/* A C expression for the maximum number of instructions to execute via
 * conditional execution instructions instead of a branch.  A value of
 * `BRANCH_COST'+1 is the default if the machine does not use cc0, and 1 if it
 * does use cc0.  For c6x, we go for the default at present.  */ 

/* #define MAX_CONDITIONAL_EXECUTE */


/* Used if the target needs to perform machine-dependent modifications on the
 * conditionals used for turning basic blocks into conditionally executed code.
 * CE_INFO points to a data structure, `struct ce_if_block', which contains
 * information about the currently processed blocks.  TRUE_EXPR and FALSE_EXPR
 * are the tests that are used for converting the then-block and the
 * else-block, respectively.  Set either TRUE_EXPR or FALSE_EXPR to a null
 * pointer if the tests cannot be converted.
 *
 * For c6x: This is not relevant until after we have defined patterns that
 * recognize (cond ...) insn patterns for conditional execution.  These patterns
 * cause genconfig.c to generate a definition for `HAVE_conditional_execution',
 * which is one of the conditions for calling `cond_exec_process_if_block'
 * (in `process_if_block'), which uses this macro exclusively.
 * TODO: define this and related macros below, if necessary.  */

/* #define IFCVT_MODIFY_TESTS(CE_INFO, TRUE_EXPR, FALSE_EXPR) */


/* Like `IFCVT_MODIFY_TESTS', but used when converting more complicated
 * if-statements into conditions combined by `and' and `or' operations.  BB
 * contains the basic block that contains the test that is currently being
 * processed and about to be turned into a condition.  */ 

/* #define IFCVT_MODIFY_MULTIPLE_TESTS(CE_INFO, BB, TRUE_EXPR, FALSE_EXPR) */


/* A C expression to modify the PATTERN of an INSN that is to be converted to
 * conditional execution format.  CE_INFO points to a data structure, `struct
 * ce_if_block', which contains information about the currently processed
 * blocks. `cond_exec_process_insns' is the only function to use this macro and
 * it does accept it to be undefined.   */

/* #define IFCVT_MODIFY_INSN(CE_INFO, PATTERN, INSN) */


/* A C expression to perform any final machine dependent modifications in
 * converting code to conditional execution.  The involved basic blocks can be
 * found in the `struct ce_if_block' structure that is pointed to by CE_INFO.
 * `cond_exec_process_insns' is the only function to use this macro and it does
 * accept it to be undefined.  */ 

/* #define IFCVT_MODIFY_FINAL(CE_INFO) */


/* A C expression to cancel any machine dependent modifications in converting
 * code to conditional execution.  The involved basic blocks can be found in
 * the `struct ce_if_block' structure that is pointed to by CE_INFO.
 * `cond_exec_process_insns' is the only function to use this macro and it does
 * accept it to be undefined.  */ 

/* #define IFCVT_MODIFY_CANCEL(CE_INFO) */


/* A C expression to initialize any extra fields in a `struct ce_if_block'
 * structure, which are defined by the `IFCVT_EXTRA_FIELDS' macro.
 * `find_if_header' is the only function to use this macro and it does accept
 * it to be undefined.   */

/* #define IFCVT_INIT_EXTRA_FIELDS(CE_INFO) */


/* If defined, it should expand to a set of field declarations that will be
 * added to the `struct ce_if_block' structure.  These should be initialized by
 * the `IFCVT_INIT_EXTRA_FIELDS' macro.  */ 

/* #define IFCVT_EXTRA_FIELDS */


/* Define this target hook if you have any machine-specific built-in functions
 * that need to be defined.  It should be a function that performs the
 * necessary setup.  For details, see info gccint, node "Misc".
 * Machine specific built-in functions can be useful to expand special machine
 * instructions that would otherwise not normally be generated because they
 * have no equivalent in the source language (for example, SIMD vector
 * instructions or prefetch instructions).
 * */ 

/* #define TARGET_INIT_BUILTINS  */

     
/* Target Hook: rtx TARGET_EXPAND_BUILTIN (tree EXP, rtx TARGET, rtx SUBTARGET,
 *                      enum machine_mode MODE, int IGNORE) 
 * Expand a call to a machine specific built-in function that was set up by
 * `TARGET_INIT_BUILTINS'.  EXP is the expression for the function call; the
 * result should go to TARGET if that is convenient, and have mode MODE if that
 * is convenient.  SUBTARGET may be used as the target for computing one of
 * EXP's operands.  IGNORE is nonzero if the value is to be ignored.  This
 * function should return the result of the call to the built-in function.  */

/* #define TARGET_EXPAND_BUILTIN */


/* Take a branch insn in BRANCH1 and another in BRANCH2.  Return true if
 * redirecting BRANCH1 to the destination of BRANCH2 is possible.  On some
 * targets, branches may have a limited range.  Optimizing the filling of delay
 * slots can result in branches being redirected, and this may in turn cause a
 * branch offset to overflow.  For c6x, we currently don't care about this.  */ 

/* #define MD_CAN_REDIRECT_BRANCH(BRANCH1, BRANCH2) */


/* When the initial value of a hard register has been copied in a pseudo
 * register, it is often not necessary to actually allocate another register to
 * this pseudo register, because the original hard register or a stack slot it
 * has been saved into can be used.  For details, see info gccint, node Misc.
 * This optimization is currently only defined for the `sh'. For c6x, we leave
 * it undefined as for now.  */ 

/* #define ALLOCATE_INITIAL_VALUE(HARD_REG) */


/* Define this macro to be a C string representing the suffix for object files
 * on your target machine.  If you do not define this macro, GCC will use `.o'
 * as the suffix for object files.  */ 

/* #define TARGET_OBJECT_SUFFIX */


/* Define this macro to be a C string representing the suffix to be
 * automatically added to executable files on your target machine.  If you do
 * not define this macro, GCC will use the null string as the suffix for
 * executable files.  */ 

/* #define TARGET_EXECUTABLE_SUFFIX */


/* If defined, `collect2' will scan the individual object files specified on
 * its command line and create an export list for the linker.  Define this
 * macro for systems like AIX, where the linker discards object files that are
 * not referenced from `main' and uses export lists. Currently only defined on
 * `AIX'.  */ 

/* #define COLLECT_EXPORT_LIST */


/* Define this macro to a C expression representing a variant of the method
 * call MDECL, if Java Native Interface (JNI) methods must be invoked
 * differently from other methods on your target.  Currently only defined in
 * i386 for cygwin.  */ 

/* #define MODIFY_JNI_METHOD_CALL(MDECL) */


/* Target Hook: bool TARGET_CANNOT_MODIFY_JUMPS_P (void)
 * This target hook returns `true' past the point in which new jump
 * instructions could be created.  Currently only defined on the `sh'.  For
 * details, see info gccint, node Misc.  */

/* #define TARGET_CANNOT_MODIFY_JUMPS_P */


/**** END of definitions  */

