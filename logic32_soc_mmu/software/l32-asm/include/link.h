#ifndef _LINK_H_INCLUDE
#define _LINK_H_INCLUDE

#include <stdint.h>
#include "a_out.h"

struct module
{
	struct module *next;
	char *filename;
	char *local_sym_name;
	struct exec header;
	uint8_t *text;
	uint8_t *data;
	int symseg_offset;
	struct nlist *symbols;
	int string_size;
	char *strings;
	struct relocation_info *textrel;
	struct relocation_info *datarel;
	int text_start_address;
	int data_start_address;
	int bss_start_address;
	int local_syms_offset;
};

struct glosym
{
	struct glosym *link;
	char *name;
	long value;
	struct nlist *refs;
	int max_common_size;
	char defined;
	char referenced;
	unsigned char multiply_defined;
	char *filename;
};

extern int link_main( struct module * pm );

#endif // _LINK_H_INCLUDE
