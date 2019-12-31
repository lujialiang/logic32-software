#ifndef _SIM_H_
#define _SIM_H_

typedef enum {
	op_addr,
	op_addi,
	op_adcr,
	op_adci,
	op_subr,
	op_subi,
	op_sbcr,
	op_sbci,
	op_andr,
	op_andi,
	op_borr,
	op_bori,
	op_xorr,
	op_xori,
	op_mskr,
	op_mski,
	op_shl,
	op_shr,
	op_rol,
	op_ror,
	op_ldm,
	op_stm,
	op_inp,
	op_out,
	op_bz,
	op_bnz,
	op_bc,
	op_bnc,
	op_jmp,
	op_jsb,
	op_ret,
	op_reti,
	op_enai,
	op_disi,
	op_wait,
	op_stby,
	op_nop,
} op_t;

#endif // _SIM_H_