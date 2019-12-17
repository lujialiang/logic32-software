
#include <stdio.h>
#include "diskio.h"
#include "pffconf.h"

/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (void)
{
/*
	disk_fp = fopen("floppy.img","rb+");
	if(disk_fp == NULL) {
		return STA_NOINIT;
	} 
*/
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Read partial sector                                                   */
/*-----------------------------------------------------------------------*/
#include "disk.h"
static char disk_buf[1024];

DRESULT disk_readp (
	BYTE *buff,		/* Pointer to the read buffer (NULL:Read bytes are forwarded to the stream) */
	DWORD sector,	/* Sector number (LBA) */
	WORD offset,	/* Byte offset to read from (0..511) */
	WORD count		/* Number of bytes to read (ofs + cnt mus be <= 512) */
)
{
/*
	if( !fseek(disk_fp,sector*512+offset,SEEK_SET) ) {
		if( !fread(buff, count, 1, disk_fp) ) {
			return RES_ERROR;
		}
		return RES_OK;
	}
*/
	if(0) {
		printf("sec:%d offset:%d count:%d\n", sector, offset, count);
	}
	if( disk_readp0( disk_buf, sector) == DISK_OK ) {
		int i;
		for(i=0;i<count;i++) {
			buff[i] = disk_buf[offset+i];
		}

        if(0)
        {
			int i;
			for(i=0; i<count; i++) {
				printf("0x%02x ", buff[i]);
				if( (i+1)%16 == 0 ) { 
					putchar(10); 
				}
			}
			putchar(10); 
		}

		return RES_OK;
	}
	return RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Write partial sector                                                  */
/*-----------------------------------------------------------------------*/
#if _USE_WRITE

int disk_writep0(char *buff, int sec);

DRESULT disk_writep (
	const BYTE *buff,	/* Pointer to the bytes to be written (NULL:Initiate/Finalize sector write) */
	DWORD sc			/* Number of bytes to send, Sector number (LBA) or zero */
)
{
	DRESULT res;
	static UINT wc;
	UINT bc, ws;
	res = RES_ERROR;
#if 1
	if (buff) {		/* Send data bytes */
		printf("### sector:%d, count = %d\n", wc, sc);
		ws = disk_writep0((char*)buff, wc);
		res = ( ws == DISK_OK ) ? RES_OK : RES_ERROR;
	} else {
		if (sc) {	/* Initiate sector write transaction */
			wc = sc;
			res = RES_OK;
		} else {	/* Finalize sector write transaction */
			res = RES_OK;
		}
	}
	return res;
#else
	UINT bc, ws;

	if (buff) {		/* Send data bytes */
		bc = wc<sc?wc:sc;
		ws = fwrite(buff,bc,1,disk_fp);
		if( ws == 1 ) {
			res = RES_OK;
		}
		else {
			res = RES_ERROR;
		}
	} else {
		if (sc) {	/* Initiate sector write transaction */
			if( !fseek(disk_fp,sc*512,SEEK_SET) ) {
				wc = 512;							/* Set byte counter */
				res = RES_OK;
			}
			else {
				res = RES_ERROR;
			}
		} else {	/* Finalize sector write transaction */
			res = RES_OK;
		}
	}
	return res;
#endif
}

#endif
