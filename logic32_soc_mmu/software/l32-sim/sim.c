/*-
 * Copyright (c) 2017, 2018, 2019 Lu JiaLiang
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
#include "isa.h"
#include "alu.h"
#include "cctest.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <sys/msg.h>
#include <errno.h>

#include "a_out.h"
#include "config.h"

#include "kbhit.h"
#include "kbd.h"
#include "disk0.h"
#include "display_ram.h"
#include "irq_ctrl.h"
#include "mmu.h"
#include "systick.h"
#include "tmr0.h"
#include "rtc0.h"
#include "random.h"
#include "tlb.h"
#include "fpu.h"
#include "nic.h"

#include "soc.h"
#include "pack_types.h"

#define	STDOUT	stdout
#define	STDERR	stdout

#define MSG_ENABALE

int mouse_x = 0;
int mouse_y = 0;

#ifdef MSG_ENABALE
#include "msg_common.h"
int msgid = -1;
const long int msgtype = 0; //注意1
bool msg_enable = false;
#endif

uint32_t code_segment_load_address = 0;
uint32_t code_segment_limit = 0;
uint32_t data_segment_load_address = 0;
uint32_t data_segment_limit = 0;

bool debug_enable = false;
bool dump_enable = false;
bool gui_enable = false;
bool rgb565_mode = false;
bool kbd_enable = false;
bool disk_enable = false;
bool nic_enable = false;
bool no_fpu = false;
char *disk_image_file_name = NULL;
bool memory_protect_enable = false;

#define N_TRACE 32
bool trace_enable = false;
uint32_t trace_buffer[2][N_TRACE];
uint32_t trace_index = 0;
uint32_t trace_cnt = 0;
bool trace_call = false;

#define N_GPR	32
#define N_FPR	32
#define MMU_EN	31	// bit31 of cr0

uint8_t memory[MEMORY_SIZE];

static int fetch_flag = 0;

static int lcd_width = -1;
static int lcd_height = -1;

static uint32_t UART0_DR;
static uint32_t UART0_SR;
static uint32_t UART0_CR;

uint32_t GPR[N_GPR];
uint32_t IR;
uint32_t PC;
uint32_t REG_CR0;	// cr0	cr0[31]: mmu en
uint32_t REG_SR;	// cr1
uint32_t EPC;		// cr4
uint32_t ESR;		// cr5
bool halt;
bool disk_init_done = false;
uint32_t FPR[N_FPR];

uint32_t brn_offset;
brn_func_t brn_func;

pid_t gui_pid = 0;

void gui_exit(void)
{
	if( gui_exit > 0 ) {
		//kill(gui_pid, SIGTSTP);
	}
}

bool wall_flag = false;
uint64_t run_circles = 0;

uint32_t get_file_size(const char *file_name) {
	struct stat buf;
	if( stat(file_name,&buf)<0 ) {
		return 0;
	}
	return (uint32_t)buf.st_size;
}

void error(const char *file, int line)
{
	fprintf(STDERR,"\e[031m");
	fprintf(STDERR,"error(%s,%d)",file, line);
	fprintf(STDERR,"\tPC(%08x), IR(%08x)\n",PC, IR);
	fprintf(STDERR,"\e[0m");
}

static void _warnning(const char *msg, ...)
{
	va_list ap; 
	va_start(ap,msg);
	fprintf(STDERR,"\e[032mwarnning: circles = %ld, ",run_circles);
	vfprintf(STDERR,msg,ap);
	fprintf(STDERR,"\e[0m\n");
	va_end(ap);
}

static void _error(const char *msg, ...)
{
	va_list ap; 
	va_start(ap,msg);
	fprintf(STDERR,"\e[031merror: circels = %ld, ", run_circles);
	vfprintf(STDERR,msg,ap);
	fprintf(STDERR,"\n\e[0m");
	va_end(ap);
}

void _info(const char *msg, ...)
{
	va_list ap; 
	va_start(ap,msg);
	fprintf(stdout,"\e[032minfo: circels = %ld, ", run_circles);
	vfprintf(stdout,msg,ap);
	fprintf(stdout,"\n\e[0m");
	va_end(ap);
}

void _debug_printf(const char*s,...)
{
	bool I = psw_get_bit( PSW_I );
	bool N = psw_get_bit( PSW_N );
	bool Z = psw_get_bit( PSW_Z );
	bool V = psw_get_bit( PSW_V );
	bool C = psw_get_bit( PSW_C );
	va_list ap;
	va_start(ap,s);
	fprintf(stdout,"PC(%08x) %d %d%d%d%d:\t IR(0x%08x)\t",PC, I, N, Z, V, C, IR);
	vfprintf(stdout,s,ap);
	fprintf(stdout,"\n");
	va_end(ap);
}
#define debug_printf(format, args...) if(debug_enable) _debug_printf(format, ##args)

int load_word(uint32_t addr, uint32_t *pdata)
{
#if 0
	if ( fetch_flag == 0 ) {
		_info("%s(addr: 0x%x)", __func__, addr);	// LJL
	}
#endif
	if(addr & 3) {
		if( fetch_flag ) {
			_error( "unalign fetch word at 0x%08x", addr );
			debug_enable = true;
			exit(1);
		}
		else {
			if( wall_flag ) {
				_warnning("unalign load word at 0x%08x", addr );
			}
		}
	}
	// main memory
	if( (addr & ~MEMORY_ADDR_MASK) == 0 ) {
		*pdata =  *((uint32_t *)&memory[addr]);
		return 0;
	}
	// io space
	switch( addr ) {
		case ADDR_UART0_DR:
			*pdata = UART0_DR;
			return 0;
		case ADDR_UART0_CR:
			*pdata = UART0_CR;
			return 0;
		case ADDR_UART0_SR:
			*pdata = UART0_SR;
			return 0;
		case ADDR_DMA_CH0_SR:
			*pdata = 0;
			return 0;
		case ADDR_DMA_CH0_CR:
			*pdata = 0;
			return 0;
		case ADDR_DMA_CH0_LR:
			*pdata = 0;
			return 0;
		case ADDR_SYSTICK_CTRL:
			*pdata = reg_systick_ctrl;
			return 0;
			break;
		case ADDR_SYSTICK_RELOAD:
			*pdata = reg_systick_reload;
			return 0;
		case ADDR_SYSTICK_COUNT:
			*pdata = reg_systick_count;
			return 0;
		case ADDR_RANDOM_NUMBER:
			*pdata = random_read();
			return 0;
		case ADDR_MOUSE_X:
			*pdata = mouse_x;
			return 0;
		case ADDR_MOUSE_Y:
			*pdata = mouse_y;
			return 0;
		default:
			if( is_in_display_ram(addr) ) {
				*pdata = dispay_ram_read_word(addr);
				return 0;
			}
			else if( in_addr_tmr0(addr) ) {
				*pdata = tmr0_read_word(addr);
				return 0;
			}
			else if( in_addr_rtc0(addr) ) {
				*pdata = rtc0_read_word(addr);
				return 0;
			}
			else if( in_addr_disk0(addr) ) {
				if( disk_enable ) {
					*pdata = disk0_read_word(addr);
				}
				else {
					*pdata = 0;
				}
				return 0;
			}
			else if( in_addr_nic(addr) ) {
				if( nic_enable ) {
					*pdata = nic_read_word(addr);
				}
				else {
					*pdata = 0;
				}
				return 0;
			}
			else if( in_addr_tlb(addr) ) {
				*pdata = tlb_read(addr);
				return 0;
			}
			else {
				_error("@ %s: %d, load_word(0x%08x), pc = 0x%08x", __FILE__, __LINE__, addr, PC );
				irq_set(IRQ_BUS_ERROR);
				return -1;
			}
	}
	_error("%s: %d, fatal error!!!", __FILE__, __LINE__);
	exit(1);
	return -1;
}

int store_word(uint32_t addr, uint32_t data)
{
#if 0
	_info("%s(addr: 0x%x, data: 0x%x)", __func__, addr, data);	// LJL
#endif
	if( wall_flag && (addr & 3) ) {
		_warnning("unalign store word at 0x%08x", addr );
	}
	if( addr < code_segment_limit ) {
		irq_set(IRQ_HARD_FAULT);
		_error("overwrite code space,\ncircles = %ld, addr = 0x%x, data = 0x%x, PC= 0x%x, IR = 0x%x",\
			run_circles, addr, data, PC, IR );
		return -1;
	}
	// main memory
	if( (addr & ~MEMORY_ADDR_MASK) == 0 ) {
		if (memory_protect_enable == true ) {
			if ( (addr & ~0xffff ) == 0 ) {
				_error("@ %s: %d, store_word(0x%08x, 0x%08x), pc = 0x%08x", __FILE__, __LINE__, addr, data, PC );
				irq_set(IRQ_BUS_ERROR);
				return -1;
			}
			else {
				*((uint32_t *)&memory[addr]) = data;
			}
		}
		else {
			*((uint32_t *)&memory[addr]) = data;
		}
		return 0;
	}
	// io space
	switch( addr ) {
		// uart
		case ADDR_UART0_DR:
			kbd_writechar(data);
			UART0_SR |= UART0_TX_DONE;	// set tx done bit
			break;
		case ADDR_UART0_CR:
			UART0_CR = data;
			break;
		case ADDR_UART0_SR:
			// read only
			break;
		// dma
		case ADDR_DMA_CH0_SR:
			break;
		case ADDR_DMA_CH0_CR:
			break;
		case ADDR_DMA_CH0_LR:
			break;
		// systick
		case ADDR_SYSTICK_CTRL:
			reg_systick_ctrl = data;
			break;
		case ADDR_SYSTICK_RELOAD:
			reg_systick_reload = data;
			break;
		case ADDR_SYSTICK_COUNT:
			reg_systick_count = data;
			break;
		case ADDR_DEBUG_PORT0:
			_info("debug port0 <= 0x%08x (d: %d)", data, data);
			break;
		case ADDR_DEBUG_PORT1:
			_info("debug port1 <= 0x%08x (d: %d)", data, data);
			break;
		case ADDR_DEBUG_PORT2:
			_info("debug port2 <= 0x%08x (d: %d)", data, data);
			break;
		case ADDR_DEBUG_PORT3:
			_info("debug port3 <= 0x%08x (d: %d)", data, data);
			break;
		default:
			if( is_in_display_ram(addr) ) {
				dispay_ram_write_word(addr, data);
			}
			else if( in_addr_tmr0(addr) ) {
				tmr0_write_word(addr, data);
			}
			else if( in_addr_rtc0(addr) ) {
				rtc0_write_word(addr, data);
			}
			else if( in_addr_disk0(addr) ) {
				if( disk_enable ) {
					disk0_write_word(addr, data);
				}
			}
			else if( in_addr_nic(addr) ) {
				if( nic_enable ) {
					nic_write_word(addr, data);
				}
			}
			else if( in_addr_tlb(addr) ) {
				tlb_write(addr, data);
			}
			else {
				_error("@ %s: %d, store_word(0x%08x, 0x%08x), pc = 0x%08x", __FILE__, __LINE__, addr, data, PC );
				irq_set(IRQ_BUS_ERROR);
				return -1;
			}
	}
	return 0;
}

int store_half(uint32_t addr, uint16_t data)
{
	if( wall_flag && (addr & 1) ) {
		_warnning("unalign store halt at 0x%08x", addr );
	}
	if( addr < code_segment_limit ) {
		_error("overwrite code space,\ncircles = %ld, addr = 0x%x, data = 0x%x, PC = 0x%x, IR = 0x%x", run_circles, addr, data, PC, IR );
		irq_set(IRQ_HARD_FAULT);
		return -1;
	}
	if( is_in_display_ram(addr) ) {
		dispay_ram_write_half(addr, data);
		return 0;
	}
	else if( addr & ~MEMORY_ADDR_MASK ) {
		_error("@ %s: %d, store_half(0x%08x, 0x%04x), pc = 0x%08x", __FILE__, __LINE__, addr, data, PC );
		irq_set(IRQ_BUS_ERROR);
		return -1;
	}
	else {
		// main memory
		if (memory_protect_enable == true ) {
			if ( (addr & ~0xffff ) == 0 ) {
				_error("@ %s: %d, store_half(0x%08x, 0x%04x), pc = 0x%08x", __FILE__, __LINE__, addr, data, PC );
				irq_set(IRQ_BUS_ERROR);
				return -1;
			}
			else {
				*((uint16_t *)&memory[addr]) = data;
			}
		}
		else {
			*((uint16_t *)&memory[addr]) = data;
		}
		return 0;
	}
}

int load_half(uint32_t addr, uint16_t *pdata)
{
	if( wall_flag && (addr & 1) ) {
		_warnning("unalign load half at 0x%08x", addr );
	}
	if( is_in_display_ram(addr) ) {
		*pdata = dispay_ram_read_half(addr);
		return 0;
	}
	else if( addr & ~MEMORY_ADDR_MASK ) {
		_error("@ %s: %d, load_half(0x%08x), pc = 0x%08x", __FILE__, __LINE__, addr, PC );
		irq_set(IRQ_BUS_ERROR);
		return -1;
	}
	else {
		*pdata = *((uint16_t *)&memory[addr]);
		return 0;
	}
	_error("%s, %d: fatal error!", __FILE__, __LINE__ );
	exit(1);
	return -1;
}

int load_byte(uint32_t addr, uint8_t *pdata)
{
	// main memory
	if( (addr & ~MEMORY_ADDR_MASK) == 0 ) {
		addr &= MEMORY_ADDR_MASK;
		*pdata = memory[addr];
		return 0;
	}
	// io space
	switch( addr ) {
		case ADDR_UART0_DR:
			*pdata = UART0_DR;
			UART0_SR &= ~UART0_RX_DONE;	// clearn rx done bit
			return 0;
		case ADDR_UART0_CR:
			*pdata = UART0_CR;
			return 0;
		case ADDR_UART0_SR:
			*pdata = UART0_SR;
			return 0;
		default:
			if( in_addr_disk0(addr) ) {
				if( disk_enable ) {
					uint32_buffer_t tmp;
					tmp.value = disk0_read_word(addr&~3);
					*pdata = tmp.buff[addr&3];
				}
				else {
					*pdata = 0;
				}
				return 0;
			}
			_error("@ %s: %d, load_byte(0x%08x), pc = 0x%08x", __FILE__, __LINE__, addr, PC );
			irq_set(IRQ_BUS_ERROR);
			return -1;
	}
}

int store_byte(uint32_t addr, uint8_t data)
{
	if( addr < code_segment_limit ) {
		_error("overwrite code space, circles = %ld, addr = 0x%x, data = 0x%x, PC = 0x%x, IR = 0x%x", run_circles, addr, data, PC, IR );
		irq_set(IRQ_HARD_FAULT);
	}
	// main memory
	if( (addr & ~MEMORY_ADDR_MASK) == 0 ) {
		if (memory_protect_enable == true ) {
			if ( (addr & ~0xffff ) == 0 ) {
				_error("@ %s: %d, store_byte(0x%08x, 0x%02x), pc = 0x%08x", __FILE__, __LINE__, addr, data, PC );
				irq_set(IRQ_BUS_ERROR);
				return -1;
			}
			else {
				addr &= MEMORY_ADDR_MASK;
				memory[addr] = data;
			}
		}
		else {
			addr &= MEMORY_ADDR_MASK;
			memory[addr] = data;
		}
		return 0;
	}
	// io space
	switch( addr ) {
		case ADDR_UART0_DR:
			kbd_writechar(data);
			UART0_SR |= UART0_TX_DONE;	// set tx done bit
			return 0;
		case ADDR_UART0_CR:
			UART0_CR = (UART0_CR & 0xffffff00) | data;
			return 0;
		case ADDR_UART0_SR:
			// read only
			return 0;
		default:
			_error("@ %s: %d, store_byte(0x%08x, 0x%02x), pc = 0x%08x", __FILE__, __LINE__, addr, data, PC );
			irq_set(IRQ_BUS_ERROR);
			return -1;
	}
}

static void on_exception(int irq, uint32_t vector)
{
	irq_clr(irq);
	ESR = REG_SR;
	psw_set_bit(PSW_I, false);
	psw_set_bit(PSW_E, false);
	EPC = PC;
	PC = vector;
}

static void on_undefined_instruction(char *file, int line, uint32_t ir)
{
	_info("@ %s, %d: undefined instrution! pc: 0x%08x, ir: 0x%08x", file, line, PC, ir);
	irq_set(IRQ_UNDF);
}

void reset(void)
{
	int i;
	for(i=0; i<N_GPR; i++) {
		GPR[i] = 0;
	}
	for(i=0; i<N_FPR; i++) {
		FPR[i] = 0;
	}
	PC = 0;
	REG_CR0 = 0;
	REG_SR = 0;
	IR = 0;
	halt = false;
	EPC = 0;
	ESR = 0;
}

int step(void)
{
	uint8_t rs_idx, rs2_idx, rd_idx;
	uint32_t rs, rs2, rd;
	uint32_t immed;
	uint32_t immed_sex;
	uint32_t mem_offset;
	uint32_t va, pa;
	uint32_t tmp32;
	uint16_t tmp16;
	uint8_t tmp8;
	// fetch
	fetch_flag = 1;
	if( mmu_v2p(PC, &pa) != 0 ) {
		irq_set(IRQ_ITLB_MISS);
		return -1;
	}
	if( load_word(pa, &IR) != 0 ) {
		return -1;
	}
	if( trace_enable ) {
		if( trace_call == false || (IR & 0xc0000000) == OP_JAL || (IR>>26&0x1f) == OP_JALR) { // || ((IR>>26)&0x1f) == OP_JMPR ) {
			trace_buffer[0][trace_index] = PC;
			trace_buffer[1][trace_index] = IR;
			trace_index++;
			trace_index %= N_TRACE;
			if( trace_cnt <= N_TRACE ) {
				trace_cnt++;
			}
		}
	}
	fetch_flag = 0;
	// decode & execute
	GPR[0] = 0;
	rd_idx = (IR>>21)&0x1f;
	rs_idx = (IR>>16)&0x1f;
	rs2_idx = (IR>>11)&0x1f;
	rs = GPR[rs_idx];
	rs2 = GPR[rs2_idx];
	rd = GPR[rd_idx];
	immed_sex = immed = (IR & 0xffff);
	if( immed & 0x8000 ) {
		immed_sex |= 0xffff0000;
	}
	mem_offset = immed_sex;	// LJL
	if( IR & 0x80000000 ) {
		uint32_t jmp_target = (IR & 0x3fffffff) << 2;
		if( (IR & 0xc0000000) == OP_AJMP ) {
			debug_printf("ajmp %08x", jmp_target);
			PC = jmp_target;
			return 0;
		}
		debug_printf("jal %08x", jmp_target);
		GPR[LR_REG_IDX] = PC + 4;
		PC = (IR & 0x3fffffff) << 2;
		return 0;
	}
	switch(IR>>26&0x1f)
	{
		case OP_ALUR:
			switch( IR&0x1f ) {
				case ALUR_ADD:
					debug_printf("add r%d, r%d, r%d", rd_idx, rs_idx, rs2_idx);
					GPR[rd_idx] = alu_add(rs, rs2, 0);
					PC += 4;
					return 0;
				case ALUR_ADC:
					debug_printf("adc r%d, r%d, r%d", rd_idx, rs_idx, rs2_idx);
					GPR[rd_idx] = alu_add(rs, rs2, psw_get_bit(PSW_C));
					PC += 4;
					return 0;
				case ALUR_SUB:
					debug_printf("sub r%d, r%d, r%d", rd_idx, rs_idx, rs2_idx);
					GPR[rd_idx] = alu_sub(rs, rs2, 0);
					PC += 4;
					return 0;
				case ALUR_SBC:
					debug_printf("sbc r%d, r%d, r%d", rd_idx, rs_idx, rs2_idx);
					GPR[rd_idx] = alu_sub(rs, rs2, psw_get_bit(PSW_C));
					PC += 4;
					return 0;
				case ALUR_AND:
					debug_printf("and r%d, r%d, r%d", rd_idx, rs_idx, rs2_idx);
					GPR[rd_idx] = alu_logic(rs, rs2, alu_op_and);
					PC += 4;
					return 0;
				case ALUR_ORR:
					debug_printf("orr r%d, r%d, r%d", rd_idx, rs_idx, rs2_idx);
					GPR[rd_idx] = alu_logic(rs, rs2, alu_op_or);
					PC += 4;
					return 0;
				case ALUR_XOR:
					debug_printf("xor r%d, r%d, r%d", rd_idx, rs_idx, rs2_idx);
					GPR[rd_idx] = alu_logic(rs, rs2, alu_op_xor);
					PC += 4;
					return 0;
				case ALUR_LSL:
					debug_printf("lsl r%d, r%d, r%d", rd_idx, rs_idx, rs2_idx);
					GPR[rd_idx] = alu_shift(rs, alu_op_lsl, (rs2&0x1f));
					PC += 4;
					return 0;
				case ALUR_LSR:
					debug_printf("lsr r%d, r%d, r%d", rd_idx, rs_idx, rs2_idx);
					GPR[rd_idx] = alu_shift(rs, alu_op_lsr, (rs2&0x1f));
					PC += 4;
					return 0;
				case ALUR_ASR:
					debug_printf("asr r%d, r%d, r%d", rd_idx, rs_idx, rs2_idx);
					GPR[rd_idx] = alu_shift(rs, alu_op_asr, (rs2&0x1f));
					PC += 4;
					return 0;
				case ALUR_MUL:
					debug_printf("mul r%d, r%d, r%d", rd_idx, rs_idx, rs2_idx);
					GPR[rd_idx] = alu_multiplier(rs, rs2, alu_op_mul);
					PC += 4;
					return 0;
				case ALUR_DIV:
					debug_printf("div r%d, r%d, r%d", rd_idx, rs_idx, rs2_idx);
					if( rs2 == 0 ) {
						irq_set(IRQ_DIV_BY_ZERO);
						return -1;
					}
					else {
						GPR[rd_idx] = alu_multiplier(rs, rs2, alu_op_div);
						PC += 4;
						return 0;
					}
				case ALUR_MOD:
					debug_printf("mod r%d, r%d, r%d", rd_idx, rs_idx, rs2_idx);
					if( rs2 == 0 ) {
						irq_set(IRQ_DIV_BY_ZERO);
						return -1;
					}
					else {
						GPR[rd_idx] = alu_multiplier(rs, rs2, alu_op_mod);
						PC += 4;
						return 0;
					}
				case ALUR_MULU:
					debug_printf("mulu r%d, r%d, r%d", rd_idx, rs_idx, rs2_idx);
					GPR[rd_idx] = alu_multiplier(rs, rs2, alu_op_mulu);
					PC += 4;
					return 0;
				case ALUR_DIVU:
					debug_printf("divu r%d, r%d, r%d", rd_idx, rs_idx, rs2_idx);
					if( rs2 == 0 ) {
						irq_set(IRQ_DIV_BY_ZERO);
						return -1;
					}
					else {
						GPR[rd_idx] = alu_multiplier(rs, rs2, alu_op_divu);
						PC += 4;
						return 0;
					}
				case ALUR_MODU:
					debug_printf("modu r%d, r%d, r%d", rd_idx, rs_idx, rs2_idx);
					if( rs2 == 0 ) {
						irq_set(IRQ_DIV_BY_ZERO);
						return -1;
					}
					else {
						GPR[rd_idx] = alu_multiplier(rs, rs2, alu_op_modu);
						PC += 4;
						return 0;
					}
				default:
					on_undefined_instruction(__FILE__, __LINE__, IR);
					return -1;
			}
			on_undefined_instruction(__FILE__, __LINE__, IR);
			return -1;

		case OP_FPU:
			if( no_fpu == true ) {
				on_undefined_instruction(__FILE__, __LINE__, IR);
				return -1;
			}
			switch( IR&0x1f ) {
				case FPU_ADDS:
					debug_printf("add.s f%d, f%d, f%d", rd_idx, rs_idx, rs2_idx);
					FPR[rd_idx] = fpu_single( FPR[rs_idx], FPR[rs2_idx], fpu_op_add_s);
					PC += 4;
					return 0;
				case FPU_SUBS:
					debug_printf("sub.s f%d, f%d, f%d", rd_idx, rs_idx, rs2_idx);
					FPR[rd_idx] = fpu_single( FPR[rs_idx], FPR[rs2_idx], fpu_op_sub_s);
					PC += 4;
					return 0;
				case FPU_MULS:
					debug_printf("mul.s f%d, f%d, f%d", rd_idx, rs_idx, rs2_idx);
					FPR[rd_idx] = fpu_single( FPR[rs_idx], FPR[rs2_idx], fpu_op_mul_s );
					PC += 4;
					return 0;
				case FPU_DIVS:
					debug_printf("div.s f%d, f%d, f%d", rd_idx, rs_idx, rs2_idx);
					FPR[rd_idx] = fpu_single( FPR[rs_idx], FPR[rs2_idx], fpu_op_div_s );
					PC += 4;
					return 0;
				case FPU_ADDD:
					debug_printf("add.d f%d, f%d, f%d", rd_idx, rs_idx, rs2_idx);
					{
						uint64_t a1, a2, b1, b2;
						uint64_t a, b, c;
						a1 = FPR[rs_idx];
						a2 = FPR[(rs_idx+1)&0x1f];
						a = (a2 << 32) | a1; 
						b1 = FPR[rs2_idx];
						b2 = FPR[(rs2_idx+1)&0x1f];
						b = (b2 << 32) | b1; 
						c = fpu_double(a, b, fpu_op_add_d);
						FPR[rd_idx] = c & 0xffffffff;
						FPR[(rd_idx+1)&0x1f] = (c>>32) & 0xffffffff;
					}
					PC += 4;
					return 0;
				case FPU_SUBD:
					debug_printf("sub.d f%d, f%d, f%d", rd_idx, rs_idx, rs2_idx);
					{
						uint64_t a1, a2, b1, b2;
						uint64_t a, b, c;
						a1 = FPR[rs_idx];
						a2 = FPR[(rs_idx+1)&0x1f];
						a = (a2 << 32) | a1; 
						b1 = FPR[rs2_idx];
						b2 = FPR[(rs2_idx+1)&0x1f];
						b = (b2 << 32) | b1; 
						c = fpu_double(a, b, fpu_op_sub_d);
						FPR[rd_idx] = c & 0xffffffff;
						FPR[(rd_idx+1)&0x1f] = (c>>32) & 0xffffffff;
					}
					PC += 4;
					return 0;
				case FPU_MULD:
					debug_printf("mul.d f%d, f%d, f%d", rd_idx, rs_idx, rs2_idx);
					{
						uint64_t a1, a2, b1, b2;
						uint64_t a, b, c;
						a1 = FPR[rs_idx];
						a2 = FPR[(rs_idx+1)&0x1f];
						a = (a2 << 32) | a1; 
						b1 = FPR[rs2_idx];
						b2 = FPR[(rs2_idx+1)&0x1f];
						b = (b2 << 32) | b1; 
						c = fpu_double(a, b, fpu_op_mul_d);
						FPR[rd_idx] = c & 0xffffffff;
						FPR[(rd_idx+1)&0x1f] = (c>>32) & 0xffffffff;
					}
					PC += 4;
					return 0;
				case FPU_DIVD:
					debug_printf("div.d f%d, f%d, f%d", rd_idx, rs_idx, rs2_idx);
					{
						uint64_t a1, a2, b1, b2;
						uint64_t a, b, c;
						a1 = FPR[rs_idx];
						a2 = FPR[(rs_idx+1)&0x1f];
						a = (a2 << 32) | a1; 
						b1 = FPR[rs2_idx];
						b2 = FPR[(rs2_idx+1)&0x1f];
						b = (b2 << 32) | b1; 
						c = fpu_double(a, b, fpu_op_div_d);
						FPR[rd_idx] = c & 0xffffffff;
						FPR[(rd_idx+1)&0x1f] = (c>>32) & 0xffffffff;
					}
					PC += 4;
					return 0;
				case FPU_MOVS:
					debug_printf("mov.s f%d, f%d", rd_idx, rs_idx);
					FPR[rd_idx] = FPR[rs_idx];
					PC += 4;
					return 0;
				case FPU_MOVD:
					debug_printf("mov.d f%d, f%d", rd_idx, rs_idx);
					FPR[rd_idx] = FPR[rs_idx];
					FPR[(rd_idx+1)&0x1f] = FPR[(rs_idx+1)&0x1f];
					PC += 4;
					return 0;
				case FPU_CVDF:
					debug_printf("cvdf f%d, f%d", rd_idx, rs_idx);
					{
						uint64_t a1, a2, a;
						double *p1;
						uint32_t *p2;
						float f;
						a1 = FPR[rs_idx];
						a2 = FPR[(rs_idx+1)&0x1f];
						a = (a2 << 32) | a1; 
						p1 = (double *)&a;
						f = *p1;
						p2 = (uint32_t *)&f;
						FPR[rd_idx] = *p2;
					}
					PC += 4;
					return 0;
				case FPU_CVFD:
					debug_printf("cvfd f%d, f%d", rd_idx, rs_idx);
					{
						uint32_t a;
						float *p1;
						double d;
						uint64_t *p2;
						a = FPR[rs_idx];
						p1 = (float *)&a;
						d = (double)*p1;
						p2 = (uint64_t *)&d;
						FPR[rd_idx] = *p2&0xffffffff;
						FPR[(rd_idx+1)&0x1f] = (*p2>>32)&0xffffffff;
					}
					PC += 4;
					return 0;
				case FPU_CVWD:
					debug_printf("cvwd f%d, f%d", rd_idx, rs_idx);
					{
						uint32_t a;
						double d;
						uint64_t *p1;
						a = FPR[rs_idx];
						d = (double)a;
						p1 = (uint64_t *)&d;
						FPR[rd_idx] = *p1&0xffffffff;
						FPR[(rd_idx+1)&0x1f] = (*p1>>32)&0xffffffff;
					}
					PC += 4;
					return 0;
				case FPU_CVDW:
					debug_printf("cvdw f%d, f%d, f%d", rd_idx, rs_idx);
					{
						uint64_t a1, a2;
						uint64_t a;
						double *p1;
						uint32_t t;
						a1 = FPR[rs_idx];
						a2 = FPR[(rs_idx+1)&0x1f];
						a = (a2 << 32) | a1;
						p1 = (double *)&a;
						t = (uint32_t)*p1;
						FPR[rd_idx] = t;
					}
					PC += 4;
					return 0;
				case FPU_NEGD:
					debug_printf("neg.d f%d, f%d, f%d", rd_idx, rs_idx);
					{
						uint64_t a1, a2;
						uint64_t a;
						double *p1;
						uint64_t *p2;
						double val_double;
						a1 = FPR[rs_idx];
						a2 = FPR[(rs_idx+1)&0x1f];
						a = (a2 << 32) | a1;
						p1 = (double *)&a;
						val_double = -(*p1);
						p2 = (uint64_t *)&val_double;
						FPR[rd_idx] = *p2&0xffffffff;
						FPR[(rd_idx+1)&0x1f] = (*p2>>32)&0xffffffff;
					}
					PC += 4;
					return 0;
				case FPU_NEGS:
					debug_printf("neg.s f%d, f%d, f%d", rd_idx, rs_idx);
					{
						uint32_t a;
						float *p1;
						uint32_t *p2;
						float val_float;
						a = FPR[rs_idx];
						p1 = (float *)&a;
						val_float = -(*p1);
						p2 = (uint32_t *)&val_float;
						FPR[rd_idx] = *p2;
					}
					PC += 4;
					return 0;
				default:
					on_undefined_instruction(__FILE__, __LINE__, IR);
					return -1;
			}
			on_undefined_instruction(__FILE__, __LINE__, IR);
			return -1;
		case OP_FPU_MISC:
			if( no_fpu == true ) {
				on_undefined_instruction(__FILE__, __LINE__, IR);
				return -1;
			}
			switch( IR&0xf ) {
				case FPU_MISC_MFC1:
					debug_printf("mfc1 r%d, f%d", rd_idx, rs_idx);
					GPR[rd_idx] = FPR[rs_idx];
					PC += 4;
					return 0;
				case FPU_MISC_MTC1:
					debug_printf("mtc1 r%d, f%d", rd_idx, rs_idx);
					FPR[rs_idx] = GPR[rd_idx];
					PC += 4;
					return 0;
				case FPU_MISC_MFC1_D:
					debug_printf("mfc1.d r%d, f%d", rd_idx, rs_idx);
					GPR[rd_idx] = FPR[rs_idx];
					GPR[(rd_idx+1)&0x1f] = FPR[(rs_idx+1)&0x1f];
					PC += 4;
					return 0;
				case FPU_MISC_MTC1_D:
					debug_printf("mtc1.d r%d, f%d", rd_idx, rs_idx);
					FPR[rs_idx] = GPR[rd_idx];
					FPR[(rs_idx+1)&0x1f] = GPR[(rd_idx+1)&0x1f];
					PC += 4;
					return 0;
				case FPU_MISC_CMP_EQD:
					debug_printf("c.eq.d f%d, f%d", rd_idx, rs_idx);
					if( FPR[rs_idx] == FPR[rd_idx] && FPR[(rs_idx+1)&0x1f] == FPR[(rd_idx+1)&0x1f] ) {
						psw_set_bit(PSW_FLT, true);
					}
					else {
						psw_set_bit(PSW_FLT, false);
					}
					PC += 4;
					return 0;
				case FPU_MISC_CMP_LED:
					debug_printf("c.le.d f%d, f%d", rd_idx, rs_idx);
					{
						uint64_t a1, a2, b1, b2;
						uint64_t a, b;
						double *p1, *p2;
						a1 = FPR[rd_idx];
						a2 = FPR[(rd_idx+1)&0x1f];
						a = (a2 << 32) | a1;
						b1 = FPR[rs_idx];
						b2 = FPR[(rs_idx+1)&0x1f];
						b = (b2 << 32) | b1;
						p1 = (double *)&a;
						p2 = (double *)&b;
						if( *p1 <= *p2 ) {
							psw_set_bit(PSW_FLT, true);
						}
						else {
							psw_set_bit(PSW_FLT, false);
						}
					}
					PC += 4;
					return 0;
				case FPU_MISC_CMP_LTD:
					debug_printf("c.lt.d f%d, f%d", rd_idx, rs_idx);
					{
						uint64_t a1, a2, b1, b2;
						uint64_t a, b;
						double *p1, *p2;
						a1 = FPR[rd_idx];
						a2 = FPR[(rd_idx+1)&0x1f];
						a = (a2 << 32) | a1;
						b1 = FPR[rs_idx];
						b2 = FPR[(rs_idx+1)&0x1f];
						b = (b2 << 32) | b1;
						p1 = (double *)&a;
						p2 = (double *)&b;
						if( *p1 < *p2 ) {
							psw_set_bit(PSW_FLT, true);
						}
						else {
							psw_set_bit(PSW_FLT, false);
						}
					}
					PC += 4;
					return 0;
				case FPU_MISC_CMP_EQS:
					debug_printf("c.eq.s f%d, f%d", rd_idx, rs_idx);
					{
						uint32_t a, b;
						float *p1, *p2;
						a = FPR[rd_idx];
						b = FPR[rs_idx];
						p1 = (float *)&a;
						p2 = (float *)&b;
						if( *p1 == *p2 ) {
							psw_set_bit(PSW_FLT, true);
						}
						else {
							psw_set_bit(PSW_FLT, false);
						}
					}
					PC += 4;
					return 0;
				case FPU_MISC_CMP_LES:
					debug_printf("c.le.s f%d, f%d", rd_idx, rs_idx);
					{
						uint32_t a, b;
						float *p1, *p2;
						a = FPR[rd_idx];
						b = FPR[rs_idx];
						p1 = (float *)&a;
						p2 = (float *)&b;
						if( *p1 <= *p2 ) {
							psw_set_bit(PSW_FLT, true);
						}
						else {
							psw_set_bit(PSW_FLT, false);
						}
					}
					PC += 4;
					return 0;
				case FPU_MISC_CMP_LTS:
					debug_printf("c.lt.s f%d, f%d", rd_idx, rs_idx);
					{
						uint32_t a, b;
						float *p1, *p2;
						a = FPR[rd_idx];
						b = FPR[rs_idx];
						p1 = (float *)&a;
						p2 = (float *)&b;
						if( *p1 < *p2 ) {
							psw_set_bit(PSW_FLT, true);
						}
						else {
							psw_set_bit(PSW_FLT, false);
						}
					}
					PC += 4;
					return 0;
				case FPU_MISC_BC1F:
					brn_offset = ((IR>>4)&0x3fffff)<<2;
					if( brn_offset & 0x200000 ) {
						brn_offset |= 0xffc00000;
					}
					debug_printf("bc1f 0x%08x", brn_offset);
					if( psw_get_bit(PSW_FLT) == false ) {
						PC += brn_offset;
					}
					else {
						PC += 4;
					}
					return 0;
				case FPU_MISC_BC1T:
					brn_offset = ((IR>>4)&0x3fffff)<<2;
					if( brn_offset & 0x200000 ) {
						brn_offset |= 0xffc00000;
					}
					debug_printf("bc1t 0x%08x", brn_offset);
					if( psw_get_bit(PSW_FLT) == true ) {
						PC += brn_offset;
					}
					else {
						PC += 4;
					}
					return 0;
				default:
					on_undefined_instruction(__FILE__, __LINE__, IR);
					return -1;
			}
			on_undefined_instruction(__FILE__, __LINE__, IR);
			return -1;
		case OP_LDD:
			debug_printf("l.d f%d, 0x%04x(r%d)", rd_idx, mem_offset, rs_idx);
			va = rs + mem_offset;
			if( mmu_v2p(va, &pa) != 0 ) {
				irq_set(IRQ_DTLB_MISS);
				return -1;
			}
			if( load_word(pa, &FPR[rd_idx]) != 0 ) {
				return -1;
			}
			va = rs + mem_offset + 4;
			if( mmu_v2p(va, &pa) != 0 ) {
				irq_set(IRQ_DTLB_MISS);
				return -1;
			}
			if( load_word(pa, &FPR[(rd_idx+1)&0x1f]) != 0 ) {
				return -1;
			}
			PC += 4;
			return 0;
		case OP_LDF:
			debug_printf("l.f f%d, 0x%04x(r%d)", rd_idx, mem_offset, rs_idx);
			va = rs + mem_offset;
			if( mmu_v2p(va, &pa) != 0 ) {
				irq_set(IRQ_DTLB_MISS);
				return -1;
			}
			if( load_word(pa, &FPR[rd_idx] ) != 0 ) {
				return -1;
			}
			PC += 4;
			return 0;
		case OP_STD:
			debug_printf("s.d f%d, 0x%04x(r%d)", rd_idx, mem_offset, rs_idx);
			va = rs + mem_offset;
			if( mmu_v2p(va, &pa) != 0 ) {
				irq_set(IRQ_DTLB_MISS);
				return -1;
			}
			if( store_word(pa, FPR[rd_idx]) != 0 ) {
				return -1;
			}
			va = rs + mem_offset + 4;
			if( mmu_v2p(va, &pa) != 0 ) {
				irq_set(IRQ_DTLB_MISS);
				return -1;
			}
			if( store_word(pa, FPR[(rd_idx+1)&0x1f]) != 0 ) {
				return -1;
			}
			PC += 4;
			return 0;
		case OP_STF:
			debug_printf("s.f f%d, 0x%04x(r%d)", rd_idx, mem_offset, rs_idx);
			va = rs + mem_offset;
			if( mmu_v2p(va, &pa) != 0 ) {
				irq_set(IRQ_DTLB_MISS);
				return -1;
			}
			if( store_word(pa, FPR[rd_idx]) != 0 ) {
				return -1;
			}
			PC += 4;
			return 0;

		case OP_ADDI:
			debug_printf("addi r%d, r%d, 0x%08x", rd_idx, rs_idx, immed_sex);
			GPR[rd_idx] = alu_add(rs, immed_sex, 0);
			PC += 4;
			return 0;
		case OP_ANDI:
			debug_printf("andi r%d, r%d, 0x%04x", rd_idx, rs_idx, immed);
			GPR[rd_idx] = alu_logic(rs, immed, alu_op_and);
			PC += 4;
			return 0;
		case OP_ORI:
			debug_printf("ori r%d, r%d, 0x%04x", rd_idx, rs_idx, immed);
			GPR[rd_idx] = alu_logic(rs, immed, alu_op_or);
			PC += 4;
			return 0;
		case OP_XORI:
			debug_printf("xori r%d, r%d, 0x%04x", rd_idx, rs_idx, immed);
			GPR[rd_idx] = alu_logic(rs, immed, alu_op_xor);
			PC += 4;
			return 0;
		case OP_LSLI:
			debug_printf("lsli r%d, r%d, 0x%04x", rd_idx, rs_idx, immed);
			GPR[rd_idx] = alu_shift(rs, alu_op_lsl, (immed&0x1f));
			PC += 4;
			return 0;
		case OP_LSRI:
			debug_printf("lsri r%d, r%d, 0x%04x", rd_idx, rs_idx, immed);
			GPR[rd_idx] = alu_shift(rs, alu_op_lsr, (immed&0x1f));
			PC += 4;
			return 0;
		case OP_ASRI:
			debug_printf("asri r%d, r%d, 0x%04x", rd_idx, rs_idx, immed);
			GPR[rd_idx] = alu_shift(rs, alu_op_asr, (immed&0x1f));
			PC += 4;
			return 0;
		case OP_LDIH:
			debug_printf("ldih r%d, 0x%04x", rd_idx, immed);
			if( rd_idx ) {
				GPR[rd_idx] = immed<<16;
			}
			PC += 4;
			return 0;
		case OP_BRN:
			brn_func = (brn_func_t)((IR>>22)&0xf);
			brn_offset = ( (IR&0x3fffff)<<2);
			if( brn_offset & 0x200000 ) {
				brn_offset |= 0xffc00000;
			}
			// note: we have 16 burnch function, so burnch function no undefined state
			if( debug_enable ) {
				switch( brn_func ) {
					case brn_func_beq:
						debug_printf("beq %08x", brn_offset);
						break;
					case brn_func_bne:
						debug_printf("bne %08x", brn_offset);
						break;
					case brn_func_bcs:
						debug_printf("bcs %08x", brn_offset);
						break;
					case brn_func_bcc:
						debug_printf("bcc %08x", brn_offset);
						break;
					case brn_func_bmi:
						debug_printf("beq %08x", brn_offset);
						break;
					case brn_func_bhs:
						debug_printf("bhs %08x", brn_offset);
						break;
					case brn_func_blo:
						debug_printf("blo %08x", brn_offset);
						break;
					case brn_func_bvs:
						debug_printf("bvs %08x", brn_offset);
						break;
					case brn_func_bvc:
						debug_printf("bvc %08x", brn_offset);
						break;
					case brn_func_bhi:
						debug_printf("bhi %08x", brn_offset);
						break;
					case brn_func_bls:
						debug_printf("bls %08x", brn_offset);
						break;
					case brn_func_bge:
						debug_printf("bge %08x", brn_offset);
						break;
					case brn_func_blt:
						debug_printf("blt %08x", brn_offset);
						break;
					case brn_func_bgt:
						debug_printf("bgt %08x", brn_offset);
						break;
					case brn_func_ble:
						debug_printf("ble %08x", brn_offset);
						break;
					case brn_func_bal:
						debug_printf("bal %08x", brn_offset);
						break;
					case brn_func_undefined:
					default:
						debug_printf("brn ??? %08x", brn_offset);
						break;
				}
			}
			if( cctest(brn_func) ) {
				PC += brn_offset;
			}
			else {
				PC += 4;
			}
			return 0;

		case OP_LDW:
			debug_printf("l.w r%d, 0x%04x(r%d)", rd_idx, mem_offset, rs_idx);
			va = rs + mem_offset;
			if( mmu_v2p(va, &pa) != 0 ) {
				irq_set(IRQ_DTLB_MISS);
				return -1;
			}
			if( load_word(pa, &tmp32) != 0 ) {
				return -1;
			}
			GPR[rd_idx] = tmp32;
			PC += 4;
			return 0;
		case OP_STW:
			debug_printf("s.w 0x%04x(r%d), r%d", mem_offset, rs_idx, rd_idx);
			va = rs + mem_offset;
			if( mmu_v2p(va, &pa) != 0 ) {
				irq_set(IRQ_DTLB_MISS);
				return -1;
			}
			if( store_word(pa, rd) != 0 ) {
				return -1;
			}
			PC += 4;
			return 0;
		case OP_LDB:
			debug_printf("l.b r%d, 0x%04x(r%d)", rd_idx, mem_offset, rs_idx);
			va = rs + mem_offset;
			if( mmu_v2p(va, &pa) != 0 ) {
				irq_set(IRQ_DTLB_MISS);
				return -1;
			}
			if( load_byte(pa, &tmp8) != 0 ) {
				return -1;
			}
			if( tmp8 & 0x80 ) {
				GPR[rd_idx] = 0xffffff00 | tmp8;
			}
			else {
				GPR[rd_idx] = tmp8;
			}
			PC += 4;
			return 0;
		case OP_STB:
			debug_printf("s.b 0x%04x(r%d), r%d", mem_offset, rs_idx, rd_idx);
			va = rs + mem_offset;
			if( mmu_v2p(va, &pa) != 0 ) {
				irq_set(IRQ_DTLB_MISS);
				return -1;
			}
			if( store_byte(pa, rd) != 0 ) {
				return -1;
			}
			PC += 4;
			return 0;
		case OP_JMPR:
			debug_printf("jmpr r%d", rd_idx);
			PC = rd;
			return 0;
		case OP_JALR:
			debug_printf("jalr r%d", rd_idx);
			GPR[LR_REG_IDX] = PC + 4;
			PC = rd;
			return 0;
		case OP_LDH:
			debug_printf("l.h r%d, 0x%04x(r%d)", rd_idx, mem_offset, rs_idx);
			va = rs + mem_offset;
			if( mmu_v2p(va, &pa) != 0 ) {
				irq_set(IRQ_DTLB_MISS);
				return -1;
			}
			if( load_half(pa, &tmp16) != 0 ) {
				return -1;
			}
			if( tmp16 & 0x8000 ) {
				GPR[rd_idx] = 0xffff0000 | tmp16;
			}
			else {
				GPR[rd_idx] = tmp16;
			}
			PC += 4;
			return 0;
		case OP_STH:
			debug_printf("s.h 0x%04x(r%d), r%d", mem_offset, rs_idx, rd_idx);
			va = rs + mem_offset;
			if( mmu_v2p(va, &pa) != 0 ) {
				irq_set(IRQ_DTLB_MISS);
				return -1;
			}
			if( store_half(pa, rd) != 0 ) {
				return -1;
			}
			PC += 4;
			return 0;
		case OP_LDBU:
			debug_printf("l.bu r%d, 0x%04x(r%d)", rd_idx, mem_offset, rs_idx);
			va = rs + mem_offset;
			if( mmu_v2p(va, &pa) != 0 ) {
				irq_set(IRQ_DTLB_MISS);
				return -1;
			}
			if( load_byte(pa, &tmp8) != 0 ) {
				return -1;
			}
			GPR[rd_idx] = tmp8;
			PC += 4;
			return 0;
		case OP_LDHU:
			debug_printf("l.hu r%d, 0x%04x(r%d)", rd_idx, mem_offset, rs_idx);
			va = rs + mem_offset;
			if( mmu_v2p(va, &pa) != 0 ) {
				irq_set(IRQ_DTLB_MISS);
				return -1;
			}
			if( load_half(pa, &tmp16) != 0 ) {
				return -1;
			}
			GPR[rd_idx] = tmp16;
			PC += 4;
			return 0;

		case OP_MISC:
			switch( IR&0xf ) {
				case OP_MISC_RETI:
					debug_printf("reti");
					PC = EPC;
					REG_SR = ESR;
					return 0;
				case OP_MISC_TRAP:
					debug_printf("trap");
					irq_set(IRQ_SOFTWARE);
					PC += 4;
					return 0;
				case OP_MISC_HALT:
					debug_printf("halt");
					halt = true;
					PC += 4;
					return 0;
				case OP_MISC_ENAI:
					debug_printf("enai");
					psw_set_bit( PSW_I, true );
					PC += 4;
					return 0;
				case OP_MISC_DISI:
					debug_printf("disi");
					psw_set_bit( PSW_I, false );
					PC += 4;
					return 0;
				case OP_MISC_MTC:
					debug_printf("mtc c%d, r%d", rd_idx, rs_idx);
					if( rd_idx == CR0_REG_IDX ) {
						REG_CR0 = rs;
						mmu_en = (REG_CR0 & (1<<MMU_EN)) ? true : false;
					}
					else if( rd_idx == PSW_REG_IDX ) {
						REG_SR = rs;
					}
					else if( rd_idx == EPC_REG_IDX ) {
						EPC = rs;
					}
					else if( rd_idx == ESR_REG_IDX ) {
						ESR = rs;
					}
					PC += 4;
					return 0;
				case OP_MISC_MFC:
					debug_printf("mfc r%d, c%d", rd_idx, rs_idx);
					if( rd_idx ) {
						if( rs_idx == CR0_REG_IDX ) {
							GPR[rd_idx] = REG_CR0;
						}
						else if( rs_idx == PSW_REG_IDX ) {
							GPR[rd_idx] = REG_SR;
						}
						else if( rs_idx == EPC_REG_IDX ) {
							GPR[rd_idx] = EPC;
						}
						else if( rs_idx == ESR_REG_IDX ) {
							GPR[rd_idx] = ESR;
						}
						else {
							GPR[rd_idx] = 0;
						}
					}
					PC += 4;
					return 0;
				case OP_MISC_NOT:
					debug_printf("not r%d, r%d", rd_idx, rs_idx);
					GPR[rd_idx]  = alu_logic(rs, -1, alu_op_xor);
					PC += 4;
					return 0;
				case OP_MISC_LEV:
				{
					uint32_t frame_size = (IR>>4)&0xffff;
					if( frame_size & 0x8000 ) {
						frame_size |= 0xffff0000;
					}
					GPR[SP_REG_IDX] += frame_size;
					PC += GPR[LR_REG_IDX];
					return 0;
				}
				case OP_MISC_TLB_MR_W:
				{
					int tlb_nr = (IR>>4)&0x3f;
					debug_printf("tlbmrw %d, r%d", tlb_nr, rs_idx);
					mr_tlb[tlb_nr] = rs;
					PC += 4;
					return 0;
				}
				case OP_MISC_TLB_MR_R:
				{
					int tlb_nr = (IR>>4)&0x3f;
					debug_printf("tlbmrr %d, r%d", tlb_nr, rs_idx);
					if( rs_idx != 0 ) {
						GPR[rs_idx] = mr_tlb[tlb_nr];
					}
					PC += 4;
					return 0;
				}
				case OP_MISC_TLB_TR_W:
				{
					int tlb_nr = (IR>>4)&0x3f;
					debug_printf("tlbtrw %d, r%d", tlb_nr, rs_idx);
					tr_tlb[tlb_nr] = rs;
					PC += 4;
					return 0;
				}
				case OP_MISC_TLB_TR_R:
				{
					int tlb_nr = (IR>>4)&0x3f;
					debug_printf("tlbtrr %d, r%d", tlb_nr, rs_idx);
					if( rs_idx != 0 ) {
						GPR[rs_idx] = tr_tlb[tlb_nr];
					}
					PC += 4;
					return 0;
				}
				default:
					on_undefined_instruction(__FILE__, __LINE__, IR);
					return -1;
			}
			on_undefined_instruction(__FILE__, __LINE__, IR);
			return -1;
		default:
			on_undefined_instruction(__FILE__, __LINE__, IR);
			return -1;
	}
	/* !!! cannot arrive here */
	error(__FILE__, __LINE__);
	halt = true;
	exit(1);
	return -1;
}

void dump(void)
{
	int i;
	bool I = psw_get_bit( PSW_I );
	bool N = psw_get_bit( PSW_N );
	bool Z = psw_get_bit( PSW_Z );
	bool V = psw_get_bit( PSW_V );
	bool C = psw_get_bit( PSW_C );
	printf("\n");
	printf("PC = 0x%08X\n", PC );
	printf("I = %d, N = %d,  Z = %d,  V = %d,	C=%d\n", I, N, Z, V, C );
	for( i=0; i<32; i++ ) {
		printf("R[%d] = 0x%08X\n", i, GPR[i] );
	}
	putchar(10);
	for( i=0; i<32; i++ ) {
		printf("F[%d] = 0x%08X\n", i, FPR[i] );
	}
	putchar(10);
}

int soc_init(void)
{
	display_ram_init();
	return 0;
}

void soc_deinit(void)
{
}

void usage(const char *exec_name)
{
	fprintf(STDERR, "\nusage: %s <-p prog> options\n", exec_name);
	fprintf(STDERR, "\nusage: %s <-bin binary> options\n", exec_name);
	fprintf(STDERR, "options:\n");
	fprintf(STDERR, "[-c]: run circles\n");
	fprintf(STDERR, "[-d]: debug enable\n");
	fprintf(STDERR, "[-D]: dump enable\n");
	fprintf(STDERR, "[-kbd-en]: enable keyboard\n");
	fprintf(STDERR, "[-nic-en]: enable network\n");
	fprintf(STDERR, "[-disk diskfile]: enable disk\n");
	fprintf(STDERR, "[-gui]: enable gui\n");
	fprintf(STDERR, "[-w]: gui width, default 320\n");
	fprintf(STDERR, "[-h]: gui height, default 240\n");
	fprintf(STDERR, "[-mp]: enable memory protect\n");
}

int main(int argc, char *argv[])
{
	FILE *fp_prog = NULL;
	char *rom_filename = NULL;
	char *bin_filename = NULL;
	bool have_prog_file = false;
	bool have_bin_file = false;
	int file_size = 0;
	int cycles = -1;
	int i;
	clock_t sim_start_time;
	clock_t sim_finish_time;
	debug_enable = false;
	dump_enable = false;
	gui_enable = false;
	memory_protect_enable = false;
	rgb565_mode = false;
	kbd_enable = false;
	disk_enable = false;
	nic_enable = false;
	no_fpu = false;
	trace_enable = false;
	trace_call = false;
	gui_pid = -1;
	wall_flag = false;
	run_circles = 0;
	for( i=1; i< argc; ) {
		if( strcmp( argv[i], "-d" ) == 0 ) {
			debug_enable = true;
			i++;
		}
		else if( strcmp( argv[i], "-D" ) == 0 ) {
			dump_enable = true;
			i++;
		}
		else if( strcmp( argv[i], "-gui" ) == 0 ) {
			gui_enable = true;
			i++;
		}
		else if( strcmp( argv[i], "-mp" ) == 0 ) {
			memory_protect_enable = true;
			i++;
		}
		else if( strcmp( argv[i], "-rgb565" ) == 0 ) {
			rgb565_mode = true;
			i++;
		}
		else if( strcmp("-w", argv[i]) == 0 ) {
			if( i == argc - 1 ) {
				fprintf(STDERR, "no gui width specify\n");	
				usage(argv[0]);
				return -1;
			}
			i++;
			lcd_width = atoi(argv[i]);
			i++;
		}
		else if( strcmp("-h", argv[i]) == 0 ) {
			if( i == argc - 1 ) {
				fprintf(STDERR, "no gui height specify\n");
				usage(argv[0]);
				return -1;
			}
			i++;
			lcd_height = atoi(argv[i]);
			i++;
		}
		else if( strcmp( argv[i], "-kbd-en" ) == 0 ) {
			kbd_enable = true;
			i++;
		}
		else if( strcmp( argv[i], "-nic-en" ) == 0 ) {
			nic_enable = true;
			i++;
		}
		else if( strcmp( argv[i], "-no-fpu" ) == 0 ) {
			no_fpu = true;
			i++;
		}
		else if( strcmp("-disk", argv[i]) == 0 ) {
			if( i == argc - 1 ) {
				fprintf(STDERR, "no disk image file specify\n");
				usage(argv[0]);
				return -1;
			}
			disk_enable = true;
			i++;
			disk_image_file_name = argv[i];
			i++;
		}
		else if( strcmp( argv[i], "-trace-en" ) == 0 ) {
			trace_enable = true;
			trace_index = 0;
			trace_cnt = 0;
			memset(trace_buffer, 0, sizeof(trace_buffer));
			i++;
		}
		else if( strcmp( argv[i], "-trace-call" ) == 0 ) {
			trace_call = true;
			i++;
		}
		else if( strcmp( argv[i], "-Wall" ) == 0 ) {
			wall_flag = true;
			i++;
		}
		else if( strcmp( argv[i], "-p" ) == 0 ) {
			if( i == argc - 1 ) {
				fprintf( STDERR, "no program file specify\n");
				usage(argv[0]);
				return -1;
			}
			if( have_prog_file ) {
				fprintf( STDERR, "too many program file\n");
				usage(argv[0]);
				return -1;
			}
			else {
				rom_filename = argv[i+1];
				have_prog_file = true;
				i += 2;
			}
		}
		else if( strcmp( argv[i], "-bin" ) == 0 ) {
			if( i == argc - 1 ) {
				fprintf( STDERR, "no binary file specify\n");
				usage(argv[0]);
				return -1;
			}
			if( have_bin_file ) {
				fprintf( STDERR, "too many binary file\n");
				usage(argv[0]);
				return -1;
			}
			else {
				bin_filename = argv[i+1];
				have_bin_file = true;
				i += 2;
			}
		}
		else if( strcmp( argv[i], "-c" ) == 0 ) {
			if( i == argc - 1 ) {
				fprintf( STDERR, "no run cycles specify\n");
				usage(argv[0]);
				return -1;
			}
			cycles = atoi(argv[i+1]);
			_info("set cycles = %d", cycles);
			i += 2;
		}
		else {
			fprintf(STDERR, "argument error\n");
			usage(argv[0]);
			return -1;
		}
	}

	if( have_prog_file == false && have_bin_file == false ) {
		fprintf( STDERR, "no program or binary file specify\n");
		usage(argv[0]);
		return -1;
	}

	if( soc_init() != 0 ) {
		return -1;
	}

	// reset cpu
	reset();

	if ( have_prog_file ) {
		/* load rom and ram */
		memset( memory, 0, sizeof(memory));
		struct exec header;
		file_size = get_file_size( rom_filename );
		if( file_size < sizeof(struct exec) ) {
			fprintf(STDERR, "file size too small\n");
			return -1;
		}
		if( (fp_prog = fopen( rom_filename, "rb" )) == NULL ) {
			perror( rom_filename );
			return -1;
		}
		fread( &header, sizeof(struct exec), 1, fp_prog );
		if( file_size < (sizeof(struct exec) + header.a_text + header.a_data) )
		{
			fprintf(STDERR,"\e[031mfatal error: ");
			fprintf(STDERR, "file size check error\n");
			fprintf(STDERR, "file size: %d\n", file_size);
			fprintf(STDERR, "exec head size: %ld\n", sizeof(struct exec));
			fprintf(STDERR, "text size: %d\n", header.a_text);
			fprintf(STDERR, "data size: %d\n", header.a_data);
			fprintf(STDERR, "bss size: %d\n", header.a_bss);
			fprintf(STDERR, "head + text + data = %ld\n",\
				(sizeof(struct exec) + header.a_text + header.a_data )); 
			fprintf(STDERR,"\n\e[0m");
			fclose(fp_prog);
			return -1;
		}
		if( header.a_text > sizeof(memory) ) {
			fprintf( STDERR, "rom size too large\n");
			fclose(fp_prog);
			return -1;
		}

		code_segment_load_address = 0;
		code_segment_limit = header.a_text;
		data_segment_load_address = header.a_text;
		data_segment_limit = header.a_data;
		_info("load code segment at 0x%08x, limit = %d",\
			code_segment_load_address, code_segment_limit); 
		_info("load data segment at 0x%08x, limit = %d",\
			data_segment_load_address, data_segment_limit);
		_info("load bss  segment at 0x%08x, limit = %d",\
			data_segment_load_address + data_segment_limit, header.a_bss);

		fread( memory, header.a_text, 1, fp_prog );
		fread( memory+header.a_text, header.a_data, 1, fp_prog );

		#ifdef DUMP_LOAD_DATA
		{
			int i;
			for(i=0;i<header.a_data;i++) {
				printf("%02x%c", memory[header.a_text+i], (i+1)%16?32:10);
			}
		}
		#endif

		fclose(fp_prog);
	}
	else if ( have_bin_file ) {
		memset( memory, 0, sizeof(memory));
		file_size = get_file_size( bin_filename );
		if( file_size <= 0 ) {
			fprintf(STDERR, "bad binary file size\n");
			return -1;
		}
		else if ( file_size > sizeof(memory) ) {
			fprintf(STDERR, "binary file too large, must small then %d\n", sizeof(memory));
			return -1;
		}
		if( (fp_prog = fopen( bin_filename, "rb" )) == NULL ) {
			perror( bin_filename );
			return -1;
		}
		_info("load binary file: %s, size: %d", bin_filename, file_size);
		fread( memory, file_size, 1, fp_prog );
		fclose(fp_prog);
		#ifdef DUMP_LOAD_DATA
		{
			int i;
			for(i=0;i<file_size;i++) {
				printf("%02x%c", memory[header.a_text+i], (i+1)%16?32:10);
			}
		}
		#endif
	}

	rtc0_init();
	tmr0_init();
	irq_ctrl_init();
	mmu_init();
	systick_init();
	random_init();

	if( disk_enable ) {
		if( disk0_init(disk_image_file_name) == 0 ) {
			disk_init_done = true;
		}
		else {
			disk_init_done = false;
			fprintf(STDERR,"\e[031m" "error: disk_initialize failue!!!\n" "\e[0m");
		}
	}
	
	if( nic_enable ) {
		uint8_t ip_addr[4],netmask[4],gateway[4];
		/* ip address */
		ip_addr[0] = 192;
		ip_addr[1] = 168;
		ip_addr[2] = 1;
		ip_addr[3] = 168;
		/* net mask */
		netmask[0] = 255;
		netmask[1] = 255;
		netmask[2] = 255;
		netmask[3] = 0;
		/* gataway */
		gateway[0] = 192;
		gateway[1] = 168;
		gateway[2] = 1;
		gateway[3] = 1;
		nic_init(ip_addr,netmask,gateway);
	}

	kbd_init();
	atexit(kbd_deinit);

	if( gui_enable ) {
		if( (gui_pid = fork()) == 0 ) {
			char buf1[32], buf2[32];
			sprintf(buf1, "%d", lcd_width);
			sprintf(buf2, "%d", lcd_height);
			if( lcd_width > 0 && lcd_height > 0 ) {
				if( rgb565_mode ) {
					execl("/usr/local/bin/sdl_shm", "gui", "-rgb565", "-w", buf1, "-h", buf2, NULL);
				}
				else {
					execl("/usr/local/bin/sdl_shm", "gui", "-w", buf1, "-h", buf2, NULL);
				}
			}
			else {
				if( rgb565_mode ) {
					execl("/usr/local/bin/sdl_shm", "gui", "-rgb565", "-w", "320", "-h", "240", NULL);
				}
				else {
					execl("/usr/local/bin/sdl_shm", "gui", "-w", "320", "-h", "240", NULL);
				}
			}
			_error("create gui process failed");
			return -1;
		}
		else if( gui_pid < 0 ) {
			perror("gui process create failed:");
		}
		else {
			_info("create gui process, pid = %d", gui_pid);
			atexit(gui_exit);
		}
	}
	#ifdef MSG_ENABALE
		msgid = msgget((key_t)MSG_KEY, 0666 | IPC_CREAT);
		msg_enable = true;
		if(msgid == -1)
		{
			msg_enable = false;
			fprintf(STDERR, "%s\n", strerror(errno));
		}
	#endif

	sim_start_time = clock();

	while( halt == false ) {
		step();
		run_circles++;

		if( halt == true ) {
			break;
		}

		if( cycles > 0 ) {
			cycles--;
			if( cycles == 0) {
				cycles = -1;
				halt = true;
				break;
			}
		}

		systick_circle();

		if( kbd_enable ) {
			if( run_circles % 7999 == 0 ) {
				int key;
				if( (key = kbd_readchar()) != CHAR_NONE ) {
					//_info("key done, key value = 0x%08x", key);
					if( key == 0x1b ) {	// ESC key
						halt = true;
						break;
					}
					if( UART0_CR & UART0_RX_IE ) {
						irq_set(IRQ_KEYBORAD);
					}
					UART0_SR |= UART0_RX_DONE;
					UART0_DR = key;
				}
			}
		}

#ifdef MSG_ENABALE
		if( msg_enable ) {
			if( run_circles % 8999 == 0 ) {
				struct msg_st data;
				if(msgrcv(msgid, (void*)&data, MSG_BUFFER_SIZE, msgtype, IPC_NOWAIT) != -1)
				{
					mouse_x = data.buffer[0];
					mouse_y = data.buffer[1];
				}
			}
		}
#endif

		tmr0_cycle();
		if( tmr0_check_irq() ) {
			irq_set(IRQ_TMR0);
		}

		int ie_flag = psw_get_bit( PSW_I );
		int exception_flag = psw_get_bit( PSW_E );
		int irq = irq_assert();
		if( irq >= 0 ) {
			switch( irq ) {
				case IRQ_KEYBORAD: 
				{
					UART0_SR &= ~UART0_RX_DONE;	// clearn rx done bit
					if( ie_flag && !exception_flag ) {
						on_exception(irq, VECTOR_IRQ_KEYBOARD);
					}
					break;
				}
				case IRQ_DISK0:
				{
					if( ie_flag && !exception_flag ) {
						on_exception(irq, VECTOR_IRQ_DISK0);
					}
					break;
				}
				case IRQ_SYSTICK:
				{
					if( ie_flag && !exception_flag ) {
						//_info("systick isr assert");
						on_exception(irq, VECTOR_IRQ_SYSTICK);
					}
					break;
				}
				case IRQ_TMR0:
				{
					if( ie_flag && !exception_flag ) {
						on_exception(irq, VECTOR_IRQ_TMR0);
					}
					break;
				}
				case IRQ_UNDF:
					_info("undefined instrution");
					on_exception(irq, VECTOR_TRAP_UNDF);
					break;
				case IRQ_SOFTWARE:
					on_exception(irq, VECTOR_TRAP_SOFTWAVE);
					break;
				case IRQ_DIV_BY_ZERO:
					_info("division by zero");
					on_exception(irq, VECTOR_DIV_BY_ZERO);
					break;
				case IRQ_BUS_ERROR:
					_info("bus error");
					on_exception(irq, VECTOR_BUS_ERROR);
					break;
				case IRQ_ITLB_MISS:
					_info("itlb miss");
					on_exception(irq, VECTOR_ITBL_MISS);
					break;
				case IRQ_DTLB_MISS:
					_info("dtlb miss");
					on_exception(irq, VECTOR_DTBL_MISS);
					break;
				case IRQ_HARD_FAULT:
					_info("HARD_FAULT");
					on_exception(irq, VECTOR_HARD_FAULT);
					break;
				default:
					irq_clr(irq);
					error( __FILE__, __LINE__ );
			}
		}
	}

	sim_finish_time = clock();
	halt = true;

	if( dump_enable ) {
		dump();
	}
	if( trace_enable ) {
		extern int disassembly(uint32_t IR);
		_info("# dump trace:");
		if( trace_cnt <= N_TRACE ) {
			for(i=0;i<trace_cnt;i++) {
				printf("%2d: 0x%08x\t", i, trace_buffer[0][i]);
				disassembly(trace_buffer[1][i]);
				putchar('\r');
			}
		}
		else
		{
			int i;
			for( i=0; i<N_TRACE; i++) {
				printf("%2d: 0x%08x\t", i, trace_buffer[0][trace_index]);
				disassembly(trace_buffer[1][trace_index]);
				trace_index++;
				trace_index %= N_TRACE;
				putchar('\r');
			}
		}
		printf("\n");
	}

	if( disk_enable ) {
		disk0_uninit();
	}
	kbd_deinit();
	soc_deinit();

	_info("run circles: %ld, times: %ld, second: %d\n", run_circles, (sim_finish_time - sim_start_time), (sim_finish_time - sim_start_time)/CLOCKS_PER_SEC );

	return 0;
}