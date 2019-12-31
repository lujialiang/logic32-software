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
#include <string.h>
#include "rtc0.h"
#include "soc.h"

static time_t curr_time;
static struct tm curr_tm;

static uint32_t reg_rtc0_data;
static uint32_t reg_rtc0_ctrl;

static int rtc0_load(void);

int in_addr_rtc0(uint32_t addr)
{
	if( addr >= ADDR_RTC0_START && addr <= ADDR_RTC0_END ) {
		return 1;
	}
	return 0;
}

uint32_t rtc0_read_word(uint32_t addr)
{
	uint32_t read_data;
	switch(addr) {
		case ADDR_RTC0_DAT:
			reg_rtc0_data = rtc0_load();
			return reg_rtc0_data;
		case ADDR_RTC0_CTR:
			read_data = reg_rtc0_ctrl;
			reg_rtc0_ctrl &= ~(1<<RTC0_IF);	// always clearn interrupt flag
			return read_data;
		case ADDR_RTC0_YEAR:
			return curr_tm.tm_year + 1900;
		case ADDR_RTC0_MON:
			return curr_tm.tm_mon;
		case ADDR_RTC0_DAY:
			return curr_tm.tm_mday;
		case ADDR_RTC0_HOUR:
			return curr_tm.tm_hour;
		case ADDR_RTC0_MIN:
			return curr_tm.tm_min;
		case ADDR_RTC0_SEC:
			return curr_tm.tm_sec;
		case ADDR_RTC0_WEEK:
			return curr_tm.tm_wday;
		default:
			return 0;
	}
}

int rtc0_write_word(uint32_t addr, uint32_t data)
{
	if( addr == ADDR_RTC0_CTR ) {
		reg_rtc0_ctrl = data;
		if( reg_rtc0_ctrl & (1<<RTC0_LD) ) {
			reg_rtc0_data = rtc0_load();
		}
		return 0;
	}
	return -1;
}

void rtc0_init(void)
{
	curr_time = 0;
	memset(&curr_tm, 0, sizeof(struct tm));
	reg_rtc0_data = 0;
	reg_rtc0_ctrl = 0;
}

static int rtc0_load(void)
{
	struct tm *p;
	time(&curr_time);
	p = localtime(&curr_time);
	if( p ) {
		memcpy(&curr_tm, p, sizeof(struct tm));
		return 0;
	}
	return -1;
}

