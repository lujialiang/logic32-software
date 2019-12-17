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
#ifndef _SOC_H_INCLUDE_
#define _SOC_H_INCLUDE_

// memory
#define MEMORY_SIZE			(1024*1024*32)
#define MEMORY_ADDR_MASK	(0x1ffffff)

// irq flags bits
#if 0
#define IRQ_KEYBORAD	0
#define IRQ_DISK0		1
#define IRQ_SYSTICK		2
#define IRQ_TMR0		3
#define IRQ_UNDF		4
#define IRQ_SOFTWARE	5
#define IRQ_DIV_BY_ZERO	6
#define IRQ_BUS_ERROR	7
#define IRQ_ITLB_MISS	8
#define IRQ_DTLB_MISS	9
#define IRQ_HARD_FAULT	10
#define IRQ_NIC_TX		11
#define IRQ_NIC_RX		12
#else
// umi
#define IRQ_HARD_FAULT	0
#define IRQ_BUS_ERROR	1
#define IRQ_ITLB_MISS	2
#define IRQ_DTLB_MISS	3
#define IRQ_DIV_BY_ZERO	4
#define IRQ_UNDF		5
#define IRQ_SOFTWARE	6
// irq
#define IRQ_KEYBORAD	7
#define IRQ_DISK0		8
#define IRQ_SYSTICK		9
#define IRQ_TMR0		10
#define IRQ_NIC_TX		11
#define IRQ_NIC_RX		12
#endif

// uart0
#define ADDR_UART0_DR  0x80000000
#define ADDR_UART0_SR  0x80000004
#define ADDR_UART0_CR  0x80000008
#define UART0_RX_DONE	1
#define UART0_TX_DONE	2
#define UART0_RX_IE		4
#define UART0_TX_IE		8

// dma0
#define ADDR_DMA_CH0_SR 0x81000000
#define ADDR_DMA_CH0_CR 0x81000004
#define ADDR_DMA_CH0_LR 0x81000008

// disk0
#define ADDR_DISK0_START 0x82000000
#define ADDR_DISK0_STOP  0x82000014

#define ADDR_DISK0_CTR	0x82000000
#define ADDR_DISK0_RSCT	0x82000004
#define ADDR_DISK0_WSCT	0x82000008
#define ADDR_DISK0_RIDX	0x8200000c
#define ADDR_DISK0_WIDX	0x82000010
#define ADDR_DISK0_DAT	0x82000014
#define DISK0_EN	0
#define DISK0_IE	1
#define DISK0_IF	2
#define DISK0_RDP	3
#define DISK0_WRP	4
#define DISK0_RDY	5

// systick
#define ADDR_SYSTICK_CTRL	0x83000000
#define ADDR_SYSTICK_RELOAD	0x83000004
#define ADDR_SYSTICK_COUNT	0x83000008

// rtc
#define ADDR_RTC0_START	0x84000000
#define ADDR_RTC0_END	0x84000020

#define ADDR_RTC0_DAT	0x84000000
#define ADDR_RTC0_CTR	0x84000004
#define ADDR_RTC0_YEAR	0x84000008
#define ADDR_RTC0_MON	0x8400000c
#define ADDR_RTC0_DAY	0x84000010
#define ADDR_RTC0_HOUR	0x84000014
#define ADDR_RTC0_MIN	0x84000018
#define ADDR_RTC0_SEC	0x8400001c
#define ADDR_RTC0_WEEK	0x84000020
#define RTC0_EN	0
#define RTC0_IE	1
#define RTC0_IF	2
#define RTC0_LD	3

// timer0
#define ADDR_TMR0_CNT	0X85000000	// timer0 counter regster, 32 bit
#define ADDR_TMR0_RLD	0X85000004	// timer0 reload regster, 32 bit
#define ADDR_TMR0_CTR	0X85000008	// timer0 control regster, 32 bit
#define TMR0_EN	0	// timer0 enable
#define TMR0_IE	1	// timer0 interrupt enable
#define TMR0_DI	2	// timer0 counting direction, 0: down, 1: up
#define TMR0_IF	3	// timer0 interrup flag, when set, read timer0 control regster will clear it

// random number generate
#define ADDR_RANDOM_NUMBER	0x86000000

// mouse postion
#define ADDR_MOUSE_X	0x87000000
#define ADDR_MOUSE_Y	0x87000004

// TLB
#define ADDR_TLB_BASE	0x87000000
#define TLB_LIMIT		(128*4)

// network
#define ADDR_NIC_START 0x88000000
#define ADDR_NIC_STOP  0x88000028

#define ADDR_NIC_CTR	0x88000000
//#define ADDR_NIC_RSCT	0x88000004
//#define ADDR_NIC_WSCT	0x88000008
#define ADDR_NIC_RIDX	0x8800000c
#define ADDR_NIC_WIDX	0x88000010
#define ADDR_NIC_RDAT	0x88000014
#define ADDR_NIC_WDAT	0x88000018
#define ADDR_NIC_RSIZ	0x8800002c
#define ADDR_NIC_WSIZ	0x88000028
#define NIC_EN	0
#define NIC_IE	1
#define NIC_IF	2
#define NIC_RDP	3
#define NIC_WRP	4
#define NIC_RDY	5

// DEBUG
#define	ADDR_DEBUG_PORT0	0x89000000
#define	ADDR_DEBUG_PORT1	0x89000004
#define	ADDR_DEBUG_PORT2	0x89000008
#define	ADDR_DEBUG_PORT3	0x8900000c

#endif
