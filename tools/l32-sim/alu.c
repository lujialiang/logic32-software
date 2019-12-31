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
#include "common.h"
#include "alu.h"
#include "isa.h"

//#define DBUG_ALU_ADD
//#define DBUG_ALU_SUB

int BIT(uint32_t a,int x)
{
	return (((a & (1<<x)) >> x) & 1);
}

uint32_t alu_add(const uint32_t a, const uint32_t b, const bool cin)
{
	const bool Na = (a&0x80000000)?true:false;
	const bool Nb = (b&0x80000000)?true:false;
	const uint32_t al = a & 0xffff;
	const uint32_t ah = (a >> 16)&0xffff;
	const uint32_t bl = b & 0xffff;
	const uint32_t bh = (b >> 16)&0xffff;
	uint32_t x, y, z;
	uint32_t ql, qh, q;
	bool qlc, qhc;
	// low
	x = (al<<1) | (cin?1:0); 
	y = (bl<<1) | (cin?1:0); 
	z = x + y;
	ql = (z>>1)&0xffff;
	qlc = ((z>>17)&1) ? true : false;
	// high
	x = ( ah <<1 ) | (qlc?1:0);
	y = ( bh <<1 ) | (qlc?1:0);
	z = x + y;
	qh = (z>>1)&0xffff;
	qhc = ((z>>17)&1) ? true : false;
	q = (qh<<16)|(ql&0xffff);
	psw_set_bit( PSW_N, BIT(q, ALU_BITS - 1) );
	psw_set_bit( PSW_Z, (q == 0) );
	psw_set_bit( PSW_V, (Na == Nb && Na != psw_get_bit(PSW_N) ) );
	psw_set_bit( PSW_C, qhc );
#ifdef DBUG_ALU_ADD
	printf("Q: %08x + %08x + %d = %08x\n\r", a, b, cin, q);
	printf("N = %d, Z = %d, V = %d, C = %d\n\n\r",\
		psw_get_bit(PSW_N),\
		psw_get_bit(PSW_Z),\
		psw_get_bit(PSW_V),\
		psw_get_bit(PSW_C)\
	);
#endif
	return q;
}

uint32_t alu_sub(const uint32_t a, const uint32_t b, const bool cin)
{
	const bool Na = (a&0x80000000)?true:false;
	const bool Nb = (b&0x80000000)?true:false;
	const uint32_t al = a & 0xffff;
	const uint32_t ah = (a >> 16)&0xffff;
	const uint32_t bl = (~b) & 0xffff;
	const uint32_t bh = (~b >> 16)&0xffff;
	uint32_t x, y, z;
	uint32_t ql, qh, q;
	bool qlc, qhc;
	// low
	x = (al<<1) | (cin?0:1); 
	y = (bl<<1) | (cin?0:1); 
	z = x + y;
	ql = (z>>1)&0xffff;
	qlc = ((z>>17)&1) ? true : false;
#ifdef DBUG_ALU_SUB
	printf("L: %08x - %08x - %d = %08x, qlc = %d\n\r", x, y, cin, z, qlc);
#endif
	// high
	x = ( ah <<1 ) | (qlc?1:0);
	y = ( bh <<1 ) | (qlc?1:0);
	z = x + y;
	qh = (z>>1)&0xffff;
	//qhc = ((z>>17)&1) ? true : false;
	qhc = ((z>>17)&1) ? false : true;
#ifdef DBUG_ALU_SUB
	printf("H: %08x - %08x - %d  = %08x, qhc = %d\n\r", x, y, qlc, z, qhc);
#endif
	q = (qh<<16)|(ql&0xffff);
	psw_set_bit( PSW_N, BIT(q, ALU_BITS -1 ) );
	psw_set_bit( PSW_Z, (q == 0) );
	psw_set_bit( PSW_V, (Na != Nb && Na != psw_get_bit(PSW_N) ) );
	psw_set_bit( PSW_C, qhc);
#ifdef DBUG_ALU_SUB
	printf("Q: %08x - %08x - %d = %08x\n\r", a, b, cin, q);
	printf("N = %d, Z = %d, V = %d, C = %d\n\n\r",\
		psw_get_bit(PSW_N),\
		psw_get_bit(PSW_Z),\
		psw_get_bit(PSW_V),\
		psw_get_bit(PSW_C)\
	);
#endif
	return q;
}

uint32_t alu_logic(uint32_t a, uint32_t b, alu_op_t op)
{
	uint32_t alu_q;
	switch(op) {
		case alu_op_and:
			alu_q = a&b; break;
		case alu_op_or:
			alu_q = a|b; break;
		case alu_op_not:
			alu_q = ~a; break;
		case alu_op_xor:
			alu_q = a^b; break;
		default:
			error(__FILE__,__LINE__);
			return 0;
	}
	psw_set_bit( PSW_N , BIT(alu_q, ALU_BITS-1) );
	psw_set_bit( PSW_Z , (alu_q == 0) );
	psw_set_bit( PSW_V , 0 );
	psw_set_bit( PSW_C , 0 );
	return alu_q;
}

uint32_t alu_shift(uint32_t a, alu_op_t op, uint8_t count)
{
	uint32_t alu_q;
	if( count & ~0x1f ) {
		error(__FILE__,__LINE__);
	}
	count &= 0x1f;
	switch(op) {
		case alu_op_lsl:
		case alu_op_asl:
			if( count ) {
				psw_set_bit( PSW_C , BIT(a, ALU_BITS-count) );
			}
			alu_q = a << count;
			break;
		case alu_op_lsr:
			if( count ) {
				psw_set_bit( PSW_C , BIT(a, count-1) );
			}
			alu_q = a >> count;
			break;
		case alu_op_asr:
			if( count ) {
				psw_set_bit( PSW_C , BIT(a, count-1) );
			}
			alu_q = (int)((int)a >> count);
			break;
		case alu_op_rol:
			if( count ) {
				psw_set_bit( PSW_C , 0 );
			}
			alu_q = (a << count) | (a >> (ALU_BITS - count));
			break;
		case alu_op_ror:
			if( count ) {
				psw_set_bit( PSW_C , 0 );
			}
			alu_q = (a >> count) | (a << (ALU_BITS - count));
			break;
		default:
			error(__FILE__,__LINE__);
			return 0;
	}
	psw_set_bit( PSW_N , BIT(alu_q, ALU_BITS-1) );
	psw_set_bit( PSW_Z , (alu_q == 0) );
	psw_set_bit( PSW_V , 0 );
	return alu_q;
}

uint32_t alu_multiplier(uint32_t a, uint32_t b, alu_op_t op)
{
	uint32_t alu_q;
	int32_t x, y, z;
	x = a;
	y = b;
	switch(op) {
		case alu_op_mul:
			z = x*y;
			alu_q = z;
			break;
		case alu_op_div:
			z = x/y;
			alu_q = z;
			break;
		case alu_op_mod:
			z = x%y;
			alu_q = z;
			break;
	#if 1	// LJL 20171018
		case alu_op_mulu:
			alu_q = a*b; break;
		case alu_op_divu:
			alu_q = a/b; break;
		case alu_op_modu:
			alu_q = a%b; break;
	#endif
		default:
			error(__FILE__,__LINE__);
			return 0;
	}
	psw_set_bit( PSW_N , BIT(alu_q, ALU_BITS-1) );
	psw_set_bit( PSW_Z , (alu_q == 0) );
	psw_set_bit( PSW_V , 0 );
	psw_set_bit( PSW_C , 0 );
	return alu_q;
}

#if 0
uint32_t alu(uint32_t a, uint32_t b, alu_op_t op)
{
	switch(op) {
		case alu_op_add:
			return alu_add(a,b,0);
		case alu_op_adc:
			return alu_add(a,b,psw_get_bit(PSW_C));
		case alu_op_sub:
			return alu_sub(a,b,0);
		case alu_op_sbc:
			return alu_sub(a,b,psw_get_bit(PSW_C));
		case alu_op_neg:
			return alu_sub(0,b,0);
		case alu_op_and:
		case alu_op_or:
		case alu_op_not:
		case alu_op_xor:
			return alu_logic(a,b,op);
		case alu_op_lsl:
		case alu_op_asl:
		case alu_op_lsr:
		case alu_op_asr:
		case alu_op_rol:
		case alu_op_ror:
			return alu_shift(a,op);
		default:
			error(__FILE__,__LINE__);
			break;
	}
}
#endif
