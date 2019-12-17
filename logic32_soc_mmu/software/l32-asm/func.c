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

#include "asm.h"
#include "isa.h"
#include "keyword_hash.h"

extern int const_expression(pnode_t p);

static inst_t inst;

void out_code(unsigned int n);
unsigned int bit_field(int h, int l, unsigned int v);

extern int inst_size;

static int alu_common(uint32_t opc_immed)
{
	struct nlist *sp;
	memset(&inst,0,sizeof(inst_t));
	if( next_token() != sym_gpr) {
		error("expert a register");
		return -1;
	}
	inst.rd = token.value;
	if( next_token() != sym_comma ) {
		error("expert ','");
		return -1;
	}
	if( next_token() != sym_gpr) {
		error("expert a register");
		return -1;
	}
	inst.rs = token.value;
	if( next_token() != sym_comma ) {
		error("expert ','");
		return -1;
	}
	if( next_token() == sym_gpr ) {
		inst.rs2 = token.value;
		return 0;
	}
	// immed
	else if( token.type == sym_num ) {
		inst.immed_flag = 1;
		inst.immed = token.value;
		return 0;
	}
	// minus immed
	else if( token.type == sym_minus ) {
		next_token();
		if( token.type == sym_num ) {
			inst.immed_flag = 1;
			inst.immed = -token.value;
			return 0;
		}
		else {
			error("expect a number");
			return -1;
		}
	}
	else if( token.type == sym_name ) {
		inst.immed_flag = 1;
		sp = lookup();
		if( sp == NULL ) {
			error("undefined symbol '%s'",token_buf);
			return -1;
		}
		if( (sp->n_type & ~N_EXT) != N_ABS ) {
			error("symbol '%s' must be a const symbol", sp->n_name);
			return -1;
		}
		inst.immed = sp->n_value;
		return 0;
	}
	else {
		error("syntax error");
		return -1;
	}
}

static int shift_common(uint32_t opc_alur, uint32_t opc_immed)
{
	unsigned int v;
	if( next_token() != sym_gpr ) {
		error("expert a register");
		return -1;
	}
	inst.rd = token.value;
	if( next_token() != sym_comma ) {
		error("expert ','");
		return -1;
	}
	if( next_token() != sym_gpr ) {
		error("expert a register");
		return -1;
	}
	inst.rs = token.value;
	if( next_token() != sym_comma ) {
		error("expert ','");
		return -1;
	}
	if( next_token() == sym_num ) {
		inst.immed = token.value;
		if( inst.immed < 0 || inst.immed > 31 ) {
			error("immediate number(=%d, =0x%x) overflow, must in range 0 to 31",\
				inst.immed, inst.immed);
			return -1;
		}
		if( pass == 2 ) {
			v = bit_field(31,26,opc_immed);	// op
			v |= bit_field(25,21,inst.rd);	// rd
			v |= bit_field(20,16,inst.rs);	// rs
			v |= bit_field(15,0,inst.immed);// immed
			out_code(v);
		}
		return 0;
	}
	else if( token.type == sym_gpr ) {
		if( pass == 2 ) {
			inst.rs2 = token.value;
			v = bit_field(31,26, 0x0);	// op
			v |= bit_field(25,21,inst.rd);	// rd
			v |= bit_field(20,16,inst.rs);	// rs
			v |= bit_field(15,11,inst.rs2);	// rs2
			v |= bit_field(3,0,opc_alur);	// register
			out_code(v);
		}
		return 0;
	}
	else {
		error("syntax error");
		return -1;
	}
}

static int branch_common(int func)
{
	struct nlist *sp;
	unsigned int v;
	next_token();
	if( token.type == sym_num ) {
		if( pass == 2 ) {
			v = bit_field(31,26,OP_BRN);		// op
			v |= bit_field(25,22,func);			// func
			v |= bit_field(21,0,token.value);	// disp
			out_code(v);
		}
		return 0;
	}
	else if( token.type == sym_name ) {
		if( pass != 2 ) {
			return 0;
		}
		sp = lookup();
		if( sp == NULL ) {
			error("undefined symbol '%s'", token_buf);
			return -1;
		}
		if( (sp->n_type & ~N_EXT) == N_ABS ) {
			v = bit_field(31,26,OP_BRN);	// op
			v |= bit_field(25,22,func);		// func
			v |= bit_field(21,0,(sp->n_value - *pcur_loc)>>2);	// disp
			out_code(v);
			return 0;
		}
		if( (sp->n_type & ~N_EXT) != N_UNDF ) {
			v = bit_field(31,26,OP_BRN);	// op
			v |= bit_field(25,22,func);		// func
			v |= bit_field(21,0,(sp->n_value - *pcur_loc)>>2);	// disp
			out_code(v);
			return 0;
		}
		else {
			error("undefined symbol '%s'", token_buf);
			return -1;
		}
	}
	else if( token.type == sym_local_symbol ) {
		if( pass == 2 ) {
			char ch = token.name[0];
			int i;
			if( local_symbol_cnt == 0 ) {
				error("no local label defined");
				out_code(0);
				return -1;
			}
			if( ch == 'b' || ch == 'B' ) {
				uint32_t new_value = 0;
				int new_value_flag = 0;
				for(i=0;i<local_symbol_cnt;i++) {
					if( local_symbol_table[i].value > *pcur_loc ) {
						break;
					}
					if( local_symbol_table[i].lable == token.value ) {
						new_value = local_symbol_table[i].value;
						new_value_flag = 1;
					}
				}
				if( new_value_flag == 0 ) {
					error("undefined local symbol '%db'", token.value);
					out_code(0);
					return -1;
				}
				else {
					v = bit_field(31,26,OP_BRN);	// op
					v |= bit_field(25,22,func);		// func
					v |= bit_field(21,0,(new_value - *pcur_loc)>>2);	// disp
					out_code(v);
					return 0;
				}
			}
			else if( ch == 'f' || ch == 'F' ) {
				uint32_t new_value = 0;
				int new_value_flag = 0;
				for(i=0;i<local_symbol_cnt;i++) {
					if( local_symbol_table[i].value > *pcur_loc ) {
						if( local_symbol_table[i].lable == token.value ) {
							new_value = local_symbol_table[i].value;
							new_value_flag = 1;
							break;
						}
					}
				}
				if( new_value_flag == 0 ) {
					error("undefined local symbol '%df'", token.value);
					out_code(0);
					return -1;
				}
				else {
					v = bit_field(31,26,OP_BRN);	// op
					v |= bit_field(25,22,func);		// func
					v |= bit_field(21,0,(new_value - *pcur_loc)>>2);	// disp
					out_code(v);
					return 0;
				}
			}
			else {
				error("syntax error");
				out_code(0);
				return -1;
			}
		}
	}
	error("syntax error");
	return -1;
}

int jump_common(int func)
{
	struct nlist *sp;
	unsigned int v;
	next_token();
	if( token.type == sym_num ) {
		if( pass != 2 ) {
			return 0;
		}
		v = func;
		v |= bit_field(29,0,token.value);
		out_code(v);
		return 0;
	}
	else if( token.type == sym_gpr && func == OP_AJMP ) {
		if( pass != 2 ) {
			return 0;
		}
		inst.rd = token.value;
		v = bit_field(31,26, OP_JMPR);
		v |= bit_field(25,21,inst.rd);
		out_code(v);
		return 0;
	}
	else if( token.type == sym_gpr && func == OP_JAL ) {
		if( pass != 2 ) {
			return 0;
		}
		inst.rd = token.value;
		v = bit_field(31,26, OP_JALR);
		v |= bit_field(25,21,inst.rd);
		out_code(v);
		return 0;
	}
	else if( token.type == sym_name ) {
		if( pass != 2 ) {
			return 0;
		}
		sp = lookup();
		if( sp == NULL ) {
			error("undefined symbol '%s'",token_buf);
			return -1;
		}
		if( (sp->n_type & N_EXT) == N_ABS ) {
			v = func;
			v |= bit_field(29,0,sp->n_value);	// addr
			out_code(v);
			return 0;
		}
		else {
			v = func;
			add_reloc(*pcur_loc,BIT30,sp);
			v |= bit_field(29,0,sp->n_value>>2);	// addr
			out_code(v);
			return 0;
		}
	}
	else {
		error("syntax error");
		return -1;
	}
}

static int alu_common_ex(uint32_t opc_alur, uint32_t opc_immed, const char *inst_str)
{
	unsigned int v;
	if( alu_common(opc_immed) != 0 ) {
		return -1;
	}

	if( inst.immed_flag ) {
		if( opc_immed == OP_ANDI || opc_immed == OP_ORI || opc_immed == OP_XORI ) {
			if( inst.immed & 0xffff0000 ) {
				inst_size = 12;
			}
		}
		else {
			if( inst.immed < -32768 || inst.immed > 32767 ) {
				inst_size = 12;
			}
		}
	}

	if( pass != 2 ) {
		return 0;
	}
	if( inst.immed_flag ) {
		if( opc_immed != -1 ) {
			if( inst_size == 4 ) {
				v = bit_field(31,26,opc_immed);	// op
				v |= bit_field(25,21,inst.rd);	// rd
				v |= bit_field(20,16,inst.rs);	// rs
				v |= bit_field(15,0,inst.immed);// immed
				out_code(v);
			}
			else if( inst_size == 12 ) {
				v = bit_field(31,26, OP_LDIH);
				v |= bit_field(25,21,RESERVED_REG);
				v |= bit_field(15,0,(inst.immed>>16));
				out_code(v);
				v = bit_field(31,26, OP_ORI);
				v |= bit_field(25,21,RESERVED_REG);
				v |= bit_field(20,16,RESERVED_REG);
				v |= bit_field(15,0,inst.immed);
				out_code(v);
				v = bit_field(31,26,0x00);			// op
				v |= bit_field(25,21,inst.rd);		// rd
				v |= bit_field(20,16,inst.rs);		// rs
				v |= bit_field(15,11,RESERVED_REG);	// rs2
				v |= bit_field(3,0,opc_alur);		// func
				out_code(v);
			}
			else {
				error("%s:%d internal error", __FILE__, __LINE__);
			}
		}
		else {
			if( opc_alur == ALUR_SUB ) {
				if( inst_size == 4 ) {
					v = bit_field(31,26, OP_ADDI);		// op
					v |= bit_field(25,21,inst.rd);		// rd
					v |= bit_field(20,16,inst.rs);		// rs
					v |= bit_field(15,0,-inst.immed);	// immed
					out_code(v);
				}
				else if( inst_size == 12 ) {
					v = bit_field(31,26, OP_LDIH);
					v |= bit_field(25,21,RESERVED_REG);
					v |= bit_field(15,0,(inst.immed>>16));
					out_code(v);
					v = bit_field(31,26, OP_ORI);
					v |= bit_field(25,21,RESERVED_REG);
					v |= bit_field(20,16,RESERVED_REG);
					v |= bit_field(15,0,inst.immed);
					out_code(v);
					v = bit_field(31,26,0x00);			// op
					v |= bit_field(25,21,inst.rd);		// rd
					v |= bit_field(20,16,inst.rs);		// rs
					v |= bit_field(15,11,RESERVED_REG);	// rs2
					v |= bit_field(3,0,ALUR_SUB);		// func
					out_code(v);
				}
				else {
					error("%s:%d internal error", __FILE__, __LINE__);
				}
			}
			else {
				error("'%s' no immed instruction", inst_str);
			}
			return -1;
		}
	}
	else {
		if( inst_size == 4 ) {
			v = bit_field(31,26,0x00);		// op
			v |= bit_field(25,21,inst.rd);	// rd
			v |= bit_field(20,16,inst.rs);	// rs
			v |= bit_field(15,11,inst.rs2);	// rs2
			v |= bit_field(3,0,opc_alur);	// func
			out_code(v);
		}
		else {
			error("%s:%d internal error", __FILE__, __LINE__);
		}
	}
	return 0;
}

int do_add(void)
{
	return alu_common_ex(ALUR_ADD, OP_ADDI, "add");
}

int do_adc(void)
{
	return alu_common_ex(ALUR_ADC, -1, "adc");
}

int do_sub(void)
{
	return alu_common_ex(ALUR_SUB, -1, "sub");
}

int do_sbc(void)
{
	return alu_common_ex(ALUR_SBC, -1, "sbc");
}

int do_and(void)
{
	return alu_common_ex(ALUR_AND, OP_ANDI, "and");
}

int do_or(void)
{
	return alu_common_ex(ALUR_ORR, OP_ORI, "or");
}

int do_xor(void)
{
	return alu_common_ex(ALUR_XOR, OP_XORI, "xor");
}

int do_mul(void)
{
	return alu_common_ex(ALUR_MUL, -1, "mul");
}

int do_div(void)
{
	return alu_common_ex(ALUR_DIV, -1, "div");
}

int do_mod(void)
{
	return alu_common_ex(ALUR_MOD, -1, "mod");
}

int do_mulu(void)
{
	return alu_common_ex(ALUR_MULU, -1, "mulu");
}

int do_divu(void)
{
	return alu_common_ex(ALUR_DIVU, -1, "divu");
}

int do_modu(void)
{
	return alu_common_ex(ALUR_MODU, -1, "modu");
}

int do_lsl(void)
{
	return shift_common(ALUR_LSL, OP_LSLI);
}

int do_lsr(void)
{
	return shift_common(ALUR_LSR, OP_LSRI);
}

int do_asr(void)
{
	return shift_common(ALUR_ASR, OP_ASRI);
}

int do_ldih(void)
{
	unsigned int v;
	if( next_token() != sym_gpr ) {
		error("expect a register");
		return -1;
	}
	inst.rd = token.value;
	if( next_token() != sym_comma ) {
		error("expect ','");
		return -1;
	}
	next_token();
	if( token.type == sym_num ) {
		inst.immed = token.value;
		if( inst.immed < 0 || inst.immed > 65535 ) {
			error("immediate number(=%d, =0x%x) overflow, must in range 0 to 65535",\
				inst.immed, inst.immed);
			return -1;
		}
		if( pass != 2 ) {
			return 0;
		}
		v = bit_field(31,26, OP_LDIH);	// op
		v |= bit_field(25,21,inst.rd);	// rd
		v |= bit_field(15,0,inst.immed);// immed
		out_code(v);
		return 0;
	}
	else {
		error("syntax error");
		return -1;
	}
}

// lw $n, immed
// lw $n, idt
// lw $n, ($n)
// lw $n, immed($n)
// lw $n, idt($n)
static int mem_common(uint32_t opc)
{
	unsigned int v;
	int minus_flag = 0;
	int immed_avliable = 0;
	if( opc != OP_LDD && opc != OP_LDF && opc != OP_STD && opc != OP_STF ) {
		if( next_token() != sym_gpr ) {
			error("expect a register");
			return -1;
		}
	}
	else {
		if( next_token() != sym_fpr ) {
			error("expect a float register");
			return -1;
		}
	}
	memset(&inst,0,sizeof(inst_t));
	inst.rd = token.value;
	if( next_token() != sym_comma ) {
		error("expect ','");
		return -1;
	}
	next_token();

	if( token.type == sym_minus || token.type == sym_plus ) {
		minus_flag = (token.type == sym_minus ) ? 1 : 0;
		next_token();
		if( token.type != sym_num ) {
			error("syntax error, expect a number");
			return -1;
		}
	}

	struct nlist *sp = NULL;

	if( token.type == sym_num || token.type == sym_name ) {
		node_t node;
		if( token.type == sym_name ) {
			if( pass == 2 ) {
				sp = lookup();
				if( sp == NULL ) {
					error("undefined symbol '%s'", token.name);
					return -1;
				}
				inst.immed = sp->n_value;
			}
		}
		else {
			inst.immed = minus_flag ? -token.value : token.value;
			immed_avliable = 1;
		}
		next_token();
		if( token.type == sym_plus || token.type == sym_minus) {
			int sig = (token.type == sym_minus)?-1:1;
			if( const_expression(&node) < 0 ) {
				return -1;
			}
			inst.immed += sig * node.value;
		}
		if( token.type == sym_lbrack ) {
			next_token();
			if( token.type != sym_gpr ) {
				error("syntax error, expect a register");
				return -1;
			}
			inst.rs = token.value;
			next_token();
			if( token.type != sym_rbrack ) {
				error("syntax error, expect ')'");
				return -1;
			}
			next_token();
		}
	}
	else if( token.type == sym_lbrack ) {
		immed_avliable = 1;
		next_token();
		if( token.type != sym_gpr ) {
			error("syntax error, expect a register");
			return -1;
		}
		inst.rs = token.value;
		next_token();
		if( token.type != sym_rbrack ) {
			error("syntax error, expect ')'");
			return -1;
		}
		next_token();
	}
	else {
		error("syntax error");
		return -1;
	}

	if( immed_avliable ) {
		if( opc != -1 ) {
			// lw sw lh sh lb sb lhu lbu
			if( inst.immed & 0xffff0000 ) {
				inst_size = (inst.rs == 0) ? 8 : 12;
			}
		}
		else {
			// lea
			if( inst.immed & 0xffff8000 ) {
				inst_size = (inst.rs == 0) ? 8 : 12;
			}
		}
	}
	else {
		inst_size = (inst.rs == 0) ? 8 : 12;
	}

	if( pass != 2 ) {
		return 0;
	}
	
	if( inst_size == 12 ) {
		if( opc != -1 ) {
			if( sp ) {
				// (5) sw $2, g_a($3)
				// (6) sw $2, g_a + 0x12345678($3)
				if( (sp->n_type & ~N_EXT) != N_ABS ) {
					add_reloc(*pcur_loc,BIT_16H_NONE_16L,sp);
				}
			}
			else
			{
				// (7) sw $2, 0x12345678($3)
			}
			v = bit_field(31,26, OP_LDIH);
			v |= bit_field(25,21,RESERVED_REG);
			v |= bit_field(15,0,inst.immed>>16);
			out_code(v);
			v = bit_field(31,26,0x00);
			v |= bit_field(25,21,RESERVED_REG);
			v |= bit_field(20,16,RESERVED_REG);
			v |= bit_field(15,11,inst.rs);
			v |= bit_field(3,0,ALUR_ADD);
			out_code(v);
			v = bit_field(31,26, opc);
			v |= bit_field(25,21,inst.rd);
			v |= bit_field(20,16,RESERVED_REG);
			v |= bit_field(15,0,inst.immed);
			out_code(v);
		}
		else {
			// lea
			if( sp ) {
				// (5) lea $2, g_a($3)
				// (6) lea $2, g_a + 0x12345678($3)
				if( (sp->n_type & ~N_EXT) != N_ABS ) {
					add_reloc(*pcur_loc,BIT_16H_16L,sp);
				}
			}
			else
			{
				// (7) lea $2, 0x12345678($3)
			}
			v = bit_field(31,26, OP_LDIH);
			v |= bit_field(25,21,RESERVED_REG);
			v |= bit_field(15,0,inst.immed>>16);
			out_code(v);
			v = bit_field(31,26, OP_ORI);
			v |= bit_field(25,21,RESERVED_REG);
			v |= bit_field(20,16,RESERVED_REG);
			v |= bit_field(15,0,inst.immed);
			out_code(v);
			v = bit_field(31,26,0x00);
			v |= bit_field(25,21,inst.rd);
			v |= bit_field(20,16,RESERVED_REG);
			v |= bit_field(15,11,inst.rs);
			v |= bit_field(3,0,ALUR_ADD);
			out_code(v);
		}
	}
	else if( inst_size == 8 ) {
		if( opc != -1 ) {
			if( sp ) {
				// (1) sw $2, g_a
				// (4) sw $2, g_a + 0x12345678
				if( (sp->n_type & ~N_EXT) != N_ABS ) {
					add_reloc(*pcur_loc,BIT_16H_16L,sp);
				}
			}
			else {
				// (2) sw $2, 0x12345678
			}
			v = bit_field(31,26, OP_LDIH);
			v |= bit_field(25,21,RESERVED_REG);
			v |= bit_field(15,0,inst.immed>>16);
			out_code(v);
			v = bit_field(31,26, opc);
			v |= bit_field(25,21,inst.rd);
			v |= bit_field(20,16,RESERVED_REG);
			v |= bit_field(15,0,inst.immed);
			out_code(v);
		}
		else {
			if( sp ) {
				// (1) lea $2, g_a
				// (4) lea $2, g_a + 0x12345678
				if( (sp->n_type & ~N_EXT) != N_ABS ) {
					add_reloc(*pcur_loc,BIT_16H_16L,sp);
				}
			}
			else {
				// (2) lea $2, 0x12345678
			}
			v = bit_field(31,26, OP_LDIH);
			v |= bit_field(25,21,RESERVED_REG);
			v |= bit_field(15,0,inst.immed>>16);
			out_code(v);
			v = bit_field(31,26, OP_ORI);
			v |= bit_field(25,21,inst.rd);
			v |= bit_field(20,16,RESERVED_REG);
			v |= bit_field(15,0,inst.immed);
			out_code(v);
		}
	}
	else if( inst_size == 4 ) {
		if( opc != -1 ) {
			// (3) lw $2, 0x1234
			v = bit_field(31,26, opc);
			v |= bit_field(25,21,inst.rd);
			v |= bit_field(20,16,inst.rs);
			v |= bit_field(15,0,inst.immed);
			out_code(v);
		}
		else {
			// lea
			// (3) lea $2, 0x1234
			v = bit_field(31,26, OP_ADDI);
			if( inst.immed < 0 || inst.immed > 32767 ) {
				error("immed overflow");
				return -1;
			}
			v |= bit_field(25,21,inst.rd);
			v |= bit_field(20,16,inst.rs);	// rs == 0
			v |= bit_field(15,0,inst.immed);
			out_code(v);
		}
	}
	else {
		error("%s:%d internal error", __FILE__, __LINE__);
		return -1;
	}
	return 0;
}

int do_lea(void)
{
	return mem_common(-1);
}

int do_lw(void)
{
	return mem_common(OP_LDW);
}

int do_sw(void)
{
	return mem_common(OP_STW);
}

int do_lh(void)
{
	return mem_common(OP_LDH);
}

int do_sh(void)
{
	return mem_common(OP_STH);
}

int do_lb(void)
{
	return mem_common(OP_LDB);
}

int do_sb(void)
{
	return mem_common(OP_STB);
}

int do_lbu(void)
{
	return mem_common(OP_LDBU);
}

int do_lhu(void)
{
	return mem_common(OP_LDHU);
}

int do_beq(void)
{
	return branch_common(brn_func_beq);
}

int do_bne(void) {
	return branch_common(brn_func_bne);
}

int do_bcs(void) {
	return branch_common(brn_func_bcs);
}

int do_bcc(void) {
	return branch_common(brn_func_bcc);
}

int do_bmi(void)
{
	return branch_common(brn_func_bmi);
}

int do_bhs(void)
{
	return branch_common(brn_func_bhs);
}

int do_blo(void)
{
	return branch_common(brn_func_blo);
}

int do_bvs(void)
{
	return branch_common(brn_func_bvs);
}

int do_bvc(void)
{
	return branch_common(brn_func_bvc);
}

int do_bhi(void)
{
	return branch_common(brn_func_bhi);
}

int do_bls(void)
{
	return branch_common(brn_func_bls);
}

int do_bge(void)
{
	return branch_common(brn_func_bge);
}

int do_blt(void)
{
	return branch_common(brn_func_blt);
}

int do_bgt(void)
{
	return branch_common(brn_func_bgt);
}

int do_ble(void)
{
	return branch_common(brn_func_ble);
}

int do_brn(void)
{
	return branch_common(brn_func_bal);
}

int do_jmp(void)
{
	return jump_common(OP_AJMP);
}

int do_jal(void)
{
	 return jump_common(OP_JAL);
}

int do_reti(void)
{
	if( pass != 2 ) {
		return 0;
	}
	unsigned int v;
	v = bit_field(31,26,OP_MISC);
	v |= bit_field(3,0,OP_MISC_RETI);
	out_code(v);
	return 0;
}

int do_trap(void)
{
	if( pass != 2 ) {
		return 0;
	}
	unsigned int v;
	v = bit_field(31,26,OP_MISC);
	v |= bit_field(3,0,OP_MISC_TRAP);
	out_code(v);
	return 0;
}

int do_halt(void)
{
	if( pass != 2 ) {
		return 0;
	}
	unsigned int v;
	v = bit_field(31,26,OP_MISC);
	v |= bit_field(3,0,OP_MISC_HALT);
	out_code(v);
	return 0;
}

int do_enai(void)
{
	unsigned int v;
	if( pass != 2 ) {
		return 0;
	}
	v = bit_field(31,26,OP_MISC);
	v |= bit_field(3,0,OP_MISC_ENAI);
	out_code(v);
	return 0;
}

int do_disi(void)
{
	unsigned int v;
	if( pass != 2 ) {
		return 0;
	}
	v = bit_field(31,26,OP_MISC);
	v |= bit_field(3,0,OP_MISC_DISI);
	out_code(v);
	return 0;
}

// not rd, rs
int do_not(void)
{
	unsigned int v;
	if( next_token() != sym_gpr ) {
		error("expect a register");
		return -1;
	}
	inst.rd = token.value;
	if( next_token() != sym_comma ) {
		error("expect ','");
		return -1;
	}
	if( next_token() != sym_gpr ) {
		error("expect a register");
		return -1;
	}
	inst.rs = token.value;
	if( pass != 2 ) {
		return 0;
	}
	v = bit_field(31,26, OP_MISC);	// op
	v |= bit_field(3,0,OP_MISC_NOT);
	v |= bit_field(25,21,inst.rd);	// rd
	v |= bit_field(20,16,inst.rs);	// rs
	out_code(v);
	return 0;
}

int do_lev(void)
{
	if( next_token() != sym_num ) {
		error("expect a number");
		return -1;
	}
	inst.immed = token.value;
	if( inst.immed < -32768 || inst.immed > 32767 ) {
		error("immed value (= %d) overflow, must in range -32768 to 32767", inst.immed);
		return -1;
	}
	if( pass != 2 ) {
		return 0;
	}
	unsigned int v;
	v = bit_field(31,26,OP_MISC);
	v |= bit_field(3,0,OP_MISC_LEV);
	v |= bit_field(19,4,inst.immed);
	out_code(v);
	return 0;
}

int do_mov(void)
{
	unsigned int v;
	if( next_token() != sym_gpr ) {
		error("expect a register");
		return -1;
	}
	inst.rd = token.value;
	if( next_token() != sym_comma ) {
		error("expect ','");
		return -1;
	}
	inst.rs = 0;	// rs always zero
	next_token();
	if( token.type == sym_gpr ) {
		inst.rs2 = token.value;
		if( pass != 2 ) {
			next_token();
			return 0;
		}
		v = bit_field(31,26,0x00);		// op
		v |= bit_field(25,21,inst.rd);	// rd
		v |= bit_field(20,16,0);		// rs = zero
		v |= bit_field(15,11,inst.rs2);	// rs2
		v |= bit_field(3,0,ALUR_ORR);	// func
		out_code(v);
		return 0;
	}
	else {
		error("syntax error");
		return -1;
	}
}

// LJL 20170523 pm
// cmp rs, rs2
int do_cmp(void)
{
	unsigned int v;
	int rs, rs2;
	if( next_token() != sym_gpr ) {
		error("expect a register");
		return -1;
	}
	rs = token.value;
	if( next_token() != sym_comma ) {
		error("expect ','");
		return -1;
	}
	if( next_token() != sym_gpr ) {
		error("expect a register");
		return -1;
	}
	if( pass != 2 ) {
		return 0;
	}
	rs2 = token.value;
	v = bit_field(31,26,0x00);	// op
	v |= bit_field(25,21,0);	// r0
	v |= bit_field(20,16,rs);	// rs
	v |= bit_field(15,11,rs2);	// rs2
	v |= bit_field(3,0,ALUR_SUB);	// func
	out_code(v);
	return 0;
}

// LJL 20171010 pm
// neg rd, rs
int do_neg(void)
{
	unsigned int v;
	int rd, rs;
	if( next_token() != sym_gpr ) {
		error("expect a register");
		return -1;
	}
	rd = token.value;
	if( next_token() != sym_comma ) {
		error("expect ','");
		return -1;
	}
	if( next_token() != sym_gpr ) {
		error("expect a register");
		return -1;
	}
	if( pass != 2 ) {
		return 0;
	}
	rs = token.value;
	v = bit_field(31,26,0x00);				// op
	v |= bit_field(25,21,rd);				// rd
	v |= bit_field(20,16,0);				// r0
	v |= bit_field(15,11,rs);				// rs
	v |= bit_field(3,0,ALUR_SUB);			// func
	out_code(v);
	return 0;
}

// LJL 20171019
// mtc $cn, $n
int do_mtc(void)
{
	unsigned int v;
	int cpr, gpr;
	if( next_token() != sym_cpr ) {
		error("expect a control register");
		return -1;
	}
	cpr = token.value;
	if( next_token() != sym_comma ) {
		error("expect ','");
		return -1;
	}
	if( next_token() != sym_gpr ) {
		error("expect a register");
		return -1;
	}
	if( pass != 2 ) {
		return 0;
	}
	gpr = token.value;
	v = bit_field(31,26, OP_MISC);	// op
	v |= bit_field(3,0,OP_MISC_MTC);
	v |= bit_field(25,21,cpr);	// rd
	v |= bit_field(20,16,gpr);	// rs
	out_code(v);
	return 0;
}

// LJL 20171019
// mfc $n, $cn
int do_mfc(void)
{
	unsigned int v;
	int cpr, gpr;
	if( next_token() != sym_gpr ) {
		error("expect a register");
		return -1;
	}
	gpr = token.value;
	if( next_token() != sym_comma ) {
		error("expect ','");
		return -1;
	}
	if( next_token() != sym_cpr ) {
		error("expect a control register");
		return -1;
	}
	if( pass != 2 ) {
		return 0;
	}
	cpr = token.value;
	v = bit_field(31,26, OP_MISC);	// op
	v |= bit_field(3,0,OP_MISC_MFC);
	v |= bit_field(25,21,gpr);		// rd
	v |= bit_field(20,16,cpr);		// rs
	out_code(v);
	return 0;
}

int do_pusha(void)
{
	unsigned int v;
	inst_size = 30*4;
	if( pass != 2 ) {
		return 0;
	}
	int i;
	for(i=0;i<32;i++) {
		if( i == 0 || i == SP_REG_IDX ) {
			continue;
		}
		v = bit_field(31,26, OP_STW);
		v |= bit_field(25,21,i);
		v |= bit_field(20,16,SP_REG_IDX);
		v |= bit_field(15,0,i*4);
		out_code(v);
	}
	return 0;
}

int do_popa(void)
{
	unsigned int v;
	inst_size = 30*4;
	if( pass != 2 ) {
		return 0;
	}
	int i;
	for(i=0;i<32;i++) {
		if( i == 0 || i == SP_REG_IDX ) {
			continue;
		}
		v = bit_field(31,26, OP_LDW);
		v |= bit_field(25,21,i);
		v |= bit_field(20,16,SP_REG_IDX);
		v |= bit_field(15,0,i*4);
		out_code(v);
	}
	return 0;
}

static int fpu_commom(int fpu_op)
{
	unsigned int v = 0;
	memset(&inst,0,sizeof(inst_t));
	if( next_token() != sym_fpr ) {
		error("syntax error, expect a float register1");
		return -1;
	}
	inst.rd = token.value;
	if( next_token() != sym_comma ) {
		error("syntax error, expect ','");
		return -1;
	}
	if( next_token() != sym_fpr ) {
		error("syntax error, expect a float register2");
		return -1;
	}
	inst.rs = token.value;
	if( fpu_op != FPU_MOVS && fpu_op != FPU_MOVD && fpu_op != FPU_CVDF\
		&& fpu_op != FPU_CVFD && fpu_op != FPU_CVWD && fpu_op != FPU_CVDW\
		&& fpu_op != FPU_NEGD && fpu_op != FPU_NEGS )
	{
		if( next_token() != sym_comma ) {
			error("syntax error, expect ','");
			return -1;
		}
		if( next_token() != sym_fpr ) {
			error("syntax error, expect a float register");
			return -1;
		}
		inst.rs2 = token.value;
	}
	if( pass == 2 ) {
		v = bit_field(31,26,OP_FPU);
		v |= bit_field(25,21,inst.rd);
		v |= bit_field(20,16,inst.rs);
		v |= bit_field(15,11,inst.rs2);
		v |= bit_field(3,0, fpu_op);
		out_code(v);
	}
	return 0;
}

int do_add_s(void)
{
	return fpu_commom(FPU_ADDS);
}

int do_sub_s(void)
{
	return fpu_commom(FPU_SUBS);
}

int do_mul_s(void)
{
	return fpu_commom(FPU_MULS);
}

int do_div_s(void)
{
	return fpu_commom(FPU_DIVS);
}

int do_mov_s(void)
{
	return fpu_commom(FPU_MOVS);
}

int do_add_d(void)
{
	return fpu_commom(FPU_ADDD);
}

int do_sub_d(void)
{
	return fpu_commom(FPU_SUBD);
}

int do_mul_d(void)
{
	return fpu_commom(FPU_MULD);
}

int do_div_d(void)
{
	return fpu_commom(FPU_DIVD);
}

int do_mov_d(void)
{
	return fpu_commom(FPU_MOVD);
}

int do_neg_d(void)
{
	return fpu_commom(FPU_NEGD);
}

int do_neg_s(void)
{
	return fpu_commom(FPU_NEGS);
}

static int mfc1_mtc1_common(int func)
{
	unsigned int v = 0;
	memset(&inst,0,sizeof(inst_t));
	if( next_token() != sym_gpr ) {
		error("syntax error, expert a register");
		return -1;
	}
	inst.rd = token.value;
	if( next_token() != sym_comma ) {
		error("syntax error, expert ','");
		return -1;
	}
	if( next_token() != sym_fpr ) {
		error("syntax error, expert a float register");
		return -1;
	}
	inst.rs = token.value;
	if( pass == 2 ) {
		v = bit_field(31,26, OP_FPU_MISC);
		v |= bit_field(3,0, func);
		v |= bit_field(25,21,inst.rd);
		v |= bit_field(20,16,inst.rs);
		out_code(v);
	}
	return 0;
}

int do_mtc1(void)
{
	return mfc1_mtc1_common(FPU_MISC_MTC1);
}

int do_mfc1(void)
{
	return mfc1_mtc1_common(FPU_MISC_MFC1);
}

int do_mtc1_d(void)
{
	return mfc1_mtc1_common(FPU_MISC_MTC1_D);
}

int do_mfc1_d(void)
{
	return mfc1_mtc1_common(FPU_MISC_MFC1_D);
}

// convet double to word
int do_cvt_w_d(void)
{
	return fpu_commom(FPU_CVDW);
}

// convet double to single
int do_cvt_s_d(void)
{
	return fpu_commom(FPU_CVDF);
}

// convet word to double
int do_cvt_d_w(void)
{
	return fpu_commom(FPU_CVWD);
}

// covvet single to double
int do_cvt_d_s(void)
{
	return fpu_commom(FPU_CVFD);
}

// load double
int do_ld_d(void)
{
	return mem_common(OP_LDD);
}

// load single
int do_ld_s(void)
{
	return mem_common(OP_LDF);
}

// store double
int do_st_d(void)
{
	return mem_common(OP_STD);
}

// load single
int do_st_s(void)
{
	return mem_common(OP_STF);
}

static int fcmp_common(int func)
{
	unsigned int v;
	int rs, rs2;
	if( next_token() != sym_fpr ) {
		error("expect float register1");
		return -1;
	}
	rs = token.value;
	if( next_token() != sym_comma ) {
		error("expect ','");
		return -1;
	}
	if( next_token() != sym_fpr ) {
		error("expect float register2");
		return -1;
	}
	if( pass == 2 ) {
		rs2 = token.value;
		v = bit_field(31,26, OP_FPU_MISC);
		v |= bit_field(3,0, func);
		v |= bit_field(25,21,rs);
		v |= bit_field(20,16,rs2);
		out_code(v);
	}
	return 0;
}


int do_c_eq_d(void)
{
	return fcmp_common(FPU_MISC_CMP_EQD);
}

int do_c_le_d(void)
{
	return fcmp_common(FPU_MISC_CMP_LED);
}

int do_c_lt_d(void)
{
	return fcmp_common(FPU_MISC_CMP_LTD);
}

int do_c_eq_s(void)
{
	return fcmp_common(FPU_MISC_CMP_EQS);
}

int do_c_le_s(void)
{
	return fcmp_common(FPU_MISC_CMP_LES);
}

int do_c_lt_s(void)
{
	return fcmp_common(FPU_MISC_CMP_LTS);
}

static int bc1f_bc1t_common(int func)
{
	struct nlist *sp;
	unsigned int v;
	next_token();
	if( token.type == sym_num ) {
		if( pass == 2 ) {
			v = bit_field(31,26, OP_FPU_MISC);
			v |= bit_field(25,4,token.value);
			v |= bit_field(3,0, func);
			out_code(v);
		}
		return 0;
	}
	else if( token.type == sym_name ) {
		if( pass != 2 ) {
			return 0;
		}
		sp = lookup();
		if( sp == NULL ) {
			error("undefined symbol '%s'", token_buf);
			return -1;
		}
		if( (sp->n_type & ~N_EXT) == N_ABS ) {
			v = bit_field(31,26,OP_FPU_MISC);
			v |= bit_field(25,4,(sp->n_value - *pcur_loc)>>2);
			v |= bit_field(3,0,func);
			out_code(v);
			return 0;
		}
		if( (sp->n_type & ~N_EXT) != N_UNDF ) {
			v = bit_field(31,26,OP_FPU_MISC);
			v |= bit_field(25,4,(sp->n_value - *pcur_loc)>>2);
			v |= bit_field(3,0,func);
			out_code(v);
			return 0;
		}
		else {
			error("undefined symbol '%s'", token_buf);
			return -1;
		}
	}
	else {			
		error("syntax error");
		return -1;
	}
}

int do_bc1t(void)
{
	return bc1f_bc1t_common(FPU_MISC_BC1T);
}

int do_bc1f(void)
{
	return bc1f_bc1t_common(FPU_MISC_BC1F);
}

static int tlb_common(int func)
{
	int n, reg;
	uint32_t v;
	if( next_token() != sym_num ) {
		error("syntax error, expect a number");
		return -1;
	}
	n = token.value;
	if( n < 0 || n > 63 ) {
		error("syntax error, tlb number error");
		return -1;
	}
	if( next_token() != sym_comma ) {
		error("syntax error, expect a ','");
		return -1;
	}
	if( next_token() != sym_gpr ) {
		error("syntax error, expect a register");
		return -1;
	}
	if( pass != 2 ) {
		return 0;
	}
	reg = token.value;
	v = bit_field(31,26, OP_MISC);	// op
	v |= bit_field(3,0, func);
	v |= bit_field(9, 4, n);		// rd
	v |= bit_field(20,16,reg);		// rs
	out_code(v);
	return 0;
}

int do_tlb_mr_w(void)
{
	return tlb_common(OP_MISC_TLB_MR_W);
}

int do_tlb_mr_r(void)
{
	return tlb_common(OP_MISC_TLB_MR_R);
}

int do_tlb_tr_w(void)
{
	return tlb_common(OP_MISC_TLB_TR_W);
}

int do_tlb_tr_r(void)
{
	return tlb_common(OP_MISC_TLB_TR_R);
}

const keyword_hash_t function_buffer[] =
{
	{ "jmp", sym_opcode, 0, do_jmp },
	{ "jal", sym_opcode, 0, do_jal },
	{ "add", sym_opcode, 0, do_add },
	{ "adc", sym_opcode, 0, do_adc },
	{ "sub", sym_opcode, 0, do_sub },
	{ "sbc", sym_opcode, 0, do_sbc },
	{ "and", sym_opcode, 0, do_and },
	{ "or",  sym_opcode, 0, do_or },
	{ "xor", sym_opcode, 0, do_xor },
	{ "mul", sym_opcode, 0, do_mul },
	{ "div", sym_opcode, 0, do_div },
	{ "mod", sym_opcode, 0, do_mod },
	{ "mulu", sym_opcode, 0, do_mulu },
	{ "divu", sym_opcode, 0, do_divu },
	{ "modu", sym_opcode, 0, do_modu },
	{ "lsl", sym_opcode, 0, do_lsl },
	{ "lsr", sym_opcode, 0, do_lsr },
	{ "asr", sym_opcode, 0, do_asr },
	{ "ldih",sym_opcode, 0, do_ldih },
	{ "lw", sym_opcode, 0, do_lw },
	{ "sw", sym_opcode, 0, do_sw },
	{ "lh", sym_opcode, 0, do_lh },
	{ "sh", sym_opcode, 0, do_sh },
	{ "lb", sym_opcode, 0, do_lb },
	{ "sb", sym_opcode, 0, do_sb },
	{ "lbu",sym_opcode, 0, do_lbu },
	{ "lhu",sym_opcode, 0, do_lhu },
	{ "beq", sym_opcode, 0, do_beq },
	{ "bne", sym_opcode, 0, do_bne },
	/*
	{ "bz",  sym_opcode, 0, do_beq },
	{ "bnz", sym_opcode, 0, do_bne },
	*/
	{ "bge", sym_opcode, 0, do_bge },
	{ "bgt", sym_opcode, 0, do_bgt },
	{ "ble", sym_opcode, 0, do_ble },
	{ "blt", sym_opcode, 0, do_blt },
	{ "bhs", sym_opcode, 0, do_bhs },
	{ "bhi", sym_opcode, 0, do_bhi },
	{ "blo", sym_opcode, 0, do_blo },
	{ "bls", sym_opcode, 0, do_bls },
	{ "bgeu", sym_opcode, 0, do_bhs },
	{ "bgtu", sym_opcode, 0, do_bhi },
	{ "bltu", sym_opcode, 0, do_blo },
	{ "bleu", sym_opcode, 0, do_bls },
	{ "bcs", sym_opcode, 0, do_bcs },
	{ "bcc", sym_opcode, 0, do_bcc },
	{ "bvs", sym_opcode, 0, do_bvs },
	{ "bvc", sym_opcode, 0, do_bvc },
	{ "brn", sym_opcode, 0, do_brn },
	{ "bmi", sym_opcode, 0, do_bmi },
	{ "reti",sym_opcode, 0, do_reti },
	{ "trap",sym_opcode, 0, do_trap },
	{ "halt",sym_opcode, 0, do_halt },
	{ "enai",sym_opcode, 0, do_enai },
	{ "disi",sym_opcode, 0, do_disi },
	{ "not", sym_opcode, 0, do_not },
	{ "lev", sym_opcode, 0, do_lev },
	{ "mtc", sym_opcode, 0, do_mtc },
	{ "mfc", sym_opcode, 0, do_mfc },
	{ ".popa",sym_opcode, 0, do_popa },
	{ ".pusha",sym_opcode, 0, do_pusha },
	{ "tlbmrw", sym_opcode, 0, do_tlb_mr_w },
	{ "tlbmrr", sym_opcode, 0, do_tlb_mr_r },
	{ "tlbtrw", sym_opcode, 0, do_tlb_tr_w },
	{ "tlbtrr", sym_opcode, 0, do_tlb_tr_r },
	{ "mov", sym_opcode, 0, do_mov },
	{ "cmp", sym_opcode, 0, do_cmp },
	{ "neg", sym_opcode, 0, do_neg },
	{ "lea", sym_opcode, 0, do_lea },
	{ "add.s", sym_opcode, 0, do_add_s },
	{ "sub.s", sym_opcode, 0, do_sub_s },
	{ "mul.s", sym_opcode, 0, do_mul_s },
	{ "div.s", sym_opcode, 0, do_div_s },
	{ "mov.s", sym_opcode, 0, do_mov_s },
	{ "add.d", sym_opcode, 0, do_add_d },
	{ "sub.d", sym_opcode, 0, do_sub_d },
	{ "mul.d", sym_opcode, 0, do_mul_d },
	{ "div.d", sym_opcode, 0, do_div_d },
	{ "mov.d", sym_opcode, 0, do_mov_d },
	{ "mfc1",  sym_opcode, 0, do_mfc1 },
	{ "mtc1",  sym_opcode, 0, do_mtc1 },
	{ "mfc1.d",  sym_opcode, 0, do_mfc1_d },
	{ "mtc1.d",  sym_opcode, 0, do_mtc1_d },
	//{ "cvt.w.s", sym_opcode, 0, do_cvt_w_s },
	{ "cvt.w.d", sym_opcode, 0, do_cvt_w_d },
	//{ "cvt.s.w", sym_opcode, 0, do_cvt_s_w },
	{ "cvt.s.d", sym_opcode, 0, do_cvt_s_d },
	{ "cvt.d.w", sym_opcode, 0, do_cvt_d_w },
	{ "cvt.d.s", sym_opcode, 0, do_cvt_d_s },
	{ "l.d", sym_opcode, 0, do_ld_d },
	{ "l.s", sym_opcode, 0, do_ld_s },
	{ "s.d", sym_opcode, 0, do_st_d },
	{ "s.s", sym_opcode, 0, do_st_s },
	{ "trunc.w.d", sym_opcode, 0, do_cvt_w_d }, //do_trunc_w_d,
	{ "c.eq.d", sym_opcode, 0, do_c_eq_d },
	{ "c.le.d", sym_opcode, 0, do_c_le_d },
	{ "c.lt.d", sym_opcode, 0, do_c_lt_d },
	{ "c.eq.s", sym_opcode, 0, do_c_eq_s },
	{ "c.le.s", sym_opcode, 0, do_c_le_s },
	{ "c.lt.s", sym_opcode, 0, do_c_lt_s },
	{ "bc1f", sym_opcode, 0, do_bc1f },
	{ "bc1t", sym_opcode, 0, do_bc1t },
	{ "neg.d", sym_opcode, 0, do_neg_d },
	{ "neg.s", sym_opcode, 0, do_neg_s },
};

int get_function_buffer_size(void)
{
	return sizeof(function_buffer)/sizeof(keyword_hash_t); 
}
