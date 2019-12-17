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
#include <stdint.h>
#include "soc.h"
#include "mmu.h"

int in_addr_tlb(uint32_t addr)
{
	if( addr >= ADDR_TLB_BASE && addr < (ADDR_TLB_BASE + TLB_LIMIT) ) {
		return 1;
	}
	return 0;
}

int tlb_write(uint32_t addr, uint32_t data)
{
	int index;
	if( addr >= ADDR_TLB_BASE && addr < (ADDR_TLB_BASE + TLB_LIMIT) ) {
		index = (addr - ADDR_TLB_BASE)/4;
		if( index < 64 ) {
			mr_tlb[index] = data;
		}
		else {
			tr_tlb[index-64] = data;
		}
		return 0;
	}
	return -1;
}

uint32_t tlb_read(uint32_t addr)
{
	int index;
	uint32_t read_data;
	if( addr >= ADDR_TLB_BASE && addr < (ADDR_TLB_BASE + TLB_LIMIT) ) {
		index = (addr - ADDR_TLB_BASE)/4;
		if( index < 64 ) {
			read_data = mr_tlb[index];
		}
		else {
			read_data = tr_tlb[index-64];
		}
		return read_data;
	}
	return -1;
}
