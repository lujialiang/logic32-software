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
#include "fpu.h"

uint32_t fpu_single(uint32_t a, uint32_t b, int op)
{
	fu_t ua, ub, uc;
	ua.u = a;
	ub.u = b;
	switch(op) {
		case fpu_op_add_s:
			uc.f = ua.f + ub.f;
			return uc.u;
		case fpu_op_sub_s:
			uc.f = ua.f - ub.f;
			return uc.u;
		case fpu_op_mul_s:
			uc.f = ua.f * ub.f;
			return uc.u;
		case fpu_op_div_s:
			uc.f = ua.f / ub.f;
			return uc.u;
		default:
			return 0;
	}
	return 0;
}

uint64_t fpu_double(uint64_t a, uint64_t b, int op)
{
	du_t ua, ub, uc;
	ua.u = a;
	ub.u = b;
	switch(op) {
		case fpu_op_add_d:
			uc.d = ua.d + ub.d;
			return uc.u;
		case fpu_op_sub_d:
			uc.d = ua.d - ub.d;
			return uc.u;
		case fpu_op_mul_d:
			uc.d = ua.d * ub.d;
			return uc.u;
		case fpu_op_div_d:
			uc.d = ua.d / ub.d;
			return uc.u;
		default:
			return 0;
	}
	return 0;
}
