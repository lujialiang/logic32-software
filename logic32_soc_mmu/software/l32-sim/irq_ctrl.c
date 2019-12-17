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

#include "irq_ctrl.h"
#include <stdio.h>
#include <stdbool.h>
#include "isa.h"

static uint32_t intrrupt_bits;

int irq_ctrl_init(void)
{
	intrrupt_bits = 0;
	return 0;
}

int irq_assert(void)
{
	int i;
	i = -1;
	if( intrrupt_bits ) {
		for( i=0; i<32; i++ ) {
			if( intrrupt_bits & (1<<i) ) {
				break;
			}
		}
	}
	return i;
}

int irq_set( uint8_t irq )
{
	if( irq >= 32 ) {
		return -1;
	}
	intrrupt_bits |= (1<<irq);
	return 0;
}

void _info(const char *msg, ...);
bool psw_get_bit(uint32_t bit);

int irq_clr( uint8_t irq )
{
	uint32_t last = intrrupt_bits;
	if( irq >= 32 ) {
		return -1;
	}
	intrrupt_bits &= ~(1<<irq);
	if( intrrupt_bits != 0 ) {
		int ie_flag = psw_get_bit( PSW_I );
		_info("### ie: %d, last = 0x%08x  now = 0x%08x", ie_flag, last, intrrupt_bits);	// LJL
	}
	return 0;
}
