#ifndef _DISK_H_INCLUDE
#define _DISK_H_INCLUDE

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

#define DSK0_RDY_MASK	0x00000010

#define DISK_OK		0
#define DISK_ERR	-1

int disk_initialize(void);
int disk_readp0(unsigned char *buff, int sec);
int disk_writep0(unsigned char *buff, int sec);

#endif
