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

#include "tmr0.h"
#include "soc.h"

static uint32_t reg_tmr0_cnt;
static uint32_t reg_tmr0_rld;
static uint32_t reg_tmr0_ctr;

int in_addr_tmr0(uint32_t addr)
{
	return (addr == ADDR_TMR0_CNT || addr == ADDR_TMR0_RLD || addr == ADDR_TMR0_CTR );
}

void tmr0_init(void)
{
	reg_tmr0_cnt = 0;
	reg_tmr0_rld = 0;
	reg_tmr0_ctr = 0;
}

int tmr0_write_word(uint32_t addr, uint32_t data)
{
	switch(addr) {
		case ADDR_TMR0_CNT:
			reg_tmr0_cnt = data;
			return 0;
		case ADDR_TMR0_RLD:
			reg_tmr0_rld = data;
			if( reg_tmr0_ctr & (1<<TMR0_DI )) {
				reg_tmr0_cnt = 0;
			}
			else {
				reg_tmr0_cnt = data;
			}
			return 0;
		case ADDR_TMR0_CTR:
			reg_tmr0_ctr = data;
			return 0;
		default:
			return -1;
	}
}

uint32_t tmr0_read_word(uint32_t addr)
{
	uint32_t ret;
	switch(addr) {
		case ADDR_TMR0_CNT:
			return reg_tmr0_cnt;
		case ADDR_TMR0_RLD:
			return reg_tmr0_rld;
		case ADDR_TMR0_CTR:
			ret = reg_tmr0_ctr;
			reg_tmr0_ctr &= ~(1<<TMR0_IF);	// alway clear IF flag
			return ret;
		default:
			return 0;
	}
}

void tmr0_cycle(void)
{
	if( reg_tmr0_ctr & (1<<TMR0_EN) ) {
		if( reg_tmr0_ctr & (1<<TMR0_DI )) {	// counting up
			if( reg_tmr0_cnt == reg_tmr0_rld ) {
				if( reg_tmr0_ctr & (1<<TMR0_IE) ) {
					reg_tmr0_ctr |= (1<<TMR0_IF);	// irq
				}
				reg_tmr0_cnt = 0;
			}
			else {
				reg_tmr0_cnt++;
			}
		}
		else {	// counting down
			if( reg_tmr0_cnt == 0 ) {
				if( reg_tmr0_ctr & (1<<TMR0_IE) ) {
					reg_tmr0_ctr |= (1<<TMR0_IF);	// irq
				}
				reg_tmr0_cnt = reg_tmr0_rld;
			}
			else {
				reg_tmr0_cnt--;
			}
		}
	}
}

int tmr0_check_irq(void)
{
	return ( (reg_tmr0_ctr & (1<<TMR0_IF)) != 0 );
}

void tmr0_clear_irq(void)
{
	reg_tmr0_ctr &= ~(1<<TMR0_IF);
}
