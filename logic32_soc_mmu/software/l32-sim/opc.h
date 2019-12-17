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
#ifndef _OPC_H_INCLUE_
#define _OPC_H_INCLUE_

enum {
	op_add,
	op_adc,
	op_sub,
	op_sbc,
	op_and,
	op_orr,
	op_xor,
	op_lsl,
	op_lsr,
	op_asr,
	op_addi,
	op_andi,
	op_ori,
	op_xori,
	op_lsli,
	op_lsri,
	op_asri,
	op_ldih,
	op_brn,
	op_ldw,
	op_stw,
	op_ldb,
	op_stb,
	op_ajmp,
	op_jmpr,
	op_jal,		// LJL 20171013
	op_jalr,	// LJL 20171013
	op_misc_reti,
	op_misc_trap,
	op_misc_halt,
	op_misc_enai,
	op_misc_disi,
	op_misc_mtc,
	op_misc_mfc,
	op_misc_not,	// LJL 20170503
	op_mul,
	op_div,
	op_mod,
	op_ldh,
	op_sth,
	op_ldbu,
	op_ldhu,
	op_mulu,
	op_divu,
	op_modu,
	op_misc_lev,
	op_misc_lev2,
	op_misc_tlb_mr_w,
	op_misc_tlb_mr_r,
	op_misc_tlb_tr_w,
	op_misc_tlb_tr_r,
	op_fpu_adds,
	op_fpu_subs,
	op_fpu_muls,
	op_fpu_divs,
	op_fpu_addd,
	op_fpu_subd,
	op_fpu_muld,
	op_fpu_divd,
	op_fpu_movs,
	op_fpu_movd,
	op_fpu_misc_mfc1,
	op_fpu_misc_mtc1,
	op_fpu_cvdf,
	op_fpu_cvfd,
	op_fpu_cvwd,
	op_fpu_cvdw,
	op_ld_d,
	op_ld_f,
	op_st_d,
	op_st_f,
	op_fcmp_eq_d,
	op_fcmp_le_d,
	op_fcmp_lt_d,
	op_fcmp_eq_s,
	op_fcmp_le_s,
	op_fcmp_lt_s,
	op_fpu_bc1f,
	op_fpu_bc1t,
	op_fpu_misc_mfc1_d,
	op_fpu_misc_mtc1_d,
	op_fpu_negd,
	op_fpu_negs,
	op_undefined = -1,
};

#endif
