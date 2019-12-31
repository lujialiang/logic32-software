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
