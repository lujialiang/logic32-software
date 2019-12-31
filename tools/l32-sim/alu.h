/*-
 * Copyright (c) 2017 Lu JiaLiang
 * Email: 1056568029@qq.com
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id$
 */
#ifndef _ALU_H_INCLUDE_
#define _ALU_H_INCLUDE_

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

#define ALU_BITS 32

typedef enum {
	alu_op_add,
	alu_op_adc,
	alu_op_sub,
	alu_op_sbc,
	alu_op_neg,
	alu_op_and,
	alu_op_or,
	alu_op_not,
	alu_op_xor,
	alu_op_lsl,
	alu_op_asl,
	alu_op_lsr,
	alu_op_asr,
	alu_op_rol,
	alu_op_ror,
	alu_op_mul,
	alu_op_div,
	alu_op_mod,
	alu_op_mulu,
	alu_op_divu,
	alu_op_modu,
} alu_op_t;

extern uint32_t alu(uint32_t a, uint32_t b, alu_op_t op);
extern uint32_t alu_add(uint32_t a, uint32_t b, bool cin);
extern uint32_t alu_sub(uint32_t a, uint32_t b, bool cin);
extern uint32_t alu_logic(uint32_t a, uint32_t b, alu_op_t op);
extern uint32_t alu_shift(uint32_t a, alu_op_t op, uint8_t);
extern uint32_t alu_multiplier(uint32_t a, uint32_t b, alu_op_t op);

#ifdef __cplusplus
}
#endif

#endif // _ALU_H_INCLUDE_
