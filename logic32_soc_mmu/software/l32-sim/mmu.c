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
#include "mmu.h"

void _info(const char *msg, ...);

uint32_t mr_tlb[64];
uint32_t tr_tlb[64];

bool mmu_en = false;
bool tlb_miss = false;

void mmu_init(void)
{
	int i;
	for(i=0;i<64;i++) {
		mr_tlb[i] = 0;
		tr_tlb[i] = 0;
	}
	mmu_en = false;
	tlb_miss = false;
}

int mmu_v2p(uint32_t va, uint32_t *pa)
{
	int i;
	uint32_t tmp;
	tlb_miss = true;
	if( mmu_en == false ) {
		*pa = va;
		return 0;
	}
	for(i=0;i<64;i++) {
		tmp = mr_tlb[i];
		if(tmp&1) {
			if( ((tmp>>1)&0xfffff) == (va>>12) ) {
				tlb_miss = false;
				*pa = ((tr_tlb[i] >> 4) << 12) | (va & 0xfff);
				return 0;
			}
		}
	}
	_info("tlb miss, addr = 0x%x", va);
	*pa = 0;
	return -1;
}
