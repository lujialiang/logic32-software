/* Prototype definitions for the c6x target.

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

/* This file contains prototypes for the functions in `config/c6x/c6x.c'.
 * `gcc/config.gcc' (sourced by `gcc/configure') sets `tm_p_file' to
 * `${cpu_type}/${cpu_type}-protos.h' by default.  For c6x, that would be
 * `c6x/c6x-protos.h'.  `gcc/Makefile.in' uses the value in `tm_p_file' and
 * passes it to `mkconfig.sh', which creates `tm_p.h' and outputs a
 * corresponding `#include' line to it.  `tm_p.h', finally is included by
 * various GCC modules, some of which might require the function prototypes
 * when containing instances of target macros that expand to calls to functions
 * in `c6x.c'.  */

/* Only include the following functions, if RTX_CODE is defined (see `rtl.h').
 * The `rtx' and `enum rtx_code' types are not needed and not available in
 * GCC modules that don't include `rtl.h', so omit the affected prototypes
 * in that case.  */
#ifdef RTX_CODE
extern int c6x_comparison_operator PARAMS((register rtx, enum machine_mode));
extern void c6x_expand_branch PARAMS((enum rtx_code, rtx));
extern rtx c6x_expand_compare PARAMS((enum rtx_code, enum machine_mode));
extern int c6x_legitimate_address_p PARAMS((enum machine_mode, rtx, int));
extern void c6x_print_operand PARAMS((FILE *, rtx, int));
extern int c6x_call_operand PARAMS((rtx, enum machine_mode));
extern void c6x_emit_push PARAMS((rtx, enum machine_mode));
extern void c6x_emit_pop PARAMS((rtx, enum machine_mode));
extern void c6x_expand_call PARAMS((rtx, rtx));
extern void c6x_print_operand_address PARAMS((FILE *, rtx));
extern rtx c6x_legitimize_address PARAMS((register rtx, register rtx, enum machine_mode));
extern void c6x_expand_prologue PARAMS((void));
extern void c6x_expand_epilogue PARAMS((int));
#else /* ! defined RTX_CODE */
/* Put prototypes using neither `rtx' nor `enum rtx_code' here.  */

#endif /* RTX_CODE */

