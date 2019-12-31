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
#include "cctest.h"
#include "isa.h"

bool cctest(brn_func_t what)
{
	bool fN = psw_get_bit( PSW_N );
	bool fZ = psw_get_bit( PSW_Z );
	bool fV = psw_get_bit( PSW_V );
	bool fC = psw_get_bit( PSW_C );
	//bool result;
	switch(what) {
		case brn_func_bal:		// ever true
		  return 1;
		case brn_func_bcs:		// carry set
			return fC;
		case brn_func_bcc:		// carry clear
			return !fC;
		case brn_func_bvs:		// overfllow set
			return fV;
		case brn_func_bvc:		// overfllow clear
			return !fV;
		case brn_func_bmi:		// minus
			return fN;
		/*
		case brn_func_bpl:		// positive
			return !fN;
		*/
		case brn_func_beq:		// ==
			return fZ;
		case brn_func_bne:		// !=
			return !fZ;

		case brn_func_blt:		// <
			return (fN ^ fV);

		case brn_func_bge:		// >=
			return !( fN ^ fV );

		case brn_func_ble:		// <=
			return ( fN ^ fV ) | fZ;

		case brn_func_bgt:		// >
			return !( ( fN ^ fV ) | fZ );

		case brn_func_blo:		// <  unsigned
			return fC;

		case brn_func_bhs:		// >= unsigned
			return !fC;

		case brn_func_bls:		// <= unsigned
			return (fZ | fC);

		case brn_func_bhi:		// >  unsigned
			return !(fZ | fC);

		default:
			error(__FILE__,__LINE__);
			return 0;
	}
	return 0;
}
