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
#include <stdio.h>
#include <stdint.h>

#include "nic.h"
#include "soc.h"

extern int tapdev_init(uint8_t *ip_addr,uint8_t *netmask,uint8_t *gateway);
extern char *tapdev_read(unsigned char *recv_buf,int *recv_len);
extern void tapdev_send(unsigned char *pkg_data,unsigned int len);

int nic_init(uint8_t *ip_addr,uint8_t *netmask,uint8_t *gateway)
{
	return tapdev_init(ip_addr, netmask, gateway);
}

int nic_uninit(void)
{
	return 0;
}

int in_addr_nic(uint32_t addr)
{
	if( addr >= ADDR_NIC_START && addr <= ADDR_NIC_STOP ) {
		return 1;
	}
	return 0;
}

static uint32_t reg_nic_ctr;
static uint32_t reg_nic_ridx;
static uint32_t reg_nic_widx;

int nic_write_word(uint32_t addr, uint32_t data)
{
	switch(addr) {
		case ADDR_NIC_CTR:
			reg_nic_ctr = data;
			return 0;
		case ADDR_NIC_RIDX:
			reg_nic_ridx = data;
			return 0;
		case ADDR_NIC_WIDX:
			reg_nic_widx = data;
			return 0;
		/*
		case ADDR_NIC_DAT:
			return 0;
		*/
		default:
			return -1;
	}
}

uint32_t nic_read_word(uint32_t addr)
{
	//uint32_t read_data = addr;
	switch(addr) {
		case ADDR_NIC_CTR:
			return 0;
		case ADDR_NIC_RIDX:
			return reg_nic_ridx;
		case ADDR_NIC_WIDX:
			return reg_nic_widx;
		/*
		case ADDR_NIC_DAT:
			return 0;
		*/
		default:
			return 0;
	}
}

