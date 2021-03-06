/*
 * sim-write.S -- write interface for frv simulator
 * 
 * Copyright (c) 2002 Red Hat, Inc
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions. No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 */

#include <frv-asm.h>

#include "syscall.h"

/*
 * Input:
 * gr8	  -- File descriptor.
 * gr9	  -- String to be printed.
 * gr10	  -- Length of the string.
 *
 * Output:
 * gr8	  -- Length written or -1.
 * errno  -- Set if an error
 */

	.globl	EXT(_write)
	.type	EXT(_write),@function
	.weak	EXT(write)
	.text
EXT(_write):
EXT(write):
	setlos	#SYS_write,gr7
	tira	gr0,#0
	ret
.Lend:
	.size	EXT(_write),.Lend-EXT(_write)
