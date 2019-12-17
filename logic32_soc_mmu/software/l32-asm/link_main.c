#include "a_out.h"
#include "link.h"
#include "hash_ex.h"
#include "asm.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef my_msg
	#undef my_msg
	#define my_msg
#endif

int error_cnt = 0;

void error(char*s,...)
{
	int32_t i;
	va_list ap;
	va_start(ap,s);
	fprintf(stderr,"error: ");
	vfprintf(stderr,s,ap);
	fprintf(stderr,"\n");
	va_end(ap);
	error_cnt++;
}

void fatal_error(char*s,...)
{
	int32_t i;
	va_list ap;
	va_start(ap,s);
	fprintf(stderr,"fatal error: ");
	vfprintf(stderr,s,ap);
	fprintf(stderr,"\n");
	va_end(ap);
	error_cnt++;
	exit(1);
}

int link_main( struct module * pm )
{
    int i, j,n;
    if( pm == NULL ) {
        return -1;
    }
	while(pm) {
		//printf("link file '%s'\n", pm->filename);
		/* if j == 0 , solve text relocate information */
		/* if j == 1 , solve data relocate information */
		#define TEXT_RELOC	0
		#define DATA_RELOC	1
		for(j=0; j<2; j++ ) {
			if( j == TEXT_RELOC ) {
				n = (pm->header.a_trsize)/sizeof(struct relocation_info);
			}
			else {
				n = (pm->header.a_drsize)/sizeof(struct relocation_info);
			}
			for( i=0; i<n; i++ ) {
				struct relocation_info *pr;
				uint32_t *p32;
				uint16_t *p16;
				if( j == 0 ) { 
					pr = &(pm->textrel[i]);
					p32 = (uint32_t*)(pm->text + pr->address);
					p16 = (uint16_t*)(pm->text + pr->address);
				}
				else {
					pr = &(pm->datarel[i]);
					p32 = (uint32_t*)(pm->data + pr->address);
					p16 = (uint16_t*)(pm->data + pr->address);
				}
				if( pr->extern_flag ) {
					// global symbols
					struct glosym *pglosym = NULL;
					pglosym = hash_ex_lookup( pm->symbols[pr->index].n_name );
					if( pglosym == NULL ) {
						error("undefined symbol '%s'\n", (pm->symbols[pr->index]).n_name);
					}
					else {
						while( pglosym ) {
							if( pglosym->defined ) {
								switch( pr->length ) {
									case BIT16:
									{
										uint32_t tmp = *p16+(pglosym->value & 0xffff);
										if( tmp & 0xffff0000 ) {
											printf("!!! BIT16 overfllow\n");
										}
										*p16 += pglosym->value & 0xffff;
										break;
									}
									case BIT16H:
									{
										uint32_t tmp = *p16+((pglosym->value>>16) & 0xffff);
										if( tmp & 0xffff0000 ) {
											printf("!!! BIT16H overfllow\n");
										}
										*p16 += (pglosym->value>>16) & 0xffff;
										break;
									}
									case BIT30:
										*p32 = (*p32 & 0xc0000000) | (((uint32_t)pglosym->value)>>2);
										break;
									case BIT32:
										*p32 = pglosym->value;
										break;
									case BIT_16H_16L:
									{
										uint32_t immed_value;
										immed_value = (p16[0]<<16) + p16[2];
										immed_value += (uint32_t)pglosym->value;
										p16[0] = immed_value>>16;
										p16[2] = immed_value;
										break;
									}
									case BIT_16H_NONE_16L:
									{
										uint32_t immed_value;
										immed_value = (p16[0]<<16) + p16[4];
										immed_value += (uint32_t)pglosym->value;
										p16[0] = immed_value>>16;
										p16[4] = immed_value;
										break;
									}
									default:
										fatal_error("(%s, %d): unknow internal error", __FILE__, __LINE__);
										break;
								}
								break;
							}
							pglosym = pglosym->link;
						}
						if( pglosym == NULL ) {
							error("undefined symbol '%s'\n", (pm->symbols[pr->index]).n_name); 
						}
					}
				}
				else {
					// local symbols
					switch( pr->length ) {
						case BIT16:
						{
							switch( pr->index & ~N_EXT) {
								case N_TEXT:
									*p16 = (pm->text_start_address + *p16)&0xffff;
									break;
								case N_DATA:
									*p16 = (pm->data_start_address + *p16)&0xffff;
									break;
								case N_BSS:
									*p16 = (pm->bss_start_address + *p16)&0xffff;
									break;
								default:
									fatal_error("(%s, %d): unknow internal error", __FILE__, __LINE__);
									break;
							}
							break;
						}
						case BIT16H:
						{
							switch( pr->index & ~N_EXT) {
								case N_TEXT:
									*p16 = ((pm->text_start_address + *p16)>>16)&0xffff;
									break;
								case N_DATA:
									*p16 = ((pm->data_start_address + *p16)>>16)&0xffff;
									break;
								case N_BSS:
									*p16 = ((pm->bss_start_address + *p16)>>16)&0xffff;
									break;
								default:
									fatal_error("(%s, %d): internal error", __FILE__, __LINE__);
									break;
							}
							break;
						}
						case BIT_16H_16L:
						{
							uint32_t immed_value;
							immed_value = (p16[0]<<16) + p16[2];
							switch( pr->index & ~N_EXT) {
								case N_TEXT:
									immed_value += pm->text_start_address;
									break;
								case N_DATA:
									immed_value += pm->data_start_address;
									break;
								case N_BSS:
									immed_value += pm->bss_start_address;
									break;
								default:
									fatal_error("(%s, %d): internal error", __FILE__, __LINE__);
									break;
							}
							p16[0] = immed_value>>16;
							p16[2] = immed_value;
							break;
						}
						case BIT_16H_NONE_16L:
						{
							uint32_t immed_value;
							immed_value = (p16[0]<<16) + p16[4];
							switch( pr->index & ~N_EXT) {
								case N_TEXT:
									immed_value += pm->text_start_address;
									break;
								case N_DATA:
									immed_value += pm->data_start_address;
									break;
								case N_BSS:
									immed_value += pm->bss_start_address;
									break;
								default:
									fatal_error("(%s, %d): internal error", __FILE__, __LINE__);
									break;
							}
							p16[0] = immed_value>>16;
							p16[4] = immed_value;
							break;
						}
						case BIT30:
						{
							uint32_t data30b = (*p32 & 0x3fffffff)<<2;
							switch( pr->index & ~N_EXT) {
								case N_TEXT:
									data30b = (pm->text_start_address + data30b)>>2&0x3fffffff;
									*p32 &= 0xc0000000;
									*p32 |= data30b;
									break;
								case N_DATA:
									data30b = (pm->data_start_address + data30b)>>2&0x3fffffff;
									*p32 &= 0xc0000000;
									*p32 |= data30b;
									break;
								case N_BSS:
									data30b = (pm->bss_start_address + data30b)&0x3fffffff;
									*p32 &= 0xc0000000;
									*p32 |= data30b;
									break;
								default:
									fatal_error("(%s, %d): unknow internal error", __FILE__, __LINE__);
									break;
							}
							break;
						}
						case BIT32:
						{
							uint32_t data32b = *p32;
							switch( pr->index & ~N_EXT) {
								case N_TEXT:
									*p32 = pm->text_start_address + data32b;
									break;
								case N_DATA:
									*p32 = pm->data_start_address + data32b;
									break;
								case N_BSS:
									*p32 = pm->bss_start_address + data32b;
									break;
								default:
									fatal_error("(%s, %d): unknow internal error", __FILE__, __LINE__);
									break;
							}
							break;
						}
						default:
							fatal_error("(%s, %d): unknow internal error", __FILE__, __LINE__);
					}
				}
			}
		}
        pm = pm->next;
	}
    return 0;
}
