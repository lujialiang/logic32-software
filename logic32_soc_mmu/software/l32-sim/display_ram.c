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
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "display_ram.h"
//#include "screen.h"
#include "config.h"

#ifdef USE_SHARE_MEMORY

#define SHM_KEY 29

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

static uint8_t *display_ram = NULL;
static int shmid;
#else
static uint8_t display_ram[DISPLAY_RAM_SIZE];
#endif

void display_ram_init(void)
{
#ifdef USE_SHARE_MEMORY
	if( (shmid = shmget(SHM_KEY, DISPLAY_RAM_SIZE, IPC_CREAT|0666)) == -1 ) {
		fprintf(stderr, "shmget error\n");
		exit(1);
	}
	if( (display_ram = shmat(shmid, 0, 0)) == (void *)-1 ) {
		fprintf(stderr, "shmat error\n");
		exit(1);
	}
#endif
	memset(display_ram, 0x55, DISPLAY_RAM_SIZE);
}

void display_ram_deinit(void)
{
}

int is_in_display_ram(uint32_t addr)
{
	if( addr >= DISPLAY_RAM_BEGIN_ADDR && addr <= DISPLAY_RAM_END_ADDR ) {
		return 1;
	}
	return 0;
}

int dispay_ram_write_word(uint32_t addr, uint32_t dat)
{
#if defined(VERIFY_ADDRESS)
	if( !is_in_display_ram(addr) ) {
		return -1;
	}
#endif
	uint32_t offset = (addr - DISPLAY_RAM_BEGIN_ADDR);
	uint32_t *p = (uint32_t *)(display_ram + offset);
	*p = dat;
	return 0;
}

uint32_t dispay_ram_read_word(uint32_t addr)
{
#if defined(VERIFY_ADDRESS)
	if( !is_in_display_ram(addr) ) {
		return 0;
	}
#endif
	uint32_t offset = (addr - DISPLAY_RAM_BEGIN_ADDR);
	uint32_t *p = (uint32_t *)(display_ram + offset);
	return *p;
}

int dispay_ram_write_half(uint32_t addr, uint16_t dat)
{
#if defined(VERIFY_ADDRESS)
	if( !is_in_display_ram(addr) ) {
		return -1;
	}
#endif
	uint32_t offset = (addr - DISPLAY_RAM_BEGIN_ADDR);
	uint16_t *p = (uint16_t *)(display_ram + offset);
	*p = dat;
	return 0;
}

uint16_t dispay_ram_read_half(uint32_t addr)
{
#if defined(VERIFY_ADDRESS)
	if( !is_in_display_ram(addr) ) {
		return 0;
	}
#endif
	uint32_t offset = (addr - DISPLAY_RAM_BEGIN_ADDR);
	uint16_t *p = (uint16_t *)(display_ram + offset);
	return *p;
}
