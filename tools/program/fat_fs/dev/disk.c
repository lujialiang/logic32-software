
#include <stdio.h>

#include "disk.h"

int disk_initialize(void)
{
	return DISK_OK;
}

int disk_readp0(unsigned char *buff, int sec)
{
	int *p;
	int cnt;
	int i;
	cnt = 0;
	p = (int *)ADDR_DISK0_CTR;
	while( (*p & DSK0_RDY_MASK == 0) && cnt < 20 ) {
		cnt++;
	}
	if( cnt >= 20 ) {
		puts("disk0 read timeout1!");
		return DISK_ERR;
	}
	p = (int *)ADDR_DISK0_RSCT;
	*p = sec;
	p = (int *)ADDR_DISK0_CTR;
	*p |= (1<<DISK0_RDP);
	while( (*p & DSK0_RDY_MASK == 0) && cnt < 20 ) {
		cnt++;
	}
	if( cnt >= 20 ) {
		puts("disk0 read timeout2!");
		return DISK_ERR;
	}
	p = (int *)ADDR_DISK0_RIDX;
	*p = 0;
	p = (int *)ADDR_DISK0_DAT;
	for(i=0;i<512;i++) {
		buff[i] = *p;
	}
	return DISK_OK;
}

int disk_writep0(unsigned char *buff, int sec)
{
	int *p;
	int cnt;
	int i;
	cnt = 0;
	p = (int *)ADDR_DISK0_CTR;
	while( (*p & DSK0_RDY_MASK == 0) && cnt < 20 ) {
		cnt++;
	}
	if( cnt >= 20 ) {
		puts("disk0 write timeout1!");
		return DISK_ERR;
	}
	p = (int *)ADDR_DISK0_WSCT;
	*p = sec;
	p = (int *)ADDR_DISK0_WIDX;
	*p = 0;
	p = (int *)ADDR_DISK0_DAT;
	for(i=0;i<512;i++) {
		*p = buff[i];
	}
	p = (int *)ADDR_DISK0_CTR;
	*p |= (1<<DISK0_WRP);
	while( (*p & DSK0_RDY_MASK == 0) && cnt < 20 ) {
		cnt++;
	}
	if( cnt >= 20 ) {
		puts("disk0 write timeout2!");
		return DISK_ERR;
	}
	return DISK_OK;
}
