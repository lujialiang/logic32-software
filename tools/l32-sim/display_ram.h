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

#ifndef DISPLAY_RAM_H_INCLUDE
#define DISPLAY_RAM_H_INCLUDE

#ifdef __cplusplus
extern "C" {
#endif

#define DISPLAY_RAM_BEGIN_ADDR (1<<25)
#define DISPLAY_RAM_SIZE	(1<<25)
#define DISPLAY_RAM_END_ADDR (DISPLAY_RAM_BEGIN_ADDR + DISPLAY_RAM_SIZE - 1)

void display_ram_init(void);
void display_ram_deinit(void);
int is_in_display_ram(uint32_t addr);
int dispay_ram_write_word(uint32_t addr, uint32_t dat);
uint32_t dispay_ram_read_word(uint32_t addr);
int dispay_ram_write_half(uint32_t addr, uint16_t dat);
uint16_t dispay_ram_read_half(uint32_t addr);

#ifdef __cplusplus
	}
#endif

#endif
