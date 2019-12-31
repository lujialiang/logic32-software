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

#include "disk0.h"
#include "soc.h"

static int disk0_readp (uint8_t *buff, uint32_t sector, uint16_t offset, uint16_t count);
static int disk0_writep (const uint8_t *buff, uint32_t sc);

static FILE *disk_fp = NULL;

static uint8_t disk0_read_buffer[DISK_BUFFER_SIZE];
static uint8_t disk0_write_buffer[DISK_BUFFER_SIZE];

static uint32_t reg_disk0_ctrl = 0;
static uint32_t reg_disk0_read_sct_num = 0;
static uint32_t reg_disk0_write_sct_num = 0;
static uint32_t reg_disk0_read_index = 0;
static uint32_t reg_disk0_write_index= 0;

int in_addr_disk0(uint32_t addr)
{
	if( addr >= ADDR_DISK0_START && addr <= ADDR_DISK0_STOP ) {
		return 1;
	}
	return 0;
}

int disk0_write_word(uint32_t addr, uint32_t data)
{
	if( disk_fp == NULL ) {
		return -1;
	}
	switch(addr) {
		case ADDR_DISK0_CTR:
			reg_disk0_ctrl = data;
			if( reg_disk0_ctrl & (1<<DISK0_RDP) ) {
				// read disk
				reg_disk0_ctrl &= ~(1<<DISK0_RDY);
				disk0_readp(disk0_read_buffer, reg_disk0_read_sct_num, 0, 512);
				reg_disk0_ctrl |= (1<<DISK0_RDY);
			}
			else if( reg_disk0_ctrl & (1<<DISK0_WRP) ) {
				// write disk
				reg_disk0_ctrl &= ~(1<<DISK0_RDY);
				disk0_writep(disk0_write_buffer, reg_disk0_write_sct_num);
				reg_disk0_ctrl |= (1<<DISK0_RDY);
			}
			return 0;
		case ADDR_DISK0_RSCT:
			reg_disk0_read_sct_num = data;
			return 0;
		case ADDR_DISK0_WSCT:
			reg_disk0_write_sct_num = data;
			return 0;
		case ADDR_DISK0_RIDX:
			reg_disk0_read_index = data;
			return 0;
		case ADDR_DISK0_WIDX:
			reg_disk0_write_index = data;
			return 0;
		case ADDR_DISK0_DAT:
			reg_disk0_write_index %= sizeof(disk0_write_buffer);
			disk0_write_buffer[reg_disk0_write_index++] = data;
			reg_disk0_write_index %= sizeof(disk0_write_buffer);
			return 0;
		default:
			return -1;
	}
}

uint32_t disk0_read_word(uint32_t addr)
{
	if( disk_fp == NULL ) {
		return 0;
	}
	uint32_t read_data;
	switch(addr) {
		case ADDR_DISK0_CTR:
			read_data = reg_disk0_ctrl;
			reg_disk0_ctrl &= ~(1<<DISK0_IF);	// always clean interrupt flag
			return read_data;
		case ADDR_DISK0_RSCT:
			return reg_disk0_read_sct_num;
		case ADDR_DISK0_WSCT:
			return reg_disk0_write_sct_num;
		case ADDR_DISK0_RIDX:
			return reg_disk0_read_index;
		case ADDR_DISK0_WIDX:
			return reg_disk0_write_index;
		case ADDR_DISK0_DAT:
			reg_disk0_read_index %= sizeof(disk0_read_buffer);
			read_data = disk0_read_buffer[reg_disk0_read_index++];
			reg_disk0_read_index %= sizeof(disk0_read_buffer);
			return read_data;
		default:
			return 0;
	}
}

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/
int disk0_init(const char *image_name)
{
	disk_fp = fopen(image_name, "rb+");
	if(disk_fp == NULL) {
		return -1;
	} 
	reg_disk0_ctrl = 0;
	reg_disk0_read_sct_num = 0;
	reg_disk0_write_sct_num = 0;
	reg_disk0_read_index = 0;
	reg_disk0_write_index= 0;
	return 0;
}

int disk0_uninit(void)
{
	if( disk_fp ) {
		fclose(disk_fp);
	}
	return 0;
}

/*-----------------------------------------------------------------------*/
/* Read partial sector                                                   */
/*-----------------------------------------------------------------------*/
static int disk0_readp (
	uint8_t *buff,		/* Pointer to the read buffer (NULL:Read bytes are forwarded to the stream) */
	uint32_t sector,	/* Sector number (LBA) */
	uint16_t offset,	/* Byte offset to read from (0..511) */
	uint16_t count		/* Number of bytes to read (ofs + cnt mus be <= 512) */
)
{
	if( !disk_fp ) {
		return -1;
	}
	if( !fseek(disk_fp,sector*512+offset,SEEK_SET) ) {
		if( !fread(buff, count, 1, disk_fp) ) {
			return -1;
		}
		return 0;
	}
	return -1;
}

/*-----------------------------------------------------------------------*/
/* Write partial sector                                                  */
/*-----------------------------------------------------------------------*/
static int disk0_writep (
	const uint8_t *buff,	/* Pointer to the bytes to be written (NULL:Initiate/Finalize sector write) */
	uint32_t sc			/* Number of bytes to send, Sector number (LBA) or zero */
)
{
	if( !disk_fp ) {
		return -1;
	}
	if( !fseek(disk_fp,sc*512,SEEK_SET) ) {
		if( !fwrite(buff,512,1,disk_fp) ) {
			return -1;
		}
		return 0;
	}
	return -1;
}
