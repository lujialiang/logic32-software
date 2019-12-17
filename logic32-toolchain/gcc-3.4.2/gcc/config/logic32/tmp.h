
#if 0
/* Definitions for register eliminations.

   This is an array of structures.  Each structure initializes one pair
   of eliminable registers.  The "from" register number is given first,
   followed by "to".  Eliminations of the same "from" register are listed
   in order of preference.

   We have two registers that are eliminated on the logic32. The pseudo arg
   pointer and pseudo frame pointer registers can always be eliminated;
   they are replaced with either the stack or the real frame pointer.  */

#define ELIMINABLE_REGS				\
{						\
  {ARG_POINTER_REGNUM,	 STACK_POINTER_REGNUM},	\
  {ARG_POINTER_REGNUM,	 FRAME_POINTER_REGNUM},	\
  {FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM}	\
}
#endif

#if 0
/* Given FROM and TO register numbers, say whether this elimination is allowed.
   Frame pointer elimination is automatically handled.

   All other eliminations are valid.  */
#define CAN_ELIMINATE(FROM, TO)						\
 ((TO) == FRAME_POINTER_REGNUM || !frame_pointer_needed)
#endif

#if 0	/* LJL */
/* Define the offset between two registers, one to be eliminated, and the other
   its replacement, at the start of a routine.  */
#define INITIAL_ELIMINATION_OFFSET(from_reg,to_reg,offset) {offset=logic32_initial_elimination_offset(from_reg, to_reg);}
#endif

#if 0
/* Register in which static-chain is passed to a function.  */
#if 0
#define STATIC_CHAIN_REGNUM	        SOFT_Z_REGNUM
#endif
#endif

/* A C expression that is nonzero if hard register number REGNO2 can be
   considered for use as a rename register for REGNO1 */
#if 0
#define HARD_REGNO_RENAME_OK(REGNO1,REGNO2) \
  logic32_hard_regno_rename_ok ((REGNO1), (REGNO2))
#endif

#if 0
/* Forward type declaration for prototypes definitions.
   rtx_ptr is equivalent to rtx. Can't use the same name.  */
struct rtx_def;
typedef struct rtx_def *rtx_ptr;

union tree_node;
typedef union tree_node *tree_ptr;

/* We can't declare enum machine_mode forward nor include 'machmode.h' here.
   Prototypes defined here will use an int instead. It's better than no
   prototype at all.  */
typedef int enum_machine_mode;
#endif

