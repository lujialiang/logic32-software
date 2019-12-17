/*
 * bin2mif/hex converter
 *
 * Copyright (C) 2011 zengjun <zengjun<AT>opencpu.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * USAGE:
 * bin2mif.exe main.bin main.mif 32  (QuartusII memory initialize file)
 * bin2mif.exe main.bin main.hex 8   (VerilogHDL $readmemh(filename,registers) file)
 *
 */

#include <stdio.h>
#include <string.h>

#define LOWER(x) (((x)>='A' && (x)<='Z')?(((x)-'A')+'a'):(x))

typedef enum {
	f_quartus,
	f_ieeeverilog,
	f_invalid
} filetype_t;

int main(int argc,char *argv[])
{
	FILE *fr = NULL;
	FILE *fw = NULL;

	int size = 0;
	int i = 0;
	int ch = 0;
	filetype_t ftype = f_invalid;
	int len = 0;
	int bus_width = 0;

	if (argc == 4) {
		fr = fopen(argv[1],"rb");
		if (fr != NULL) {
			fseek(fr,0,SEEK_END);
			size = ftell(fr);
			fseek(fr,0,SEEK_SET);

			len = strlen(argv[2]);
			if (len > strlen(".mif")) {
				if (argv[2][len-4] == '.' && LOWER(argv[2][len-3]) == 'm' && LOWER(argv[2][len-2]) == 'i' && LOWER(argv[2][len-1]) == 'f') {
					ftype = f_quartus;
				}

				if (argv[2][len-4] == '.' && LOWER(argv[2][len-3]) == 'h' && LOWER(argv[2][len-2]) == 'e' && LOWER(argv[2][len-1]) == 'x') {
					ftype = f_ieeeverilog;
				}
			}

			if (ftype != f_invalid)
				fw = fopen(argv[2],"wb");
			else {
				printf("invalid output file type.\n");
				return -1;
			}

			if (fw == NULL) {
				printf("open %s failed.\n",argv[2]);
				return -2;
			}
		} else {
			printf("open %s failed.\n",argv[1]);
			return -3;
		}

		/* check bus width */
		bus_width = atoi(argv[3]);
		if (ftype == f_quartus) {
			switch (bus_width) {
				case 8:
				case 16:
				case 32:
					/* right,nothing to do */
					break;
				default:
					printf("bus_width = %d invalid,only 8,16 and 32 supported.\n",bus_width);
					return -4;
			}
		} else if (ftype == f_ieeeverilog) {
			switch (bus_width) {
				case 8:
					/* right,nothing to do */
					break;
				default:
					printf("bus_width = %d invalid,only 8 supported.\n",bus_width);
					return -5;
			}
		}

		if (fw != NULL) {
			if (ftype == f_quartus) {
				if (bus_width == 8) {
					fprintf(fw,"%% CREATED BY OPENCPU.COM %%\r\n\r\nDEPTH = %d;\r\nWIDTH = 8;\r\nADDRESS_RADIX = DEC;\r\nDATA_RADIX = HEX;\r\n\r\nCONTENT\r\nBEGIN\r\n",size);

					for (i=0; i<size; i++) {
						fprintf(fw,"\t%6d\t:\t",i);

						fseek(fr,i,SEEK_SET);
						fread(&ch,1,1,fr);
						fprintf(fw,"%02x;\r\n",ch);
					}

					fprintf(fw,"END;\r\n");
				} else if (bus_width == 16) {
					fprintf(fw,"%% CREATED BY OPENCPU.COM %%\r\n\r\nDEPTH = %d;\r\nWIDTH = 16;\r\nADDRESS_RADIX = DEC;\r\nDATA_RADIX = HEX;\r\n\r\nCONTENT\r\nBEGIN\r\n",size >> 1);

					for (i=0; i<size; i+=2) {
						fprintf(fw,"\t%6d\t:\t",i >> 1);

						fseek(fr,i+1,SEEK_SET);
						fread(&ch,1,1,fr);
						fprintf(fw,"%02x",ch);

						fseek(fr,i,SEEK_SET);
						fread(&ch,1+0,1,fr);
						fprintf(fw,"%02x",ch);

						fprintf(fw,";\r\n");
					}

					fprintf(fw,"END;\r\n");
				} else if (bus_width == 32) {
					fprintf(fw,"%% CREATED BY OPENCPU.COM %%\r\n\r\nDEPTH = %d;\r\nWIDTH = 32;\r\nADDRESS_RADIX = DEC;\r\nDATA_RADIX = HEX;\r\n\r\nCONTENT\r\nBEGIN\r\n",size >> 2);

					for (i=0; i<size; i+=4) {
						fprintf(fw,"\t%6d\t:\t",i >> 2);

						fseek(fr,i+3,SEEK_SET);
						fread(&ch,1,1,fr);
						fprintf(fw,"%02x",ch);

						fseek(fr,i+2,SEEK_SET);
						fread(&ch,1,1,fr);
						fprintf(fw,"%02x",ch);

						fseek(fr,i+1,SEEK_SET);
						fread(&ch,1,1,fr);
						fprintf(fw,"%02x",ch);

						fseek(fr,i+0,SEEK_SET);
						fread(&ch,1,1,fr);
						fprintf(fw,"%02x",ch);

						fprintf(fw,";\r\n");
					}

					fprintf(fw,"END;\r\n");
				}
			} else if (ftype == f_ieeeverilog) {
				for (i=0; i<size; i++) {
					fseek(fr,i,SEEK_SET);
					fread(&ch,1,1,fr);

					if (i % 16 == 15) {
						if (i == size -1)
							fprintf(fw,"%02x",ch);
						else
							fprintf(fw,"%02x\r\n",ch);
					} else
						fprintf(fw,"%02x ",ch);
				}
			}
		}

		if (fr != NULL)
			fclose(fr);

		if (fw != NULL)
			fclose(fw);
	} else {
		printf("USAGE:\r\n%s main.bin main.mif 32\t(QuartusII memory initialize file)\r\n%s main.bin main.hex 8 \t(VerilogHDL $readmemh(filename,registers) file)\r\n\r\nCREATED BY OPENCPU.COM\r\n",argv[0],argv[0]);
		return -6;
	}

	return 0;
}
