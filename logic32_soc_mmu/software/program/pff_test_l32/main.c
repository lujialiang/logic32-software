/*----------------------------------------------------------------------*/
/* Petit FatFs sample project for generic uC  (C)ChaN, 2010             */
/*----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "pff.h"


//extern FILE *disk_fp;

void die (		/* Stop with dying message */
	FRESULT rc	/* FatFs return value */
)
{
	printf("Failed with rc=%u.\n", rc);
	exit(1);
}


/*-----------------------------------------------------------------------*/
/* Program Main                                                          */
/*-----------------------------------------------------------------------*/

int main (void)
{
	FATFS fatfs;			/* File system object */
	DIR dir;				/* Directory object */
	FILINFO fno;			/* File information object */
	UINT bw, br, i;
	BYTE buff[64];
	FRESULT rc;

	printf("\nMount a volume.\n");
	rc = pf_mount(&fatfs);
	if (rc) die(rc);

	puts("mount ok!");

#if _USE_WRITE
	printf("\nOpen a file to write (write.txt).\n");
	rc = pf_open("WRITE.TXT");
	if (rc) die(rc);

	printf("\nWrite a text data. (Hello world!)\n");
	//rc = pf_write("1234567890", 10, &bw);
	//rc = pf_write("ABCDEFGHIJ", 10, &bw);
	rc = pf_write("Hello world!\r\n", 14, &bw);
	/*
	for (;;) {
		rc = pf_write("Hello world!\r\n", 14, &bw);
		printf("rc = %d\tbw = %d\n",rc,bw);
		if (rc || !bw) break;
	}
	*/
	if (rc) die(rc);

	printf("\nTerminate the file write process.\n");
	rc = pf_write(0, 0, &bw);
	if (rc) die(rc);
#endif

	printf("\nOpen a test file (write.txt).\n");
	rc = pf_open("WRITE.TXT");
	if (rc) die(rc);

	printf("\nType the file content.\n");
	for (;;) {
		rc = pf_read(buff, sizeof(buff), &br);	/* Read a chunk of file */
		if (rc || !br) break;			/* Error or end of file */
		for (i = 0; i < br; i++)		/* Type the data */
			putchar(buff[i]);
	}
	if (rc) die(rc);

#if _USE_DIR
	printf("\nOpen root directory.\n");
	rc = pf_opendir(&dir, "");
	if (rc) die(rc);

	printf("\nDirectory listing...\n");
	for (;;) {
		rc = pf_readdir(&dir, &fno);	/* Read a directory item */
		if (rc || !fno.fname[0]) break;	/* Error or end of dir */
		if (fno.fattrib & AM_DIR)
			printf("   <dir>  %s\n", fno.fname);
		else
			printf("%8lu  %s\n", fno.fsize, fno.fname);
	}
	if (rc) die(rc);
#endif

	printf("\nTest completed.\n");
	//fclose(disk_fp);
	return 0;
}
