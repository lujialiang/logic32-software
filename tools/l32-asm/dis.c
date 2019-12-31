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

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#include "isa.h"
#include "a_out.h"

static uint32_t get_file_size(const char *file_name) {
	struct stat buf;
	if( stat(file_name,&buf)<0 ) {
		return 0;
	}
	return (uint32_t)buf.st_size;
}

static uint32_t IR;

static int execute(void)
{
	uint8_t rd,rs,rs2;
	uint16_t immed;
	uint32_t offset;
	uint32_t ajmp_offset;
	rd = (IR>>21)&0x1f;
	rs = (IR>>16)&0x1f;
	rs2 = (IR>>11)&0x1f;
	immed = IR&0xffff;
	offset = IR&0xffff;
	ajmp_offset = (IR&0x3fffffff)<<2;
	if( IR & 0x80000000 ) {
		if( (IR & 0xc0000000) == OP_AJMP ) {
			printf("ajmp 0x%08x\n", ajmp_offset);
			return 0;
		}
		printf("jal 0x%08x\n", ajmp_offset);
		return 0;
	}
	switch(IR>>26&0x1f)
	{
		case OP_ALUR:
			switch( IR&0x1f ) {
				case ALUR_ADD:
					printf("add r%d, r%d, r%d\n", rd, rs, rs2);
					return 0;
				case ALUR_ADC:
					printf("adc r%d, r%d, r%d\n", rd, rs, rs2);
					return 0;
				case ALUR_SUB:
					printf("sub r%d, r%d, r%d\n", rd, rs, rs2);
					return 0;
				case ALUR_SBC:
					printf("sbc r%d, r%d, r%d\n", rd, rs, rs2);
					return 0;
				case ALUR_AND:
					printf("and r%d, r%d, r%d\n", rd, rs, rs2);
					return 0;
				case ALUR_ORR:
					printf("orr  r%d, r%d, r%d\n", rd, rs, rs2);
					return 0;
				case ALUR_XOR:
					printf("xor r%d, r%d, r%d\n", rd, rs, rs2);
					return 0;
				case ALUR_LSL:
					printf("lsl r%d, r%d, r%d\n", rd, rs, rs2);
					return 0;
				case ALUR_LSR:
					printf("lsr r%d, r%d, r%d\n", rd, rs, rs2);
					return 0;
				case ALUR_ASR:
					printf("asr r%d, r%d, r%d\n", rd, rs, rs2);
					return 0;
				case ALUR_MUL:
					printf("mul r%d, r%d, r%d\n", rd, rs, rs2);
					return 0;
				case ALUR_DIV:
					printf("div r%d, r%d, r%d\n", rd, rs, rs2);
					return 0;
				case ALUR_MOD:
					printf("mod r%d, r%d, r%d\n", rd, rs, rs2);
					return 0;
				case ALUR_MULU:
					printf("mulu r%d, r%d, r%d\n", rd, rs, rs2);
					return 0;
				case ALUR_DIVU:
					printf("divu r%d, r%d, r%d\n", rd, rs, rs2);
					return 0;
				case ALUR_MODU:
					printf("modu r%d, r%d, r%d\n", rd, rs, rs2);
					return 0;
				default:
					printf("????: %08x\n",IR);
					return -1;
			}
			printf("????: %08x\n",IR);
			return -1;
		case OP_FPU:
			switch( IR&0x1f ) {
				case FPU_ADDS:
					printf("add.s f%d, f%d, f%d\n", rd, rs, rs2); 
					return 0;
				case FPU_SUBS:
					printf("sub.s f%d, f%d, f%d\n", rd, rs, rs2); 
					return 0;
				case FPU_MULS:
					printf("mul.s f%d, f%d, f%d\n", rd, rs, rs2); 
					return 0;
				case FPU_DIVS:
					printf("divs.s f%d, f%d, f%d\n", rd, rs, rs2); 
					return 0;
				case FPU_ADDD:
					printf("add.d f%d, f%d, f%d\n", rd, rs, rs2); 
					return 0;
				case FPU_SUBD:
					printf("sub.d f%d, f%d, f%d\n", rd, rs, rs2); 
					return 0;
				case FPU_MULD:
					printf("mul.d f%d, f%d, f%d\n", rd, rs, rs2); 
					return 0;
				case FPU_DIVD:
					printf("div.d f%d, f%d, f%d\n", rd, rs, rs2); 
					return 0;
				case FPU_MOVS:
					printf("mov.s f%d, f%d\n", rd, rs); 
					return 0;
				case FPU_MOVD:
					printf("mov.d f%d, f%d\n", rd, rs); 
					return 0;
				case FPU_CVDF:
					printf("cvd2f f%d, f%d\n", rd, rs);
					return 0;
				case FPU_CVFD:
					printf("cvf2d f%d, f%d\n", rd, rs);
					return 0;
				case FPU_CVWD:
					printf("cvw2d f%d, f%d\n", rd, rs);
					return 0;
				case FPU_CVDW:
					printf("cvd2w f%d, f%d\n", rd, rs);
					return 0;
				case FPU_NEGD:
					printf("neg.d f%d, f%d\n", rd, rs);
					return 0;
				case FPU_NEGS:
					printf("neg.s f%d, f%d\n", rd, rs);
					return 0;
				default:
					printf("????: %08x\n",IR);
					return -1;
			}
			printf("????: %08x\n",IR);
			return -1;
		case OP_FPU_MISC:
			switch( IR&0xf ) {
				case FPU_MISC_MFC1:
					printf("mfc1 r%d, f%d\n", rd, rs);
					return 0;
				case FPU_MISC_MTC1:
					printf("mtc1 r%d, f%d\n", rd, rs);
					return 0;
				case FPU_MISC_MFC1_D:
					printf("mfc1.d r%d, f%d\n", rd, rs);
					return 0;
				case FPU_MISC_MTC1_D:
					printf("mtc1.d r%d, f%d\n", rd, rs);
					return 0;
				case FPU_MISC_CMP_EQD:
					printf("c.eq.d f%d, f%d\n", rd, rs);
					return 0;
				case FPU_MISC_CMP_LED:
					printf("c.le.d f%d, f%d\n", rd, rs);
					return 0;
				case FPU_MISC_CMP_LTD:
					printf("c.lt.d f%d, f%d\n", rd, rs);
					return 0;
				case FPU_MISC_CMP_EQS:
					printf("c.eq.s f%d, f%d\n", rd, rs);
					return 0;
				case FPU_MISC_CMP_LES:
					printf("c.le.s f%d, f%d\n", rd, rs);
					return 0;
				case FPU_MISC_CMP_LTS:
					printf("c.lt.s f%d, f%d\n", rd, rs);
					return 0;
				case FPU_MISC_BC1F:
				{
					uint32_t brn_offset = ((IR>>4) & 0x3fffff);
					brn_offset <<= 2;
					if( brn_offset & 0x200000 ) {
						brn_offset |= 0xffc00000;
					}
					printf("bc1f 0x%08x\n", brn_offset);
					return 0;
				}
				case FPU_MISC_BC1T:
				{
					uint32_t brn_offset = ((IR>>4) & 0x3fffff);
					brn_offset <<= 2;
					if( brn_offset & 0x200000 ) {
						brn_offset |= 0xffc00000;
					}
					printf("bc1t 0x%08x\n", brn_offset);
					return 0;
				}
				default:
					printf("????: %08x\n",IR);
					return -1;
			}
			printf("????: %08x\n",IR);
			return -1;
		case OP_LDD:
			printf("l.d f%d,0x%04x(r%d)\n",rd,offset,rs);
			return 0;
		case OP_LDF:
			printf("l.f f%d,0x%04x(r%d)\n",rd,offset,rs);
			return 0;
		case OP_STD:
			printf("s.d f%d,0x%04x(r%d)\n",rd,offset,rs);
			return 0;
		case OP_STF:
			printf("s.f f%d,0x%04x(r%d)\n",rd,offset,rs);
			return 0;

		case OP_ADDI:
			printf("addi r%d, r%d, 0x%04x\n", rd, rs, immed);
			return 0;
		case OP_ANDI:
			printf("andi r%d, r%d, 0x%04x\n", rd, rs, immed);
			return 0;
		case OP_ORI:
			printf("ori  r%d, r%d, 0x%04x\n", rd, rs, immed);
			return 0;
		case OP_XORI:
			printf("xori r%d, r%d, 0x%04x\n", rd, rs, immed);
			return 0;
		case OP_LSLI:
			printf("lsli r%d, r%d, %d\n", rd, rs, (IR&0x1f));
			return 0;
		case OP_LSRI:
			printf("lsri r%d, r%d, %d\n", rd, rs, (IR&0x1f));
			return 0;
		case OP_ASRI:
			printf("asri r%d, r%d, %d\n", rd, rs, (IR&0x1f));
			return 0;
		case OP_LDIH:
			printf("ldih r%d, 0x%04x\n", rd, immed);
			return 0;
		case OP_BRN:
		{
			uint32_t brn_offset = (IR & 0x3fffff);
			brn_offset <<= 2;
			if( brn_offset & 0x200000 ) {
				brn_offset |= 0xffc00000;
			}
			switch( (IR>>22)&0x0f ) {
				case brn_func_beq:
					printf("beq 0x%08x\n", brn_offset);
					return 0;
				case brn_func_bne:
					printf("bne 0x%08x\n", brn_offset);
					return 0;
				case brn_func_bcs:
					printf("bcs 0x%08x\n", brn_offset);
					return 0;
				case brn_func_bcc:
					printf("bcc 0x%08x\n", brn_offset);
					return 0;
				case brn_func_bhs:
					printf("bhs 0x%08x\n", brn_offset);
					return 0;
				case brn_func_blo:
					printf("blo 0x%08x\n", brn_offset);
					return 0;
				case brn_func_bvs:
					printf("bvs 0x%08x\n", brn_offset);
					return 0;
				case brn_func_bvc:
					printf("bvc 0x%08x\n", brn_offset);
					return 0;
				case brn_func_bhi:
					printf("bhi 0x%08x\n", brn_offset);
					return 0;
				case brn_func_bls:
					printf("bls 0x%08x\n", brn_offset);
					return 0;
				case brn_func_bge:
					printf("bge 0x%08x\n", brn_offset);
					return 0;
				case brn_func_blt:
					printf("blt 0x%08x\n", brn_offset);
					return 0;
				case brn_func_bgt:
					printf("bgt 0x%08x\n", brn_offset);
					return 0;
				case brn_func_ble:
					printf("ble 0x%08x\n", brn_offset);
					return 0;
				case brn_func_bal:
					printf("brn 0x%08x\n", brn_offset);
					return 0;
				case brn_func_bmi:
					printf("bmi 0x%08x\n", brn_offset);
					return 0;
				case brn_func_undefined:
				default:
					printf("brn ??? 0x%08x\n", brn_offset);
			}
			return -1;
		}
		case OP_LDW:
			printf("l.w r%d,0x%04x(r%d)\n",rd,offset,rs);
			return 0;
		case OP_STW:
			printf("s.w r%d,0x%04x(r%d)\n",rd,offset,rs);
			return 0;
		case OP_LDB:
			printf("l.b r%d,0x%04x(r%d)\n",rd,offset,rs);
			return 0;
		case OP_STB:
			printf("s.b r%d,0x%04x(r%d)\n",rd,offset,rs);
			return 0;
		case OP_JMPR:
			printf("jmp r%d\n",rd);
			return 0;
		case OP_JALR:
			printf("jal r%d\n",rd);
			return 0;
		case OP_LDH:
			printf("l.h r%d,0x%04x(r%d)\n",rd,offset,rs);
			return 0;
		case OP_STH:
			printf("s.h r%d,0x%04x(r%d)\n",rd,offset,rs);
			return 0;
		case OP_LDBU:
			printf("l.bu r%d,0x%04x(r%d)\n",rd,offset,rs);
			return 0;
		case OP_LDHU:
			printf("l.hu r%d,0x%04x(r%d)\n",rd,offset,rs);
			return 0;
		case OP_MISC:
			switch( IR&0xf ) {
				case OP_MISC_RETI:
					printf("reti\n");
					return 0;
				case OP_MISC_TRAP:
					printf("trap\n");
					return 0;
				case OP_MISC_HALT:
					printf("halt\n");
					return 0;
				case OP_MISC_ENAI:
					printf("enai\n");
					return 0;
				case OP_MISC_DISI:
					printf("disi\n");
					return 0;
				case OP_MISC_NOT:
					printf("not r%d, r%d\n", rd, rs);
					return 0;
				case OP_MISC_LEV:
					printf("lev %04x\n",  (IR>>4)&0xffff);
					return 0;
				case OP_MISC_MTC:
					printf("mtc c%d, r%d\n", rd, rs);
					return 0;
				case OP_MISC_MFC:
					printf("mfc r%d, c%d\n", rd, rs);
					return 0;
				case OP_MISC_TLB_MR_W:
					printf("tlbmrw %d, $%d\n", ((IR>>4)&0x3f), rs);
					return 0;
				case OP_MISC_TLB_MR_R:
					printf("tlbmrr %d, $%d\n", ((IR>>4)&0x3f), rs);
					return 0;
				case OP_MISC_TLB_TR_W:
					printf("tlbtrw %d, $%d\n", ((IR>>4)&0x3f), rs);
					return 0;
				case OP_MISC_TLB_TR_R:
					printf("tlbtrr %d, $%d\n", ((IR>>4)&0x3f), rs);
					return 0;
				default:
					printf("????: %08x\n",IR);
					return -1;
			}
		default:
			printf("????: %08x\n",IR);
			return -1;
	}
	printf("????: %08x\n",IR);
	return -1;
}

static int is_a_out_file(char *file_name)
{
	int len = strlen(file_name);
	int suffix_len = strlen(".out");
	char *p;
	if( len <= suffix_len ) {
		return 0;
	}
	else {
		p = file_name + (len - suffix_len);
		if(strcmp(".out", p) != 0) {
			return 0;
		}
		return 1;
	}
}

int main(int argc,char *argv[])
{
	FILE *fp_prog = NULL;
	if( argc != 2 ) {
		fprintf(stderr, "usage: %s xxx.bin\n", argv[0]);
		return -1;
	}
	if( is_a_out_file(argv[1]) == 0 ) {
		fprintf(stderr, "%s not a a.out file\n", argv[1]);
		return -1;
	}
	if( (fp_prog = fopen(argv[1],"rb")) == NULL ) {
		perror(argv[1]);
		return -1;
	}
	struct exec header;
	uint32_t file_size = get_file_size(argv[1]);
	if( file_size < sizeof(struct exec) ) {
		fprintf(stderr,"error:file too small\n");
		fclose(fp_prog);
		return -1;
	}
	fread( &header, sizeof(struct exec), 1, fp_prog );
	if( header.a_text + header.a_data + sizeof(struct exec) < file_size ) {
		fprintf(stderr,"error:file size error\n");
		fclose(fp_prog);
		return -1;
	}
	uint32_t *rom_buffer = (uint32_t *)malloc(header.a_text);
	if( rom_buffer == NULL ) {
		perror("malloc");
		fclose(fp_prog);
		return -1;
	}
	fread( rom_buffer, header.a_text, 1, fp_prog );
	fclose( fp_prog );
	uint32_t n = header.a_text / sizeof(uint32_t);
	int pc;
	printf("n = %d\n", n );
	putchar(10);
	for( pc=0; pc<n; pc++ )
	{
		IR = rom_buffer[pc];
		printf("%08x:    ",pc*4);
		printf("%08x:    ",IR);
		execute();
	}
	putchar(10);
	return 0;
}
